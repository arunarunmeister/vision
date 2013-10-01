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
 * \file vx_factory_corners.c
 * \example vx_factory_corners.c
 * \author Erik Rainey <erik.rainey@ti.com>
 * \brief An example of a graph to pre-process image data for a Corner Registration Graph.
 */

#include <vx_graph_factory.h>

/*! \brief An example of Corner Detection Graph Factory.
 * \ingroup group_example
 */
vx_graph vxCornersGraphFactory(vx_context context)
{
    vx_status  status = VX_SUCCESS;
    vx_uint32  i;
    vx_float32 strength_thresh = 10000.0f;
    vx_float32 r = 1.5f;
    vx_float32 sensitivity = 0.14f;
    vx_int32 window_size = 3;
    vx_int32 block_size = 3;
    vx_enum channel = VX_CHANNEL_Y;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_image virts[] = {
            vxCreateVirtualImage(context),
            vxCreateVirtualImage(context),
        };
        vx_kernel kernels[] = {
            vxGetKernelByEnum(context, VX_KERNEL_CHANNEL_EXTRACT),
            vxGetKernelByEnum(context, VX_KERNEL_MEDIAN_3x3),
            vxGetKernelByEnum(context, VX_KERNEL_HARRIS_CORNERS),
        };
        vx_node nodes[dimof(kernels)] = {
            vxCreateNode(graph, kernels[0]),
            vxCreateNode(graph, kernels[1]),
            vxCreateNode(graph, kernels[2]),
        };
        vx_scalar scalars[] = {
            vxCreateScalar(context, VX_TYPE_ENUM, &channel),
            vxCreateScalar(context, VX_TYPE_FLOAT32, &strength_thresh),
            vxCreateScalar(context, VX_TYPE_FLOAT32, &r),
            vxCreateScalar(context, VX_TYPE_FLOAT32, &sensitivity),
            vxCreateScalar(context, VX_TYPE_INT32, &window_size),
            vxCreateScalar(context, VX_TYPE_INT32, &block_size),
        };
        // Channel Extract
        status |= vxAddParameterToGraphByIndex(graph, nodes[0], 0);
        status |= vxSetParameterByIndex(nodes[0], 1, VX_INPUT, scalars[0]);
        status |= vxSetParameterByIndex(nodes[0], 2, VX_OUTPUT, virts[0]);
        // Median Filter
        status |= vxSetParameterByIndex(nodes[1], 0, VX_INPUT, virts[0]);
        status |= vxSetParameterByIndex(nodes[1], 1, VX_OUTPUT, virts[1]);
        // Harris Corners
        status |= vxSetParameterByIndex(nodes[2], 0, VX_INPUT, virts[1]);
        status |= vxSetParameterByIndex(nodes[2], 1, VX_INPUT, scalars[1]);
        status |= vxSetParameterByIndex(nodes[2], 2, VX_INPUT, scalars[2]);
        status |= vxSetParameterByIndex(nodes[2], 3, VX_INPUT, scalars[3]);
        status |= vxSetParameterByIndex(nodes[2], 4, VX_INPUT, scalars[4]);
        status |= vxSetParameterByIndex(nodes[2], 5, VX_INPUT, scalars[5]);
        status |= vxAddParameterToGraphByIndex(graph, nodes[2], 6);

        for (i = 0; i < dimof(scalars); i++)
        {
            vxReleaseScalar(&scalars[i]);
        }
        for (i = 0; i < dimof(virts); i++)
        {
            vxReleaseImage(&virts[i]);
        }
        for (i = 0; i < dimof(kernels); i++)
        {
            vxReleaseKernel(&kernels[i]);
        }
        for (i = 0; i < dimof(nodes);i++)
        {
            vxReleaseNode(&nodes[i]);
        }
    }
    return graph;
}
