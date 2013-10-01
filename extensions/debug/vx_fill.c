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
 * \brief The Fill Object Kernels.
 * \author Erik Rainey <erik.rainey@ti.com>
 * \defgroup group_debug_ext Debugging Extension
 */

#include <VX/vx.h>
#include <VX/vx_ext_debug.h>
#include <VX/vx_helper.h>

static vx_status vxFillImageKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_SUCCESS;
    if (num == 2)
    {
        vx_scalar fill = (vx_scalar)parameters[0];
        vx_image image = (vx_image)parameters[1];
        vx_uint8 value[4] = {0xDE, 0xAD, 0xBE, 0xEF};
        vx_uint32 planes = 0u, count = 0u, x = 0u, y = 0u, p = 0u;
        vx_int32 i = 0;

        vxAccessScalarValue(fill, value);
        vxQueryImage(image, VX_IMAGE_ATTRIBUTE_PLANES, &planes, sizeof(planes));
        for (p = 0u; p < planes; p++)
        {
            void *ptr = NULL;
            vx_imagepatch_addressing_t addr;
            vx_rectangle rect = vxGetValidRegionImage(image);
            status = vxAccessImagePatch(image, rect, p, &addr, &ptr);
            if ((status == VX_SUCCESS) && (ptr) && (rect))
            {
                for (y = 0u; y < addr.dim_y; y+=addr.step_y)
                {
                    for (x = 0u; x < addr.dim_x; x+=addr.step_x)
                    {
                        vx_uint8 *pixel = vxFormatImagePatchAddress2d(ptr, x, y, &addr);
                        for (i = 0; i < addr.stride_x; i++)
                        {
                            pixel[i] = value[i];
                            count++;
                        }
                    }
                }
                status = vxCommitImagePatch(image, rect, p, &addr, ptr);
                if (status != VX_SUCCESS)
                {
                    vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Failed to set image patch for "VX_FMT_REF"\n", image);
                }
            }
            else
            {
                vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Failed to get image patch for "VX_FMT_REF"\n", image);
            }
            vxReleaseRectangle(&rect);
        }
    }
    return status;
}

static vx_status vxFillBufferKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_SUCCESS;
    if (num == 2)
    {
        vx_scalar fill = (vx_scalar)parameters[0];
        vx_buffer buffer = (vx_buffer)parameters[1];
        vx_uint8 value = 0u;
        vx_size numUnits = 0ul, unitSize = 0ul, size = 0ul;
        void *ptr = NULL;

        vxQueryBuffer(buffer, VX_BUFFER_ATTRIBUTE_NUMUNITS, &numUnits, sizeof(numUnits));
        vxQueryBuffer(buffer, VX_BUFFER_ATTRIBUTE_UNITSIZE, &unitSize, sizeof(unitSize));
        vxQueryBuffer(buffer, VX_BUFFER_ATTRIBUTE_SIZE, &size, sizeof(size));
        vxAccessScalarValue(fill, (void *)&value);
        status = vxAccessBufferRange(buffer, 0, numUnits, &ptr);
        if (status == VX_SUCCESS)
        {
            memset(ptr, value, size);
            status = vxCommitBufferRange(buffer, 0, numUnits, ptr);
            if (status != VX_SUCCESS)
            {
                vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Failed to set buffer range for "VX_FMT_REF"\n", buffer);
            }
        }
        else
        {
            vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Failed to get buffer range for "VX_FMT_REF"\n", buffer);
        }
    }
    return status;
}

static vx_status vxFillImageInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_scalar scalar;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &scalar, sizeof(scalar));
            if (scalar)
            {
                vx_enum stype = 0;
                vxQueryScalar(scalar, VX_SCALAR_ATTRIBUTE_TYPE, &stype, sizeof(stype));
                if (stype == VX_TYPE_UINT32)
                {
                    status = VX_SUCCESS;
                }
            }
        }
    }
    return status;
}

static vx_status vxFillBufferInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_scalar scalar;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &scalar, sizeof(scalar));
            if (scalar)
            {
                vx_enum stype = 0;
                vxQueryScalar(scalar, VX_SCALAR_ATTRIBUTE_TYPE, &stype, sizeof(stype));
                if (stype == VX_TYPE_UINT8)
                {
                    status = VX_SUCCESS;
                }
            }
        }
    }
    return status;
}

static vx_status vxFillImageOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_FAILURE;
    if (index == 1)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_image image = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &image, sizeof(image));
            if (image)
            {
                ptr->type = VX_TYPE_IMAGE;
                vxQueryImage(image, VX_IMAGE_ATTRIBUTE_WIDTH, &ptr->dim.image.width, sizeof(ptr->dim.image.width));
                vxQueryImage(image, VX_IMAGE_ATTRIBUTE_HEIGHT, &ptr->dim.image.height, sizeof(ptr->dim.image.height));
                vxQueryImage(image, VX_IMAGE_ATTRIBUTE_FORMAT, &ptr->dim.image.format, sizeof(ptr->dim.image.format));
                status = VX_SUCCESS;
            }
            else
            {
                status = VX_ERROR_INVALID_REFERENCE;
            }
            vxReleaseParameter(&param);
        }
        else
        {
            status = VX_ERROR_INVALID_PARAMETERS;
        }
    }
    return status;
}

static vx_status vxFillBufferOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 1)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_buffer buffer = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &buffer, sizeof(buffer));
            if (buffer)
            {
                ptr->type = VX_TYPE_BUFFER;
                vxQueryBuffer(buffer, VX_BUFFER_ATTRIBUTE_NUMUNITS, &ptr->dim.buffer.numUnits, sizeof(ptr->dim.buffer.numUnits));
                vxQueryBuffer(buffer, VX_BUFFER_ATTRIBUTE_UNITSIZE, &ptr->dim.buffer.unitSize, sizeof(ptr->dim.buffer.unitSize));
                status = VX_SUCCESS;
            }
            vxReleaseParameter(&param);
        }
    }
    return VX_SUCCESS;
}

static vx_param_description_t fill_image_params[] = {
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

static vx_param_description_t fill_buffer_params[] = {
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_BUFFER, VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t fillimage_kernel = {
    VX_KERNEL_FILL_IMAGE,
    "org.khronos.debug.fill_image",
    vxFillImageKernel,
    fill_image_params, dimof(fill_image_params),
    vxFillImageInputValidator,
    vxFillImageOutputValidator,
    NULL, NULL,
};

vx_kernel_description_t fillbuffer_kernel = {
    VX_KERNEL_FILL_BUFFER,
    "org.khronos.debug.fill_buffer",
    vxFillBufferKernel,
    fill_buffer_params, dimof(fill_buffer_params),
    vxFillBufferInputValidator,
    vxFillBufferOutputValidator,
    NULL, NULL,
};
