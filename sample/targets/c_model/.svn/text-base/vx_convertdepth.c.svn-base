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
 * \brief The Convert Depth Kernel
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>

static vx_status vxConvertDepthKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 4)
    {
        vx_image input = (vx_image)parameters[0];
        vx_image output = (vx_image)parameters[1];
        vx_scalar spol = (vx_scalar)parameters[2];
        vx_scalar sshf = (vx_scalar)parameters[3];
        vx_uint32 y, x;
        void *dst_base = NULL;
        void *src_base = NULL;
        vx_imagepatch_addressing_t dst_addr, src_addr;
        vx_rectangle rect;
        vx_enum format[2];
        vx_enum policy = 0;
        vx_int32 shift = 0;

        status  = VX_SUCCESS;
        status |= vxAccessScalarValue(spol, &policy);
        status |= vxAccessScalarValue(sshf, &shift);
        status |= vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format[0], sizeof(format[0]));
        status |= vxQueryImage(output, VX_IMAGE_ATTRIBUTE_FORMAT, &format[1], sizeof(format[1]));
        rect = vxGetValidRegionImage(input);
        status |= vxAccessImagePatch(input, rect, 0, &src_addr, &src_base);
        status |= vxAccessImagePatch(output, rect, 0, &dst_addr, &dst_base);
        for (y = 0; y < src_addr.dim_y; y++)
        {
            for (x = 0; x < src_addr.dim_x; x++)
            {
                if ((format[0] == FOURCC_U8) && (format[1] == FOURCC_U16))
                {
                    vx_uint8 *src = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                    vx_uint16 *dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    *dst = ((vx_uint16)(*src)) << shift;
                }
                else if ((format[0] == FOURCC_U8) && (format[1] == FOURCC_S16))
                {
                    vx_uint8 *src = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                    vx_int16 *dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    *dst = ((vx_int16)(*src)) << shift;
                }
                else if ((format[0] == FOURCC_U8) && (format[1] == FOURCC_U32))
                {
                    vx_uint8 *src = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                    vx_uint32 *dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    *dst = ((vx_uint32)(*src)) << shift;
                }
                else if ((format[0] == FOURCC_U16) && (format[1] == FOURCC_U32))
                {
                    vx_uint16 *src = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                    vx_uint32 *dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    *dst = ((vx_uint32)(*src)) << shift;
                }
                else if ((format[0] == FOURCC_S16) && (format[1] == FOURCC_S32))
                {
                    vx_int16 *src = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                    vx_int32 *dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    *dst = ((vx_int32)(*src)) << shift;
                }
                else if ((format[0] == FOURCC_U16) && (format[1] == FOURCC_U8))
                {
                    vx_uint16 *src = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                    vx_uint8 *dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    if (policy == VX_CONVERT_POLICY_TRUNCATE)
                    {
                        *dst = (vx_uint8)((*src) >> shift);
                    }
                    else if (policy == VX_CONVERT_POLICY_SATURATE)
                    {
                        vx_uint16 value = (*src) >> shift;
                        value = (value > UINT8_MAX ? UINT8_MAX : value);
                        *dst = (vx_uint8)value;
                    }
                }
                else if ((format[0] == FOURCC_S16) && (format[1] == FOURCC_U8))
                {
                    vx_int16 *src = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                    vx_uint8 *dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    if (policy == VX_CONVERT_POLICY_TRUNCATE)
                    {
                        *dst = (vx_uint8)((*src) >> shift);
                    }
                    else if (policy == VX_CONVERT_POLICY_SATURATE)
                    {
                        vx_int16 value = (*src) >> shift;
                        value = (value < 0 ? 0 : value);
                        value = (value > UINT8_MAX ? UINT8_MAX : value);
                        *dst = (vx_uint8)value;
                    }
                }
                else if ((format[0] == FOURCC_U32) && (format[1] == FOURCC_U8))
                {
                    vx_uint32 *src = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                    vx_uint8 *dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    if (policy == VX_CONVERT_POLICY_TRUNCATE)
                    {
                        *dst = (vx_uint8)((*src) >> shift);
                    }
                    else if (policy == VX_CONVERT_POLICY_SATURATE)
                    {
                        vx_uint32 value = (*src) >> shift;
                        value = (value > UINT8_MAX ? UINT8_MAX : value);
                        *dst = (vx_uint8)value;
                    }
                }
                else if ((format[0] == FOURCC_U32) && (format[1] == FOURCC_U16))
                {
                    vx_uint32 *src = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                    vx_uint16 *dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    if (policy == VX_CONVERT_POLICY_TRUNCATE)
                    {
                        *dst = (vx_uint16)((*src) >> shift);
                    }
                    else if (policy == VX_CONVERT_POLICY_SATURATE)
                    {
                        vx_uint32 value = (*src) >> shift;
                        value = (value > UINT16_MAX ? UINT16_MAX : value);
                        *dst = (vx_uint16)value;
                    }
                }
                else if ((format[0] == FOURCC_S32) && (format[1] == FOURCC_S16))
                {
                    vx_int32 *src = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                    vx_int16 *dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    if (policy == VX_CONVERT_POLICY_TRUNCATE)
                    {
                        *dst = (vx_int16)((*src) >> shift);
                    }
                    else if (policy == VX_CONVERT_POLICY_SATURATE)
                    {
                        vx_int32 value = (*src) >> shift;
                        value = (value < INT16_MIN ? INT16_MIN : value);
                        value = (value > INT16_MAX ? INT16_MAX : value);
                        *dst = (vx_int16)value;
                    }
                }
            }
        }
        status |= vxCommitImagePatch(input, 0, 0, &src_addr, src_base);
        status |= vxCommitImagePatch(output, rect, 0, &dst_addr, dst_base);
        vxReleaseRectangle(&rect);
    }
    return status;
}

static vx_status vxConvertDepthInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_image input = 0;
            status = vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
            if ((status == VX_SUCCESS) && input)
            {
                vx_fourcc format = 0;
                status = vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
                if ((status != VX_SUCCESS) ||
                    (format == FOURCC_U8)  ||
                    (format == FOURCC_U16) ||
                    (format == FOURCC_U32) ||
                    (format == FOURCC_S16) ||
                    (format == FOURCC_S32))
                {
                    status = VX_SUCCESS;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
            }
            vxReleaseParameter(&param);
        }
    }
    else if (index == 2)
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
                if (stype == VX_TYPE_ENUM)
                {
                    vx_enum overflow_policy = 0;
                    vxAccessScalarValue(scalar, &overflow_policy);
                    if ((overflow_policy == VX_CONVERT_POLICY_TRUNCATE) ||
                        (overflow_policy == VX_CONVERT_POLICY_SATURATE))
                    {
                        status = VX_SUCCESS;
                    }
                    else
                    {
                        printf("Overflow given as %08x\n", overflow_policy);
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
    else if (index == 3)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_scalar scalar = 0;
            status = vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &scalar, sizeof(scalar));
            if (status == VX_SUCCESS)
            {
                vx_enum type = 0;
                vxQueryScalar(scalar, VX_SCALAR_ATTRIBUTE_TYPE, &type, sizeof(type));
                if (type == VX_TYPE_INT32)
                {
                    vx_int32 shift = 0;
                    status = vxAccessScalarValue(scalar, &shift);
                    if (status == VX_SUCCESS)
                    {
                        /*! \internal Allowing \f$ 0 \le shift < 32 \f$ could
                         * produce weird results for smaller bit depths */
                        if (shift < 0 || shift >= 32)
                        {
                            status = VX_ERROR_INVALID_VALUE;
                        }
                        /* status should be VX_SUCCESS from call */
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

static vx_status vxConvertDepthOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 1)
    {
        vx_parameter param[2] = {
            vxGetParameterByIndex(node, 0),
            vxGetParameterByIndex(node, 1),
        };
        if (param[0] && param[1])
        {
            vx_image images[2] = {0,0};
            status  = VX_SUCCESS;
            status |= vxQueryParameter(param[0], VX_PARAMETER_ATTRIBUTE_REF, &images[0], sizeof(images[0]));
            status |= vxQueryParameter(param[1], VX_PARAMETER_ATTRIBUTE_REF, &images[1], sizeof(images[1]));
            if ((status == VX_SUCCESS) && (images[0]) && (images[1]))
            {
                vx_uint32 width = 0, height = 0;
                vx_fourcc format[2] = {FOURCC_VIRT, FOURCC_VIRT};
                status |= vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
                status |= vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
                status |= vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_FORMAT, &format[0], sizeof(format[0]));
                status |= vxQueryImage(images[1], VX_IMAGE_ATTRIBUTE_FORMAT, &format[1], sizeof(format[1]));
                if (((format[0] == FOURCC_U8)  && (format[1] == FOURCC_U16)) ||
                    ((format[0] == FOURCC_U8)  && (format[1] == FOURCC_S16)) ||
                    ((format[0] == FOURCC_U8)  && (format[1] == FOURCC_U32)) ||
                    ((format[0] == FOURCC_U16) && (format[1] == FOURCC_U8))  ||
                    ((format[0] == FOURCC_S16) && (format[1] == FOURCC_U8))  ||
                    ((format[0] == FOURCC_U16) && (format[1] == FOURCC_U32)) ||
                    ((format[0] == FOURCC_S16) && (format[1] == FOURCC_S32)) ||
                    ((format[0] == FOURCC_U32) && (format[1] == FOURCC_U8))  ||
                    ((format[0] == FOURCC_U32) && (format[1] == FOURCC_U16)) ||
                    ((format[0] == FOURCC_S32) && (format[1] == FOURCC_S16)))
                {
                    ptr->type = VX_TYPE_IMAGE;
                    ptr->dim.image.format = format[1];
                    ptr->dim.image.width = width;
                    ptr->dim.image.height = height;
                    status = VX_SUCCESS;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
            }
            vxReleaseParameter(&param[0]);
            vxReleaseParameter(&param[1]);
        }
    }
    return status;
}

static vx_param_description_t convertdepth_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR,  VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR,  VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t convertdepth_kernel = {
    VX_KERNEL_CONVERTDEPTH,
    "org.khronos.openvx.convert_depth",
    vxConvertDepthKernel,
    convertdepth_kernel_params, dimof(convertdepth_kernel_params),
    vxConvertDepthInputValidator,
    vxConvertDepthOutputValidator,
    NULL,
    NULL,
};
