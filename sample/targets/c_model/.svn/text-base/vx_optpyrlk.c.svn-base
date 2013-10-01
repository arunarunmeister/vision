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
 * \brief The Absolute Difference Kernel.
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>

static vx_status vxOpticalFlowPyrLKKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 4)
    {
        /*! \todo Implement Optical Flow Pyramid LK. */
        status = VX_ERROR_NOT_IMPLEMENTED;
    }
    return status;
}

static vx_status vxOpticalFlowPyrLKInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0 || index == 1)
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
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_list list = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &list, sizeof(list));
            if (list)
            {
                vx_enum type = 0;
                vxQueryList(list, VX_LIST_ATTRIBUTE_TYPE, &type, sizeof(type));
                if (type == VX_TYPE_COORDINATES)
                {
                    status = VX_SUCCESS;
                }
            }
        }
        vxReleaseParameter(&param);
    }
    return status;
}

static vx_status vxOpticalFlowPyrLKOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 3)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_list list = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &list, sizeof(list));
            if (list)
            {
                vx_enum type = 0;
                vxQueryList(list, VX_LIST_ATTRIBUTE_TYPE, &type, sizeof(type));
                if (type == VX_TYPE_COORDINATES)
                {
                    ptr->type = VX_TYPE_LIST;
                    ptr->dim.list.type = type;
                    ptr->dim.list.initial = 1000; // guess
                    status = VX_SUCCESS;
                }
            }
        }
        vxReleaseParameter(&param);
    }
    return status;
}

static vx_param_description_t optpyrlk_kernel_params[] = {
    {VX_INPUT, VX_TYPE_PYRAMID, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_PYRAMID, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_LIST, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_LIST, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_ENUM, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SIZE, VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t optpyrlk_kernel = {
    VX_KERNEL_OPTICAL_FLOW_PYR_LK,
    "org.khronos.openvx.opticalflow_pyr_lk",
    vxOpticalFlowPyrLKKernel,
    optpyrlk_kernel_params, dimof(optpyrlk_kernel_params),
    vxOpticalFlowPyrLKInputValidator,
    vxOpticalFlowPyrLKOutputValidator,
    NULL,
    NULL,
};

