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

Buffer::Buffer(vx_size unitSize,
               vx_size numUnits) :
               Reference(vxCreateBuffer(Context::getContext().handle(), unitSize, numUnits))
{
    ; // do nothing
}

Buffer::Buffer() : Reference(vxCreateVirtualBuffer(Context::getContext().handle()))
{
    ; // do nothing.
}

Buffer::~Buffer()
{
    vxReleaseBuffer(&m_handle);
}

vx_size Buffer::size()
{
    vx_size len;
    vxQueryBuffer(handle(), VX_BUFFER_ATTRIBUTE_SIZE, &len, sizeof(len));
    return len;
}

vx_size Buffer::numUnits()
{
    vx_size v;
    vxQueryBuffer(handle(), VX_BUFFER_ATTRIBUTE_NUMUNITS, &v, sizeof(v));
    return v;
}

vx_size Buffer::unitSize()
{
    vx_size v;
    vxQueryBuffer(handle(), VX_BUFFER_ATTRIBUTE_UNITSIZE, &v, sizeof(v));
    return v;
}

vx_status Buffer::GetBufferRange(vx_uint32 su, vx_uint32 eu, void **ptr)
{
    return vxAccessBufferRange(handle(), su, eu, ptr);
}

vx_status Buffer::SetBufferRange(vx_uint32 su, vx_uint32 eu, void *ptr)
{
    return vxCommitBufferRange(handle(), su, eu, ptr);
}


