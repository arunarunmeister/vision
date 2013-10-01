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

# Define all but don't do anything with it yet.
.PHONY: all
all::

# Define our pathing and default variable values
HOST_ROOT ?= $(abspath .)
BUILD_FOLDER ?= concerto
CONCERTO_ROOT ?= $(HOST_ROOT)/$(BUILD_FOLDER)
BUILD_OUTPUT ?= out
BUILD_TARGET ?= $(CONCERTO_ROOT)/target.mak
# If no directories were specified, then assume "source"
DIRECTORIES ?= source
ifeq ($(NO_OPTIMIZE),1)
TARGET_BUILD:=debug
else
TARGET_BUILD:=release
endif

include $(CONCERTO_ROOT)/os.mak
include $(CONCERTO_ROOT)/machine.mak
include $(CONCERTO_ROOT)/shell.mak
include $(BUILD_TARGET)
include $(CONCERTO_ROOT)/scm.mak

# Define the prelude and finale files so that SUBMAKEFILEs know what they are
# And if the users go and make -f concerto.mak then it will not work right.
PRELUDE := $(CONCERTO_ROOT)/prelude.mak
FINALE  := $(CONCERTO_ROOT)/finale.mak
SUBMAKEFILE := concerto.mak

# Remove the implicit rules for compiling.
.SUFFIXES:

# Allows the commands to be printed out when invoked
ifeq ($(BUILD_DEBUG),1)
Q:=
else
Q:=@
endif

# Define a macro to make the output target path
MAKE_OUT = $(1)/$(BUILD_OUTPUT)/$(TARGET_OS)/$(TARGET_CPU)/$(TARGET_BUILD)

# Define the output folder for all generated components
TARGET_OUT ?= $(call MAKE_OUT,$(HOST_ROOT))
TARGET_DOC ?= $(HOST_ROOT)/$(BUILD_OUTPUT)/docs

# Find all the Makfiles in the subfolders, these will be pulled in to make
ifeq ($(HOST_OS),Windows_NT)
TARGET_MAKEFILES:=$(foreach d,$(DIRECTORIES),$(shell cd $(d) && cmd.exe /C dir /b /s $(SUBMAKEFILE)))
else
TARGET_MAKEFILES:=$(foreach d,$(DIRECTORIES),$(shell find $(d)/ -name $(SUBMAKEFILE)))
endif

# Create the MODULES list by parsing the makefiles.
MODULES:=
include $(TARGET_MAKEFILES)

ifneq ($(NO_BANNER),1)
$(info HOST_ROOT=$(HOST_ROOT))
$(info HOST_COMPILER=$(HOST_COMPILER))
$(info HOST_OS=$(HOST_OS))
$(info HOST_CPU=$(HOST_CPU))
$(info HOST_ARCH=$(HOST_ARCH))
$(info TARGET_OS=$(TARGET_OS))
$(info TARGET_CPU=$(TARGET_CPU))
$(info TARGET_PLATFORM=$(TARGET_PLATFORM))
$(info TARGET_BUILD=$(TARGET_BUILD))
$(info TARGET_OUT=$(TARGET_OUT))
$(info TARGET_DOC=$(TARGET_DOC))
$(info MODULES=$(MODULES))
ifeq ($(BUILD_DEBUG),1)
$(info TARGET_MAKEFILES=$(TARGET_MAKEFILES))
endif
$(info SCM_ROOT=$(SCM_ROOT))
$(info SCM_VERSION=$(SCM_VERSION))
endif

ifndef NO_TARGETS
.PHONY: all dir depend build install uninstall clean clean_target targets scrub vars test docs clean_docs pdf

depend::

all:: build

build:: dir depend

install:: build

uninstall::

targets::
	$(PRINT) TARGETS=$(MODULES)

scrub::
	$(PRINT) Deleting $(BUILD_OUTPUT)
	-$(Q)$(CLEANDIR) $(call PATH_CONV,$(BUILD_OUTPUT))

vars:: $(foreach mod,$(MODULES),$(mod)_vars)
	$(PRINT) HOST_ROOT=$(HOST_ROOT)
	$(PRINT) HOST_OS=$(HOST_OS)
	$(PRINT) HOST_COMPILER=$(HOST_COMPILER)
	$(PRINT) TARGET_CPU=$(TARGET_CPU)
	$(PRINT) TARGET_BUILD=$(TARGET_BUILD)
	$(PRINT) TARGET_OS=$(TARGET_OS)
	$(PRINT) MAKEFILE_LIST=$(MAKEFILE_LIST)
	$(PRINT) TARGET_MAKEFILES=$(TARGET_MAKEFILES)

test:: $(foreach mod,$(MODULES),$(mod)_test)

todo:
	$(Q)fgrep -Rn @TODO $(HOST_ROOT) --exclude-dir=.git \
									 --exclude-dir=.svn \
									 --exclude-dir=docs \
									 --exclude-dir=$(BUILD_FOLDER) \
									 --exclude-dir=$(BUILD_OUTPUT)

endif
