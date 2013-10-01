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
#include <VX/vx_ext_xyz.h>
#include <VX/vx_helper.h>

/*!
 * \file vx_xyz_ext.c
 * \example vx_xyz_ext.c
 * \brief An example of how to write a user mode extension to OpenVX.
 * \author Erik Rainey <erik.rainey@ti.com>
 */

vx_node vxXYZNode(vx_graph graph, vx_image input, vx_uint32 value, vx_image output, vx_buffer temp)
{
    vx_uint32 i;
    vx_node node = 0;
    vx_context context = vxGetContext(graph);
    vx_status status = vxLoadKernels(context, "xyz");
    if (status == VX_SUCCESS)
    {
        vx_kernel kernel = vxGetKernelByName(context, "org.khronos.example.xyz");
        if (kernel)
        {
            node = vxCreateNode(graph, kernel);
            if (node)
            {
                vx_status statuses[4];
                vx_scalar scalar = vxCreateScalar(context, VX_TYPE_INT32, &value);
                statuses[0] = vxSetParameterByIndex(node, 0, VX_INPUT, input);
                statuses[1] = vxSetParameterByIndex(node, 1, VX_INPUT, scalar);
                statuses[2] = vxSetParameterByIndex(node, 2, VX_OUTPUT, output);
                statuses[3] = vxSetParameterByIndex(node, 3, VX_BIDIRECTIONAL, temp);
                for (i = 0; i < dimof(statuses); i++)
                {
                    if (statuses[i] != VX_SUCCESS)
                    {
                        status = VX_ERROR_INVALID_PARAMETERS;
                        vxReleaseNode(&node);
                        vxReleaseKernel(&kernel);
                        node = 0;
                        kernel = 0;
                        break;
                    }
                }
            }
            else
            {
                vxReleaseKernel(&kernel);
            }
        }
    }
    return node;
}

vx_status vxuXYZ(vx_image input, vx_uint32 value, vx_image output, vx_buffer temp)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxXYZNode(graph, input, value, output, temp);
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
    return status;
}

