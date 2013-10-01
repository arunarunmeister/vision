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
 * \brief The Integral Image Kernel
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>

static vx_status vxIntegralImageKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
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

        status = VX_SUCCESS;
        rect = vxGetValidRegionImage(src);
        status |= vxAccessImagePatch(src, rect, 0, &src_addr, &src_base);
        status |= vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);

        for (y = 0; (y < src_addr.dim_y) && (status == VX_SUCCESS); y++)
        {
            vx_uint8 *pixels = vxFormatImagePatchAddress2d(src_base, 0, y, &src_addr);
            vx_uint32 *sums = vxFormatImagePatchAddress2d(dst_base, 0, y, &dst_addr);

            if (y == 0)
            {
                sums[0] = pixels[0];
                for (x = 1; x < src_addr.dim_x; x++)
                {
                    sums[x] = sums[x-1] + pixels[x];
                }
            }
            else
            {
                vx_uint32 *prev_sums = vxFormatImagePatchAddress2d(dst_base, 0, y-1, &dst_addr);
                sums[0] = prev_sums[0] + pixels[0];
                for (x = 1; x < src_addr.dim_x; x++)
                {
                    sums[x] = pixels[x] + sums[x-1] + prev_sums[x] - prev_sums[x-1];
                }
            }
        }
        status |= vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
        status |= vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
        vxReleaseRectangle(&rect);
    }
    return status;
}

static vx_status vxIntegralInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0)
    {
        vx_image input = 0;
        vx_parameter param = vxGetParameterByIndex(node, index);

        vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
        if (input)
        {
            vx_uint32 width = 0, height = 0;
            vx_fourcc format = 0;
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
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

static vx_status vxIntegralOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 1)
    {
        vx_parameter param = vxGetParameterByIndex(node, 0); /* we reference the input image */
        if (param)
        {
            vx_image input = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
            if (input)
            {
                vx_uint32 width = 0, height = 0;
                vxQueryImage(input, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
                vxQueryImage(input, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
                ptr->type = VX_TYPE_IMAGE;
                ptr->dim.image.format = FOURCC_U32;
                ptr->dim.image.width = width;
                ptr->dim.image.height = height;
                status = VX_SUCCESS;
            }
            vxReleaseParameter(&param);
        }
    }
    return status;
}

static vx_param_description_t integral_image_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t integral_image_kernel = {
    VX_KERNEL_INTEGRAL_IMAGE,
    "org.khronos.openvx.integral_image",
    vxIntegralImageKernel,
    integral_image_kernel_params, dimof(integral_image_kernel_params),
    vxIntegralInputValidator,
    vxIntegralOutputValidator,
    NULL,
    NULL,
};


