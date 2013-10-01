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
 * \brief The Mean and Standard Deviation Kernel
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>

static vx_status vxMeanStdDevKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 3)
    {
        vx_image input = (vx_image)parameters[0];
        vx_scalar s_mean = (vx_scalar)parameters[1];
        vx_scalar s_stddev = (vx_scalar)parameters[2];
        vx_float32 mean = 0.0f, sum = 0.0f, sum_diff_sqrs = 0.0f, stddev = 0.0f;
        vx_fourcc format = 0;
        vx_rectangle rect = 0;
        vx_imagepatch_addressing_t addrs;
        void *base_ptr = NULL;
        vx_uint32 x, y;

        vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
        rect = vxGetValidRegionImage(input);
        status  = VX_SUCCESS;
        status |= vxAccessImagePatch(input, rect, 0, &addrs, &base_ptr);
        for (y = 0; y < addrs.dim_y; y++)
        {
            for (x = 0; x < addrs.dim_x; x++)
            {
                if (format == FOURCC_U8)
                {
                    vx_uint8 *pixel = vxFormatImagePatchAddress2d(base_ptr, x, y, &addrs);
                    sum += (vx_float32)(*pixel);
                }
                else if (format == FOURCC_U16)
                {
                    vx_uint16 *pixel = vxFormatImagePatchAddress2d(base_ptr, x, y, &addrs);
                    sum += (vx_float32)(*pixel);
                }
            }
        }
        mean = sum / (addrs.dim_x*addrs.dim_y);
        for (y = 0; y < addrs.dim_y; y++)
        {
            for (x = 0; x < addrs.dim_x; x++)
            {
                if (format == FOURCC_U8)
                {
                    vx_uint8 *pixel = vxFormatImagePatchAddress2d(base_ptr, x, y, &addrs);
                    sum_diff_sqrs += (vx_float32)pow((vx_float32)(*pixel) - mean, 2);
                }
                else if (format == FOURCC_U16)
                {
                    vx_uint16 *pixel = vxFormatImagePatchAddress2d(base_ptr, x, y, &addrs);
                    sum_diff_sqrs += (vx_float32)pow((vx_float32)(*pixel) - mean, 2);
                }
            }
        }
        stddev = (vx_float32)sqrt(sum_diff_sqrs / (addrs.dim_x*addrs.dim_y));
        status |= vxCommitScalarValue(s_mean, &mean);
        status |= vxCommitScalarValue(s_stddev, &stddev);
        status |= vxCommitImagePatch(input, rect, 0, &addrs, base_ptr);
    }
    else
    {
        status = VX_ERROR_INVALID_PARAMETERS;
    }
    return status;
}

static vx_status vxMeanStdDevInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        vx_image image = 0;
        vx_fourcc format = 0;

        vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &image, sizeof(image));
        if (image == 0)
        {
            status = VX_ERROR_INVALID_PARAMETERS;
        }
        else
        {
            status = VX_SUCCESS;
            vxQueryImage(image, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
            if (format != FOURCC_U8 && format != FOURCC_U16)
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
        }
        vxReleaseParameter(&param);
    }
    return status;
}

static vx_status vxMeanStdDevOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 1 || index == 2)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_scalar output = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &output, sizeof(output));
            if (output)
            {
                vx_enum type = 0;
                vxQueryScalar(output, VX_SCALAR_ATTRIBUTE_TYPE, &type, sizeof(type));
                if (type == VX_TYPE_FLOAT32)
                {
                    ptr->type = VX_TYPE_SCALAR;
                    ptr->dim.scalar.type = type;
                    status = VX_SUCCESS;
                }
            }
            vxReleaseParameter(&param);
        }
    }
    VX_PRINT(VX_ZONE_API, "%s:%u returned %d\n", __FUNCTION__, index, status);
    return status;
}


/*! \brief Declares the parameter types for \ref vxuColorConvert.
 * \ingroup group_implementation
 */
static vx_param_description_t mean_stddev_kernel_params[] = {
    {VX_INPUT,  VX_TYPE_IMAGE,   VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
};

/*! \brief The exported kernel table entry */
vx_kernel_description_t mean_stddev_kernel = {
    VX_KERNEL_MEAN_STDDEV,
    "org.khronos.openvx.mean_stddev",
    vxMeanStdDevKernel,
    mean_stddev_kernel_params, dimof(mean_stddev_kernel_params),
    vxMeanStdDevInputValidator,
    vxMeanStdDevOutputValidator,
    NULL,
    NULL,
};

