#/* 
#
#  This file is provided under a dual BSD/GPLv2 license.  When using or 
#  redistributing this file, you may do so under either license.
#
#  GPL LICENSE SUMMARY
#
#  Copyright(c) 2008-2012 Intel Corporation. All rights reserved.
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
#  Copyright(c) 2008-2012 Intel Corporation. All rights reserved.
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

#==========================================================================
#INTEL CONFIDENTIAL
#Copyright 2008-2012 Intel Corporation All Rights Reserved.
#
#The source code contained or described herein and all documents related to the source code ("Material") are owned by Intel Corporation or its suppliers or licensors. Title to the Material remains with Intel Corporation or its suppliers and licensors. The Material contains trade secrets and proprietary and confidential information of Intel or its suppliers and licensors. The Material is protected by worldwide copyright and trade secret laws and treaty provisions. No part of the Material may be used, copied, reproduced, modified, published, uploaded, posted, transmitted, distributed, or disclosed in any way without Intel's prior express written permission.
#
#No license under any patent, copyright, trade secret or other intellectual property right is granted to or conferred upon you by disclosure or delivery of the Materials, either expressly, by implication, inducement, estoppel or otherwise. Any license under such intellectual property rights must be express and approved by Intel in writing.
#
# =========================================================================*/

export COMPONENT := pwm
export PWD := $(shell pwd)
ifndef PWM_BASE
export PWM_BASE := $(PWD)
endif


.PHONY: all debug clean test doc install install_dev install_target $(COMPONENT)

all: $(COMPONENT)

include Makefile.inc

SUB_DIRS := driver  

$(COMPONENT):
	@echo ">>>Building  Components $(COMPONENT)"
	@$(foreach SUBDIR, $(SUB_DIRS), $(MAKE) -C $(SUBDIR) -f Makefile && ) exit 0;

debug:
	@echo ">>>Building  Components $(COMPONENT)(debug)"
	@$(foreach SUBDIR, $(SUB_DIRS), $(MAKE) -C $(SUBDIR) -f Makefile CFLAGS+=-g  && ) exit 0;

clean:
	@echo ">>>Cleaninng up  Components $(COMPONENT)"
	@$(foreach SUBDIR, $(SUB_DIRS), $(MAKE) -C $(SUBDIR) -f Makefile clean && ) exit 0;

test:
	@echo ">>>Building test unit for  Components $(COMPONENT)"

doc:
	@echo ">>>Building doc for  Components $(COMPONENT)"
	make -C docs

doc-clean:
	@echo ">>>Building doc for  Components $(COMPONENT)"
	make clean -C docs

install: install_dev install_target
	@echo ">>>Install Components $(COMPONENT) done"

install_dev: all  install_dev_dirs
	@echo ">>>Installing dev for  Components $(COMPONENT)"
	install -c -m 554 $(PWD)/driver/pwm.ko $(BUILD_DEST)/lib/modules/pwm.ko
	install -c -m 554 $(PWD)/lib/libpwm.a  $(BUILD_DEST)/usr/lib/libpwm.a
	install -c -m 554 $(PWD)/init_pwm  $(BUILD_DEST)/etc/init.d/init_pwm
	install -c -m 664 $(PWD)/include/pwm.h $(BUILD_DEST)/usr/include/pwm.h
	install -c -m 664 $(PWD)/include/pwm_abi.h $(BUILD_DEST)/usr/include/pwm_abi.h


install_dev_dirs:
	if [ ! -d $(BUILD_DEST)/lib/modules ] ;  then\
		mkdir -p $(BUILD_DEST)/lib/modules; \
	fi
	if [ ! -d $(BUILD_DEST)/etc/init.d ] ; then\
		mkdir -p $(BUILD_DEST)/etc/init.d; \
	fi

install_target: install_target_dirs
	@echo ">>>Installing target for  Components $(COMPONENT)"
	install -c -m 554 $(PWD)/driver/pwm.ko $(FSROOT)/lib/modules/pwm.ko
	install -c -m 554 $(PWD)/init_pwm  $(FSROOT)/etc/init.d/init_pwm

install_target_dirs:
	if [ ! -d $(FSROOT)/lib/modules ] ;  then\
		mkdir -p $(FSROOT)/lib/modules; \
	fi
	if [ ! -d $(FSROOT)/etc/init.d ] ; then\
		mkdir -p $(FSROOT)/etc/init.d; \
	fi







