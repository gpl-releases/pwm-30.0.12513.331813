/*
#
#  This file is provided under  BSD license.  When using or
#  redistributing this file, you may do so under either license.
#
#
#
#  BSD LICENSE
#
#  Copyright(c) 2011-2012 Intel Corporation. All rights reserved.
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
 * File Name: pwm_abi.h
 *------------------------------------------------------------------------------
 */
//! \file
#ifndef  __PWM_ABI_H
#define  __PWM_ABI_H 

#ifdef __KERNEL__
#include <linux/types.h>
#else 
#include <stdint.h>
#endif

#include <linux/ioctl.h>

#define PWM_MAGIC   (('P' << 16) || ('W' << 8) || ('M'))

typedef struct pwm_internal {
	uint32_t magic;
	void *data;
	uint32_t host_id;
	uint32_t channel;
	
}pwm_internal;

typedef struct pwm_chan_config
{
	uint32_t  flags;
	uint32_t  duty_ns;
	uint32_t  period_ns;
	uint32_t  duty_percent;
	uint32_t  polarity;

}pwm_chan_config;

#define  PWM_SET_DUTY				(1 << 0)
#define  PWM_SET_PERIOD				(1 << 1)
#define  PWM_SET_DUTY_PERCENT		(1 << 2)
#define  PWM_SET_POLARITY    		(1 << 3)

#define PWM_IOC_MAGIC    'P'

#define PWM_IOC_SET_CONF		_IOW(PWM_IOC_MAGIC, 1, struct pwm_chan_config)
#define PWM_IOC_GET_CONF		_IOR(PWM_IOC_MAGIC, 2, struct pwm_chan_config)



//#define PWM_IOC_GET_CHAN		_IOW(PWM_IOC_MAGIC, 3, struct pwm_info_user)
//#define PWM_IOC_PUT_CHAN		_IOW(PWM_IOC_MAGIC, 4, struct pwm_info_user)

#define PWM_IOC_ENABLE		_IO(PWM_IOC_MAGIC, 5)
#define PWM_IOC_DISABLE		_IO(PWM_IOC_MAGIC, 6)
#define PWM_IOC_SYNC		_IO(PWM_IOC_MAGIC, 7)
#define PWM_IOC_UNSYNC		_IO(PWM_IOC_MAGIC, 8)


#endif

