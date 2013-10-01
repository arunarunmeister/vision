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

void vxPrintItem(vx_item_t *item, vx_char *prefix)
{
    VX_PRINT(VX_ZONE_LIST, "%s: Item %p, Next=%p, Prev=%p ref="VX_FMT_REF"\n",
             prefix, item, item->next, item->prev, item->ref);
}

void vxPrintList(vx_list_t *list)
{
    vx_item_t *tmp = list->head;
    VX_PRINT(VX_ZONE_LIST, "Head=%p, Tail=%p Count=%lu\n",
                list->head, list->tail, list->count);
    while (tmp)
    {
        vxPrintItem(tmp, "List");
        tmp = tmp->next;
    }
}

vx_bool vxIsValidListType(vx_enum type)
{
#if defined(OPENVX_STRICT_1_0)
    if ((list->type == VX_TYPE_COORDINATES) ||
        (list->type == VX_TYPE_RECTANGLE) ||
        (list->type == VX_TYPE_KEYPOINT))
#else
    if (VX_TYPE_IS_OBJECT(type))
#endif
    {
        return vx_true_e;
    }
    return vx_false_e;
}

vx_bool vxIsValidList(vx_list_t *list)
{
    vxPrintReference((vx_reference_t *)list);
    if (vxIsValidReference((vx_reference_t *)list) == vx_true_e)
    {
        return vx_true_e;
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR,"Invalid List\n");
        return vx_false_e;
    }
}

vx_item_t *vxAllocItem(vx_reference ref)
{
    vx_item_t *item = VX_CALLOC(vx_item_t);
    if (item)
    {
        item->ref = ref;
    }
    return item;
}

void vxFreeItem(vx_list_t *list, vx_iterator *it, vx_item_t *item)
{
    vx_reference_t *ref = (vx_reference_t *)item->ref;
    vx_item_t **pIter = (vx_item_t **)it;
    /* if we are removing the "iterator" item in the list, modify accordingly */
    if (pIter && (*pIter) == item)
    {
        if (item->prev == NULL || item->next == NULL)
        {
            VX_PRINT(VX_ZONE_LIST, "Iter being removed! Reseting!\n");
            (*pIter) = NULL;
        }
        if ((item->prev != NULL) && (item->next != NULL))
        {
            VX_PRINT(VX_ZONE_LIST, "Moving iterator to previous\n");
            (*pIter) = item->prev;
        }
    }

    if (item->prev)
    {
        item->prev->next = item->next;
    }
    else
    {
        list->head = item->next;
    }
    if (item->next)
    {
        item->next->prev = item->prev;
    }
    else
    {
        list->tail = item->prev;
    }

    item->next = NULL;
    item->prev = NULL;
    list->count--;
    free(item);
    switch (ref->type)
    {
        case VX_TYPE_COORDINATES:
            vxReleaseCoordinates((vx_coordinates *)&ref);
            break;
        case VX_TYPE_RECTANGLE:
            vxReleaseParameter((vx_rectangle *)&ref);
            break;
        case VX_TYPE_KEYPOINT:
            vxReleaseKeypoint((vx_keypoint *)&ref);
            break;
        /*! \todo add other types to release list */
        default:
            break;
    }
}

void vxEmptyListInternal(vx_list_t *list)
{
    if (list)
    {
        vx_item_t *item = list->head;
        vx_iterator it = 0;
        while (item)
        {
            vx_item_t *next = item->next;
            vxFreeItem(list, &it, item);
            item = next;
        }
        list->head = NULL;
        list->tail = NULL;
    }
}

vx_status vxInsertItem(vx_list_t *list, vx_item_t *item, vx_ref_compare_f sorter)
{
    vx_item_t *iter = NULL;
    vx_status status = VX_ERROR_INVALID_VALUE;
    do {
        if (list->count == 0)
        {
            /* no items at all */
            VX_PRINT(VX_ZONE_LIST, "List is empty for insert\n");
            list->head = item;
            list->tail = item;
            list->count++;
            status = VX_SUCCESS;
        }
        else if (list->count == 1)
        {
            vx_int32 cmp = 0;
            /* a single item */
            iter = list->head;
            cmp = sorter(item->ref, iter->ref);
            if (cmp == VX_COMPARE_LT)
            {
                VX_PRINT(VX_ZONE_LIST, "List head insert\n");
                list->head = item;
                item->next = iter;
                iter->prev = item;
                //list->tail is untouched
                list->count++;
                status = VX_SUCCESS;
            }
            else
            {
                VX_PRINT(VX_ZONE_LIST, "List tail insert\n");
                list->tail = item;
                iter->next = item;
                item->prev = iter;
                //list->head is untouched
                list->count++;
                status = VX_SUCCESS;
            }
        }
        else /* 2 or more items */
        {
            /* check for pre-head */
            if (sorter(item->ref, list->head->ref) == VX_COMPARE_LT)
            {
                VX_PRINT(VX_ZONE_LIST, "List head insert\n");
                item->next = list->head;
                list->head->prev = item;
                list->head = item;
                list->count++;
                status = VX_SUCCESS;
                break;
            }
            /* check for post-tail */
            else if (sorter(item->ref, list->tail->ref) == VX_COMPARE_GT)
            {
                VX_PRINT(VX_ZONE_LIST, "List tail insert\n");
                item->prev = list->tail;
                list->tail->next = item;
                list->tail = item;
                list->count++;
                status = VX_SUCCESS;
                break;
            }
            VX_PRINT(VX_ZONE_LIST, "Central sorting for list!\n");
            /* go into the second item */
            iter = list->head->next;
            while (iter)
            {
                vx_enum cmp_prev = sorter(item->ref, iter->prev->ref);
                vx_enum cmp_curr = sorter(item->ref, iter->ref);
                VX_PRINT(VX_ZONE_LIST, "Prev=%x, Curr=%x\n", cmp_prev, cmp_curr);
                if (((cmp_prev == VX_COMPARE_GT) ||
                     (cmp_prev == VX_COMPARE_EQ)) &&
                    ((cmp_curr == VX_COMPARE_EQ) ||
                     (cmp_curr == VX_COMPARE_LT)))
                {
                    VX_PRINT(VX_ZONE_LIST, "List middle insert\n");
                    iter->prev->next = item;
                    item->prev = iter->prev;
                    iter->prev = item;
                    item->next = iter;
                    list->count++;
                    status = VX_SUCCESS;
                    break;
                }
                iter = iter->next;
            }
        }
    } while (0);
    return status;
}

/******************************************************************************/
/* PUBLIC FUNCTIONS */
/******************************************************************************/

vx_list vxCreateList(vx_context context, vx_enum type, vx_size initial)
{
    vx_list_t *list = NULL;

    if (vxIsValidListType(type) == vx_false_e)
        return 0;

    if (vxIsValidContext((vx_context_t *)context) == vx_true_e)
    {
        list = VX_CALLOC(vx_list_t);
        if (list)
        {
            vxInitReference((vx_reference_t *)list, (vx_context_t *)context, VX_TYPE_LIST);
            vxIncrementReference(&list->base);
            vxAddReference(list->base.context, (vx_reference_t *)list);
            initial |= 0; // meaningless for this implementation.
            list->type = type;
            list->count = 0ul;
        }
    }
    return (vx_list)list;
}

static void vxDestructList(vx_reference_t *ref)
{
    vx_list_t *list = (vx_list_t *)ref;
    vxEmptyListInternal(list);
    VX_PRINT(VX_ZONE_LIST, "Ref Count Zero, Releasing "VX_FMT_REF"\n", list);
}

void vxReleaseListInt(vx_list_t *list)
{
    vxReleaseReference((vx_reference_t *)list, VX_TYPE_LIST, vx_true_e, vxDestructList);
}

void vxReleaseList(vx_list *l)
{
    vx_list_t *list = (vx_list_t *)(l?*l:0);
    vxReleaseReference((vx_reference_t *)list, VX_TYPE_LIST, vx_false_e, vxDestructList);
    if (l) *l = 0;
}

vx_status vxQueryList(vx_list listref, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_ERROR_INVALID_REFERENCE;
    vx_list_t *list = (vx_list_t *)listref;
    if (vxIsValidList(list) == vx_true_e)
    {
        status = VX_SUCCESS;
        vxSemWait(&list->base.lock);
        switch (attribute)
        {
            case VX_LIST_ATTRIBUTE_TYPE:
                if (VX_CHECK_PARAM(ptr, size, vx_enum, 0x3))
                {
                    *(vx_uint32 *)ptr = list->type;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_LIST_ATTRIBUTE_LENGTH:
                if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
                {
                    *(vx_size *)ptr = list->count;
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
        vxSemPost(&list->base.lock);
    }
    else
    {
        status = VX_ERROR_INVALID_REFERENCE;
    }
    return status;
}

vx_reference vxGetListItem(vx_list listref, vx_iterator *it, vx_enum iter)
{
    vx_reference ref = 0;
    vx_list_t *list = (vx_list_t *)listref;
    vx_item_t **pIter = (vx_item_t **)it;
    if (pIter == NULL && (iter != VX_LIST_FIRST && iter != VX_LIST_LAST))
        return 0;
    if (vxIsValidList(list) == vx_true_e)
    {
        vxSemWait(&list->base.lock);
        VX_PRINT(VX_ZONE_LIST, "Asking for iter=%x\n", iter);
        switch (iter)
        {
            case VX_LIST_FRONT:
                VX_PRINT(VX_ZONE_LIST, "Reseting current for head!\n");
                /* reset the list */
                (*pIter) = list->head;
                /* no break */
            case VX_LIST_FIRST:
                if (list->head)
                {
                    ref = list->head->ref;
                }
                break;
            case VX_LIST_BACK:
                /* reset the list */
                (*pIter) = list->tail;
                /* no break */
            case VX_LIST_LAST:
                if (list->tail)
                {
                    ref = list->tail->ref;
                }
                break;
            case VX_LIST_NEXT:
                if ((*pIter) && (*pIter)->next)
                {
                    VX_PRINT(VX_ZONE_LIST, "Moving to next item!\n");
                    (*pIter) = (*pIter)->next;
                    ref = (*pIter)->ref;
                }
                else if ((*pIter) == NULL)
                {
                    VX_PRINT(VX_ZONE_LIST, "Iter not set, assuming head!\n");
                    (*pIter) = list->head;
                    if ((*pIter))
                    {
                        ref = (*pIter)->ref;
                    }
                }
                break;
            case VX_LIST_PREVIOUS:
                if ((*pIter) && (*pIter)->prev)
                {
                    VX_PRINT(VX_ZONE_LIST, "Moving to previous item!\n");
                    (*pIter) = (*pIter)->prev;
                    ref = (*pIter)->ref;
                }
                else if ((*pIter) == NULL)
                {
                    VX_PRINT(VX_ZONE_LIST, "Iter not set, assuming tail!\n");
                    (*pIter) = list->tail;
                    if ((*pIter))
                    {
                        ref = (*pIter)->ref;
                    }
                }
                break;
        }
        vxSemPost(&list->base.lock);
    }
    VX_PRINT(VX_ZONE_LIST, "Iter="VX_FMT_REF" Returning "VX_FMT_REF"!\n", ((*pIter)?(*pIter)->ref:0), ref);
    return ref;
}

vx_status vxEraseListItem(vx_list listref, vx_iterator *it, vx_reference ref)
{
    vx_status status = VX_ERROR_INVALID_REFERENCE;
    vx_list_t *list = (vx_list_t *)listref;
    if (vxIsValidList(list) == vx_true_e)
    {
        vx_item_t *item = NULL;
        vxSemWait(&list->base.lock);
        item = list->head;
        while (item)
        {
            if (item->ref == ref)
            {
                vxPrintItem(item, "Removing");
                /* the list will decrement the reference internally */
                vxFreeItem(list, it, item);
                status = VX_SUCCESS;
                break;
            }
            else
            {
                item = item->next;
            }
        }
        vxSemPost(&list->base.lock);
    }
    return status;
}

vx_status vxAddListItem(vx_list listref, vx_reference ref)
{
    vx_status status = VX_ERROR_INVALID_REFERENCE;
    vx_list_t *list = (vx_list_t *)listref;
    vx_enum type = 0;

    if (listref == ref)
    {
        VX_PRINT(VX_ZONE_ERROR, "Can not add a list to itself\n");
        return VX_ERROR_INVALID_VALUE;
    }

    vxQueryReference(ref, VX_REF_ATTRIBUTE_TYPE, &type, sizeof(type));
    if (type != list->type)
    {
        VX_PRINT(VX_ZONE_ERROR, "Ref is Invalid type!\n");
        return VX_ERROR_INVALID_TYPE;
    }

    if (vxIsValidList(list) == vx_true_e)
    {
        vx_item_t *item = vxAllocItem(ref);
        vx_reference_t *reft = (vx_reference_t *)ref;
        status = VX_SUCCESS;
        vxSemWait(&list->base.lock);
        vxPrintItem(item, "New");
        vxPrintList(list);
        vxIncrementReference(reft);
        item->prev = list->tail;
        if (list->head == NULL)
        {
            list->head = item;
        }
        if (list->tail)
        {
            list->tail->next = item;
        }
        list->tail = item;
        list->count++;
        vxSemPost(&list->base.lock);
    }
    return status;
}

vx_status vxSortList(vx_list listref, vx_ref_compare_f sorter)
{
    vx_status status = VX_ERROR_INVALID_REFERENCE;
    vx_list_t *list = (vx_list_t *)listref;

    if (vxIsValidList(list) == vx_true_e)
    {
        status = VX_SUCCESS;
        if (list->count > 1)
        {
            vx_item_t *tmp = NULL, *next = NULL;
            vxSemWait(&list->base.lock);
            VX_PRINT(VX_ZONE_LIST, "Sorting List with sorter %p\n", sorter);
            /* remember the old list */
            tmp = list->head;
            next = tmp->next;
            /* reset the old list */
            list->head = NULL;
            list->tail = NULL;
            list->count = 0;
            do {
                /* remove tmp from the old list */
                tmp->next = NULL;
                tmp->prev = NULL;
                /* add it to the new list */
                status = vxInsertItem(list, tmp, sorter);
                if (status != VX_SUCCESS)
                {
                    /* list is in an indeterminate state. */
                    break;
                }
                /* go to the next old list item */
                tmp = next;
                if (next)
                {
                    next = next->next;
                }
            } while (tmp);
            vxSemPost(&list->base.lock);
        }
    }
    return status;
}

vx_status vxInsertListItem(vx_list listref, vx_reference ref, vx_ref_compare_f sorter)
{
    vx_status status = VX_ERROR_INVALID_REFERENCE;
    vx_list_t *list = (vx_list_t *)listref;
    vx_enum type = 0;

    if (listref == ref)
    {
        VX_PRINT(VX_ZONE_ERROR, "Can not add a list to itself\n");
        return VX_ERROR_INVALID_VALUE;
    }

    vxQueryReference(ref, VX_REF_ATTRIBUTE_TYPE, &type, sizeof(type));
    if (type != list->type)
    {
        VX_PRINT(VX_ZONE_ERROR, "Ref is Invalid type!\n");
        return VX_ERROR_INVALID_TYPE;
    }

    if ((vxIsValidList(list) == vx_true_e) && (sorter != NULL))
    {
        vx_item_t *item = vxAllocItem(ref);
        vx_reference_t *reft = (vx_reference_t *)ref;
        vxSemWait(&list->base.lock);
        vxPrintItem(item, "Inserting");
        vxPrintList(list);
        vxIncrementReference(reft);
        status = vxInsertItem(list, item, sorter);
        vxSemPost(&list->base.lock);
    }
    return status;
}

vx_status vxEmptyList(vx_list list)
{
    vx_status status = VX_ERROR_INVALID_REFERENCE;
    vx_list_t *plist = (vx_list_t *)list;
    if (vxIsValidList(plist) == vx_true_e)
    {
        vxEmptyListInternal(plist);
        status = VX_SUCCESS;
    }
    return status;
}
