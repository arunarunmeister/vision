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

void vxInitReference(vx_reference_t *ref, vx_context_t *context, vx_enum type)
{
    if (ref)
    {
        ref->context = context;
        ref->magic = VX_MAGIC;
        ref->type = type;
        ref->internal_count = 0;
        ref->external_count = 0;
        ref->write_count = 0;
        ref->read_count = 0;
        ref->extracted = vx_false_e;
        vxCreateSem(&ref->lock, 1);
    }
}

vx_bool vxAddReference(vx_context_t *context, vx_reference_t *ref)
{
    vx_uint32 r;
    vx_bool ret = vx_false_e;
    for (r = 0; r < VX_INT_MAX_REF; r++)
    {
        if (context->reftable[r] == NULL)
        {
            context->reftable[r] = ref;
            context->numRefs++;
            ret = vx_true_e;
            break;
        }
    }
    return ret;
}

void vxReleaseReference(vx_reference_t *ref,
                        vx_enum type,
                        vx_bool internal,
                        vx_destructor_f destructor)
{
    if (vxIsValidSpecificReference(ref, type) == vx_true_e)
    {
        vx_bool result = vx_false_e;
        if (internal == vx_true_e)
            result = vxDecrementIntReference(ref);
        else
            result = vxDecrementReference(ref);
        if ((result == vx_true_e) && (vxTotalReferenceCount(ref) == 0))
        {
            /* if the caller supplied a destructor, call it. */
            if (destructor)
            {
                destructor(ref);
            }
            vxRemoveReference(ref->context, ref);
            ref->magic = 0; /* make sure no existing copies of refs can use ref again */
            free(ref);
        }
    }
}

vx_reference_t *vxCreateReference(vx_context_t *context, vx_enum type)
{
    vx_reference_t *ref = VX_CALLOC(vx_reference_t);
    if (ref)
    {
        vxInitReference(ref, context, type);
    }
    return ref;
}

vx_bool vxIsValidReference(vx_reference_t * ref)
{
    vx_bool ret = vx_false_e;
    if (ref != NULL)
    {
        vxPrintReference(ref);
        if ((ref->magic == VX_MAGIC) &&
            (vxIsValidType(ref->type) && ref->type != VX_TYPE_CONTEXT) &&
            (vxIsValidContext(ref->context) == vx_true_e))
        {
            ret = vx_true_e;
        }
        else
        {
            VX_PRINT(VX_ZONE_ERROR, "%p is not a valid reference!\n", ref);
        }
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Reference was NULL\n");
    }
    return ret;
}

vx_bool vxIsValidSpecificReference(vx_reference_t * ref, vx_enum type)
{
    vx_bool ret = vx_false_e;
    if (ref != NULL)
    {
        vxPrintReference(ref);
        if ((ref->magic == VX_MAGIC) &&
            (ref->type == type) &&
            (vxIsValidContext(ref->context) == vx_true_e))
        {
            ret = vx_true_e;
        }
        else
        {
            VX_PRINT(VX_ZONE_ERROR, "%p is not a valid reference!\n", ref);
        }
    }
    else
    {
        VX_PRINT(VX_ZONE_WARNING, "Reference was NULL\n");
    }
    return ret;
}

vx_bool vxRemoveReference(vx_context_t *context, vx_reference_t *ref)
{
    vx_uint32 r;
    for (r = 0; r < VX_INT_MAX_REF; r++)
    {
        if (context->reftable[r] == ref)
        {
            context->reftable[r] = NULL;
            context->numRefs--;
            return vx_true_e;
        }
    }
    return vx_false_e;
}

void vxPrintReference(vx_reference_t *ref)
{
    if (ref)
    {
        VX_PRINT(VX_ZONE_REFERENCE, "vx_reference_t:%p magic:%08x type:%08x count:[%u,%u] context:%p\n", ref, ref->magic, ref->type, ref->external_count, ref->internal_count, ref->context);
    }
}

vx_status vxQueryReference(vx_reference r, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_reference_t *ref = (vx_reference_t *)r;

    /* if it is not a reference and not a context */
    if ((vxIsValidReference(ref) == vx_false_e) &&
        (vxIsValidContext((vx_context_t *)ref) == vx_false_e)) {
        return VX_ERROR_INVALID_REFERENCE;
    }
    switch (attribute)
    {
        case VX_REF_ATTRIBUTE_COUNT:
            if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
            {
                *(vx_uint32 *)ptr = ref->external_count;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_REF_ATTRIBUTE_TYPE:
            if (VX_CHECK_PARAM(ptr, size, vx_enum, 0x3))
            {
                *(vx_enum *)ptr = ref->type;
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

void vxIncrementReference(vx_reference_t *ref)
{
    if (ref)
    {
        vxSemWait(&ref->lock);
        ref->external_count++;
        VX_PRINT(VX_ZONE_REFERENCE, "Incremented External Reference Count to %u on "VX_FMT_REF"\n", ref->external_count, ref);
        vxSemPost(&ref->lock);
    }
}

vx_bool vxDecrementReference(vx_reference_t *ref)
{
    vx_bool result = vx_false_e;
    if (ref)
    {
        vxSemWait(&ref->lock);
        if (ref->external_count == 0)
        {
            VX_PRINT(VX_ZONE_WARNING, "#### EXTERNAL REF COUNT IS ALREADY ZERO!!! "VX_FMT_REF" type:%08x #####\n", ref, ref->type);
        }
        else
        {
            ref->external_count--;
            VX_PRINT(VX_ZONE_REFERENCE, "Decremented External Reference Count to %u on "VX_FMT_REF" type:%08x\n", ref->external_count, ref, ref->type);
            if ((ref->external_count == 0) && (ref->extracted == vx_true_e))
            {
                ref->extracted = vx_false_e;
            }
            result = vx_true_e;
        }
        vxSemPost(&ref->lock);
    }
    return result;
}

void vxIncrementIntReference(vx_reference_t *ref)
{
    if (ref)
    {
        vxSemWait(&ref->lock);
        ref->internal_count++;
        VX_PRINT(VX_ZONE_REFERENCE, "Incremented Internal Reference Count to %u on "VX_FMT_REF"\n", ref->internal_count, ref);
        vxSemPost(&ref->lock);
    }
}

vx_bool vxDecrementIntReference(vx_reference_t *ref)
{
    vx_bool result = vx_false_e;
    if (ref)
    {
        vxSemWait(&ref->lock);
        if (ref->internal_count == 0)
        {
            VX_PRINT(VX_ZONE_WARNING, "#### INTERNAL REF COUNT IS ALREADY ZERO!!! "VX_FMT_REF" type:%08x #####\n", ref, ref->type);
        }
        else
        {
            ref->internal_count--;
            VX_PRINT(VX_ZONE_REFERENCE, "Decremented Internal Reference Count to %u on "VX_FMT_REF"\n", ref->internal_count, ref);
            result = vx_true_e;
        }
        vxSemPost(&ref->lock);
    }
    return result;
}

vx_uint32 vxTotalReferenceCount(vx_reference_t *ref)
{
    vx_uint32 count = 0;
    if (ref)
    {
        vxSemWait(&ref->lock);
        count = ref->external_count + ref->internal_count;
        vxSemPost(&ref->lock);
    }
    return count;
}

void vxWroteToReference(vx_reference_t *ref)
{
    if (ref)
    {
        vxSemWait(&ref->lock);
        ref->write_count++;
        if (ref->extracted == vx_true_e)
        {
            vxContaminateGraphs(ref);
        }
        vxSemPost(&ref->lock);
    }
}

void vxReadFromReference(vx_reference_t *ref)
{
    if (ref)
    {
        vxSemWait(&ref->lock);
        ref->read_count++;
        vxSemPost(&ref->lock);
    }
}

