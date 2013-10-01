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

TARGET_PLATFORM ?= PC

SYSIDIRS := $(HOST_ROOT)/include
SYSLDIRS :=
SYSDEFS  := OPENVX_USE_LIST OPENVX_USE_NODE_MEMORY

ifeq ($(TARGET_PLATFORM),PC)
    TARGET_OS=$(HOST_OS)
    TARGET_CPU?=$(HOST_CPU)
    ifeq ($(TARGET_OS),LINUX)
        INSTALL_LIB := /usr/lib
        INSTALL_BIN := /usr/bin
        INSTALL_INC := /usr/include
        TARGET_NUM_CORES:=$(shell cat /proc/cpuinfo | grep processor | wc -l)
        SYSIDIRS += /usr/include
        SYSLDIRS += /usr/lib
        SYSDEFS+=_XOPEN_SOURCE=700 _BSD_SOURCE=1 _GNU_SOURCE=1
        ifneq (,$(OPENCL_ROOT))
            OCL_LIB ?= OpenCL
            SYSDEFS += OPENVX_USE_OPENCL=1
            SYSIDIRS += $(OPENCL_ROOT)/include $(OPENCL_ROOT)/inc
            ifeq ($(TARGET_CPU),X86)
                SYSLDIRS += $(OPENCL_ROOT)/lib/Linux32
            else ifeq ($(TARGET_CPU),x86_64)
                SYSLDIRS += $(OPENCL_ROOT)/lib/Linux64 $(OPENCL_ROOT)/lib64
            endif
        endif
    else ifeq ($(TARGET_OS),DARWIN)
        INSTALL_LIB := /opt/local/lib
        INSTALL_BIN := /opt/local/bin
        INSTALL_INC := /opt/local/include
        TARGET_NUM_CORES ?= 2
        SYSDEFS += _XOPEN_SOURCE=700 _BSD_SOURCE=1 _GNU_SOURCE=1
        SYSDEFS += VX_USE_OPENCL=1
    else ifeq ($(TARGET_OS),CYGWIN)
        INSTALL_LIB := /usr/lib
        INSTALL_BIN := /usr/bin
        INSTALL_INC := /usr/include
        TARGET_NUM_CORES ?= 2
        SYSDEFS+=_XOPEN_SOURCE=700 _BSD_SOURCE=1 _GNU_SOURCE=1 WINVER=0x501
    else ifeq ($(TARGET_OS),Windows_NT)
        INSTALL_LIB := "${windir}\\system32"
        INSTALL_BIN := "${windir}\\system32"
        INSTALL_INC :=
        TARGET_NUM_CORES := $(NUMBER_OF_PROCESSORS)
        SYSIDIRS+=$(HOST_ROOT)/include/windows
        SYSDEFS+=WIN32_LEAN_AND_MEAN WIN32 _WIN32 _CRT_SECURE_NO_DEPRECATE WINVER=0x0501 _WIN32_WINNT=0x0501
        ifdef OPENCL_ROOT
            OCL_LIB ?= OpenCL
            SYSDEFS += VX_USE_OPENCL=1
            SYSDEFS += CL_USE_DEPRECATED_OPENCL_1_0_APIS=1
            SYSDEFS += CL_USE_DEPRECATED_OPENCL_1_1_APIS=1
            SYSIDIRS += $(OPENCL_ROOT)/inc $(OPENCL_ROOT)/include
            ifeq ($(HOST_CPU),X86)
                SYSLDIRS += $(OPENCL_ROOT)/lib/Win32
            else ifeq ($(HOST_CPU),X64)
                SYSLDIRS += $(OPENCL_ROOT)/lib/x64
            endif
        endif
    endif
endif

SYSDEFS += $(TARGET_OS) $(TARGET_CPU) $(TARGET_PLATFORM) TARGET_NUM_CORES=$(TARGET_NUM_CORES)

ifeq ($(TARGET_OS),LINUX)
    PLATFORM_LIBS := dl pthread rt
else ifeq ($(TARGET_OS),DARWIN)
    PLATFORM_LIBS :=
else ifeq ($(TARGET_OS),Windows_NT)
    PLATFORM_LIBS := Ws2_32 user32
else ifeq ($(TARGET_OS),__QNX__)
    PLATFORM_LIBS := screen socket
else ifeq ($(TARGET_OS),CYGWIN)
    PLATFORM_LIBS := pthread
endif

ifeq ($(TARGET_CPU),X86)
    TARGET_ARCH=32
    ENDIAN=LITTLE
else ifeq ($(TARGET_CPU),X64)
    TARGET_ARCH=64
    ENDIAN=LITTLE
else ifeq ($(TARGET_CPU),x86_64)
    TARGET_ARCH=64
    ENDIAN=LITTLE
else ifeq ($(TARGET_CPU),ARM)
    TARGET_ARCH=32
    ENDIAN=LITTLE
else ifeq ($(TARGET_CPU),i386)
    TARGET_ARCH=32
    ENDIAN=LITTLE
endif

ENDIAN?=LITTLE

TARGET_ARCH?=32

SYSDEFS+=ARCH_$(TARGET_ARCH) 
ifeq ($(filter $(HOST_COMPILER),GCC CLANG CL QCC),)
SYSDEFS+=_$(ENDIAN)_ENDIAN_
endif

ifeq ($(BUILD_DEBUG),1)
$(info SYSDEFS = $(SYSDEFS))
$(info SYSIDIRS = $(SYSIDIRS))
$(info SYSLDIRS = $(SYSLDIRS))
endif

