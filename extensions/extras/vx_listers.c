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
 * \brief The Filter Kernel (Extras)
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_ext_extras.h>
#include <VX/vx_helper.h>

static vx_status vxImageListerKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 2)
    {
        vx_image src = (vx_image)parameters[0];
        vx_list list = (vx_image)parameters[1];
        vx_uint32 y, x;
        void *src_base = NULL;
        vx_imagepatch_addressing_t src_addr;
        vx_rectangle rect;
        vx_fourcc format;

        status = VX_SUCCESS;
        rect = vxGetValidRegionImage(src);
        status |= vxAccessImagePatch(src, rect, 0, &src_addr, &src_base);
        status |= vxQueryImage(src, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
        for (y = 0; y < src_addr.dim_y; y++)
        {
            for (x = 0; x < src_addr.dim_x; x++)
            {
                void *ptr = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                if (ptr)
                {
                    vx_bool set = vx_false_e;
                    vx_float32 strength = 0.0f;
                    if (format == FOURCC_U8)
                    {
                        vx_uint8 pixel = *(vx_uint8 *)ptr;
                        strength = (vx_float32)pixel;
                        set = vx_true_e;
                    }
                    else if (ptr && format == FOURCC_S32)
                    {
                        vx_int32 pixel = *(vx_int32 *)ptr;
                        strength = (vx_float32)pixel;
                        set = vx_true_e;
                    }
                    if ((set == vx_true_e) && (strength > 0.0f))
                    {
                        vx_keypoint_t *keypoint = NULL;
                        vx_keypoint kp = vxCreateKeypoint(vxGetContext(node));
                        vxAccessKeypoint(kp, &keypoint);
                        keypoint->x = x;
                        keypoint->y = y;
                        keypoint->strength = strength;
                        vxCommitKeypoint(kp, keypoint);
                        status = vxAddListItem(list, kp); /* this will keep the reference internally */
                        //printf("pixel(%d,%d) = %lf (status = %d)\n",x,y,strength, status);
                        vxReleaseKeypoint(&kp);
                    }
                }
            }
        }
        status |= vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
        vxReleaseRectangle(&rect);
    }
    return status;
}

static vx_status vxFilterInputValidator(vx_node node, vx_uint32 index)
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
            {
                status = VX_SUCCESS;
            }
        }
        vxReleaseParameter(&param);
    }
    return status;
}

static vx_status vxFilterOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 1)
    {
        vx_image input = 0;
        vx_parameter param = vxGetParameterByIndex(node, 0); /* we reference the input image */

        vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
        if (input)
        {
            vx_uint32 width = 0, height = 0;
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
            ptr->type = VX_TYPE_IMAGE;
            ptr->dim.image.format = FOURCC_U8;
            ptr->dim.image.width = width;
            ptr->dim.image.height = height;
            status = VX_SUCCESS;
        }
        vxReleaseParameter(&param);
    }
    return status;
}

static vx_param_description_t lister_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_LIST, VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t lister_kernel = {
    VX_KERNEL_EXTRAS_IMAGE_LISTER,
    "org.khronos.extras.image_to_list",
    vxImageListerKernel,
    lister_kernel_params, dimof(lister_kernel_params),
    vxFilterInputValidator,
    vxFilterOutputValidator,
    NULL,
    NULL,
};
