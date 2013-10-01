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

const vx_char implementation[VX_MAX_IMPLEMENTATION_NAME] = "khronos.sample";

vx_char targetModules[][VX_MAX_TARGET_NAME] = {
    "openvx-c_model",
#if defined(OPENVX_USE_OPENCL)
    "openvx-opencl",
#endif
};

const vx_char extensions[] =
#if defined(OPENVX_USE_OPENCL)
    OPENVX_KHR_OPENCL" "
#endif
#if defined(OPENVX_USE_LIST)
    OPENVX_KHR_LIST" "
#endif
#if defined(OPENVX_USE_NODE_MEMORY)
    OPENVX_KHR_NODE_MEMORY" "
#endif
    "vx_khr_u8 "
    "vx_khr_s16 "
    " ";

static vx_value_t threadGraphExec(void *arg)
{
    vx_processor_t *proc = (vx_processor_t *)arg;
    VX_PRINT(VX_ZONE_CONTEXT, "Starting thread!\n");
    while (proc->running == vx_true_e)
    {
        vx_graph g = 0;
        vx_status s = VX_FAILURE;
        vx_value_t v1, v2;
        if (vxReadQueue(&proc->input,&v1,&v2) == vx_true_e)
        {
            g = (vx_graph)v1;
            // s = (vx_status)v2;
            VX_PRINT(VX_ZONE_CONTEXT, "Read graph=" VX_FMT_REF ", status=%d\n",g,s);
            s = vxProcessGraph(g);
            VX_PRINT(VX_ZONE_CONTEXT, "Writing graph=" VX_FMT_REF ", status=%d\n",g,s);
            v1 = (vx_value_t)g;
            v2 = (vx_status)s;
            if (vxWriteQueue(&proc->output, v1, v2) == vx_false_e)
                VX_PRINT(VX_ZONE_ERROR, "Failed to write graph=" VX_FMT_REF " status=%d\n", g, s);
        }
    }
    VX_PRINT(VX_ZONE_CONTEXT,"Stopping thread!\n");
    return 0;
}

vx_bool vxIsValidType(vx_enum type)
{
    vx_bool ret = vx_false_e;
    if (type <= VX_TYPE_INVALID)
    {
        ret = vx_false_e;
    }
    else if (VX_TYPE_IS_SCALAR(type)) /* some scalar */
    {
        ret = vx_true_e;
    }
    else if (VX_TYPE_IS_OBJECT(type)) /* some object */
    {
        ret = vx_true_e;
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Type 0x%08x is invalid!\n");
    }
    return ret; /* otherwise, not a valid type */
}

vx_bool vxIsValidImport(vx_enum type)
{
    vx_bool ret = vx_false_e;
    switch(type)
    {
        case VX_IMPORT_TYPE_HOST:
            ret = vx_true_e;
            break;
        case VX_IMPORT_TYPE_NONE:
        default:
            ret = vx_false_e;
            break;
    }
    return ret;
}

vx_bool vxIsValidContext(vx_context_t *context)
{
    vx_bool ret = vx_false_e;
    if ((context != NULL) &&
        (context->base.magic == VX_MAGIC) &&
        (context->base.type == VX_TYPE_CONTEXT) &&
        (context->base.context == NULL))
    {
        ret = vx_true_e; /* this is the top level context */
    }
    if (ret == vx_false_e)
    {
        VX_PRINT(VX_ZONE_ERROR, "%p is not a valid context!\n", context);
    }
    return ret;
}

vx_bool vxIsValidBorderMode(vx_enum mode)
{
    vx_bool ret = vx_true_e;
    switch (mode)
    {
        case VX_BORDER_MODE_UNDEFINED:
        case VX_BORDER_MODE_CONSTANT:
        case VX_BORDER_MODE_REPLICATE:
            break;
        default:
            ret = vx_false_e;
            break;
    }
    return ret;
}

/******************************************************************************/
/* PUBLIC API */
/******************************************************************************/

static vx_context_t *single_context = NULL;
static vx_sem_t context_lock = INIT_SEM;

vx_context vxCreateContext() {
    vx_context_t *context = NULL;

    vxSemWait(&context_lock);
    if (single_context == NULL)
    {
        /* read the variables for debugging flags */
        vx_set_debug_zone_from_env();

        context = VX_CALLOC(vx_context_t); /* \todo get from allocator? */
        if (context)
        {
            vx_uint32 p = 0u, p2 = 0u, t = 0u;
            vxInitReference((vx_reference_t *)context, NULL, VX_TYPE_CONTEXT);
            vxIncrementReference(&context->base);
            /* we don't add the reference to context to the references list */
            vxInitLog(&context->log);

            /* load all targets */
            for (t = 0u; t < dimof(targetModules); t++)
            {
                if (vxLoadTarget(context, targetModules[t]) == VX_SUCCESS)
                {
                    context->numTargets++;
                }
            }

            /* initialize all targets */
            for (t = 0u; t < context->numTargets; t++)
            {
                if (context->targets[t].module.handle)
                {
                    /* call the init function */
                    if (context->targets[t].funcs.init(&context->targets[t]) != VX_SUCCESS)
                    {
                        VX_PRINT(VX_ZONE_WARNING, "Target %s failed to initialize!\n", context->targets[t].name);
                        /* unload this module */
                        vxUnloadTarget(context, t);
                        break;
                    }
                    else
                    {
                        context->targets[t].enabled = vx_true_e;
                    }
                }
            }

            /* assign the targets by priority into the list */
            p2 = 0u;
            for (p = 0u; p < VX_TARGET_PRIORITY_MAX; p++)
            {
                for (t = 0u; t < context->numTargets; t++)
                {
                    vx_target_t * target = &context->targets[t];
                    if (p == target->priority)
                    {
                        context->priority_targets[p2] = t;
                        p2++;
                    }
                }
            }
            /* print out the priority list */
            for (t = 0u; t < context->numTargets; t++)
            {
                vx_target_t *target = &context->targets[context->priority_targets[t]];
                if (target->enabled == vx_true_e)
                {
                    VX_PRINT(VX_ZONE_TARGET, "target[%u]: %s\n",
                                target->priority,
                                target->name);
                }
            }

            // create the internal thread which processes graphs for asynchronous mode.
            vxInitQueue(&context->proc.input);
            vxInitQueue(&context->proc.output);
            context->proc.running = vx_true_e;
            context->proc.thread = vxCreateThread(threadGraphExec, &context->proc);
            single_context = context;
        }
    }
    else
    {
        context = single_context;
        vxIncrementReference(&context->base);
    }
    vxSemPost(&context_lock);
    return (vx_context)context;
}

void vxReleaseContext(vx_context *c)
{
    vx_context_t *context = (vx_context_t *)(c?*c:0);
    vx_uint32 r,m,t;
    if (c) *c = 0;
    vxSemWait(&context_lock);
    if (vxIsValidContext(context) == vx_true_e)
    {
        vxDecrementReference(&context->base);
        if (vxTotalReferenceCount(&context->base) == 0)
        {
            context->proc.running = vx_false_e;
            vxPopQueue(&context->proc.input);
            vxJoinThread(context->proc.thread, NULL);

            /* de-initialize each target */
            for (t = 0u; t < context->numTargets; t++)
            {
                if (context->targets[t].enabled == vx_true_e)
                {
                    context->targets[t].funcs.deinit(&context->targets[t]);
                    vxUnloadTarget(context, t);
                    context->targets[t].enabled = vx_false_e;
                }
            }

            for (m = 0; m < context->numMods; m++)
            {
                if (context->modules[m].handle)
                {
                    vxUnloadModule(context->modules[m].handle);
                    memset(context->modules[m].name, 0, sizeof(context->modules[m].name));
                    context->modules[m].handle = VX_MODULE_INIT;
                }
            }

            /*! \internal Garbage Collect All References */
            for (r = 0; r < context->numRefs; r++)
            {
                if (context->reftable[r])
                {
                    vx_reference_t *ref = context->reftable[r];
                    if (vxTotalReferenceCount(ref) > 0)
                    {
                        VX_PRINT(VX_ZONE_WARNING,"Stale Reference "VX_FMT_REF" of type %08x at count %u\n",
                                ref,
                                ref->type,
                                vxTotalReferenceCount(ref));
                        vxPrintReference(ref);
                        /* force a destroy since the USER did not */
                        switch (ref->type)
                        {
                            case VX_TYPE_BUFFER:
                                vxReleaseBuffer((vx_buffer *)&ref);
                                break;
                            case VX_TYPE_CONVOLUTION:
                                vxReleaseConvolution((vx_convolution *)&ref);
                                break;
                            case VX_TYPE_COORDINATES:
                                vxReleaseCoordinates((vx_coordinates *)&ref);
                                break;
                            case VX_TYPE_DELAY:
                                vxReleaseDelay((vx_delay *)&ref);
                                break;
                            case VX_TYPE_DISTRIBUTION:
                                vxReleaseDistribution((vx_distribution *)&ref);
                                break;
                            case VX_TYPE_GRAPH:
                                vxReleaseGraph((vx_graph *)&ref);
                                break;
                            case VX_TYPE_IMAGE:
                                vxReleaseImage((vx_image *)&ref);
                                break;
                            case VX_TYPE_KERNEL:
                                vxReleaseKernel((vx_kernel *)&ref);
                                break;
                            case VX_TYPE_LIST:
                                vxReleaseList((vx_list *)&ref);
                                break;
                            case VX_TYPE_LUT:
                                vxReleaseLUT((vx_lut *)&ref);
                                break;
                            case VX_TYPE_MATRIX:
                                vxReleaseMatrix((vx_matrix *)&ref);
                                break;
                            case VX_TYPE_NODE:
                                vxReleaseNode((vx_node *)&ref);
                                break;
                            case VX_TYPE_PARAMETER:
                                vxReleaseParameter((vx_parameter *)&ref);
                                break;
                            case VX_TYPE_PYRAMID:
                                vxReleasePyramid((vx_pyramid *)&ref);
                                break;
                            case VX_TYPE_RECTANGLE:
                                vxReleaseParameter((vx_rectangle *)&ref);
                                break;
                            case VX_TYPE_REFERENCE:
                                /* this should not be possible, but let's clean up anyway */
                                vxReleaseReference(ref, VX_TYPE_REFERENCE, vx_true_e, NULL);
                                break;
                            case VX_TYPE_SCALAR:
                                vxReleaseScalar((vx_scalar *)&ref);
                                break;
                            case VX_TYPE_TARGET:
                                vxReleaseTarget((vx_target *)&ref);
                                break;
                            case VX_TYPE_THRESHOLD:
                                vxReleaseThreshold((vx_threshold *)&ref);
                                break;
                            /*! \todo add more types here as they come along!!! */
                            default:
                                VX_PRINT(VX_ZONE_ERROR, "#### Attempting to destroy reference "VX_FMT_REF" with unknown type 0x%08x\n", ref, ref->type);
                                vxPrintReference(ref);
                                break;
                        }
                    }
                    else
                    {
                        VX_PRINT(VX_ZONE_ERROR, "#### Reference "VX_FMT_REF" with zero referents!\n", ref);
                        vxPrintReference(ref);
                    }
                }
            }
            /*! \internal wipe away the context memory first */
            memset(context, 0, sizeof(vx_context_t));
            free((void *)context);
            single_context = NULL;
        }
        else
        {
            VX_PRINT(VX_ZONE_WARNING, "Context still has %u holders\n", vxTotalReferenceCount(&context->base));
        }
    }
    vxSemPost(&context_lock);
}

vx_context vxGetContext(vx_reference reference) {
    vx_reference_t *ref = (vx_reference_t *)reference;
    vx_context context = 0;
    if (vxIsValidReference(ref) == vx_true_e)
    {
        context = (vx_context)ref->context;
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "%p is not a valid reference\n", ref);
    }
    return context;
}

vx_status vxQueryContext(vx_context c, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_context_t *context = (vx_context_t *)c;
    if (vxIsValidContext(context) == vx_false_e)
    {
        status = VX_ERROR_INVALID_REFERENCE;
    }
    else
    {
        switch (attribute)
        {
            case VX_CONTEXT_ATTRIBUTE_VENDOR_ID:
                if (VX_CHECK_PARAM(ptr, size, vx_uint16, 0x1))
                {
                    *(vx_uint16 *)ptr = VX_ID_KHRONOS;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_CONTEXT_ATTRIBUTE_VERSION:
                if (VX_CHECK_PARAM(ptr, size, vx_uint16, 0x1))
                {
                    *(vx_uint16 *)ptr = VX_VERSION;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_CONTEXT_ATTRIBUTE_NUMKERNELS:
                if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
                {
                    *(vx_uint32 *)ptr = context->numKernels;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_CONTEXT_ATTRIBUTE_NUMMODULES:
                if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
                {
                    *(vx_uint32 *)ptr = context->numMods;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_CONTEXT_ATTRIBUTE_NUMREFS:
                if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
                {
                    *(vx_uint32 *)ptr = context->numRefs;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_CONTEXT_ATTRIBUTE_NUMTARGETS:
                if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
                {
                    *(vx_uint32 *)ptr = context->numTargets;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_CONTEXT_ATTRIBUTE_IMPLEMENTATION:
                if (size <= VX_MAX_IMPLEMENTATION_NAME && ptr)
                {
                    strncpy(ptr, implementation, VX_MAX_IMPLEMENTATION_NAME);
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_CONTEXT_ATTRIBUTE_EXTENSIONS_SIZE:
                if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
                {
                    *(vx_size *)ptr = sizeof(extensions);
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_CONTEXT_ATTRIBUTE_EXTENSIONS:
                if (size <= VX_MAX_IMPLEMENTATION_NAME && ptr)
                {
                    strncpy(ptr, extensions, sizeof(extensions));
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_CONTEXT_ATTRIBUTE_CONVOLUTION_MAXIMUM_DIMENSION:
                if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
                {
                    *(vx_size *)ptr = VX_CONVOLUTION_MAX_DIM;
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
    return status;
}

vx_status vxHint(vx_context c, vx_reference reference, vx_enum hint) {
    vx_status status = VX_SUCCESS;
    vx_context_t *context = (vx_context_t *)c;
    vx_reference_t *ref = (vx_reference_t *)reference;
    if (vxIsValidContext(context) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;
    if (vxIsValidReference(ref) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;
    switch (hint)
    {
        /*! \todo add hints to the sample implementation */
        default:
            status = VX_ERROR_NOT_SUPPORTED;
            break;
    }
    return status;
}

vx_status vxDirective(vx_context c, vx_reference reference, vx_enum directive) {
    vx_status status = VX_SUCCESS;
    vx_context_t *context = (vx_context_t *)c;
    vx_reference_t *ref = (vx_reference_t *)reference;
    if (vxIsValidContext(context) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;
    if (vxIsValidReference(ref) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;
    switch (directive)
    {
        case VX_DIRECTIVE_DISABLE_LOGGING:
            context->log.enabled = vx_false_e;
            break;
        case VX_DIRECTIVE_ENABLE_LOGGING:
            context->log.enabled = vx_true_e;
            break;
        default:
            status = VX_ERROR_NOT_SUPPORTED;
            break;
    }
    return status;
}

vx_status vxSetImmediateBorderMode(vx_context context, vx_border_mode_t *config)
{
    if (vxIsValidContext((vx_context_t *)context) == vx_false_e)
        return VX_ERROR_INVALID_REFERENCE;
    if (config == NULL)
        return VX_ERROR_INVALID_PARAMETERS;
    if (vxIsValidBorderMode(config->mode) == vx_false_e)
        return VX_ERROR_INVALID_VALUE;
    /*! \internal we don't support any border mode other than undefined for
     * immediate mode functions. */
    return VX_ERROR_NOT_SUPPORTED;
}
