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
# Always produce list using foward slashes
_SDIR := $(call PATH_CONV,$(SDIR)/)
all-type-files-in = $(subst \,/,$(subst $(_SDIR),,$(shell cd $(_SDIR)$(2) && cmd.exe /C dir /b /s $(1))))
else
all-type-files-in = $(subst $(SDIR)/,,$(shell find $(SDIR)/$(2) -name $(1)))
endif
all-type-files = $(call all-type-files-in,$(1),)
all-java-files = $(call all-type-files,*.java)
all-c-files    = $(call all-type-files,*.c)
all-cpp-files  = $(call all-type-files,*.cpp)
all-h-files    = $(call all-type-files,*.h)
all-S-files    = $(call all-type-files,*.S)
all-java-files-in = $(call all-type-files-in,*.java,$(1))
all-c-files-in    = $(call all-type-files-in,*.c,$(1))
all-cpp-files-in  = $(call all-type-files-in,*.cpp,$(1))
all-h-files-in    = $(call all-type-files-in,*.h,$(1))
all-S-files-in    = $(call all-type-files-in,*.S,$(1))
