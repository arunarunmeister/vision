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

ifeq ($(HOST_OS),Windows_NT)
    $(info Windows Processor Architecture $(PROCESSOR_ARCHITECTURE))
    $(info Windows Processor Identification $(word 1, $(PROCESSOR_IDENTIFIER)))
    TYPE=$(word 1, $(PROCESSOR_IDENTIFIER))
    ifeq ($(TYPE),x86)
        HOST_CPU=X86
        HOST_ARCH=32
    else ifeq ($(TYPE),Intel64)
        HOST_CPU=X64
        HOST_ARCH=64
    else
        $(error Unknown Processor Architecture on Windows!)
    endif
else
    HOST_CPU=$(shell uname -m)
    ifeq ($(HOST_CPU),Power Macintosh)
        HOST_CPU=PPC
        HOST_ARCH=32
    else ifeq ($(HOST_CPU),x86_64)
        HOST_CPU=x86_64
        HOST_ARCH=64
    else ifeq ($(HOST_CPU),i686)
        HOST_CPU=X86
        HOST_ARCH=32
    else ifeq ($(HOST_CPU),i586)
        HOST_CPU=X86
        HOST_ARCH=32
    else ifeq ($(HOST_CPU),i486)
        HOST_CPU=X86
        HOST_ARCH=32
    else ifeq ($(HOST_CPU),i386)
        HOST_CPU=X86
        HOST_ARCH=32
    else ifeq ($(HOST_CPU),ARM)
        HOST_CPU=ARM
        HOST_ARCH=32
    else ifeq ($(HOST_CPU),armv7l)
        HOST_CPU=ARM
        HOST_ARCH=32
    endif
endif


