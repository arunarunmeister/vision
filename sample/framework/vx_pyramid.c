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

#include <vx_internal.h>

void vxDestructPyramid(vx_reference_t *ref)
{
    vx_pyramid_t *pyramid = (vx_pyramid_t *)ref;
    vx_uint32 i = 0;
    for (i = 0; i < pyramid->numLevels; i++)
    {
        if (pyramid->levels[i])
        {
            vxDecrementIntReference((vx_reference_t *)pyramid->levels[i]);
            vxReleaseImage(&pyramid->levels[i]);
        }
    }
    free(pyramid->levels);
}

void vxReleasePyramidInt(vx_pyramid_t *pyramid)
{
    vxReleaseReference((vx_reference_t *)pyramid, VX_TYPE_PYRAMID, vx_false_e, vxDestructPyramid);
}

void vxReleasePyramid(vx_pyramid *pyr)
{
    vx_pyramid_t *pyramid = (vx_pyramid_t *)(pyr?*pyr:0);
    vxReleaseReference((vx_reference_t *)pyramid, VX_TYPE_PYRAMID, vx_false_e, vxDestructPyramid);
    if (pyr) *pyr = 0;
}

vx_pyramid vxCreatePyramid(vx_context c, vx_size levels, vx_float32 scale, vx_uint32 width, vx_uint32 height, vx_fourcc format)
{
    vx_context_t *context = (vx_context_t *)c;
    vx_pyramid_t *pyramid = NULL;
    if ((scale != VX_SCALE_PYRAMID_DOUBLE) &&
        (scale != VX_SCALE_PYRAMID_HALF) &&
        (scale != VX_SCALE_PYRAMID_ORB))
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid scale %lf for pyramid!\n",scale);
        return 0;
    }
    pyramid = VX_CALLOC(vx_pyramid_t);
    if (pyramid)
    {
        vx_uint32 w = 0, h = 0;
        vx_int32 i;
        vxInitReference(&pyramid->base, context, VX_TYPE_PYRAMID);
        vxIncrementReference(&pyramid->base);
        vxAddReference(context, &pyramid->base);
        pyramid->numLevels = levels;
        pyramid->levels = (vx_image *)calloc(levels, sizeof(vx_image_t *));
        if (pyramid->levels)
        {
            w = width;
            h = height;
            for (i = 0; i < pyramid->numLevels; i++)
            {
                VX_PRINT(VX_ZONE_IMAGE, "Creating Level %u %ux%u\n", i, w, h);
                pyramid->levels[i] = vxCreateImage(c, w, h, format);
                if (w != 1 && h != 1)
                {
                    w = (vx_uint32)((vx_float32)w * scale);
                    h = (vx_uint32)((vx_float32)h * scale);
                }
                else
                {
                    vxReleasePyramid((vx_pyramid *)&pyramid);
                    pyramid = NULL;
                    break;
                }
                /* increment the internal counter on the image */
                vxIncrementIntReference((vx_reference_t *)pyramid->levels[i]);
            }
        }
    }
    return (vx_pyramid)pyramid;
}

vx_status vxQueryPyramid(vx_pyramid pyr, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_pyramid_t *pyramid = (vx_pyramid_t *)pyr;
    if (vxIsValidSpecificReference(&pyramid->base, VX_TYPE_PYRAMID) == vx_true_e)
    {
        switch (attribute)
        {
            case VX_PYRAMID_ATTRIBUTE_LEVELS:
                if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
                {
                    *(vx_size *)ptr = pyramid->numLevels;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_PYRAMID_ATTRIBUTE_SCALE:
                if (VX_CHECK_PARAM(ptr, size, vx_float32, 0x3))
                {
                    *(vx_float32 *)ptr = pyramid->scale;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            default:
                status = VX_ERROR_NOT_SUPPORTED;
                break;
        }
    }
    return status;
}

vx_image vxGetPyramidLevel(vx_pyramid pyr, vx_uint32 index)
{
    vx_image image = 0;
    vx_pyramid_t *pyramid = (vx_pyramid_t *)pyr;
    if (vxIsValidSpecificReference(&pyramid->base, VX_TYPE_PYRAMID) == vx_true_e)
    {
        if (index < pyramid->numLevels)
        {
            image = pyramid->levels[index];
            vxIncrementReference((vx_reference_t *)image);
        }
    }
    return image;
}

