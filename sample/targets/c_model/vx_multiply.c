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
 * \brief The Pixelwise Multiplication Kernel.
 * \author Hans-Peter Nilsson <hp@axis.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>

static vx_status vxMultiplyInputValidator(vx_node node, vx_uint32 index)
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
            if (format == FOURCC_U8 || format == FOURCC_S16)
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
            vx_fourcc format1;

            vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_WIDTH, &width[0], sizeof(width[0]));
            vxQueryImage(images[1], VX_IMAGE_ATTRIBUTE_WIDTH, &width[1], sizeof(width[1]));
            vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_HEIGHT, &height[0], sizeof(height[0]));
            vxQueryImage(images[1], VX_IMAGE_ATTRIBUTE_HEIGHT, &height[1], sizeof(height[1]));
            vxQueryImage(images[1], VX_IMAGE_ATTRIBUTE_FORMAT, &format1, sizeof(format1));
            if (width[0] == width[1] && height[0] == height[1] &&
                (format1 == FOURCC_U8 || format1 == FOURCC_S16))
                status = VX_SUCCESS;
        }
        vxReleaseParameter(&param[0]);
        vxReleaseParameter(&param[1]);
    }
    else if (index == 2)        /* scale: must be non-negative. */
    {
        vx_scalar scalar = 0;
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &scalar, sizeof(scalar));
            if (scalar)
            {
                vx_enum type = -1;
                vxQueryScalar(scalar, VX_SCALAR_ATTRIBUTE_TYPE, &type, sizeof(type));
                if (type == VX_TYPE_FLOAT32)
                {
                    vx_float32 scale = 0.0f;
                    if ((vxAccessScalarValue(scalar, &scale) == VX_SUCCESS) &&
                        (scale >= 0))
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
    else if (index == 3)        /* overflow_policy: truncate or saturate. */
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

static vx_status vxMultiplyOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 4)
    {
        /*
         * We need to look at both input images, but only for the format:
         * if either is S16 or the output type is not U8, then it's S16.
         * The geometry of the output image is copied from the first parameter:
         * the input images are known to match from input parameters validation.
         */
        vx_parameter param[] = {
            vxGetParameterByIndex(node, 0),
            vxGetParameterByIndex(node, 1),
            vxGetParameterByIndex(node, index),
        };
        if (param[0] && param[1] && param[2])
        {
            vx_image images[3];
            vxQueryParameter(param[0], VX_PARAMETER_ATTRIBUTE_REF, &images[0], sizeof(images[0]));
            vxQueryParameter(param[1], VX_PARAMETER_ATTRIBUTE_REF, &images[1], sizeof(images[1]));
            vxQueryParameter(param[2], VX_PARAMETER_ATTRIBUTE_REF, &images[2], sizeof(images[2]));
            if (images[0] && images[1] && images[2])
            {
                vx_uint32 width = 0, height = 0;
                vx_fourcc informat[2] = {FOURCC_VIRT, FOURCC_VIRT};
                vx_fourcc outformat = FOURCC_VIRT;

                /*
                 * When passing on the geometry to the output image, we only look at
                 * image 0, as both input images are verified to match, at input
                 * validation.
                 */
                vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
                vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
                vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_FORMAT, &informat[0], sizeof(informat[0]));
                vxQueryImage(images[1], VX_IMAGE_ATTRIBUTE_FORMAT, &informat[1], sizeof(informat[1]));
                vxQueryImage(images[2], VX_IMAGE_ATTRIBUTE_FORMAT, &outformat, sizeof(outformat));

                if (informat[0] == FOURCC_U8 && informat[1] == FOURCC_U8)
                {
                    if ((outformat == FOURCC_U8) ||
                        (outformat == FOURCC_S16))
                    {
                        status = VX_SUCCESS;
                    }
                }
                else
                {
                    status = VX_SUCCESS;
                    outformat = FOURCC_S16;
                }

                ptr->type = VX_TYPE_IMAGE;
                ptr->dim.image.format = outformat;
                ptr->dim.image.width = width;
                ptr->dim.image.height = height;
            }
            vxReleaseParameter(&param[0]);
            vxReleaseParameter(&param[1]);
        }
    }
    return status;
}

static vx_param_description_t multiply_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

static vx_status vxMultiplyKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 5)
    {
        vx_image in0 = (vx_image)parameters[0];
        vx_image in1 = (vx_image)parameters[1];
        vx_scalar scale_param = (vx_scalar)parameters[2];
        vx_float32 scale = 0.0f;
        vx_scalar policy_param = (vx_scalar)parameters[3];
        vx_enum overflow_policy = -1;
        vx_image output = (vx_image)parameters[4];
        vx_uint32 y, x;
        void *dst_base   = NULL;
        void *src_base[2] = {NULL, NULL};
        vx_imagepatch_addressing_t dst_addr, src_addr[2];
        vx_rectangle rect;
        vx_fourcc in0_format = 0;
        vx_fourcc in1_format = 0;
        vx_fourcc out_format = 0;
        vxQueryImage(output, VX_IMAGE_ATTRIBUTE_FORMAT, &out_format, sizeof(out_format));
        vxQueryImage(in0, VX_IMAGE_ATTRIBUTE_FORMAT, &in0_format, sizeof(in0_format));
        vxQueryImage(in1, VX_IMAGE_ATTRIBUTE_FORMAT, &in1_format, sizeof(in1_format));

        rect = vxGetValidRegionImage(in0);
        status = VX_SUCCESS;
        status |= vxAccessImagePatch(in0, rect, 0, &src_addr[0], (void **)&src_base[0]);
        status |= vxAccessImagePatch(in1, rect, 0, &src_addr[1], (void **)&src_base[1]);
        status |= vxAccessImagePatch(output, rect, 0, &dst_addr, (void **)&dst_base);
        status |= vxAccessScalarValue(scale_param, &scale);
        status |= vxAccessScalarValue(policy_param, &overflow_policy);
        for (y = 0; y < dst_addr.dim_y; y++)
        {
            for (x = 0; x < dst_addr.dim_x; x++)
            {
                /* Either image may be U8 or S16. */
                void *src0p = vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]);
                void *src1p = vxFormatImagePatchAddress2d(src_base[1], x, y, &src_addr[1]);
                void *dstp = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);

                /* Convert inputs to int32, perform the primary multiplication. */
                vx_int32 src0 = in0_format == FOURCC_U8 ? *(vx_uint8 *)src0p : *(vx_int16 *)src0p;
                vx_int32 src1 = in1_format == FOURCC_U8 ? *(vx_uint8 *)src1p : *(vx_int16 *)src1p;
                vx_int32 unscaled_unconverted_result = src0 * src1;

                /*
                 * Convert the intermediate result to float32, then scale down
                 * that number.
                 */
                vx_float32 unscaled_result = (vx_float32)unscaled_unconverted_result;
                vx_float32 scaled_result = scale * unscaled_result;

                /*
                 * We need to first convert to an integer type where the
                 * possible values fit, or else we get platform-specific
                 * overflow behavior for truncation. For saturation, we just
                 * avoid further floating-point operations and rely on that a
                 * floating-point value overflowing a vx_int32 would similarly
                 * overflow a vx_int16 and vx_uint8.
                 */
                vx_int32 int_typed_result = (vx_int32)scaled_result;

                vx_int32 final_result_value;

                /* Finally, overflow-check as per the target type and policy. */
                if (overflow_policy == VX_CONVERT_POLICY_SATURATE)
                {
                    if (out_format == FOURCC_U8)
                    {
                        if (int_typed_result > UINT8_MAX)
                            final_result_value = UINT8_MAX;
                        else if (int_typed_result < 0)
                            final_result_value = 0;
                        else
                            final_result_value = int_typed_result;
                    }
                    else /* Already verified to be FOURCC_S16 */
                    {
                        if (int_typed_result > INT16_MAX)
                            final_result_value = INT16_MAX;
                        else if (int_typed_result < INT16_MIN)
                            final_result_value = INT16_MIN;
                        else
                            final_result_value = int_typed_result;
                    }
                }
                else /* Already verified to be VX_CONVERT_POLICY_TRUNCATE. */
                {
                    /* Just for show: the final assignment will trunce too. */
                    final_result_value = (out_format == FOURCC_U8) ?
                        (vx_uint8)int_typed_result : (vx_int16)int_typed_result;
                }

                if (out_format == FOURCC_U8)
                  *(vx_uint8 *)dstp = (vx_uint8)final_result_value;
                else
                  *(vx_int16 *)dstp = (vx_int16)final_result_value;
            }
        }
        status |= vxCommitImagePatch(in0, 0, 0, &src_addr[0], src_base[0]);
        status |= vxCommitImagePatch(in1, 0, 0, &src_addr[1], src_base[1]);
        status |= vxCommitImagePatch(output, rect, 0, &dst_addr, dst_base);
        vxReleaseRectangle(&rect);
    }
    return status;
}

vx_kernel_description_t multiply_kernel = {
    VX_KERNEL_MULTIPLY,
    "org.khronos.openvx.multiply",
    vxMultiplyKernel,
    multiply_kernel_params, dimof(multiply_kernel_params),
    vxMultiplyInputValidator,
    vxMultiplyOutputValidator,
    NULL,
    NULL,
};
