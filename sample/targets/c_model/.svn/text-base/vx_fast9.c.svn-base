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
 * \brief The Filter Kernels.
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>

#define PERMUTATIONS 16
#define APERTURE 3

static vx_uint8 indexes[PERMUTATIONS][9] = {
    { 0, 1, 2, 3, 4, 5, 6, 7, 8},
    {15, 0, 1, 2, 3, 4, 5, 6, 7},
    {14,15, 0, 1, 2, 3, 4, 5, 6},
    {13,14,15, 0, 1, 2, 3, 4, 5},
    {12,13,14,15, 0, 1, 2, 3, 4},
    {11,12,13,14,15, 0, 1, 2, 3},
    {10,11,12,13,14,15, 0, 1, 2},
    { 9,10,11,12,13,14,15, 0, 1},
    { 8, 9,10,11,12,13,14,15, 0},
    { 7, 8, 9,10,11,12,13,14,15},
    { 6, 7, 8, 9,10,11,12,13,14},
    { 5, 6, 7, 8, 9,10,11,12,13},
    { 4, 5, 6, 7, 8, 9,10,11,12},
    { 3, 4, 5, 6, 7, 8, 9,10,11},
    { 2, 3, 4, 5, 6, 7, 8, 9,10},
    { 1, 2, 3, 4, 5, 6, 7, 8, 9},
};

/* offsets from "p" */
static vx_int32 offsets[16][2] = {
    {  0, -3},
    {  1, -3},
    {  2, -2},
    {  3, -1},
    {  3,  0},
    {  3,  1},
    {  2,  2},
    {  1,  3},
    {  0,  3},
    { -1,  3},
    { -2,  2},
    { -3,  1},
    { -3,  0},
    { -3, -1},
    { -2, -2},
    { -1, -3},
};

vx_status vxFast9CornersKernel(vx_node node, vx_reference parameters[], vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 4)
    {
        vx_image src = (vx_image)parameters[0];
        vx_scalar sens = (vx_scalar)parameters[1];
        vx_scalar nonm = (vx_scalar)parameters[2];
        vx_list points = (vx_image)parameters[3];
        vx_float32 b = 0.0f;
        vx_imagepatch_addressing_t src_addr;
        void *src_base = NULL;
        vx_rectangle rect = 0;
        vx_border_mode_t borders = {VX_BORDER_MODE_UNDEFINED, 0};
        vx_uint8 tolerance = 0;

        status = VX_SUCCESS;
        rect = vxGetValidRegionImage(src);
        status |= vxAccessScalarValue(sens, &b);
        /* remove any pre-existing points */
        status |= vxEmptyList(points);
        status |= vxAccessImagePatch(src, rect, 0, &src_addr, &src_base);
        status |= vxQueryNode(node, VX_NODE_ATTRIBUTE_BORDER_MODE, &borders, sizeof(borders));
        tolerance = (vx_uint8)(b * UINT8_MAX);
        if (status == VX_SUCCESS)
        {
            /*! \todo implement other Fast9 Corners border modes */
            if (borders.mode == VX_BORDER_MODE_UNDEFINED)
            {
                vx_int32 y, x, a, i;
                for (y = APERTURE; y < (src_addr.dim_y - APERTURE); y++)
                {
                    for (x = APERTURE; x < (src_addr.dim_x - APERTURE); x++)
                    {
                        vx_uint8 *ptr = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                        vx_uint8 p = *ptr;
                        for (a = 0; a < PERMUTATIONS; a++)
                        {
                            vx_bool isacorner = vx_true_e;
                            for (i = 0; i < dimof(indexes[a]); i++)
                            {
                                vx_uint8 j = indexes[a][i];
                                vx_uint8 *vptr = vxFormatImagePatchAddress2d(
                                            src_base,
                                            x+offsets[j][0],
                                            y+offsets[j][1],
                                            &src_addr);
                                vx_uint8 v = *vptr;
                                if (v < (p + tolerance))
                                {
                                    isacorner = vx_false_e;
                                }
                            }
                            if (isacorner == vx_true_e)
                            {
                                vx_keypoint key = vxCreateKeypoint(vxGetContext(node));
                                vx_keypoint_t kp, *pkp = &kp;
                                printf("Permutation[%d] is corner at x,y={%d,%d}!\n",a,x,y);
                                status |= vxAccessKeypoint(key, &pkp);
                                kp.x = x;
                                kp.y = y;
                                /* \todo find the strengh of the keypoint */
                                status |= vxCommitKeypoint(key, pkp);
                                status |= vxAddListItem(points, key);
                                vxReleaseKeypoint(&key);
                                break;
                            }
                            isacorner = vx_true_e;
                            for (i = 0; i < dimof(indexes[a]); i++)
                            {
                                vx_uint8 j = indexes[a][i];
                                vx_uint8 *vptr = vxFormatImagePatchAddress2d(
                                            src_base,
                                            x+offsets[j][0],
                                            y+offsets[j][1],
                                            &src_addr);
                                vx_uint8 v = *vptr;
                                if (v > (p - tolerance))
                                {
                                    isacorner = vx_false_e;
                                }
                            }
                            if (isacorner == vx_true_e)
                            {
                                vx_keypoint key = vxCreateKeypoint(vxGetContext(node));
                                vx_keypoint_t kp, *pkp = &kp;
                                printf("Permutation[%d] is corner at x,y={%d,%d}!\n",a,x,y);
                                status |= vxAccessKeypoint(key, &pkp);
                                kp.x = x;
                                kp.y = y;
                                /* \todo find the strengh of the keypoint */
                                status |= vxCommitKeypoint(key, pkp);
                                status |= vxAddListItem(points, key);
                                vxReleaseKeypoint(&key);
                                break;
                            }
                        }
                    }
                }
            }
            else
            {
                status = VX_ERROR_NOT_IMPLEMENTED;
            }
            status |= vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
        }
    }
    return 0;
}


static vx_status vxFast9InputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_image input = 0;
            status = vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
            if ((status == VX_SUCCESS) && (input))
            {
                vx_fourcc format = 0;
                status = vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
                if ((status == VX_SUCCESS) && (format == FOURCC_U8))
                {
                    status = VX_SUCCESS;
                }
            }
            vxReleaseParameter(&param);
        }
    }
    if (index == 1)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_scalar sens = 0;
            status = vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &sens, sizeof(sens));
            if ((status == VX_SUCCESS) && (sens))
            {
                vx_enum type = VX_TYPE_INVALID;
                vxQueryScalar(sens, VX_SCALAR_ATTRIBUTE_TYPE, &type, sizeof(type));
                if (type == VX_TYPE_FLOAT32)
                {
                    vx_float32 k = 0.0f;
                    status = vxAccessScalarValue(sens, &k);
                    if ((status == VX_SUCCESS) && (k >= 0.003) && (k <= 0.5))
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
    if (index == 2)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_scalar sens = 0;
            status = vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &sens, sizeof(sens));
            if ((status == VX_SUCCESS) && (sens))
            {
                vx_enum type = VX_TYPE_INVALID;
                vxQueryScalar(sens, VX_SCALAR_ATTRIBUTE_TYPE, &type, sizeof(type));
                if (type == VX_TYPE_BOOL)
                {
                    vx_bool nonmax;
                    status = vxAccessScalarValue(sens, &nonmax);
                    if ((status == VX_SUCCESS) && ((nonmax == vx_false_e) ||
                                                   (nonmax == vx_true_e)))
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

static vx_status vxFast9OutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 3)
    {
        vx_parameter param = vxGetParameterByIndex(node, index); /* we reference the input image */
        if (param)
        {
            vx_list list = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &list, sizeof(list));
            if (list)
            {
                vx_enum type = 0;
                status = vxQueryList(list, VX_LIST_ATTRIBUTE_TYPE, &type, sizeof(type));
                if ((status == VX_SUCCESS) && (type == VX_TYPE_COORDINATES))
                {
                    ptr->type = VX_TYPE_LIST;
                    ptr->dim.list.type = type;
                    /*! \note there's no hard reason for this number,
                     * just an estimate
                     */
                    ptr->dim.list.initial = 2000;
                    status = VX_SUCCESS;
                }
            }
            vxReleaseParameter(&param);
        }
    }
    return status;
}

static vx_param_description_t fast9_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_LIST, VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t fast9_kernel = {
    VX_KERNEL_FAST_CORNERS,
    "org.khronos.openvx.fast_corners",
    vxFast9CornersKernel,
    fast9_kernel_params, dimof(fast9_kernel_params),
    vxFast9InputValidator,
    vxFast9OutputValidator,
};


