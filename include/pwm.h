/*
#
#  This file is provided under  BSD license.  When using or
#  redistributing this file, you may do so under either license.
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
 * File Name: pwm.h
 *------------------------------------------------------------------------------
 */

#ifndef __LINUX_PWM_API_H
#define __LINUX_PWM_API_H 

#include "pwm_abi.h"

/** @defgroup pwm_ PWM API  */
/** @ingroup pwm_ */
/*@{*/
typedef void *pwm_handle;
typedef enum {
	PWM_OK	              = 0x0, /**< 0x0 */ 
	PWM_INVALID_PARAM        , /**< 0x1 */
	PWM_NOT_INITIALIZED      , /**< 0x2 */
	PWM_ALREADY_INITIALIZED  , /**< 0x3 */
	PWM_NO_DEV       		 , /**< 0x5 */
	PWM_NOT_IMPLEMENTED 	 , /**< 0x6 */
	PWM_DEVICE_BUSY  		 , /**< 0x7 */
	PWM_NO_MEM 				 , /**< 0x8 */
	PWM_TIMED_OUT   		 , /**< 0x9 */
	PWM_FIRST_ERROR          = PWM_INVALID_PARAM, 
	PWM_LAST_ERROR           = PWM_TIMED_OUT
} pwm_result_t;

/**           
* This function is used to open PWM channel, and get the PWM handle.
* @param host_id -  PWM host ID. (CE5300, CE2600 supports: 0)
* @param channel - PWM channel's index of the selected PWM host.(CE5300 supports: 0, 1, 2,3 and  CE2600 supports 0, 1)
* @param handle - pointer to store the returned PWM handle.
* 
* 
* @retval PWM_OK pwm will point to the returned PWM handle.[Successfully]
* @retval PWM_DEVICE_BUSY PWM channel is opened by other users.[error occurs].
* @retval PWM_NO_DEV PWM channel don't exist.[error occurs].
* @retval PWM_NO_MEM fail to allocate memory.[error occurs].
* @retval PWM_INVALID_PARAM invalid parameter.[error occurs].
*/

pwm_result_t pwm_open(uint32_t host_id, uint32_t channel, pwm_handle *handle);

/**
* This function is used to close the PWM channel.
* @param handle - PWM channel handle.
*
* @retval PWM_OK [Successfully].
* @retval PWM_INVALID_PARAM invalid parameter[error occurs].
*          
*/

pwm_result_t pwm_close(pwm_handle handle);

/** 
* This function is used to configure PWM channel.
* @param handle - PWM channel handle.
* @param c - pointer to PWM channel configure info.<BR/>
* 843750Hz < CLK < 27MHz, 0.37us < step < 1.185us <BR/>
* 0 < duty_ns  <= period_ns < 155 344 593ns <BR/>
* 0 <= duty_percent <= 100 <BR/>
*
* @retval PWM_OK [Successfully].
* @retval PWM_INVALID_PARAM invalid parameter[error occurs].
*
*/
pwm_result_t pwm_set_config(pwm_handle handle, pwm_chan_config *c);

/**
* This function is used to get configure info of PWM channel.
* @param handle - PWM channel handle.
* @param c - pointer to PWM channel configure info.<BR/>
* 
* @retval PWM_OK [Successfully].
* @retval PWM_INVALID_PARAM invalid parameter[error occurs].
*/

pwm_result_t pwm_get_config(pwm_handle handle, pwm_chan_config *c);

/** 
* This function is used to enable PWM channel to produce pulse.
* @param handle - PWM channel handle.
* 
* @retval PWM_OK [Successfully].
* @retval PWM_INVALID_PARAM invalid parameter[error occurs].
*/
pwm_result_t pwm_enable(pwm_handle handle);

/**
* This function is used to disable PWM channel to stop producing pulse.
* @param handle - the other PWM channel handle.
* 
* @retval PWM_OK [Successfully].
* @retval PWM_INVALID_PARAM invalid parameter[error occurs].
*/

pwm_result_t pwm_disable(pwm_handle handle);

/** 
* This function is used to synchronize PWM channel to the other PWM channel.
* @param handle - PWM channel handle.
* @param to_pwm - the other PWM channel handle.
* 
* @retval PWM_OK [Successfully].
* @retval PWM_INVALID_PARAM invalid parameter[error occurs].
* @retval PWM_NOT_IMPLEMENTED the operation is not support [error occurs].
*/
pwm_result_t pwm_sync(pwm_handle handle, pwm_handle to_pwm);

/**
* This function is used to un-synchronize PWM channel from the other PWM channel.
* @param handle - PWM channel handle.
* @param from_pwm - the other PWM channel handle.
* 
* @retval PWM_OK [Successfully].
* @retval PWM_INVALID_PARAM invalid parameter[error occurs].
* @retval PWM_NOT_IMPLEMENTED the operation is not support [error occurs].
*/
pwm_result_t pwm_unsync(pwm_handle handle, pwm_handle from_pwm);

/*@}*/

static inline int is_pwm_err(pwm_result_t ret)
{
	if ((ret >= PWM_FIRST_ERROR) && (ret <= PWM_LAST_ERROR))
		return 1;
	else
		return 0;
}	

#endif
