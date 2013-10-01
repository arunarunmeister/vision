# Copyright (c) 2012-2013 The Khronos Group Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and/or associated documentation files (the
# "Materials"), to deal in the Materials without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Materials, and to
# permit persons to whom the Materials are furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Materials.
#
# THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.

ifeq ($(BOARD_USES_OPENVX),1)

OPENVX_TOP := $(call my-dir)
OPENVX_INC := $(OPENVX_TOP)/include
OPENVX_DEFS:= -D_LITTLE_ENDIAN_
OPENVX_SRC := sample
include $(OPENVX_TOP)/$(OPENVX_SRC)/Android.mk
include $(OPENVX_TOP)/examples/Android.mk
include $(OPENVX_TOP)/conformance/Android.mk
include $(OPENVX_TOP)/helper/Android.mk
include $(OPENVX_TOP)/extensions/Android.mk

endif