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

#define vxIsValidDelay(d) vxIsValidSpecificReference((vx_reference_t *)(d), VX_TYPE_DELAY)

void vxFreeDelay(vx_delay_t *delay)
{
    if (delay)
    {
        if (delay->set)
        {
            vx_uint32 i;
            for (i = 0; i < delay->count; i++)
            {
                vx_delay_param_t *cur = delay->set[i].next;
                while (cur != NULL)
                {
                    vx_delay_param_t *next = cur->next;
                    free(cur);
                    cur = next;
                }
            }
            free(delay->set);
        }
        if (delay->refs)
            free(delay->refs);
        free(delay);
    }
}

vx_delay_t *vxAllocateDelay(vx_size count)
{
    vx_delay_t * delay = VX_CALLOC(vx_delay_t);
    if (delay)
    {
        delay->set = (vx_delay_param_t *)calloc(count, sizeof(vx_delay_param_t));
        delay->refs = (vx_reference *)calloc(count, sizeof(vx_reference));
        if (delay->set == NULL || delay->refs == NULL)
        {
            vxFreeDelay(delay);
            delay = NULL;
        }
    }
    return delay;
}

vx_image vxGetRefFromDelay(vx_delay_t * delay, vx_int32 index)
{
    vx_reference ref = 0;
    if (delay && (vx_uint32)abs(index) < delay->count)
    {
        vx_int32 i = (delay->index + abs(index)) % (vx_int32)delay->count;
        ref = delay->refs[i];
        VX_PRINT(VX_ZONE_DELAY, "Retrieving relative index %d => " VX_FMT_REF  " from Delay (%d)\n", index, ref, i);
    }
    return ref;
}

vx_bool vxAddAssociationToDelay(vx_delay_t * delay, vx_int32 index, vx_node n, vx_uint32 i, vx_enum d)
{
    index = abs(index);
    if (delay->set[index].node == 0) // head is empty
    {
        delay->set[index].node = n;
        delay->set[index].index = i;
        delay->set[index].direction = d;
    } else {
        vx_delay_param_t **ptr = &delay->set[index].next;
        do {
            if (*ptr == NULL)
            {
                *ptr = VX_CALLOC(vx_delay_param_t);
                if (*ptr != NULL)
                {
                    (*ptr)->node = n;
                    (*ptr)->index = i;
                    (*ptr)->direction = d;
                    break;
                }
                else
                    return vx_false_e;
            }
            else
                ptr = &((*ptr)->next);
        } while (1);
    }
    VX_PRINT(VX_ZONE_DELAY, "Associating Index %d with Node "VX_FMT_REF"[%u] dir:%d type:%d\n",
                  index, n, i, d, delay->type);
    return vx_true_e;
}

vx_bool vxRemoveAssociationToDelay(vx_delay_t * delay, vx_int32 index, vx_node n, vx_uint32 i)
{
    index = abs(index);
    if (delay->set[index].node == n && delay->set[index].index == i) // head is a match
    {
        delay->set[index].node = 0;
        delay->set[index].index = 0;
        delay->set[index].direction = 0;
    }
    else
    {
        vx_delay_param_t **ptr = &delay->set[index].next;
        vx_delay_param_t *next = NULL;
        do {
            if (*ptr != NULL)
            {
                if ((*ptr)->node == n && (*ptr)->index == i)
                {
                    (*ptr)->node = 0;
                    (*ptr)->index = 0;
                    (*ptr)->direction = 0;
                    next = (*ptr)->next;
                    free(*ptr);
                    *ptr = next;
                    break;
                }
                else {
                   ptr = &((*ptr)->next);
                }
            }
            else
                return vx_false_e;
        } while (1);
    }
    VX_PRINT(VX_ZONE_DELAY, "Dissociating Index %d with Node "VX_FMT_REF"[%u]\n",
                  index, n, i);
    return vx_true_e;
}

/******************************************************************************/
/* PUBLIC INTERFACE */
/******************************************************************************/

vx_status vxQueryDelay(vx_delay delay, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_delay_t *d = (vx_delay_t*)delay;
    if (vxIsValidDelay(d) == vx_true_e)
    {
        switch (attribute)
        {
            case VX_DELAY_ATTRIBUTE_TYPE:
                if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
                    *(vx_enum *)ptr = d->type;
                else
                    status = VX_ERROR_INVALID_PARAMETERS;
                break;
            case VX_DELAY_ATTRIBUTE_COUNT:
                if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
                    *(vx_size *)ptr = d->count;
                else
                    status = VX_ERROR_INVALID_PARAMETERS;
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
    VX_PRINT(VX_ZONE_API, "%s returned %d\n",__FUNCTION__, status);
    return status;
}

void vxReleaseDelay(vx_delay *d)
{
    vx_delay_t *delay = (vx_delay_t *)(d?*d:0);
    vx_uint32 i = 0;
    if (d) *d = 0;
    if (vxIsValidDelay(delay) == vx_true_e)
    {
        for (i = 0; i < delay->count; i++)
        {
            if (delay->type == VX_TYPE_IMAGE)
            {
                vxReleaseImage((vx_image *)&delay->refs[i]);
            }
            else if (delay->type == VX_TYPE_BUFFER)
            {
                vxReleaseImage((vx_buffer*)&delay->refs[i]);
            }
            else if (delay->type == VX_TYPE_SCALAR)
            {
                vxReleaseScalar((vx_scalar*)&delay->refs[i]);
            }
        }
        vxRemoveReference(delay->base.context, &delay->base);
        vxFreeDelay(delay);
    }
}

vx_delay vxCreateImageDelay(vx_context context,
                            vx_uint32 width,
                            vx_uint32 height,
                            vx_fourcc format,
                            vx_size count)
{
    vx_context_t *c = (vx_context_t *)context;
    vx_delay_t *delay = vxAllocateDelay(count);
    if (delay)
    {
        vx_uint32 i = 0;
        vxInitReference(&delay->base, c, VX_TYPE_DELAY);
        vxIncrementReference(&delay->base);
        vxAddReference(c, &delay->base);
        delay->type = VX_TYPE_IMAGE;
        delay->count = count;
        VX_PRINT(VX_ZONE_DELAY, "Creating Delay of %u images\n", count);
        for (i = 0; i < count; i++)
        {
            delay->refs[i] = (vx_reference)vxCreateImage(context, width, height, format);
        }
    }
    return (vx_delay)delay;
}

vx_delay vxCreateBufferDelay(vx_context context,
                             vx_size unitSize,
                             vx_size numUnits,
                             vx_size count)
{
    vx_context_t *c = (vx_context_t *)context;
    vx_delay_t *delay = vxAllocateDelay(count);
    if (delay)
    {
        vx_size i = 0;
        vxInitReference(&delay->base, c, VX_TYPE_DELAY);
        vxIncrementReference(&delay->base);
        vxAddReference(c, &delay->base);
        delay->type = VX_TYPE_BUFFER;
        delay->count = count;
        VX_PRINT(VX_ZONE_DELAY, "Creating Delay of %u buffers!\n", count);
        for (i = 0; i < count; i++)
        {
            delay->refs[i] = (vx_reference)vxCreateBuffer(context, unitSize, numUnits);
        }
    }
    return (vx_delay)delay;
}

vx_image vxGetImageFromDelay(vx_delay d, vx_int32 index)
{
    vx_delay_t *delay = (vx_delay_t *)d;
    if ((vxIsValidDelay(delay) == vx_true_e) &&
        (delay->type == VX_TYPE_IMAGE))
    {
        return (vx_image)vxGetRefFromDelay(delay, index);
    }
    return 0;
}

vx_image vxGetBufferFromDelay(vx_delay d, vx_int32 index)
{
    vx_delay_t *delay = (vx_delay_t *)d;
    if ((vxIsValidDelay(delay) == vx_true_e) &&
        (delay->type == VX_TYPE_BUFFER))
    {
        return (vx_buffer)vxGetRefFromDelay(delay, index);
    }
    return 0;
}

vx_status vxAgeDelay(vx_delay d)
{
    vx_status status = VX_SUCCESS;
    vx_delay_t *delay = (vx_delay_t *)d;
    if (vxIsValidDelay(delay) == vx_true_e)
    {
        vx_int32 i,j;

        // increment the index
        delay->index = (delay->index + 1) % (vx_uint32)delay->count;

        VX_PRINT(VX_ZONE_DELAY, "Delay has shifted by 1, base index is now %d\n", delay->index);

        // then reassign the parameters
        for (i = 0; i < delay->count; i++)
        {
            vx_delay_param_t *param = NULL;

            j = (delay->index + i) % (vx_int32)delay->count;
            param = &delay->set[i];
            do {
                if (param->node != 0)
                {
                    status = vxSetParameterByIndex(param->node,
                                                   param->index,
                                                   param->direction,
                                                   delay->refs[j]);
                    if (status != VX_SUCCESS)
                    {
                        VX_PRINT(VX_ZONE_ERROR, "Failed to set parameter for base index %d, rel index %d\n", i,j);
                        break;
                    }
                }
                param = param->next;
            } while (param != NULL);
        }
    }
    else
    {
        status = VX_ERROR_INVALID_REFERENCE;
    }
    return status;
}

vx_status vxAssociateDelayWithNode(vx_delay d,
                                   vx_int32 delay_index,
                                   vx_node n,
                                   vx_uint32 param_index,
                                   vx_enum param_direction)
{
    vx_status status = VX_SUCCESS;
    vx_delay_t *delay = (vx_delay_t *)d;
    if (vxIsValidDelay(delay) == vx_false_e)
        status = VX_ERROR_INVALID_REFERENCE;
    else if (abs(delay_index) < delay->count)
    {
        if (vxAddAssociationToDelay(delay, delay_index, n, param_index, param_direction) == vx_false_e)
            status = VX_FAILURE;
    }
    else
        status = VX_ERROR_INVALID_PARAMETERS;
    return status;
}

vx_status vxDissociateDelayFromNode(vx_delay d,
                                    vx_int32 index,
                                    vx_node n,
                                    vx_uint32 pi)
{
    vx_status status = VX_SUCCESS;
    vx_delay_t *delay = (vx_delay_t *)d;
    if (vxIsValidDelay(delay) == vx_false_e)
        status = VX_ERROR_INVALID_REFERENCE;
    else if (abs(index) < delay->count)
    {
        if (vxRemoveAssociationToDelay(delay, index, n, pi) == vx_false_e)
            status = VX_FAILURE;
    }
    else
        status = VX_ERROR_INVALID_PARAMETERS;
    return status;
}

