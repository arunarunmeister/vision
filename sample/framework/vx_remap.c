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

vx_remap vxCreateRemap(vx_context c, vx_uint32 src_x, vx_uint32 src_y,
                                     vx_uint32 dst_x, vx_uint32 dst_y)
{
    vx_remap_t *remap = NULL;
    vx_context_t *context = (vx_context_t *)c;
    if (vxIsValidContext(context) == vx_true_e)
    {
        remap = VX_CALLOC(vx_remap_t);
        if (remap)
        {
            vxInitReference(&remap->base, context, VX_TYPE_REMAP);
            vxIncrementReference(&remap->base);
            vxAddReference(context, (vx_reference_t *)remap);
            remap->src_x = src_x;
            remap->src_y = src_y;
            remap->dst_x = dst_x;
            remap->dst_y = dst_y;
            remap->memory.ndims = 3;
            remap->memory.nptrs = 1;
            remap->memory.dims[0][VX_DIM_C] = sizeof(vx_float32) * 2;
            remap->memory.dims[0][VX_DIM_X] = dst_x;
            remap->memory.dims[0][VX_DIM_Y] = dst_y;
        }
    }
    return (vx_remap)remap;
}

void vxDestructRemap(vx_reference_t *ref)
{
    vx_remap_t *remap = (vx_remap_t *)ref;
    vxFreeMemory(remap->base.context, &remap->memory);
}

void vxReleaseRemapInt(vx_remap_t *remap)
{
    vxReleaseReference((vx_reference_t *)remap, VX_TYPE_REMAP, vx_true_e, vxDestructRemap);
}

void vxReleaseRemap(vx_remap *r)
{
    vx_remap_t *remap = (vx_remap_t *)(r?*r:0);
    vxReleaseReference((vx_reference_t *)remap, VX_TYPE_REMAP, vx_false_e, vxDestructRemap);
    if (r) *r = 0;
}

vx_status vxQueryRemap(vx_remap r, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_remap_t *remap = (vx_remap_t *)r;

    if (vxIsValidSpecificReference(&remap->base, VX_TYPE_REMAP) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;

    switch (attribute)
    {
        case VX_REMAP_ATTRIBUTE_SOURCE_WIDTH:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                *(vx_uint32 *)ptr = remap->src_x;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_REMAP_ATTRIBUTE_SOURCE_HEIGHT:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                *(vx_uint32 *)ptr = remap->src_y;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_REMAP_ATTRIBUTE_DESTINATION_WIDTH:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                *(vx_uint32 *)ptr = remap->dst_x;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_REMAP_ATTRIBUTE_DESTINATION_HEIGHT:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                *(vx_uint32 *)ptr = remap->dst_y;
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

vx_status vxSetRemapPoint(vx_remap r, vx_uint32 dst_x, vx_uint32 dst_y,
                                      vx_float32 src_x, vx_float32 src_y)
{
    vx_status status = VX_FAILURE;
    vx_remap_t *remap = (vx_remap_t *)r;
    if (vxIsValidSpecificReference(&remap->base, VX_TYPE_REMAP) == vx_true_e)
    {
        vx_uint32 t_src_x = (src_x < 0.0f ? 0 : (vx_uint32)src_x);
        vx_uint32 t_src_y = (src_y < 0.0f ? 0 : (vx_uint32)src_y);
        if ((dst_x < remap->dst_x) &&
            (dst_y < remap->dst_y) &&
            (t_src_x < remap->src_x) &&
            (t_src_y && remap->src_y))
        {
            vx_int32 offset = (dst_y * remap->memory.strides[0][VX_DIM_Y]) +
                              (dst_x * remap->memory.strides[0][VX_DIM_X]) +
                              (0 *     remap->memory.strides[0][VX_DIM_C]);
            vx_float32 *coord = (vx_float32 *)&remap->memory.ptrs[0][offset];
            coord[0] = src_x;
            coord[1] = src_y;
        }
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Not a valid object!\n");
    }
    return status;
}

vx_status vGetRemapPoint(vx_remap r, vx_uint32 dst_x, vx_uint32 dst_y,
                                     vx_float32 *src_x, vx_float32 *src_y)
{
    vx_status status = VX_FAILURE;
    vx_remap_t *remap = (vx_remap_t *)r;
    if (vxIsValidSpecificReference(&remap->base, VX_TYPE_REMAP) == vx_true_e)
    {
        if ((dst_x < remap->dst_x) &&
            (dst_y < remap->dst_y))
        {
            vx_int32 offset = (dst_y * remap->memory.strides[0][VX_DIM_Y]) +
                              (dst_x * remap->memory.strides[0][VX_DIM_X]) +
                              (0 *     remap->memory.strides[0][VX_DIM_C]);
            vx_float32 *coord = (vx_float32 *)&remap->memory.ptrs[0][offset];
            *src_x = coord[0];
            *src_y = coord[1];
        }
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Not a valid object!\n");
    }
    return status;
}

