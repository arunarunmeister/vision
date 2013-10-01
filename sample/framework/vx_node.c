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

void vxPrintNode(vx_node_t *node)
{
    if (node)
        VX_PRINT(VX_ZONE_NODE, "vx_node_t:%p %s:%d affinity:%s\n",
            node,
            node->kernel->name,
            node->kernel->enumeration,
            node->base.context->targets[node->affinity].name);
}

vx_node vxCreateNode(vx_graph g, vx_kernel k)
{
    vx_graph_t *graph = (vx_graph_t *)g;
    vx_kernel_t *kernel = (vx_kernel_t *)k;
    if ((vxIsValidSpecificReference(&graph->base, VX_TYPE_GRAPH) == vx_true_e) &&
        (vxIsValidSpecificReference(&kernel->base, VX_TYPE_KERNEL) == vx_true_e))
    {
        vx_uint32 n = 0;
        vx_node_t *node = NULL;
        vxSemWait(&graph->base.lock);
        for (n = 0; n < VX_INT_MAX_REF; n++)
        {
            if (graph->nodes[n] == NULL)
            {
                node = VX_CALLOC(vx_node_t);
                if (node)
                {
                    vxInitReference((vx_reference_t *)node, graph->base.context, VX_TYPE_NODE);
                    vxIncrementReference(&node->base); /* one for the user */
                    vxAddReference(node->base.context, (vx_reference_t *)node);

                    /* reference the abstract kernel. */
                    node->kernel = kernel;
                    node->affinity = kernel->affinity;

                    /* show that there are potentially multiple nodes using this kernel. */
                    vxIncrementIntReference(&kernel->base);

                    /* copy the attributes over */
                    memcpy(&node->attributes, &kernel->attributes, sizeof(vx_kernel_attr_t));

                    /* setup our forward and back references to the node/graph */
                    graph->nodes[n] = node;
                    vxIncrementIntReference(&node->base); /* one for the graph */
                    node->graph = graph;

                    /* increase the count of nodes in the graph. */
                    graph->numNodes++;

                    vxInitPerf(&graph->nodes[n]->perf);

                    /* force a re-verify */
                    graph->verified = vx_false_e;

                    VX_PRINT(VX_ZONE_NODE, "Created Node %p %s affinity:%s\n", node, node->kernel->name, node->base.context->targets[node->affinity].name);
                }
                break; /* suceed or fail, break. */
            }
        }
        vxSemPost(&graph->base.lock);
        vxPrintReference((vx_reference_t *)node);
        return (vx_node)node;
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Either graph %p or kernel %p was invalid!\n", graph, kernel);
    }
    return 0;
}

vx_status vxQueryNode(vx_node n, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_node_t *node = (vx_node_t *)n;
    if (vxIsValidSpecificReference(&node->base, VX_TYPE_NODE) == vx_true_e)
    {
        switch (attribute)
        {
            case VX_NODE_ATTRIBUTE_PERFORMANCE:
                if (VX_CHECK_PARAM(ptr, size, vx_perf_t, 0x3))
                {
                    memcpy(ptr, &node->perf, size);
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_NODE_ATTRIBUTE_STATUS:
                if (VX_CHECK_PARAM(ptr, size, vx_status, 0x3))
                {
                    *(vx_status *)ptr = node->status;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_NODE_ATTRIBUTE_LOCAL_DATA_SIZE:
                if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
                {
                    *(vx_size *)ptr = node->attributes.localDataSize;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_NODE_ATTRIBUTE_LOCAL_DATA_PTR:
                if (VX_CHECK_PARAM(ptr, size, vx_ptr_t, 0x3))
                {
                    *(vx_ptr_t *)ptr = node->attributes.localDataPtr;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
#ifdef OPENVX_KHR_NODE_MEMORY
            case VX_NODE_ATTRIBUTE_GLOBAL_DATA_SIZE:
                if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
                {
                    *(vx_size *)ptr = node->attributes.globalDataSize;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_NODE_ATTRIBUTE_GLOBAL_DATA_PTR:
                if (VX_CHECK_PARAM(ptr, size, vx_ptr_t, 0x3))
                {
                    *(vx_ptr_t *)ptr = node->attributes.globalDataPtr;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
#endif
            case VX_NODE_ATTRIBUTE_BORDER_MODE:
                if (VX_CHECK_PARAM(ptr, size, vx_border_mode_t, 0x3))
                {
                    VX_PRINT(VX_ZONE_NODE, "Border mode %x set!\n", node->attributes.borders.mode);
                    memcpy((vx_border_mode_t *)ptr, &node->attributes.borders, sizeof(vx_border_mode_t));
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
#ifdef OPENVX_KHR_TILING
            case VX_NODE_ATTRIBUTE_INPUT_NEIGHBORHOOD:
                if (VX_CHECK_PARAM(ptr, size, vx_neighborhood_size_t, 0x3))
                {
                    memcpy(ptr, &node->kernel->attributes.nhbdinfo, size);
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_NODE_ATTRIBUTE_OUTPUT_TILE_BLOCK_SIZE:
                if (VX_CHECK_PARAM(ptr, size, vx_tile_block_size_t, 0x3))
                {
                    memcpy(ptr, &node->kernel->attributes.blockinfo, size);
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_NODE_ATTRIBUTE_TILE_MEMORY_SIZE:
                if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
                {
                    *(vx_size *)ptr = node->attributes.tileDataSize;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_NODE_ATTRIBUTE_TILE_MEMORY_PTR:
                if (VX_CHECK_PARAM(ptr, size, vx_ptr_t, 0x3))
                {
                    *(vx_ptr_t *)ptr = node->attributes.tileDataPtr;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
#endif
            default:
                status = VX_ERROR_NOT_SUPPORTED;
                break;
        }
    }
    else
    {
        status = VX_ERROR_INVALID_REFERENCE;
    }
    VX_PRINT(VX_ZONE_API, "Returned %d\n", status);
    return status;
}

vx_status vxSetNodeAttribute(vx_node n, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_node_t *node = (vx_node_t *)n;
    if (vxIsValidSpecificReference(&node->base, VX_TYPE_NODE) == vx_true_e)
    {
        if (node->graph->verified == vx_true_e)
        {
            return VX_ERROR_NOT_SUPPORTED;
        }
        switch (attribute)
        {
            case VX_NODE_ATTRIBUTE_LOCAL_DATA_SIZE:
                if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
                {
                    node->attributes.localDataSize = *(vx_size *)ptr;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_NODE_ATTRIBUTE_LOCAL_DATA_PTR:
                if (VX_CHECK_PARAM(ptr, size, vx_ptr_t, 0x3))
                {
                    node->attributes.localDataPtr = *(vx_ptr_t *)ptr;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_NODE_ATTRIBUTE_BORDER_MODE:
                if (VX_CHECK_PARAM(ptr, size, vx_border_mode_t, 0x3))
                {
#ifdef OPENVX_KHR_TILING
                    if (node->attributes.borders.mode == VX_BORDER_MODE_SELF)
                    {
                        status = VX_ERROR_INVALID_VALUE;
                    }
                    else
#endif
                    {
                        memcpy(&node->attributes.borders, (vx_border_mode_t *)ptr, sizeof(vx_border_mode_t));
                    }
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
    }
    else
    {
        status = VX_ERROR_INVALID_REFERENCE;
    }
    return status;
}

void vxReleaseNode(vx_node *n)
{
    vx_node_t *node = (vx_node_t *)(n?*n:0);
    vxReleaseNodeInt(node, vx_false_e);
    if (n) *n = 0;
}

void vxReleaseNodeInt(vx_node_t *node, vx_bool internal)
{
    if (vxIsValidSpecificReference(&node->base, VX_TYPE_NODE) == vx_true_e)
    {
        if (internal == vx_false_e)
            vxDecrementReference(&node->base);
        else
            vxDecrementIntReference(&node->base);
        if (vxTotalReferenceCount(&node->base) == 0)
        {
            vx_uint32 n = 0;
            vx_uint32 p = 0;
            if (node->graph)
            {
                vxSemWait(&node->graph->base.lock);
                /* remove the reference from the graph */
                for (n = 0; n < node->graph->numNodes; n++)
                {
                    if (node->graph->nodes[n] == node)
                    {
                        node->graph->nodes[n] = NULL;
                        /* force the graph to be verified again */
                        node->graph->verified = vx_false_e;
                    }
                }
                vxSemPost(&node->graph->base.lock);
            }

            if (node->kernel == NULL)
            {
                VX_PRINT(VX_ZONE_ERROR, "Node has no kernel!\n");
                return;
            }

            /* remove, don't delete, all references from the node itself */
            for (p = 0; p < node->kernel->signature.numParams; p++)
            {
                vx_reference_t *ref = node->parameters[p];
                if (ref)
                {
                    switch (ref->type)
                    {
                        case VX_TYPE_BUFFER:
                            vxReleaseBufferInt((vx_buffer_t *)ref);
                            break;
                        case VX_TYPE_CONVOLUTION:
                            vxReleaseConvolutionInt((vx_convolution_t *)ref);
                            break;
                        case VX_TYPE_COORDINATES:
                            vxReleaseCoordinatesInt((vx_coordinates_t *)ref);
                            break;
                        case VX_TYPE_DISTRIBUTION:
                            vxReleaseDistributionInt((vx_distribution_t *)ref);
                            break;
                        case VX_TYPE_IMAGE:
                            vxReleaseImageInt((vx_image_t *)ref);
                            break;
                        case VX_TYPE_LIST:
                            vxReleaseListInt((vx_list_t *)ref);
                            break;
                        case VX_TYPE_LUT:
                            vxReleaseLUTInt((vx_lut_t *)ref);
                            break;
                        case VX_TYPE_MATRIX:
                            vxReleaseMatrixInt((vx_matrix_t *)ref);
                            break;
                        case VX_TYPE_PYRAMID:
                            vxReleasePyramidInt((vx_pyramid_t *)ref);
                            break;
                        case VX_TYPE_RECTANGLE:
                            vxReleaseRectangleInt((vx_rectangle_t *)ref);
                            break;
                        case VX_TYPE_SCALAR:
                            vxReleaseScalarInt((vx_scalar_t *)ref);
                            break;
                        case VX_TYPE_THRESHOLD:
                            vxReleaseThresholdInt((vx_threshold_t *)ref);
                            break;
                        /*! \todo add more types here as they come along!!! */
                        default:
                            VX_PRINT(VX_ZONE_ERROR, "Attempting to destroy reference "VX_FMT_REF" of an unknown type %08x\n", ref, ref->type);
                            break;
                    }
                    node->parameters[p] = NULL;
                }
            }

            /* free the local memory */
            if (node->attributes.localDataPtr)
            {
                free(node->attributes.localDataPtr);
                node->attributes.localDataPtr = NULL;
            }

            /* de-initialize the kernel */
            if (node->kernel->deinitialize)
            {
                vx_status status;
                status = node->kernel->deinitialize((vx_node)node,
                                                    (vx_reference *)node->parameters,
                                                    node->kernel->signature.numParams);
                if (status != VX_SUCCESS)
                {
                    VX_PRINT(VX_ZONE_ERROR,"Failed to de-initialize kernel %s!\n", node->kernel->name);
                }
            }
            /* let go of the internal ref count */
            vxDecrementIntReference(&node->kernel->base);
            node->kernel = NULL;

            vxRemoveReference(node->base.context, (vx_reference_t *)node);
            free(node);
        }
    }
}

vx_status vxAssignNodeCallback(vx_node n, vx_nodecomplete_f callback)
{
    vx_status status = VX_ERROR_INVALID_REFERENCE;
    vx_node_t *node = (vx_node_t *)n;
    if (vxIsValidSpecificReference(&node->base, VX_TYPE_NODE) == vx_true_e)
    {
        if ((callback) && (node->callback))
        {
            VX_PRINT(VX_ZONE_ERROR, "Attempting to overriding existing callback %p on Node %s!\n", node->callback, node->kernel->name);
            status = VX_ERROR_NOT_SUPPORTED;
        }
        else
        {
            node->callback = callback;
            status = VX_SUCCESS;
        }
    }
    return status;
}

vx_nodecomplete_f vxRetrieveNodeCallback(vx_node n)
{
    vx_node_t *node = (vx_node_t *)n;
    vx_nodecomplete_f cb = NULL;
    if (vxIsValidSpecificReference(&node->base, VX_TYPE_NODE) == vx_true_e)
    {
        cb = node->callback;
    }
    return cb;
}


// ![SAMPLE EXTENSION]
vx_status vxSetChildGraphOfNode(vx_node n, vx_graph child)
{
    vx_status status = VX_ERROR_INVALID_GRAPH;
    vx_node_t *node = (vx_node_t *)n;
    vx_graph_t *graph = (vx_graph_t *)child;
    vx_bool valid_node = vxIsValidSpecificReference(&node->base, VX_TYPE_NODE);
    if ((valid_node == vx_true_e) &&
        (vxIsValidSpecificReference(&graph->base, VX_TYPE_GRAPH) == vx_true_e))
    {
        vx_uint32 p, num = 0;
        /* check to make sure the signature of the node matches the signature of the graph. */
        if (node->kernel->signature.numParams == graph->numParams)
        {
            vx_bool match = vx_true_e;
            for (p = 0; p < num; p++)
            {
                vx_uint32 child_index = graph->parameters[p].index;
                if ((node->kernel->signature.directions[p] != graph->parameters[p].node->kernel->signature.directions[child_index]) ||
                    (node->kernel->signature.states[p] != graph->parameters[p].node->kernel->signature.states[child_index]) ||
                    (node->kernel->signature.types[p] != graph->parameters[p].node->kernel->signature.types[child_index]))
                {
                    vxAddLogEntry(child, VX_ERROR_INVALID_PARAMETERS, "Graph parameter %u does not match node parameter %u\n", p, p);
                    match = vx_false_e;
                }
            }
            if (match == vx_true_e)
            {
                node->child = graph;
                vxIncrementIntReference((vx_reference_t *)graph);
                VX_PRINT(VX_ZONE_GRAPH, "Graph "VX_FMT_REF" set as child graph of "VX_FMT_REF"\n", graph, node->graph);
                status = VX_SUCCESS;
            }
        }
        else
        {
            vxAddLogEntry(child, VX_ERROR_INVALID_GRAPH, "Number of parameters on graph does not equal number of parameters on node!");
        }
    }
    else if ((valid_node == vx_true_e) && (graph == NULL) && (node->child != NULL))
    {
        vxDecrementIntReference(&node->child->base);
        node->child = NULL;
    }
    return status;
}

vx_graph vxGetChildGraphOfNode(vx_node n)
{
    vx_node_t *node = (vx_node_t *)n;
    vx_graph_t *graph = 0;
    if (vxIsValidSpecificReference(&node->base, VX_TYPE_NODE) == vx_true_e)
    {
        graph = node->child;
    }
    return (vx_graph)graph;
}

// ![SAMPLE EXTENSION]
