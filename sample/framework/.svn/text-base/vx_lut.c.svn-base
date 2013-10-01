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

vx_lut vxCreateLUT(vx_context c, vx_enum type, vx_size count)
{
    vx_lut_t *lut = NULL;
    vx_context_t *context = (vx_context_t *)c;
    if (vxIsValidContext(context) == vx_true_e)
    {
        if (type == VX_TYPE_UINT8)
        {
#if defined(OPENVX_STRICT_1_0)
            if (count != 256)
                return NULL;
#endif
            lut = VX_CALLOC(vx_lut_t);
            if (lut)
            {
                vxInitReference(&lut->base, context, VX_TYPE_LUT);
                vxIncrementReference(&lut->base);
                vxAddReference(context, (vx_reference_t *)lut);
                lut->numUnits = count;
                lut->unitSize = sizeof(vx_uint8);
            }
        }
#if !defined(OPENVX_STRICT_1_0)
        else if (type == VX_TYPE_UINT16)
        {
            lut = VX_CALLOC(vx_lut_t);
            if (lut)
            {
                vxInitReference(&lut->base, context, VX_TYPE_LUT);
                vxIncrementReference(&lut->base);
                vxAddReference(context, (vx_reference_t *)lut);
                lut->numUnits = count;
                lut->unitSize = sizeof(vx_uint16);
            }
        }
#endif
    }
    return (vx_lut)lut;
}

void vxDestructLUT(vx_reference_t *ref)
{
    vx_lut_t *lut = (vx_lut_t *)ref;
    vxFreeBuffer(lut);
}

void vxReleaseLUTInt(vx_lut_t *lut)
{
    vxReleaseReference((vx_reference_t *)lut, VX_TYPE_LUT, vx_true_e, vxDestructLUT);
}

void vxReleaseLUT(vx_lut *l)
{
    vx_lut_t *lut = (vx_lut_t *)(l?*l:0);
    vxReleaseReference((vx_reference_t *)lut, VX_TYPE_LUT, vx_false_e, vxDestructLUT);
    if (l) *l = 0;
}

vx_status vxQueryLUT(vx_lut l, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_lut_t *lut = (vx_lut_t *)l;

    if (vxIsValidSpecificReference(&lut->base, VX_TYPE_LUT) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;

    switch (attribute)
    {
        case VX_LUT_ATTRIBUTE_TYPE:
            if (VX_CHECK_PARAM(ptr, size, vx_enum, 0x3))
            {
                if (lut->unitSize == sizeof(vx_uint8))
                {
                    *(vx_enum *)ptr = VX_TYPE_UINT8;
                }
                else if (lut->unitSize == sizeof(vx_uint16))
                {
                    *(vx_enum *)ptr = VX_TYPE_UINT16;
                }
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_LUT_ATTRIBUTE_COUNT:
            if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
            {
                *(vx_size *)ptr = lut->numUnits;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_LUT_ATTRIBUTE_SIZE:
            if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
            {
                *(vx_size *)ptr = lut->numUnits * lut->unitSize;
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

vx_status vxAccessLUT(vx_lut l, void **ptr)
{
    vx_status status = VX_FAILURE;
    vx_lut_t *lut = (vx_lut_t *)l;
    if (vxIsValidSpecificReference(&lut->base, VX_TYPE_LUT) == vx_true_e)
    {
        status = vxAccessBufferRange(l, 0, lut->numUnits, ptr);
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Not a valid object!\n");
    }
    return status;
}

vx_status vxCommitLUT(vx_lut l, void *ptr)
{
    vx_status status = VX_FAILURE;
    vx_lut_t *lut = (vx_lut_t *)l;
    if (vxIsValidSpecificReference(&lut->base, VX_TYPE_LUT) == vx_true_e)
    {
        status = vxCommitBufferRange(l, 0, lut->numUnits, ptr);
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Not a valid object!\n");
    }
    return status;
}

