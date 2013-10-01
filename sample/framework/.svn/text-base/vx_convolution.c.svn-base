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

void vxDestructConvolution(vx_reference_t *ref)
{
    vx_convolution_t *convolution = (vx_convolution_t *)ref;
    vxFreeMemory(convolution->base.base.context, &convolution->base.memory);
}

void vxReleaseConvolutionInt(vx_convolution_t *convolution)
{
    vxReleaseReference(&convolution->base.base, VX_TYPE_CONVOLUTION, vx_true_e, vxDestructConvolution);
}

void vxReleaseConvolution(vx_convolution *conv)
{
    vx_convolution_t *convolution = (vx_convolution_t *)(conv?*conv:0);
    vxReleaseReference(&convolution->base.base, VX_TYPE_CONVOLUTION, vx_false_e, vxDestructConvolution);
    if (conv) *conv = 0;
}

static VX_INLINE int isodd(size_t a)
{
    return (int)(a & 1);
}

vx_convolution vxCreateConvolution(vx_context context, vx_size columns, vx_size rows)
{
    vx_convolution_t *convolution = NULL;
    if (vxIsValidContext((vx_context_t *)context) == vx_true_e &&
        isodd(columns) && columns >= 3 && isodd(rows) && rows >= 3)
    {
        convolution = VX_CALLOC(vx_convolution_t);
        if (convolution)
        {
            vxInitReference(&convolution->base.base, (vx_context_t *)context, VX_TYPE_CONVOLUTION);
            vxIncrementReference(&convolution->base.base);
            vxAddReference(convolution->base.base.context, &convolution->base.base);
            convolution->base.type = VX_TYPE_INT16;
            convolution->base.columns = columns;
            convolution->base.rows = rows;
            convolution->base.memory.ndims = 2;
            convolution->base.memory.nptrs = 1;
            convolution->base.memory.dims[0][0] = sizeof(vx_int16);
            convolution->base.memory.dims[0][1] = (vx_int32)(columns*rows);
            convolution->scale = 1;
        }
    }
    return (vx_convolution)convolution;
}

vx_status vxQueryConvolution(vx_convolution conv, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_convolution_t *convolution = (vx_convolution_t *)conv;
    if (vxIsValidSpecificReference(&convolution->base.base, VX_TYPE_CONVOLUTION) == vx_false_e)
    {
        return VX_ERROR_INVALID_REFERENCE;
    }
    switch (attribute)
    {
        case VX_CONVOLUTION_ATTRIBUTE_ROWS:
            if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
            {
                *(vx_size *)ptr = convolution->base.rows;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_CONVOLUTION_ATTRIBUTE_COLUMNS:
            if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
            {
                *(vx_size *)ptr = convolution->base.columns;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_CONVOLUTION_ATTRIBUTE_SCALE:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                *(vx_uint32 *)ptr = convolution->scale;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_CONVOLUTION_ATTRIBUTE_SIZE:
            if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
            {
                *(vx_size *)ptr = convolution->base.columns * convolution->base.rows * sizeof(vx_int16);
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
    return status;
}

static vx_bool vxIsPowerOfTwo(vx_uint32 a)
{
    if (a == 0)
        return vx_false_e;
    else if ((a & ((a) - 1)) == 0)
        return vx_true_e;
    else
        return vx_false_e;
}

vx_status vxSetConvolutionAttribute(vx_convolution conv, vx_enum attr, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_convolution_t *convolution = (vx_convolution_t *)conv;

    if (vxIsValidSpecificReference(&convolution->base.base, VX_TYPE_CONVOLUTION) == vx_false_e)
    {
        return VX_ERROR_INVALID_REFERENCE;
    }
    switch (attr)
    {
        case VX_CONVOLUTION_ATTRIBUTE_SCALE:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                vx_uint32 scale = *(vx_uint32 *)ptr;
                if (vxIsPowerOfTwo(scale) == vx_true_e)
                {
                    VX_PRINT(VX_ZONE_INFO, "Convolution Scale assigned to %u\n", scale);
                    convolution->scale = scale;
                }
                else
                {
                    status = VX_ERROR_INVALID_VALUE;
                }
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        default:
            status = VX_ERROR_INVALID_PARAMETERS;
            break;
    }
    if (status != VX_SUCCESS)
    {
        VX_PRINT(VX_ZONE_ERROR, "Failed to set attribute on convolution! (%d)\n", status);
    }
    return status;
}

vx_status vxAccessConvolutionCoefficients(vx_convolution conv, vx_int16 *array)
{
    vx_convolution_t *convolution = (vx_convolution_t *)conv;
    vx_status status = VX_ERROR_INVALID_REFERENCE;
    if ((vxIsValidSpecificReference(&convolution->base.base, VX_TYPE_CONVOLUTION) == vx_true_e) &&
        (vxAllocateMemory(convolution->base.base.context, &convolution->base.memory) == vx_true_e))
    {
        vxSemWait(&convolution->base.base.lock);
        if (array)
        {
            vx_size size = convolution->base.memory.strides[0][1] *
                           convolution->base.memory.dims[0][1];
            memcpy(array, convolution->base.memory.ptrs[0], size);
        }
        vxSemPost(&convolution->base.base.lock);
        vxReadFromReference(&convolution->base.base);
        vxIncrementReference(&convolution->base.base);
        status = VX_SUCCESS;
    }
    return status;
}

vx_status vxCommitConvolutionCoefficients(vx_convolution conv, vx_int16 *array)
{
    vx_convolution_t *convolution = (vx_convolution_t *)conv;
    vx_status status = VX_ERROR_INVALID_REFERENCE;
    if ((vxIsValidSpecificReference(&convolution->base.base, VX_TYPE_CONVOLUTION) == vx_true_e) &&
        (vxAllocateMemory(convolution->base.base.context, &convolution->base.memory) == vx_true_e))
    {
        vxSemWait(&convolution->base.base.lock);
        if (array)
        {
            vx_size size = convolution->base.memory.strides[0][1] *
                           convolution->base.memory.dims[0][1];

            memcpy(convolution->base.memory.ptrs[0], array, size);
        }
        vxSemPost(&convolution->base.base.lock);
        vxWroteToReference(&convolution->base.base);
        vxDecrementReference(&convolution->base.base);
        status = VX_SUCCESS;
    }
    return status;
}
