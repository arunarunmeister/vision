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
 * \brief The Custom Convolution Kernel
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>

static vx_status vxConvolveKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 3)
    {
        vx_image src = (vx_image)parameters[0];
        vx_convolution conv = (vx_convolution)parameters[1];
        vx_image dst = (vx_image)parameters[2];
        vx_uint32 y, x, j, i, x1, y1;
        void *src_base = NULL;
        void *dst_base = NULL;
        vx_imagepatch_addressing_t src_addr, dst_addr;
        vx_rectangle rect;
        vx_size dims[2] = {0,0};
        vx_size dims2[2] = {0,0};
        vx_int16 mat[VX_CONVOLUTION_MAX_DIM * VX_CONVOLUTION_MAX_DIM];
        vx_int32 sum = 0, value = 0;
        vx_uint32 div = 1;
        vx_fourcc format = 0;
        vx_fourcc format2 = 0;
        vx_border_mode_t borders = {VX_BORDER_MODE_UNDEFINED, 0};

        memset(mat, 0, sizeof(mat));
        status  = VX_SUCCESS;
        status |= vxQueryImage(src, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
        status |= vxQueryImage(dst, VX_IMAGE_ATTRIBUTE_FORMAT, &format2, sizeof(format2));
        status |= vxQueryConvolution(conv, VX_CONVOLUTION_ATTRIBUTE_COLUMNS, &dims[0], sizeof(dims[0]));
        status |= vxQueryConvolution(conv, VX_CONVOLUTION_ATTRIBUTE_ROWS, &dims[1], sizeof(dims[1]));
        status |= vxQueryConvolution(conv, VX_CONVOLUTION_ATTRIBUTE_SCALE, &div, sizeof(div));
        dims2[0] = (vx_uint32)dims[0] / 2;
        dims2[1] = (vx_uint32)dims[1] / 2;
        status |= vxAccessConvolutionCoefficients(conv, mat);
        rect = vxGetValidRegionImage(src);
        status |= vxAccessImagePatch(src, rect, 0, &src_addr, &src_base);
        status |= vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
        status |= vxQueryNode(node, VX_NODE_ATTRIBUTE_BORDER_MODE, &borders, sizeof(borders));
        /*! \todo Implement other border modes */
        if (borders.mode == VX_BORDER_MODE_UNDEFINED)
        {
            /* shrink the image by 1 */
            vxAlterRectangle(rect, dims2[0], dims2[1], -dims2[0], -dims2[1]);

            for (y = (vx_uint32)dims2[1]; (y < (src_addr.dim_y - (vx_uint32)dims2[1])) && (status == VX_SUCCESS); y++)
            {
                for (x = (vx_uint32)dims2[0]; x < (src_addr.dim_x - (vx_uint32)dims2[0]); x++)
                {
                    if (format == FOURCC_U8 && format2 == FOURCC_U8)
                    {
                        vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                        sum = 0;
                        for (j = 0, y1 = (y - (vx_uint32)dims2[1]); j < (vx_uint32)dims[1]; j++, y1++)
                        {
                            for (i = 0, x1 = (x - (vx_uint32)dims2[0]); i < (vx_uint32)dims[0]; i++, x1++)
                            {
                                vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, x1, y1, &src_addr);
                                sum += mat[j*dims[1] + i] * (*srcp);
                            }
                        }
                        value = sum / div;
                        if (value > UINT8_MAX)
                            value = UINT8_MAX;
                        *dstp = (vx_uint8)value;
                    }
                    else if (format == FOURCC_U8 && format2 == FOURCC_S16)
                    {
                        vx_int16 *dstp = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                        sum = 0;
                        for (j = 0, y1 = (y - (vx_uint32)dims2[1]); j < (vx_uint32)dims[1]; j++, y1++)
                        {
                            for (i = 0, x1 = (x - (vx_uint32)dims2[0]); i < (vx_uint32)dims[0]; i++, x1++)
                            {
                                vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, x1, y1, &src_addr);
                                sum += mat[j*dims[1] + i] * (*srcp);
                            }
                        }
                        value = sum / div;
                        if (value > INT16_MAX)
                            value = INT16_MAX;
                        else if (value < INT16_MIN)
                            value = INT16_MIN;
                        *dstp = (vx_int16)value;
                    }
                    else if (format == FOURCC_S16)
                    {
                        vx_int16 *dstp = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                        sum = 0;
                        for (j = 0, y1 = (y - (vx_uint32)dims2[1]); j < (vx_uint32)dims[1]; j++, y1++)
                        {
                            for (i = 0, x1 = (x - (vx_uint32)dims2[0]); i < (vx_uint32)dims[0]; i++, x1++)
                            {
                                vx_int16 *srcp = vxFormatImagePatchAddress2d(src_base, x1, y1, &src_addr);
                                sum += mat[j*dims[1] + i] * (*srcp);
                            }
                        }
                        value = sum / div;
                        if (value > INT16_MAX)
                            value = INT16_MAX;
                        else if (value < INT16_MIN)
                            value = INT16_MIN;
                        *dstp = (vx_int16)value;
                    }
                }
            }
        }
        else
        {
            status = VX_ERROR_NOT_IMPLEMENTED;
        }
        status |= vxCommitConvolutionCoefficients(conv, NULL);
        status |= vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
        status |= vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
        vxReleaseRectangle(&rect);
    }
    return status;
}

static vx_status vxConvolveInputValidator(vx_node node, vx_uint32 index)
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
            vx_uint32 width = 0, height = 0;
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
            if ((width > VX_CONVOLUTION_MAX_DIM) &&
                (height > VX_CONVOLUTION_MAX_DIM) &&
                ((format == FOURCC_U8) ||
                 (format == FOURCC_S16)))
            {
                status = VX_SUCCESS;
            }
        }
        vxReleaseParameter(&param);
    }
    if (index == 1)
    {
        vx_convolution conv = 0;
        vx_parameter param = vxGetParameterByIndex(node, index);

        vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &conv, sizeof(conv));
        if (conv)
        {
            vx_fourcc dims[2] = {0,0};
            vxQueryConvolution(conv, VX_CONVOLUTION_ATTRIBUTE_COLUMNS, &dims[0], sizeof(dims[0]));
            vxQueryConvolution(conv, VX_CONVOLUTION_ATTRIBUTE_ROWS, &dims[1], sizeof(dims[1]));
            if ((dims[0] <= VX_CONVOLUTION_MAX_DIM) &&
                (dims[1] <= VX_CONVOLUTION_MAX_DIM))
            {
                status = VX_SUCCESS;
            }
        }
        vxReleaseParameter(&param);
    }
    return status;
}

static vx_status vxConvolveOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 2)
    {
        vx_parameter params[2] = {
            vxGetParameterByIndex(node, 0),
            vxGetParameterByIndex(node, index),
        };
        if (params[0] && params[1])
        {
            vx_image input = 0;
            vx_image output = 0;
            vxQueryParameter(params[0], VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
            vxQueryParameter(params[1], VX_PARAMETER_ATTRIBUTE_REF, &output, sizeof(output));
            if (input && output)
            {
                vx_uint32 width = 0, height = 0;
                vx_fourcc format = 0;
                vx_fourcc format2 = 0;
                vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
                vxQueryImage(input, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
                vxQueryImage(input, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));

                vxQueryImage(output, VX_IMAGE_ATTRIBUTE_FORMAT, &format2, sizeof(format2));
                if (format == FOURCC_U8 || format == FOURCC_S16)
                {
                    ptr->type = VX_TYPE_IMAGE;
                    ptr->dim.image.format = FOURCC_S16;
                    ptr->dim.image.width = width;
                    ptr->dim.image.height = height;
                    status = VX_SUCCESS;
                }
            }
            vxReleaseParameter(&params[0]);
            vxReleaseParameter(&params[1]);
        }
    }
    return status;
}

static vx_param_description_t convolution_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_CONVOLUTION, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t convolution_kernel = {
    VX_KERNEL_CUSTOM_CONVOLUTION,
    "org.khronos.openvx.custom_convolution",
    vxConvolveKernel,
    convolution_kernel_params, dimof(convolution_kernel_params),
    vxConvolveInputValidator,
    vxConvolveOutputValidator,
    NULL,
    NULL,
};

