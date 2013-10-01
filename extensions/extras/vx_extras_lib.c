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

/*!
 * \file
 * \brief The Khronos Extras Extension Node and Immediate Interfaces
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_ext_extras.h>
#include <VX/vx_helper.h>

//*****************************************************************************
// PUBLIC INTERFACE
//*****************************************************************************

vx_node vxNonMaxSuppressionNode(vx_graph graph, vx_image mag, vx_image phase, vx_image edge)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, mag},
        {VX_INPUT, phase},
        {VX_OUTPUT, edge},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_EXTRAS_NONMAXSUPPRESSION,
                                   params, dimof(params));
}

vx_status vxuNonMaxSuppression(vx_image mag, vx_image phase, vx_image edge)
{
    vx_status status = VX_SUCCESS;
    vx_graph graph = vxCreateGraph(vxGetContext(mag));
    if (graph)
    {
        vx_node node = vxNonMaxSuppressionNode(graph, mag, phase, edge);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&graph);
        }
        vxReleaseGraph(&graph);
    }
    return status;
}

vx_node vxLaplacian3x3Node(vx_graph graph, vx_image input, vx_image output)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_EXTRAS_LAPLACIAN_3x3,
                                   params,
                                   dimof(params));
}

vx_status vxuLaplacian3x3(vx_image input, vx_image output)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxLaplacian3x3Node(graph, input, output);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_node vxScharr3x3Node(vx_graph graph, vx_image input, vx_image output)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_EXTRAS_SCHARR_3x3,
                                   params,
                                   dimof(params));
}

vx_status vxuScharr3x3(vx_image input, vx_image output)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxScharr3x3Node(graph, input, output);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_node vxSobelMxNNode(vx_graph graph, vx_image input, vx_scalar ws, vx_image gx, vx_image gy)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_INPUT, ws},
        {VX_OUTPUT, gx},
        {VX_OUTPUT, gy},
    };
    vx_node node = vxCreateNodeByStructure(graph,
                                   VX_KERNEL_EXTRAS_SOBEL_MxN,
                                   params,
                                   dimof(params));
    return node;
}

vx_status vxuSobelMxN(vx_image input, vx_scalar win, vx_image gx, vx_image gy)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxSobelMxNNode(graph, input, win, gx, gy);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_node vxHarrisScoreNode(vx_graph graph,
                          vx_image gx,
                          vx_image gy,
                          vx_scalar sensitivity,
                          vx_scalar block_size,
                          vx_list corners)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, gx},
        {VX_INPUT, gy},
        {VX_INPUT, sensitivity},
        {VX_INPUT, block_size},
        {VX_OUTPUT, corners},
    };
    vx_node node = vxCreateNodeByStructure(graph,
                                           VX_KERNEL_EXTRAS_HARRIS_SCORE,
                                           params,
                                           dimof(params));
    return node;
}

vx_status vxuHarrisScore(vx_image gx,
                         vx_image gy,
                         vx_scalar sensitivity,
                         vx_scalar block_size,
                         vx_list corners)
{
    vx_context context = vxGetContext(gx);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxHarrisScoreNode(graph, gx, gy, sensitivity, block_size, corners);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_node vxEuclideanNonMaxNode(vx_graph graph,
                              vx_image input,
                              vx_scalar strength_thresh,
                              vx_scalar min_distance,
                              vx_image output)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_INPUT, strength_thresh},
        {VX_INPUT, min_distance},
        {VX_OUTPUT, output},
    };
    vx_node node = vxCreateNodeByStructure(graph,
                                           VX_KERNEL_EXTRAS_EUCLIDEAN_NONMAXSUPPRESSION,
                                           params,
                                           dimof(params));
    return node;
}

vx_status vxuEuclideanNonMax(vx_image input,
                             vx_scalar strength_thresh,
                             vx_scalar min_distance,
                             vx_image output)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxEuclideanNonMaxNode(graph, input, strength_thresh, min_distance, output);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_node vxImageListerNode(vx_graph graph, vx_image input, vx_list list)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_OUTPUT, list},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_EXTRAS_IMAGE_LISTER,
                                   params,
                                   dimof(params));
}

vx_status vxuImageLister(vx_image input,
                         vx_list list)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxImageListerNode(graph, input, list);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}
