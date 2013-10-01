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
 * \brief The Gradient Kernels (Base)
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>

static vx_status vxSobel3x3Kernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 3)
    {
        vx_image input  = (vx_image)parameters[0];
        vx_image grad_x = (vx_image)parameters[1];
        vx_image grad_y = (vx_image)parameters[2];
        vx_uint32 i, y, x;
        vx_uint8 *src_base   = NULL;
        vx_int16 *dst_base_x = NULL;
        vx_int16 *dst_base_y = NULL;
        vx_imagepatch_addressing_t src_addr, dst_addr_x, dst_addr_y;
        vx_int16 ops[] = {1,2,1,-1,-2,-1};
        vx_rectangle rect;
        vx_border_mode_t borders = {VX_BORDER_MODE_UNDEFINED, 0};

        if ((grad_x == 0) && (grad_y == 0))
        {
            return VX_ERROR_INVALID_PARAMETERS;
        }

        rect = vxGetValidRegionImage(input);
        status = vxAccessImagePatch(input, rect, 0, &src_addr, (void **)&src_base);
        if (grad_x)
            status |= vxAccessImagePatch(grad_x, rect, 0, &dst_addr_x, (void **)&dst_base_x);
        if (grad_y)
            status |= vxAccessImagePatch(grad_y, rect, 0, &dst_addr_y, (void **)&dst_base_y);
        status |= vxQueryNode(node, VX_NODE_ATTRIBUTE_BORDER_MODE, &borders, sizeof(borders));
        /*! \todo Implement other border modes */
        if (borders.mode == VX_BORDER_MODE_UNDEFINED)
        {
            /* shrink the image by 1 */
            vxAlterRectangle(rect, 1, 1, -1, -1);

            /* blank edges on output */
            for (y = 1; y < (src_addr.dim_y - 1); y++)
            {
                for (x = 1; x < (src_addr.dim_x - 1); x++)
                {
                    vx_uint8 *gdx[] = {vxFormatImagePatchAddress2d(src_base, x+1, y-1, &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x+1, y,   &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x+1, y+1, &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x-1, y-1, &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x-1, y,   &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x-1, y+1, &src_addr)};

                    vx_uint8 *gdy[] = {vxFormatImagePatchAddress2d(src_base, x-1, y+1, &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x,   y+1, &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x+1, y+1, &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x-1, y-1, &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x,   y-1, &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x+1, y-1, &src_addr)};

                    vx_int16 *out_x = vxFormatImagePatchAddress2d(dst_base_x, x, y, &dst_addr_x);
                    vx_int16 *out_y = vxFormatImagePatchAddress2d(dst_base_y, x, y, &dst_addr_y);

                    if (out_x) {
                        *out_x = 0;
                        for (i = 0; i < dimof(gdx); i++)
                            *out_x += (ops[i] * gdx[i][0]);
                    }
                    if (out_y) {
                        *out_y = 0;
                        for (i = 0; i < dimof(gdy); i++)
                            *out_y += (ops[i] * gdy[i][0]);
                    }
                }
            }
        }
        else
        {
            status = VX_ERROR_NOT_IMPLEMENTED;
        }
        status = vxCommitImagePatch(input, 0, 0, &src_addr, src_base);
        if (grad_x)
            status |= vxCommitImagePatch(grad_x, rect, 0, &dst_addr_x, dst_base_x);
        if (grad_y)
            status |= vxCommitImagePatch(grad_y, rect, 0, &dst_addr_y, dst_base_y);
        vxReleaseRectangle(&rect);
    }
    return status;
}

static vx_status vxGradientInputValidator(vx_node node, vx_uint32 index)
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
            if (width >= 3 && height >= 3 && format == FOURCC_U8)
                status = VX_SUCCESS;
        }
        vxReleaseParameter(&param);
    }
    return status;
}

static vx_status vxGradientOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 1 || index == 2)
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
                ptr->dim.image.format = FOURCC_S16;
                ptr->dim.image.width = width;
                ptr->dim.image.height = height;
                status = VX_SUCCESS;
            }
            vxReleaseParameter(&param);
        }
    }
    return status;
}

static vx_param_description_t gradient_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_OPTIONAL},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_OPTIONAL},
};

vx_kernel_description_t sobel3x3_kernel = {
    VX_KERNEL_SOBEL_3x3,
    "org.khronos.openvx.sobel3x3",
    vxSobel3x3Kernel,
    gradient_kernel_params, dimof(gradient_kernel_params),
    vxGradientInputValidator,
    vxGradientOutputValidator,
    NULL,
    NULL,
};


