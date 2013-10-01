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
 * \brief The Gradient Magnitude Kernel.
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>
#include <math.h>

static vx_status vxMagnitudeKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 3)
    {
        vx_image grad_x = (vx_image)parameters[0];
        vx_image grad_y = (vx_image)parameters[1];
        vx_image output = (vx_image)parameters[2];
        vx_uint32 y, x;
        vx_fourcc format = 0;
        vx_uint8 *dst_base   = NULL;
        vx_int16 *src_base_x = NULL;
        vx_int16 *src_base_y = NULL;
        vx_imagepatch_addressing_t dst_addr, src_addr_x, src_addr_y;
        vx_rectangle rect;
        vx_uint32 value;

        if (grad_x == 0 || grad_y == 0)
            return VX_ERROR_INVALID_PARAMETERS;
        vxQueryImage(output, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
        rect = vxGetValidRegionImage(grad_x);
        status = VX_SUCCESS;
        status |= vxAccessImagePatch(grad_x, rect, 0, &src_addr_x, (void **)&src_base_x);
        status |= vxAccessImagePatch(grad_y, rect, 0, &src_addr_y, (void **)&src_base_y);
        status |= vxAccessImagePatch(output, rect, 0, &dst_addr, (void **)&dst_base);
        for (y = 0; y < src_addr_x.dim_y; y++)
        {
            for (x = 0; x < src_addr_x.dim_x; x++)
            {
                vx_int16 *in_x = vxFormatImagePatchAddress2d(src_base_x, x, y, &src_addr_x);
                vx_int16 *in_y = vxFormatImagePatchAddress2d(src_base_y, x, y, &src_addr_y);
                if (format == FOURCC_U8)
                {
                    vx_uint8 *dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    vx_int32 grad[2] = {in_x[0]*in_x[0], in_y[0]*in_y[0]};
                    vx_float64 sum = grad[0] + grad[1];
                    value = ((vx_int32)sqrt(sum))/4;
                    *dst = (vx_uint8)(value > UINT8_MAX ? UINT8_MAX : value);
                }
                else if (format == FOURCC_S16)
                {
                    vx_int16 *dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    vx_int32 grad[2] = {in_x[0]*in_x[0], in_y[0]*in_y[0]};
                    vx_float64 sum = grad[0] + grad[1];
                    value = (vx_int32)sqrt(sum);
                    *dst = (vx_int16)(value > INT16_MAX ? INT16_MAX : value);
                }
            }
        }
        status |= vxCommitImagePatch(grad_x, 0, 0, &src_addr_x, src_base_x);
        status |= vxCommitImagePatch(grad_y, 0, 0, &src_addr_y, src_base_y);
        status |= vxCommitImagePatch(output, rect, 0, &dst_addr, dst_base);
        vxReleaseRectangle(&rect);
    }
    return status;
}

static vx_status vxMagnitudeInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0 || index == 1)
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
            if (format == FOURCC_S16)
                status = VX_SUCCESS;
        }
        vxReleaseParameter(&param);
    }
    return status;
}

static vx_status vxMagnitudeOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 2)
    {
        vx_parameter param  = vxGetParameterByIndex(node, 0);
        vx_parameter param2 = vxGetParameterByIndex(node, 2);
        if ((param) && (param2))
        {
            vx_image input = 0, output = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
            vxQueryParameter(param2, VX_PARAMETER_ATTRIBUTE_REF, &output, sizeof(output));
            if ((input) && (output))
            {
                vx_uint32 width = 0, height = 0;
                vx_fourcc format = 0;

                vxQueryImage(input, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
                vxQueryImage(input, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
                vxQueryImage(output, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
                ptr->type = VX_TYPE_IMAGE;
                if (format == FOURCC_U8)
                    ptr->dim.image.format = FOURCC_U8;
                else
                    ptr->dim.image.format = FOURCC_S16; /* virtual images, too */
                ptr->dim.image.width = width;
                ptr->dim.image.height = height;
                status = VX_SUCCESS;
            }
            vxReleaseParameter(&param2);
            vxReleaseParameter(&param);
        }
    }
    return status;
}

static vx_param_description_t magnitude_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t magnitude_kernel = {
    VX_KERNEL_MAGNITUDE,
    "org.khronos.openvx.magnitude",
    vxMagnitudeKernel,
    magnitude_kernel_params, dimof(magnitude_kernel_params),
    vxMagnitudeInputValidator,
    vxMagnitudeOutputValidator,
    NULL,
    NULL,
};

