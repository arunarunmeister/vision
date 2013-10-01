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
 * \brief The Image Scale Kernel
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>
#include <stdio.h>
#include <math.h>

static uint32_t math_gcd(uint32_t a, uint32_t b)
{
    uint32_t gcd = (a < b ? a : b);
    while (gcd > 0) {
        if ((a % gcd) == 0 && (b % gcd) == 0)
            return gcd;
        gcd -= 1;
    }
    return 0;
}

vx_status vxNearestScaling(vx_image src_image, vx_image dst_image, vx_float64 *interm, vx_size size)
{
    /*! \todo Need to implement nearest neighbor scaling */
	return VX_ERROR_NOT_IMPLEMENTED;
}

vx_status vxAreaScaling(vx_image src_image, vx_image dst_image, vx_float64 *interm, vx_size size)
{
    /*! \todo Need to implement area scaling */
	return VX_ERROR_NOT_IMPLEMENTED;
}

vx_status vxBilinearScaling(vx_image src_image, vx_image dst_image, vx_float64 *interm, vx_size size)
{
    vx_status status = VX_SUCCESS;
    void *src_base = NULL, *dst_base = NULL;
    vx_rectangle src_rect, dst_rect;
    vx_imagepatch_addressing_t src_addr, dst_addr;
    vx_uint32 by, bx, y, x, yi, xi, gcd_w, gcd_h, r1w, r1h, r2w, r2h, r_w, r_h, b_w, b_h;
    vx_uint32 w1 = 0, h1 = 0, w2 = 0, h2 = 0;

    /*! \bug Should ScaleImage use the valid region of the image
     * as the scaling information or the width,height? If it uses the valid
     * region, should is scale the valid region within bounds of the
     * image?
     */
    vxQueryImage(src_image, VX_IMAGE_ATTRIBUTE_WIDTH, &w1, sizeof(w1));
    vxQueryImage(src_image, VX_IMAGE_ATTRIBUTE_HEIGHT, &h1, sizeof(h1));
    vxQueryImage(dst_image, VX_IMAGE_ATTRIBUTE_WIDTH, &w2, sizeof(w2));
    vxQueryImage(dst_image, VX_IMAGE_ATTRIBUTE_HEIGHT, &h2, sizeof(h2));

    src_rect = vxCreateRectangle(vxGetContext(src_image), 0, 0, w1, h1);
    dst_rect = vxCreateRectangle(vxGetContext(dst_image), 0, 0, w2, h2);

    status = VX_SUCCESS;
    status |= vxAccessImagePatch(src_image, src_rect, 0, &src_addr, &src_base);
    status |= vxAccessImagePatch(dst_image, dst_rect, 0, &dst_addr, &dst_base);

    /* compute the Resize block sizes and the intermediate block size */
    gcd_w = math_gcd(w1, w2);
    gcd_h = math_gcd(h1, h2);
    r1w = w1 / gcd_w;
    r2w = w2 / gcd_w;
    r1h = h1 / gcd_h;
    r2h = h2 / gcd_h;
    r_w = r1w * r2w;
    r_h = r1h * r2h;
    b_w = w1 / r1w;
    b_h = h1 / r1h;

    /* printf("%ux%u => %ux%u :: r1:%ux%u => r2:%ux%u (%p:%ux%u) blocks:%ux%u\n",
            w1,h1, w2,h2, r1w,r1h, r2w,r2h, interm, r_w,r_h, b_w,b_h); */

    /* iterate over each block */
    for (by = 0; by < b_h; by++)
    {
        for (bx = 0; bx < b_w; bx++)
        {
            /* convert source image to intermediate */
            for (y = 0; y < r1h; y++)
            {
                for (x = 0; x < r1w; x++)
                {
                    vx_uint32 i = (((by * r1h) + y) * src_addr.stride_y) +
                                  (((bx * r1w) + x) * src_addr.stride_x);
                    vx_uint8 pixel = ((vx_uint8 *)src_base)[i];
                    for (yi = 0; yi < r2h; yi++)
                    {
                        for (xi = 0; xi < r2w; xi++)
                        {
                            vx_uint32 k = ((((y * r2h) + yi) * r_w) +
                                            ((x * r2w) + xi));
                            interm[k] = (vx_float64)pixel / (vx_float64)(r2w*r2h);
                        }
                    }
                }
            }

            /* convert the intermediate into the destination */
            for (y = 0; y < r2h; y++)
            {
                for (x = 0; x < r2w; x++)
                {
                    uint32_t sum32 = 0;
                    vx_float64 sum = 0.0f;
                    vx_uint32 i = (((by * r2h) + y) * dst_addr.stride_y) +
                                  (((bx * r2w) + x) * dst_addr.stride_x);
                    vx_uint8 *dst = &((vx_uint8 *)dst_base)[i];
                    /* sum intermediate into destination */
                    for (yi = 0; yi < r1h; yi++)
                    {
                        for (xi = 0; xi < r1w; xi++)
                        {
                            vx_uint32 k = ((((y * r1h) + yi) * r_w) +
                                            ((x * r1w) + xi));
                            sum += interm[k];
                        }
                    }
                    /* rescale the output value */
                    sum32 = (vx_uint32)(sum * (r2w*r2h)/(r1w*r1h));
                    sum32 = (sum32 > 255?255:sum32);
                    *dst = (vx_uint8)sum32;
                }
            }
        }
    }

    status |= vxCommitImagePatch(src_image, 0, 0, &src_addr, src_base);
    status |= vxCommitImagePatch(dst_image, dst_rect, 0, &dst_addr, dst_base);
    vxReleaseParameter(&src_rect);
    vxReleaseParameter(&dst_rect);
    return VX_SUCCESS;
}

static vx_status vxScaleImageKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 3)
    {
        vx_image src_image = (vx_image)parameters[0];
        vx_image dst_image = (vx_image)parameters[1];
        vx_scalar stype = (vx_scalar)parameters[2];
        vx_enum type = 0;
        vx_float64 *interm = NULL;
        vx_size size = 0ul;

        vxAccessScalarValue(stype, &type);
        vxQueryNode(node, VX_NODE_ATTRIBUTE_LOCAL_DATA_PTR, &interm, sizeof(interm));
        vxQueryNode(node, VX_NODE_ATTRIBUTE_LOCAL_DATA_SIZE,&size, sizeof(size));
        if (interm && size)
        {
            if (type == VX_INTERPOLATION_TYPE_BILINEAR)
            {
                status = vxBilinearScaling(src_image,dst_image, interm, size);
            }
            else if (type == VX_INTERPOLATION_TYPE_AREA)
            {
                status = vxAreaScaling(src_image, dst_image, interm, size);
            }
            else if (type == VX_INTERPOLATION_TYPE_NEAREST_NEIGHBOR)
            {
                status = vxNearestScaling(src_image, dst_image, interm, size);
            }
        }
        else
        {
            status = VX_ERROR_NO_RESOURCES;
        }
    }
    return status;
}

static vx_status vxScaleImageInitializer(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (num == 3)
    {
        vx_image src = (vx_image)parameters[0];
        vx_image dst = (vx_image)parameters[1];
        /* vx_scalar stype = (vx_image)parameters[2]; */
        vx_uint32 w1 = 0, h1 = 0, w2 = 0, h2 = 0, gcd_w = 0, gcd_h = 0;
        vx_size size = 0;

        vxQueryImage(src, VX_IMAGE_ATTRIBUTE_WIDTH, &w1, sizeof(w1));
        vxQueryImage(src, VX_IMAGE_ATTRIBUTE_HEIGHT, &h1, sizeof(h1));
        vxQueryImage(dst, VX_IMAGE_ATTRIBUTE_WIDTH, &w2, sizeof(w2));
        vxQueryImage(dst, VX_IMAGE_ATTRIBUTE_HEIGHT, &h2, sizeof(h2));

        gcd_w = math_gcd(w1,w2);
        gcd_h = math_gcd(h1,h2);
        /* printf("%ux%u => %ux%u :: GCD_w %u GCD_h %u\n", w1,h1, w2,h2, gcd_w, gcd_h); */
        if (gcd_w != 0 && gcd_h != 0)
        {
            size = (w1 / gcd_w) * (w2 / gcd_w) * (h1 / gcd_h) * (h2 / gcd_h) * sizeof(vx_float64);
        }
        /* printf("Requesting "VX_FMT_SIZE" bytes for resizer\n", size); */
        vxSetNodeAttribute(node, VX_NODE_ATTRIBUTE_LOCAL_DATA_SIZE, &size, sizeof(size));
        status = VX_SUCCESS;
    }
    return status;
}

static vx_status vxScaleImageInputValidator(vx_node node, vx_uint32 index)
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
                    vx_enum interp = 0;
                    vxAccessScalarValue(scalar, &interp);
                    if ((interp == VX_INTERPOLATION_TYPE_NEAREST_NEIGHBOR) ||
                        (interp == VX_INTERPOLATION_TYPE_BILINEAR) ||
                        (interp == VX_INTERPOLATION_TYPE_AREA))
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

static vx_status vxScaleImageOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 1)
    {
        vx_parameter src_param = vxGetParameterByIndex(node, 0);
        vx_parameter dst_param = vxGetParameterByIndex(node, index);
        if (src_param && dst_param)
        {
            vx_image src = 0;
            vx_image dst = 0;
            vxQueryParameter(src_param, VX_PARAMETER_ATTRIBUTE_REF, &src, sizeof(src));
            vxQueryParameter(dst_param, VX_PARAMETER_ATTRIBUTE_REF, &dst, sizeof(dst));
            if ((src) && (dst))
            {
                vx_uint32 w1 = 0, h1 = 0, w2 = 0, h2 = 0;
                vx_fourcc f1 = FOURCC_VIRT, f2 = FOURCC_VIRT;

                vxQueryImage(src, VX_IMAGE_ATTRIBUTE_WIDTH, &w1, sizeof(w1));
                vxQueryImage(src, VX_IMAGE_ATTRIBUTE_HEIGHT, &h1, sizeof(h1));
                vxQueryImage(dst, VX_IMAGE_ATTRIBUTE_WIDTH, &w2, sizeof(w2));
                vxQueryImage(dst, VX_IMAGE_ATTRIBUTE_HEIGHT, &h2, sizeof(h2));
                vxQueryImage(src, VX_IMAGE_ATTRIBUTE_FORMAT, &f1, sizeof(f1));
                vxQueryImage(dst, VX_IMAGE_ATTRIBUTE_FORMAT, &f2, sizeof(f2));
                /* output can not be virtual */
                if ((w2 != 0) && (h2 != 0) && (f2 != FOURCC_VIRT) && (f1 == f2))
                {
                    /* fill in the meta data with the attributes so that the checker will pass */
                    ptr->type = VX_TYPE_IMAGE;
                    ptr->dim.image.format = f2;
                    ptr->dim.image.width = w2;
                    ptr->dim.image.height = h2;
                    status = VX_SUCCESS;
                }
            }
            vxReleaseParameter(&src_param);
            vxReleaseParameter(&dst_param);
        }
    }
    return status;
}

static vx_param_description_t scale_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_OPTIONAL},
};

vx_kernel_description_t scale_image_kernel = {
    VX_KERNEL_SCALE_IMAGE,
    "org.khronos.openvx.scale_image",
    vxScaleImageKernel,
    scale_kernel_params, dimof(scale_kernel_params),
    vxScaleImageInputValidator,
    vxScaleImageOutputValidator,
    vxScaleImageInitializer,
    NULL,
};

