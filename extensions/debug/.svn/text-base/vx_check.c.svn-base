
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
 * \brief The Check Object Kernels.
 * \author Erik Rainey <erik.rainey@ti.com>
 * \defgroup group_debug_ext Debugging Extension
 */

#include <VX/vx.h>
#include <VX/vx_ext_debug.h>
#include <VX/vx_helper.h>

typedef union _packed_value_u {
    vx_uint8  bytes[8];
    vx_uint16  word[4];
    vx_uint32 dword[2];
    vx_uint64 qword[1];
} packed_value_u;

static vx_status vxCheckImageKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_SUCCESS;
    if (num == 3)
    {
        vx_image image = (vx_image)parameters[0];
        vx_scalar fill = (vx_scalar)parameters[1];
        vx_scalar errs = (vx_scalar)parameters[2];
        packed_value_u value;
        vx_uint32 planes = 0u, count = 0u, errors = 0u;
        vx_uint32 x = 0u, y = 0u, p = 0u;
        vx_int32 i = 0;
        vx_imagepatch_addressing_t addr;
        vx_rectangle rect;

        value.dword[0] = 0xDEADBEEF;
        vxAccessScalarValue(fill, &value.dword[0]);
        vxQueryImage(image, VX_IMAGE_ATTRIBUTE_PLANES, &planes, sizeof(planes));
        rect = vxGetValidRegionImage(image);
        for (p = 0u; (p < planes) && (rect); p++)
        {
            void *ptr = NULL;
            status = vxAccessImagePatch(image, rect, p, &addr, &ptr);
            if ((status == VX_SUCCESS) && (ptr))
            {
                for (y = 0; y < addr.dim_y; y+=addr.step_y)
                {
                    for (x = 0; x < addr.dim_x; x+=addr.step_x)
                    {
                        vx_uint8 *pixel = vxFormatImagePatchAddress2d(ptr, x, y, &addr);
                        for (i = 0; i < addr.stride_x; i++)
                        {
                            count++;
                            if (pixel[i] != value.bytes[i])
                            {
                                errors++;
                            }
                        }
                    }
                }
                if (errors > 0)
                {
                    vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Checked %p of %u sub-pixels with 0x%08x with %u errors\n", ptr, count, value.dword, errors);
                }
                vxCommitScalarValue(errs, &errors);
                status = vxCommitImagePatch(image, 0, p, &addr, ptr);
                if (status != VX_SUCCESS)
                {
                    vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Failed to set image patch for "VX_FMT_REF"\n", image);
                }
            }
            else
            {
                vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Failed to get image patch for "VX_FMT_REF"\n", image);
            }
        }
        vxReleaseRectangle(&rect);
        if (errors > 0)
        {
            status = VX_FAILURE;
        }
    }
    return status;
}


static vx_status vxCheckBufferKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_SUCCESS;
    if (num == 3)
    {
        vx_scalar buffer = (vx_buffer)parameters[0];
        vx_scalar fill = (vx_scalar)parameters[1];
        vx_scalar errs = (vx_scalar)parameters[2];
        vx_uint8 value = 0u;
        vx_size numUnits = 0ul, unitSize = 0ul, size = 0ul;
        vx_uint32 errors = 0;
        vx_uint8 *ptr = NULL;

        vxQueryBuffer(buffer, VX_BUFFER_ATTRIBUTE_NUMUNITS, &numUnits, sizeof(numUnits));
        vxQueryBuffer(buffer, VX_BUFFER_ATTRIBUTE_UNITSIZE, &unitSize, sizeof(unitSize));
        vxQueryBuffer(buffer, VX_BUFFER_ATTRIBUTE_SIZE, &size, sizeof(size));
        vxAccessScalarValue(fill, (void *)&value);
        status = vxAccessBufferRange(buffer, 0, numUnits, (void **)&ptr);
        if (status == VX_SUCCESS)
        {
            vx_size i = 0;
            for (i = 0ul; i < size; i++)
            {
                if (ptr[i] != value)
                {
                    errors++;
                }
            }
            vxCommitScalarValue(errs, &errors);
            if (errors > 0)
            {
                vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Check buffer %p of "VX_FMT_SIZE" bytes with 0x%02x, found %u errors\n", ptr, size, value, errors);
            }
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
        if (errors > 0)
        {
            status = VX_FAILURE;
        }
    }
    return status;
}

static vx_status vxCheckImageInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0)
    {
        status = VX_SUCCESS;
    }
    else if (index == 1)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_scalar scalar = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &scalar, sizeof(scalar));
            if (scalar)
            {
                vx_enum stype = 0;
                vxQueryScalar(scalar, VX_SCALAR_ATTRIBUTE_TYPE, &stype, sizeof(stype));
                if (stype == VX_TYPE_UINT32)
                {
                    status = VX_SUCCESS;
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

static vx_status vxCheckBufferInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0)
    {
        status = VX_SUCCESS;
    }
    else if (index == 1)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_scalar scalar = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &scalar, sizeof(scalar));
            if (scalar)
            {
                vx_enum stype = 0;
                vxQueryScalar(scalar, VX_SCALAR_ATTRIBUTE_TYPE, &stype, sizeof(stype));
                if (stype == VX_TYPE_UINT8)
                {
                    status = VX_SUCCESS;
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

static vx_status vxCheckOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 2)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        ptr->dim.scalar.type = VX_TYPE_UINT32;
        if (param)
        {
            vx_scalar serr = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &serr, sizeof(serr));
            if (serr)
            {
                vx_enum stype = 0;
                vxQueryScalar(serr, VX_SCALAR_ATTRIBUTE_TYPE, &stype, sizeof(stype));
                if (stype == VX_TYPE_UINT32)
                {
                    ptr->dim.scalar.type = stype;
                    status = VX_SUCCESS;
                }
            }
            vxReleaseParameter(&param);
        }
    }
    return VX_SUCCESS;
}

static vx_param_description_t check_image_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
};
static vx_param_description_t check_buffer_params[] = {
    {VX_INPUT, VX_TYPE_BUFFER, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t checkimage_kernel = {
    VX_KERNEL_CHECK_IMAGE,
    "org.khronos.debug.check_image",
    vxCheckImageKernel,
    check_image_params, dimof(check_image_params),
    vxCheckImageInputValidator,
    vxCheckOutputValidator,
    NULL, NULL,
};

vx_kernel_description_t checkbuffer_kernel = {
    VX_KERNEL_CHECK_BUFFER,
    "org.khronos.debug.check_buffer",
    vxCheckBufferKernel,
    check_buffer_params, dimof(check_buffer_params),
    vxCheckBufferInputValidator,
    vxCheckOutputValidator,
    NULL, NULL,
};
