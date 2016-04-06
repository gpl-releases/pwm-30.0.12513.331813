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
 * File Name: pwm_wrapper.c
 *------------------------------------------------------------------------------
 */


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/err.h>

#include "pwm.h"
#include "pwm_core.h"

pwm_result_t pwm_open(uint32_t host_id, uint32_t channel, pwm_handle *handle)
{

	pwm_internal *pwm;
	char host_name[8];
	struct pwm_channel *p;

	if (!handle) return PWM_INVALID_PARAM;

	sprintf(host_name, "pwm%d", host_id & 0x0F); 
	p = pwm_request(host_name, channel & 0x0F, NULL);

	if (IS_ERR(p)){	
		if (-EBUSY == PTR_ERR(p))
			return PWM_DEVICE_BUSY;
		else
			return PWM_NO_DEV;
	}	
	pwm = kzalloc(sizeof(*pwm), GFP_KERNEL);
	if (NULL == pwm) {
		return  PWM_NO_MEM;
	}
	pwm->magic = PWM_MAGIC;
	pwm->data =(void *)p;
	pwm->host_id = (host_id & 0x0F);
	pwm->channel = (channel & 0x0F);
	*handle = (pwm_handle)pwm;
	return PWM_OK;
}

pwm_result_t pwm_close(pwm_handle handle)
{
	pwm_internal *pwm;
	struct pwm_channel *p;
	pwm_result_t ret = PWM_OK;

	pwm = (pwm_internal *)handle;
	
	if (!pwm || (PWM_MAGIC !=pwm->magic) || !pwm->data)
	{
		return  PWM_INVALID_PARAM;
	}
	else 
	{
		p = (struct pwm_channel *)pwm->data;
		pwm_release(p);
		memset(pwm, 0, sizeof(*pwm));
		kfree(pwm);
	}	
	return ret;
}


pwm_result_t pwm_set_config(pwm_handle handle, pwm_chan_config *config)
{
	pwm_internal *pwm;
	struct pwm_channel *p;
	struct pwm_channel_config c;
	pwm_result_t ret = PWM_OK;

	pwm = (pwm_internal *)handle;
	if (!pwm || (PWM_MAGIC !=pwm->magic) || !pwm->data || !config)
	{
		return  PWM_INVALID_PARAM;
	}

	memset(&c, 0, sizeof(struct pwm_channel_config));
	if (PWM_SET_DUTY & config->flags)
	{  
		c.config_mask |= PWM_CONFIG_DUTY_NS;
		c.duty_ns = config->duty_ns;
	}
	
	if (PWM_SET_DUTY_PERCENT & config->flags)
	{  
		c.config_mask |= PWM_CONFIG_DUTY_PERCENT;
		c.duty_percent = config->duty_percent;
	}
	
	if (PWM_SET_PERIOD & config->flags)
	{  
		c.config_mask |= PWM_CONFIG_PERIOD_NS;
		c.period_ns= config->period_ns;
	}	
	
	if (PWM_SET_POLARITY & config->flags)
	{
		c.config_mask = PWM_CONFIG_POLARITY;
		c.polarity = config->polarity;
	}		
	p = (struct pwm_channel *)pwm->data;
	if (pwm_config(p, &c)) ret = PWM_INVALID_PARAM;
	return ret;
}

pwm_result_t pwm_get_config(pwm_handle handle, pwm_chan_config *config)
{
	pwm_internal *pwm;
	struct pwm_channel *p;
	pwm = (pwm_internal *)handle;
	if (!pwm || (PWM_MAGIC !=pwm->magic) || !pwm->data || !config)
	{
		return PWM_INVALID_PARAM;
	}
	p = (struct pwm_channel *)pwm->data;
	config->duty_ns = pwm_get_duty_ns(p);
	config->period_ns = pwm_get_period_ns(p);
	config->duty_percent = pwm_get_duty_percent(p);
	config->polarity = pwm_get_polarity(p);
	return PWM_OK;
}


pwm_result_t pwm_enable(pwm_handle handle)
{
	pwm_internal *pwm;
	struct pwm_channel *p;
	pwm_result_t ret = PWM_OK;
	pwm = (pwm_internal *)handle;	
	if (!pwm || (PWM_MAGIC !=pwm->magic) || !pwm->data)
	{
		return PWM_INVALID_PARAM;
	}
	p = (struct pwm_channel *)pwm->data;
	if (pwm_start(p))	ret = PWM_INVALID_PARAM;
	return ret;
}


pwm_result_t pwm_disable(pwm_handle handle)
{
	pwm_internal *pwm;
	struct pwm_channel *p;
	pwm_result_t ret = PWM_OK;
	pwm = (pwm_internal *)handle;	
	if (!pwm || (PWM_MAGIC !=pwm->magic) || !pwm->data)
	{
		return PWM_INVALID_PARAM;
	}

	p = (struct pwm_channel *)pwm->data;
	if (pwm_stop(p))	ret = PWM_INVALID_PARAM;
	return ret;
}

pwm_result_t pwm_sync(pwm_handle handle, pwm_handle to_handle)
{
	struct pwm_internal *pwm, *to_pwm;
	struct pwm_channel *p, *to_p;	
	pwm_result_t ret = PWM_OK;
	pwm = (pwm_internal *)handle;	
	to_pwm = (pwm_internal *)to_handle;
	if (!pwm || (PWM_MAGIC !=pwm->magic) || !pwm->data ||  \
		 !to_pwm || (PWM_MAGIC != to_pwm->magic) || !to_pwm->data)
	{
		return PWM_INVALID_PARAM;
	}
	
	p = (struct pwm_channel *)pwm->data;
	to_p = (struct pwm_channel *)to_pwm->data;
	if (pwm_synchronize(p, to_p)) {
		ret = PWM_NOT_IMPLEMENTED;
	}	
	return ret;
}


pwm_result_t pwm_unsync(pwm_handle handle, pwm_handle from_handle)
{
	struct pwm_internal *pwm, *from_pwm;

	struct pwm_channel *p, *from_p;
	pwm_result_t ret = PWM_OK;

	pwm = (pwm_internal *)handle;	
	from_pwm = (pwm_internal *)from_handle;
	
	if (!pwm || (PWM_MAGIC !=pwm->magic) || !pwm->data ||  \
		 !from_pwm || (PWM_MAGIC != from_pwm->magic) || !from_pwm->data)
	{
		return PWM_INVALID_PARAM;
	}

	p = (struct pwm_channel *)pwm->data;
	from_p = (struct pwm_channel *)from_pwm->data;
	if (pwm_unsynchronize(p, from_p)) {
		ret = PWM_NOT_IMPLEMENTED;
	}	
	return ret;
}


EXPORT_SYMBOL(pwm_open);
EXPORT_SYMBOL(pwm_close);
EXPORT_SYMBOL(pwm_set_config);
EXPORT_SYMBOL(pwm_get_config);
EXPORT_SYMBOL(pwm_enable);
EXPORT_SYMBOL(pwm_disable);
EXPORT_SYMBOL(pwm_sync);
EXPORT_SYMBOL(pwm_unsync);

