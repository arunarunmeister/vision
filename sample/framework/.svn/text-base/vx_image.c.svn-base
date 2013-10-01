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

static vx_uint32 vxComputePatchOffset(vx_uint32 x, vx_uint32 y, vx_imagepatch_addressing_t *addr)
{
    return ((addr->stride_y * ((addr->scale_y * y)/VX_SCALE_UNITY)) +
             (addr->stride_x * ((addr->scale_x * x)/VX_SCALE_UNITY)));
}

static vx_uint32 vxComputePlaneOffset(vx_image_t *image, vx_uint32 x, vx_uint32 y, vx_uint32 p)
{
    return  ((y * image->memory.strides[p][VX_DIM_Y] * image->scale[p][VX_DIM_Y]) +
             (x * image->memory.strides[p][VX_DIM_X] * image->scale[p][VX_DIM_X]));
}

static vx_uint32 vxComputePatchRangeSize(vx_uint32 range, vx_imagepatch_addressing_t *addr)
{
    return (range * addr->stride_x * addr->scale_x) / VX_SCALE_UNITY;
}

static vx_uint32 vxComputePlaneRangeSize(vx_image_t *image, vx_uint32 range, vx_uint32 p)
{
    return (range * image->memory.dims[p][VX_DIM_C] * image->scale[p][VX_DIM_X]);
}

static vx_uint32 vxComputePlaneSize(vx_image_t *image, vx_uint32 p)
{
    /* stride y contains the x dim * x stride. */
    return ((image->memory.dims[p][VX_DIM_Y]/image->scale[p][VX_DIM_Y]) * image->memory.strides[p][VX_DIM_Y]);
}

vx_bool vxIsValidImage(vx_image_t *img)
{
    if (vxIsValidReference(&img->base) &&
        vxIsSupportedFourcc(img->format) == vx_true_e)
    {
        return vx_true_e;
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid Image!\n");
        return vx_false_e;
    }
}

vx_bool vxIsSupportedFourcc(vx_fourcc code)
{
    switch (code)
    {
        case FOURCC_RGB:
        case FOURCC_RGBX:
        case FOURCC_NV12:
        case FOURCC_NV21:
        case FOURCC_UYVY:
        case FOURCC_YUYV:
        case FOURCC_IYUV:
        case FOURCC_YUV4:
        case FOURCC_U8:
        case FOURCC_U16:
        case FOURCC_S16:
        case FOURCC_U32:
        case FOURCC_S32:
        case FOURCC_VIRT:
            return vx_true_e;
        default:
            return vx_false_e;
    }
}

void vxInitPlane(vx_image_t *image,
                 vx_uint32 index,
                 vx_uint32 bpp,
                 vx_uint32 width,
                 vx_uint32 height)
{
    if (image)
    {
        image->memory.dims[index][VX_DIM_C] = bpp;
        image->memory.dims[index][VX_DIM_X] = width;
        image->memory.dims[index][VX_DIM_Y] = height;
        image->memory.ndims = VX_DIM_MAX;
        image->scale[index][VX_DIM_C] = 1;
        image->scale[index][VX_DIM_X] = 1;
        image->scale[index][VX_DIM_Y] = 1;
        image->bounds[index][VX_DIM_C][VX_BOUND_START] = 0;
        image->bounds[index][VX_DIM_C][VX_BOUND_END] = bpp;
        image->bounds[index][VX_DIM_X][VX_BOUND_START] = 0;
        image->bounds[index][VX_DIM_X][VX_BOUND_END] = width;
        image->bounds[index][VX_DIM_Y][VX_BOUND_START] = 0;
        image->bounds[index][VX_DIM_Y][VX_BOUND_END] = height;
    }
}

void vxInitImage(vx_image_t *image, vx_uint32 width, vx_uint32 height, vx_fourcc color)
{
    image->width = width;
    image->height = height;
    image->format = color;
    image->range = VX_CHANNEL_RANGE_FULL;
    /* when an image is allocated, it's not valid until it's been written to.
     * this inverted rectangle is needed for the initial write case.
     */
    image->region.sx = width;
    image->region.sy = height;
    image->region.ex = 0;
    image->region.ey = 0;
    switch (image->format)
    {
        case FOURCC_U8:
        case FOURCC_U16:
        case FOURCC_U32:
        case FOURCC_S16:
        case FOURCC_S32:
            image->space = VX_COLOR_SPACE_NONE;
            break;
        default:
            image->space = VX_COLOR_SPACE_DEFAULT;
            break;
    }

    switch (image->format)
    {
        case FOURCC_VIRT:
            break;
        case FOURCC_NV12:
        case FOURCC_NV21:
            image->planes = 2;
            vxInitPlane(image, 0, sizeof(vx_uint8), image->width, image->height);
            vxInitPlane(image, 1, sizeof(vx_uint16), image->width/2, image->height/2);
            image->scale[1][VX_DIM_X] = 2;
            image->scale[1][VX_DIM_Y] = 2;
            image->bounds[1][VX_DIM_X][VX_BOUND_END] *= image->scale[1][VX_DIM_X];
            image->bounds[1][VX_DIM_Y][VX_BOUND_END] *= image->scale[1][VX_DIM_Y];
            break;
        case FOURCC_RGB:
            image->planes = 1;
            vxInitPlane(image, 0, sizeof(vx_uint8)*3, image->width, image->height);
            break;
        case FOURCC_RGBX:
            image->planes = 1;
            vxInitPlane(image, 0, sizeof(vx_uint8)*4, image->width, image->height);
            break;
        case FOURCC_UYVY:
        case FOURCC_YUYV:
            image->planes = 1;
            vxInitPlane(image, 0, sizeof(vx_uint16), image->width, image->height);
            break;
        case FOURCC_YUV4:
            image->planes = 3;
            vxInitPlane(image, 0, sizeof(vx_uint8), image->width, image->height);
            vxInitPlane(image, 1, sizeof(vx_uint8), image->width, image->height);
            vxInitPlane(image, 2, sizeof(vx_uint8), image->width, image->height);
            break;
        case FOURCC_IYUV:
            image->planes = 3;
            vxInitPlane(image, 0, sizeof(vx_uint8), image->width, image->height);
            vxInitPlane(image, 1, sizeof(vx_uint8), image->width/2, image->height/2);
            vxInitPlane(image, 2, sizeof(vx_uint8), image->width/2, image->height/2);
            image->scale[1][VX_DIM_X] = 2;
            image->scale[1][VX_DIM_Y] = 2;
            image->scale[2][VX_DIM_X] = 2;
            image->scale[2][VX_DIM_Y] = 2;
            image->bounds[1][VX_DIM_X][VX_BOUND_END] *= image->scale[1][VX_DIM_X];
            image->bounds[1][VX_DIM_Y][VX_BOUND_END] *= image->scale[1][VX_DIM_Y];
            image->bounds[2][VX_DIM_X][VX_BOUND_END] *= image->scale[2][VX_DIM_X];
            image->bounds[2][VX_DIM_Y][VX_BOUND_END] *= image->scale[2][VX_DIM_Y];
            break;
        case FOURCC_U8:
            image->planes = 1;
            vxInitPlane(image, 0, sizeof(vx_uint8), image->width, image->height);
            break;
        case FOURCC_U16:
        case FOURCC_S16:
            image->planes = 1;
            vxInitPlane(image, 0, sizeof(vx_uint16), image->width, image->height);
            break;
        case FOURCC_U32:
        case FOURCC_S32:
            image->planes = 1;
            vxInitPlane(image, 0, sizeof(vx_uint32), image->width, image->height);
            break;
        default:
            /*! should not get here unless there's a bug in the
             * vxIsSupportedFourcc call.
             */
            VX_PRINT(VX_ZONE_ERROR, "#################################################\n");
            VX_PRINT(VX_ZONE_ERROR, "Unsupported IMAGE FORMAT!!!\n");
            VX_PRINT(VX_ZONE_ERROR, "#################################################\n");
            break;
    }
    image->memory.nptrs = image->planes;
    vxPrintImage(image);
}

void vxFreeImage(vx_image_t *image)
{
    vxFreeMemory(image->base.context, &image->memory);
}

vx_bool vxAllocateImage(vx_image_t *image)
{
    vx_bool ret = vxAllocateMemory(image->base.context, &image->memory);
    vxPrintMemory(&image->memory);
    return ret;
}

void vxPrintImage(vx_image_t *image)
{
    vx_uint32 p = 0;
    vx_char fourcc[5];
    strncpy(fourcc, (char *)&image->format, 4);
    fourcc[4] = '\0';
    vxPrintReference(&image->base);
    VX_PRINT(VX_ZONE_IMAGE, "vx_image_t:%p %s %ux%u (%s)\n", image, fourcc, image->width, image->height, (image->constant?"CONSTANT":"MUTABLE"));
    for (p = 0; p < image->planes; p++)
    {
        VX_PRINT(VX_ZONE_IMAGE, "\tplane[%u] ptr:%p dim={%u,%u,%u} stride={%u,%u,%u} scale={%u,%u,%u} bounds={%u,%ux%u,%u}\n",
                     p,
                     image->memory.ptrs[p],
                     image->memory.dims[p][VX_DIM_C],
                     image->memory.dims[p][VX_DIM_X],
                     image->memory.dims[p][VX_DIM_Y],
                     image->memory.strides[p][VX_DIM_C],
                     image->memory.strides[p][VX_DIM_X],
                     image->memory.strides[p][VX_DIM_Y],
                     image->scale[p][VX_DIM_C],
                     image->scale[p][VX_DIM_X],
                     image->scale[p][VX_DIM_Y],
                     image->bounds[p][VX_DIM_X][VX_BOUND_START],
                     image->bounds[p][VX_DIM_X][VX_BOUND_END],
                     image->bounds[p][VX_DIM_Y][VX_BOUND_START],
                     image->bounds[p][VX_DIM_Y][VX_BOUND_END]);
    }
}

void vxPrintImageAddressing(vx_imagepatch_addressing_t *addr)
{
    if (addr)
    {
        VX_PRINT(VX_ZONE_IMAGE, "addr:%p dim={%u,%u} stride={%d,%d} scale={%u,%u} step={%u,%u}\n",
                addr,
                addr->dim_x, addr->dim_y,
                addr->stride_x, addr->stride_y,
                addr->scale_x, addr->scale_y,
                addr->step_x, addr->step_y);
    }
}

vx_image vxCreateImageFromROI(vx_image img, vx_rectangle r)
{
    vx_image_t *subimage = NULL;
    vx_image_t *image = (vx_image_t *)img;
    vx_rectangle_t *rect = (vx_rectangle_t *)r;
    if ((vxIsValidImage(image) == vx_true_e) &&
        (vxIsValidSpecificReference(&rect->base, VX_TYPE_RECTANGLE) == vx_true_e))
    {
        /* perhaps the parent hasn't been allocated yet? */
        if (vxAllocateMemory(image->base.context, &image->memory) == vx_true_e)
        {
            subimage = VX_CALLOC(vx_image_t);
            if (subimage)
            {
                vx_uint32 p = 0;
                vxInitReference(&subimage->base, image->base.context, VX_TYPE_IMAGE);
                vxIncrementReference(&subimage->base);
                vxAddReference(image->base.context, &subimage->base);
                /* refer to our parent image and internally refcount it */
                subimage->parent = image;
                vxIncrementIntReference(&image->base);
                VX_PRINT(VX_ZONE_IMAGE, "Creating SubImage at {%u,%u},{%u,%u}\n",
                        rect->sx, rect->sy, rect->ex, rect->ey);
                vxPrintImage(image);
                /* duplicate the metadata */
                subimage->format = image->format;
                subimage->range = image->range;
                subimage->space = image->space;
                subimage->width = rect->ex - rect->sx;
                subimage->height = rect->ey - rect->sy;
                subimage->planes = image->planes;
                memcpy(&subimage->scale, &image->scale, sizeof(image->scale));
                memcpy(&subimage->memory, &image->memory, sizeof(image->memory));
                /* modify the dimensions */
                for (p = 0; p < subimage->planes; p++)
                {
                    vx_uint32 offset = vxComputePlaneOffset(image, rect->sx, rect->sy, p);
                    VX_PRINT(VX_ZONE_IMAGE, "Offsetting SubImage plane[%u] by %u bytes!\n", p, offset);
                    subimage->memory.dims[p][VX_DIM_X] = subimage->width;
                    subimage->memory.dims[p][VX_DIM_Y] = subimage->height;
                    subimage->memory.ptrs[p] = &image->memory.ptrs[p][offset];
                }
                vxPrintImage(subimage);
            }
            else
            {
                VX_PRINT(VX_ZONE_ERROR, "Child image failed to allocate!\n");
            }
        }
        else
        {
            VX_PRINT(VX_ZONE_ERROR, "Parent image failed to allocate!\n");
        }
    }
    return (vx_image)subimage;
}

vx_image vxCreateImage(vx_context c, vx_uint32 width, vx_uint32 height, vx_fourcc color) {
    vx_image_t *image = NULL;
    vx_context_t *context = (vx_context_t *)c;

    if ((vxIsValidContext(context) == vx_true_e) &&
        (vxIsSupportedFourcc(color) == vx_true_e))
    {
        image = VX_CALLOC(vx_image_t);
        if (image)
        {
            vxInitReference((vx_reference_t *)image, context, VX_TYPE_IMAGE);
            vxIncrementReference(&image->base);
            vxAddReference(context, (vx_reference_t *)image);
            vxInitImage(image, width, height, color);
        }
        else
        {
            VX_PRINT(VX_ZONE_IMAGE, "Image could not be allocated!\n");
            vxAddLogEntry(c, VX_ERROR_NO_MEMORY, "Image could not be allocated!\n");
        }
    }
    else
    {
        VX_PRINT(VX_ZONE_IMAGE, "Requested Image Format was invalid!\n");
        vxAddLogEntry(c, VX_ERROR_INVALID_FORMAT, "Requested Image Format was invalid!\n");
    }
    return (vx_image)image;
}

vx_image vxCreateUniformImage(vx_context c, vx_uint32 width, vx_uint32 height, vx_fourcc format, void *pixel)
{
    vx_image image = 0;
    if (pixel == NULL)
        return image;
    image = vxCreateImage(c, width, height, format);
    if (image)
    {
        vx_uint32 x, y, p, planes = 0;
        vx_rectangle rect = vxCreateRectangle(c, 0, 0, width, height);
        vxQueryImage(image, VX_IMAGE_ATTRIBUTE_PLANES, &planes, sizeof(planes));
        for (p = 0; p < planes; p++)
        {
            vx_imagepatch_addressing_t addr;
            void *base = NULL;
            if (vxAccessImagePatch(image, rect, p, &addr, &base) == VX_SUCCESS)
            {
                vxPrintImageAddressing(&addr);
                for (y = 0; y < addr.dim_y; y+=addr.step_y)
                {
                    for (x = 0; x < addr.dim_x; x+=addr.step_x)
                    {
                        if (format == FOURCC_U8)
                        {
                            vx_uint8 *ptr = vxFormatImagePatchAddress2d(base, x, y, &addr);
                            *ptr = *(vx_uint8 *)pixel;
                        }
                        else if (format == FOURCC_U16)
                        {
                            vx_uint16 *ptr = vxFormatImagePatchAddress2d(base, x, y, &addr);
                            *ptr = *(vx_uint16 *)pixel;
                        }
                        else if (format == FOURCC_U32)
                        {
                            vx_uint32 *ptr = vxFormatImagePatchAddress2d(base, x, y, &addr);
                            *ptr = *(vx_uint32 *)pixel;
                        }
                        else if (format == FOURCC_S16)
                        {
                            vx_int16 *ptr = vxFormatImagePatchAddress2d(base, x, y, &addr);
                            *ptr = *(vx_int16 *)pixel;
                        }
                        else if (format == FOURCC_S32)
                        {
                            vx_int32 *ptr = vxFormatImagePatchAddress2d(base, x, y, &addr);
                            *ptr = *(vx_int32 *)pixel;
                        }
                        else if ((format == FOURCC_RGB)  ||
                                 (format == FOURCC_RGBX) ||
                                 (format == FOURCC_YUV4) ||
                                 (format == FOURCC_IYUV))
                        {
                            vx_uint8 *ptr = vxFormatImagePatchAddress2d(base, x, y, &addr);
                            *ptr = ((vx_uint8 *)pixel)[p];
                        }
                        else if ((p == 0) &&
                                 ((format == FOURCC_NV12) ||
                                  (format == FOURCC_NV21)))
                        {
                            vx_uint8 *ptr = vxFormatImagePatchAddress2d(base, x, y, &addr);
                            *ptr = ((vx_uint8 *)pixel)[p];
                        }
                        else if ((p == 1) && (format == FOURCC_NV12))
                        {
                            vx_uint8 *ptr = vxFormatImagePatchAddress2d(base, x, y, &addr);
                            ptr[0] = ((vx_uint8 *)pixel)[1];
                            ptr[1] = ((vx_uint8 *)pixel)[2];
                        }
                        else if ((p == 1) && (format == FOURCC_NV21))
                        {
                            vx_uint8 *ptr = vxFormatImagePatchAddress2d(base, x, y, &addr);
                            ptr[1] = ((vx_uint8 *)pixel)[1];
                            ptr[0] = ((vx_uint8 *)pixel)[2];
                        }
                        else if (format == FOURCC_UYVY)
                        {
                            vx_uint8 *ptr = vxFormatImagePatchAddress2d(base, x, y, &addr);
                            ptr[0] = ((vx_uint8 *)pixel)[1];
                            ptr[1] = ((vx_uint8 *)pixel)[0];
                            ptr[2] = ((vx_uint8 *)pixel)[2];
                            ptr[3] = ((vx_uint8 *)pixel)[0];
                        }
                        else if (format == FOURCC_YUYV)
                        {
                            vx_uint8 *ptr = vxFormatImagePatchAddress2d(base, x, y, &addr);
                            ptr[0] = ((vx_uint8 *)pixel)[0];
                            ptr[1] = ((vx_uint8 *)pixel)[1];
                            ptr[2] = ((vx_uint8 *)pixel)[0];
                            ptr[3] = ((vx_uint8 *)pixel)[2];
                        }
                    }
                }
                if (vxCommitImagePatch(image, rect, p, &addr, base) != VX_SUCCESS)
                {
                    VX_PRINT(VX_ZONE_ERROR, "Failed to set initial image patch on plane %u on const image!\n", p);
                    vxReleaseImage(&image);
                    image = 0;
                    break;
                }
            }
            else
            {
                VX_PRINT(VX_ZONE_ERROR, "Failed to get image patch on plane %u in const image!\n",p);
                vxReleaseImage(&image);
                image = 0;
                break;
            }
        } /* for loop */
        if (image)
        {
            /* lock the image from being modified again! */
            ((vx_image_t *)image)->constant = vx_true_e;
        }
        vxReleaseRectangle(&rect);
    }
    return image;
}

vx_image vxCreateVirtualImage(vx_context c)
{
    return vxCreateImage(c, 0, 0, FOURCC_VIRT);
}

vx_image vxCreateVirtualImageWithFormat(vx_context c, vx_fourcc format)
{
    return vxCreateImage(c, 0, 0, format);
}

vx_image vxCreateImageFromHandle(vx_context c, vx_fourcc color, vx_imagepatch_addressing_t addrs[], void *ptrs[], vx_enum type)
{
    vx_image img = 0;

    if (vxIsValidImport(type) == vx_false_e)
        return 0;

    img = vxCreateImage(c, addrs[0].dim_x, addrs[0].dim_y, color);
    if (img)
    {
        vx_uint32 p = 0;
        vx_uint32 index = 0;
        vx_image_t *image = (vx_image_t *)img;
        image->import = type;
        /* now assign the plane pointers, assume linearity */
        for (p = 0; p < image->planes; p++)
        {
            image->memory.ptrs[p] = ptrs[p];
            image->memory.strides[p][VX_DIM_C] = 1;
            image->memory.strides[p][VX_DIM_X] = addrs[p].stride_x;
            image->memory.strides[p][VX_DIM_Y] = addrs[p].stride_y;
        }
    }
    return img;
}

vx_image vxCreateVirtualImageWithDimension(vx_context c, vx_uint32 width, vx_uint32 height)
{
    return vxCreateImage(c, width, height, FOURCC_VIRT);
}

vx_status vxQueryImage(vx_image image, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_image_t *img = (vx_image_t *)image;
    if (vxIsValidImage(img) == vx_true_e)
    {
        switch (attribute)
        {
            case VX_IMAGE_ATTRIBUTE_FORMAT:
                if (VX_CHECK_PARAM(ptr, size, vx_fourcc, 0x3))
                {
                    *(vx_fourcc *)ptr = img->format;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_IMAGE_ATTRIBUTE_WIDTH:
                if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
                {
                    *(vx_uint32 *)ptr = img->width;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_IMAGE_ATTRIBUTE_HEIGHT:
                if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
                {
                    *(vx_uint32 *)ptr = img->height;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_IMAGE_ATTRIBUTE_PLANES:
                if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
                {
                    *(vx_uint32 *)ptr = img->planes;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_IMAGE_ATTRIBUTE_SPACE:
                if (VX_CHECK_PARAM(ptr, size, vx_enum, 0x3))
                {
                    *(vx_enum *)ptr = img->space;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_IMAGE_ATTRIBUTE_RANGE:
                if (VX_CHECK_PARAM(ptr, size, vx_enum, 0x3))
                {
                    *(vx_enum *)ptr = img->range;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_IMAGE_ATTRIBUTE_SIZE:
                if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
                {
                    vx_size size = 0ul;
                    vx_uint32 p;
                    for (p = 0; p < img->planes; p++)
                    {
                        size += (abs(img->memory.strides[p][VX_DIM_Y]) * img->memory.dims[p][VX_DIM_Y]);
                    }
                    *(vx_size *)ptr = size;
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
    else
    {
        status = VX_ERROR_INVALID_REFERENCE;
    }
    VX_PRINT(VX_ZONE_API, "%s returned %d\n", __FUNCTION__, status);
    return status;
}

vx_status vxSetImageAttribute(vx_image image, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_image_t *img = (vx_image_t *)image;
    if (vxIsValidImage(img) == vx_true_e)
    {
        switch (attribute)
        {
            case VX_IMAGE_ATTRIBUTE_SPACE:
                if (VX_CHECK_PARAM(ptr, size, vx_enum, 0x3))
                {
                    img->space = *(vx_enum *)ptr;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_IMAGE_ATTRIBUTE_RANGE:
                if (VX_CHECK_PARAM(ptr, size, vx_enum, 0x3))
                {
                    *(vx_enum *)ptr = img->range;
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
    else
    {
        status = VX_ERROR_INVALID_REFERENCE;
    }
    VX_PRINT(VX_ZONE_API, "%s returned %d\n", __FUNCTION__, status);
    return status;
}

void vxDestructImage(vx_reference_t *ref)
{
    vx_image_t *img = (vx_image_t *)ref;
    if ((img->import == VX_IMPORT_TYPE_NONE) ||
        (img->parent == NULL))
    {
        vxFreeImage(img);
    }
    else if (img->parent)
    {
        vxDecrementIntReference((vx_reference_t *)img->parent);
    }
}

void vxReleaseImageInt(vx_image_t *img)
{
    vxReleaseReference((vx_reference_t *)img, VX_TYPE_IMAGE, vx_true_e, vxDestructImage);
}

void vxReleaseImage(vx_image *i)
{
    vx_image_t *img = (vx_image_t *)(i?*i:0);
    vxReleaseReference((vx_reference_t *)img, VX_TYPE_IMAGE, vx_false_e, vxDestructImage);
    if (i) *i = 0;
}

vx_size vxComputeImagePatchSize(vx_image image,
                                vx_rectangle rect,
                                vx_uint32 plane_index)
{
    vx_size size = 0ul;
    vx_image_t *img = (vx_image_t *)image;
    vx_uint32 start_x = 0u, start_y = 0u, end_x = 0u, end_y = 0u;

    vxQueryRectangle(rect, VX_RECTANGLE_ATTRIBUTE_START_X, &start_x, sizeof(start_x));
    vxQueryRectangle(rect, VX_RECTANGLE_ATTRIBUTE_START_Y, &start_y, sizeof(start_y));
    vxQueryRectangle(rect, VX_RECTANGLE_ATTRIBUTE_END_X, &end_x, sizeof(end_x));
    vxQueryRectangle(rect, VX_RECTANGLE_ATTRIBUTE_END_Y, &end_y, sizeof(end_y));

    if (vxIsValidImage(img) == vx_true_e)
    {
        if (img->memory.ptrs[0] == NULL)
        {
            if (vxAllocateImage(img) == vx_false_e)
            {
                vxAddLogEntry(image, VX_ERROR_NO_MEMORY, "Failed to allocate image!\n");
                return 0;
            }
        }
        if (plane_index < img->planes)
        {
            vx_size numPixels = ((end_x-start_x)/img->scale[plane_index][VX_DIM_X]) *
                                ((end_y-start_y)/img->scale[plane_index][VX_DIM_Y]);
            vx_size pixelSize = img->memory.strides[plane_index][VX_DIM_X];
            vxPrintImage(img);
            VX_PRINT(VX_ZONE_IMAGE, "numPixels = "VX_FMT_SIZE" pixelSize = "VX_FMT_SIZE"\n", numPixels, pixelSize);
            size = numPixels * pixelSize;
        }
        else
        {
            vxAddLogEntry(image, VX_ERROR_INVALID_PARAMETERS, "Plane index %u is out of bounds!", plane_index);
        }

        VX_PRINT(VX_ZONE_IMAGE, "image %p for patch {%u,%u to %u,%u} has a byte size of "VX_FMT_SIZE"\n",
                 img, start_x, start_y, end_x, end_y, size);
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Image Reference is invalid!\n");
    }
    return size;
}

vx_status vxAccessImagePatch(vx_image image,
                          vx_rectangle rect,
                          vx_uint32 plane_index,
                          vx_imagepatch_addressing_t *addr,
                          void **ptr)
{
    vx_image_t *img = (vx_image_t *)image;
    vx_uint8 *p = NULL;
    vx_uint32 start_x = 0u;
    vx_uint32 start_y = 0u;
    vx_uint32 end_x = 0u;
    vx_uint32 end_y = 0u;
    vx_status status = VX_ERROR_INVALID_REFERENCE;

    /*! \bug Should you be able to Get an "blank" image patch for accessing reasons? */
    if ((rect) &&
        (vxIsValidSpecificReference((vx_reference_t *)rect, VX_TYPE_RECTANGLE) == vx_true_e) &&
        (vxIsValidImage(img) == vx_true_e))
    {
        /* verify has not been call yet or will not be called (immediate mode)
         * this is a trick to "touch" an image so that it can be created
         */
        if ((img->memory.ptrs[0] == NULL) && (vxAllocateImage(img) == vx_false_e))
        {
            status = VX_ERROR_NO_MEMORY;
        }
        else
        {
            vxQueryRectangle(rect, VX_RECTANGLE_ATTRIBUTE_START_X, &start_x, sizeof(start_x));
            vxQueryRectangle(rect, VX_RECTANGLE_ATTRIBUTE_START_Y, &start_y, sizeof(start_y));
            vxQueryRectangle(rect, VX_RECTANGLE_ATTRIBUTE_END_X, &end_x, sizeof(end_x));
            vxQueryRectangle(rect, VX_RECTANGLE_ATTRIBUTE_END_Y, &end_y, sizeof(end_y));

            if ((plane_index >= img->planes) ||
                (start_x >= end_x) ||
                (start_y >= end_y) ||
                (addr == NULL) ||
                (ptr == NULL))
            {
                VX_PRINT(VX_ZONE_ERROR, "Invalid parameters! pi:%u ip:%u {%u,%u},{%u,%u},addr=%p,ptr=%p\n",
                        plane_index, img->planes, start_x, start_y, end_x, end_y, addr, ptr);
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            else
            {
                VX_PRINT(VX_ZONE_IMAGE, "GetImagePatch to %p from {%u,%u} to {%u,%u}\n", *ptr, start_x, start_y, end_x, end_y);
                // if the pointer is not set, get the direct subpointer in this implementation.
                if (*ptr == NULL && img->constant == vx_false_e)
                {
                    vx_uint32 index = 0;
                    vxSemWait(&img->base.lock);
                    p = (vx_uint8 *)img->memory.ptrs[plane_index];
                    /* use the addressing of the internal format */
                    addr->dim_x = end_x - start_x;
                    addr->dim_y = end_y - start_y;
                    addr->stride_x = img->memory.strides[plane_index][VX_DIM_X];
                    addr->stride_y = img->memory.strides[plane_index][VX_DIM_Y];
                    addr->step_x = img->scale[plane_index][VX_DIM_X];
                    addr->step_y = img->scale[plane_index][VX_DIM_Y];
                    addr->scale_x = VX_SCALE_UNITY / img->scale[plane_index][VX_DIM_X];
                    addr->scale_y = VX_SCALE_UNITY / img->scale[plane_index][VX_DIM_Y];
                    index = vxComputePatchOffset(start_x, start_y, addr);
                    *ptr = &p[index];
                    VX_PRINT(VX_ZONE_IMAGE, "Returning mapped pointer %p\n", *ptr);
                    vxSemPost(&img->base.lock);
                    vxReadFromReference(&img->base);
                    vxIncrementReference(&img->base);
                    status = VX_SUCCESS;
                }
                else if (*ptr == NULL && img->constant == vx_true_e)
                {
                    VX_PRINT(VX_ZONE_ERROR, "Can't map a pointer to constant data, only copy!\n");
                    status = VX_ERROR_NOT_SUPPORTED;
                }
                else
                {
                    vx_uint32 y, i, j, len;
                    vx_uint8 *tmp = *ptr;
                    vxSemWait(&img->base.lock);
                    /* use the dimensionality of the flat buffer. */
                    addr->dim_x = end_x - start_x;
                    addr->dim_y = end_y - start_y;
                    addr->stride_x = img->memory.strides[plane_index][VX_DIM_X];
                    addr->stride_y = addr->dim_x * addr->stride_x;
                    addr->step_x = img->scale[plane_index][VX_DIM_X];
                    addr->step_y = img->scale[plane_index][VX_DIM_Y];
                    addr->scale_x = VX_SCALE_UNITY / img->scale[plane_index][VX_DIM_X];
                    addr->scale_y = VX_SCALE_UNITY / img->scale[plane_index][VX_DIM_Y];
                    for (y = start_y; y < end_y; y+=addr->step_y)
                    {
                        i = vxComputePlaneOffset(img, start_x, y, plane_index);
                        j = vxComputePatchOffset(0, (y - start_y), addr);
                        len = vxComputePlaneRangeSize(img, (end_x - start_x), plane_index);
                        VX_PRINT(VX_ZONE_IMAGE, "%p[%u] <= %p[%u] for %u\n", tmp, j, img->memory.ptrs[plane_index], i, len);
                        memcpy(&tmp[j], &img->memory.ptrs[plane_index][i], len);
                    }
                    VX_PRINT(VX_ZONE_IMAGE, "Copied image into %p\n", *ptr);
                    vxSemPost(&img->base.lock);
                    vxReadFromReference(&img->base);
                    vxIncrementReference(&img->base);
                    status = VX_SUCCESS;
                }
            }
        }
    }
    VX_PRINT(VX_ZONE_API, "returned %d\n", status);
    return status;
}

vx_status vxCommitImagePatch(vx_image image,
                          vx_rectangle rect,
                          vx_uint32 plane_index,
                          vx_imagepatch_addressing_t *addr,
                          void *ptr)
{
    vx_status status = VX_ERROR_INVALID_REFERENCE;
    vx_int32 i = 0;
    vx_image_t *img = (vx_image_t*)image;
    vx_bool allocated = vx_true_e; // assume that it was an allocated buffer
    vx_uint32 start_x = 0u;
    vx_uint32 start_y = 0u;
    vx_uint32 end_x = 0u;
    vx_uint32 end_y = 0u;
    vx_uint8 *tmp = (vx_uint8 *)ptr;

    if (vxIsValidImage(img) == vx_true_e)
    {
        // if it's null, the rect coordinates will be zero
        vxAccessRectangleCoordinates(rect, &start_x, &start_y, &end_x, &end_y);

        VX_PRINT(VX_ZONE_IMAGE, "Setting Image Patch from ptr %p plane %u to {%u,%u},{%u,%u}\n",
            ptr, plane_index, start_x, start_y, end_x, end_y);

        vxPrintImage(img);
        vxPrintImageAddressing(addr);

        /* check the coordinates, they have to be in actual plane space */
        if (((end_x != 0) && (end_y != 0)) &&
            !((plane_index < img->planes) &&
              (ptr) && (addr) &&
              (start_x < end_x) && ((end_x - start_x) <= addr->dim_x) &&
              (start_y < end_y) && ((end_y - start_y) <= addr->dim_y) &&
              (end_x <= (vx_uint32)img->memory.dims[plane_index][VX_DIM_X] * (vx_uint32)img->scale[plane_index][VX_DIM_X]) &&
              (end_y <= (vx_uint32)img->memory.dims[plane_index][VX_DIM_Y] * (vx_uint32)img->scale[plane_index][VX_DIM_X])))
        {
            VX_PRINT(VX_ZONE_ERROR, "Invalid start,end coordinates! plane %u {%u,%u},{%u,%u}\n",
                     plane_index, start_x, start_y, end_x, end_y);
            vxPrintImage(img);
            status = VX_ERROR_INVALID_PARAMETERS;
        }
        else
        {
            /* if we aren't just freeing the image */
            if (!(start_x == 0 && start_y == 0 && end_x == 0 && end_y == 0) &&
                img->constant == vx_false_e)
            {
                vxSemWait(&img->base.lock);
                /* determine if this grows the valid region */
                if (img->region.sx > start_x)
                    img->region.sx = start_x;
                if (img->region.sy > start_y)
                    img->region.sy = start_y;
                if (img->region.ex < end_x)
                    img->region.ex = end_x;
                if (img->region.ey < end_y)
                    img->region.ey = end_y;

                /* index of 1 pixel line past last. */
                i = (img->memory.dims[plane_index][VX_DIM_Y] * img->memory.strides[plane_index][VX_DIM_Y]);

                VX_PRINT(VX_ZONE_IMAGE, "base:%p tmp:%p end:%p\n",
                    img->memory.ptrs[plane_index], ptr, &img->memory.ptrs[plane_index][i]);

                if ((img->memory.ptrs[plane_index] <= (vx_uint8 *)ptr) &&
                    ((vx_uint8 *)ptr < &img->memory.ptrs[plane_index][i]))
                {
                    /* corner case for 2d memory */
                    if (img->memory.strides[plane_index][VX_DIM_Y] !=
                        (img->memory.dims[plane_index][VX_DIM_X] * img->memory.strides[plane_index][VX_DIM_X]))
                    {
                        /* determine if the pointer is within the image boundary. */
                        vx_uint8 *base = img->memory.ptrs[plane_index];
                        vx_size offset = ((vx_size)(tmp - base)) % img->memory.strides[plane_index][VX_DIM_Y];
                        if (offset < (img->memory.dims[plane_index][VX_DIM_X] * img->memory.strides[plane_index][VX_DIM_X]))
                        {
                            VX_PRINT(VX_ZONE_IMAGE, "Pointer is within 2D image\n");
                            allocated = vx_false_e;
                        }
                    }
                    else
                    {
                        /* the pointer in contained in the image, so it was mapped, thus
                         * there's nothing else to do. */
                        allocated = vx_false_e;
                        VX_PRINT(VX_ZONE_IMAGE, "Mapped pointer detected!\n");
                    }
                }
                if (allocated == vx_true_e)
                {
                    /* copy the patch back to the image. */
                    vx_uint32 y, i, j, len;
                    for (y = start_y; y < end_y; y += addr->step_y)
                    {
                        i = vxComputePlaneOffset(img, start_x, y, plane_index);
                        j = vxComputePatchOffset(0, (y - start_y), addr);
                        len = vxComputePatchRangeSize((end_x - start_x), addr);
                        VX_PRINT(VX_ZONE_IMAGE, "%p[%u] <= %p[%u] for %u\n", img->memory.ptrs[plane_index], j, tmp, i, len);
                        memcpy(&img->memory.ptrs[plane_index][i], &tmp[j], len);
                    }
                    VX_PRINT(VX_ZONE_IMAGE, "Client must free pointer %p\n", ptr);
                }
                vxSemPost(&img->base.lock);
                vxWroteToReference(&img->base);
                vxDecrementReference(&img->base);
                status = VX_SUCCESS;
            }
            else if (!(start_x == 0 && start_y == 0 && end_x == 0 && end_y == 0) &&
                (img->constant == vx_true_e))
            {
                /* we tried to modify constant data! */
                VX_PRINT(VX_ZONE_ERROR, "Can't set constant image data!\n");
                status = VX_ERROR_NOT_SUPPORTED;
            }
            else
            {
                VX_PRINT(VX_ZONE_IMAGE, "Decrementing Image Reference\n");
                status = VX_SUCCESS;
                vxDecrementReference(&img->base);
            }
        }
    }
    VX_PRINT(VX_ZONE_API, "return %d\n", status);
    return status;
}

void *vxFormatImagePatchAddress1d(void *ptr, vx_uint32 index, vx_imagepatch_addressing_t *addr)
{
    vx_uint8 *new_ptr = NULL;
    if (ptr && index < addr->dim_x*addr->dim_y)
    {
        vx_uint32 x = index % addr->dim_x;
        vx_uint32 y = index / addr->dim_x;
        vx_uint32 offset = vxComputePatchOffset(x, y, addr);
        new_ptr = (vx_uint8 *)ptr;
        new_ptr = &new_ptr[offset];
    }
    return new_ptr;
}

void *vxFormatImagePatchAddress2d(void *ptr, vx_uint32 x, vx_uint32 y, vx_imagepatch_addressing_t *addr)
{
    vx_uint8 *new_ptr = NULL;
    if (ptr && x < addr->dim_x && y < addr->dim_y)
    {
        vx_uint32 offset = vxComputePatchOffset(x, y, addr);
        new_ptr = (vx_uint8 *)ptr;
        new_ptr = &new_ptr[offset];
    }
    return new_ptr;
}

vx_rectangle vxGetValidRegionImage(vx_image image)
{
    vx_image_t *img = (vx_image_t *)image;
    vx_rectangle rect = 0;
    if (vxIsValidImage(img) == vx_true_e)
    {
        vx_context context = vxGetContext(image);
        /* if the image was just allocated, this will be an null value */
        rect = vxCreateRectangle(context,
                img->region.sx, img->region.sy, img->region.ex, img->region.ey);
        if (rect == 0)
        {
            rect = vxCreateRectangle(context, 0, 0, img->width, img->height);
        }
    }
    return rect;
}

