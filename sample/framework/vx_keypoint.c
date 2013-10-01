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

vx_keypoint vxCreateKeypoint(vx_context c)
{
    vx_keypoint_int_t *ikp = NULL;
    vx_context_t *context = (vx_context_t *)c;
    if (vxIsValidContext(context) == vx_true_e)
    {
        ikp = VX_CALLOC(vx_keypoint_int_t);
        if (ikp)
        {
            vxInitReference(&ikp->base, context, VX_TYPE_KEYPOINT);
            vxIncrementReference(&ikp->base);
            vxAddReference(context, &ikp->base);
        }
    }
    return (vx_keypoint)ikp;
}

void vxReleaseKeypointInt(vx_keypoint_int_t *ikp)
{
    vxReleaseReference(&ikp->base, VX_TYPE_KEYPOINT, vx_true_e, NULL);
}

void vxReleaseKeypoint(vx_keypoint *kp)
{
    vx_keypoint_int_t *ikp = (vx_keypoint_int_t *)(kp?*kp:0);
    vxReleaseReference(&ikp->base, VX_TYPE_KEYPOINT, vx_false_e, NULL);
    if (kp) *kp = 0;
}

vx_status vxAccessKeypoint(vx_keypoint kp, vx_keypoint_t **ptr)
{
    vx_keypoint_int_t *ikp = (vx_keypoint_int_t *)kp;
    if (vxIsValidSpecificReference(&ikp->base, VX_TYPE_KEYPOINT) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;
    if (ptr && *ptr == NULL)
    {
        *ptr = &ikp->data;
    }
    else if (ptr && *ptr)
    {
        memcpy(*ptr, &ikp->data, sizeof(vx_keypoint_t));
    }
    vxIncrementReference(&ikp->base);
    return VX_SUCCESS;
}

vx_status vxCommitKeypoint(vx_keypoint Keypoint, vx_keypoint_t *ptr)
{
    vx_keypoint_int_t *ikp = (vx_keypoint_int_t *)Keypoint;
    if (vxIsValidSpecificReference(&ikp->base, VX_TYPE_KEYPOINT) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;
    if (ptr)
    {
        if (ptr != &ikp->data)
        {
            memcpy(&ikp->data, ptr, sizeof(vx_keypoint_t));
        }
    }
    vxDecrementReference(&ikp->base);
    return VX_SUCCESS;
}
