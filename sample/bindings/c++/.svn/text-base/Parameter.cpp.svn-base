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

Parameter::Parameter(Node *n, vx_uint32 index) : Reference(vxGetParameterByIndex(n->handle(), index))
{
}

Parameter::~Parameter()
{
    vxReleaseParameter(&m_handle);
}

vx_enum Parameter::GetDirection(vx_uint32 index)
{
    vx_enum v;
    vxQueryParameter(handle(), VX_PARAMETER_ATTRIBUTE_DIRECTION, &v, sizeof(v));
    return v;
}

vx_enum Parameter::GetType(vx_uint32 index)
{
    vx_enum v;
    vxQueryParameter(handle(), VX_PARAMETER_ATTRIBUTE_TYPE, &v, sizeof(v));
    return v;
}

vx_reference Parameter::GetReference(vx_uint32 index)
{
    vx_reference v;
    vxQueryParameter(handle(), VX_PARAMETER_ATTRIBUTE_REF, &v, sizeof(v));
    return v;
}

vx_status Parameter::SetReference(Reference *ref)
{
    return vxSetParameterByReference(handle(), ref->handle());
}

vx_status Parameter::SetReference(Reference &ref)
{
    return vxSetParameterByReference(handle(), ref.handle());
}

