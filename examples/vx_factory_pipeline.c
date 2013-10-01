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
 * \file vx_factory_pipeline.c
 * \example vx_factory_pipeline.c
 * \brief A simple image pipeline factory using OpenVX.
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <vx_graph_factory.h>
#include <VX/vx_ext_xyz.h>

/*!
 * \brief This will be called after the \ref vxMinMaxLocNode completes and determines the
 * maximum value in an image.
 * \ingroup group_example
 */
vx_action example_maximacallback(vx_node node)
{
    vx_status status = VX_SUCCESS;
    vx_uint32 max_intensity = 0;
    vx_scalar maxima;
    vx_parameter param = vxGetParameterByIndex(node, 2); // max value

    status |= vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &maxima, sizeof(maxima));
    status |= vxAccessScalarValue(maxima, &max_intensity);
    if (status == VX_SUCCESS)
    {
        if (max_intensity > 10)
            return VX_ACTION_CONTINUE;
        else if (max_intensity <= 10)
            /* edges aren't strong enough */
            return VX_ACTION_ABANDON;
    }
    return VX_ACTION_ABANDON;
}

/*! \brief A simple vision pipeline factory which also adds callbacks.
 * \ingroup group_example
 */
vx_graph vxPipelineGraphFactory(vx_context context)
{
    vx_status status = VX_SUCCESS;
    vx_bool running = vx_true_e;
    vx_uint32 i;
    vx_uint8 lo_threshold = 120;
    vx_uint8 hi_threshold = 240;
    vx_uint32 somevalue = 10;
    vx_uint8 max = 0;
    vx_enum channel = VX_CHANNEL_Y;
    vx_graph graph = vxCreateGraph(context);
    vxLoadKernels(context, "xyz");
    if (graph)
    {
        vx_image virts[] = {
            vxCreateVirtualImage(context),
            vxCreateVirtualImageWithFormat(context, FOURCC_S16),
            vxCreateVirtualImageWithFormat(context, FOURCC_S16),
            vxCreateVirtualImageWithFormat(context, FOURCC_U8),
            vxCreateVirtualImage(context),
            vxCreateVirtualImage(context),
        };
        vx_scalar scalars[] = {
            vxCreateScalar(context, VX_TYPE_UINT32, &somevalue),
            vxCreateScalar(context, VX_TYPE_ENUM, &channel),
            vxCreateScalar(context, VX_TYPE_UINT8, &max),
        };
        vx_kernel kernels[] = {
            vxGetKernelByEnum(context, VX_KERNEL_CHANNEL_EXTRACT),
            vxGetKernelByEnum(context, VX_KERNEL_SOBEL_3x3),
            vxGetKernelByEnum(context, VX_KERNEL_MAGNITUDE),
            vxGetKernelByEnum(context, VX_KERNEL_MINMAXLOC),
            vxGetKernelByEnum(context, VX_KERNEL_THRESHOLD),
            vxGetKernelByEnum(context, VX_KERNEL_KHR_XYZ),   // some extended user feature.
        };
        vx_node nodes[dimof(kernels)];
        vx_threshold thresh = vxCreateThreshold(context, VX_THRESHOLD_TYPE_BINARY);
        vx_buffer buf = vxCreateBuffer(context, 1, 374);

        vxSetThresholdAttribute(thresh, VX_THRESHOLD_ATTRIBUTE_VALUE, &lo_threshold, sizeof(lo_threshold));

        for (i = 0; i < dimof(nodes); i++)
            nodes[i] = vxCreateNode(graph, kernels[i]);

        // Channel Extract
        status |= vxAddParameterToGraphByIndex(graph, nodes[0], 0);
        status |= vxSetParameterByIndex(nodes[0], 1, VX_INPUT, scalars[1]);
        status |= vxSetParameterByIndex(nodes[0], 2, VX_OUTPUT, virts[0]);
        // Sobel 3x3
        status |= vxSetParameterByIndex(nodes[1], 0, VX_INPUT, virts[0]);
        status |= vxSetParameterByIndex(nodes[1], 1, VX_OUTPUT, virts[1]);
        status |= vxSetParameterByIndex(nodes[1], 2, VX_OUTPUT, virts[2]);
        // Magnitude
        status |= vxSetParameterByIndex(nodes[2], 0, VX_INPUT, virts[1]);
        status |= vxSetParameterByIndex(nodes[2], 1, VX_INPUT, virts[2]);
        status |= vxSetParameterByIndex(nodes[2], 2, VX_OUTPUT, virts[3]);
        // MinMaxLoc
        status |= vxSetParameterByIndex(nodes[3], 0, VX_INPUT, virts[3]);
        status |= vxSetParameterByIndex(nodes[3], 1, VX_INPUT, 0); //optional
        status |= vxSetParameterByIndex(nodes[3], 2, VX_INPUT, scalars[2]);
        status |= vxSetParameterByIndex(nodes[3], 3, VX_INPUT, 0); //optional
        status |= vxSetParameterByIndex(nodes[3], 4, VX_INPUT, 0); //optional
        // if the max is too low, abandon the graph.
        status |= vxAssignNodeCallback(nodes[3], example_maximacallback);
        // Threshold
        status |= vxSetParameterByIndex(nodes[4], 0, VX_INPUT, virts[3]);
        status |= vxSetParameterByIndex(nodes[4], 1, VX_INPUT, thresh);
        status |= vxSetParameterByIndex(nodes[4], 2, VX_OUTPUT, virts[4]);
        // XYZ
        status |= vxSetParameterByIndex(nodes[5], 0, VX_INPUT, virts[3]);
        status |= vxSetParameterByIndex(nodes[5], 1, VX_INPUT, scalars[0]);
        status |= vxAddParameterToGraphByIndex(graph, nodes[5], 2);
        status |= vxSetParameterByIndex(nodes[5], 3, VX_BIDIRECTIONAL, buf);

        vxReleaseBuffer(&buf);
        vxReleaseThreshold(&thresh);
        for (i = 0; i < dimof(nodes); i++)
        {
            vxReleaseNode(&nodes[i]);
        }
        for (i = 0; i < dimof(nodes); i++)
        {
            vxReleaseImage(&virts[i]);
        }
        for (i = 0; i < dimof(virts); i++)
        {
            vxReleaseScalar(&scalars[i]);
        }
        for (i = 0; i < dimof(kernels); i++)
        {
            vxReleaseKernel(&kernels[i]);
        }
    }
    return graph;
}


