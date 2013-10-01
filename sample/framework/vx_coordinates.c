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

void vxDestructCoordinates(vx_reference_t *ref)
{
    vx_convolution_t *convolution = (vx_convolution_t *)ref;
    vxFreeMemory(convolution->base.base.context, &convolution->base.memory);
}

void vxReleaseCoordinatesInt(vx_coordinates_t *c)
{
    vxReleaseReference(&c->base, VX_TYPE_COORDINATES, vx_true_e, vxDestructCoordinates);
}

void vxReleaseCoordinates(vx_coordinates *c)
{
    vx_coordinates_t *coord = (vx_coordinates_t *)(c?*c:0);
    vxReleaseReference(&coord->base, VX_TYPE_COORDINATES, vx_false_e, vxDestructCoordinates);
    if (c) *c = 0;
}

vx_coordinates vxCreateCoordinates(vx_context c, vx_size dims)
{
    vx_coordinates_t *coordinates = VX_CALLOC(vx_coordinates_t);
    if (coordinates)
    {
        vxInitReference(&coordinates->base, (vx_context_t *)c, VX_TYPE_COORDINATES);
        vxIncrementReference(&coordinates->base);
        vxAddReference(coordinates->base.context, &coordinates->base);
        coordinates->type = VX_TYPE_INT32;
        coordinates->columns = dims;
        coordinates->rows = 1;
        coordinates->memory.ndims = 2;
        coordinates->memory.nptrs = 1;
        coordinates->memory.dims[0][0] = sizeof(vx_int32);
        coordinates->memory.dims[0][1] = (vx_int32)dims;
    }
    return (vx_coordinates)coordinates;
}

vx_status vxQueryCoordinates(vx_coordinates coord, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_coordinates_t *coordinates = (vx_coordinates_t *)coord;
    if (vxIsValidSpecificReference(&coordinates->base, VX_TYPE_COORDINATES) == vx_false_e)
    {
        return VX_ERROR_INVALID_REFERENCE;
    }
    switch (attribute)
    {
        case VX_COORDINATES_ATTRIBUTE_DIMENSIONS:
            if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
            {
                *(vx_size *)ptr = coordinates->columns;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_COORDINATES_ATTRIBUTE_SIZE:
            if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
            {
                *(vx_size *)ptr = coordinates->columns * 1 * sizeof(vx_int32);
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

vx_status vxSetCoordinatesAttributes(vx_coordinates coord, vx_enum attr, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_coordinates_t *coordinates = (vx_coordinates_t *)coord;

    if (vxIsValidSpecificReference(&coordinates->base, VX_TYPE_COORDINATES) == vx_false_e)
    {
        return VX_ERROR_INVALID_REFERENCE;
    }
    switch (attr)
    {
        case VX_COORDINATES_ATTRIBUTE_DIMENSIONS:
            if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
            {
                vx_size dims = *(vx_size *)ptr;
                if (dims > coordinates->columns)
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                else
                {
                    coordinates->columns = dims;
                }
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        default:
            status = VX_ERROR_INVALID_PARAMETERS;
            break;
    }
    return status;
}

vx_status vxAccessCoordinates(vx_coordinates coord, vx_int32 *array)
{
    vx_coordinates_t *coordinates = (vx_coordinates_t *)coord;
    vx_status status = VX_ERROR_INVALID_REFERENCE;
    if ((vxIsValidSpecificReference(&coordinates->base, VX_TYPE_COORDINATES) == vx_true_e) &&
        (vxAllocateMemory(coordinates->base.context, &coordinates->memory) == vx_true_e))
    {
        if (array)
        {
            vx_size size = 0ul;
            vxSemWait(&coordinates->base.lock);
            size = coordinates->memory.strides[0][1] *
                   coordinates->memory.dims[0][1];
            memcpy(array, coordinates->memory.ptrs[0], size);
            vxSemPost(&coordinates->base.lock);
            vxReadFromReference(&coordinates->base);
        }
        vxIncrementReference(&coordinates->base);
        status = VX_SUCCESS;
    }
    return status;
}

vx_status vxCommitCoordinates(vx_coordinates coord, vx_int32 *array)
{
    vx_coordinates_t *coordinates = (vx_coordinates_t *)coord;
    vx_status status = VX_ERROR_INVALID_REFERENCE;
    if ((vxIsValidSpecificReference(&coordinates->base, VX_TYPE_COORDINATES) == vx_true_e) &&
        (vxAllocateMemory(coordinates->base.context, &coordinates->memory) == vx_true_e))
    {
        if (array)
        {
            vx_size size = 0ul;
            vxSemWait(&coordinates->base.lock);
            size = coordinates->memory.strides[0][1] *
                   coordinates->memory.dims[0][1];
            memcpy(coordinates->memory.ptrs[0], array, size);
            vxSemPost(&coordinates->base.lock);
            vxWroteToReference(&coordinates->base);
        }
        vxDecrementReference(&coordinates->base);
        status = VX_SUCCESS;
    }
    return status;
}
