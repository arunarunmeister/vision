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
 * \brief The Compare Object Kernels.
 * \author Erik Rainey <erik.rainey@ti.com>
 * \defgroup group_debug_ext Debugging Extension
 */

#include <VX/vx.h>
#include <VX/vx_ext_debug.h>
#include <VX/vx_helper.h>

vx_status vxCompareImagesKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 3)
    {
        vx_parameter a_param = vxGetParameterByIndex(node, 0);
        vx_parameter b_param = vxGetParameterByIndex(node, 1);
        vx_parameter param_d = vxGetParameterByIndex(node, 2);
        vx_image a,b;
        vx_scalar diffs;
        vx_uint32 numDiffs = 0u;
        vx_uint32 a_planes, a_width, a_height;
        vx_fourcc a_format;
        vx_uint32 b_planes, b_width, b_height;
        vx_fourcc b_format;

        vxQueryParameter(a_param, VX_PARAMETER_ATTRIBUTE_REF, &a, sizeof(a));
        vxQueryParameter(b_param, VX_PARAMETER_ATTRIBUTE_REF, &b, sizeof(b));
        vxQueryParameter(param_d, VX_PARAMETER_ATTRIBUTE_REF, &diffs, sizeof(diffs));

        vxQueryImage(a, VX_IMAGE_ATTRIBUTE_FORMAT, &a_format, sizeof(a_format));
        vxQueryImage(b, VX_IMAGE_ATTRIBUTE_FORMAT, &b_format, sizeof(b_format));
        vxQueryImage(a, VX_IMAGE_ATTRIBUTE_PLANES, &a_planes, sizeof(a_planes));
        vxQueryImage(b, VX_IMAGE_ATTRIBUTE_PLANES, &b_planes, sizeof(b_planes));
        vxQueryImage(a, VX_IMAGE_ATTRIBUTE_WIDTH,  &a_width,  sizeof(a_width));
        vxQueryImage(b, VX_IMAGE_ATTRIBUTE_WIDTH,  &b_width,  sizeof(b_width));
        vxQueryImage(a, VX_IMAGE_ATTRIBUTE_HEIGHT, &a_height, sizeof(a_height));
        vxQueryImage(b, VX_IMAGE_ATTRIBUTE_HEIGHT, &b_height, sizeof(b_height));

        if ((a_planes == b_planes) &&
            (a_format == b_format) &&
            (a_width  == b_width)  &&
            (a_height == b_height))
        {
            vx_uint32 x, y, p;
            vx_int32 i;
            void *a_base_ptrs[4] = {NULL, NULL, NULL, NULL};
            void *b_base_ptrs[4] = {NULL, NULL, NULL, NULL};
            vx_imagepatch_addressing_t a_addrs[4];
            vx_imagepatch_addressing_t b_addrs[4];
            vx_rectangle rect = vxCreateRectangle(vxGetContext(a), 0, 0, a_width, a_height);

            status = VX_SUCCESS;
            for (p = 0; p < a_planes; p++)
            {
                status |= vxAccessImagePatch(a, rect, p, &a_addrs[p], &a_base_ptrs[p]);
                status |= vxAccessImagePatch(b, rect, p, &b_addrs[p], &b_base_ptrs[p]);
            }
            if (status != VX_SUCCESS)
            {
                vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Failed to get patch on a and/or b\n");
            }
            for (p = 0; p < a_planes; p++)
            {
                for (y = 0; y < a_height; y++)
                {
                    for (x = 0; x < a_width; x++)
                    {
                        uint8_t *a_ptr = vxFormatImagePatchAddress2d(a_base_ptrs[p], x, y, &a_addrs[p]);
                        uint8_t *b_ptr = vxFormatImagePatchAddress2d(b_base_ptrs[p], x, y, &b_addrs[p]);
                        for (i = 0; i < a_addrs[p].stride_x; i++)
                        {
                            if (a_ptr[i] != b_ptr[i])
                            {
                                numDiffs++;
                            }
                        }
                    }
                }
            }
            vxCommitScalarValue(diffs, &numDiffs);
            for (p = 0; p < a_planes; p++)
            {
                status |= vxCommitImagePatch(a, 0, p, &a_addrs[p], a_base_ptrs[p]);
                status |= vxCommitImagePatch(b, 0, p, &b_addrs[p], b_base_ptrs[p]);
            }
            vxReleaseRectangle(&rect);
        }
        if (numDiffs > 0)
        {
            vxAddLogEntry(vxGetContext(node), VX_FAILURE, "%u differences found\n", numDiffs);
            status = VX_FAILURE;
        }
    }
    return status;
}

static vx_status vxCompareInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0)
    {
        /* any width/height/format is ok */
        status = VX_SUCCESS;
    }
    if (index == 1)
    {
        vx_image inputs[2];
        vx_parameter params[] = {
            vxGetParameterByIndex(node, 0),
            vxGetParameterByIndex(node, 1),
        };
        if (params[0] && params[1])
        {
            vxQueryParameter(params[0], VX_PARAMETER_ATTRIBUTE_REF, &inputs[0], sizeof(inputs[0]));
            vxQueryParameter(params[1], VX_PARAMETER_ATTRIBUTE_REF, &inputs[1], sizeof(inputs[1]));
            if (inputs[0] && inputs[1])
            {
                vx_uint32 width[2], height[2];
                vx_fourcc format[2];

                vxQueryImage(inputs[0], VX_IMAGE_ATTRIBUTE_WIDTH, &width[0], sizeof(width[0]));
                vxQueryImage(inputs[0], VX_IMAGE_ATTRIBUTE_HEIGHT, &height[0], sizeof(height[0]));
                vxQueryImage(inputs[0], VX_IMAGE_ATTRIBUTE_FORMAT, &format[0], sizeof(format[0]));

                vxQueryImage(inputs[1], VX_IMAGE_ATTRIBUTE_WIDTH, &width[1], sizeof(width[1]));
                vxQueryImage(inputs[1], VX_IMAGE_ATTRIBUTE_HEIGHT, &height[1], sizeof(height[1]));
                vxQueryImage(inputs[1], VX_IMAGE_ATTRIBUTE_FORMAT, &format[1], sizeof(format[1]));

                if (width[0] == width[1] &&
                    height[0] == height[1] &&
                    format[0] == format[1])
                {
                    status = VX_SUCCESS;
                }
            }
        }
    }
    return status;
}

static vx_status vxCompareOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 2)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        ptr->dim.scalar.type = VX_TYPE_UINT32;
        if (param)
        {
            vx_scalar diffs;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &diffs, sizeof(diffs));
            if (diffs)
            {
                vx_enum type = 0;
                vxQueryScalar(diffs, VX_SCALAR_ATTRIBUTE_TYPE, &type, sizeof(type));
                if (type == VX_TYPE_UINT32)
                {
                    status = VX_SUCCESS;
                }
                else
                {
                    status = VX_ERROR_INVALID_TYPE;
                }
            }
        }
    }
    return status;
}

static vx_param_description_t compare_images_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t compareimage_kernel = {
    VX_KERNEL_COMPARE_IMAGE,
    "org.khronos.debug.compare_images",
    vxCompareImagesKernel,
    compare_images_params, dimof(compare_images_params),
    vxCompareInputValidator,
    vxCompareOutputValidator,
    NULL,
    NULL,
};
