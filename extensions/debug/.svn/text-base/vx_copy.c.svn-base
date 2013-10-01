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
 * \brief The Copy Object Kernels.
 * \author Erik Rainey <erik.rainey@ti.com>
 * \defgroup group_debug_ext Debugging Extension
 */

#include <stdio.h>
#include <VX/vx.h>
#include <VX/vx_ext_debug.h>
#include <VX/vx_helper.h>

static vx_status vxCopyImagePtrKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 2)
    {
        vx_scalar input = (vx_image)parameters[0];
        vx_image output = (vx_image)parameters[1];
        vx_uint32 width = 0, height = 0, planes = 0, p = 0, y = 0, len = 0;
        void *src = NULL;
        void *dst = NULL;
        vx_imagepatch_addressing_t dst_addr;
        vx_rectangle rect;
        vx_uint8 *srcp = NULL;

        vxAccessScalarValue(input, &src);
        srcp = (vx_uint8 *)src;

        status = VX_SUCCESS; // assume success until an error occurs.
        status |= vxQueryImage(output, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
        status |= vxQueryImage(output, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
        status |= vxQueryImage(output, VX_IMAGE_ATTRIBUTE_PLANES, &planes, sizeof(planes));
        rect = vxCreateRectangle(vxGetContext(node), 0, 0, width, height);
        for (p = 0; p < planes && status == VX_SUCCESS; p++)
        {
            status = VX_SUCCESS;
            status |= vxAccessImagePatch(output, rect, p, &dst_addr, &dst);
            for (y = 0; y < height && status == VX_SUCCESS; y+=dst_addr.step_y)
            {
                vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst, 0, y, &dst_addr);
                len = (dst_addr.stride_x * dst_addr.dim_x * dst_addr.scale_x) / VX_SCALE_UNITY;
                memcpy(dstp, srcp, len);
                srcp += len;
            }
            if (status == VX_SUCCESS) {
                status |= vxCommitImagePatch(output, rect, p, &dst_addr, dst);
            }
        }
        vxReleaseRectangle(&rect);
    }
    else
    {
        status = VX_ERROR_INVALID_PARAMETERS;
    }
    return status;
}

static vx_status vxCopyImageKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 2)
    {
        vx_image input = (vx_image)parameters[0];
        vx_image output = (vx_image)parameters[1];
        vx_uint32 planes = 0, p = 0, y = 0, len = 0;
        void *src = NULL;
        void *dst = NULL;
        vx_imagepatch_addressing_t src_addr, dst_addr;
        vx_rectangle rect;

        status = VX_SUCCESS; // assume success until an error occurs.
        status |= vxQueryImage(input, VX_IMAGE_ATTRIBUTE_PLANES, &planes, sizeof(planes));
        rect = vxGetValidRegionImage(input);
        for (p = 0; p < planes && status == VX_SUCCESS; p++)
        {
            status = VX_SUCCESS;
            status |= vxAccessImagePatch(input, rect, p, &src_addr, &src);
            status |= vxAccessImagePatch(output, rect, p, &dst_addr, &dst);
            for (y = 0; y < src_addr.dim_y && status == VX_SUCCESS; y+=src_addr.step_y)
            {
                /*
                 * in the case where the secondary planes are subsampled, the
                 * scale will skip over the lines that are repeated.
                 */
                vx_uint8 *srcp = vxFormatImagePatchAddress2d(src, 0, y, &src_addr);
                vx_uint8 *dstp = vxFormatImagePatchAddress2d(dst, 0, y, &dst_addr);
                len = (src_addr.stride_x * src_addr.dim_x * src_addr.scale_x) / VX_SCALE_UNITY;
                memcpy(dstp, srcp, len);
            }
            if (status == VX_SUCCESS) {
                status |= vxCommitImagePatch(input, 0, p, &src_addr, src);
                status |= vxCommitImagePatch(output, rect, p, &dst_addr, dst);
            }
        }
        vxReleaseRectangle(&rect);
    }
    else
    {
        status = VX_ERROR_INVALID_PARAMETERS;
    }
    return status;
}

static vx_status vxCopyBufferKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 2)
    {
        vx_buffer src = (vx_buffer)parameters[0];
        vx_buffer dst = (vx_buffer)parameters[1];
        vx_size srclen = 0, dstlen = 0;
        vx_size srcnu = 0, dstnu = 0;
        void *srcp = NULL;
        void *dstp = NULL;

        status = VX_SUCCESS;
        status |= vxQueryBuffer(src, VX_BUFFER_ATTRIBUTE_SIZE, &srclen, sizeof(srclen));
        status |= vxQueryBuffer(dst, VX_BUFFER_ATTRIBUTE_SIZE, &dstlen, sizeof(dstlen));
        status |= vxQueryBuffer(src, VX_BUFFER_ATTRIBUTE_NUMUNITS, &srcnu, sizeof(srcnu));
        status |= vxQueryBuffer(dst, VX_BUFFER_ATTRIBUTE_NUMUNITS, &dstnu, sizeof(dstnu));
        if (status == VX_SUCCESS)
        {
            status |= vxAccessBufferRange(src, 0, srcnu, &srcp);
            status |= vxAccessBufferRange(dst, 0, dstnu, &dstp);

            if (srclen <= dstlen && srcp && dstp && status == VX_SUCCESS)
            {
                memcpy(dstp, srcp, srclen);
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            status |= vxCommitBufferRange(dst, 0, dstnu, dstp);
            status |= vxCommitBufferRange(src, 0, 0, srcp);
        }
    }
    else
        status = VX_ERROR_INVALID_PARAMETERS;
    return status;
}

static vx_status vxAllPassInputValidator(vx_node node, vx_uint32 index)
{
    /* as long as it's an image, we don't care */
    return VX_SUCCESS;
}

static vx_status vxAllPassOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *format)
{
    return VX_SUCCESS;
}

static vx_status vxCopyImageOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 1)
    {
        vx_parameter param = vxGetParameterByIndex(node, 0); /* input image */
        if (param)
        {
            vx_image input = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(vx_image));
            if (input)
            {
                ptr->type = VX_TYPE_IMAGE;
                vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &ptr->dim.image.format, sizeof(ptr->dim.image.format));
                vxQueryImage(input, VX_IMAGE_ATTRIBUTE_WIDTH, &ptr->dim.image.width, sizeof(ptr->dim.image.width));
                vxQueryImage(input, VX_IMAGE_ATTRIBUTE_HEIGHT, &ptr->dim.image.height, sizeof(ptr->dim.image.height));
                status = VX_SUCCESS;
            }
            vxReleaseParameter(&param);
        }
    }
    return status;
}

static vx_status vxCopyBufferOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index > 0)
    {
        vx_parameter param = vxGetParameterByIndex(node, 0);
        if (param)
        {
            vx_buffer input;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(vx_buffer));
            if (input)
            {
                ptr->type = VX_TYPE_BUFFER;
                vxQueryBuffer(input, VX_BUFFER_ATTRIBUTE_UNITSIZE, &ptr->dim.buffer.unitSize, sizeof(ptr->dim.buffer.unitSize));
                vxQueryBuffer(input, VX_BUFFER_ATTRIBUTE_NUMUNITS, &ptr->dim.buffer.numUnits, sizeof(ptr->dim.buffer.numUnits));
                status = VX_SUCCESS;
            }
            vxReleaseParameter(&param);
        }
    }
    return status;
}


/*! \brief Declares the parameter types for \ref vxCopyImageNode.
  * \ingroup group_debug_ext
  */
static vx_param_description_t copyimage_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

/*! \brief Declares the parameter types for \ref vxCopyBufferNode.
 * \ingroup group_debug_ext
 */
static vx_param_description_t copybuffer_kernel_params[] = {
    {VX_INPUT, VX_TYPE_BUFFER, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_BUFFER, VX_PARAMETER_STATE_REQUIRED},
};

/*! \brief Declares the parameter types for \ref vxCopyImageFromPtrNode.
 * \ingroup group_debug_ext
 */
static vx_param_description_t copyimageptr_kernel_params[] = {
    {VX_INPUT, VX_TYPE_SIZE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t copyimageptr_kernel = {
    VX_KERNEL_COPY_IMAGE_FROM_PTR,
    "org.khronos.debug.copy_image_ptr",
    vxCopyImagePtrKernel,
    copyimageptr_kernel_params, dimof(copyimageptr_kernel_params),
    vxAllPassInputValidator,
    vxAllPassOutputValidator,
    NULL,
    NULL,
};

vx_kernel_description_t copyimage_kernel = {
    VX_KERNEL_DEBUG_COPY_IMAGE,
    "org.khronos.debug.copy_image",
    vxCopyImageKernel,
    copyimage_kernel_params, dimof(copyimage_kernel_params),
    vxAllPassInputValidator,
    vxCopyImageOutputValidator,
    NULL,
    NULL,
};

vx_kernel_description_t copybuffer_kernel = {
    VX_KERNEL_DEBUG_COPY_BUFFER,
    "org.khronos.debug.copy_buffer",
    vxCopyBufferKernel,
    copybuffer_kernel_params, dimof(copybuffer_kernel_params),
    vxAllPassInputValidator,
    vxCopyBufferOutputValidator,
    NULL,
    NULL,
};
