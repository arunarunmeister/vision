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
 * \brief The Gradient Kernels (Extras)
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <stdio.h>
#include <VX/vx.h>
#include <VX/vx_ext_extras.h>
#include <VX/vx_helper.h>

static vx_status vxScharr3x3Kernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 3)
    {
        vx_image input  = (vx_image)parameters[0];
        vx_image grad_x = (vx_image)parameters[1];
        vx_image grad_y = (vx_image)parameters[2];
        vx_uint32 i, y, x;
        vx_uint8 *src_base   = NULL;
        vx_int16 *dst_base_x = NULL;
        vx_int16 *dst_base_y = NULL;
        vx_imagepatch_addressing_t src_addr, dst_addr_x, dst_addr_y;
        vx_int16 ops[] = {3,10,3,-3,-10,-3};
        vx_rectangle rect;
        vx_border_mode_t borders = {VX_BORDER_MODE_UNDEFINED, 0};

        if ((grad_x == 0) && (grad_y == 0))
        {
            return VX_ERROR_INVALID_PARAMETERS;
        }

        rect = vxGetValidRegionImage(input);
        status = vxAccessImagePatch(input, rect, 0, &src_addr, (void **)&src_base);
        if (grad_x)
            status |= vxAccessImagePatch(grad_x, rect, 0, &dst_addr_x, (void **)&dst_base_x);
        if (grad_y)
            status |= vxAccessImagePatch(grad_y, rect, 0, &dst_addr_y, (void **)&dst_base_y);
        status |= vxQueryNode(node, VX_NODE_ATTRIBUTE_BORDER_MODE, &borders, sizeof(borders));
        /*! \todo Implement other border modes */
        if (borders.mode == VX_BORDER_MODE_UNDEFINED)
        {
            /* shrink the image by 1 */
            vxAlterRectangle(rect, 1, 1, -1, -1);

            for (y = 1; y < (src_addr.dim_y - 1); y++)
            {
                for (x = 1; x < (src_addr.dim_x - 1); x++)
                {
                    vx_uint8 *gdx[] = {vxFormatImagePatchAddress2d(src_base, x+1, y-1, &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x+1, y,   &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x+1, y+1, &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x-1, y-1, &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x-1, y,   &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x-1, y+1, &src_addr)};

                    vx_uint8 *gdy[] = {vxFormatImagePatchAddress2d(src_base, x-1, y+1, &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x,   y+1, &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x+1, y+1, &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x-1, y-1, &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x,   y-1, &src_addr),
                                       vxFormatImagePatchAddress2d(src_base, x+1, y-1, &src_addr)};

                    vx_int16 *out_x = vxFormatImagePatchAddress2d(dst_base_x, x, y, &dst_addr_x);
                    vx_int16 *out_y = vxFormatImagePatchAddress2d(dst_base_y, x, y, &dst_addr_y);

                    if (out_x) {
                        *out_x = 0;
                        for (i = 0; i < dimof(gdx); i++)
                            *out_x += (ops[i] * gdx[i][0]);
                    }
                    if (out_y) {
                        *out_y = 0;
                        for (i = 0; i < dimof(gdy); i++)
                            *out_y += (ops[i] * gdy[i][0]);
                    }
                }
            }
        }
        else
        {
            status = VX_ERROR_NOT_IMPLEMENTED;
        }
        status = vxCommitImagePatch(input, 0, 0, &src_addr, src_base);
        if (grad_x)
            status |= vxCommitImagePatch(grad_x, rect, 0, &dst_addr_x, dst_base_x);
        if (grad_y)
            status |= vxCommitImagePatch(grad_y, rect, 0, &dst_addr_y, dst_base_y);
        vxReleaseRectangle(&rect);
    }
    return status;
}

static vx_int16 ops3_x[3][3] = {
        {1, 0, -1},
        {2, 0, -2},
        {1, 0, -1},
};

static vx_int16 ops3_y[3][3] = {
        {-1,-2,-1},
        { 0, 0, 0},
        { 1, 2, 1},
};

static vx_int16 ops5_x[5][5] = {
        {2, 1, 0, -1, -2},
        {3, 2, 0, -2, -3},
        {4, 3, 0, -3, -4},
        {3, 2, 0, -2, -3},
        {2, 1, 0, -1, -2},
};
static vx_int16 ops5_y[5][5] = {
        {-2,-3,-4,-3,-2},
        {-1,-2,-3,-2,-1},
        { 0, 0, 0, 0, 0},
        { 1, 2, 3, 2, 1},
        { 2, 3, 4, 3, 2},
};

static vx_int16 ops7_x[7][7] = {
        {3, 2, 1, 0,-1,-2,-3},
        {4, 3, 2, 0,-2,-3,-3},
        {5, 4, 3, 0,-3,-4,-4},
        {6, 5, 4, 0,-4,-5,-6},
        {5, 4, 3, 0,-3,-4,-5},
        {4, 3, 2, 0,-2,-3,-4},
        {3, 2, 1, 0,-1,-2,-3},
};
static vx_int16 ops7_y[7][7] = {
        {-3,-4,-5,-6,-5,-4,-3},
        {-2,-3,-4,-5,-4,-3,-2},
        {-1,-2,-3,-4,-3,-2,-1},
        { 0, 0, 0, 0, 0, 0, 0},
        { 1, 2, 3, 4, 3, 2, 1},
        { 2, 3, 4, 5, 4, 3, 2},
        { 3, 4, 5, 6, 5, 4, 3},
};

static vx_status vxSobelMxNKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 4)
    {
        vx_image input  = (vx_image)parameters[0];
        vx_scalar win   = (vx_scalar)parameters[1];
        vx_image grad_x = (vx_image)parameters[2];
        vx_image grad_y = (vx_image)parameters[3];
        vx_uint32 y, x;
        vx_int32 ws = 0;
        vx_uint8 *src_base   = NULL;
        vx_int16 *dst_base_x = NULL;
        vx_int16 *dst_base_y = NULL;
        vx_imagepatch_addressing_t src_addr, dst_addr_x, dst_addr_y;
        vx_int32 b,b2;
        vx_rectangle rect;
        vx_int16 *opx = NULL;
        vx_int16 *opy = NULL;
        vx_border_mode_t borders = {VX_BORDER_MODE_UNDEFINED, 0};

        if ((grad_x == 0) && (grad_y == 0))
        {
            return VX_ERROR_INVALID_PARAMETERS;
        }
        vxAccessScalarValue(win, &ws);
        b = ws/2;
        b2 = b + 1;
        /* point to the middle integer of the matrix */
        if (ws == 3)
        {
            opx = &ops3_x[b][b];
            opy = &ops3_y[b][b];
        }
        else if (ws == 5)
        {
            opx = &ops5_x[b][b];
            opy = &ops5_y[b][b];
        }
        else if (ws == 7)
        {
            opx = &ops7_x[b][b];
            opy = &ops7_y[b][b];
        }
        printf("Window Size = %d opx=%p opy=%p\n",ws,opx, opy);

        rect = vxGetValidRegionImage(input);
        status = vxAccessImagePatch(input, rect, 0, &src_addr, (void **)&src_base);
        if (grad_x)
            status |= vxAccessImagePatch(grad_x, rect, 0, &dst_addr_x, (void **)&dst_base_x);
        if (grad_y)
            status |= vxAccessImagePatch(grad_y, rect, 0, &dst_addr_y, (void **)&dst_base_y);
        status |= vxQueryNode(node, VX_NODE_ATTRIBUTE_BORDER_MODE, &borders, sizeof(borders));
        /*! \todo Implement other border modes */
        if (borders.mode == VX_BORDER_MODE_UNDEFINED)
        {
            /* shrink the image by b */
            vxAlterRectangle(rect, b, b, -b, -b);
            for (y = b; y < (src_addr.dim_y - b); y++)
            {
                for (x = b; x < (src_addr.dim_x - b); x++)
                {
                    vx_int32 i,j;
                    vx_int16 sum_x = 0;
                    vx_int16 sum_y = 0;
                    vx_int16 *out_x = NULL;
                    vx_int16 *out_y = NULL;
                    if (grad_x)
                        out_x = vxFormatImagePatchAddress2d(dst_base_x, x, y, &dst_addr_x);
                    if (grad_y)
                        out_y = vxFormatImagePatchAddress2d(dst_base_y, x, y, &dst_addr_y);
                    for (j = -b; j <= b; j++)
                    {
                        for (i = -b; i <= b; i++)
                        {
                            vx_uint8 *ptr = NULL;
                            ptr = vxFormatImagePatchAddress2d(src_base, x+i, y+j, &src_addr);
                            if (ptr)
                            {
                                sum_x += (opx[j*ws + i] * (*ptr));
                                sum_y += (opy[j*ws + i] * (*ptr));
                            }
                        }
                    }
                    if (out_x) {
                        *out_x = sum_x;
                    }
                    if (out_y) {
                        *out_y = sum_y;
                    }
                }
            }
        }
        else
        {
            status = VX_ERROR_NOT_IMPLEMENTED;
        }
        status = vxCommitImagePatch(input, 0, 0, &src_addr, src_base);
        if (grad_x)
            status |= vxCommitImagePatch(grad_x, rect, 0, &dst_addr_x, dst_base_x);
        if (grad_y)
            status |= vxCommitImagePatch(grad_y, rect, 0, &dst_addr_y, dst_base_y);
        vxReleaseRectangle(&rect);
    }
    return status;
}

static vx_status vxGradientInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0)
    {
        vx_image input = 0;
        vx_parameter param = vxGetParameterByIndex(node, index);

        vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
        if (input)
        {
            vx_uint32 width = 0, height = 0;
            vx_fourcc format = 0;
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
            if (width >= 3 && height >= 3 && format == FOURCC_U8)
                status = VX_SUCCESS;
        }
        vxReleaseParameter(&param);
    }
    return status;
}

static vx_status vxGradientOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 1 || index == 2)
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
            ptr->dim.image.format = FOURCC_S16;
            ptr->dim.image.width = width;
            ptr->dim.image.height = height;
            status = VX_SUCCESS;
        }
        vxReleaseParameter(&param);
    }
    return status;
}


static vx_status vxGradientMxNInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0)
    {
        vx_image input = 0;
        vx_parameter param = vxGetParameterByIndex(node, index);

        vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
        if (input)
        {
            vx_uint32 width = 0, height = 0;
            vx_fourcc format = 0;
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
            if (width >= 3 && height >= 3 && format == FOURCC_U8)
                status = VX_SUCCESS;
        }
        vxReleaseParameter(&param);
    }
    else if (index == 1)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_scalar win = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &win, sizeof(win));
            if (win)
            {
                vx_enum type = 0;
                vxQueryScalar(win, VX_SCALAR_ATTRIBUTE_TYPE, &type, sizeof(type));
                if (type == VX_TYPE_INT32)
                {
                    vx_uint32 ws = 0;
                    vxAccessScalarValue(win, &ws);
                    if (ws == 3 || ws == 5 || ws == 7)
                    {
                        status = VX_SUCCESS;
                    }
                }
            }
            vxReleaseParameter(&param);
        }
    }
    return status;
}

static vx_status vxGradientMxNOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 2 || index == 3)
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
            ptr->dim.image.format = FOURCC_S16;
            ptr->dim.image.width = width;
            ptr->dim.image.height = height;
            status = VX_SUCCESS;
        }
        vxReleaseParameter(&param);
    }
    return status;
}

static vx_param_description_t gradient_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_OPTIONAL},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_OPTIONAL},
};

static vx_param_description_t gradientMxN_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_OPTIONAL},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_OPTIONAL},
};

vx_kernel_description_t scharr3x3_kernel = {
    VX_KERNEL_EXTRAS_SCHARR_3x3,
    "org.khronos.extras.scharr3x3",
    vxScharr3x3Kernel,
    gradient_kernel_params, dimof(gradient_kernel_params),
    vxGradientInputValidator,
    vxGradientOutputValidator,
    NULL,
    NULL,
};

vx_kernel_description_t sobelMxN_kernel = {
    VX_KERNEL_EXTRAS_SOBEL_MxN,
    "org.khronos.extras.sobelMxN",
    vxSobelMxNKernel,
    gradientMxN_kernel_params, dimof(gradientMxN_kernel_params),
    vxGradientMxNInputValidator,
    vxGradientMxNOutputValidator,
    NULL,
    NULL,
};
