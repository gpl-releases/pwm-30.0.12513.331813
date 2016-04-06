/*

 This file is provided under a dual BSD/GPLv2 license.  When using or
 redistributing this file, you may do so under either license.

 GPL LICENSE SUMMARY

 Copyright(c) 2011-2012 Intel Corporation. All rights reserved.

 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License as
 published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 The full GNU General Public License is included in this distribution
 in the file called LICENSE.GPL.

 Contact Information:

 Intel Corporation
 2200 Mission College Blvd.
 Santa Clara, CA  97052

 BSD LICENSE

 Copyright(c) 2011-2012 Intel Corporation. All rights reserved.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/**
@weakgroup pwm_summary PWM Summary

\anchor introduce
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:18pt">1 Introduce</h2>	
<p>
This document is applicable to the Intel(R) Media Processor code name CE5300. 
<BR/>
The document describes the PWM (short for pulse width modulation) driver-programming interface for the Intel(R) CE Media Processors and explains how to control PWM host in the SOC to produce pulse.
<BR/>
It describes Application Programming Interface (API), and illustrates the use of the API with some samples. User mode APIs, and kernel mode APIs which used by other driver share identical APIs.

\anchor acronyms_and_terminology	
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:15pt">1.1 Acronyms and Terminology</h2>	

<table width="70%" style="border: 1px solid #555" border="1" cellspacing="0" cellpadding="3">
<tr style="color:#FFFFFF; background-color:#0860A8;font-weight:bold;" ><td width="50%">Term</td><td>Definition</td></tr>
<tr><td>API</td><td>Application Programming Interface</td></tr>
<tr><td>PWM</td><td>Pulse Width Modulation</td></tr>
<tr><td>NB</td><td>Northbridge</td></tr>
</tr> 
</table>
\anchor index_1_2
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:15pt">1.2 PWM module </h2>	
<p>
The PWM driver is used to control PWM host to produce pulse. The PWM user API is built on top of the driver to provide access to PWM host of SOC. PWM host could produce digital pulse as Figure 1-1.
</p>
<hr>
\image html fig1_1.png "Figure 1-1. PWM pulse"
<BR/>
In the following chapters, the acronym of SPI is referred to the short name of SPI Master if there is no special explanation. 

<BR/>
The module is very simple, it contains four parts, such as lib, include, driver, unit_test.
<hr>
\image html fig1_2.png "Figure 1-2. PWM module layout"
<BR/>
<p>
As you see from Figure 1-2, driver code is located in directory <b>driver</b>, library code is located in directory lib. The common including file which are used by driver and <b>lib</b> are put in directory <b>include</b>. Unit test is put to directory <b>unit_test</b>.
</p>
\anchor pwm_package_contents
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:18pt">2	PWM Package Contents</h2>	
The following tables list the PWM package contents:
\anchor table_2_1
@section Table_2_1  Table 2-1 Drivers
<table width="70%" style="border: 1px solid #555" border="1" cellspacing="0" cellpadding="3">
<tr  style="color:#FFFFFF; background-color:#0860A8;font-weight:bold;" >
<td width="50%"><b>driver</b></td><td><b>names</b></td></tr>
<tr><td>PWM kernel driver</td><td>pwm.ko</td></tr>
</tr> 
</table>
<br/>
\anchor table_2_2
@section Table_2_2 Table 2-2 User Libs
<table width="70%" style="border: 1px solid #555" border="1" cellspacing="0" cellpadding="3">
<tr  style="color:#FFFFFF; background-color:#0860A8;font-weight:bold;" ><td width="50%">User Libs</td><td>names</td></tr>
<tr><td>PWM user lib</td><td>libpwm.a</td></tr>
</tr> 
</table>
<br/>

\anchor table_2_3
@section Table_2_3 Table 2-3 User Head Files
<table width="70%" style="border: 1px solid #555" border="1" cellspacing="0" cellpadding="3">
<tr style="color:#FFFFFF; background-color:#0860A8;font-weight:bold;" ><td width="50%">User Head Files</td><td>names</td></tr>
<tr><td>PWM user head file</td><td>pwm.h (lib)</td></tr>
</tr> 
</table>

\anchor table_2_4
@section Table_2_4 Table 2-4 Common Head Files
<table width="70%" style="border: 1px solid #555" border="1" cellspacing="0" cellpadding="3">
<tr style="color:#FFFFFF; background-color:#0860A8;font-weight:bold;" ><td width="50%">Common Head Files/td><td>names</td></tr>
<tr><td>Common head file</td><td>pwm_abi.h</td></tr>
</tr>
</table>

\anchor api_overview	
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:18pt">3 API Overview</h2>	
In this section, the basic data types and data structures are described.
\anchor index_3_1	
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:15pt">3.1 Scalar Data Types</h2>	
The traditional (unsigned) char, (unsigned) short, (unsigned) int are defined by uint*_t types which are classified by bit width. For example, uint_32 is used to represent unsigned int. The following table describes all data types defined by the PWM API. 
\anchor table_3_1
@section Table_3_1 Table 3-1. PWM Basic Data Types
<table width="70%" style="border: 1px solid #555" border="1" cellspacing="0" cellpadding="3">
<tr style="color:#FFFFFF; background-color:#0860A8;font-weight:bold;" ><td width="50%"><b>C/C++ data type</b></td><td><b>PWM Data Types</b></td></tr>
<tr><td>unsigned char</td><td>Uint8_t</td></tr>
<tr><td>unsigned short</td><td>Uint16_t</td></tr>
<tr><td>unsigned int</td><td>Uint32_t</td></tr>
</tr> 
</table>
\anchor index_3_2
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:15pt">3.2 Compound Data Types</h2>	
Variables of these compound data types are used as inputs to and/or outputs from PWM APIs. In this section, all compound types are described.
(1)	PWM module return data type. (defined in include/pwm.h)
\code
// PORT Definitions:
typedef enum {
	PWM_OK	              	= 0x0, 		/**< 0x0 */ 
	PWM_INVALID_PARAM         , 		/**< 0x1 */
	PWM_NOT_INITIALIZED       , 		/**< 0x2 */
	PWM_ALREADY_INITIALIZED   , 		/**< 0x3 */
	PWM_NO_DEV       	    , 		/**< 0x5 */
	PWM_NOT_IMPLEMENTED 	    , 		/**< 0x6 */
	PWM_DEVICE_BUSY  	    ,		/**< 0x7 */
	PWM_NO_MEM 		    , 		/**< 0x8 */
	PWM_TIMED_OUT   	    , 		/**< 0x9 */
	PWM_FIRST_ERROR  	= PWM_INVALID_PARAM, 
	PWM_LAST_ERROR    	= PWM_TIMED_OUT
} pwm_result_t; 
\endcode
It is used to describe the result type for PWM module.

(2) PWM channel handle. (defined in include/pwm.h)
\code
typedef void * pwm_handle ;
\endcode
 It is a handle for user to control PWM channel.


(3)PWM channel configure info structure. (defined include/pwm_abi.h)
\code
typedef struct pwm_chan_config {
uint32_t  flags;
	uint32_t  duty_ns;
	uint32_t  period_ns;
	uint32_t  duty_percent;
	uint32_t  polarity;
}pwm_chan_config; 
\endcode
<p>It is used to describe PWM channel configure info. (defined include/pwm_abi.h)
<p>Some meaning bits in flags of PWM channel structure:
\code
#define  PWM_SET_DUTY			(1 << 0)
#define  PWM_SET_PERIOD		(1 << 1)
#define  PWM_SET_DUTY_PERCENT		(1 << 2)
#define  PWM_SET_POLARITY   	 	(1 << 3)
\endcode
\anchor pwm_api_functions
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:18pt">4	PWM API Functions</h2>
In this section, API functions are described.
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:15pt">4.1 PWM API</h2>
PWM APIs are shared between kernel driver and user lib. They are identical. It is sufficient to include header file "pwm.h".
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:15pt">4.1.1 PWM Functions</h2>
\ref pwm_ "Functions Link"

\anchor usage_model
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:18pt">5 PWM API Usage Model</h2>
PWM API usage model is same in application layer and kernel driver layer.
5.1.1	PWM API Usage Model 
After the PWM driver is loaded into kernel, the users should perform the following to operate PWM channel on the Intel(R) CE Media Processor:
- Open PWM channel.
- Operate on the PWM channel.
- Close PWM channel.

<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:13pt">5.1.1.1	Open PWM channel </h2>
Call <b>pwm_open</b> to get the PWM channel's handle.(only one user could open the PWM channel). A code example follows:
\code
uint32_t host_id = 0;
uint32_t channel = 1;
pwm_handle pwm;
pwm_result_t ret;
ret = pwm_open(host_id, channel, &pwm); // get the PWM channel's handle
	....
	....
\endcode
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:13pt">5.1.1.2 Operate On PWM channel </h2>
You can use <b>pwm_set_config</b> to configure PWM channel.
<BR/>
Or you can also use <b>pwm_get_config</b> to get configuration of PWM channel.
<BR/>
Or you can also use <b>pwm_enable/pwm_disable</b> to enable or disable PWM channel
<BR/>
At last, you can use <b>pwm_sync/pwm_unsync</b> to sync or unsync two channels.
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:13pt">5.1.1.3 Close library </h2>
When user doesn't want to use the PWM channel, PWM channel needs closing. A code example follows:
\code
pwm_close(pwm); // close the PWM channel. After closing, users should not use the handle pwm.
\endcode
 
\anchor pseudo_code
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:18pt">6	Pseudo Code for PWM</h2>
The following pseudo code will give some basic application examples of PWM. If development is in kernel mode, symbols are imported from PWM drivers. Otherwise, symbols are from PWM library.
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:15pt">6.1 PWM User Application Pseudo Code</h2>
The following pseudo code is for PWM API usages: (Set controller 0, channel 1's duty 1000 ns, period 3000 ns, polarity positive, then enable it, then disable it, lastly read the configuration).
\code
#include <stdlib.h>
#include <stdio.h>
#include "pwm.h"

uint32_t host_id = 0;
uint8_t channel = 1;
uint8_t duty_ns = 1000;
uint32_t period_ns = 3000;
uint32_t percent = 20;
uint32_t polarity = 1;
pwm_handle pwm;
pwm_chan_config c;
pwm_result_t ret;
ret = pwm_open(host_id, channel, &pwm);
....
....
memset(&c, 0, sizeof(pwm_chan_config));
c.flags = (PWM_SET_DUTY | PWM_SET_PERIOD | PWM_SET_POLARITY | PWM_SET_DUTY_PERCENT); 
/* PWM_SET_DUTY_PERCENT takes high priority than PWM_SET_DUTY 
In this case,  duty is set to 3000 * 20/100: 600ns */
c.duty_ns = duty_ns;
c.period_ns  = period_ns;
c.polarity = polarity;
pwm_set_config(pwm, &c);

pwm_enable(pwm);
sleep(10);
pwm_disable(pwm);

memset(&c, 0, sizeof(pwm_chan_config));
pwm_get_config(pwm, &c);
duty_ns = c.duty_ns;
period_ns = c.period_ns;
polarity = c.polarity;
percent = c. period_ns;
printf("PWM configuration:\n"
		"duty---%dns, period---%dns, polarity---%d, percent---%%%d \n"
		duty_ns, period_ns, !!polarity, percent);

pwm_close(pwm);

\endcode

*/
