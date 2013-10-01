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

void vxPrintKernel(vx_kernel_t *kernel)
{
    VX_PRINT(VX_ZONE_KERNEL, "kernel[%u] enabled?=%s %s \n",
            kernel->enumeration,
            (kernel->enabled?"TRUE":"FALSE"),
            kernel->name);
}

vx_kernel_t *vxAllocateKernel(vx_context_t *context,
                              vx_enum kenum,
                              vx_kernel_f function,
                              vx_char name[VX_MAX_KERNEL_NAME],
                              vx_param_description_t *parameters,
                              vx_uint32 numParams)
{
    vx_kernel_t *kernel = VX_CALLOC(vx_kernel_t);
    if (kernel)
    {
        vxInitReference((vx_reference_t *)kernel, context, VX_TYPE_KERNEL);
        vxIncrementReference(&kernel->base);
        /* setup the kernel meta-data */
        strncpy(kernel->name, name, VX_MAX_KERNEL_NAME);
        kernel->enumeration = kenum;
        kernel->function = function;
        kernel->signature.numParams = numParams;
        kernel->attributes.borders.mode = VX_BORDER_MODE_UNDEFINED;
        if (kernel->signature.numParams < VX_INT_MAX_PARAMS)
        {
            vx_uint32 p = 0;
            if (parameters != NULL)
            {
                for (p = 0; p < numParams; p++)
                {
                    kernel->signature.directions[p] = parameters[p].direction;
                    kernel->signature.types[p] = parameters[p].type;
                }
            }
        }
        else
        {
            free(kernel);
            kernel = NULL;
        }
    }
    return kernel;
}

vx_status vxInitializeKernel(vx_context_t *context,
                             vx_kernel_t *kernel,
                             vx_enum kenum,
                             vx_kernel_f function,
                             vx_char name[VX_MAX_KERNEL_NAME],
                             vx_param_description_t *parameters,
                             vx_uint32 numParams,
                             vx_kernel_input_validate_f in_validator,
                             vx_kernel_output_validate_f out_validator,
                             vx_kernel_initialize_f initialize,
                             vx_kernel_deinitialize_f deinitialize)
{
    if (kernel)
    {
        vxInitReference((vx_reference_t *)kernel, context, VX_TYPE_KERNEL);
        vxIncrementReference(&kernel->base);
        // setup the kernel meta-data
        strncpy(kernel->name, name, VX_MAX_KERNEL_NAME);
        kernel->enumeration = kenum;
        kernel->function = function;
        kernel->signature.numParams = numParams;
        kernel->validate_input = in_validator;
        kernel->validate_output = out_validator;
        kernel->initialize = initialize;
        kernel->deinitialize = deinitialize;
        kernel->attributes.borders.mode = VX_BORDER_MODE_UNDEFINED;
        kernel->attributes.borders.constant_value = 0;
        if (kernel->signature.numParams < VX_INT_MAX_PARAMS)
        {
            vx_uint32 p = 0;
            if (parameters != NULL)
            {
                for (p = 0; p < numParams; p++)
                {
                    kernel->signature.directions[p] = parameters[p].direction;
                    kernel->signature.types[p] = parameters[p].type;
                    kernel->signature.states[p] = parameters[p].state;
                }
                kernel->enabled = vx_true_e;
            }
        }
    }
    return VX_SUCCESS;
}

static vx_size strnindex(vx_char *str, vx_char c, vx_size limit)
{
    vx_size index = 0;
    while (*str != c && index < limit)
    {
        str++;
        index++;
    }
    return index;
}

/******************************************************************************/
/* PUBLIC FUNCTIONS */
/******************************************************************************/

vx_status vxLoadKernels(vx_context c, vx_char *name)
{
    vx_context_t *context = (vx_context_t *)c;
    vx_status status = VX_FAILURE;
    vx_char module[VX_INT_MAX_PATH];
    vx_uint32 m = 0;
    vx_publish_kernels_f publish = NULL;

    sprintf(module, VX_MODULE_NAME("%s"), (name?name:"openvx-ext"));

    if (vxIsValidContext(context) == vx_false_e)
    {
        VX_PRINT(VX_ZONE_ERROR, "Context is invalid!\n");
        return VX_ERROR_INVALID_REFERENCE;
    }
    for (m = 0; m < VX_INT_MAX_MODULES; m++)
    {
        if (context->modules[m].handle == NULL)
        {
            context->modules[m].handle = vxLoadModule(module);
            if (context->modules[m].handle)
            {
                vx_symbol_t sym = vxGetSymbol(context->modules[m].handle, "vxPublishKernels");
                publish = (vx_publish_kernels_f)sym;
                if (publish == NULL)
                {
                    VX_PRINT(VX_ZONE_ERROR, "Failed to load symbol vxPublishKernels\n");
                    status = VX_ERROR_INVALID_MODULE;
                    vxUnloadModule(context->modules[m].handle);
                    context->modules[m].handle = NULL;
                }
                else
                {
                    VX_PRINT(VX_ZONE_INFO, "Calling %s publish function\n", module);
                    status = publish((vx_context)context);
                    if (status != VX_SUCCESS)
                    {
                        VX_PRINT(VX_ZONE_ERROR, "Failed to publish kernels in module\n");
                        vxUnloadModule(context->modules[m].handle);
                        context->modules[m].handle = NULL;
                    }
                    else
                    {
                        strncpy(context->modules[m].name, name, VX_INT_MAX_PATH);
                        context->numMods++;
                    }
                }
            }
            else
            {
                VX_PRINT(VX_ZONE_ERROR, "Failed to find module %s in libraries path\n", module);
            }
            break;
        }
        else
        {
            VX_PRINT(VX_ZONE_CONTEXT, "module[%u] is used\n", m);
        }
    }
    if (status != VX_SUCCESS)
    {
        VX_PRINT(VX_ZONE_ERROR, "Failed to load module %s; error %d\n", module, status);
    }
    else
    {
        for (m = 0; m < context->numMods; m++)
        {
            VX_PRINT(VX_ZONE_INFO, "Module: %s\n", context->modules[m].name);
        }
    }
    return status;
}

vx_kernel vxGetKernelByName(vx_context c, vx_char *name)
{
    vx_kernel_t *kern = NULL;
    vx_context_t *context = (vx_context_t *)c;
    if (vxIsValidContext(context) == vx_true_e)
    {
        vx_uint32 k = 0u, t = 0u;
        VX_PRINT(VX_ZONE_KERNEL, "Scanning for kernel %s out of %d kernels\n", name, context->numKernels);

        for (t = 0; t < context->numTargets; t++)
        {
            vx_target_t *target = &context->targets[context->priority_targets[t]];
            for (k = 0; k < target->numKernels; k++)
            {
                vx_kernel_t *kernel = &target->kernels[k];
                vxPrintKernel(kernel);
                if ((kernel->enabled == vx_true_e) &&
                    (strncmp(kernel->name, name, VX_MAX_KERNEL_NAME) == 0))
                {
                    kernel->affinity = context->priority_targets[t];
                    kern = kernel;
                    vxIncrementReference(&kern->base);
                    break;
                }
                kernel = NULL;
            }
            if (kern != NULL)
                break;
        }
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid context %p\n", context);
    }
    if (kern == NULL)
    {
        VX_PRINT(VX_ZONE_ERROR, "Failed to find kernel %s\n", name);
    }
    else
    {
        VX_PRINT(VX_ZONE_KERNEL,"Found Kernel enum %d, name %s\n", kern->enumeration, kern->name);
    }
    return (vx_kernel)kern;
}

vx_kernel vxGetKernelByEnum(vx_context c, vx_enum kernelenum)
{
    vx_kernel_t *kern = NULL;
    vx_context_t *context = (vx_context_t *)c;
    vxPrintReference(&context->base);
    if (vxIsValidContext(context) == vx_true_e)
    {
        if (VX_KERNEL_INVALID >= kernelenum)
        {
            vxAddLogEntry(c, VX_ERROR_INVALID_PARAMETERS, "Invalid kernel enumeration (%d)\n", kernelenum);
        }
        else if (kernelenum > VX_KERNEL_INVALID) // no upper bound for kernel enum
        {
            vx_uint32 k = 0u, t = 0u;
            VX_PRINT(VX_ZONE_KERNEL,"Scanning for kernel enum %d out of %d kernels\n", kernelenum, context->numKernels);
            for (t = 0; t < context->numTargets; t++)
            {
                vx_target_t *target = &context->targets[context->priority_targets[t]];
                VX_PRINT(VX_ZONE_KERNEL, "Checking Target[%u]=%s for %u kernels\n", context->priority_targets[t], target->name, target->numKernels);
                for (k = 0; k < target->numKernels; k++)
                {
                    vx_kernel_t *kernel = &target->kernels[k];
                    if (kernel->enumeration == kernelenum)
                    {
                        kernel->affinity = context->priority_targets[t];
                        kern = kernel;
                        vxIncrementReference(&kern->base);
                        VX_PRINT(VX_ZONE_KERNEL,"Found Kernel[%u] enum:%d name:%s in target[%u]=%s\n", k, kernelenum, kern->name, context->priority_targets[t], target->name);
                        break;
                    }
                    kernel = NULL;
                }
                if (kern != NULL)
                    break;
            }
        }
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid context %p\n", context);
    }
    return (vx_kernel)kern;
}

void vxReleaseKernel(vx_kernel *kernel) {
    vx_kernel_t *kern = (vx_kernel_t *)(kernel?*kernel:0);
    if (vxIsValidSpecificReference(&kern->base, VX_TYPE_KERNEL) == vx_true_e)
    {
        VX_PRINT(VX_ZONE_KERNEL, "Releasing kernel "VX_FMT_REF"\n", (void *)kern);
        vxDecrementReference(&kern->base);
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid Reference!\n");
    }
    if (kernel) *kernel = 0;
}

vx_kernel vxAddKernel(vx_context c,
                       vx_char name[VX_MAX_KERNEL_NAME],
                       vx_enum enumeration,
                       vx_kernel_f func_ptr,
                       vx_uint32 numParams,
                       vx_kernel_input_validate_f input,
                       vx_kernel_output_validate_f output,
                       vx_kernel_initialize_f initialize,
                       vx_kernel_deinitialize_f deinitialize)
{
    vx_context_t *context = (vx_context_t *)c;
    vx_kernel kernel = 0;
    vx_uint32 t = 0;
    vx_size index = 0;
    vx_target_t *target = NULL;
    vx_char targetName[VX_MAX_TARGET_NAME];

    if (vxIsValidContext(context) == vx_false_e)
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid Context\n");
        return (vx_kernel)NULL;
    }
    if (func_ptr == NULL ||
        input == NULL ||
        output == NULL ||
        numParams > VX_INT_MAX_PARAMS || numParams == 0 ||
        name == NULL ||
        strncmp(name, "",  VX_MAX_KERNEL_NAME) == 0)
        /* initialize and de-initialize can be NULL */
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid Parameters!\n");
        vxAddLogEntry(c, VX_ERROR_INVALID_PARAMETERS, "Invalid Parameters supplied to vxAddKernel\n");
        return (vx_kernel)NULL;
    }

    /* find target to assign this to */
    index = strnindex(name, ':', VX_MAX_TARGET_NAME);
    if (index == VX_MAX_TARGET_NAME)
    {
        strcpy(targetName,"khronos.c_model");
    }
    else
    {
        strncpy(targetName, name, index);
    }
    VX_PRINT(VX_ZONE_KERNEL, "Deduced Name as %s\n", targetName);
    for (t = 0u; t < context->numTargets; t++)
    {
        target = &context->targets[t];
        if (strncmp(targetName,target->name, VX_MAX_TARGET_NAME) == 0)
        {
            break;
        }
        target = NULL;
    }
    if (target)
    {
        kernel = target->funcs.addkernel(target, name, enumeration,
                                         func_ptr, numParams,
                                         input, output,
                                         initialize, deinitialize);
        VX_PRINT(VX_ZONE_KERNEL,"Added Kernel %s to Target %s ("VX_FMT_REF")\n", name, target->name, kernel);
    }
    else
    {
        vxAddLogEntry(c, VX_ERROR_NO_RESOURCES, "No target named %s exists!\n", targetName);
    }
    return (vx_kernel)kernel;
}

vx_kernel vxAddTilingKernel(vx_context c,
                            vx_char name[VX_MAX_KERNEL_NAME],
                            vx_enum enumeration,
                            vx_tiling_kernel_f func_ptr,
                            vx_uint32 num_params,
                            vx_kernel_input_validate_f input,
                            vx_kernel_output_validate_f output)
{
    vx_context_t *context = (vx_context_t *)c;
    vx_kernel kernel = 0;
    vx_uint32 t = 0;
    vx_size index = 0;
    vx_target_t *target = NULL;
    vx_char targetName[VX_MAX_TARGET_NAME];

    if (vxIsValidContext(context) == vx_false_e)
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid Context\n");
        return (vx_kernel)NULL;
    }
    if (func_ptr == NULL ||
        input == NULL ||
        output == NULL ||
        num_params > VX_INT_MAX_PARAMS || num_params == 0 ||
        name == NULL ||
        strncmp(name, "",  VX_MAX_KERNEL_NAME) == 0)
        /* initialize and de-initialize can be NULL */
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid Parameters!\n");
        vxAddLogEntry(c, VX_ERROR_INVALID_PARAMETERS, "Invalid Parameters supplied to vxAddKernel\n");
        return (vx_kernel)NULL;
    }

    /* find target to assign this to */
    index = strnindex(name, ':', VX_MAX_TARGET_NAME);
    if (index == VX_MAX_TARGET_NAME)
    {
        strcpy(targetName,"khronos.c_model");
    }
    else
    {
        strncpy(targetName, name, index);
    }
    VX_PRINT(VX_ZONE_KERNEL, "Deduced Name as %s\n", targetName);
    for (t = 0u; t < context->numTargets; t++)
    {
        target = &context->targets[t];
        if (strncmp(targetName,target->name, VX_MAX_TARGET_NAME) == 0)
        {
            break;
        }
        target = NULL;
    }
    if (target && target->funcs.addtilingkernel)
    {
        kernel = target->funcs.addtilingkernel(target, name, enumeration,
                                         func_ptr, num_params,
                                         input, output);
        VX_PRINT(VX_ZONE_KERNEL,"Added Kernel %s to Target %s ("VX_FMT_REF")\n", name, target->name, kernel);
    }
    else
    {
        vxAddLogEntry(c, VX_ERROR_NO_RESOURCES, "No target named %s exists!\n", targetName);
    }
    return (vx_kernel)kernel;
}

vx_status vxFinalizeKernel(vx_kernel kernel) {
    vx_status status = VX_SUCCESS;
    vx_kernel_t *kern = (vx_kernel_t *)kernel;
    if (vxIsValidSpecificReference(&kern->base, VX_TYPE_KERNEL) == vx_true_e)
    {
        vx_uint32 p = 0;
        for (p = 0; p < VX_INT_MAX_PARAMS; p++)
        {
            if (p >= kern->signature.numParams)
            {
                break;
            }
            if ((kern->signature.directions[p] < VX_INPUT) ||
                (kern->signature.directions[p] > VX_BIDIRECTIONAL))
            {
                status = VX_ERROR_INVALID_PARAMETERS;
                break;
            }
            if (vxIsValidType(kern->signature.types[p]) == vx_false_e)
            {
                status = VX_ERROR_INVALID_PARAMETERS;
                break;
            }
        }
        if (p == kern->signature.numParams)
        {
            kern->enabled = vx_true_e;
            kern->base.context->numKernels++;
        }
    }
    else
    {
        status = VX_ERROR_INVALID_REFERENCE;
    }
    return status;
}

vx_status vxQueryKernel(vx_kernel kern, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_kernel_t *kernel = (vx_kernel_t *)kern;
    if (vxIsValidSpecificReference(&kernel->base, VX_TYPE_KERNEL) == vx_true_e)
    {
        switch (attribute)
        {
            case VX_KERNEL_ATTRIBUTE_NUMPARAMS:
                if (VX_CHECK_PARAM(ptr, size, vx_uint32, 0x3))
                {
                    *(vx_uint32 *)ptr = kernel->signature.numParams;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_KERNEL_ATTRIBUTE_NAME:
                if (ptr != NULL && size <= VX_MAX_KERNEL_NAME)
                {
                    strncpy(ptr, kernel->name, VX_MAX_KERNEL_NAME);
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_KERNEL_ATTRIBUTE_ENUM:
                if (VX_CHECK_PARAM(ptr, size, vx_enum, 0x3))
                {
                    *(vx_enum *)ptr = kernel->enumeration;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_KERNEL_ATTRIBUTE_LOCAL_DATA_SIZE:
                if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
                {
                    *(vx_size *)ptr = kernel->attributes.localDataSize;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
#ifdef OPENVX_KHR_NODE_MEMORY
            case VX_KERNEL_ATTRIBUTE_GLOBAL_DATA_SIZE:
                if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
                {
                    *(vx_size *)ptr = kernel->attributes.globalDataSize;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_KERNEL_ATTRIBUTE_GLOBAL_DATA_PTR:
                if (VX_CHECK_PARAM(ptr, size, vx_ptr_t, 0x1))
                {
                    *(vx_ptr_t *)ptr = kernel->attributes.globalDataPtr;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
#endif
#ifdef OPENVX_KHR_TILING
            case VX_KERNEL_ATTRIBUTE_INPUT_NEIGHBORHOOD:
                if (VX_CHECK_PARAM(ptr, size, vx_neighborhood_size_t, 0x3))
                {
                    memcpy(ptr, &kernel->attributes.nhbdinfo, size);
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;

            case VX_KERNEL_ATTRIBUTE_OUTPUT_TILE_BLOCK_SIZE:
                if (VX_CHECK_PARAM(ptr, size, vx_tile_block_size_t, 0x3))
                {
                    memcpy(ptr, &kernel->attributes.blockinfo, size);
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
    return status;
}

vx_status vxAddParameterToKernel(vx_kernel kernel,
                                 vx_uint32 index,
                                 vx_enum dir,
                                 vx_enum type,
                                 vx_enum state) {
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    vx_kernel_t *kern = (vx_kernel_t *)kernel;

    VX_PRINT(VX_ZONE_KERNEL,"INFO: Adding index %u, type 0x%x, dir:%d state:%d\n", index, type, dir, state);

    if (vxIsValidSpecificReference(&kern->base, VX_TYPE_KERNEL) == vx_true_e)
    {
        if (index < kern->signature.numParams)
        {
            if (kern->tiling_function)
            {
                if (((type != VX_TYPE_IMAGE) &&
                     (type != VX_TYPE_SCALAR)) ||
                    (vxIsValidDirection(dir) == vx_false_e) ||
                    (vxIsValidState(state) == vx_false_e))
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                else
                {
                    kern->signature.directions[index] = dir;
                    kern->signature.types[index] = type;
                    kern->signature.states[index] = state;
                    status = VX_SUCCESS;
                }
            }
            else
            {
                if ((vxIsValidType(type) == vx_false_e) ||
                    (vxIsValidDirection(dir) == vx_false_e) ||
                    (vxIsValidState(state) == vx_false_e))
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                else
                {
                    kern->signature.directions[index] = dir;
                    kern->signature.types[index] = type;
                    kern->signature.states[index] = state;
                    status = VX_SUCCESS;
                }
            }
        }
        else
        {
            status = VX_ERROR_INVALID_PARAMETERS;
        }
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Not a valid reference!\n");
        status = VX_ERROR_INVALID_REFERENCE;
    }
    return status;
}

vx_status vxRemoveKernel(vx_kernel kernel) {
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    vx_kernel_t *kern = (vx_kernel_t *)kernel;
    if (vxIsValidSpecificReference(&kern->base, VX_TYPE_KERNEL) == vx_true_e)
    {
        vxDecrementReference(&kern->base);
        kern->enabled = vx_false_e;
        kern->enumeration = VX_KERNEL_INVALID;
        kern->base.context->numKernels--;
        status = VX_SUCCESS;
    }
    return status;
}

vx_status vxSetKernelAttribute(vx_kernel k, vx_enum attribute, void * ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_kernel_t *kernel = (vx_kernel_t *)k;

    if (vxIsValidSpecificReference(&kernel->base, VX_TYPE_KERNEL) == vx_false_e)
    {
        return VX_ERROR_INVALID_REFERENCE;
    }
    if (kernel->enabled == vx_true_e)
    {
        return VX_ERROR_NOT_SUPPORTED;
    }
    switch (attribute)
    {
        case VX_KERNEL_ATTRIBUTE_LOCAL_DATA_SIZE:
            if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
            {
                kernel->attributes.localDataSize = *(vx_size *)ptr;
                VX_PRINT(VX_ZONE_KERNEL, "Set Local Data Size to "VX_FMT_SIZE" bytes\n", kernel->attributes.localDataSize);
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_KERNEL_ATTRIBUTE_LOCAL_DATA_PTR:
            if (VX_CHECK_PARAM(ptr, size, vx_ptr_t, 0x1))
            {
                kernel->attributes.localDataPtr = *(vx_ptr_t *)ptr;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_KERNEL_ATTRIBUTE_GLOBAL_DATA_SIZE:
            if (VX_CHECK_PARAM(ptr, size, vx_size, 0x3))
            {
                kernel->attributes.globalDataSize = *(vx_size *)ptr;
                VX_PRINT(VX_ZONE_KERNEL, "Set Global Data Size to "VX_FMT_SIZE" bytes\n", kernel->attributes.globalDataSize);
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_KERNEL_ATTRIBUTE_GLOBAL_DATA_PTR:
            if (VX_CHECK_PARAM(ptr, size, vx_ptr_t, 0x1))
            {
                kernel->attributes.globalDataPtr = *(vx_ptr_t *)ptr;
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
#ifdef OPENVX_KHR_TILING
        case VX_KERNEL_ATTRIBUTE_INPUT_NEIGHBORHOOD:
            if (VX_CHECK_PARAM(ptr, size, vx_neighborhood_size_t, 0x3))
            {
                memcpy(&kernel->attributes.nhbdinfo, ptr, size);
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_KERNEL_ATTRIBUTE_OUTPUT_TILE_BLOCK_SIZE:
            if (VX_CHECK_PARAM(ptr, size, vx_tile_block_size_t, 0x3))
            {
                memcpy(&kernel->attributes.blockinfo, ptr, size);
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            break;
        case VX_KERNEL_ATTRIBUTE_BORDER:
            if (VX_CHECK_PARAM(ptr, size, vx_border_mode_t, 0x3))
            {
                vx_border_mode_t *border = (vx_border_mode_t *)ptr;
                if ((border->mode == VX_BORDER_MODE_SELF) ||
                    (border->mode == VX_BORDER_MODE_UNDEFINED))
                {
                    memcpy(&kernel->attributes.borders, border, sizeof(vx_border_mode_t));
                }
                else
                {
                    status = VX_ERROR_INVALID_VALUE;
                }
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
    return status;
}

