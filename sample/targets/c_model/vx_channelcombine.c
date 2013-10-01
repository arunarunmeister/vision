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
 * \brief The Channel Combine Kernels
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>

static vx_status vxChannelCombineKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 5)
    {
        vx_image inputs[4] = {
            (vx_image)parameters[0],
            (vx_image)parameters[1],
            (vx_image)parameters[2],
            (vx_image)parameters[3],
        };
        vx_image output = (vx_image)parameters[4];
        vx_fourcc format = 0;
        vx_rectangle rect;
        vxQueryImage(output, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
        rect = vxGetValidRegionImage(inputs[0]);
        if ((format == FOURCC_RGB) || (format == FOURCC_RGBX))
        {
            /* write all the channels back out in interleaved format */
            vx_imagepatch_addressing_t src_addrs[4];
            vx_imagepatch_addressing_t dst_addr;
            void *base_src_ptrs[4] = {NULL, NULL, NULL, NULL};
            void *base_dst_ptr = NULL;
            uint32_t x, y, p;
            uint32_t numplanes = 3;

            if (format == FOURCC_RGBX)
            {
                numplanes = 4;
            }

            // get the planes
            for (p = 0; p < numplanes; p++)
            {
                vxAccessImagePatch(inputs[p], rect, 0, &src_addrs[p], &base_src_ptrs[p]);
            }
            vxAccessImagePatch(output, rect, 0, &dst_addr, &base_dst_ptr);
            for (y = 0; y < dst_addr.dim_y; y+=dst_addr.step_y)
            {
                for (x = 0; x < dst_addr.dim_x; x+=dst_addr.step_x)
                {
                    uint8_t *planes[4] = {
                        vxFormatImagePatchAddress2d(base_src_ptrs[0], x, y, &src_addrs[0]),
                        vxFormatImagePatchAddress2d(base_src_ptrs[1], x, y, &src_addrs[1]),
                        vxFormatImagePatchAddress2d(base_src_ptrs[2], x, y, &src_addrs[2]),
                        NULL,
                    };
                    uint8_t *dst = vxFormatImagePatchAddress2d(base_dst_ptr, x, y, &dst_addr);
                    dst[0] = planes[0][0];
                    dst[1] = planes[1][0];
                    dst[2] = planes[2][0];
                    if (format == FOURCC_RGBX)
                    {
                        planes[3] = vxFormatImagePatchAddress2d(base_src_ptrs[3], x, y, &src_addrs[3]);
                        dst[3] = planes[3][0];
                    }
                }
            }
            // write the data back
            vxCommitImagePatch(output, rect, 0, &dst_addr, base_dst_ptr);
            // release the planes
            for (p = 0; p < numplanes; p++)
            {
                vxCommitImagePatch(inputs[p], 0, 0, &src_addrs[p], &base_src_ptrs[p]);
            }
        }
        else if (format == FOURCC_YUV4)
        {
            /* write all the channels back out in the planar format */
            vx_imagepatch_addressing_t src_addrs[3];
            vx_imagepatch_addressing_t dst_addrs[3];
            void *base_src_ptrs[3] = {NULL, NULL, NULL};
            void *base_dst_ptrs[3] = {NULL, NULL, NULL};
            uint32_t x, y, p;

            // get the planes
            for (p = 0; p < 3; p++)
            {
                vxAccessImagePatch(inputs[p], rect, 0, &src_addrs[p], &base_src_ptrs[p]);
                vxAccessImagePatch(output, rect, 0, &dst_addrs[p], &base_dst_ptrs[p]);
            }

            for (y = 0; y < dst_addrs[0].dim_y; y+=dst_addrs[0].step_y)
            {
                for (x = 0; x < dst_addrs[0].dim_x; x+=dst_addrs[0].step_x)
                {
                    uint8_t *planes[3] = {
                        vxFormatImagePatchAddress2d(base_src_ptrs[0], x, y, &src_addrs[0]),
                        vxFormatImagePatchAddress2d(base_src_ptrs[1], x, y, &src_addrs[1]),
                        vxFormatImagePatchAddress2d(base_src_ptrs[2], x, y, &src_addrs[2]),
                    };
                    uint8_t *dsts[3] = {
                        vxFormatImagePatchAddress2d(base_dst_ptrs[0], x, y, &dst_addrs[0]),
                        vxFormatImagePatchAddress2d(base_dst_ptrs[0], x, y, &dst_addrs[0]),
                        vxFormatImagePatchAddress2d(base_dst_ptrs[0], x, y, &dst_addrs[0]),
                    };
                    dsts[0][0] = planes[0][0];
                    dsts[1][0] = planes[1][0];
                    dsts[2][0] = planes[2][0];
                }
            }
            // release the planes
            for (p = 0; p < 3; p++)
            {
                // write the data back
                vxCommitImagePatch(output, rect, 0, &dst_addrs[p], base_dst_ptrs[p]);
                // release the input
                vxCommitImagePatch(inputs[p], 0, 0, &src_addrs[p], &base_src_ptrs[p]);
            }
        }
        vxReleaseRectangle(&rect);
        status = VX_SUCCESS;
    }
    else
        status = VX_ERROR_INVALID_PARAMETERS;
    return status;
}

static vx_status vxChannelCombineInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index < 4)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_image image = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &image, sizeof(image));
            if (image)
            {
                vx_fourcc format = 0;
                vxQueryImage(image, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
                if (format == FOURCC_U8)
                {
                    status = VX_SUCCESS;
                }
            }
            vxReleaseParameter(&param);
        }
    }
    return status;
}

static vx_status vxChannelCombineOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 4)
    {
        vx_uint32 p, width = 0, height = 0;
        vx_parameter params[] = {
                vxGetParameterByIndex(node, 0),
                vxGetParameterByIndex(node, 1),
                vxGetParameterByIndex(node, 2),
                vxGetParameterByIndex(node, 3),
                vxGetParameterByIndex(node, index)
        };
        /* check for equal plane sizes */
        for (p = 0; p < index; p++)
        {
            if (params[p])
            {
                vx_image image = 0;
                vxQueryParameter(params[p], VX_PARAMETER_ATTRIBUTE_REF, &image, sizeof(image));
                if (image)
                {
                    uint32_t w = 0, h = 0;
                    vxQueryImage(image, VX_IMAGE_ATTRIBUTE_WIDTH, &w, sizeof(w));
                    vxQueryImage(image, VX_IMAGE_ATTRIBUTE_HEIGHT, &w, sizeof(h));
                    if (width == 0 && height == 0)
                    {
                        width = w;
                        height = h;
                    }
                    else if (width != w || height != h)
                    {
                        status = VX_ERROR_INVALID_DIMENSION;
                        goto exit;
                    }
                }
            }
        }
        if (params[index])
        {
            vx_image output = 0;
            vxQueryParameter(params[index], VX_PARAMETER_ATTRIBUTE_REF, &output, sizeof(output));
            if (output)
            {
                vx_fourcc format = FOURCC_VIRT;
                vxQueryImage(output, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
                if (format == FOURCC_RGB || format == FOURCC_RGBX || format == FOURCC_YUV4)
                {
                    if ((params[2]) ||  /* all need 3 planes */
                        (format == FOURCC_RGBX && params[3] != 0)) /* RGBX needs 4 */
                    {
                        ptr->type = VX_TYPE_IMAGE;
                        ptr->dim.image.format = format;
                        ptr->dim.image.width = width;
                        ptr->dim.image.height = height;
                        status = VX_SUCCESS;
                    }
                    else
                    {
                        VX_PRINT(VX_ZONE_API, "Valid format but missing planes!\n");
                    }
                }
            }
        }
exit:
        for (p = 0; p < dimof(params); p++)
        {
            if (params[p])
            {
                vxReleaseParameter(&params[p]);
            }
        }
    }
    VX_PRINT(VX_ZONE_API, "%s:%u returned %d\n", __FUNCTION__, index, status);
    return status;
}


static vx_param_description_t channel_combine_kernel_params[] = {
    {VX_INPUT,  VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT,  VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT,  VX_TYPE_IMAGE, VX_PARAMETER_STATE_OPTIONAL},
    {VX_INPUT,  VX_TYPE_IMAGE, VX_PARAMETER_STATE_OPTIONAL},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t channelcombine_kernel = {
    VX_KERNEL_CHANNEL_COMBINE,
    "org.khronos.openvx.channel_combine",
    vxChannelCombineKernel,
    channel_combine_kernel_params, dimof(channel_combine_kernel_params),
    vxChannelCombineInputValidator,
    vxChannelCombineOutputValidator,
    NULL,
    NULL,
};
