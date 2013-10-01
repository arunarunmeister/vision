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

/*!
 * \file
 * \brief The Min and Maximum Locations Kernel.
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>

static vx_status vxMinMaxLocKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 5)
    {
        vx_image input = (vx_image)parameters[0];
        vx_scalar sminVal = (vx_scalar)parameters[1];
        vx_scalar smaxVal = (vx_scalar)parameters[2];
        vx_coordinates sminLoc = (vx_coordinates)parameters[3];
        vx_coordinates smaxLoc = (vx_coordinates)parameters[4];
        vx_uint32 y, x;
        void *src_base = NULL;
        vx_imagepatch_addressing_t src_addr;
        vx_rectangle rect;
        vx_enum type;
        vx_int64 minVal = INT64_MAX;
        vx_int64 maxVal = INT64_MIN;
        vx_int32 minLoc[2];
        vx_int32 maxLoc[2];

        status = VX_SUCCESS;
        status |= vxQueryScalar(sminVal, VX_SCALAR_ATTRIBUTE_TYPE, &type, sizeof(type));
        rect = vxGetValidRegionImage(input);
        status |= vxAccessImagePatch(input, rect, 0, &src_addr, (void **)&src_base);
        for (y = 0; y < src_addr.dim_y; y++)
        {
            for (x = 0; x < src_addr.dim_x; x++)
            {
                void *src = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                if (type == VX_TYPE_UINT8)
                {
                    vx_uint8 v = *(vx_uint8 *)src;
                    if (v > maxVal)
                    {
                        maxVal = (vx_int64)v;
                        maxLoc[0] = x;
                        maxLoc[1] = y;
                    }
                    if (v < minVal)
                    {
                        minVal = (vx_int64)v;
                        minLoc[0] = x;
                        minLoc[1] = y;
                    }
                }
                else if (type == VX_TYPE_UINT16)
                {
                    vx_uint16 v = *(vx_uint16 *)src;
                    if (v > maxVal)
                    {
                        maxVal = (vx_int64)v;
                        maxLoc[0] = x;
                        maxLoc[1] = y;
                    }
                    if (v < minVal)
                    {
                        minVal = (vx_int64)v;
                        minLoc[0] = x;
                        minLoc[1] = y;
                    }
                }
                else if (type == VX_TYPE_UINT32)
                {
                    vx_uint32 v = *(vx_uint32 *)src;
                    if (v > maxVal)
                    {
                        maxVal = (vx_int64)v;
                        maxLoc[0] = x;
                        maxLoc[1] = y;
                    }
                    if (v < minVal)
                    {
                        minVal = (vx_int64)v;
                        minLoc[0] = x;
                        minLoc[1] = y;
                    }
                }
                else if (type == VX_TYPE_INT16)
                {
                    vx_int16 v = *(vx_int16 *)src;
                    if (v > maxVal)
                    {
                        maxVal = (vx_int64)v;
                        maxLoc[0] = x;
                        maxLoc[1] = y;
                    }
                    if (v < minVal)
                    {
                        minVal = (vx_int64)v;
                        minLoc[0] = x;
                        minLoc[1] = y;
                    }
                }
                else if (type == VX_TYPE_INT32)
                {
                    vx_int32 v = *(vx_int32 *)src;
                    if (v > maxVal)
                    {
                        maxVal = (vx_int64)v;
                        maxLoc[0] = x;
                        maxLoc[1] = y;
                    }
                    if (v < minVal)
                    {
                        minVal = (vx_int64)v;
                        minLoc[0] = x;
                        minLoc[1] = y;
                    }
                }
            }
        }
        VX_PRINT(VX_ZONE_INFO, "Min = %ld Max = %ld\n", minVal, maxVal);
        status |= vxCommitImagePatch(input, 0, 0, &src_addr, src_base);
        vxCommitScalarValue(sminVal, &minVal);
        vxCommitScalarValue(smaxVal, &maxVal);
        vxCommitCoordinates(sminLoc, minLoc);
        vxCommitCoordinates(smaxLoc, maxLoc);
        vxReleaseRectangle(&rect);
    }
    return status;
}

static vx_status vxMinMaxLocInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0)
    {
        vx_image input = 0;
        vx_parameter param = vxGetParameterByIndex(node, index);

        vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
        if (input)
        {
            vx_fourcc format = 0;
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
            if ((format == FOURCC_U8) ||
                (format == FOURCC_U16) ||
                (format == FOURCC_S16) ||
                (format == FOURCC_U32) ||
                (format == FOURCC_S32))
            {
                status = VX_SUCCESS;
            }
        }
        vxReleaseParameter(&param);
    }
    return status;
}

static vx_status vxMinMaxLocOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if ((index == 1) || (index == 2))
    {
        vx_parameter param = vxGetParameterByIndex(node, 0);
        if (param)
        {
            vx_image input = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
            if (input)
            {
                vx_fourcc format;
                vx_enum type = VX_TYPE_INVALID;
                vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
                switch (format)
                {
                    case FOURCC_U8:
                        type = VX_TYPE_UINT8;
                        break;
                    case FOURCC_U16:
                        type = VX_TYPE_UINT16;
                        break;
                    case FOURCC_U32:
                        type = VX_TYPE_UINT32;
                        break;
                    case FOURCC_S16:
                        type = VX_TYPE_INT16;
                        break;
                    case FOURCC_S32:
                        type = VX_TYPE_INT32;
                        break;
                    default:
                        type = VX_TYPE_INVALID;
                        break;
                }
                if (type != VX_TYPE_INVALID)
                {
                    status = VX_SUCCESS;
                    ptr->type = VX_TYPE_SCALAR;
                    ptr->dim.scalar.type = type;
                }
                else
                {
                    status = VX_ERROR_INVALID_TYPE;
                }
            }
            vxReleaseParameter(&param);
        }
    }
    if ((index == 3) || (index == 4))
    {
        /* nothing to check here */
        ptr->type = VX_TYPE_COORDINATES;
        status = VX_SUCCESS;
    }
    return status;
}

static vx_param_description_t minmaxloc_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_OPTIONAL},
    {VX_OUTPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_OPTIONAL},
    {VX_OUTPUT, VX_TYPE_COORDINATES, VX_PARAMETER_STATE_OPTIONAL},
    {VX_OUTPUT, VX_TYPE_COORDINATES, VX_PARAMETER_STATE_OPTIONAL},
};

vx_kernel_description_t minmaxloc_kernel = {
    VX_KERNEL_MINMAXLOC,
    "org.khronos.openvx.min_max_loc",
    vxMinMaxLocKernel,
    minmaxloc_kernel_params, dimof(minmaxloc_kernel_params),
    vxMinMaxLocInputValidator,
    vxMinMaxLocOutputValidator,
    NULL,
    NULL,
};


