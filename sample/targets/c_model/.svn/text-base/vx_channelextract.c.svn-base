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
 * \brief The Channel Extract Kernels
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>

static vx_status vxChannelExtractKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 3)
    {
        vx_image src = (vx_image)parameters[0];
        vx_scalar channel = (vx_scalar)parameters[1];
        vx_image dst = (vx_image)parameters[2];
        vx_enum chan = -1;
        vx_uint32 y, x;
        void *src_base = NULL;
        void *dst_base = NULL;
        vx_fourcc format = 0;
        vx_imagepatch_addressing_t src_addr = {0};
        vx_imagepatch_addressing_t dst_addr = {0};
        vx_rectangle rect;

        vxAccessScalarValue(channel, &chan);
        vxQueryImage(src, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
        rect = vxGetValidRegionImage(src);
        if (format == FOURCC_RGB)
        {
            vx_uint32 index = 0;
            if (!(VX_CHANNEL_0 <= chan && chan < VX_CHANNEL_3))
                return VX_ERROR_INVALID_PARAMETERS;
            if (chan == VX_CHANNEL_R)
                index = 0;
            if (chan == VX_CHANNEL_G)
                index = 1;
            if (chan == VX_CHANNEL_B)
                index = 2;
            vxAccessImagePatch(src, rect, 0, &src_addr, &src_base);
            vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
            for (y = 0; y < src_addr.dim_y; y++)
            {
                for (x = 0; x < src_addr.dim_x; x++)
                {
                    vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                    vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    dstp[0] = srcp[index];
                }
            }
            vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
            vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
        }
        else if (format == FOURCC_RGBX)
        {
            vx_uint32 index = 0;
            if (!(VX_CHANNEL_0 <= chan && chan <= VX_CHANNEL_3))
                return VX_ERROR_INVALID_PARAMETERS;
            if (chan == VX_CHANNEL_R)
                index = 0;
            if (chan == VX_CHANNEL_G)
                index = 1;
            if (chan == VX_CHANNEL_B)
                index = 2;
            if (chan == VX_CHANNEL_A)
                index = 3;
            vxAccessImagePatch(src, rect, 0, &src_addr, &src_base);
            vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
            for (y = 0; y < src_addr.dim_y; y++)
            {
                for (x = 0; x < src_addr.dim_x; x++)
                {
                    vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                    vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    dstp[0] = srcp[index];
                }
            }
            vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
            vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
        }
        else if (format == FOURCC_NV21 || format == FOURCC_NV12)
        {
            if (!(VX_CHANNEL_0 <= chan && chan < VX_CHANNEL_3))
                return VX_ERROR_INVALID_PARAMETERS;

            if (chan == VX_CHANNEL_Y)
            {
                vxAccessImagePatch(src, rect, 0, &src_addr, &src_base);
                vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
                for (y = 0; y < src_addr.dim_y; y++)
                {
                    vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, 0, y, &src_addr);
                    vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst_base, 0, y, &dst_addr);
                    memcpy(dstp, srcp, src_addr.dim_x*sizeof(vx_uint8));
                }
                vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
                vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
            }
            else if ((chan == VX_CHANNEL_U && format == FOURCC_NV12) ||
                     (chan == VX_CHANNEL_V && format == FOURCC_NV21))
            {
                vxAccessImagePatch(src, rect, 1, &src_addr, &src_base);
                vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
                for (y = 0; y < src_addr.dim_y; y+=2)
                {
                    for (x = 0; x < src_addr.dim_x; x+=2)
                    {
                        vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                        vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst_base, x/2, y/2, &dst_addr);
                        dstp[0] = srcp[0];
                    }
                }
                vxCommitImagePatch(src, 0, 1, &src_addr, src_base);
                vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
            }
            else if ((chan == VX_CHANNEL_V && format == FOURCC_NV12) ||
                     (chan == VX_CHANNEL_U && format == FOURCC_NV21))
            {
                vxAccessImagePatch(src, rect, 1, &src_addr, &src_base);
                vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
                for (y = 0; y < src_addr.dim_y; y+=2)
                {
                    for (x = 0; x < src_addr.dim_x; x+=2)
                    {
                        vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                        vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst_base, x/2, y/2, &dst_addr);
                        dstp[0] = srcp[1];
                    }
                }
                vxCommitImagePatch(src, 0, 1, &src_addr, src_base);
                vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
            }
        }
        else if (format == FOURCC_YUYV)
        {
            if (!(VX_CHANNEL_0 <= chan && chan < VX_CHANNEL_3))
                return VX_ERROR_INVALID_PARAMETERS;
            if (chan == VX_CHANNEL_Y)
            {
                vxAccessImagePatch(src, rect, 0, &src_addr, &src_base);
                vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
                for (y = 0; y < src_addr.dim_y; y++)
                {
                    for (x = 0; x < src_addr.dim_x; x++)
                    {
                        vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                        vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                        dstp[0] = srcp[0];
                        dstp[1] = srcp[2];
                    }
                }
                vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
                vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
            }
            else if (chan == VX_CHANNEL_U)
            {
                vxAccessImagePatch(src, rect, 0, &src_addr, &src_base);
                vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
                for (y = 0; y < src_addr.dim_y; y++)
                {
                    for (x = 0; x < src_addr.dim_x; x+=2)
                    {
                        vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                        vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst_base, x/2, y, &dst_addr);
                        dstp[0] = srcp[1];
                    }
                }
                vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
                vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
            }
            else if (chan == VX_CHANNEL_V)
            {
                vxAccessImagePatch(src, rect, 0, &src_addr, &src_base);
                vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
                for (y = 0; y < src_addr.dim_y; y++)
                {
                    for (x = 0; x < src_addr.dim_x; x+=2)
                    {
                        vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                        vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst_base, x/2, y, &dst_addr);
                        dstp[0] = srcp[3];
                    }
                }
                vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
                vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
            }
        }
        else if (format == FOURCC_UYVY)
        {
            if (!(VX_CHANNEL_0 <= chan && chan < VX_CHANNEL_3))
                return VX_ERROR_INVALID_PARAMETERS;
            if (chan == VX_CHANNEL_Y)
            {
                vxAccessImagePatch(src, rect, 0, &src_addr, &src_base);
                vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
                for (y = 0; y < src_addr.dim_y; y++)
                {
                    for (x = 0; x < src_addr.dim_x; x++)
                    {
                        vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                        vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                        dstp[0] = srcp[1];
                        dstp[1] = srcp[3];
                    }
                }
                vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
                vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
            }
            else if (chan == VX_CHANNEL_U)
            {
                vxAccessImagePatch(src, rect, 0, &src_addr, &src_base);
                vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
                for (y = 0; y < src_addr.dim_y; y++)
                {
                    for (x = 0; x < src_addr.dim_x; x+=2)
                    {
                        vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                        vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst_base, x/2, y, &dst_addr);
                        dstp[0] = srcp[0];
                    }
                }
                vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
                vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
            }
            else if (chan == VX_CHANNEL_V)
            {
                vxAccessImagePatch(src, rect, 0, &src_addr, &src_base);
                vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
                for (y = 0; y < src_addr.dim_y; y++)
                {
                    for (x = 0; x < src_addr.dim_x; x+=2)
                    {
                        vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                        vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst_base, x/2, y, &dst_addr);
                        dstp[0] = srcp[2];
                    }
                }
                vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
                vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
            }
        }
        else if (format == FOURCC_IYUV)
        {
            if (!(VX_CHANNEL_0 <= chan && chan < VX_CHANNEL_3))
                return VX_ERROR_INVALID_PARAMETERS;
            if (chan == VX_CHANNEL_Y)
            {
                vxAccessImagePatch(src, rect, 0, &src_addr, &src_base);
                vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
                for (y = 0; y < src_addr.dim_y; y++)
                {
                    vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, 0, y, &src_addr);
                    vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst_base, 0, y, &dst_addr);
                    memcpy(dstp, srcp, src_addr.dim_x*sizeof(vx_uint8));
                }
                vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
                vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
            }
            else if (chan == VX_CHANNEL_U)
            {
                vxAccessImagePatch(src, rect, 1, &src_addr, &src_base);
                vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
                for (y = 0; y < src_addr.dim_y; y+=2)
                {
                    vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, 0, y, &src_addr);
                    vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst_base, 0, y/2, &dst_addr);
                    memcpy(dstp, srcp, src_addr.dim_x*sizeof(vx_uint8)/2);
                }
                vxCommitImagePatch(src, 0, 1, &src_addr, src_base);
                vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
            }
            else if (chan == VX_CHANNEL_V)
            {
                vxAccessImagePatch(src, rect, 2, &src_addr, &src_base);
                vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
                for (y = 0; y < src_addr.dim_y; y+=2)
                {
                    vx_uint8 *srcp = vxFormatImagePatchAddress2d(src_base, 0, y, &src_addr);
                    vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst_base, 0, y/2, &dst_addr);
                    memcpy(dstp, srcp, src_addr.dim_x*sizeof(vx_uint8)/2);
                }
                vxCommitImagePatch(src, 0, 2, &src_addr, src_base);
                vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
            }
        }
        vxReleaseRectangle(&rect);
        status = VX_SUCCESS;
    }
    else
        status = VX_ERROR_INVALID_PARAMETERS;
    return status;
}

static vx_status vxChannelExtractInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_SUCCESS;
    vx_parameter param = vxGetParameterByIndex(node, index);
    if (index == 0)
    {
        vx_image image = 0;
        vx_fourcc format = 0;
        vx_uint32 width, height;
        vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &image, sizeof(image));
        if (image == 0)
            return VX_ERROR_INVALID_PARAMETERS;
        vxQueryImage(image, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
        vxQueryImage(image, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
        vxQueryImage(image, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
        // check to make sure the input format is supported.
        switch (format)
        {
            case FOURCC_RGB:
            case FOURCC_RGBX:
            /* 4:2:0 */
            case FOURCC_NV21:
            case FOURCC_IYUV:
                if (height & 1)
                    return VX_ERROR_INVALID_DIMENSION;
                /* no break */
            case FOURCC_YUYV: /* 4:2:2 */
            case FOURCC_UYVY:
                if (width & 1)
                    return VX_ERROR_INVALID_DIMENSION;
                status = VX_SUCCESS;
                break;
            default:
                status = VX_ERROR_INVALID_FORMAT;
                break;
        }
    }
    else if (index == 1)
    {
        vx_scalar scalar;
        vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &scalar, sizeof(scalar));
        if (scalar)
        {
            vx_enum channel = 0;
            vx_enum type = 0;
            vxQueryScalar(scalar, VX_SCALAR_ATTRIBUTE_TYPE, &type, sizeof(type));
            if (type == VX_TYPE_ENUM)
            {
                vxAccessScalarValue(scalar, &channel);
                if (VX_CHANNEL_0 <= channel && channel <= VX_CHANNEL_3)
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
    }
    else
    {
        status = VX_ERROR_INVALID_PARAMETERS;
    }
    vxReleaseParameter(&param);
    return status;
}

static vx_status vxChannelExtractOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 2)
    {
        vx_parameter param0 = vxGetParameterByIndex(node, 0);
        vx_parameter param1 = vxGetParameterByIndex(node, 1);

        if ((param0) && (param1))
        {
            vx_image input = 0;
            vx_scalar chan = 0;
            vx_enum channel = 0;
            vxQueryParameter(param0, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
            vxQueryParameter(param1, VX_PARAMETER_ATTRIBUTE_REF, &chan, sizeof(chan));
            vxAccessScalarValue(chan, &channel);

            if ((input) && (chan))
            {
                vx_uint32 width = 0, height = 0;
                vx_fourcc format = FOURCC_VIRT;

                vxQueryImage(input, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
                vxQueryImage(input, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
                vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));

                if ((format == FOURCC_IYUV) && (channel > VX_CHANNEL_0))
                {
                    width /= 2;
                    height /= 2;
                }
                else if ((format == FOURCC_NV12) && (channel == VX_CHANNEL_1))
                {
                    height /= 2;
                }
                ptr->type = VX_TYPE_IMAGE;
                ptr->dim.image.format = FOURCC_U8;
                ptr->dim.image.width = width;
                ptr->dim.image.height = height;
                status = VX_SUCCESS;
            }
            vxReleaseParameter(&param0);
            vxReleaseParameter(&param1);
        }
    }
    else
    {
        status = VX_ERROR_INVALID_PARAMETERS;
    }
    VX_PRINT(VX_ZONE_API, "%s:%u returned %d\n", __FUNCTION__, index, status);
    return status;
}


/*! \brief Declares the parameter types for \ref vxuColorConvert.
 * \ingroup group_implementation
 */
static vx_param_description_t channel_extract_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

/*! \brief The exported kernel table entry */
vx_kernel_description_t channelextract_kernel = {
    VX_KERNEL_CHANNEL_EXTRACT,
    "org.khronos.openvx.channel_extract",
    vxChannelExtractKernel,
    channel_extract_kernel_params, dimof(channel_extract_kernel_params),
    vxChannelExtractInputValidator,
    vxChannelExtractOutputValidator,
    NULL,
    NULL,
};

