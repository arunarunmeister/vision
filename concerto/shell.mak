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

ifeq ($(HOST_OS),Windows_NT) # cmd.exe
CLEAN    := cmd.exe /C del /Q
CLEANDIR := cmd.exe /C rmdir /Q /S
COPY     := cmd.exe /C copy /Y /Z /V
PRINT    := @echo
SET_RW   := cmd.exe /C attrib -R
SET_EXEC := cmd.exe /C echo
LINK     := cmd.exe /C junction
TOUCH    := cmd.exe /C type NUL >
INSTALL  := cmd.exe /C copy /Y /Z /V
MKDIR    := cmd.exe /C mkdir
CAT      := cmd.exe /C type
else # Bash variants
CLEAN    := rm -f
CLEANDIR := rm -rf
COPY     := cp -f
PRINT    := @echo
SET_RW   := chmod a+rw
SET_EXEC := chmod a+x
LINK     := ln -s -f
TOUCH    := touch
INSTALL  := install -C -m 755
MKDIR    := mkdir -p
CAT      := cat
endif

