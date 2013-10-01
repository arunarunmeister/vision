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

#include <VX/vx.h>
#include <VX/vx_helper.h>
#ifdef OPENVX_USE_LIST
#include <VX/vx_khr_list.h>
#endif
#include <math.h>

/*! \file vx_helper.c
 * \brief The OpenVX Helper Implementation.
 * \author Erik Rainey <erik.rainey@ti.com>
 * \example vx_helper.c
 */

vx_target vxFindTarget(vx_context context, vx_char name[VX_MAX_TARGET_NAME])
{
    vx_uint32 t, targets;
    vx_target target = 0;
    vx_char targetName[VX_MAX_TARGET_NAME];
    vxQueryContext(context, VX_CONTEXT_ATTRIBUTE_NUMTARGETS, &targets, sizeof(targets));
    for (t = 0u; t < targets; t++)
    {
        target = vxGetTargetByIndex(context,t);
        vxQueryTarget(target, VX_TARGET_ATTRIBUTE_NAME, &targetName, VX_MAX_TARGET_NAME);
        if (strncmp(targetName, name, VX_MAX_TARGET_NAME) == 0)
        {
            break;
        }
        vxReleaseTarget(&target);
        target = 0;
    }
    return target;
}

vx_node vxCreateNodeByStructure(vx_graph graph,
                                vx_enum kernelenum,
                                vx_parameter_item_t *params,
                                vx_uint32 num)
{
    vx_status status = VX_SUCCESS;
    vx_node node = 0;
    vx_context context = vxGetContext(graph);
    vx_kernel kernel = vxGetKernelByEnum(context, kernelenum);
    if (kernel)
    {
        node = vxCreateNode(graph, kernel);
        if (node)
        {
            vx_uint32 p = 0;
            for (p = 0; p < num; p++)
            {
                status = vxSetParameterByIndex(node,
                                               p,
                                               params[p].direction,
                                               params[p].reference);
                if (status != VX_SUCCESS)
                {
                    vxAddLogEntry(graph, status, "Kernel %d Parameter %u is invalid.\n", kernelenum, p);
                    vxReleaseNode(&node);
                    node = 0;
                    break;
                }
            }
        }
        else
        {
            vxAddLogEntry(graph, VX_ERROR_INVALID_PARAMETERS, "Failed to create node with kernel enum %d\n", kernelenum);
            status = VX_ERROR_NO_MEMORY;
        }
        vxReleaseKernel(&kernel);
    }
    else
    {
        vxAddLogEntry(graph, VX_ERROR_INVALID_PARAMETERS, "failed to retrieve kernel enum %d\n", kernelenum);
        status = VX_ERROR_NOT_SUPPORTED;
    }
    return node;
}


void vxClearLog(vx_graph graph)
{
    char message[VX_MAX_LOG_MESSAGE_LEN];
    vx_status status = VX_SUCCESS;
    do {
        status = vxGetLogEntry(graph, message);
    } while (status != VX_SUCCESS);
}

vx_status vxLinkParametersByIndex(vx_node node_a, vx_enum index_a, vx_node node_b, vx_enum index_b) {
    vx_parameter param_a = vxGetParameterByIndex(node_a, index_a);
    vx_parameter param_b = vxGetParameterByIndex(node_b, index_b);
    vx_status status = vxLinkParametersByReference(param_a, param_b);
    vxReleaseParameter(&param_a);
    vxReleaseParameter(&param_b);
    return status;
}

vx_status vxLinkParametersByReference(vx_parameter a, vx_parameter b) {
    vx_status status = VX_SUCCESS;
    vx_enum dirs[2] = {0,0};
    vx_enum types[2] = {0,0};
    vx_reference ref = 0;

    status = vxQueryParameter(a, VX_PARAMETER_ATTRIBUTE_DIRECTION, &dirs[0], sizeof(dirs[0]));
    if (status != VX_SUCCESS)
        return status;

    status = vxQueryParameter(b, VX_PARAMETER_ATTRIBUTE_DIRECTION, &dirs[1], sizeof(dirs[1]));
    if (status != VX_SUCCESS)
        return status;

    status = vxQueryParameter(a, VX_PARAMETER_ATTRIBUTE_TYPE, &types[0], sizeof(types[0]));
    if (status != VX_SUCCESS)
        return status;

    status = vxQueryParameter(b, VX_PARAMETER_ATTRIBUTE_TYPE, &types[1], sizeof(types[1]));
    if (status != VX_SUCCESS)
        return status;

    if (types[0] == types[1])
    {
        if ((dirs[0] == VX_OUTPUT || dirs[0] == VX_BIDIRECTIONAL) && dirs[1] == VX_INPUT)
        {
            status = vxQueryParameter(a, VX_PARAMETER_ATTRIBUTE_REF, &ref, sizeof(ref));
            if (status != VX_SUCCESS)
                return status;
            status = vxSetParameterByReference(b, ref);
        }
        else if ((dirs[1] == VX_OUTPUT || dirs[1] == VX_BIDIRECTIONAL) && dirs[0] == VX_INPUT)
        {
            status = vxQueryParameter(b, VX_PARAMETER_ATTRIBUTE_REF, &ref, sizeof(ref));
            if (status != VX_SUCCESS)
                return status;
            status = vxSetParameterByReference(a, ref);
        }
        else
            status = VX_ERROR_INVALID_LINK;
    }
    return status;
}


vx_status vxSetAffineRotationMatrix(vx_matrix matrix,
                                    vx_float32 angle,
                                    vx_float32 scale,
                                    vx_float32 center_x,
                                    vx_float32 center_y)
{
    vx_status status = VX_FAILURE;
    vx_float32 mat[3][2];
    vx_size columns = 0ul, rows = 0ul;
    vx_enum type = 0;
    vxQueryMatrix(matrix, VX_MATRIX_ATTRIBUTE_COLUMNS, &columns, sizeof(columns));
    vxQueryMatrix(matrix, VX_MATRIX_ATTRIBUTE_ROWS, &rows, sizeof(rows));
    vxQueryMatrix(matrix, VX_MATRIX_ATTRIBUTE_TYPE, &type, sizeof(type));
    if ((columns == 2) && (rows == 3) && (type == VX_TYPE_FLOAT32))
    {
        status = vxAccessMatrix(matrix, mat);
        if (status == VX_SUCCESS)
        {
            vx_float32 radians = (angle / 360.0f) * (VX_TAU);
            vx_float32 a = scale * cos(radians);
            vx_float32 b = scale * sin(radians);
            mat[0][0] = a;
            mat[1][0] = b;
            mat[2][0] = ((1.0f - a) * center_x) - (b * center_y);
            mat[0][1] = -b;
            mat[1][1] = a;
            mat[2][1] = (b * center_y) + ((1.0f - a) * center_y);
            status = vxCommitMatrix(matrix, mat);
        }
    }
    else
    {
        vxAddLogEntry(matrix, status, "Failed to set affine matrix due to type or dimension mismatch!\n");
    }
    return status;
}

#ifdef OPENVX_KHR_LIST
vx_enum vxHarrisScoreSorter(vx_reference a, vx_reference b)
{
    vx_keypoint ca = (vx_keypoint)a;
    vx_keypoint cb = (vx_keypoint)b;
    vx_keypoint_t kpa, *pkpa = &kpa;
    vx_keypoint_t kpb, *pkpb = &kpb;
    vx_enum cmp = VX_COMPARE_UNKNOWN;
    if ((vxAccessKeypoint(ca, &pkpa) == VX_SUCCESS) &&
        (vxAccessKeypoint(cb, &pkpb) == VX_SUCCESS))
    {
        if (kpa.strength < kpb.strength)
            cmp = VX_COMPARE_LT;
        else if (kpa.strength == kpb.strength)
            cmp = VX_COMPARE_EQ;
        else if (kpa.strength > kpb.strength)
            cmp = VX_COMPARE_GT;
        vxCommitKeypoint(ca, pkpa);
        vxCommitKeypoint(cb, pkpb);
    }
    if (cmp == VX_COMPARE_UNKNOWN)
    {
        vxAddLogEntry(a, VX_ERROR_INVALID_REFERENCE, "FATAL: References given to list sorter are not desired type!\n");
    }
    return cmp;
}
#endif

vx_status vxAlterRectangle(vx_rectangle rect,
                           vx_int32 dsx,
                           vx_int32 dsy,
                           vx_int32 dex,
                           vx_int32 dey)
{
    vx_uint32 sx, sy, ex, ey;
    if (vxAccessRectangleCoordinates(rect, &sx, &sy, &ex, &ey) == VX_SUCCESS)
    {
        sx += dsx;
        sy += dsy;
        ex += dex;
        ey += dey;
        return vxCommitRectangleCoordinates(rect, sx, sy, ex, ey);
    }
    return VX_ERROR_INVALID_REFERENCE;
}

vx_status vxAddParameterToGraphByIndex(vx_graph g, vx_node n, vx_uint32 index)
{
    vx_parameter p = vxGetParameterByIndex(n, index);
    vx_status status = vxAddParameterToGraph(g, p);
    vxReleaseParameter(&p);
    return status;
}
