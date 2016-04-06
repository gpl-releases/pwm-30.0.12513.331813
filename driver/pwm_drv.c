/*
#
#  This file is provided under a dual BSD/GPLv2 license.  When using or
#  redistributing this file, you may do so under either license.
#
#  GPL LICENSE SUMMARY
#
#  Copyright(c) 2010-2012 Intel Corporation. All rights reserved.
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of version 2 of the GNU General Public License as
#  published by the Free Software Foundation.
#
#  This program is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
#  The full GNU General Public License is included in this distribution
#  in the file called LICENSE.GPL.
#
#  Contact Information:
#  intel.com
#  Intel Corporation
#  2200 Mission College Blvd.
#  Santa Clara, CA  95052
#  USA
#  (408) 765-8080
#
#
#  BSD LICENSE
#
#  Copyright(c) 2010-2012 Intel Corporation. All rights reserved.
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in
#      the documentation and/or other materials provided with the
#      distribution.
#    * Neither the name of Intel Corporation nor the names of its
#      contributors may be used to endorse or promote products derived
#      from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#*/
/*------------------------------------------------------------------------------
 * File Name: pwm_drv.c
 *------------------------------------------------------------------------------
 */



#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/wait.h>
#include <linux/list.h>
#include <linux/device.h>
#include <linux/pci.h>
#include <linux/cdev.h>

#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)
    #include <asm/semaphore.h>
#else
    #include <linux/semaphore.h>
#endif

#include "pwm_abi.h"
#include "pwm_core.h"

/* Driver identification */
MODULE_AUTHOR("Intel Corporation, (C) 2006 - 2010 - All Rights Reserved");
MODULE_DESCRIPTION("PWM Device Driver for Linux 2.6");
MODULE_SUPPORTED_DEVICE("Intel Media Processors");
MODULE_LICENSE("Dual BSD/GPL"); /* Inform kernel that driver is not GPL. */

/* Unique name for driver */
#ifndef DEVNAME
#define DEVNAME   "pwm"
#endif


#ifndef MOD_NAME
#define MOD_NAME "pwm.ko"
#endif

#ifndef VER
#define VER   "0.1"
#endif
char *version_string = "#@#" MOD_NAME " " VER;


#define NR_PWM_HOSTS  1
#define MAX_PWM_CHANS  4

const static char devname[] = DEVNAME;

static int pwm_major;


struct pwm_chrdev
{
	struct list_head list;
	struct pwm_channel *pwm;
	uint32_t host_id;
	uint32_t channel;
	pid_t ppid;

};

static LIST_HEAD(chrdev_list);
static DEFINE_SPINLOCK(host_list_lock);
        
/*------------------------------------------------------------------------------
 * pwm_drv_open
 *------------------------------------------------------------------------------
 */

static int pwm_drv_open(struct inode *i, struct file *filp)
{ 
	struct pwm_channel *pwm;
	struct pwm_chrdev *pwm_chr;
	unsigned int host_id;
	unsigned int channel;
	char host_name[8];
	host_id = (iminor(i) & 0xF0) >> 4;
	channel = (iminor(i) & 0x0F);

	sprintf(host_name, "pwm%d", host_id);
	pwm = pwm_request(host_name, channel, NULL);
	if (IS_ERR(pwm)) {
		return PTR_ERR(pwm);
	}	

	pwm_chr  = kzalloc(sizeof(struct pwm_chrdev), GFP_KERNEL);

	if (NULL == pwm_chr) {
			pwm_release(pwm);
			return -ENOMEM;
	}

	pwm_chr->pwm = pwm;
	pwm_chr->host_id = host_id;
	pwm_chr->channel = channel;
	pwm_chr->ppid = current->tgid;
	spin_lock(&host_list_lock);
	list_add_tail(&pwm_chr->list, &chrdev_list);
	spin_unlock(&host_list_lock);	
	filp->private_data = pwm;	
    return 0;
}

/*------------------------------------------------------------------------------
 * pwm_drv_close
 *------------------------------------------------------------------------------
 */
        
static int pwm_drv_close(struct inode *inode, struct file *filp)
{
	struct pwm_channel *pwm;
	struct pwm_chrdev *pwm_chr, *next;
	pwm = filp->private_data;
	spin_lock(&host_list_lock);
	list_for_each_entry_safe(pwm_chr, next, &chrdev_list, list) {
		if (pwm_chr->pwm == pwm)
		{
			list_del(&pwm_chr->list);
			break;
		}
	}	
	spin_unlock(&host_list_lock);	
	pwm_release(pwm);
	kfree(pwm_chr);
	filp->private_data = NULL;
	return 0;
}
        

/*------------------------------------------------------------------------------
 * pwm_ioctl
 *------------------------------------------------------------------------------
 */
static long pwm_unlocked_ioctl(struct file *filp, u_int cmd, u_long arg)
{
	struct pwm_channel *pwm, *to_pwm, *from_pwm;
	struct pwm_chrdev *pwm_chr,*pwm_chrB;
	struct pwm_chan_config config;
	struct pwm_channel_config c;	
	int found = 0;
	int ret = 0;
	pwm = (struct pwm_channel *)filp->private_data;


        /* Execute ioctl request */
	switch (cmd) 
	{
    	case PWM_IOC_SET_CONF:
			if (0 == arg) return -EINVAL;
			
			if (copy_from_user(&config, (void *)arg, sizeof(struct pwm_chan_config)))
				return -EFAULT;
			memset(&c, 0, sizeof(struct pwm_channel_config));
			if (PWM_SET_DUTY & config.flags)
			{  
				c.config_mask |= PWM_CONFIG_DUTY_NS;
				c.duty_ns = config.duty_ns;
			}
			
			if (PWM_SET_DUTY_PERCENT & config.flags)
			{  
				c.config_mask |= PWM_CONFIG_DUTY_PERCENT;
				c.duty_percent = config.duty_percent;
			}
			
			if (PWM_SET_PERIOD & config.flags)
			{  
				c.config_mask |= PWM_CONFIG_PERIOD_NS;
				c.period_ns= config.period_ns;
			}	
			
			if (PWM_SET_POLARITY & config.flags)
			{
				c.config_mask |= PWM_CONFIG_POLARITY;
				c.polarity = config.polarity;
			}		
			ret = pwm_config(pwm, &c);
			break;
			
		case PWM_IOC_GET_CONF:
			if (0 == arg) return -EINVAL;

			config.duty_ns = pwm_get_duty_ns(pwm);
			config.period_ns = pwm_get_period_ns(pwm);
			config.duty_percent = pwm_get_duty_percent(pwm);
			config.polarity = pwm_get_polarity(pwm);
			
			if (copy_to_user((void *)arg, &config, sizeof(struct pwm_chan_config)))
				return -EFAULT;
			break;
			

		case PWM_IOC_ENABLE:
			ret = pwm_start(pwm);
			break;

		case PWM_IOC_DISABLE:
			ret = pwm_stop(pwm);
			break;
						
		case PWM_IOC_SYNC:
			list_for_each_entry(pwm_chr, &chrdev_list, list) 
			{
				if (pwm_chr->pwm == pwm)
				{
					found = 1;
					break;
				}  
			}
			
			BUG_ON(!found);
			
			found = 0;
			spin_lock(&host_list_lock);
			list_for_each_entry(pwm_chrB, &chrdev_list, list) {
				if ((pwm_chrB->host_id == pwm_chr->host_id) && (pwm_chrB->channel== (uint32_t)arg)) { 
					found = 1;
					break;
				}  
			}

			if (!found) {
				ret = -EINVAL;
			}else {
			
					if (pwm_chr->ppid != pwm_chrB->ppid){
						ret = -EPERM;
					}else {
						to_pwm = pwm_chrB->pwm;
						ret = pwm_synchronize(pwm, to_pwm);
					}
			}
			spin_unlock(&host_list_lock);
			break;
			
		case PWM_IOC_UNSYNC:
			list_for_each_entry(pwm_chr, &chrdev_list, list) 
			{
				if (pwm_chr->pwm == pwm)
				{
					found = 1;
					break;
				}  
			}
			
			BUG_ON(!found);
			
			found = 0;
			spin_lock(&host_list_lock);
			list_for_each_entry(pwm_chrB, &chrdev_list, list) {
				if ((pwm_chrB->host_id == pwm_chr->host_id) && (pwm_chrB->channel== (uint32_t)arg)) { 
					found = 1;
					break;
				}  
			}
			if (!found) {
				ret = -EINVAL;
			}else {
			
					if (pwm_chr->ppid != pwm_chrB->ppid){
						ret = -EPERM;
					}else {
						from_pwm = pwm_chrB->pwm;
						ret = pwm_synchronize(pwm, from_pwm);
					}
			}
			spin_unlock(&host_list_lock);
			break;

		default:
			
			break;
		
        }

        return ret;
}

static unsigned int pwm_poll(struct file *filp, struct poll_table_struct *table)
{
	return -ENOSYS;
}


/* Structure to map driver functions to kernel */
struct file_operations pwm_drv_fops = {
        .owner   = THIS_MODULE,
        .unlocked_ioctl   = pwm_unlocked_ioctl, 
        .poll	 = pwm_poll,
        .open    = pwm_drv_open, 
        .release = pwm_drv_close,
};


/*------------------------------------------------------------------------------
 * mdoule init
 *------------------------------------------------------------------------------
 */
int
pwm_drv_init(void)
{
	int ret = 0;
	if ((pwm_major = register_chrdev(0, devname, &pwm_drv_fops)) < 0){
			printk(KERN_ERR "allocate character major number error!\n");
			ret = pwm_major;
			return ret;
	  	
	}
	return 0;
		
}

		
/*------------------------------------------------------------------------------
 * module exit
 *------------------------------------------------------------------------------
 */

void
pwm_drv_exit(void)
{

	unregister_chrdev(pwm_major, devname);
	pwm_major = 0;

}
