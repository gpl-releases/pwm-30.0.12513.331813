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

@mainpage Intel(R) Pulse Width Modulation API


<h1 align="center" style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:18pt">Contents</H1>   

			- \ref introduce "1 Introduction"
				- \ref acronyms_and_terminology "1.1	Acronyms and Terminology"
				- \ref index_1_2 "1.2 PWM module "

			- \ref pwm_package_contents "2	PWM Package Contents"

			- \ref api_overview "3 API Overview"
				- \ref index_3_1 "3.1 Scalar Data Types "
				- \ref index_3_2 "3.2 Compound Data Types "

			- \ref pwm_api_functions "4 PWM API Function"
			- \ref usage_model "5 PWM API Usage Model"
			- \ref pseudo_code "6 Pseudo Code for PWM"					
						
<h1 align="center" style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:18pt">Legal Statement</h1>	
<p><b>
INFORMATION IN THIS DOCUMENT IS PROVIDED IN CONNECTION WITH INTEL PRODUCTS. NO LICENSE, EXPRESS OR IMPLIED, BY ESTOPPEL OR OTHERWISE, TO ANY INTELLECTUAL PROPERTY RIGHTS IS GRANTED BY THIS DOCUMENT. EXCEPT AS PROVIDED IN INTEL'S TERMS AND CONDITIONS OF SALE FOR SUCH PRODUCTS, INTEL ASSUMES NO LIABILITY WHATSOEVER, AND INTEL DISCLAIMS ANY EXPRESS OR IMPLIED WARRANTY, RELATING TO SALE AND/OR USE OF INTEL PRODUCTS INCLUDING LIABILITY OR WARRANTIES RELATING TO FITNESS FOR A PARTICULAR PURPOSE, MERCHANTABILITY, OR INFRINGEMENT OF ANY PATENT, COPYRIGHT OR OTHER INTELLECTUAL PROPERTY RIGHT. 
</b></p>
<p>
Intel products are not intended for use in medical, life saving, life sustaining applications.
<BR/>
Intel may make changes to specifications and product descriptions at any time, without notice.
<BR/>
Designers must not rely on the absence or characteristics of any features or instructions marked "reserved" or "undefined." Intel reserves these for future definition and shall have no responsibility whatsoever for conflicts or incompatibilities arising from future changes to them.
<BR/>
This manual may contain design defects or errors known as errata, which may cause the product to deviate from published specifications. Current characterized errata are available on request.
<BR/>
This manual as well as the software, hardware and/or technology described in it, if it is furnished under a license then it may only be used or copied in accordance with the terms of such license. The information in this document is furnished for informational use only, is subject to change without notice, and should not be construed as a commitment by Intel Corporation. <b>Intel Corporation assumes no responsibility or liability for any errors or inaccuracies that may appear in this document or any software that may be provided in association with this document.</b>
<BR/>
Except as permitted by any such license that may be provided with any Intel product no part of this document may be reproduced, stored in a retrieval system, or transmitted in any form or by any means without the express written consent of Intel Corporation. 
<BR/>
Supply of this Implementation of Dolby technology does not convey a license nor imply a right under any patent, or any other industrial or intellectual property right of Dolby Laboratories, to use this Implementation in any finished end-user or ready-to-use final product.  It is hereby notified that a license for such use is required from Dolby Laboratories.
<BR/>
The Intel(R) Media Processor CE 3100 and Intel(R) Atom&tm; processor CE4100 includes graphics functionality based on the POWERVR SGX535 from Imagination Technologies.
<BR/>
BunnyPeople, Celeron, Celeron Inside, Centrino, Centrino logo, Chips, Core Inside, Dialogic, EtherExpress, ETOX, FlashFile, i386, i486, i960, iCOMP, InstantIP, Intel, Intel logo, Intel386, Intel486, Intel740, Intel(R) Media Processor CE 3100, Intel(R) Atom&tm; processor CE4100, IntelDX2, IntelDX4, IntelSX2, Intel Core, Intel Inside, Intel Inside logo, Intel. Leap ahead., Intel. Leap ahead. logo, Intel NetBurst, Intel NetMerge, Intel NetStructure, Intel SingleDriver, Intel SpeedStep, Intel StrataFlash, Intel Viiv, Intel XScale, IPLink, Itanium, Itanium Inside, MCS, MMX, MMX logo, Optimizer logo, OverDrive, Paragon, PDCharm, Pentium, Pentium II Xeon, Pentium III Xeon, Performance at Your Command, Pentium Inside, skoool, Sound Mark, The Computer Inside., The Journey Inside, VTune, Xeon, Xeon Inside and Xircom are trademarks or registered trademarks of Intel Corporation or its subsidiaries in the United States and other countries.
<BR/>
Contact your local Intel sales office or your distributor to obtain the latest specifications and before placing your product order.
<BR/>
Copies of documents which have an ordering number and are referenced in this document, or other Intel literature may be obtained by calling 1-800-548-4725 or by visiting Intel's website at http://www.intel.com.
<BR/>
*Other names and brands may be claimed as the property of others.
<BR/>
Copyright 2008 - 2011, Intel Corporation
</p>


*/


