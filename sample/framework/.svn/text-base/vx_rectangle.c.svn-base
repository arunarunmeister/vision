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

vx_rectangle vxCreateRectangle(vx_context c,
                               vx_uint32 sx,
                               vx_uint32 sy,
                               vx_uint32 ex,
                               vx_uint32 ey)
{
    vx_rectangle_t *rect = NULL;
    vx_context_t *context = (vx_context_t *)c;
    if (vxIsValidContext(context) == vx_true_e)
    {
        if ((sx <= ex) && (sy <= ey))
        {
            rect = VX_CALLOC(vx_rectangle_t);
            if (rect)
            {
                vxInitReference(&rect->base, context, VX_TYPE_RECTANGLE);
                vxIncrementReference(&rect->base);
                vxAddReference(context, &rect->base);
                rect->sx = sx;
                rect->ex = ex;
                rect->sy = sy;
                rect->ey = ey;
            }
        }
    }
    return (vx_rectangle)rect;
}

void vxReleaseRectangleInt(vx_rectangle_t *rect)
{
    vxReleaseReference(&rect->base, VX_TYPE_RECTANGLE, vx_true_e, NULL);
}

void vxReleaseRectangle(vx_rectangle *r)
{
    vx_rectangle_t *rect = (vx_rectangle_t *)(r?*r:0);
    vxReleaseReference(&rect->base, VX_TYPE_RECTANGLE, vx_false_e, NULL);
    if (r) *r = 0;
}

vx_status vxQueryRectangle(vx_rectangle r, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_rectangle_t *rect = (vx_rectangle_t *)r;

    if (vxIsValidSpecificReference(&rect->base, VX_TYPE_RECTANGLE) == vx_false_e)
    {
        return VX_ERROR_INVALID_REFERENCE;
    }
    if (rect->base.type != VX_TYPE_RECTANGLE)
    {
        return VX_ERROR_INVALID_TYPE;
    }
    switch (attribute)
    {
        case VX_RECTANGLE_ATTRIBUTE_START_X:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                *(vx_uint32 *)ptr = rect->sx;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_RECTANGLE_ATTRIBUTE_START_Y:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                *(vx_uint32 *)ptr = rect->sy;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_RECTANGLE_ATTRIBUTE_END_X:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                *(vx_uint32 *)ptr = rect->ex;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_RECTANGLE_ATTRIBUTE_END_Y:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                *(vx_uint32 *)ptr = rect->ey;
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
    return status;
}

vx_status vxSetRectangleAttribute(vx_rectangle r, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_rectangle_t *rect = (vx_rectangle_t *)r;

    if (vxIsValidSpecificReference(&rect->base, VX_TYPE_RECTANGLE) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;
    switch (attribute)
    {
        case VX_RECTANGLE_ATTRIBUTE_START_X:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                rect->sx = *(vx_uint32 *)ptr;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_RECTANGLE_ATTRIBUTE_START_Y:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                rect->sy = *(vx_uint32 *)ptr;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_RECTANGLE_ATTRIBUTE_END_X:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                rect->ex = *(vx_uint32 *)ptr;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_RECTANGLE_ATTRIBUTE_END_Y:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                rect->ey = *(vx_uint32 *)ptr;
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
    return status;
}

vx_status vxAccessRectangleCoordinates(vx_rectangle rectangle,
                                    vx_uint32 *psx,
                                    vx_uint32 *psy,
                                    vx_uint32 *pex,
                                    vx_uint32 *pey)
{
    vx_rectangle_t *rect = (vx_rectangle_t *)rectangle;
    if (vxIsValidSpecificReference(&rect->base, VX_TYPE_RECTANGLE) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;
    if (psx) *psx = rect->sx;
    if (psy) *psy = rect->sy;
    if (pex) *pex = rect->ex;
    if (pey) *pey = rect->ey;
    return VX_SUCCESS;
}

vx_status vxCommitRectangleCoordinates(vx_rectangle rectangle,
                                    vx_uint32 sx,
                                    vx_uint32 sy,
                                    vx_uint32 ex,
                                    vx_uint32 ey)
{
    vx_rectangle_t *rect = (vx_rectangle_t *)rectangle;
    if (vxIsValidSpecificReference(&rect->base, VX_TYPE_RECTANGLE) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;
    rect->sx = sx;
    rect->sy = sy;
    rect->ex = ex;
    rect->ey = ey;
    return VX_SUCCESS;
}
