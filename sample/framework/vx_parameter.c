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

vx_bool vxIsValidDirection(vx_enum dir)
{
    if ((dir == VX_INPUT) || (dir == VX_OUTPUT) || (dir == VX_BIDIRECTIONAL))
    {
        return vx_true_e;
    }
    else
    {
        return vx_false_e;
    }
}

vx_bool vxIsValidTypeMatch(vx_enum expected, vx_enum supplied)
{
    vx_bool match = vx_false_e;
    if (expected == supplied)
    {
        match = vx_true_e;
    }
    if (match == vx_false_e)
    {
        VX_PRINT(VX_ZONE_ERROR, "Expected %08x and got %08x!\n", expected, supplied);
    }
    return match;
}

vx_bool vxIsValidState(vx_enum state)
{
    if ((state == VX_PARAMETER_STATE_REQUIRED) ||
        (state == VX_PARAMETER_STATE_OPTIONAL))
    {
        return vx_true_e;
    }
    else
    {
        return vx_false_e;
    }
}

/******************************************************************************/
/* PUBLIC API */
/******************************************************************************/

vx_parameter vxGetParameterByIndex(vx_node n, vx_uint32 index)
{
    vx_parameter_t *param = NULL;
    vx_node_t *node = (vx_node_t *)n;

    if (vxIsValidSpecificReference(&node->base, VX_TYPE_NODE) == vx_false_e)
    {
        return (vx_parameter)param;
    }
    if (/*0 <= index &&*/ index < VX_INT_MAX_PARAMS)
    {
        if (node->kernel == NULL)
        {
            vxAddLogEntry(n, VX_FAILURE, "Node was created without a kernel! Fatal Error!\n");
        }
        else
        {
            param = VX_CALLOC(vx_parameter_t);
            if (param)
            {
                vxInitReference((vx_reference_t *)param, node->base.context, VX_TYPE_PARAMETER);
                vxIncrementReference(&param->base);
                vxAddReference(node->base.context, (vx_reference_t *)param);
                param->index = index;
                param->node = node;
            }
            else
            {
                vxAddLogEntry(n, VX_ERROR_NO_RESOURCES, "Failed to allocate vx_parameter on node %s parameter[%u]!\n", node->kernel->name, index);
            }
        }
    }
    else
    {
        vxAddLogEntry(n, VX_ERROR_INVALID_PARAMETERS, "Index %u out of range for node %s (numparams = %u)!\n", index, node->kernel->name, node->kernel->signature.numParams);
    }

    VX_PRINT(VX_ZONE_API, "%s: returning %p\n", __FUNCTION__, param);
    return (vx_parameter)param;
}

void vxReleaseParameter(vx_parameter *param)
{
    vx_parameter_t *parameter = (vx_parameter_t *)(param?*param:0);
    if (param) *param = 0;
    if (vxIsValidSpecificReference(&parameter->base, VX_TYPE_PARAMETER) == vx_true_e)
    {
        vxDecrementReference(&parameter->base);
        if (vxTotalReferenceCount(&parameter->base) == 0)
        {
            vxRemoveReference(parameter->base.context, (vx_reference_t *)parameter);
            free(parameter);
        }
    }
}

vx_status vxSetParameterByIndex(vx_node n, vx_uint32 index, vx_enum direction, vx_reference value)
{
    vx_status status = VX_SUCCESS;
    vx_node_t *node = (vx_node_t *)n;
    vx_enum type = 0;

    if (vxIsValidSpecificReference(&node->base, VX_TYPE_NODE) == vx_false_e)
    {
        VX_PRINT(VX_ZONE_ERROR, "Supplied node was not actually a node\n");
        status = VX_ERROR_INVALID_REFERENCE;
        goto exit;
    }

    VX_PRINT(VX_ZONE_PARAMETER, "Attempting to set parameter[%u] on %s (enum:%d) to "VX_FMT_REF"\n",
                    index,
                    node->kernel->name,
                    node->kernel->enumeration,
                    value);

    /* is the index out of bounds? */
    if ((index >= node->kernel->signature.numParams) || (index >= VX_INT_MAX_PARAMS))
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid index %u\n", index);
        status = VX_ERROR_INVALID_VALUE;
        goto exit;
    }

    /* if it's an optional parameter, it's ok to be NULL */
    if ((value == 0) && (node->kernel->signature.states[index] == VX_PARAMETER_STATE_OPTIONAL))
    {
        status = VX_SUCCESS;
        goto exit;
    }

    /* if it's required, it's got to exist */
    if (vxIsValidReference((vx_reference_t *)value) == vx_false_e)
    {
        VX_PRINT(VX_ZONE_ERROR, "Supplied value was not actually a reference\n");
        status = VX_ERROR_INVALID_REFERENCE;
        goto exit;
    }

    /* if it was a valid reference then get the type from it */
    vxQueryReference(value, VX_REF_ATTRIBUTE_TYPE, &type, sizeof(type));
    VX_PRINT(VX_ZONE_PARAMETER, "Query returned type %08x for ref "VX_FMT_REF"\n", type, value);
    if (vxIsValidTypeMatch(node->kernel->signature.types[index], type) == vx_false_e)
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid type 0x%08x!\n", type);
        status = VX_ERROR_INVALID_TYPE;
        goto exit;
    }

    if (direction != node->kernel->signature.directions[index])
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid direction for parameter!\n");
        status = VX_ERROR_INVALID_PARAMETERS;
        goto exit;
    }

    if (node->parameters[index])
    {
        /* we already have a value here. */
        vxDecrementIntReference(node->parameters[index]);
    }
    /* if the node has a child graph, find out which parameter is this */
    if (node->child)
    {
        vx_uint32 p = 0;
        for (p = 0; p < node->child->numParams; p++)
        {
            if ((node->child->parameters[p].node == node) &&
                (node->child->parameters[p].index == index))
            {
                status = vxSetGraphParameterByIndex((vx_graph)node->child, p, node->kernel->signature.directions[index], value);
                break;
            }
        }
    }
    node->parameters[index] = (vx_reference_t *)value;
    vxIncrementIntReference(node->parameters[index]);
    status = VX_SUCCESS;
exit:
    if (status == VX_SUCCESS)
    {
        VX_PRINT(VX_ZONE_PARAMETER, "Assigned Node[%u] %p type:%08x dir:%d ref="VX_FMT_REF"\n",
                 index, node, type, direction, value);
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Specified: parameter[%u] dir:%d type:%08x => "VX_FMT_REF"\n",
                        index, direction, type, value);
        VX_PRINT(VX_ZONE_ERROR, "Required: parameter[%u] dir:%d type:%08x\n",
            index,
            node->kernel->signature.directions[index],
            node->kernel->signature.types[index]);
    }
    return status;
}

vx_status vxSetParameterByReference(vx_parameter parameter, vx_reference value) {
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    vx_parameter_t *param = (vx_parameter_t *)parameter;
    if (vxIsValidSpecificReference((vx_reference_t *)param, VX_TYPE_PARAMETER) == vx_true_e)
    {
        if (param->node)
        {
            /*! \bug What if this parameter was given into a sub-graph? How do we update that?
             * We have to wait until Graph Parameter exist */
            if (param->node->parameters[param->index])
            {
                /* we already have a value here. */
                vxDecrementIntReference(param->node->parameters[param->index]);
            }
            param->node->parameters[param->index] = (vx_reference_t *)value;
            vxIncrementReference(param->node->parameters[param->index]);
            status = VX_SUCCESS;
        }
    }
    return status;
}

vx_status vxQueryParameter(vx_parameter param, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_parameter_t *parameter = (vx_parameter_t *)param;
    if (vxIsValidSpecificReference(&parameter->base, VX_TYPE_PARAMETER) == vx_true_e)
    {
        switch (attribute)
        {
            case VX_PARAMETER_ATTRIBUTE_DIRECTION:
                if (VX_CHECK_PARAM(ptr, size, vx_enum, 0x3))
                    *(vx_enum *)ptr = parameter->node->kernel->signature.directions[parameter->index];
                else
                    status = VX_ERROR_INVALID_PARAMETERS;
                break;
            case VX_PARAMETER_ATTRIBUTE_INDEX:
                if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
                    *(vx_uint32 *)ptr = parameter->index;
                else
                    status = VX_ERROR_INVALID_PARAMETERS;
                break;
            case VX_PARAMETER_ATTRIBUTE_TYPE:
                if (VX_CHECK_PARAM(ptr, size, vx_enum, 0x3))
                    *(vx_enum *)ptr = parameter->node->kernel->signature.types[parameter->index];
                else
                    status = VX_ERROR_INVALID_PARAMETERS;
                break;
            case VX_PARAMETER_ATTRIBUTE_STATE:
                if (VX_CHECK_PARAM(ptr, size, vx_enum, 0x3))
                    *(vx_enum *)ptr = (vx_enum)parameter->node->kernel->signature.states[parameter->index];
                else
                    status = VX_ERROR_INVALID_PARAMETERS;
                break;
            case VX_PARAMETER_ATTRIBUTE_REF:
                if (VX_CHECK_PARAM(ptr, size, vx_reference, 0x3))
                {
                    vx_reference_t *ref = parameter->node->parameters[parameter->index];
                    /* does this object have USER access? */
                    if (ref && (ref->external_count == 0))
                    {
                        /*! \internal this could potentially allow the user to break
                         * a currently chosen optimization! We need to alert the
                         * system that if a write occurs to this data, put the graph
                         * into an unverified state.
                         */
                        vxIncrementReference(ref);
                        ref->extracted = vx_true_e;
                    }
                    *(vx_reference *)ptr = (vx_reference)ref;
                }
                else
                    status = VX_ERROR_INVALID_PARAMETERS;
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


