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

void vxPrintScalarValue(vx_scalar_t *scalar)
{
    switch (scalar->type)
    {
        case VX_TYPE_CHAR:
            VX_PRINT(VX_ZONE_SCALAR, "scalar "VX_FMT_REF" = %c\n", scalar, scalar->data.chr);
            break;
        case VX_TYPE_INT8:
            VX_PRINT(VX_ZONE_SCALAR, "scalar "VX_FMT_REF" = %d\n", scalar, scalar->data.s08);
            break;
        case VX_TYPE_UINT8:
            VX_PRINT(VX_ZONE_SCALAR, "scalar "VX_FMT_REF" = %u\n", scalar, scalar->data.u08);
            break;
        case VX_TYPE_INT16:
            VX_PRINT(VX_ZONE_SCALAR, "scalar "VX_FMT_REF" = %hd\n", scalar, scalar->data.s16);
            break;
        case VX_TYPE_UINT16:
            VX_PRINT(VX_ZONE_SCALAR, "scalar "VX_FMT_REF" = %hu\n", scalar, scalar->data.u16);
            break;
        case VX_TYPE_INT32:
            VX_PRINT(VX_ZONE_SCALAR, "scalar "VX_FMT_REF" = %d\n", scalar, scalar->data.s32);
            break;
        case VX_TYPE_UINT32:
            VX_PRINT(VX_ZONE_SCALAR, "scalar "VX_FMT_REF" = %u\n", scalar, scalar->data.u32);
            break;
        case VX_TYPE_INT64:
            VX_PRINT(VX_ZONE_SCALAR, "scalar "VX_FMT_REF" = %ld\n", scalar, scalar->data.s64);
            break;
        case VX_TYPE_UINT64:
            VX_PRINT(VX_ZONE_SCALAR, "scalar "VX_FMT_REF" = %lu\n", scalar, scalar->data.u64);
            break;
#if OVX_SUPPORT_HALF_FLOAT
        case VX_TYPE_FLOAT16:
            VX_PRINT(VX_ZONE_SCALAR, "scalar "VX_FMT_REF" = %f\n", scalar, scalar->data.f16);
            break;
#endif
        case VX_TYPE_FLOAT32:
            VX_PRINT(VX_ZONE_SCALAR, "scalar "VX_FMT_REF" = %lf\n", scalar, scalar->data.f32);
            break;
        case VX_TYPE_FLOAT64:
            VX_PRINT(VX_ZONE_SCALAR, "scalar "VX_FMT_REF" = %lf\n", scalar, scalar->data.f64);
            break;
        case VX_TYPE_FOURCC:
            VX_PRINT(VX_ZONE_SCALAR, "scalar "VX_FMT_REF" = %08x\n", scalar, scalar->data.fcc);
            break;
        case VX_TYPE_ENUM:
            VX_PRINT(VX_ZONE_SCALAR, "scalar "VX_FMT_REF" = %d\n", scalar, scalar->data.enm);
            break;
        case VX_TYPE_SIZE:
            VX_PRINT(VX_ZONE_SCALAR, "scalar "VX_FMT_REF" = %zu\n", scalar, scalar->data.size);
            break;
        default:
            VX_PRINT(VX_ZONE_ERROR, "some case is not covered!\n");
            break;
    }
}

vx_scalar vxCreateScalar(vx_context c, vx_enum type, void *ptr)
{
    vx_scalar_t *scalar = NULL;
    vx_context_t *context = (vx_context_t *)c;

    if (vxIsValidContext(context) == vx_false_e)
        return 0;

    if (!VX_TYPE_IS_SCALAR(type))
        return 0;

    scalar = VX_CALLOC(vx_scalar_t);
    if (scalar)
    {
        vxInitReference((vx_reference_t *)scalar, context, VX_TYPE_SCALAR);
        vxIncrementReference(&scalar->base);
        vxAddReference(context, (vx_reference_t *)scalar);
        scalar->type = type;
        vxCommitScalarValue((vx_scalar)scalar, ptr);
    }
    return (vx_scalar)scalar;
}

void vxReleaseScalarInt(vx_scalar_t *scalar)
{
    vxReleaseReference((vx_reference_t *)scalar, VX_TYPE_SCALAR, vx_false_e, NULL);
}

void vxReleaseScalar(vx_scalar *s)
{
    vx_scalar_t *scalar = (vx_scalar_t *)(s?*s:0);
    vxReleaseReference((vx_reference_t *)scalar, VX_TYPE_SCALAR, vx_false_e, NULL);
    if (s) *s = 0;
}

vx_status vxQueryScalar(vx_scalar scalar, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_scalar_t *pscalar = (vx_scalar_t *)scalar;

    if (vxIsValidSpecificReference(&pscalar->base,VX_TYPE_SCALAR) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;

    switch (attribute)
    {
        case VX_SCALAR_ATTRIBUTE_TYPE:
            if (VX_CHECK_PARAM(ptr, size, vx_enum, 0x3))
            {
                *(vx_enum *)ptr = pscalar->type;
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

vx_status vxAccessScalarValue(vx_scalar s, void *ptr)
{
    vx_status status = VX_SUCCESS;
    vx_scalar_t *scalar = (vx_scalar_t *)s;

    if (vxIsValidSpecificReference(&scalar->base,VX_TYPE_SCALAR) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;

    if (ptr == NULL)
        return VX_ERROR_INVALID_PARAMETERS;

    vxSemWait(&scalar->base.lock);
    vxPrintScalarValue(scalar);
    switch (scalar->type)
    {
        case VX_TYPE_CHAR:
            *(vx_char *)ptr = scalar->data.chr;
            break;
        case VX_TYPE_INT8:
            *(vx_int8 *)ptr = scalar->data.s08;
            break;
        case VX_TYPE_UINT8:
            *(vx_uint8 *)ptr = scalar->data.u08;
            break;
        case VX_TYPE_INT16:
            *(vx_int16 *)ptr = scalar->data.s16;
            break;
        case VX_TYPE_UINT16:
            *(vx_uint16 *)ptr = scalar->data.u16;
            break;
        case VX_TYPE_INT32:
            *(vx_int32 *)ptr = scalar->data.s32;
            break;
        case VX_TYPE_UINT32:
            *(vx_uint32 *)ptr = scalar->data.u32;
            break;
        case VX_TYPE_INT64:
            *(vx_int64 *)ptr = scalar->data.s64;
            break;
        case VX_TYPE_UINT64:
            *(vx_uint64 *)ptr = scalar->data.u64;
            break;
#if OVX_SUPPORT_HALF_FLOAT
        case VX_TYPE_FLOAT16:
            *(vx_float16 *)ptr = scalar->data.f16;
            break;
#endif
        case VX_TYPE_FLOAT32:
            *(vx_float32 *)ptr = scalar->data.f32;
            break;
        case VX_TYPE_FLOAT64:
            *(vx_float64 *)ptr = scalar->data.f64;
            break;
        case VX_TYPE_FOURCC:
            *(vx_fourcc *)ptr = scalar->data.fcc;
            break;
        case VX_TYPE_ENUM:
            *(vx_enum *)ptr = scalar->data.enm;
            break;
        case VX_TYPE_SIZE:
            *(vx_size *)ptr = scalar->data.size;
            break;
        default:
            VX_PRINT(VX_ZONE_ERROR, "some case is not covered in %s\n", __FUNCTION__);
            status = VX_ERROR_NOT_SUPPORTED;
            break;
    }
    vxSemPost(&scalar->base.lock);
    vxReadFromReference(&scalar->base);
    return status;
}

vx_status vxCommitScalarValue(vx_scalar s, void *ptr)
{
    vx_status status = VX_SUCCESS;
    vx_scalar_t *scalar = (vx_scalar_t *)s;

    if (vxIsValidSpecificReference(&scalar->base,VX_TYPE_SCALAR) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;

    if (ptr == NULL)
        return VX_ERROR_INVALID_PARAMETERS;

    vxSemWait(&scalar->base.lock);
    switch (scalar->type)
    {
        case VX_TYPE_CHAR:
            scalar->data.chr = *(vx_char *)ptr;
            break;
        case VX_TYPE_INT8:
            scalar->data.s08 = *(vx_int8 *)ptr;
            break;
        case VX_TYPE_UINT8:
            scalar->data.u08 = *(vx_uint8 *)ptr;
            break;
        case VX_TYPE_INT16:
            scalar->data.s16 = *(vx_int16 *)ptr;
            break;
        case VX_TYPE_UINT16:
            scalar->data.u16 = *(vx_uint16 *)ptr;
            break;
        case VX_TYPE_INT32:
            scalar->data.s32 = *(vx_int32 *)ptr;
            break;
        case VX_TYPE_UINT32:
            scalar->data.u32 = *(vx_uint32 *)ptr;
            break;
        case VX_TYPE_INT64:
            scalar->data.s64 = *(vx_int64 *)ptr;
            break;
        case VX_TYPE_UINT64:
            scalar->data.u64 = *(vx_uint64 *)ptr;
            break;
#if OVX_SUPPORT_HALF_FLOAT
        case VX_TYPE_FLOAT16:
            scalar->data.f16 = *(vx_float16 *)ptr;
            break;
#endif
        case VX_TYPE_FLOAT32:
            scalar->data.f32 = *(vx_float32 *)ptr;
            break;
        case VX_TYPE_FLOAT64:
            scalar->data.f64 = *(vx_float64 *)ptr;
            break;
        case VX_TYPE_FOURCC:
            scalar->data.fcc = *(vx_fourcc *)ptr;
            break;
        case VX_TYPE_ENUM:
            scalar->data.enm = *(vx_enum *)ptr;
            break;
        case VX_TYPE_SIZE:
            scalar->data.size = *(vx_size *)ptr;
            break;
        default:
            VX_PRINT(VX_ZONE_ERROR, "some case is not covered in %s\n", __FUNCTION__);
            status = VX_ERROR_NOT_SUPPORTED;
            break;
    }
    vxPrintScalarValue(scalar);
    vxSemPost(&scalar->base.lock);
    vxWroteToReference(&scalar->base);
    return status;
}
