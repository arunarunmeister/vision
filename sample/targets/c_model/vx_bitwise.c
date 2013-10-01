/*
 * Copyright (c) 2013 The Khronos Group Inc.
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
 * \brief The Bitwise Kernels: And, Or, Xor, Not.
 * \author Hans-Peter Nilsson <hp@axis.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>

/*
 * The three bitwise kernels with binary parameters have the same parameter domain so
 * let's just have one set of validators.
 */

static vx_status vxBinaryBitwiseInputValidator(vx_node node, vx_uint32 index)
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
            if (width[0] == width[1] && height[0] == height[1] && format[0] == format[1])
                status = VX_SUCCESS;
        }
        vxReleaseParameter(&param[0]);
        vxReleaseParameter(&param[1]);
    }
    return status;
}

static vx_status vxBinaryBitwiseOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 2)
    {
        vx_parameter param0 = vxGetParameterByIndex(node, 0);
        if (param0)
        {
            vx_image image0 = 0;
            vxQueryParameter(param0, VX_PARAMETER_ATTRIBUTE_REF, &image0, sizeof(image0));
            /*
             * When passing on the geometry to the output image, we only look at image 0, as
             * both input images are verified to match, at input validation.
             */
            if (image0)
            {
                vx_uint32 width = 0, height = 0;
                vxQueryImage(image0, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
                vxQueryImage(image0, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
                ptr->type = VX_TYPE_IMAGE;
                ptr->dim.image.format = FOURCC_U8;
                ptr->dim.image.width = width;
                ptr->dim.image.height = height;
                status = VX_SUCCESS;
            }
            vxReleaseParameter(&param0);
        }
    }
    return status;
}

static vx_status vx_binary_u8_kernel(vx_node node, vx_reference *parameters, vx_uint32 num,
                                     vx_uint8 (op)(vx_uint8, vx_uint8))
{
    vx_status status = VX_FAILURE;
    if (num == 3)
    {
        vx_image in1 = (vx_image)parameters[0];
        vx_image in2 = (vx_image)parameters[1];
        vx_image output = (vx_image)parameters[2];
        vx_uint32 y, x, width = 0, height = 0;
        void *dst_base   = NULL;
        void *src_base[2] = {NULL, NULL};
        vx_imagepatch_addressing_t dst_addr, src_addr[2];
        vx_rectangle rect;

        rect = vxGetValidRegionImage(in1);
        status = VX_SUCCESS;
        status |= vxAccessImagePatch(in1, rect, 0, &src_addr[0], (void **)&src_base[0]);
        status |= vxAccessImagePatch(in2, rect, 0, &src_addr[1], (void **)&src_base[1]);
        status |= vxAccessImagePatch(output, rect, 0, &dst_addr, (void **)&dst_base);
        width = src_addr[0].dim_x;
        height = src_addr[0].dim_y;
        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                vx_uint8 *src[2] = {
                    vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]),
                    vxFormatImagePatchAddress2d(src_base[1], x, y, &src_addr[1]),
                };
                vx_uint8 *dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);

                *dst = op(*src[0], *src[1]);
            }
        }
        status |= vxCommitImagePatch(in1, 0, 0, &src_addr[0], src_base[0]);
        status |= vxCommitImagePatch(in2, 0, 0, &src_addr[1], src_base[1]);
        status |= vxCommitImagePatch(output, rect, 0, &dst_addr, dst_base);
        vxReleaseRectangle(&rect);
    }
    return status;
}

static vx_param_description_t binary_bitwise_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

static vx_uint8 vx_and_op(vx_uint8 a, vx_uint8 b)
{
    return a & b;
}

static vx_status vxAndKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    return vx_binary_u8_kernel(node, parameters, num, vx_and_op);
}

vx_kernel_description_t and_kernel = {
    VX_KERNEL_AND,
    "org.khronos.openvx.and",
    vxAndKernel,
    binary_bitwise_kernel_params, dimof(binary_bitwise_kernel_params),
    vxBinaryBitwiseInputValidator,
    vxBinaryBitwiseOutputValidator,
    NULL,
    NULL,
};

static vx_uint8 vx_or_op(vx_uint8 a, vx_uint8 b)
{
    return a | b;
}

static vx_status vxOrKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    return vx_binary_u8_kernel(node, parameters, num, vx_or_op);
}

vx_kernel_description_t or_kernel = {
    VX_KERNEL_OR,
    "org.khronos.openvx.or",
    vxOrKernel,
    binary_bitwise_kernel_params, dimof(binary_bitwise_kernel_params),
    vxBinaryBitwiseInputValidator,
    vxBinaryBitwiseOutputValidator,
    NULL,
    NULL,
};

static vx_uint8 vx_xor_op(vx_uint8 a, vx_uint8 b)
{
    return a ^ b;
}

static vx_status vxXorKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    return vx_binary_u8_kernel(node, parameters, num, vx_xor_op);
}

vx_kernel_description_t xor_kernel = {
    VX_KERNEL_XOR,
    "org.khronos.openvx.xor",
    vxXorKernel,
    binary_bitwise_kernel_params, dimof(binary_bitwise_kernel_params),
    vxBinaryBitwiseInputValidator,
    vxBinaryBitwiseOutputValidator,
    NULL,
    NULL,
};

/* The Not kernel is an unary operator, requiring separate validators. */

static vx_status vxUnaryBitwiseInputValidator(vx_node node, vx_uint32 index)
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
                status = VX_SUCCESS;
        }
        vxReleaseParameter(&param);
    }
    return status;
}

static vx_status vxUnaryBitwiseOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 1)
    {
        vx_parameter param = vxGetParameterByIndex(node, 0);
        if (param)
        {
            vx_image inimage = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &inimage, sizeof(inimage));
            if (inimage)
            {
                vx_uint32 width = 0, height = 0;
                vxQueryImage(inimage, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
                vxQueryImage(inimage, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
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

static vx_param_description_t unary_bitwise_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};


static vx_status vxNotKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 2)
    {
        vx_image input = (vx_image)parameters[0];
        vx_image output = (vx_image)parameters[1];
        vx_uint32 y, x, width = 0, height = 0;
        void *dst_base = NULL;
        void *src_base = NULL;
        vx_imagepatch_addressing_t dst_addr, src_addr;
        vx_rectangle rect;

        rect = vxGetValidRegionImage(input);
        status = VX_SUCCESS;
        status |= vxAccessImagePatch(input, rect, 0, &src_addr, (void **)&src_base);
        status |= vxAccessImagePatch(output, rect, 0, &dst_addr, (void **)&dst_base);
        height = src_addr.dim_y;
        width = src_addr.dim_x;
        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                vx_uint8 *src = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                vx_uint8 *dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);

                *dst = ~*src;
            }
        }
        status |= vxCommitImagePatch(input, 0, 0, &src_addr, src_base);
        status |= vxCommitImagePatch(output, rect, 0, &dst_addr, dst_base);
        vxReleaseRectangle(&rect);
    }
    return status;
}

vx_kernel_description_t not_kernel = {
    VX_KERNEL_NOT,
    "org.khronos.openvx.not",
    vxNotKernel,
    unary_bitwise_kernel_params, dimof(unary_bitwise_kernel_params),
    vxUnaryBitwiseInputValidator,
    vxUnaryBitwiseOutputValidator,
    NULL,
    NULL,
};