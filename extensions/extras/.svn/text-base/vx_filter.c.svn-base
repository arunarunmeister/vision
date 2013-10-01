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
 * \brief The Filter Kernel (Extras)
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_ext_extras.h>
#include <VX/vx_helper.h>

static vx_int16 laplacian[3][3] = {
    {1, 1, 1},
    {1,-8, 1},
    {1, 1, 1},
};

static vx_uint8 vx_clamp8_i32(vx_int32 value)
{
    vx_uint8 v = 0;
    if (value > 255)
    {
        v = 255;
    }
    else if (value < 0)
    {
        v = 0;
    }
    else
    {
        v = (vx_uint8)value;
    }
    return v;
}

vx_int32 vx_convolve8with16(void *base, vx_uint32 x, vx_uint32 y, vx_imagepatch_addressing_t *addr, vx_int16 conv[3][3])
{
    vx_int32 stride_y = (addr->stride_y * addr->scale_y)/VX_SCALE_UNITY;
    vx_int32 stride_x = (addr->stride_x * addr->scale_x)/VX_SCALE_UNITY;
    vx_uint8 *ptr = (vx_uint8 *)base;
    vx_uint32 i = (y * stride_y) + (x + stride_x);
    vx_uint32 indexes[3][3] = {
        {i - stride_y - stride_x, i - stride_y, i - stride_y + stride_x},
        {i - stride_x,            i,            i + stride_x},
        {i + stride_y - stride_x, i + stride_y, i + stride_y + stride_x},
    };
    vx_uint8 pixels[3][3] = {
        {ptr[indexes[0][0]], ptr[indexes[0][1]], ptr[indexes[0][2]]},
        {ptr[indexes[1][0]], ptr[indexes[1][1]], ptr[indexes[1][2]]},
        {ptr[indexes[2][0]], ptr[indexes[2][1]], ptr[indexes[2][2]]},
    };
    vx_int32 div = conv[0][0] + conv[0][1] + conv[0][2] +
                   conv[1][0] + conv[1][1] + conv[1][2] +
                   conv[2][0] + conv[2][1] + conv[2][2];
    vx_int32 sum = (conv[0][0] * pixels[0][0]) + (conv[0][1] * pixels[0][1]) + (conv[0][2] * pixels[0][2]) +
                   (conv[1][0] * pixels[1][0]) + (conv[1][1] * pixels[1][1]) + (conv[1][2] * pixels[1][2]) +
                   (conv[2][0] * pixels[2][0]) + (conv[2][1] * pixels[2][1]) + (conv[2][2] * pixels[2][2]);
    if (div == 0)
        div = 1;
    return sum / div;
}

static vx_status vxLaplacian3x3Kernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 2)
    {
        vx_image src = (vx_image)parameters[0];
        vx_image dst = (vx_image)parameters[1];
        vx_uint32 y, x;
        void *src_base = NULL;
        void *dst_base = NULL;
        vx_imagepatch_addressing_t src_addr, dst_addr;
        vx_rectangle rect;
        vx_border_mode_t borders = {VX_BORDER_MODE_UNDEFINED, 0};

        status = VX_SUCCESS;
        rect = vxGetValidRegionImage(src);
        status |= vxAccessImagePatch(src, rect, 0, &src_addr, &src_base);
        status |= vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
        status |= vxQueryNode(node, VX_NODE_ATTRIBUTE_BORDER_MODE, &borders, sizeof(borders));
        /*! \todo Implement other border modes */
        if (borders.mode == VX_BORDER_MODE_UNDEFINED)
        {
            /* shrink the image by 1 */
            vxAlterRectangle(rect, 1, 1, -1, -1);

            for (y = 1; y < (src_addr.dim_y - 1); y++)
            {
                for (x = 1; x < (src_addr.dim_x - 1); x++)
                {
                    vx_uint8 *dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    vx_int32 value = vx_convolve8with16(src_base, x, y, &src_addr, laplacian);
                    *dst = vx_clamp8_i32(value);
                }
            }
        }
        else
        {
            status = VX_ERROR_NOT_IMPLEMENTED;
        }
        status |= vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
        status |= vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
        vxReleaseRectangle(&rect);
    }
    return status;
}

static vx_status vxFilterInputValidator(vx_node node, vx_uint32 index)
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
            if (format == FOURCC_U8)
            {
                status = VX_SUCCESS;
            }
        }
        vxReleaseParameter(&param);
    }
    return status;
}

static vx_status vxFilterOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 1)
    {
        vx_image input = 0;
        vx_parameter param = vxGetParameterByIndex(node, 0); /* we reference the input image */

        vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
        if (input)
        {
            vx_uint32 width = 0, height = 0;
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
            ptr->type = VX_TYPE_IMAGE;
            ptr->dim.image.format = FOURCC_U8;
            ptr->dim.image.width = width;
            ptr->dim.image.height = height;
            status = VX_SUCCESS;
        }
        vxReleaseParameter(&param);
    }
    return status;
}

static vx_param_description_t filter_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t laplacian3x3_kernel = {
    VX_KERNEL_EXTRAS_LAPLACIAN_3x3,
    "org.khronos.extras.laplacian3x3",
    vxLaplacian3x3Kernel,
    filter_kernel_params, dimof(filter_kernel_params),
    vxFilterInputValidator,
    vxFilterOutputValidator,
    NULL,
    NULL,
};
