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

static vx_status vxWarpPerspectiveKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 4)
    {
        vx_image src_image = (vx_image)parameters[0];
        vx_matrix matrix = (vx_matrix)parameters[1];
        vx_scalar stype = (vx_scalar)parameters[2];
        vx_image dst_image = (vx_image)parameters[3];
        vx_enum type = 0;
        void *src_base = NULL;
        void *dst_base = NULL;
        vx_imagepatch_addressing_t src_addr, dst_addr;
        vx_uint32 y = 0u, x = 0u;
        vx_float32 m[3][3];
        vx_rectangle rect = 0;

        memset(m, 0, sizeof(m));
        rect = vxGetValidRegionImage(src_image);
        status = VX_SUCCESS;
        status |= vxAccessMatrix(matrix, m);
        status |= vxAccessScalarValue(stype, &type);
        /*! \todo you don't necessarily get the valid region? */
        status |= vxAccessImagePatch(src_image, rect, 0, &src_addr, &src_base);
        status |= vxAccessImagePatch(dst_image, rect, 0, &dst_addr, &dst_base);
        /*! \todo get border mode */
        if (status == VX_SUCCESS)
        {
            for (y = 0u; y < src_addr.dim_y; y++)
            {
                for (x = 0u; x < src_addr.dim_x; x++)
                {
                    vx_uint32 x0 = 0u, y0 = 0u;
                    vx_float32 z0 = 0.0f;
                    vx_uint8 *src = NULL, *dst = NULL;
                    if (type == VX_INTERPOLATION_TYPE_NEAREST_NEIGHBOR)
                    {
                        /* this truncates the floats to uint32's) */
                        z0 = (m[0][2] * x) + (m[1][2] * y) + m[2][2];
                        x0 = (vx_uint32)(x * m[0][0]) + (vx_uint32)(y * m[1][0]) + (vx_uint32)m[2][0];
                        y0 = (vx_uint32)(x * m[0][1]) + (vx_uint32)(y * m[1][1]) + (vx_uint32)m[2][1];
                        x0 = (vx_uint32)((vx_float32)x0 / z0);
                        y0 = (vx_uint32)((vx_float32)y0 / z0);
                    }
                    else if (type == VX_INTERPOLATION_TYPE_BILINEAR)
                    {
                        /*! \todo implement bilinear perspective warp */
                    }
                    else if (type == VX_INTERPOLATION_TYPE_AREA)
                    {
                        /*! \todo implement area perspective warp */
                    }
                    src = vxFormatImagePatchAddress2d(src_base, x0, y0, &src_addr);
                    dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    if ((src) && (dst))
                    {
                        *dst = *src;
                    }
                }
            }
            /*! \todo compute maximum area rectangle */
        }
        vxCommitMatrix(matrix, m);
        status |= vxCommitImagePatch(src_image, 0, 0, &src_addr, src_base);
        status |= vxCommitImagePatch(dst_image, rect, 0, &dst_addr, dst_base);
        vxReleaseRectangle(&rect);
        status = VX_SUCCESS;
    }
    return status;
}

static vx_status vxWarpAffineKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 4)
    {
        vx_image src_image = (vx_image)parameters[0];
        vx_matrix matrix = (vx_matrix)parameters[1];
        vx_scalar stype = (vx_scalar)parameters[2];
        vx_image dst_image = (vx_image)parameters[3];
        vx_enum type = 0;
        void *src_base = NULL;
        void *dst_base = NULL;
        vx_imagepatch_addressing_t src_addr, dst_addr;
        vx_uint32 y = 0u, x = 0u, width = 0u, height = 0u;
        vx_float32 m[3][2];
        vx_rectangle rect = 0;

        memset(m, 0, sizeof(m));
        vxAccessScalarValue(stype, &type);
        vxQueryImage(src_image, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
        vxQueryImage(src_image, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
        rect = vxCreateRectangle(vxGetContext(src_image), 0, 0, width, height);
        vxAccessMatrix(matrix, m);
        status = VX_SUCCESS;
        status |= vxAccessImagePatch(src_image, rect, 0, &src_addr, &src_base);
        status |= vxAccessImagePatch(dst_image, rect, 0, &dst_addr, &dst_base);
        if (status == VX_SUCCESS)
        {
            for (y = 0u; y < height; y++)
            {
                for (x = 0u; x < width; x++)
                {
                    vx_uint32 x0 = 0;
                    vx_uint32 y0 = 0;
                    vx_uint8 *src = NULL, *dst = NULL;
                    if (type == VX_INTERPOLATION_TYPE_NEAREST_NEIGHBOR)
                    {
                        /* this truncates the floats to uint32's */
                        x0 = (vx_uint32)(x * m[0][0]) + (vx_uint32)(y * m[1][0]) + (vx_uint32)m[2][0];
                        y0 = (vx_uint32)(x * m[0][1]) + (vx_uint32)(y * m[1][1]) + (vx_uint32)m[2][1];
                    }
                    else if (type == VX_INTERPOLATION_TYPE_BILINEAR)
                    {
                        /*! \todo implement bilinear affine warp */
                    }
                    else if (type == VX_INTERPOLATION_TYPE_AREA)
                    {
                        /*! \todo implement area affine warp */
                    }
                    /* printf("x0 = %u y0 = %u, x = %u, y = %u\n", x0, y0, x, y);
                    fflush(stdout); */
                    src = vxFormatImagePatchAddress2d(src_base, x0, y0, &src_addr);
                    dst = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    if ((src) && (dst))
                    {
                        *dst = *src;
                    }
                }
            }
        }
        vxCommitMatrix(matrix, m);
        status |= vxCommitImagePatch(src_image, 0, 0, &src_addr, src_base);
        status |= vxCommitImagePatch(dst_image, rect, 0, &dst_addr, dst_base);
        vxReleaseRectangle(&rect);
        status = VX_SUCCESS;
    }
    return status;
}


static vx_status vxWarpInputValidator(vx_node node, vx_uint32 index)
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
    else if (index == 1)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        vx_matrix matrix;
        vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &matrix, sizeof(matrix));
        if (matrix)
        {
            vx_size rows = 0ul, columns = 0ul;
            vxQueryMatrix(matrix, VX_MATRIX_ATTRIBUTE_ROWS, &rows, sizeof(rows));
            vxQueryMatrix(matrix, VX_MATRIX_ATTRIBUTE_COLUMNS, &columns, sizeof(columns));
            if (((columns == 2) || (columns == 3)) && (rows == 3))
            {
                status = VX_SUCCESS;
            }
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

static vx_status vxWarpOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 3)
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
                vx_uint32 w1 = 0, h1 = 0;
                vx_fourcc f1 = FOURCC_VIRT;

                vxQueryImage(src, VX_IMAGE_ATTRIBUTE_WIDTH, &w1, sizeof(w1));
                vxQueryImage(src, VX_IMAGE_ATTRIBUTE_HEIGHT, &h1, sizeof(h1));
                vxQueryImage(src, VX_IMAGE_ATTRIBUTE_FORMAT, &f1, sizeof(f1));
                /* output can not be virtual */
                if ((w1 != 0) && (h1 != 0) && (f1 == FOURCC_U8))
                {
                    /* fill in the meta data with the attributes so that the checker will pass */
                    ptr->type = VX_TYPE_IMAGE;
                    ptr->dim.image.format = FOURCC_U8;
                    ptr->dim.image.width = w1;
                    ptr->dim.image.height = h1;
                    status = VX_SUCCESS;
                }
            }
            vxReleaseParameter(&src_param);
            vxReleaseParameter(&dst_param);
        }
    }
    return status;
}

static vx_param_description_t warp_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_MATRIX, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_OPTIONAL},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t warp_affine_kernel = {
    VX_KERNEL_WARP_AFFINE,
    "org.khronos.openvx.warp_affine",
    vxWarpAffineKernel,
    warp_kernel_params, dimof(warp_kernel_params),
    vxWarpInputValidator,
    vxWarpOutputValidator,
    NULL,
    NULL,
};

vx_kernel_description_t warp_perspective_kernel = {
    VX_KERNEL_WARP_PERSPECTIVE,
    "org.khronos.openvx.warp_perspective",
    vxWarpPerspectiveKernel,
    warp_kernel_params, dimof(warp_kernel_params),
    vxWarpInputValidator,
    vxWarpOutputValidator,
    NULL,
    NULL,
};

