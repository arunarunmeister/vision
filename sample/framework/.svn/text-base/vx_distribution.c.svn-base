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

vx_distribution vxCreateDistribution(vx_context c, vx_size numBins, vx_size offset, vx_size range)
{
    vx_distribution_t *distribution = NULL;
    vx_context_t *context = (vx_context_t *)c;
    if (vxIsValidContext(context) == vx_true_e)
    {
        distribution = VX_CALLOC(vx_distribution_t);
        if (distribution)
        {
            vxInitReference(&distribution->base, context, VX_TYPE_DISTRIBUTION);
            vxIncrementReference(&distribution->base);
            vxAddReference(context, (vx_reference_t *)distribution);
            distribution->memory.ndims = 2;
            distribution->memory.nptrs = 1;
            distribution->memory.dims[0][VX_DIM_C] = sizeof(vx_int32);
            distribution->memory.dims[0][VX_DIM_X] = (vx_int32)numBins;
            distribution->memory.dims[0][VX_DIM_Y] = 1;
            distribution->window_x = (vx_uint32)range/(vx_uint32)numBins;
            distribution->window_y = 1;
            distribution->offset_x = (vx_uint32)offset;
            distribution->offset_y = 0;
        }
    }
    return (vx_distribution)distribution;
}

void vxDestructDistribution(vx_reference_t *ref)
{
    vx_distribution_t *distribution = (vx_distribution_t *)ref;
    vxFreeMemory(distribution->base.context, &distribution->memory);
}

void vxReleaseDistributionInt(vx_distribution_t *distribution)
{
    vxReleaseReference((vx_reference_t *)distribution, VX_TYPE_DISTRIBUTION, vx_true_e, vxDestructDistribution);
}

void vxReleaseDistribution(vx_distribution *d)
{
    vx_distribution_t *distribution = (vx_distribution_t *)(d?*d:0);
    vxReleaseReference((vx_reference_t *)distribution, VX_TYPE_DISTRIBUTION, vx_false_e, vxDestructDistribution);
    if (d) *d = 0;
}

vx_status vxQueryDistribution(vx_distribution d, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_distribution_t *distribution = (vx_distribution_t *)d;

    if (vxIsValidSpecificReference(&distribution->base, VX_TYPE_DISTRIBUTION) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;

    switch (attribute)
    {
        case VX_DISTRIBUTION_ATTRIBUTE_DIMENSIONS:
            if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
            {
                *(vx_size *)ptr = (vx_size)(distribution->memory.ndims - 1);
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_DISTRIBUTION_ATTRIBUTE_RANGE:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                *(vx_uint32 *)ptr = distribution->memory.dims[0][VX_DIM_X] *
                                    distribution->window_x;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_DISTRIBUTION_ATTRIBUTE_BINS:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                *(vx_uint32 *)ptr = (vx_uint32)distribution->memory.dims[0][VX_DIM_X];
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_DISTRIBUTION_ATTRIBUTE_WINDOW:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                *(vx_uint32 *)ptr = distribution->window_x;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_DISTRIBUTION_ATTRIBUTE_OFFSET:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                *(vx_uint32 *)ptr = distribution->offset_x;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_DISTRIBUTION_ATTRIBUTE_SIZE:
            if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
            {
                vx_int32 d = distribution->memory.ndims - 1;
                *(vx_uint32 *)ptr = distribution->memory.strides[0][d] *
                                    distribution->memory.dims[0][d];
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

vx_status vxAccessDistribution(vx_distribution d, void **ptr)
{
    vx_status status = VX_FAILURE;
    vx_distribution_t *distribution = (vx_distribution_t *)d;
    if ((vxIsValidSpecificReference(&distribution->base, VX_TYPE_DISTRIBUTION) == vx_true_e) &&
        (vxAllocateMemory(distribution->base.context, &distribution->memory) == vx_true_e))
    {
        if (ptr != NULL)
        {
            vxSemWait(&distribution->base.lock);
            {
                vx_int32 dims = distribution->memory.ndims;
                vx_size size = distribution->memory.strides[0][dims-1] *
                               distribution->memory.dims[0][dims-1];
                vxPrintMemory(&distribution->memory);
                if (*ptr == NULL)
                {
                    *ptr = distribution->memory.ptrs[0];
                }
                else if (*ptr != NULL)
                {
                    memcpy(*ptr, distribution->memory.ptrs[0], size);
                }
            }
            vxSemPost(&distribution->base.lock);
            vxReadFromReference(&distribution->base);
        }
        vxIncrementReference(&distribution->base);
        status = VX_SUCCESS;
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Not a valid object!\n");
    }
    return status;
}

vx_status vxCommitDistribution(vx_distribution d, void *ptr)
{
    vx_status status = VX_FAILURE;
    vx_distribution_t *distribution = (vx_distribution_t *)d;
    if ((vxIsValidSpecificReference(&distribution->base, VX_TYPE_DISTRIBUTION) == vx_true_e) &&
        (vxAllocateMemory(distribution->base.context, &distribution->memory) == vx_true_e))
    {
        if (ptr != NULL)
        {
            vxSemWait(&distribution->base.lock);
            {
                vx_int32 dims = distribution->memory.ndims;
                vx_size size = distribution->memory.strides[0][dims-1] *
                               distribution->memory.dims[0][dims-1];
                if (ptr != distribution->memory.ptrs[0])
                {
                    memcpy(distribution->memory.ptrs[0], ptr, size);
                }
            }
            vxSemPost(&distribution->base.lock);
            vxWroteToReference(&distribution->base);
        }
        vxDecrementReference(&distribution->base);
        status = VX_SUCCESS;
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Not a valid object!\n");
    }
    return status;
}


