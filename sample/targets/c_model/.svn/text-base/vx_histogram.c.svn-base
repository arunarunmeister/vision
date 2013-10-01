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
 * \brief The Histogram Kernels
 * \author Erik Rainey <erik.rainey@ti.com>
 * \author Shervin Emami <semami@nvidia.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>
#include <math.h>

static vx_status vxHistogramKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 2)
    {
        vx_image src_image = (vx_image)parameters[0];
        vx_distribution dist = (vx_scalar)parameters[1];
        vx_rectangle src_rect;
        vx_imagepatch_addressing_t src_addr;
        void *src_base = NULL, *dist_ptr = NULL;
        vx_fourcc format = 0;
        vx_uint32 y = 0, x = 0;
        vx_uint32 offset = 0, range = 0, numBins = 0, window_size = 0;

        vxQueryImage(src_image, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
        vxQueryDistribution(dist, VX_DISTRIBUTION_ATTRIBUTE_BINS, &numBins, sizeof(numBins));
        vxQueryDistribution(dist, VX_DISTRIBUTION_ATTRIBUTE_RANGE, &range, sizeof(range));
        vxQueryDistribution(dist, VX_DISTRIBUTION_ATTRIBUTE_OFFSET, &offset, sizeof(offset));
        vxQueryDistribution(dist, VX_DISTRIBUTION_ATTRIBUTE_WINDOW, &window_size, sizeof(window_size));
        src_rect = vxGetValidRegionImage(src_image);
        status = VX_SUCCESS;
        status |= vxAccessImagePatch(src_image, src_rect, 0, &src_addr, &src_base);
        status |= vxAccessDistribution(dist, &dist_ptr);
        printf("distribution:%p bins:%u off:%u ws:%u range:%u\n", dist_ptr, numBins, offset, window_size, range);
        if (status == VX_SUCCESS)
        {
            vx_int32 *dist_tmp = dist_ptr;

            /* clear the distribution */
            for (x = 0; x < numBins; x++)
            {
                dist_tmp[x] = 0;
            }

            for (y = 0; y < src_addr.dim_y; y++)
            {
                for (x = 0; x < src_addr.dim_x; x++)
                {
                    if (format == FOURCC_U8)
                    {
                        vx_uint8 *src_ptr = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                        vx_uint8 pixel = *src_ptr;
                        if ((offset <= (vx_size)pixel) && ((vx_size)pixel < (offset+range)))
                        {
                            vx_size index = (pixel - (vx_uint16)offset) / window_size;
                            dist_tmp[index]++;
                        }
                    }
                    else if (format == FOURCC_U16)
                    {
                        vx_uint16 *src_ptr = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                        vx_uint16 pixel = *src_ptr;
                        if ((offset <= (vx_size)pixel) && ((vx_size)pixel < (offset+range)))
                        {
                            vx_size index = (pixel - (vx_uint16)offset) / window_size;
                            dist_tmp[index]++;
                        }
                    }
                }
            }
        }
        status |= vxCommitDistribution(dist, dist_ptr);
        status |= vxCommitImagePatch(src_image, 0, 0, &src_addr, src_base);
        vxReleaseParameter(&src_rect);
    }
    return status;
}

static vx_status vxEqualizeHistKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 2)
    {
        vx_image src = (vx_image)parameters[0];
        vx_image dst = (vx_image)parameters[1];
        vx_uint32 y, x, width = 0, height = 0;
        void *src_base = NULL;
        void *dst_base = NULL;
        vx_imagepatch_addressing_t src_addr, dst_addr;
        vx_rectangle rect;

        status = VX_SUCCESS;
        status |= vxQueryImage(src, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
        status |= vxQueryImage(src, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
        rect = vxCreateRectangle(vxGetContext(node), 0, 0, width, height);
        status |= vxAccessImagePatch(src, rect, 0, &src_addr, &src_base);
        status |= vxAccessImagePatch(dst, rect, 0, &dst_addr, &dst_base);
        if (status == VX_SUCCESS)
        {
            /* for 16-bit support (U16 or S16), the code can be duplicated with NUM_BINS = 65536 and PIXEL = vx_uint16. */
            #define NUM_BINS 256

            /* allocate a fixed-size temp array to store the image histogram & cumulative distribution */
            vx_uint32 hist[NUM_BINS];
            vx_uint32 cdf[NUM_BINS];
            vx_uint32 sum = 0;
            vx_uint32 maxVal = 0;
            vx_float32 scaleFactor = 0.0f;

            /* calculate the distribution (histogram) */
            memset(hist, 0, sizeof(hist));
            for (y = 0; y < height; y++)
            {
                for (x = 0; x < width; x++)
                {
                    vx_uint8 *src_ptr = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                    vx_uint8 pixel = *src_ptr;
                    hist[pixel]++;
                }
            }

            /* calculate the cumulative distribution (summed histogram) */
            for (x = 0; x < NUM_BINS; x++)
            {
                cdf[x] = sum;
                sum += hist[x];
            }

            /* find the scale factor from the max cdf value */
            maxVal = cdf[0];
            for (x = 1; x < NUM_BINS; x++)
            {
                if (maxVal < cdf[x])
                {
                    maxVal = cdf[x];
                }
            }
            scaleFactor = 255.0f / (float)maxVal;
            //printf("* maxVal = %d, scaleFactor = %f\n", maxVal, scaleFactor);

            /* map the src pixel values to the equalized pixel values */
            for (y = 0; y < height; y++)
            {
                for (x = 0; x < width; x++)
                {
                    vx_uint8 *src_ptr = vxFormatImagePatchAddress2d(src_base, x, y, &src_addr);
                    vx_uint8 *dst_ptr = vxFormatImagePatchAddress2d(dst_base, x, y, &dst_addr);
                    vx_uint32 equalized_int = cdf[(*src_ptr)];
                    *dst_ptr = (vx_uint8)(equalized_int * scaleFactor + 0.5f);
                }
            }
        }

        status |= vxCommitImagePatch(src, 0, 0, &src_addr, src_base);
        status |= vxCommitImagePatch(dst, rect, 0, &dst_addr, dst_base);
        vxReleaseRectangle(&rect);
    }
    return status;
}


static vx_status vxHistogramInputValidator(vx_node node, vx_uint32 index)
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
            if (format == FOURCC_U8 || format == FOURCC_U16)
            {
                status = VX_SUCCESS;
            }
        }
        vxReleaseParameter(&param);
    }
    return status;
}

static vx_status vxHistogramOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 1)
    {
        vx_image src = 0;
        vx_parameter src_param = vxGetParameterByIndex(node, 0);
        vx_parameter dst_param = vxGetParameterByIndex(node, 1);
        vx_distribution dist;

        vxQueryParameter(src_param, VX_PARAMETER_ATTRIBUTE_REF, &src, sizeof(src));
        vxQueryParameter(dst_param, VX_PARAMETER_ATTRIBUTE_REF, &dist, sizeof(dist));
        if ((src) && (dist))
        {
            vx_uint32 width = 0, height = 0;
            vx_fourcc format;
            vx_size numBins = 0;
            vxQueryDistribution(dist, VX_DISTRIBUTION_ATTRIBUTE_BINS, &numBins, sizeof(numBins));
            vxQueryImage(src, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(height));
            vxQueryImage(src, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
            vxQueryImage(src, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
            /* fill in the meta data with the attributes so that the checker will pass */
            ptr->type = VX_TYPE_DISTRIBUTION;
            ptr->dim.buffer.unitSize = sizeof(vx_int32);
            ptr->dim.buffer.numUnits = numBins;
            status = VX_SUCCESS;
        }
        vxReleaseParameter(&src_param);
    }
    return status;
}

static vx_status vxEqualizeHistInputValidator(vx_node node, vx_uint32 index)
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

static vx_status vxEqualizeHistOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 1)
    {
        vx_parameter param = vxGetParameterByIndex(node, 0); /* we reference the input image */
        if (param)
        {
            vx_image input = 0;

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
    }
    return status;
}


static vx_param_description_t histogram_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_DISTRIBUTION, VX_PARAMETER_STATE_REQUIRED},
};

static vx_param_description_t equalize_hist_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};


vx_kernel_description_t histogram_kernel = {
    VX_KERNEL_HISTOGRAM,
    "org.khronos.openvx.histogram",
    vxHistogramKernel,
    histogram_kernel_params, dimof(histogram_kernel_params),
    vxHistogramInputValidator,
    vxHistogramOutputValidator,
    NULL,
    NULL,
};

vx_kernel_description_t equalize_hist_kernel = {
    VX_KERNEL_EQUALIZE_HISTOGRAM,
    "org.khronos.openvx.equalize_histogram",
    vxEqualizeHistKernel,
    equalize_hist_kernel_params, dimof(equalize_hist_kernel_params),
    vxEqualizeHistInputValidator,
    vxEqualizeHistOutputValidator,
    NULL,
    NULL,
};

