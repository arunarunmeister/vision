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

_MODULE     := vx_xyz_ext
include $(PRELUDE)
TARGET      := vx_xyz_ext
TARGETTYPE  := library
CSOURCES    := vx_xyz_ext.c
SHARED_LIBS := openvx
include $(FINALE)

_MODULE     := xyz
include $(PRELUDE)
TARGET      := xyz
TARGETTYPE  := dsmo
DEFFILE     := xyz.def
CSOURCES    := xyz_module.c
SHARED_LIBS := openvx
include $(FINALE)

_MODULE     := vx_example
include $(PRELUDE)
TARGET      := vx_example
TARGETTYPE  := exe
CSOURCES    := vx_graph_factory.c vx_factory_corners.c vx_factory_pipeline.c vx_factory_edge.c 
IDIRS       := $(HOST_ROOT)/$(OPENVX_SRC)/include $(HOST_ROOT)/$(OPENVX_SRC)/extensions/include
STATIC_LIBS := vx_xyz_ext openvx-debug-lib openvx-helper
SHARED_LIBS := openvx
SYS_SHARED_LIBS := $(PLATFORM_LIBS)
include $(FINALE)

_MODULE     := vx_example_code
include $(PRELUDE)
TARGET      := vx_example_code
TARGETTYPE  := library
CSOURCES    := vx_imagepatch.c vx_bufferrange.c vx_delaygraph.c vx_clearlog.c vx_super_res.c vx_matrix_access.c vx_parameters.c
include $(FINALE)

_MODULE	    := openvx-tiling
include $(PRELUDE)
TARGET	    := openvx-tiling
TARGETTYPE  := dsmo
CSOURCES    := vx_tiling_add.c vx_tiling_alpha.c vx_tiling_gaussian.c vx_tiling_box.c vx_tiling_ext.c
SHARED_LIBS := openvx
include $(FINALE)

_MODULE	    := tiling_test
include $(PRELUDE)
TARGET	    := tiling_test
TARGETTYPE  := exe
CSOURCES    := vx_tiling_main.c
STATIC_LIBS := openvx-debug-lib openvx-helper
SHARED_LIBS := openvx
include $(FINALE)
