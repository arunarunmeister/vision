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
 * \brief The Non-Maxima Suppression Kernel (Extras)
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_ext_extras.h>
#include <VX/vx_helper.h>
#include <math.h>

static vx_status vxEuclideanNonMaxSuppressionKernel(vx_node node, vx_reference parameters[], vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 4)
    {
        vx_image src = (vx_image)parameters[0];
        vx_scalar rad = (vx_scalar)parameters[1];
        vx_scalar thr = (vx_scalar)parameters[2];
        vx_image dst = (vx_image)parameters[3];
        void *src_base = NULL, *dst_base = NULL;
        vx_imagepatch_addressing_t src_addr, dst_addr;
        vx_float32 radius = 0.0f;
        vx_int32 r = 0, r2 = 0;
        vx_float32 thresh = 0;
        vx_int32 threshold = 0;
        vx_rectangle rect = vxGetValidRegionImage(src);

        status = VX_SUCCESS;
        status |= vxAccessScalarValue(rad, &radius);
        status |= vxAccessScalarValue(thr, &thresh);
        status |= vxAccessImagePatch(src, rect, 0, &src_addr, &src_base);
        status |= vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
        r = (vx_uint32)radius;
        r = (r <=0 ? 1 : r);
        r2 = r / 2;
        threshold = (vx_int32)thresh;
        if (status == VX_SUCCESS)
        {
            vx_int32 y, x, i, j;
            vx_float32 d = 0;
            for (y = 0; y < src_addr.dim_y; y++)
            {
                for (x = 0; x < src_addr.dim_x; x++)
                {
                    vx_int32 max = 0;
                    vx_int32 *ptr = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                    vx_int32 *out = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);

                    if ((*ptr) < threshold)
                    {
                        continue;
                    }
                    //printf("src(%d,%d) = %d > %d (r2=%u)\n",x,y,*ptr,threshold,r2);
                    for (j = -r2; j <= r2; j++)
                    {
                        if ((y+j < 0) || (y+j >= src_addr.dim_y))
                        {
                            continue;
                        }
                        for (i = -r2; i <= r2; i++)
                        {
                            if ((x+i < 0) || (x+i >= src_addr.dim_x))
                            {
                                continue;
                            }
                            else
                            {
                                vx_int32 dx = (x - (x+i));
                                vx_int32 dy = (y - (y+j));
                                d = sqrt((dx*dx) + (dy*dy));
                                //printf("{%d,%d} is %lf from {%d,%d} radius=%lf\n",x+i,y+j,d,x,y,radius);
                                if (d < radius)
                                {
                                    vx_int32 *non = vxFormatImagePatchAddress2d(src_base, x+i, y+j, &src_addr);
                                    //printf("src(%d,%d) = %d ? %d\n",x+i,y+j,(*non),max);
                                    if (max < (*non))
                                        max = (*non);
                                }
                            }
                        }
                    }
                    //printf("src(%d,%d) = %d ? %d\n",x+i,y+j,(*ptr),max);
                    if ((*ptr) == max)
                        *out = *ptr;
                    else
                        *out = 0;
                }
            }
        }
        status |= vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
        status |= vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
    }
    return status;
}

static vx_status vxNonMaxSuppressionKernel(vx_node node, vx_reference parameters[], vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 3)
    {
        vx_image i_mag = (vx_image)parameters[0];
        vx_image i_ang = (vx_image)parameters[1];
        vx_image i_edge = (vx_image)parameters[2];
        vx_uint32 y = 0, x = 0;
        void *mag_base = NULL;
        void *ang_base = NULL;
        void *edge_base = NULL;
        vx_imagepatch_addressing_t mag_addr, ang_addr, edge_addr;
        vx_rectangle rect;
        vx_fourcc format = 0;
        vx_border_mode_t borders = {VX_BORDER_MODE_UNDEFINED, 0};

        status  = VX_SUCCESS; // assume success until an error occurs.
        status |= vxQueryImage(i_mag, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
        rect = vxGetValidRegionImage(i_mag);
        status |= vxAccessImagePatch(i_mag, rect, 0, &mag_addr, &mag_base);
        status |= vxAccessImagePatch(i_ang, rect, 0, &ang_addr, &ang_base);
        status |= vxAccessImagePatch(i_edge, rect, 0, &edge_addr, &edge_base);
        /*! \todo Implement other border modes */
        if (borders.mode == VX_BORDER_MODE_UNDEFINED)
        {
            /* shrink the image by 1 */
            vxAlterRectangle(rect, 1, 1, -1, -1);
            for (y = 1; (y < (edge_addr.dim_y - 1)) && (status == VX_SUCCESS); y++)
            {
                for (x = 1; x < (edge_addr.dim_x - 1); x++)
                {
                    vx_uint8 *ang = vxFormatImagePatchAddress2d(ang_base, x, y, &ang_addr);
                    vx_uint8 step = 16;
                    vx_uint8 angle = *ang;
                    if (format == FOURCC_U8)
                    {
                        vx_uint8 *mag[] = {
                            vxFormatImagePatchAddress2d(mag_base, x-1, y-1, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+0, y-1, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+1, y-1, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x-1, y+0, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+0, y+0, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+1, y+0, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x-1, y+1, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+0, y+1, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+1, y+1, &mag_addr),
                        };
                        vx_uint8 *edge = vxFormatImagePatchAddress2d(edge_base, x, y, &edge_addr);
                        //printf("x,y = %u,%u  angle = %u\n", x, y, *ang);
                        //printf("mag[0][1][2] = {%u %u %u}\n", *mag[0], *mag[1], *mag[2]);
                        //printf("mag[3][4][5] = {%u %u %u}\n", *mag[3], *mag[4], *mag[5]);
                        //printf("mag[6][7][8] = {%u %u %u}\n", *mag[6], *mag[7], *mag[8]);
                        /* half of 0 range */
                        if ((/* 0 <= *ang && */ angle < (0+step)) && *mag[1] < *mag[4] && *mag[7] < *mag[4])
                            *edge = *mag[4];
                        /* other half */
                        else if ((240 <= angle /* && *ang <= 255 */) && *mag[1] < *mag[4] && *mag[7] < *mag[4])
                            *edge = *mag[4];
                        /* 45 degrees */
                        else if ((32-step <= angle && angle < 32+step) && *mag[0] < *mag[4] && *mag[8] < *mag[4])
                            *edge = *mag[4];
                        /* 90 degrees */
                        else if ((64-step <= angle && angle < 64+step) && *mag[3] < *mag[4] && *mag[5] < *mag[4])
                            *edge = *mag[4];
                        /* 135 degrees */
                        else if ((96-step <= angle && angle < 96+step) && *mag[0] < *mag[4] && *mag[8] < *mag[4])
                            *edge = *mag[4];
                        /* 180 degrees */
                        else if ((128-step < angle && angle < 128+step) && *mag[1] < *mag[4] && *mag[7] < *mag[4])
                            *edge = *mag[4];
                        else if ((160-step < angle && angle < 160+step) && *mag[0] < *mag[4] && *mag[8] < *mag[4])
                            *edge = *mag[4];
                        else if ((192-step < angle && angle < 192+step) && *mag[3] < *mag[4] && *mag[5] < *mag[4])
                            *edge = *mag[4];
                        else if ((224-step < angle && angle < 224+step) && *mag[2] < *mag[4] && *mag[6] < *mag[4])
                            *edge = *mag[4];
                        else
                            *edge = 0;
                    }
                    else if (format == FOURCC_S16)
                    {
                        vx_int16 *mag[] = {
                            vxFormatImagePatchAddress2d(mag_base, x-1, y-1, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+0, y-1, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+1, y-1, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x-1, y+0, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+0, y+0, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+1, y+0, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x-1, y+1, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+0, y+1, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+1, y+1, &mag_addr),
                        };
                        vx_int16 *edge = vxFormatImagePatchAddress2d(edge_base, x, y, &edge_addr);
                        //printf("x,y = %u,%u  angle = %u\n", x, y, *ang);
                        //printf("mag[0][1][2] = {%u %u %u}\n", *mag[0], *mag[1], *mag[2]);
                        //printf("mag[3][4][5] = {%u %u %u}\n", *mag[3], *mag[4], *mag[5]);
                        //printf("mag[6][7][8] = {%u %u %u}\n", *mag[6], *mag[7], *mag[8]);
                        /* half of 0 range */
                        if ((/* 0 <= *ang && */ angle < (0+step)) && *mag[1] < *mag[4] && *mag[7] < *mag[4])
                            *edge = *mag[4];
                        /* other half */
                        else if ((240 <= angle /* && *ang <= 255 */) && *mag[1] < *mag[4] && *mag[7] < *mag[4])
                            *edge = *mag[4];
                        /* 45 degrees */
                        else if ((32-step <= angle && angle < 32+step) && *mag[0] < *mag[4] && *mag[8] < *mag[4])
                            *edge = *mag[4];
                        /* 90 degrees */
                        else if ((64-step <= angle && angle < 64+step) && *mag[3] < *mag[4] && *mag[5] < *mag[4])
                            *edge = *mag[4];
                        /* 135 degrees */
                        else if ((96-step <= angle && angle < 96+step) && *mag[0] < *mag[4] && *mag[8] < *mag[4])
                            *edge = *mag[4];
                        /* 180 degrees */
                        else if ((128-step < angle && angle < 128+step) && *mag[1] < *mag[4] && *mag[7] < *mag[4])
                            *edge = *mag[4];
                        else if ((160-step < angle && angle < 160+step) && *mag[0] < *mag[4] && *mag[8] < *mag[4])
                            *edge = *mag[4];
                        else if ((192-step < angle && angle < 192+step) && *mag[3] < *mag[4] && *mag[5] < *mag[4])
                            *edge = *mag[4];
                        else if ((224-step < angle && angle < 224+step) && *mag[2] < *mag[4] && *mag[6] < *mag[4])
                            *edge = *mag[4];
                        else
                            *edge = 0;
                    }
                    else if (format == FOURCC_U16)
                    {
                        vx_uint16 *mag[] = {
                            vxFormatImagePatchAddress2d(mag_base, x-1, y-1, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+0, y-1, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+1, y-1, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x-1, y+0, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+0, y+0, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+1, y+0, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x-1, y+1, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+0, y+1, &mag_addr),
                            vxFormatImagePatchAddress2d(mag_base, x+1, y+1, &mag_addr),
                        };
                        vx_uint16 *edge = vxFormatImagePatchAddress2d(edge_base, x, y, &edge_addr);
                        //printf("x,y = %u,%u  angle = %u\n", x, y, *ang);
                        //printf("mag[0][1][2] = {%u %u %u}\n", *mag[0], *mag[1], *mag[2]);
                        //printf("mag[3][4][5] = {%u %u %u}\n", *mag[3], *mag[4], *mag[5]);
                        //printf("mag[6][7][8] = {%u %u %u}\n", *mag[6], *mag[7], *mag[8]);
                        /* half of 0 range */
                        if ((/* 0 <= *ang && */ angle < (0+step)) && *mag[1] < *mag[4] && *mag[7] < *mag[4])
                            *edge = *mag[4];
                        /* other half */
                        else if ((240 <= angle /* && *ang <= 255 */) && *mag[1] < *mag[4] && *mag[7] < *mag[4])
                            *edge = *mag[4];
                        /* 45 degrees */
                        else if ((32-step <= angle && angle < 32+step) && *mag[0] < *mag[4] && *mag[8] < *mag[4])
                            *edge = *mag[4];
                        /* 90 degrees */
                        else if ((64-step <= angle && angle < 64+step) && *mag[3] < *mag[4] && *mag[5] < *mag[4])
                            *edge = *mag[4];
                        /* 135 degrees */
                        else if ((96-step <= angle && angle < 96+step) && *mag[0] < *mag[4] && *mag[8] < *mag[4])
                            *edge = *mag[4];
                        /* 180 degrees */
                        else if ((128-step < angle && angle < 128+step) && *mag[1] < *mag[4] && *mag[7] < *mag[4])
                            *edge = *mag[4];
                        else if ((160-step < angle && angle < 160+step) && *mag[0] < *mag[4] && *mag[8] < *mag[4])
                            *edge = *mag[4];
                        else if ((192-step < angle && angle < 192+step) && *mag[3] < *mag[4] && *mag[5] < *mag[4])
                            *edge = *mag[4];
                        else if ((224-step < angle && angle < 224+step) && *mag[2] < *mag[4] && *mag[6] < *mag[4])
                            *edge = *mag[4];
                        else
                            *edge = 0;
                    }
                }
            }
        }
        else
        {
            status = VX_ERROR_NOT_IMPLEMENTED;
        }
        status |= vxCommitImagePatch(i_mag, 0, 0, &mag_addr, mag_base);
        status |= vxCommitImagePatch(i_ang, 0, 0, &ang_addr, ang_base);
        status |= vxCommitImagePatch(i_edge, rect, 0, &edge_addr, edge_base);
        vxReleaseRectangle(&rect);
    }
    else
    {
        status = VX_ERROR_INVALID_PARAMETERS;
    }
    return status;
}

static vx_status vxEuclideanNonMaxInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0) /* image */
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_image img = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &img, sizeof(img));
            if (img)
            {
                vx_fourcc format = FOURCC_VIRT;
                vxQueryImage(img, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
                if (format == FOURCC_S32)
                {
                    status = VX_SUCCESS;
                }
            }
            vxReleaseParameter(&param);
        }
    }
    else if (index == 1)
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
                if (stype == VX_TYPE_FLOAT32)
                {
                    vx_float32 radius = 0;
                    vxAccessScalarValue(scalar, &radius);
                    if ((1.0 < radius) && (radius < 5.0))
                    {
                        status = VX_SUCCESS;
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
                if (stype == VX_TYPE_INT32)
                {
                    status = VX_SUCCESS;
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

static vx_status vxNonMaxSuppressionInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0) /* magnitude */
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_image img = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &img, sizeof(img));
            if (img)
            {
                vx_fourcc format = FOURCC_VIRT;
                vxQueryImage(img, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
                if (format == FOURCC_U8 || format == FOURCC_S16 || format == FOURCC_U16)
                {
                    status = VX_SUCCESS;
                }
            }
            vxReleaseParameter(&param);
        }
    }
    if (index == 1)
    {
        vx_parameter params[] = {
            vxGetParameterByIndex(node, 0),
            vxGetParameterByIndex(node, 1),
        };
        if (params[0] && params[1])
        {
            vx_image images[] = {0, 0};

            vxQueryParameter(params[0], VX_PARAMETER_ATTRIBUTE_REF, &images[0], sizeof(images[0]));
            vxQueryParameter(params[1], VX_PARAMETER_ATTRIBUTE_REF, &images[1], sizeof(images[1]));
            if (images[0] && images[1])
            {
                vx_uint32 width[2], height[2];
                vx_fourcc format = FOURCC_VIRT;
                vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_WIDTH, &width[0], sizeof(width[0]));
                vxQueryImage(images[0], VX_IMAGE_ATTRIBUTE_HEIGHT, &height[0], sizeof(height[0]));
                vxQueryImage(images[1], VX_IMAGE_ATTRIBUTE_WIDTH, &width[1], sizeof(width[1]));
                vxQueryImage(images[1], VX_IMAGE_ATTRIBUTE_HEIGHT, &height[1], sizeof(height[1]));
                vxQueryImage(images[1], VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
                if ((format == FOURCC_U8) &&
                    (width[0] == width[1]) &&
                    (height[0] == height[1]))
                {
                    status = VX_SUCCESS;
                }
            }
            vxReleaseParameter(&params[0]);
            vxReleaseParameter(&params[1]);
        }
    }
    return status;
}

static vx_status vxNonMaxSuppressionOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 2)
    {
        vx_parameter param = vxGetParameterByIndex(node, 0);
        if (param)
        {
            vx_image img = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &img, sizeof(img));
            if (img)
            {
                vx_fourcc format = FOURCC_VIRT;
                vx_uint32 width, height;
                vxQueryImage(img, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
                vxQueryImage(img, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
                vxQueryImage(img, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
                status = VX_SUCCESS;
                ptr->type = VX_TYPE_IMAGE;
                ptr->dim.image.format = format; /* takes the format of the magnitude image */
                ptr->dim.image.width = width;
                ptr->dim.image.height = height;
            }
            vxReleaseParameter(&param);
        }
    }
    return status;
}

static vx_status vxEuclideanNonMaxOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 3)
    {
        vx_parameter param = vxGetParameterByIndex(node, 0);
        if (param)
        {
            vx_image img = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &img, sizeof(img));
            if (img)
            {
                vx_fourcc format = FOURCC_VIRT;
                vx_uint32 width, height;
                vxQueryImage(img, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
                vxQueryImage(img, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
                vxQueryImage(img, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
                status = VX_SUCCESS;
                ptr->type = VX_TYPE_IMAGE;
                ptr->dim.image.format = format; /* takes the format of the input image */
                ptr->dim.image.width = width;
                ptr->dim.image.height = height;
            }
            vxReleaseParameter(&param);
        }
    }
    return status;
}

static vx_param_description_t nonmaxsuppression_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

static vx_param_description_t euclidean_non_max_suppression_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t nonmax_kernel = {
    VX_KERNEL_EXTRAS_NONMAXSUPPRESSION,
    "org.khronos.extra.nonmaximasuppression",
    vxNonMaxSuppressionKernel,
    nonmaxsuppression_params, dimof(nonmaxsuppression_params),
    vxNonMaxSuppressionInputValidator,
    vxNonMaxSuppressionOutputValidator,
    NULL,
    NULL,
};

vx_kernel_description_t euclidian_nonmax_kernel = {
    VX_KERNEL_EXTRAS_EUCLIDEAN_NONMAXSUPPRESSION,
    "org.khronos.extra.euclidean_nonmaxsuppression",
    vxEuclideanNonMaxSuppressionKernel,
    euclidean_non_max_suppression_params, dimof(euclidean_non_max_suppression_params),
    vxEuclideanNonMaxInputValidator,
    vxEuclideanNonMaxOutputValidator,
};
