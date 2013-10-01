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

static vx_uint32 vxNextNode(vx_graph_t *graph, vx_uint32 index)
{
    return ((index + 1) % graph->numNodes);
}

/*! \brief This function starts on the next node in the list and loops until we
 * hit the original node again. Parse over the nodes in circular fashion.
 */
static vx_status vxFindNodesWithReference(vx_graph_t *graph,
                                          vx_reference_t *ref,
                                          vx_uint32 refnodes[],
                                          vx_uint32 *count,
                                          vx_enum reftype)
{
    vx_uint32 n, p, nc = 0, max;
    vx_status status = VX_ERROR_INVALID_LINK;

    /* save the maximum number of nodes to find */
    max = *count;

    /* reset the current count to zero */
    *count = 0;

    VX_PRINT(VX_ZONE_GRAPH,"Find nodes with reference "VX_FMT_REF" type %d over %u nodes upto %u finds\n", ref, reftype, graph->numNodes, max);
    for (n = 0; n < graph->numNodes; n++)
    {
        for (p = 0; p < graph->nodes[n]->kernel->signature.numParams; p++)
        {
            vx_enum dir = graph->nodes[n]->kernel->signature.directions[p];
            vx_reference_t *thisref = graph->nodes[n]->parameters[p];

            /* VX_PRINT(VX_ZONE_GRAPH,"\tchecking node[%u].parameter[%u] dir = %d ref = "VX_FMT_REF" (=?%d:"VX_FMT_REF")\n", n, p, dir, thisref, reftype, ref); */
            if (((dir == reftype) || (dir == VX_BIDIRECTIONAL)) && (thisref == ref))
            {
                if (nc < max)
                {
                    VX_PRINT(VX_ZONE_GRAPH, "match at node[%u].parameter[%u]\n", n, p);
                    refnodes[nc] = n;
                    nc++;
                    status = VX_SUCCESS;
                }
                else
                {
                    VX_PRINT(VX_ZONE_ERROR, "ERROR: Overflow in refnodes[]\n");
                }
            }
        }
    }
    *count = nc;
    VX_PRINT(VX_ZONE_GRAPH, "Found %u nodes with reference "VX_FMT_REF" status = %d\n", nc, ref, status);
    return status;
}


void vxClearVisitation(vx_graph_t *graph)
{
    vx_uint32 n = 0;
    for (n = 0; n < graph->numNodes; n++)
        graph->nodes[n]->visited = vx_false_e;
}

void vxClearExecution(vx_graph_t *graph)
{
    vx_uint32 n = 0;
    for (n = 0; n < graph->numNodes; n++)
        graph->nodes[n]->executed = vx_false_e;
}

vx_status vxTraverseGraph(vx_graph_t *graph,
                          vx_uint32 parentIndex,
                          vx_uint32 childIndex)
{
    /* this is expensive, but needed in order to know who references a parameter */
    static vx_uint32 refNodes[VX_INT_MAX_REF];
    /* this keeps track of the available starting point in the static buffer */
    static vx_uint32 refStart = 0;
    /* this makes sure we don't have any odd conditions about infinite depth */
    static vx_uint32 depth = 0;

    vx_uint32 refCount = 0;
    vx_uint32 refIndex = 0;
    vx_uint32 thisIndex = 0;
    vx_status status = VX_SUCCESS;
    vx_uint32 p = 0;

    VX_PRINT(VX_ZONE_GRAPH, "refStart = %u\n", refStart);

    if (parentIndex == childIndex && parentIndex != VX_INT_MAX_NODES)
    {
        VX_PRINT(VX_ZONE_ERROR, "################################\n");
        VX_PRINT(VX_ZONE_ERROR, "ERROR: CYCLE DETECTED! node[%u]\n", parentIndex);
        VX_PRINT(VX_ZONE_ERROR, "################################\n");
        /* there's a cycle in the graph */
        status = VX_ERROR_INVALID_GRAPH;
    }
    else if (depth > graph->numNodes) /* should be impossible under normal circumstances */
    {
        /* there's a cycle in the graph */
        status = VX_ERROR_INVALID_GRAPH;
    }
    else
    {
        /* if the parent is an invalid index, then we assume we're processing a
         * head of a graph which has no parent index.
         */
        if (parentIndex == VX_INT_MAX_NODES)
        {
            parentIndex = childIndex;
            thisIndex = parentIndex;
            VX_PRINT(VX_ZONE_GRAPH, "Starting head-first traverse of graph from node[%u]\n", thisIndex);
        }
        else
        {
            thisIndex = childIndex;
            VX_PRINT(VX_ZONE_GRAPH, "continuing traverse of graph from node[%u] on node[%u] start=%u\n", parentIndex, thisIndex, refStart);
        }

        for (p = 0; p < graph->nodes[thisIndex]->kernel->signature.numParams; p++)
        {
            vx_enum dir = graph->nodes[thisIndex]->kernel->signature.directions[p];
            vx_reference_t *ref = graph->nodes[thisIndex]->parameters[p];

            if (dir != VX_INPUT && ref != NULL)
            {
                VX_PRINT(VX_ZONE_GRAPH, "[traverse] node[%u].parameter[%u] = "VX_FMT_REF"\n", thisIndex, p, ref);
                /* send the maximum number of possible nodes to find */
                refCount = dimof(refNodes) - refStart;
                status = vxFindNodesWithReference(graph, ref, &refNodes[refStart], &refCount, VX_INPUT);
                VX_PRINT(VX_ZONE_GRAPH, "status = %d at node[%u] start=%u count=%u\n", status, thisIndex, refStart, refCount);
                if (status == VX_SUCCESS)
                {
                    vx_uint32 refStop = refStart + refCount;
                    VX_PRINT(VX_ZONE_GRAPH, "Looping from %u to %u\n", refStart, refStop);
                    for (refIndex = refStart; refIndex < refStop; refIndex++)
                    {
                        VX_PRINT(VX_ZONE_GRAPH, "node[%u] => node[%u]\n", parentIndex, refNodes[refIndex]);
                        refStart += refCount;
                        depth++; /* go one more level in */
                        status = vxTraverseGraph(graph, parentIndex, refNodes[refIndex]);
                        depth--; /* pull out one level */
                        refStart -= refCount;
                        VX_PRINT(VX_ZONE_GRAPH, "status = %d at node[%u]\n", status, thisIndex);
                    }
                }
                if (status == VX_ERROR_INVALID_LINK) /* no links at all */
                {
                    VX_PRINT(VX_ZONE_GRAPH, "[Ok] No link found for node[%u].parameter[%u]\n", thisIndex, p);
                    status = VX_SUCCESS;
                }
            }
            else
            {
                VX_PRINT(VX_ZONE_GRAPH, "[ ignore ] node[%u].parameter[%u] = "VX_FMT_REF" type %d\n", childIndex, p, ref, dir);
            }
            if (status == VX_ERROR_INVALID_GRAPH)
                break;
        }

        if (status == VX_SUCCESS)
        {
            /* mark it visited for the next check to pass */
            graph->nodes[thisIndex]->visited = vx_true_e;
        }
    }
    VX_PRINT(VX_ZONE_GRAPH, "returning status %d\n", status);
    return status;
}

void vxFindNextNodes(vx_graph_t *graph,
                     vx_uint32 last_nodes[VX_INT_MAX_REF], vx_uint32 numLast,
                     vx_uint32 next_nodes[VX_INT_MAX_REF], vx_uint32 *numNext,
                     vx_uint32 left_nodes[VX_INT_MAX_REF], vx_uint32 *numLeft)
{
    vx_uint32 poss_next[VX_INT_MAX_REF];
    vx_uint32 i,n,p,n1,numPoss = 0;

    VX_PRINT(VX_ZONE_GRAPH, "Entering with %u left nodes\n", *numLeft);
    for (n = 0; n < *numLeft; n++)
    {
        VX_PRINT(VX_ZONE_GRAPH, "leftover: node[%u] = %s\n", left_nodes[n], graph->nodes[left_nodes[n]]->kernel->name);
    }

    numPoss = 0;
    *numNext = 0;

    /* for each last node, add all output to input nodes to the list of possible. */
    for (i = 0; i < numLast; i++)
    {
        n = last_nodes[i];
        for (p = 0; p < graph->nodes[n]->kernel->signature.numParams; p++)
        {
            vx_enum dir = graph->nodes[n]->kernel->signature.directions[p];
            vx_reference_t *ref =  graph->nodes[n]->parameters[p];
            if (((dir == VX_OUTPUT) || (dir == VX_BIDIRECTIONAL)) && (ref != NULL))
            {
                /* send the max possible nodes */
                n1 = dimof(poss_next) - numPoss;
                if (vxFindNodesWithReference(graph, ref, &poss_next[numPoss], &n1, VX_INPUT) == VX_SUCCESS)
                {
                    VX_PRINT(VX_ZONE_GRAPH, "Adding %u nodes to possible list\n", n1);
                    numPoss += n1;
                }
            }
        }
    }

    VX_PRINT(VX_ZONE_GRAPH, "There are %u possible nodes\n", numPoss);

    /* add back all the left over nodes (making sure to not include duplicates) */
    for (i = 0; i < *numLeft; i++)
    {
        vx_uint32 j;
        vx_bool match = vx_false_e;
        for (j = 0; j < numPoss; j++)
        {
            if (left_nodes[i] == poss_next[j])
            {
                match = vx_true_e;
            }
        }
        if (match == vx_false_e)
        {
            VX_PRINT(VX_ZONE_GRAPH, "Adding back left over node[%u] %s\n", left_nodes[i], graph->nodes[left_nodes[i]]->kernel->name);
            poss_next[numPoss++] = left_nodes[i];
        }
    }
    *numLeft = 0;

    /* now check all possible next nodes to see if the parent nodes are visited. */
    for (i = 0; i < numPoss; i++)
    {
        vx_uint32 poss_params[VX_INT_MAX_PARAMS];
        vx_uint32 pi, numPossParam = 0;
        vx_bool ready = vx_true_e;

        n = poss_next[i];
        VX_PRINT(VX_ZONE_GRAPH, "possible: node[%u] = %s\n", n, graph->nodes[n]->kernel->name);
        for (p = 0; p < graph->nodes[n]->kernel->signature.numParams; p++)
        {
            if (graph->nodes[n]->kernel->signature.directions[p] == VX_INPUT)
            {
                VX_PRINT(VX_ZONE_GRAPH,"nodes[%u].parameter[%u] predicate needs to be checked\n", n, p);
                poss_params[numPossParam] = p;
                numPossParam++;
            }
        }

        /* now check to make sure all possible input parameters have their */
        /* parent nodes executed. */
        for (pi = 0; pi < numPossParam; pi++)
        {
            vx_uint32 predicate_nodes[VX_INT_MAX_REF];
            vx_uint32 predicate_count = 0;
            vx_uint32 predicate_index = 0;
            vx_reference_t *ref = 0;

            p = poss_params[pi];
            ref = graph->nodes[n]->parameters[p];
            VX_PRINT(VX_ZONE_GRAPH, "checking node[%u].parameter[%u] = "VX_FMT_REF"\n", n, p, ref);
            /* set the size of predicate nodes going in */
            predicate_count = dimof(predicate_nodes);
            if (vxFindNodesWithReference(graph, ref, predicate_nodes, &predicate_count, VX_OUTPUT) == VX_SUCCESS)
            {
                /* check to see of all of the predicate nodes are executed */
                for (predicate_index = 0;
                     predicate_index < predicate_count;
                     predicate_index++)
                {
                    n1 = predicate_nodes[predicate_index];
                    if (graph->nodes[n1]->executed == vx_false_e)
                    {
                        VX_PRINT(VX_ZONE_GRAPH, "predicated: node[%u] = %s\n", n1, graph->nodes[n1]->kernel->name);
                        ready = vx_false_e;
                        break;
                    }
                }
            }
        }
        if (ready == vx_true_e)
        {
            /* make sure we don't schedule this node twice */
            if (graph->nodes[n]->visited == vx_false_e)
            {
                next_nodes[(*numNext)++] = n;
                graph->nodes[n]->visited = vx_true_e;
            }
        }
        else
        {
            /* put the node back into the possible list for next time */
            left_nodes[(*numLeft)++] = n;
            VX_PRINT(VX_ZONE_GRAPH, "notready: node[%u] = %s\n", n, graph->nodes[n]->kernel->name);
        }
    }

    VX_PRINT(VX_ZONE_GRAPH, "%u Next Nodes\n", *numNext);
    for (i = 0; i < *numNext; i++)
    {
        n = next_nodes[i];
        VX_PRINT(VX_ZONE_GRAPH, "next: node[%u] = %s\n", n, graph->nodes[n]->kernel->name);
    }
    VX_PRINT(VX_ZONE_GRAPH, "%u Left Nodes\n", *numLeft);
    for (i = 0; i < *numLeft; i++)
    {
        n = left_nodes[i];
        VX_PRINT(VX_ZONE_GRAPH, "left: node[%u] = %s\n", n, graph->nodes[n]->kernel->name);
    }
}

void vxContaminateGraphs(vx_reference_t *ref)
{
    if (vxIsValidReference(ref) == vx_true_e)
    {
        vx_uint32 r;
        vx_context_t *context = ref->context;
        /*! \internal Scan the entire context for graphs which may contain
         * this reference and mark them as unverified.
         */
        for (r = 0u; r < context->numRefs; r++)
        {
            if (context->reftable[r] == NULL)
                continue;
            if (context->reftable[r]->type == VX_TYPE_GRAPH)
            {
                vx_uint32 n;
                vx_bool found = vx_false_e;
                vx_graph_t *graph = (vx_graph_t *)context->reftable[r];
                for (n = 0u; n < (graph->numNodes) && (found == vx_false_e); n++)
                {
                    vx_uint32 p;
                    for (p = 0u; p < graph->nodes[n]->kernel->signature.numParams; p++)
                    {
                        if (graph->nodes[n]->kernel->signature.directions[p] == VX_OUTPUT)
                        {
                            continue;
                        }
                        if (graph->nodes[n]->parameters[p] == ref)
                        {
                            found = vx_true_e;
                            graph->verified = vx_false_e;
                            break;
                        }
                    }
                }
            }
        }
    }
}

/******************************************************************************/
/* PUBLIC FUNCTIONS */
/******************************************************************************/

vx_graph vxCreateGraph(vx_context c)
{
    vx_graph_t * graph = NULL;
    vx_context_t *context = (vx_context_t *)c;

    if (vxIsValidContext(context) == vx_false_e)
    {
        VX_PRINT(VX_ZONE_ERROR,"Context in %s is invalid!\n", __FUNCTION__);
    }
    else
    {
        graph = VX_CALLOC(vx_graph_t);
        if (graph)
        {
            vxInitReference((vx_reference_t *)graph, context, VX_TYPE_GRAPH);
            vxIncrementReference(&graph->base);
            vxAddReference(context, (vx_reference_t *)graph);
            vxInitPerf(&graph->perf);
            vxCreateSem(&graph->lock, 1);

            VX_PRINT(VX_ZONE_GRAPH,"Created Graph %p\n", graph);
            vxPrintReference((vx_reference_t *)graph);
        }
    }
    return (vx_graph)graph;
}

vx_status vxQueryGraph(vx_graph g, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_graph_t *graph = (vx_graph_t *)g;
    if (vxIsValidReference(&graph->base) == vx_true_e)
    {

        VX_PRINT(VX_ZONE_GRAPH,"INFO: Query:0x%x:%d\n", attribute, (attribute & VX_ATTRIBUTE_ID_MASK));

        switch (attribute)
        {
            case VX_GRAPH_ATTRIBUTE_PERFORMANCE:
                if (VX_CHECK_PARAM(ptr, size, vx_perf_t, 0x3))
                {
                    memcpy(ptr, &graph->perf, size);
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_GRAPH_ATTRIBUTE_STATUS:
                if (VX_CHECK_PARAM(ptr, size, vx_status, 0x3))
                {
                    *(vx_status *)ptr = graph->status;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_GRAPH_ATTRIBUTE_NUMNODES:
                if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
                {
                    *(vx_uint32 *)ptr = graph->numNodes;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_GRAPH_ATTRIBUTE_NUMPARAMETERS:
                if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
                {
                    *(vx_uint32 *)ptr = graph->numParams;
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

void vxReleaseGraph(vx_graph *g)
{
    vx_graph_t * graph= (vx_graph_t *)(g?*g:0);
    vxReleaseGraphInt(graph, vx_false_e);
    if (g) *g = 0;
}

/* \todo Add graph destruct */

void vxReleaseGraphInt(vx_graph_t *graph, vx_bool internal)
{
    if (vxIsValidReference(&graph->base) == vx_true_e)
    {
        if (internal == vx_false_e)
            vxDecrementReference(&graph->base);
        else
            vxDecrementIntReference(&graph->base);
        if (vxTotalReferenceCount(&graph->base) == 0)
        {
            vx_uint32 n;
            for (n = 0; n < graph->numNodes; n++)
            {
                vxReleaseNodeInt(graph->nodes[n], vx_true_e);
            }
            vxRemoveReference(graph->base.context, (vx_reference_t *)graph);
            vxDestroySem(&graph->lock);
            free(graph);
        }
    }
}

vx_status vxVerifyGraph(vx_graph g)
{
    vx_status status = VX_SUCCESS;
    vx_graph_t *graph = (vx_graph_t *)g;

    if (vxIsValidReference(&graph->base) == vx_true_e)
    {
        vx_uint32 h,n,p;
        vx_bool hasACycle = vx_false_e;

        /* lock the graph */
        vxSemWait(&graph->base.lock);

        VX_PRINT(VX_ZONE_GRAPH,"###########################\n");
        VX_PRINT(VX_ZONE_GRAPH,"Parameter Validation Phase!\n");
        VX_PRINT(VX_ZONE_GRAPH,"###########################\n");

        for (n = 0; n <graph->numNodes; n++)
        {
            /* check to make sure that a node has all required parameters */
            for (p = 0; p < graph->nodes[n]->kernel->signature.numParams; p++)
            {
                if (graph->nodes[n]->kernel->signature.states[p] == VX_PARAMETER_STATE_REQUIRED &&
                    graph->nodes[n]->parameters[p] == NULL)
                {
                    vxAddLogEntry(g, VX_ERROR_INVALID_PARAMETERS, "Node %s: Some parameters were not supplied!\n", graph->nodes[n]->kernel->name);
                    VX_PRINT(VX_ZONE_ERROR, "Node "VX_FMT_REF" (%s) Parameter[%u] was required and not supplied!\n",
                        graph->nodes[n],
                        graph->nodes[n]->kernel->name,p);
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
            }
            if (status != VX_SUCCESS)
            {
                goto exit;
            }

            /* debugging, show that we can detect "constant" data or "unreferenced data" */
            for (p = 0; p < graph->nodes[n]->kernel->signature.numParams; p++)
            {
                vx_reference_t *ref = (vx_reference_t *)graph->nodes[n]->parameters[p];
                if (ref)
                {
                    if (ref->external_count == 0)
                    {
                        VX_PRINT(VX_ZONE_INFO, "%s[%u] = "VX_FMT_REF" (CONSTANT) type:%08x\n", graph->nodes[n]->kernel->name, p, ref, ref->type);
                    }
                    else
                    {
                        VX_PRINT(VX_ZONE_INFO, "%s[%u] = "VX_FMT_REF" (MUTABLE) type:%08x count:%d\n", graph->nodes[n]->kernel->name, p, ref, ref->type, ref->external_count);
                    }
                }
            }

            /* first pass for inputs */
            for (p = 0; p < graph->nodes[n]->kernel->signature.numParams; p++)
            {
                if (((graph->nodes[n]->kernel->signature.directions[p] == VX_BIDIRECTIONAL) ||
                     (graph->nodes[n]->kernel->signature.directions[p] == VX_INPUT)) &&
                    (graph->nodes[n]->parameters[p] != NULL))
                {
                    status = graph->nodes[n]->kernel->validate_input((vx_node)graph->nodes[n], p);
                    if (status != VX_SUCCESS)
                    {
                        vxAddLogEntry(g, status, "Node[%u] %s: parameter[%u] failed input/bi validation!\n",
                            n, graph->nodes[n]->kernel->name,
                            p);
                        VX_PRINT(VX_ZONE_GRAPH,"Failed on validation of parameter %u of kernel %s in node #%d (status=%d)\n",
                                 p, graph->nodes[n]->kernel->name, n, status);
                    }
                }
            }
            /* second pass for bi/output (we may encounter "virtual" objects here,
             * then we must reparse graph to replace with new objects)
             */
            /*! \bug Bidirectional parameters currently break parsing. */
            for (p = 0; p < graph->nodes[n]->kernel->signature.numParams; p++)
            {
                if (graph->nodes[n]->parameters[p] == NULL)
                    continue;

                VX_PRINT(VX_ZONE_GRAPH,"Checking Node[%u].Parameter[%u]\n", n, p);
                if (graph->nodes[n]->kernel->signature.directions[p] == VX_OUTPUT)
                {
                    vx_meta_format_t meta;
                    vx_reference_t *vref = NULL;

                    if (graph->nodes[n]->parameters[p]->type == VX_TYPE_IMAGE)
                    {
                        vx_image_t *img = (vx_image_t *)graph->nodes[n]->parameters[p];
                        if ((img->width == 0 && img->height == 0) || (img->format == FOURCC_VIRT))
                        {
                            VX_PRINT(VX_ZONE_GRAPH,"Virtual Image Detected at kernel %s parameter %u\n", graph->nodes[n]->kernel->name, p);
                            vref = graph->nodes[n]->parameters[p];
                        }
                    }
                    else if (graph->nodes[n]->parameters[p]->type == VX_TYPE_BUFFER)
                    {
                        vx_buffer_t *buf = (vx_buffer_t *)graph->nodes[n]->parameters[p];
                        if (buf->unitSize == 0 && buf->numUnits == 0)
                        {
                            VX_PRINT(VX_ZONE_GRAPH,"Virtual Buffer Detected at kernel %s parameter %u\n", graph->nodes[n]->kernel->name, p);
                            vref = graph->nodes[n]->parameters[p];
                        }
                    }
                    /*! \todo virtual pyramids? */
                    /*! \todo virtual lists? */
                    /*! \todo virtual other objects? */
                    memset(&meta, 0, sizeof(meta));
                    // this is known by the system, so let the system set it by
                    // default.
                    meta.type = graph->nodes[n]->kernel->signature.types[p];
                    status = graph->nodes[n]->kernel->validate_output((vx_node)graph->nodes[n], p, &meta);
                    if (status == VX_SUCCESS)
                    {
                        if (vxIsValidType(meta.type) == vx_false_e)
                        {
                            status = VX_ERROR_INVALID_TYPE;
                            vxAddLogEntry(g, status,
                                "Node: %s: parameter[%u] is not a valid type %d!\n",
                                graph->nodes[n]->kernel->name, p, meta.type);
                        }
                        if (meta.type == VX_TYPE_IMAGE)
                        {
                            vx_image_t *img = (vx_image_t *)graph->nodes[n]->parameters[p];
                            VX_PRINT(VX_ZONE_GRAPH,"meta: type 0x%08x, %ux%u\n", meta.type, meta.dim.image.width, meta.dim.image.height);
                            if (vref == (vx_reference_t *)img)
                            {
                                VX_PRINT(VX_ZONE_GRAPH,"Creating Image From Meta Data!\n");
                                img->width = meta.dim.image.width;
                                img->height = meta.dim.image.height;
                                if (img->format == FOURCC_VIRT)
                                    img->format = meta.dim.image.format;
                                /* we have to go set all the other dimensional information up. */
                                vxInitImage(img, img->width, img->height, img->format);
                                vxPrintImage(img); /* show that it's been created. */
                            }
                            else
                            {
                                /* check the data that came back from the output validator against the object */
                                if ((img->width != meta.dim.image.width) ||
                                    (img->height != meta.dim.image.height))
                                {
                                    status = VX_ERROR_INVALID_DIMENSION;
                                    vxAddLogEntry(g, status,
                                        "Node: %s: parameter[%u] is an invalid dimension %ux%u!\n",
                                        graph->nodes[n]->kernel->name, p, img->width, img->height);
                                }
                                if (img->format != meta.dim.image.format)
                                {
                                    status = VX_ERROR_INVALID_FORMAT;
                                    vxAddLogEntry(g, status,
                                        "Node: %s: parameter[%u] is an invalid format %08x!\n",
                                        graph->nodes[n]->kernel->name, p, img->format);
                                }
                            }
                        }
                        else if (meta.type == VX_TYPE_BUFFER)
                        {
                            vx_buffer_t *buf = (vx_buffer_t *)graph->nodes[n]->parameters[p];
                            VX_PRINT(VX_ZONE_GRAPH,"meta: type 0x%08x, "VX_FMT_SIZE" "VX_FMT_SIZE"\n", meta.type, meta.dim.buffer.unitSize, meta.dim.buffer.numUnits);
                            if (vref == (vx_reference_t *)buf)
                            {
                                VX_PRINT(VX_ZONE_GRAPH,"Creating Buffer From Meta Data!\n");
                                buf->numUnits = meta.dim.buffer.numUnits;
                                buf->unitSize = meta.dim.buffer.unitSize;
                            }
                            else
                            {
                                if ((buf->numUnits < meta.dim.buffer.numUnits) ||
                                    (buf->unitSize != meta.dim.buffer.unitSize))
                                {
                                    status = VX_ERROR_INVALID_DIMENSION;
                                    vxAddLogEntry(g, VX_ERROR_INVALID_DIMENSION,
                                        "Node: %s: parameter[%u] is an invalid number of units "VX_FMT_SIZE" or unit size "VX_FMT_SIZE"\n",
                                        graph->nodes[n]->kernel->name, p, buf->numUnits, buf->unitSize);
                                }
                            }
                        }
                        else if (meta.type == VX_TYPE_PYRAMID)
                        {
                            vx_pyramid_t *pyr = NULL;
                            vx_image_t *pyr0 = NULL;
                            VX_PRINT(VX_ZONE_GRAPH,"meta: type 0x%08x, %ux%u\n", meta.type, meta.dim.pyramid.width, meta.dim.pyramid.height);
                            VX_PRINT(VX_ZONE_GRAPH,"Nodes[%u] %s parameters[%u]\n", n, graph->nodes[n]->kernel->name, p);
                            pyr = (vx_pyramid_t *)graph->nodes[n]->parameters[p];
                            pyr0 = (vx_image_t *)pyr->levels[0];
                            /*! \todo attach output pyramids to allocators */
                        }
                        else if (meta.type == VX_TYPE_LIST)
                        {
                            vx_list_t *list = (vx_list_t *)graph->nodes[n]->parameters[p];
                            VX_PRINT(VX_ZONE_GRAPH,"meta: type 0x%08x, "VX_FMT_SIZE" "VX_FMT_SIZE"\n", meta.type, meta.dim.list.type);
                            if (list->type != meta.dim.list.type)
                            {
                                status = VX_ERROR_INVALID_TYPE;
                                vxAddLogEntry(g, VX_ERROR_INVALID_TYPE,
                                        "List contains invalid typed objects for node %s\n", graph->nodes[n]->kernel->name);
                            }
                        }
                        else if (meta.type == VX_TYPE_SCALAR)
                        {
                            vx_scalar_t *scalar = (vx_scalar_t *)graph->nodes[n]->parameters[p];
                            if (scalar->type != meta.dim.scalar.type)
                            {
                                status = VX_ERROR_INVALID_TYPE;
                                vxAddLogEntry(g, VX_ERROR_INVALID_TYPE,
                                      "Scalar contains invalid typed objects for node %s\n", graph->nodes[n]->kernel->name);
                            }
                        }
                        /*! \todo support other output types for safety checks in graph verification parameters phase */
                        else
                        {
                            VX_PRINT(VX_ZONE_GRAPH,"Returned Meta type %d\n", meta.type);
                        }
                    }
                    else
                    {
                        vxAddLogEntry(g, status, "Node %s: parameter[%u] failed output validation! (status = %d)\n",
                            graph->nodes[n]->kernel->name, p, status);
                        VX_PRINT(VX_ZONE_ERROR,"Failed on validation of output parameter[%u] on kernel %s, status=%d\n",
                                p,
                                graph->nodes[n]->kernel->name,
                                status);
                    }
                }
            }
        }

        VX_PRINT(VX_ZONE_GRAPH,"####################\n");
        VX_PRINT(VX_ZONE_GRAPH,"Single Writer Phase!\n");
        VX_PRINT(VX_ZONE_GRAPH,"####################\n");

        for (n = 0; n < graph->numNodes && status == VX_SUCCESS; n++)
        {
            for (p = 0; p < graph->nodes[n]->kernel->signature.numParams; p++)
            {
                if (graph->nodes[n]->parameters[p] &&
                    ((graph->nodes[n]->kernel->signature.directions[p] == VX_OUTPUT) ||
                     (graph->nodes[n]->kernel->signature.directions[p] == VX_BIDIRECTIONAL)))
                {
                    vx_uint32 n1, p1;
                    /* check for other output references to this parameter in the graph. */
                    for (n1 = vxNextNode(graph, n); n1 != n; n1=vxNextNode(graph, n1))
                    {
                        for (p1 = 0; p1 < graph->nodes[n]->kernel->signature.numParams; p1++)
                        {
                            if (graph->nodes[n]->parameters[p] == graph->nodes[n1]->parameters[p1] &&
                                ((graph->nodes[n1]->kernel->signature.directions[p1] == VX_OUTPUT) ||
                                 (graph->nodes[n1]->kernel->signature.directions[p1] == VX_BIDIRECTIONAL)))
                            {
                                status = VX_ERROR_MULTIPLE_WRITERS;
                                VX_PRINT(VX_ZONE_GRAPH, "Multiple Writer to a reference found, check log!\n");
                                vxAddLogEntry(g, status, "Node %u and Node %u are trying to output to the same reference "VX_FMT_REF"\n", n, n1, graph->nodes[n]->parameters[p]);
                            }
                        }
                    }
                }
            }
        }

        VX_PRINT(VX_ZONE_GRAPH,"########################\n");
        VX_PRINT(VX_ZONE_GRAPH,"Memory Allocation Phase!\n");
        VX_PRINT(VX_ZONE_GRAPH,"########################\n");

        /* now make sure each parameter is backed by memory. */
        for (n = 0; n < graph->numNodes && status == VX_SUCCESS; n++)
        {
            VX_PRINT(VX_ZONE_GRAPH,"Checking node %u\n",n);

            for (p = 0; p < graph->nodes[n]->kernel->signature.numParams; p++)
            {
                if (graph->nodes[n]->parameters[p])
                {
                    VX_PRINT(VX_ZONE_GRAPH,"\tparameter[%u]=%p type %d sig type %d\n", p,
                                 graph->nodes[n]->parameters[p],
                                 graph->nodes[n]->parameters[p]->type,
                                 graph->nodes[n]->kernel->signature.types[p]);

                    if (vxIsSupportedFourcc(graph->nodes[n]->kernel->signature.types[p]) == vx_true_e)
                    {
                        if (vxAllocateImage((vx_image_t *)graph->nodes[n]->parameters[p]) == vx_false_e)
                        {
                            vxAddLogEntry(g, VX_ERROR_NO_MEMORY, "Failed to allocate image at node[%u] %s parameter[%u]\n",
                                n, graph->nodes[n]->kernel->name, p);
                            VX_PRINT(VX_ZONE_ERROR, "See log\n");
                        }
                    }
                    else if (graph->nodes[n]->kernel->signature.types[p] == VX_TYPE_BUFFER)
                    {
                        if (vxAllocateBuffer((vx_buffer_t*)graph->nodes[n]->parameters[p]) == vx_false_e)
                        {
                            vxAddLogEntry(g, VX_ERROR_NO_MEMORY, "Failed to allocate buffer at node[%u] %s parameter[%u]\n",
                                n, graph->nodes[n]->kernel->name, p);
                            VX_PRINT(VX_ZONE_ERROR, "See log\n");
                        }
                    }
                    else if ((VX_TYPE_IS_SCALAR(graph->nodes[n]->parameters[p]->type)) ||
                             (graph->nodes[n]->parameters[p]->type == VX_TYPE_RECTANGLE) ||
                             (graph->nodes[n]->parameters[p]->type == VX_TYPE_THRESHOLD))
                    {
                        /* these objects don't need to be allocated */
                    }
                    else if (graph->nodes[n]->parameters[p]->type == VX_TYPE_LUT)
                    {
                        vx_lut_t *lut = (vx_lut_t *)graph->nodes[n]->parameters[p];
                        if (vxAllocateMemory(graph->base.context, &lut->memory) == vx_false_e)
                        {
                            vxAddLogEntry(g, VX_ERROR_NO_MEMORY, "Failed to allocate lut at node[%u] %s parameter[%u]\n",
                                n, graph->nodes[n]->kernel->name, p);
                            VX_PRINT(VX_ZONE_ERROR, "See log\n");
                        }
                    }
                    else if (graph->nodes[n]->parameters[p]->type == VX_TYPE_DISTRIBUTION)
                    {
                        vx_distribution_t *dist = (vx_distribution_t *)graph->nodes[n]->parameters[p];
                        if (vxAllocateMemory(graph->base.context, &dist->memory) == vx_false_e)
                        {
                            vxAddLogEntry(g, VX_ERROR_NO_MEMORY, "Failed to allocate distribution at node[%u] %s parameter[%u]\n",
                                n, graph->nodes[n]->kernel->name, p);
                            VX_PRINT(VX_ZONE_ERROR, "See log\n");
                        }
                    }
                    else if (graph->nodes[n]->parameters[p]->type == VX_TYPE_PYRAMID)
                    {
                        vx_pyramid_t *pyr = (vx_pyramid_t *)graph->nodes[n]->parameters[p];
                        vx_uint32 i = 0;
                        for (i = 0; i < pyr->numLevels; i++)
                        {
                            if (vxAllocateImage((vx_image_t *)pyr->levels[i]) == vx_false_e)
                            {
                                vxAddLogEntry(g, VX_ERROR_NO_MEMORY, "Failed to allocate pyramid image at node[%u] %s parameter[%u]\n",
                                    n, graph->nodes[n]->kernel->name, p);
                                VX_PRINT(VX_ZONE_ERROR, "See log\n");
                            }
                        }
                    }
                    else if ((graph->nodes[n]->parameters[p]->type == VX_TYPE_MATRIX) ||
                             (graph->nodes[n]->parameters[p]->type == VX_TYPE_COORDINATES) ||
                             (graph->nodes[n]->parameters[p]->type == VX_TYPE_CONVOLUTION))
                    {
                        vx_matrix_t *mat = (vx_matrix_t *)graph->nodes[n]->parameters[p];
                        if (vxAllocateMemory(graph->base.context, &mat->memory) == vx_false_e)
                        {
                            vxAddLogEntry(g, VX_ERROR_NO_MEMORY, "Failed to allocate matrix (or subtype) at node[%u] %s parameter[%u]\n",
                                n, graph->nodes[n]->kernel->name, p);
                            VX_PRINT(VX_ZONE_ERROR, "See log\n");
                        }
                    }
                    /*! \todo add other memory objects to graph auto-allocator as needed! */
                }
            }
        }

        VX_PRINT(VX_ZONE_GRAPH,"################################\n");
        VX_PRINT(VX_ZONE_GRAPH,"Head Nodes Determination Phase! \n");
        VX_PRINT(VX_ZONE_GRAPH,"################################\n");

        memset(graph->heads, 0, sizeof(graph->heads));
        graph->numHeads = 0;

        /* now traverse the graph and put nodes with no predecessor in the head list */
        for (n = 0; n < graph->numNodes && status == VX_SUCCESS; n++)
        {
            uint32_t n1,p1;
            vx_bool isAHead = vx_true_e; /* assume every node is a head until proven otherwise */

            for (p = 0; p < graph->nodes[n]->kernel->signature.numParams && isAHead == vx_true_e; p++)
            {
                if (graph->nodes[n]->kernel->signature.directions[p] == VX_INPUT &&
                    graph->nodes[n]->parameters[p] != NULL)
                {
                    /* ring loop over the node array, checking every node but this nth node. */
                    for (n1 = vxNextNode(graph, n); n1 != n && isAHead == vx_true_e; n1 = vxNextNode(graph, n1))
                    {
                        for (p1 = 0; p1 < graph->nodes[n1]->kernel->signature.numParams && isAHead == vx_true_e; p1++)
                        {
                            if (graph->nodes[n1]->kernel->signature.directions[p1] != VX_INPUT)
                            {
                                VX_PRINT(VX_ZONE_GRAPH,"Checking input nodes[%u].parameter[%u] to nodes[%u].parameters[%u]\n", n, p, n1, p1);
                                /* if the parameter is referenced elsewhere */
                                if (graph->nodes[n1]->parameters[p1] == graph->nodes[n]->parameters[p])
                                {
                                    VX_PRINT(VX_ZONE_GRAPH,"\tnodes[%u].parameter[%u] referenced in nodes[%u].parameter[%u]\n", n,p,n1,p1);
                                    isAHead = vx_false_e; /* this will cause all the loops to break too. */
                                }
                            }
                        }
                    }
                }
            }

            if (isAHead == vx_true_e)
            {
                VX_PRINT(VX_ZONE_GRAPH,"Found a head in node[%u] => %s\n", n, graph->nodes[n]->kernel->name);
                graph->heads[graph->numHeads++] = n;
            }
        }

        /* graph has a cycle as there are no starting points! */
        if (graph->numHeads == 0 && status == VX_SUCCESS)
        {
            status = VX_ERROR_INVALID_GRAPH;
            VX_PRINT(VX_ZONE_ERROR,"Graph has no heads!\n");
            vxAddLogEntry(g, status, "Cycle: Graph has no head nodes!\n");
        }

        VX_PRINT(VX_ZONE_GRAPH,"##############\n");
        VX_PRINT(VX_ZONE_GRAPH,"Cycle Checking\n");
        VX_PRINT(VX_ZONE_GRAPH,"##############\n");

        vxClearVisitation(graph);

        /* cycle checking by traversal of the graph from heads to tails */
        for (h = 0; h < graph->numHeads; h++)
        {
            status = vxTraverseGraph(graph, VX_INT_MAX_NODES, graph->heads[h]);
            if (status != VX_SUCCESS)
            {
                VX_PRINT(VX_ZONE_ERROR,"Cycle found in graph!");
                vxAddLogEntry(g, status, "Cycle: Graph has a cycle!\n");
                goto exit;
            }
        }

        VX_PRINT(VX_ZONE_GRAPH,"############################\n");
        VX_PRINT(VX_ZONE_GRAPH,"Checking for Unvisited Nodes\n");
        VX_PRINT(VX_ZONE_GRAPH,"############################\n");

        for (n = 0; n < graph->numNodes && status == VX_SUCCESS; n++)
        {
            if (graph->nodes[n]->visited == vx_false_e)
            {
                VX_PRINT(VX_ZONE_ERROR, "UNVISITED: %s node[%u]\n", graph->nodes[n]->kernel->name, n);
                status = VX_ERROR_INVALID_GRAPH;
                vxAddLogEntry(g, status, "Node %s: unvisited!\n", graph->nodes[n]->kernel->name);
            }
        }

        vxClearVisitation(graph);

        if (hasACycle == vx_true_e)
        {
            status = VX_ERROR_INVALID_GRAPH;
            vxAddLogEntry(g, status, "Cycle: Graph has a cycle!\n");
            goto exit;
        }

        VX_PRINT(VX_ZONE_GRAPH,"#########################\n");
        VX_PRINT(VX_ZONE_GRAPH,"Target Verification Phase\n");
        VX_PRINT(VX_ZONE_GRAPH,"#########################\n");

        for (n = 0; (n < graph->numNodes) && (status == VX_SUCCESS); n++)
        {
            vx_uint32 index = graph->nodes[n]->affinity;
            vx_target_t *target = &graph->base.context->targets[index];
            if (target)
            {
                status = target->funcs.verify(target, graph->nodes[n]);
                if (status != VX_SUCCESS)
                {
                    vxAddLogEntry(g, status, "Target: %s Failed to Verify Node %s\n", target->name, graph->nodes[n]->kernel->name);
                }
            }
        }

        VX_PRINT(VX_ZONE_GRAPH,"#######################\n");
        VX_PRINT(VX_ZONE_GRAPH,"Kernel Initialize Phase\n");
        VX_PRINT(VX_ZONE_GRAPH,"#######################\n");

        for (n = 0; n < graph->numNodes && status == VX_SUCCESS; n++)
        {
            vx_node_t *node = graph->nodes[n];
            if (node->kernel->initialize)
            {
                /* call the kernel initialization routine */
                status = node->kernel->initialize((vx_node)node,
                                                  (vx_reference *)node->parameters,
                                                  node->kernel->signature.numParams);
                if (status != VX_SUCCESS)
                {
                    vxAddLogEntry(g, status, "Kernel: %s failed to initialize!\n", node->kernel->name);
                }
            }

            /* once the kernel has been initialized, create any local data for it */
            if ((node->attributes.localDataSize > 0) &&
                (node->attributes.localDataPtr == NULL))
            {
                node->attributes.localDataPtr = calloc(1, node->attributes.localDataSize);
                VX_PRINT(VX_ZONE_GRAPH, "Local Data Allocated "VX_FMT_SIZE" bytes for node into %p\n!",
                        node->attributes.localDataSize,
                        node->attributes.localDataPtr);
            }
#ifdef OPENVX_KHR_TILING
            /* if this is a tiling kernel, we can also have tile memory (the sample only makes 1 buffer) */
            if ((node->attributes.tileDataSize > 0) &&
                (node->attributes.tileDataPtr == NULL))
            {
                node->attributes.tileDataPtr = calloc(1, node->attributes.tileDataSize);
            }
#endif
        }
exit:
        if (status == VX_SUCCESS)
        {
            graph->verified = vx_true_e;
        }
        else
        {
            graph->verified = vx_false_e;
        }

        vxPrintAllLogEntries(&graph->base.context->log, VX_ZONE_GRAPH);

        /* unlock the graph */
        vxSemPost(&graph->base.lock);
    }
    else
    {
        status = VX_ERROR_INVALID_REFERENCE;
    }
    VX_PRINT(VX_ZONE_GRAPH,"Returning status %d\n", status);

    return status;
}

static vx_status vxExecuteGraph(vx_graph g)
{
    vx_status status = VX_SUCCESS;
    vx_action action = VX_ACTION_CONTINUE;
    vx_graph_t *graph = (vx_graph_t *)g;
    vx_uint32 n, numLast, numNext, numLeft = 0;
    vx_uint32 last_nodes[VX_INT_MAX_REF];
    vx_uint32 next_nodes[VX_INT_MAX_REF];
    vx_uint32 left_nodes[VX_INT_MAX_REF];

    if (vxIsValidReference(&graph->base) == vx_false_e)
    {
        return VX_ERROR_INVALID_REFERENCE;
    }
    if (graph->verified == vx_false_e)
    {
        status = vxVerifyGraph((vx_graph)graph);
        if (status != VX_SUCCESS)
        {
            return status;
        }
    }
restart:
    VX_PRINT(VX_ZONE_GRAPH,"************************\n");
    VX_PRINT(VX_ZONE_GRAPH,"*** PROCESSING GRAPH ***\n");
    VX_PRINT(VX_ZONE_GRAPH,"************************\n");

    vxClearVisitation(graph);
    vxClearExecution(graph);

    /* initialize the next_nodes as the graph heads */
    memcpy(next_nodes, graph->heads, graph->numHeads * sizeof(vx_uint32));
    numNext = graph->numHeads;

    do {
        for (n = 0; n < numNext; n++)
        {
            vxPrintNode(graph->nodes[next_nodes[n]]);
        }

        /* execute the next nodes */
        for (n = 0; n < numNext; n++)
        {
            if (graph->nodes[next_nodes[n]]->executed == vx_false_e)
            {
                vx_uint32 t = graph->nodes[next_nodes[n]]->affinity;
                vx_target_t *target = &graph->base.context->targets[t];
                vx_node_t *node = graph->nodes[next_nodes[n]];
                VX_PRINT(VX_ZONE_TARGET, "Calling %s:%s\n", target->name, node->kernel->name);
                action = target->funcs.process(target, &node, 0, 1);
                if ((action == VX_ACTION_ABANDON) ||
                    (action == VX_ACTION_RESTART))
                {
                    VX_PRINT(VX_ZONE_WARNING, "Node[%u] %s:%s returned action %d\n",
                             next_nodes[n],
                             target->name, node->kernel->name,
                             action);
                    break;
                }
            }
            else
            {
                VX_PRINT(VX_ZONE_ERROR, "Multiple executions attempted!\n");
                break;
            }
        }

        if ((action == VX_ACTION_ABANDON) ||
            (action == VX_ACTION_RESTART))
        {
            break;
        }

        /* copy next_nodes to last_nodes */
        memcpy(last_nodes, next_nodes, numNext * sizeof(vx_uint32));
        numLast = numNext;

        /* determine the next nodes */
        vxFindNextNodes(graph, last_nodes, numLast, next_nodes, &numNext, left_nodes, &numLeft);

    } while (numNext > 0);

    if (action == VX_ACTION_RESTART)
    {
        goto restart;
    }
    if (action == VX_ACTION_ABANDON)
    {
        status = VX_ERROR_GRAPH_ABANDONED;
    }
    vxClearVisitation(graph);

    VX_PRINT(VX_ZONE_GRAPH,"Process returned status %d\n", status);
    for (n = 0; n < graph->numNodes; n++)
    {
        VX_PRINT(VX_ZONE_PERF,"nodes[%u] %s[%d] last:"VX_FMT_TIME" avg:"VX_FMT_TIME"\n",
                 n,
                 graph->nodes[n]->kernel->name,
                 graph->nodes[n]->kernel->enumeration,
                 graph->nodes[n]->perf.tmp,
                 graph->nodes[n]->perf.avg);
    }
    return status;
}

vx_status vxScheduleGraph(vx_graph g)
{
    vx_status status = VX_SUCCESS;
    vx_graph_t *graph = (vx_graph_t *)g;
    if (vxIsValidReference(&graph->base) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;

    if (vxSemTryWait(&graph->lock) == vx_true_e)
    {
        /* now add the graph to the queue */
        VX_PRINT(VX_ZONE_GRAPH,"Writing graph=" VX_FMT_REF ", status=%d\n",g, status);
        if (vxWriteQueue(&graph->base.context->proc.input,
                         (vx_value_t)g,
                         (vx_value_t)status) == vx_true_e)
            status = VX_SUCCESS;
        else
        {
            vxSemPost(&graph->lock);
            status = VX_ERROR_NO_RESOURCES;
        }
    }
    else
        status = VX_ERROR_GRAPH_SCHEDULED;
    return status;
}

vx_status vxWaitGraph(vx_graph g)
{
    vx_status status = VX_SUCCESS;
    vx_graph_t *graph = (vx_graph_t *)g;

    if (vxIsValidReference(&graph->base) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;

    if (vxSemTryWait(&graph->lock) == vx_false_e) // locked
    {
        vx_graph g2;
        vx_bool ret = vx_true_e;
        vx_value_t v1, v2;
        do
        {
            ret = vxReadQueue(&graph->base.context->proc.output, &v1, &v2);
            if (ret == vx_false_e)
            {
                /* graph was locked but the queue was empty... */
                VX_PRINT(VX_ZONE_ERROR, "Queue was empty but graph was locked.\n");
                status = VX_FAILURE;
            }
            else
            {
                g2 = (vx_graph)v1;
                status = (vx_status)v2;
                if (g2 == g) /* great, it's the graph we want. */
                {
                    vxSemPost(&graph->lock); /* unlock the graph. */
                    break;
                }
                else
                {
                    /* not the right graph, put it back. */
                    vxWriteQueue(&graph->base.context->proc.output, v1, v2);
                }
            }
        } while (ret == vx_true_e);
    }
    else
    {
        vxSemPost(&graph->lock); /* was free, release */
    }
    return status;
}

vx_status vxProcessGraph(vx_graph g)
{
    return vxExecuteGraph(g);
}

vx_status vxAddParameterToGraph(vx_graph g, vx_parameter p)
{
    vx_graph_t *graph = (vx_graph_t *)g;
    vx_parameter_t *param = (vx_parameter_t *)p;
    vx_status status = VX_ERROR_INVALID_REFERENCE;
    if ((vxIsValidSpecificReference(&graph->base, VX_TYPE_GRAPH) == vx_true_e) &&
        (vxIsValidSpecificReference(&param->base, VX_TYPE_PARAMETER) == vx_true_e))
    {
        graph->parameters[graph->numParams].node = param->node;
        graph->parameters[graph->numParams].index = param->index;
        graph->numParams++;
        status = VX_SUCCESS;
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Graph "VX_FMT_REF" or Parameter "VX_FMT_REF" was invalid!\n", g, p);
    }
    return status;
}

vx_status vxSetGraphParameterByIndex(vx_graph g, vx_uint32 index, vx_enum dir, vx_reference value)
{
    vx_graph_t *graph = (vx_graph_t *)g;
    vx_status status = VX_ERROR_INVALID_REFERENCE;
    if (vxIsValidSpecificReference(&graph->base, VX_TYPE_GRAPH) == vx_true_e)
    {
        if (index < VX_INT_MAX_PARAMS)
        {
            status = vxSetParameterByIndex((vx_node)graph->parameters[index].node,
                                           graph->parameters[index].index,
                                           dir,
                                           value);
        }
        else
        {
            status = VX_ERROR_INVALID_VALUE;
        }
    }
    return status;
}

vx_parameter vxGetGraphParameterByIndex(vx_graph g, vx_uint32 index)
{
    vx_parameter parameter = 0;
    vx_graph_t *graph = (vx_graph_t *)g;
    if (vxIsValidSpecificReference(&graph->base, VX_TYPE_GRAPH) == vx_true_e)
    {
        if ((index < VX_INT_MAX_PARAMS) && (index < graph->numParams))
        {
            vx_uint32 node_index = graph->parameters[index].index;
            parameter = vxGetParameterByIndex((vx_node)graph->parameters[index].node, node_index);
        }
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid Graph!\n");
        vxAddLogEntry(g, VX_ERROR_INVALID_REFERENCE, "Invalid Graph given to %s\n", __FUNCTION__);
    }
    return parameter;
}

vx_bool vxIsGraphVerified(vx_graph graph)
{
    vx_bool verified = vx_false_e;
    vx_graph_t *pgraph = (vx_graph_t *)graph;
    if (vxIsValidSpecificReference(&pgraph->base, VX_TYPE_GRAPH) == vx_true_e)
    {
        VX_PRINT(VX_ZONE_GRAPH, "Graph is %sverified\n", (pgraph->verified == vx_true_e?"":"NOT "));
        verified = pgraph->verified;
    }
    return verified;
}

