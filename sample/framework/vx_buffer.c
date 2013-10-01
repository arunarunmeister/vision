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

#include <vx_internal.h>

vx_bool vxIsValidBuffer(vx_buffer_t *buf)
{
    vxPrintReference((vx_reference_t *)buf);
    if ((buf != NULL) && (buf->base.magic == VX_MAGIC) &&
        (vxIsValidContext(buf->base.context) == vx_true_e) &&
        ((buf->base.type == VX_TYPE_BUFFER) ||
         (buf->base.type == VX_TYPE_LUT) ||
         (buf->base.type == VX_TYPE_DISTRIBUTION)))
    {
        return vx_true_e;
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR,"Invalid Buffer\n");
        return vx_false_e;
    }
}

void vxFreeBuffer(vx_buffer_t *buffer)
{
    vxFreeMemory(buffer->base.context, &buffer->memory);
}

vx_bool vxAllocateBuffer(vx_buffer_t *buffer)
{
    vx_bool ret = vx_false_e;
    if (buffer->memory.ptrs[0] == NULL)
    {
        buffer->memory.nptrs = 1;
        buffer->memory.ndims = 2;
        buffer->memory.dims[0][0] = (int32_t)buffer->unitSize;
        buffer->memory.dims[0][1] = (int32_t)buffer->numUnits;
        if (vxAllocateMemory(buffer->base.context, &buffer->memory) == vx_true_e)
        {
            VX_PRINT(VX_ZONE_BUFFER, "allocated buffer ptr:%p of %ux%u\n",
                buffer->memory.ptrs[0],
                buffer->memory.dims[0][0],
                buffer->memory.dims[0][1]);
            ret= vx_true_e;
        }
    }
    else
    {
        ret = vx_true_e;
    }
    return ret;
}

vx_buffer vxCreateBuffer(vx_context c, vx_size unitSize, vx_size numUnits) {
    vx_buffer_t *buf = NULL;
    vx_context_t *context = (vx_context_t *)c;
    if (vxIsValidContext(context) == vx_true_e)
    {
        buf = VX_CALLOC(vx_buffer_t);
        if (buf)
        {
            vxInitReference((vx_reference_t *)buf, context, VX_TYPE_BUFFER);
            vxIncrementReference(&buf->base);
            vxAddReference(context, (vx_reference_t *)buf);
            buf->unitSize = unitSize;
            buf->numUnits = numUnits;
            VX_PRINT(VX_ZONE_BUFFER, "Created Buffer %p\n", buf);
        }
    }
    return (vx_buffer)buf;
}

vx_buffer vxCreateVirtualBuffer(vx_context c)
{
    return vxCreateBuffer(c, 0, 0);
}

vx_buffer vxCreateBufferFromHandle(vx_context context, vx_size unitSize, vx_size numUnits, void *ptr, vx_enum type)
{
    vx_buffer buf = 0;

    if (vxIsValidImport(type) == vx_false_e)
        return 0;

    buf = vxCreateBuffer(context, unitSize, numUnits);
    if (buf)
    {
        vx_buffer_t *buffer = (vx_buffer_t *)buf;
        buffer->memory.ptrs[0] = ptr;
        buffer->import = type;
    }
    return buf;
}

vx_status vxQueryBuffer(vx_buffer buffer, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_buffer_t *buf = (vx_buffer_t*)buffer;
    if (vxIsValidBuffer(buf) == vx_true_e)
    {
        switch (attribute)
        {
            case VX_BUFFER_ATTRIBUTE_NUMUNITS:
                if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
                {
                    *(vx_size *)ptr = buf->numUnits;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_BUFFER_ATTRIBUTE_UNITSIZE:
                if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
                {
                    *(vx_size *)ptr = buf->unitSize;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_BUFFER_ATTRIBUTE_SIZE:
                if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
                {
                    *(vx_size *)ptr = buf->unitSize * buf->numUnits;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            default:
                status = VX_ERROR_NOT_SUPPORTED;
                break;
        }
    }
    else
    {
        status = VX_ERROR_INVALID_REFERENCE;
    }
    VX_PRINT(VX_ZONE_API, "%s returned %d\n",__FUNCTION__, status);
    return status;
}

void vxDestructBuffer(vx_reference_t *ref)
{
    vx_buffer_t *buf = (vx_buffer_t *)ref;
    vxFreeBuffer(buf);
    VX_PRINT(VX_ZONE_BUFFER, "Ref Count Zero, Releasing "VX_FMT_REF"\n", ref);
}

void vxReleaseBufferInt(vx_buffer_t *buf)
{
    vxReleaseReference((vx_reference_t *)buf, VX_TYPE_BUFFER, vx_true_e, vxDestructBuffer);
}

void vxReleaseBuffer(vx_buffer *buffer)
{
    vx_buffer_t *buf = (vx_buffer_t *)(*buffer);
    vxReleaseReference((vx_reference_t *)buf, VX_TYPE_BUFFER, vx_false_e, vxDestructBuffer);
    if (buffer) *buffer = 0;
}

vx_status vxAccessBufferRange(vx_buffer buffer, vx_size start, vx_size end, void **ptr)
{
    vx_status status = VX_SUCCESS;
    vx_buffer_t * buf = (vx_buffer_t *)buffer;

    VX_PRINT(VX_ZONE_BUFFER, "buffer="VX_FMT_REF" s,e={"VX_FMT_SIZE", "VX_FMT_SIZE"} ptr=%p, *ptr=%p\n",
            buffer, start, end, ptr, (ptr?*ptr:NULL));

    if (vxIsValidBuffer(buf) == vx_false_e)
    {
        VX_PRINT(VX_ZONE_ERROR,"Not a valid reference!\n");
        return VX_ERROR_INVALID_REFERENCE;
    }

    /* verify has not run or will not run yet. this allows this API to "touch"
     * the buffer to create it.
     */
    if (buf->memory.ptrs[0] == NULL)
    {
        if (vxAllocateBuffer(buf) == vx_false_e)
        {
            return VX_ERROR_NO_MEMORY;
        }
    }

    if ((start < end) && (end <= buf->numUnits) && (ptr != NULL))
    {
        vxSemWait(&buf->base.lock);
        {
            vx_size index = (start * buf->unitSize);

            if (*ptr != NULL)
            {
                vx_size len = ((end - start) * buf->unitSize);
                memcpy(*ptr, &buf->memory.ptrs[0][index], len);
                VX_PRINT(VX_ZONE_BUFFER, "vxGetBufferRange was a copy!\n");
            }
            else
            {
                *ptr = &buf->memory.ptrs[0][index];
                VX_PRINT(VX_ZONE_BUFFER, "Buffer ptr %p is a map from %p!\n", *ptr, buf->memory.ptrs[0]);
            }
        }
        vxSemPost(&buf->base.lock);
        vxReadFromReference(&buf->base);
        vxIncrementReference(&buf->base);
    }
    else
    {
        VX_PRINT(VX_ZONE_BUFFER, "numUnits=%u\n", buf->numUnits);
        status = VX_ERROR_INVALID_PARAMETERS;
    }
    return status;
}

vx_status vxCommitBufferRange(vx_buffer buffer, vx_size start, vx_size end, void *ptr)
{
    vx_status status = VX_SUCCESS;
    vx_buffer_t *buf = (vx_buffer_t *)buffer;

    if (vxIsValidBuffer(buf) == vx_false_e)
    {
        return VX_ERROR_INVALID_REFERENCE;
    }
    if ((start <= end) && (end <= buf->numUnits) && (ptr != NULL))
    {
        vxSemWait(&buf->base.lock);
        {
            vx_size index = (start * buf->unitSize);
            vx_uint8 *beg_ptr = (vx_uint8 *)buf->memory.ptrs[0];
            vx_uint8 *end_ptr = &beg_ptr[buf->unitSize * buf->numUnits];
            if (!(beg_ptr <= (vx_uint8 *)ptr && (vx_uint8 *)ptr < end_ptr)) // the pointer was not mapped, copy.
            {
                // copy the range back
                vx_size len = (end - start) * buf->unitSize;
                memcpy(&beg_ptr[index], ptr, len);
                VX_PRINT(VX_ZONE_BUFFER, "Copied %u bytes back to buffer\n", len);
            }
            else
            {
                VX_PRINT(VX_ZONE_BUFFER, "Buffer ptr %p was a map!\n", ptr);
            }
        }
        vxSemPost(&buf->base.lock);
        vxWroteToReference(&buf->base);
        vxDecrementReference(&buf->base);
    }
    else
        status = VX_ERROR_INVALID_PARAMETERS;
    return status;
}

vx_size vxComputeBufferRangeSize(vx_buffer buffer, vx_uint32 start, vx_uint32 end)
{
    vx_buffer_t *buf = (vx_buffer_t *)buffer;
    vx_size size = 0;

    if (vxIsValidBuffer(buf) == vx_false_e)
        return size;

    if ((start < end) && (end <= buf->numUnits))
    {
        size = (end-start)*buf->unitSize;
    }
    VX_PRINT(VX_ZONE_BUFFER, "buffer:%p has size "VX_FMT_SIZE" over %u,%u\n", buf, size, start, end);

    return size;
}
