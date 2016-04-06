/*
 *
 * Driver for Intel PWM controllers
 * Copyright (c) 2010 Bill Gatliff <bgat@billgatliff.com>
 * Copyright (c) 2008 Eric Miao (eric.miao@marvell.com>
 *
 * This program is free software; you may redistribute and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
 
 /*------------------------------------------------------------------------------
 * File Name: _pwm.c
 *------------------------------------------------------------------------------
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/irq.h>

#include <asm/div64.h>

#include "pwm_core.h"
#include "_pwm.h"



int  pwm_drv_init(void);
void  pwm_drv_exit(void);

int  pwm_core_init(void);
void  pwm_core_exit(void);

/* vendor, device, subvendor, subdevice, class, class_mask, driver_data */
static const struct pci_device_id pwm_id_tables[] __devinitdata = {
	{ PCI_DEVICE( 0x8086, 0x089f), .driver_data = (0 << 16)},
	{0},
};
MODULE_DEVICE_TABLE(pci, pwm_id_tables);


struct pwm{
	struct pwm_device	pwm;
	spinlock_t		lock;
	unsigned long   clk[PWM_CHAN_MAX];  /*   XX Hz  */
	struct pwm_regs	*regs;
	uint32_t 	base;
	struct pci_dev *pci_dev;
	int     irq;
	char bus_id[12];
};


static inline void __pwm_write32(const volatile void *addr, u32 value)
{
//	pwm_dbg("write addr: 0x%x, value 0x%x\n",addr, value);
	__raw_writel(value, addr);
} 

static inline u32 __pwm_read32(const volatile void *addr)
{
//	pwm_dbg("read addr:0x%x\n",addr);
	return __raw_readl(addr);
} 

static inline struct pwm *to_pwm(const struct pwm_channel *p)
{
	return container_of(p->pwm, struct pwm, pwm);
}

static inline int __pwm_start(struct pwm_channel *p)
{
	struct pwm *pwm;
	struct pwm_chan  *pchan;
	u32 value;
	int ret = 0;
	
	pwm = to_pwm(p);

	pchan = &(pwm->regs->chan[p->chan]);
	
	value = __pwm_read32( &pchan->pwm_ctrl);
	value |= (PWM_CNT_EN | PWM_PRT_OE);
	__pwm_write32(&pchan->pwm_ctrl, value);

	return ret;
}

static inline int __pwm_stop(struct pwm_channel *p)
{
	struct pwm *pwm;
	struct pwm_chan  *pchan;
	u32 value;
	int ret = 0;

	pwm = to_pwm(p);

	pchan = &(pwm->regs->chan[p->chan]);
	
	value = __pwm_read32( &pchan->pwm_ctrl);
	value &=  (~(PWM_CNT_EN | PWM_PRT_OE));
	__pwm_write32(&pchan->pwm_ctrl, value);

	return ret;

}

static int __config_duty_ticks(struct pwm_channel *p,
				   struct pwm_channel_config *c)
{
	struct pwm *pwm;
	struct pwm_chan  *pchan;
	unsigned old_prescale, duty;
	u32 value;


	pwm = to_pwm(p);
	pchan = &(pwm->regs->chan[p->chan]);


	value = __pwm_read32(&pchan->pwm_ctrl);
	
	old_prescale = value & PWM_PRESCALE_MSK;

	duty = c->duty_ticks / (old_prescale + 1); /*no need to debuct 1, it is different from period register*/
	
	p->duty_ticks = c->duty_ticks;


	if (p->duty_ticks == p->period_ticks)
		__pwm_write32(&pchan->pwm_duty, PWM_FDCYCLE);
	else
		__pwm_write32(&pchan->pwm_duty, (duty & PWM_DUTY_MSK));

	return 0;
}

static int __config_period_ticks(struct pwm_channel *p,
				     struct pwm_channel_config *c)
{
	struct pwm *pwm;
	struct pwm_chan  *pchan;
	unsigned old_prescale, prescale, perval,duty;
	u32 value;
	
	pwm = to_pwm(p);
	pchan = &(pwm->regs->chan[p->chan]);

	prescale = (c->period_ticks - 1) >> PWM_PERVAL_BITS;
	if (prescale > PWM_PRESCALE_MAX)
		return -EINVAL;

	perval = c->period_ticks / (prescale + 1) - 1;
	p->period_ticks = c->period_ticks;

	value = __pwm_read32(&pchan->pwm_ctrl);

	old_prescale = value & PWM_PRESCALE_MSK;
	value = value &(~PWM_PRESCALE_MSK);

	if (old_prescale != prescale)
	{

		value |= prescale;
		__pwm_write32(&pchan->pwm_ctrl, value);
		
		duty = p->duty_ticks / (prescale + 1) - 1; /* prescale change, so need to update duty counter*/
		__pwm_write32(&pchan->pwm_duty, (duty & PWM_DUTY_MSK));
	}
	
	if (p->period_ticks == p->duty_ticks)
		__pwm_write32(&pchan->pwm_duty, PWM_FDCYCLE);
	else
		__pwm_write32(&pchan->pwm_perval, (perval & PWM_PERVAL_MSK));

	return 0;
}


static int __config_polarity(struct pwm_channel *p,
				     struct pwm_channel_config *c)
{
	struct pwm *pwm;
	struct pwm_chan  *pchan;
	u32 value;
	pwm = to_pwm(p);
	pchan = &(pwm->regs->chan[p->chan]);

	value = __pwm_read32(&pchan->pwm_ctrl);
	if (c->polarity)
	{
		value &= (~PWM_PLS_POL);
		
	}	
	else
	{
		value |= PWM_PLS_POL;
	}
	__pwm_write32(&pchan->pwm_ctrl, value);
	p->active_high = c->polarity;

	return 0;
}

static int __pwm_request(struct pwm_channel *p)
{
	struct pwm *pwm;
	pwm = to_pwm(p);
	
	p->tick_hz = pwm->clk[p->chan];

	return 0;
}


static int __pwm_config_nosleep(struct pwm_channel *p,
				  struct pwm_channel_config *c)
{
	unsigned long flags;

	if (!(c->config_mask & (PWM_CONFIG_START
				| PWM_CONFIG_STOP
				| PWM_CONFIG_PERIOD_TICKS
				| PWM_CONFIG_DUTY_TICKS
				| PWM_CONFIG_DUTY_TICKS
				| PWM_CONFIG_POLARITY)))
		return -EINVAL;

	spin_lock_irqsave(&p->lock, flags);
	
	if (c->config_mask & PWM_CONFIG_START)
		__pwm_start(p);
	
	if (c->config_mask & PWM_CONFIG_STOP)
		__pwm_stop(p);

	if (c->config_mask & PWM_CONFIG_PERIOD_TICKS)
		__config_period_ticks(p, c);

	if (c->config_mask & PWM_CONFIG_DUTY_TICKS)
		__config_duty_ticks(p, c);
	
	if (c->config_mask & PWM_CONFIG_POLARITY)
		__config_polarity(p, c);

	spin_unlock_irqrestore(&p->lock, flags);
	return 0;
}

static int __pwm_config(struct pwm_channel *p,
			  struct pwm_channel_config *c)
{
	return pwm_config_nosleep(p, c);
}

static int
__pwm_set_callback(struct pwm_channel *p,
		       pwm_callback_t callback)
{
	struct pwm *pwm = to_pwm(p);
	unsigned long flags;

	spin_lock_irqsave(&pwm->lock, flags);
	p->callback = callback;
	spin_unlock_irqrestore(&pwm->lock, flags);

	return 0;
}
static irqreturn_t pwm_irq(int irq, void *data)
{
	struct pwm *pwm = data;
	struct pwm_channel *p;
	u32 status;
	int i;
	unsigned long flags;
	irqreturn_t ret = IRQ_NONE;
	spin_lock_irqsave(&pwm->lock, flags);

	status = __pwm_read32(&pwm->regs->int_trg_reg);
	__pwm_write32(&pwm->regs->int_trg_reg, (status & (PWM_INT_MSK3 | PWM_INT_MSK2 | \
		  PWM_INT_MSK1 | PWM_INT_MSK0 | PWM_TRG_INT3 | PWM_TRG_INT2 | PWM_TRG_INT1 | PWM_TRG_INT0)));

	for (i=0; i < pwm->pwm.nchan; i++, status >>= 1) 
	{
		p = &pwm->pwm.channels[i];
		if (status & (1 << PWM_TRG_INT0_BIT)) 
		{
			if (p->callback)
				p->callback(p);
			if (p->flags & (1 << FLAG_STOP)) 
			{
				__pwm_stop(p);
				clear_bit(FLAG_STOP, &p->flags);
			}
			complete_all(&p->complete);
			ret = IRQ_HANDLED;
		}
	}

	spin_unlock_irqrestore(&pwm->lock, flags);

	return ret;
}


#ifdef CONFIG_PM
static struct pwm_regs pwm_regs;

int  pwm_save_state(struct pwm *pwm)
{
    struct pwm_regs *p = pwm->regs;
    int i = 0;
    
    for (i=0; i < pwm->pwm.nchan; i++)
    {
        pwm_regs.chan[i].pwm_ctrl = p->chan[i].pwm_ctrl;
        pwm_regs.chan[i].pwm_duty = p->chan[i].pwm_duty;
        pwm_regs.chan[i].pwm_perval = p->chan[i].pwm_perval;
        pwm_regs.chan[i].cntr_hold = p->chan[i].cntr_hold;
        pwm_regs.chan[i].pnum_cntr = p->chan[i].pnum_cntr;
    }
    pwm_regs.dds_rate = p->dds_rate;
    pwm_regs.clk_src = p->clk_src;
    pwm_regs.int_trg_reg = p->int_trg_reg;

}  

static int pwm_device_suspend(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct pwm *pwm = pci_get_drvdata(pdev);
    
    pwm_save_state(pwm);
	return 0;
}

int pwm_restore_state(struct pwm *pwm)
{
    struct pwm_regs *p = pwm->regs;
    int i = 0;

    p->dds_rate = pwm_regs.dds_rate;
    p->clk_src = pwm_regs.clk_src;
    p->int_trg_reg = (pwm_regs.int_trg_reg & 0xf0f0);
    
    for (i=0; i < pwm->pwm.nchan; i++)
    {
        p->chan[i].pwm_duty = pwm_regs.chan[i].pwm_duty;
        p->chan[i].pwm_perval = pwm_regs.chan[i].pwm_perval;
        p->chan[i].cntr_hold = pwm_regs.chan[i].cntr_hold;
        p->chan[i].pnum_cntr = pwm_regs.chan[i].pnum_cntr;
        p->chan[i].pwm_ctrl = pwm_regs.chan[i].pwm_ctrl;
    }

}  
static int pwm_device_resume(struct device *dev)
{

	struct pci_dev *pdev = to_pci_dev(dev);
	struct pwm *pwm = pci_get_drvdata(pdev);

    pwm_restore_state(pwm);
	return 0;
}

static const struct dev_pm_ops pwm_pm_ops = { 
	.suspend    = pwm_device_suspend,
	.resume     = pwm_device_resume,
};
#endif


static int __devinit pwm_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
	struct pwm *pwm = NULL;
	uint32_t mem_iobase = 0;
	unsigned int soc;
	int ret = 0;
	int i = 0;

	ret = pci_enable_device(pdev);
	if(ret)
		return ret;

	mem_iobase = pci_resource_start(pdev, 0);
	
	intelce_get_soc_info(&soc, NULL); 
	pci_request_regions(pdev, "pwm");

	pwm = kzalloc(sizeof(struct pwm), GFP_KERNEL);
	if (!pwm)
	{
		dev_err(&pdev->dev, "failed to allocate memory\n");
		ret = -ENOMEM;
		goto free_mem_region;
	}


	spin_lock_init(&pwm->lock);
	pwm->pwm.dev = &pdev->dev;
	sprintf(pwm->bus_id, "pwm%d", (id->driver_data >> 16) & 0xFFFF);
	pwm->pwm.bus_id = (const char *)(pwm->bus_id);
	pwm->pwm.owner = THIS_MODULE;
	pwm->pwm.request = __pwm_request;
	pwm->pwm.config = __pwm_config;
	pwm->pwm.config_nosleep = __pwm_config_nosleep;
	pwm->pwm.set_callback = __pwm_set_callback;
	pwm->pci_dev = pdev;
	pwm->base = mem_iobase;
	pwm->irq = pdev->irq;
	switch (soc) 
	{
		case CE5300_SOC_DEVICE_ID:
			pwm->pwm.nchan = 4;
			break;
		case CE2600_SOC_DEVICE_ID:
			pwm->pwm.nchan = 2;
		default:
			break;
	
	}
	pwm_dbg("bus_id %s\n", pwm->pwm.bus_id);
	pwm_dbg("pwm nchan is %d\n", pwm->pwm.nchan);
	pwm_dbg("phybase is 0x%x, irq is 0x%x\n", pwm->base, pwm->irq);


	pwm->regs = (struct pwm_regs *)ioremap_nocache(mem_iobase, PCI_PWM_IOMEM_LEN);
	if (!pwm->regs){
		dev_err(&pdev->dev, "error, failed to ioremap() registers\n");
		ret = -ENODEV;
		goto free_mem;
	}

	pwm_dbg("pwm->regs virtural addr  is 0x%x\n",pwm->regs);
	ret = request_irq(pwm->irq, pwm_irq, IRQF_SHARED, pwm->pwm.bus_id, (void *)pwm);
	if (ret)
		goto unmmap_iomem;


	__pwm_write32((const void *)&pwm->regs->clk_src, 0xF);
	for (i=0; i < pwm->pwm.nchan; i++)
	{  
			pwm->clk[i] = 27*1000*1000; //* clk 27MHz by default*/
	} 

	ret =  pwm_register(&pwm->pwm);

	if (ret)
		goto free_irqn;

	pci_set_drvdata(pdev, pwm);
	return 0;

free_irqn:
	free_irq(pwm->irq, pwm);

unmmap_iomem:
	iounmap((void *)pwm->regs);

free_mem:
	kfree(pwm);

free_mem_region:
	pci_set_drvdata(pdev, NULL);
	pci_release_regions(pdev);
	pci_disable_device(pdev);
	return ret;
}

static void __devexit pwm_remove(struct pci_dev *pdev)
{
	struct pwm *pwm = pci_get_drvdata(pdev);


	pwm_unregister(&pwm->pwm);
	free_irq(pwm->irq, pwm);
	iounmap((void *)pwm->regs);
	pci_set_drvdata(pdev, NULL);
	pci_release_regions(pdev);
	pci_disable_device(pdev);
	kfree(pwm);

}

static struct pci_driver pwm_driver = {
	.name = "pwm",
	.probe		= pwm_probe,
	.remove		= __devexit_p(pwm_remove),
	.id_table	= pwm_id_tables,
#ifdef CONFIG_PM
	.driver.pm	= &pwm_pm_ops,
#endif
};

static int __init pwm_init(void)
{
	pwm_core_init();
	pwm_drv_init();
	return pci_register_driver(&pwm_driver);
}

static void __exit pwm_exit(void)
{
	pci_unregister_driver(&pwm_driver);
	pwm_drv_exit();
	pwm_core_exit();
}


module_init(pwm_init);
module_exit(pwm_exit);
