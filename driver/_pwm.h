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
 * File Name: _pwm.h
 *------------------------------------------------------------------------------
 */

#ifndef __INTEL_PWM_H
#define __INTEL_PWM_H



#define   PWM_STRT_MDE_BIT			15
#define   PWM_PRT_OE_BIT			14
#define	  PWM_CNT_EN_BIT			13
#define	  PWM_EFF_EDG_BIT			11
#define	  PWM_PLS_POL_BIT			10
#define   PWM_PLS_MSK_BIT			9
#define   PWM_CNT_TRG_RSTRT_BIT			8
#define   PWM_INT_SRC_BIT			6

#define   PWM_STRT_MDE 			(1 << PWM_STRT_MDE_BIT)
#define   PWM_PRT_OE			(1 << PWM_PRT_OE_BIT) 
#define	  PWM_CNT_EN			(1 << PWM_CNT_EN_BIT)

#define	  PWM_RISE_EDG			(1 << PWM_EFF_EDG_BIT)  /* 0 or 1 are same rising edge*/
#define	  PWM_FALL_EDG			(2 << PWM_EFF_EDG_BIT)
#define	  PWM_BOTH_EDG			(3 << PWM_EFF_EDG_BIT)

#define	  PWM_PLS_POL			(1 <<  PWM_PLS_POL_BIT)
#define   PWM_PLS_MSK			(1 <<  PWM_PLS_MSK_BIT)
#define   PWM_CNT_TRG_RSTRT		(1 <<  PWM_CNT_TRG_RSTRT_BIT)
#define   PWM_INT_SRC			(1 <<  PWM_INT_SRC_BIT)




#define   PWM_PRESCALE_BITS			6
#define	  PWM_DUTY_BITS				17
#define	  PWM_PERVAL_BITS			17
#define	  PWM_CNTR_HOLD_BITS		17
#define	  PWM_TRG_CNT_BITS			24
#define	  PWM_CNT_RD_BITS			17
#define	  PWM_PNUM_CNTR_BITS		16
#define   PWM_DDS_RATE_BITS			24

#define   PWM_PRESCALE_MSK			((1 << (PWM_PRESCALE_BITS +1)) - 1)
#define   PWM_PRESCALE_MAX			((1 << (PWM_PRESCALE_BITS +1)) - 1)
#define   PWM_DUTY_MSK				((1 << (PWM_DUTY_BITS + 1)) - 1)
#define	  PWM_PERVAL_MSK			((1 << (PWM_PERVAL_BITS + 1)) - 1)
#define	  PWM_CNTR_HOLD_MSK			((1 << (PWM_CNTR_HOLD_BITS + 1)) - 1)
#define	  PWM_TRG_CNT_MSK			((1 << (PWM_TRG_CNT_BITS + 1)) - 1)
#define	  PWM_CNT_RD_MSK			((1 << (PWM_CNT_RD_BITS + 1)) - 1)
#define   PWM_PNUM_CNTR_MSK			((1 << (PWM_PNUM_CNTR_BITS + 1)) -1)
#define   PWM_DDS_RATE_MSK			((1 << (PWM_DDS_RATE_BITS + 1)) -1)


#define   PWM_CLK_SRC_SEL3_BIT		3
#define   PWM_CLK_SRC_SEL2_BIT		2
#define   PWM_CLK_SRC_SEL1_BIT		1
#define   PWM_CLK_SRC_SEL0_BIT		0

#define  PWM_CLK_SRC_SEL3			(1 << PWM_CLK_SRC_SEL3_BIT)
#define  PWM_CLK_SRC_SEL2			(1 << PWM_CLK_SRC_SEL2_BIT)
#define  PWM_CLK_SRC_SEL1			(1 << PWM_CLK_SRC_SEL1_BIT)
#define  PWM_CLK_SRC_SEL0			(1 << PWM_CLK_SRC_SEL0_BIT)


#define	  PWM_FDCYCLE				(1<<17)


#define	  PWM_INT_MSK3_BIT			15
#define	  PWM_INT_MSK2_BIT			14
#define	  PWM_INT_MSK1_BIT			13
#define	  PWM_INT_MSK0_BIT			12

#define	  PWM_INT_MSK3				(1 << PWM_INT_MSK3_BIT)
#define	  PWM_INT_MSK2				(1 << PWM_INT_MSK2_BIT)
#define	  PWM_INT_MSK1				(1 << PWM_INT_MSK1_BIT)
#define	  PWM_INT_MSK0				(1 << PWM_INT_MSK0_BIT)


#define   PWM_TRG_INT3_BIT			11
#define   PWM_TRG_INT2_BIT			10
#define   PWM_TRG_INT1_BIT			9
#define   PWM_TRG_INT0_BIT			8

#define   PWM_TRG_INT3				(1 << PWM_TRG_INT3_BIT)
#define   PWM_TRG_INT2				(1 << PWM_TRG_INT2_BIT)
#define   PWM_TRG_INT1				(1 << PWM_TRG_INT1_BIT)
#define   PWM_TRG_INT0				(1 << PWM_TRG_INT0_BIT)


#define	  PWM_TRG_SEL3_BIT			7
#define	  PWM_TRG_SEL2_BIT			6
#define	  PWM_TRG_SEL1_BIT			5
#define	  PWM_TRG_SEL0_BIT			4

#define	  PWM_TRG_SEL3				(1 << PWM_TRG_SEL3_BIT)
#define	  PWM_TRG_SEL2				(1 << PWM_TRG_SEL2_BIT)
#define	  PWM_TRG_SEL1				(1 << PWM_TRG_SEL1_BIT)
#define	  PWM_TRG_SEL0				(1 << PWM_TRG_SEL0_BIT)


#define   PWM_SW_TRG3_BIT				3
#define   PWM_SW_TRG2_BIT				2
#define   PWM_SW_TRG1_BIT				1
#define   PWM_SW_TRG0_BIT				0

#define   PWM_SW_TRG3					(1 << PWM_SW_TRG3_BIT)
#define   PWM_SW_TRG2					(1 << PWM_SW_TRG2_BIT)
#define   PWM_SW_TRG1					(1 << PWM_SW_TRG1_BIT)
#define   PWM_SW_TRG0					(1 << PWM_SW_TRG0_BIT)


#define PWM_CHAN_MAX			4

typedef struct pwm_chan
{
  volatile uint32_t pwm_ctrl;
  volatile uint32_t pwm_duty;
  volatile uint32_t pwm_perval;
  volatile uint32_t cntr_hold;
  volatile uint32_t trg_cnt;
  volatile uint32_t trg_trg_cnt;
  volatile uint32_t main_cnt;
  volatile uint32_t pnum_cntr; 


}*ppwm_chan;

typedef struct pwm_regs
{
  struct pwm_chan chan[PWM_CHAN_MAX];
  volatile uint32_t dds_rate;
  volatile uint32_t clk_src;
  volatile uint32_t int_trg_reg; /* share register */
}*ppwm_regs;	

#define   PCI_PWM_IOMEM_LEN			(256)
#endif
