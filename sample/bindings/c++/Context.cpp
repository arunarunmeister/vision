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

#include <stdlib.h>
#include <vx.hpp>

using namespace OpenVX;

vx_context Context::Handle()
{
    if (!m_handle)
    {
        m_handle = vxCreateContext();
    }
    return m_handle;
}

Context::Context() : Reference(Handle())
{
}

Context::~Context()
{
    vxReleaseContext(&m_handle);
}

vx_status Context::LoadKernels(const char *filepath)
{
    return vxLoadKernels(getContext().Handle(), (vx_char *)filepath);
}

vx_uint32 Context::numModules()
{
    vx_uint32 modules = 0;
    vxQueryContext(getContext().Handle(), VX_CONTEXT_ATTRIBUTE_NUMMODULES, &modules, sizeof(modules));
    return modules;
}

vx_uint32 Context::numKernels()
{
    vx_uint32 kernels = 0;
    vxQueryContext(getContext().Handle(), VX_CONTEXT_ATTRIBUTE_NUMKERNELS, &kernels, sizeof(kernels));
    return kernels;
}

vx_uint32 Context::numTargets()
{
    vx_uint32 targets = 0;
    vxQueryContext(getContext().Handle(), VX_CONTEXT_ATTRIBUTE_NUMTARGETS, &targets, sizeof(targets));
    return targets;
}

const char *Context::implementation()
{
    static vx_char impl_name[VX_MAX_IMPLEMENTATION_NAME] = {0};
    if (impl_name[0] == '\0')
    {
        vxQueryContext(getContext().Handle(), VX_CONTEXT_ATTRIBUTE_IMPLEMENTATION, impl_name, sizeof(impl_name));
    }
    return (const char *)&impl_name[0];
}

vx_uint16 Context::vendorID()
{
    vx_uint16 vendor_id;
    vxQueryContext(getContext().Handle(), VX_CONTEXT_ATTRIBUTE_VENDOR_ID, &vendor_id, sizeof(vendor_id));
    return vendor_id;
}

vx_uint16 Context::version()
{
    vx_uint16 version;
    vxQueryContext(getContext().Handle(), VX_CONTEXT_ATTRIBUTE_VERSION, &version, sizeof(version));
    return version;
}

const char *Context::extensions()
{
    static vx_char *extensions = NULL;
    if (extensions == NULL)
    {
        vx_size extsize = 0;
        vxQueryContext(getContext().Handle(), VX_CONTEXT_ATTRIBUTE_EXTENSIONS_SIZE, &extsize, sizeof(extsize));
        extensions = (vx_char *)calloc(sizeof(vx_char),extsize);
        if (extensions)
        {
            vxQueryContext(getContext().Handle(), VX_CONTEXT_ATTRIBUTE_EXTENSIONS, extensions, extsize);
        }
    }
    return (const char *)extensions;
}

