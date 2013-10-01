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
 * \brief The Debug Extensions Interface Library.
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_ext_debug.h>
#include <VX/vx_helper.h>

//*****************************************************************************
// PUBLIC INTERFACE
//*****************************************************************************

vx_node vxCopyImageNode(vx_graph graph, vx_image input, vx_image output)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph, VX_KERNEL_DEBUG_COPY_IMAGE, params, dimof(params));
}

vx_node vxCopyBufferNode(vx_graph graph, vx_buffer input, vx_buffer output)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph, VX_KERNEL_DEBUG_COPY_BUFFER, params, dimof(params));
}

vx_node vxFWriteImageNode(vx_graph graph, vx_image image, vx_char name[VX_MAX_FILE_NAME])
{
    vx_char *ptr = NULL;
    vx_status status = VX_SUCCESS;
    vx_node node = 0;
    vx_buffer filepath = vxCreateBuffer(vxGetContext(graph), sizeof(vx_char), VX_MAX_FILE_NAME);
    status = vxAccessBufferRange(filepath, 0, VX_MAX_FILE_NAME, (void **)&ptr);
    if (ptr && filepath && status == VX_SUCCESS)
    {
        vx_parameter_item_t params[] = {
            {VX_INPUT, image},
            {VX_INPUT, filepath},
        };
        strncpy(ptr, name, VX_MAX_FILE_NAME);
        vxCommitBufferRange(filepath, 0, VX_MAX_FILE_NAME, ptr);

        node = vxCreateNodeByStructure(graph, VX_KERNEL_DEBUG_FWRITE_IMAGE, params, dimof(params));
        vxReleaseBuffer(&filepath); /* the graph should add a reference to this, so we don't need it. */
    }
    return node;
}

vx_node vxFWriteBufferNode(vx_graph graph, vx_buffer buffer, vx_char name[VX_MAX_FILE_NAME])
{
    vx_char *ptr = NULL;
    vx_status status = VX_SUCCESS;
    vx_node node = 0;
    vx_buffer filepath = vxCreateBuffer(vxGetContext(graph), sizeof(vx_char), VX_MAX_FILE_NAME);
    status = vxAccessBufferRange(filepath, 0, VX_MAX_FILE_NAME, (void **)&ptr);
    if (ptr && filepath && status == VX_SUCCESS)
    {
        vx_parameter_item_t params[] = {
            {VX_INPUT, buffer},
            {VX_INPUT, filepath},
        };
        strncpy(ptr, name, VX_MAX_FILE_NAME);
        vxCommitBufferRange(filepath, 0, VX_MAX_FILE_NAME, ptr);

        node = vxCreateNodeByStructure(graph, VX_KERNEL_DEBUG_FWRITE_BUFFER, params, dimof(params));
        vxReleaseBuffer(&filepath); // the graph should add a reference to this, so we don't need it.
    }
    return node;
}

vx_node vxFReadImageNode(vx_graph graph, vx_char name[VX_MAX_FILE_NAME], vx_image image)
{
    vx_char *ptr = NULL;
    vx_status status = VX_SUCCESS;
    vx_node node = 0;
    vx_buffer filepath = vxCreateBuffer(vxGetContext(graph), sizeof(vx_char), VX_MAX_FILE_NAME);
    status = vxAccessBufferRange(filepath, 0, VX_MAX_FILE_NAME, (void **)&ptr);
    if (ptr && filepath && status == VX_SUCCESS)
    {
        vx_parameter_item_t params[] = {
            {VX_INPUT, filepath},
            {VX_OUTPUT, image},
        };
        strncpy(ptr, name, VX_MAX_FILE_NAME);
        vxCommitBufferRange(filepath, 0, VX_MAX_FILE_NAME, ptr);

        node = vxCreateNodeByStructure(graph, VX_KERNEL_DEBUG_FREAD_IMAGE, params, dimof(params));
        vxReleaseBuffer(&filepath); // the graph should add a reference to this, so we don't need it.
    }
    return node;
}

vx_node vxFReadBufferNode(vx_graph graph, vx_char name[VX_MAX_FILE_NAME], vx_buffer buffer)
{
    vx_char *ptr = NULL;
    vx_status status = VX_SUCCESS;
    vx_node node = 0;
    vx_buffer filepath = vxCreateBuffer(vxGetContext(graph), sizeof(vx_char), VX_MAX_FILE_NAME);
    status = vxAccessBufferRange(filepath, 0, VX_MAX_FILE_NAME, (void **)&ptr);
    if (ptr && filepath && status == VX_SUCCESS)
    {
        vx_parameter_item_t params[] = {
            {VX_INPUT, filepath},
            {VX_OUTPUT, buffer},
        };
        strncpy(ptr, name, VX_MAX_FILE_NAME);
        vxCommitBufferRange(filepath, 0, VX_MAX_FILE_NAME, ptr);

        node = vxCreateNodeByStructure(graph, VX_KERNEL_DEBUG_FWRITE_BUFFER, params, dimof(params));
        vxReleaseBuffer(&filepath); // the graph should add a reference to this, so we don't need it.
    }
    return node;
}

vx_node vxFillImageNode(vx_graph graph, vx_uint32 value, vx_image output)
{
    vx_scalar fill = vxCreateScalar(vxGetContext(graph),VX_TYPE_UINT32, &value);
    vx_parameter_item_t params[] = {
        {VX_INPUT, fill},
        {VX_OUTPUT, output},
    };
    vx_node node = vxCreateNodeByStructure(graph, VX_KERNEL_FILL_IMAGE, params, dimof(params));
    vxReleaseScalar(&fill); /* the graph will keep a reference */
    return node;
}

vx_node vxCheckImageNode(vx_graph graph, vx_image input, vx_uint32 value, vx_scalar errors)
{
    vx_scalar fill = vxCreateScalar(vxGetContext(graph),VX_TYPE_UINT32, &value);
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_INPUT, fill},
        {VX_OUTPUT, errors},
    };
    vx_node node = vxCreateNodeByStructure(graph, VX_KERNEL_CHECK_IMAGE, params, dimof(params));
    vxReleaseScalar(&fill); /* the graph will keep a reference */
    return node;
}

vx_node vxFillBufferNode(vx_graph graph, vx_uint8 value, vx_buffer output)
{
    vx_scalar fill = vxCreateScalar(vxGetContext(graph),VX_TYPE_UINT8, &value);
    vx_parameter_item_t params[] = {
        {VX_INPUT, fill},
        {VX_OUTPUT, output},
    };
    vx_node node = vxCreateNodeByStructure(graph, VX_KERNEL_FILL_BUFFER, params, dimof(params));
    vxReleaseScalar(&fill); /* the graph will keep a reference */
    return node;
}

vx_node vxCheckBufferNode(vx_graph graph, vx_buffer input, vx_uint8 value, vx_scalar errors)
{
    vx_scalar fill = vxCreateScalar(vxGetContext(graph),VX_TYPE_UINT8, &value);
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_INPUT, fill},
        {VX_OUTPUT, errors},
    };
    vx_node node = vxCreateNodeByStructure(graph, VX_KERNEL_CHECK_BUFFER, params, dimof(params));
    vxReleaseScalar(&fill); /* the graph will keep a reference */
    return node;
}

vx_node vxCompareImagesNode(vx_graph graph, vx_image a, vx_image b, vx_scalar diffs)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, a},
        {VX_INPUT, b},
        {VX_OUTPUT, diffs},
    };
    vx_node node = vxCreateNodeByStructure(graph, VX_KERNEL_COMPARE_IMAGE, params, dimof(params));
    return node;
}

vx_node vxCopyImageFromPtrNode(vx_graph graph, void *ptr, vx_image output)
{
    vx_scalar scalar = vxCreateScalar(vxGetContext(graph), VX_TYPE_SIZE, &ptr);
    vx_parameter_item_t params[] = {
        {VX_INPUT, scalar},
        {VX_OUTPUT, output},
    };
    vx_node node = vxCreateNodeByStructure(graph,
                                           VX_KERNEL_COPY_IMAGE_FROM_PTR,
                                           params,
                                           dimof(params));
    vxReleaseScalar(&scalar);
    return node;
}

/* IMMEDIATE INTERFACES */

vx_status vxuCopyImage(vx_image src, vx_image dst)
{
    vx_context context = vxGetContext(src);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxCopyImageNode(graph, src, dst);
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

vx_status vxuCopyBuffer(vx_buffer src, vx_buffer dst)
{
    vx_context context = vxGetContext(src);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxCopyBufferNode(graph, src, dst);
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


vx_status vxuFWriteImage(vx_image image, vx_char name[VX_MAX_FILE_NAME])
{
    vx_context context = vxGetContext(image);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxFWriteImageNode(graph, image, name);
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

vx_status vxuFWriteBuffer(vx_buffer buffer, vx_char name[VX_MAX_FILE_NAME])
{
    vx_context context = vxGetContext(buffer);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxFWriteBufferNode(graph, buffer, name);
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

vx_status vxuFillImage(vx_uint32 value, vx_image output)
{
    vx_context context = vxGetContext(output);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxFillImageNode(graph, value, output);
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

vx_status vxuCheckImage(vx_image input, vx_uint32 value, vx_uint32 *numErrors)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_scalar errs = vxCreateScalar(context, VX_TYPE_UINT32, numErrors);
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxCheckImageNode(graph, input, value, errs);
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
    vxCommitScalarValue(errs, numErrors);
    vxReleaseScalar(&errs);
    return status;
}


vx_status vxuFillBuffer(vx_uint8 value, vx_buffer output)
{
    vx_context context = vxGetContext(output);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxFillBufferNode(graph, value, output);
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

vx_status vxuCheckBuffer(vx_buffer input, vx_uint8 value, vx_uint32 *numErrors)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_scalar errs = vxCreateScalar(context, VX_TYPE_UINT32, numErrors);
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxCheckBufferNode(graph, input, value, errs);
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
    vxCommitScalarValue(errs, numErrors);
    vxReleaseScalar(&errs);
    return status;
}

vx_status vxuCompareImages(vx_image a, vx_image b, vx_uint32 *numDiffs)
{
    vx_context context = vxGetContext(a);
    vx_status status = VX_FAILURE;
    vx_scalar diffs = vxCreateScalar(context, VX_TYPE_UINT32, numDiffs);
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxCompareImagesNode(graph, a, b, diffs);
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
    vxCommitScalarValue(diffs, numDiffs);
    vxReleaseScalar(&diffs);
    return status;
}


vx_status vxuCopyImageFromPtr(void *ptr, vx_image dst)
{
    vx_context context = vxGetContext(dst);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxCopyImageFromPtrNode(graph, ptr, dst);
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

