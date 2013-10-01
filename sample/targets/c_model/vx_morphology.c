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
 * \brief The Morphology kernels.
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>

static vx_status vxErode3x3Kernel(vx_node node, vx_reference *parameters, vx_uint32 num)
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
        if (borders.mode == VX_BORDER_MODE_UNDEFINED)
        {
            /* shrink by 1 */
            vxAlterRectangle(rect, 1, 1, -1, -1);

            for (y = 1; (y < (src_addr.dim_y - 1)) && (status == VX_SUCCESS); y++)
            {
                for (x = 1; x < (src_addr.dim_x - 1); x++)
                {
                    vx_uint8 *srcp[9] = {vxFormatImagePatchAddress2d(src_base, x-1, y-1, &src_addr),
                                         vxFormatImagePatchAddress2d(src_base, x+0, y-1, &src_addr),
                                         vxFormatImagePatchAddress2d(src_base, x+1, y-1, &src_addr),
                                         vxFormatImagePatchAddress2d(src_base, x-1, y+0, &src_addr),
                                         vxFormatImagePatchAddress2d(src_base, x+0, y+0, &src_addr),
                                         vxFormatImagePatchAddress2d(src_base, x+1, y+0, &src_addr),
                                         vxFormatImagePatchAddress2d(src_base, x-1, y+1, &src_addr),
                                         vxFormatImagePatchAddress2d(src_base, x+0, y+1, &src_addr),
                                         vxFormatImagePatchAddress2d(src_base, x+1, y+1, &src_addr)};
                    vx_uint8 *dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    vx_uint8 pixels[] = {
                        *srcp[0], *srcp[1], *srcp[2],
                        *srcp[3], *srcp[4], *srcp[5],
                        *srcp[6], *srcp[7], *srcp[8]
                    };
                    vx_uint32 i = 0;
                    vx_uint8 m = *srcp[4]; /* start with original pixel */
                    for (i = 0; i < dimof(pixels); i++)
                    {
                        if (pixels[i] < m)
                        {
                            m = pixels[i];
                        }
                    }
                    *dst = m;
                }
            }
        }
        status |= vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
        status |= vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
        vxReleaseRectangle(&rect);
    }
    return status;
}


static vx_status vxDilate3x3Kernel(vx_node node, vx_reference *parameters, vx_uint32 num)
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
        if (borders.mode == VX_BORDER_MODE_UNDEFINED)
        {
            /* shrink by 1 */
            vxAlterRectangle(rect, 1, 1, -1, -1);

            for (y = 1; (y < (src_addr.dim_y - 1)) && (status == VX_SUCCESS); y++)
            {
                for (x = 1; x < (src_addr.dim_x - 1); x++)
                {
                    vx_uint8 *srcp[9] = {vxFormatImagePatchAddress2d(src_base, x-1, y-1, &src_addr),
                                         vxFormatImagePatchAddress2d(src_base, x+0, y-1, &src_addr),
                                         vxFormatImagePatchAddress2d(src_base, x+1, y-1, &src_addr),
                                         vxFormatImagePatchAddress2d(src_base, x-1, y+0, &src_addr),
                                         vxFormatImagePatchAddress2d(src_base, x+0, y+0, &src_addr),
                                         vxFormatImagePatchAddress2d(src_base, x+1, y+0, &src_addr),
                                         vxFormatImagePatchAddress2d(src_base, x-1, y+1, &src_addr),
                                         vxFormatImagePatchAddress2d(src_base, x+0, y+1, &src_addr),
                                         vxFormatImagePatchAddress2d(src_base, x+1, y+1, &src_addr)};
                    vx_uint8 *dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    vx_uint8 pixels[] = {
                        *srcp[0], *srcp[1], *srcp[2],
                        *srcp[3], *srcp[4], *srcp[5],
                        *srcp[6], *srcp[7], *srcp[8]
                    };
                    vx_uint32 i = 0;
                    vx_uint8 m = *srcp[4]; /* start with original pixel */
                    for (i = 0; i < dimof(pixels); i++)
                    {
                        if (pixels[i] > m)
                        {
                            m = pixels[i];
                        }
                    }
                    *dst = m;
                }
            }
        }
        status |= vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
        status |= vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
        vxReleaseRectangle(&rect);
    }
    return status;
}

static vx_status vxMorphologyInputValidator(vx_node node, vx_uint32 index)
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

static vx_status vxMorphologyOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
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
                ptr->dim.image.format = FOURCC_U8;
                ptr->dim.image.width = width;
                ptr->dim.image.height = height;
                status = VX_SUCCESS;
            }
            vxReleaseParameter(&param);
        }
    }
    return status;
}

static vx_param_description_t morphology_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t erode3x3_kernel = {
    VX_KERNEL_ERODE_3x3,
    "org.khronos.openvx.erode3x3",
    vxErode3x3Kernel,
    morphology_kernel_params, dimof(morphology_kernel_params),
    vxMorphologyInputValidator,
    vxMorphologyOutputValidator,
    NULL,
    NULL,
};

vx_kernel_description_t dilate3x3_kernel = {
    VX_KERNEL_DILATE_3x3,
    "org.khronos.openvx.dilate3x3",
    vxDilate3x3Kernel,
    morphology_kernel_params, dimof(morphology_kernel_params),
    vxMorphologyInputValidator,
    vxMorphologyOutputValidator,
    NULL,
    NULL,
};


