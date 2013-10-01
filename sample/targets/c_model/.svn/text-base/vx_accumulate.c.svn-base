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
 * \brief The Accumulation Kernels
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>

static vx_status vxAccumulateKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 2)
    {
        vx_image input = (vx_image)parameters[0];
        vx_image accum = (vx_image)parameters[1];
        vx_uint32 y, x, width = 0, height = 0;
        void *dst_base = NULL;
        void *src_base = NULL;
        vx_imagepatch_addressing_t dst_addr, src_addr;
        vx_rectangle rect;

        rect = vxGetValidRegionImage(input);
        status = VX_SUCCESS;
        status |= vxAccessImagePatch(input, rect, 0, &src_addr, (void **)&src_base);
        status |= vxAccessImagePatch(accum, rect, 0, &dst_addr, (void **)&dst_base);
        width = src_addr.dim_x;
        height = src_addr.dim_y;
        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                vx_uint16 *dstp = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                *dstp += (vx_uint16)(*srcp);
            }
        }
        status |= vxCommitImagePatch(input, 0, 0, &src_addr, src_base);
        status |= vxCommitImagePatch(accum, rect, 0, &dst_addr, dst_base);
        vxReleaseRectangle(&rect);
    }
    return status;
}

static vx_status vxAccumulateWeightedKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 3)
    {
        vx_image input = (vx_image)parameters[0];
        vx_scalar scalar = (vx_scalar)parameters[1];
        vx_image accum = (vx_image)parameters[2];
        vx_uint32 y, x, width = 0, height = 0;
        void *dst_base = NULL;
        void *src_base = NULL;
        vx_imagepatch_addressing_t dst_addr, src_addr;
        vx_rectangle rect;
        vx_float32 alpha = 0.0f;

        vxQueryImage(input, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
        vxQueryImage(input, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
        rect = vxCreateRectangle(vxGetContext(node), 0, 0, width, height);
        status  = VX_SUCCESS;
        status |= vxAccessImagePatch(input, rect, 0, &src_addr, (void **)&src_base);
        status |= vxAccessImagePatch(accum, rect, 0, &dst_addr, (void **)&dst_base);
        status |= vxAccessScalarValue(scalar, &alpha);
        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                vx_uint16 *dstp = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                *dstp = (vx_uint16)(((1 - alpha) * (*dstp)) + ((alpha) * (vx_uint16)(*srcp)));
            }
        }
        status |= vxCommitImagePatch(input, 0, 0, &src_addr, src_base);
        status |= vxCommitImagePatch(accum, rect, 0, &dst_addr, dst_base);
        vxReleaseRectangle(&rect);
    }
    return status;
}

static vx_status vxAccumulateSquareKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 2)
    {
        vx_image input = (vx_image)parameters[0];
        vx_image accum = (vx_image)parameters[1];
        vx_uint32 y, x, width = 0, height = 0;
        void *dst_base = NULL;
        void *src_base = NULL;
        vx_imagepatch_addressing_t dst_addr, src_addr;
        vx_rectangle rect;

        vxQueryImage(input, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
        vxQueryImage(input, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
        rect = vxCreateRectangle(vxGetContext(node), 0, 0, width, height);
        status = VX_SUCCESS;
        status |= vxAccessImagePatch(input, rect, 0, &src_addr, (void **)&src_base);
        status |= vxAccessImagePatch(accum, rect, 0, &dst_addr, (void **)&dst_base);
        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                vx_uint16 *dstp = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                *dstp += ((vx_uint16)(*srcp) * (vx_uint16)(*srcp));
            }
        }
        status |= vxCommitImagePatch(input, 0, 0, &src_addr, src_base);
        status |= vxCommitImagePatch(accum, rect, 0, &dst_addr, dst_base);
        vxReleaseRectangle(&rect);
    }
    return status;
}

static vx_status vxAccumulateInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0 )
    {
        vx_image input = 0;
        vx_parameter param = vxGetParameterByIndex(node, index);

        vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
        if (input)
        {
            vx_fourcc format = 0;
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
            if (format == FOURCC_U8)
                status = VX_SUCCESS;
        }
        vxReleaseParameter(&param);
    }
    else if (index == 1)
    {
        vx_image images[2];
        vx_parameter param[2] = {
            vxGetParameterByIndex(node, 0),
            vxGetParameterByIndex(node, 1),
        };
        vxQueryParameter(param[0], VX_PARAMETER_ATTRIBUTE_REF, &images[0], sizeof(images[0]));
        vxQueryParameter(param[1], VX_PARAMETER_ATTRIBUTE_REF, &images[1], sizeof(images[1]));
        if (images[0] && images[1])
        {
            vx_uint32 width[2], height[2];
            vx_fourcc format[2];

            vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_WIDTH, &width[0], sizeof(width[0]));
            vxQueryImage(images[1], VX_IMAGE_ATTRIBUTE_WIDTH, &width[1], sizeof(width[1]));
            vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_HEIGHT, &height[0], sizeof(height[0]));
            vxQueryImage(images[1], VX_IMAGE_ATTRIBUTE_HEIGHT, &height[1], sizeof(height[1]));
            vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_FORMAT, &format[0], sizeof(format[0]));
            vxQueryImage(images[1], VX_IMAGE_ATTRIBUTE_FORMAT, &format[1], sizeof(format[1]));
            if (width[0] == width[1] &&
               height[0] == height[1] &&
               format[0] == FOURCC_U8 &&
               format[1] == FOURCC_U16)
            {
                status = VX_SUCCESS;
            }
        }
        vxReleaseParameter(&param[0]);
        vxReleaseParameter(&param[1]);
    }
    return status;
}


static vx_status vxAccumulateWeightedInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0 )
    {
        vx_image input = 0;
        vx_parameter param = vxGetParameterByIndex(node, index);

        vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
        if (input)
        {
            vx_fourcc format = 0;
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
            if (format == FOURCC_U8)
                status = VX_SUCCESS;
        }
        vxReleaseParameter(&param);
    }
    else if (index == 2)
    {
        vx_image images[2];
        vx_parameter param[2] = {
            vxGetParameterByIndex(node, 0),
            vxGetParameterByIndex(node, 2),
        };
        vxQueryParameter(param[0], VX_PARAMETER_ATTRIBUTE_REF, &images[0], sizeof(images[0]));
        vxQueryParameter(param[1], VX_PARAMETER_ATTRIBUTE_REF, &images[1], sizeof(images[1]));
        if (images[0] && images[1])
        {
            vx_uint32 width[2], height[2];
            vx_fourcc format[2];

            vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_WIDTH, &width[0], sizeof(width[0]));
            vxQueryImage(images[1], VX_IMAGE_ATTRIBUTE_WIDTH, &width[1], sizeof(width[1]));
            vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_HEIGHT, &height[0], sizeof(height[0]));
            vxQueryImage(images[1], VX_IMAGE_ATTRIBUTE_HEIGHT, &height[1], sizeof(height[1]));
            vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_FORMAT, &format[0], sizeof(format[0]));
            vxQueryImage(images[1], VX_IMAGE_ATTRIBUTE_FORMAT, &format[1], sizeof(format[1]));
            if (width[0] == width[1] &&
               height[0] == height[1] &&
               format[0] == FOURCC_U8 &&
               format[1] == FOURCC_U16)
            {
                status = VX_SUCCESS;
            }
        }
        vxReleaseParameter(&param[0]);
        vxReleaseParameter(&param[1]);
    }
    else if (index == 1) /* only weighted average */
    {
        vx_scalar scalar = 0;
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &scalar, sizeof(scalar));
            if (scalar)
            {
                vx_enum type = 0;
                vxQueryScalar(scalar, VX_SCALAR_ATTRIBUTE_TYPE, &type, sizeof(type));
                if (type == VX_TYPE_FLOAT32)
                {
                    vx_float32 alpha = 0.0f;
                    if ((vxAccessScalarValue(scalar, &alpha) == VX_SUCCESS) &&
                        (0.0f <= alpha) && (alpha <= 1.0f))
                    {
                        status = VX_SUCCESS;
                    }
                    else
                    {
                        status = VX_ERROR_INVALID_VALUE;
                    }
                }
                else
                {
                    status = VX_ERROR_INVALID_TYPE;
                }
            }
            vxReleaseParameter(&param);
        }
    }
    return status;
}

static vx_status vxAccumulateOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    return status;
}

static vx_param_description_t accumulate_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_BIDIRECTIONAL, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};


static vx_param_description_t accumulate_weighted_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_BIDIRECTIONAL, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};


vx_kernel_description_t accumulate_kernel = {
    VX_KERNEL_ACCUMULATE,
    "org.khronos.openvx.accumulate",
    vxAccumulateKernel,
    accumulate_kernel_params, dimof(accumulate_kernel_params),
    vxAccumulateInputValidator,
    vxAccumulateOutputValidator,
    NULL,
    NULL,
};

vx_kernel_description_t accumulate_weighted_kernel = {
    VX_KERNEL_ACCUMULATE_WEIGHTED,
    "org.khronos.openvx.accumulate_weighted",
    vxAccumulateWeightedKernel,
    accumulate_weighted_kernel_params, dimof(accumulate_weighted_kernel_params),
    vxAccumulateWeightedInputValidator,
    vxAccumulateOutputValidator,
    NULL,
    NULL,
};

vx_kernel_description_t accumulate_square_kernel = {
    VX_KERNEL_ACCUMULATE_SQUARE,
    "org.khronos.openvx.accumulate_square",
    vxAccumulateSquareKernel,
    accumulate_kernel_params, dimof(accumulate_kernel_params),
    vxAccumulateInputValidator,
    vxAccumulateOutputValidator,
    NULL,
    NULL,
};

