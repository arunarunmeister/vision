/*
 * Copyright (c) 2012-2013 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 */

#include <vx.hpp>

using namespace OpenVX;

Kernel::Kernel(const char *name) : Reference(vxGetKernelByName(Context::getContext().handle(), (vx_char *)name))
{
    strncpy(kernel_name, name, VX_MAX_KERNEL_NAME);
    vxQueryKernel(handle(), VX_KERNEL_ATTRIBUTE_ENUM, &kernel_enum, sizeof(kernel_enum));
}

Kernel::Kernel(vx_enum kenum) : Reference(vxGetKernelByEnum(Context::getContext().handle(), kenum))
{
    kernel_enum = kenum;
    vxQueryKernel(handle(), VX_KERNEL_ATTRIBUTE_NAME, &kernel_name, sizeof(kernel_name));
}

Kernel::~Kernel()
{
    vxReleaseKernel(&m_handle);
}

const char *Kernel::name()
{
    return kernel_name;
}

vx_enum Kernel::enumeration()
{
    return kernel_enum;
}

vx_uint32 Kernel::numParameters()
{
    vx_uint32 v;
    vxQueryKernel(handle(), VX_KERNEL_ATTRIBUTE_NUMPARAMS, &v, sizeof(v));
    return v;
}


