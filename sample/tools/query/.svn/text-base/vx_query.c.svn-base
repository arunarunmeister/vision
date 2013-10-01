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

#include <stdio.h>
#include <stdlib.h>
#include <VX/vx.h>
#include <VX/vx_helper.h>
#ifdef OPENVX_USE_NODE_MEMORY
#include <VX/vx_khr_node_memory.h>
#endif

typedef struct _vx_type_name_t {
    vx_char name[128];
    vx_enum tenum;
    vx_enum type;
    vx_size size;
} vx_type_name_t;

#define _STR2(x) #x,(vx_enum)x

/*!< The list of settable attributes on a kernel */
vx_type_name_t attribute_names[] = {
    {_STR2(VX_KERNEL_ATTRIBUTE_LOCAL_DATA_SIZE),  VX_TYPE_SIZE, sizeof(vx_size)},
#ifdef OPENVX_KHR_NODE_MEMORY
    {_STR2(VX_KERNEL_ATTRIBUTE_GLOBAL_DATA_SIZE), VX_TYPE_SIZE, sizeof(vx_size)},
    {_STR2(VX_KERNEL_ATTRIBUTE_GLOBAL_DATA_PTR),  VX_TYPE_SIZE, sizeof(vx_size)},
#endif
};

vx_type_name_t parameter_names[] = {
    {_STR2(VX_TYPE_INVALID), VX_TYPE_ENUM, sizeof(vx_enum)},
    /* SCALARS */
    {_STR2(VX_TYPE_CHAR), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_INT8), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_UINT8), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_INT16), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_UINT16), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_INT32), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_UINT32), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_INT64), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_UINT64), VX_TYPE_ENUM, sizeof(vx_enum)},
#if defined(OVX_PLATFORM_SUPPORT_FLOAT16)
    {_STR2(VX_TYPE_FLOAT16), VX_TYPE_ENUM, sizeof(vx_enum)},
#endif
    {_STR2(VX_TYPE_FLOAT32), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_FLOAT64), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_ENUM), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_SIZE), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_FOURCC), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_SCALAR), VX_TYPE_ENUM, sizeof(vx_enum)},

    /* OBJECTS */
    {_STR2(VX_TYPE_REFERENCE), VX_TYPE_ENUM, sizeof(vx_enum)},

    /* FRAMEWORK OBJECTS */
    {_STR2(VX_TYPE_CONTEXT), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_GRAPH), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_NODE), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_KERNEL), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_PARAMETER), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_TARGET), VX_TYPE_ENUM, sizeof(vx_enum)},

    /* DATA OBJECTS */
    {_STR2(VX_TYPE_BUFFER), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_IMAGE), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_RECTANGLE), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_COORDINATES), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_CONVOLUTION), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_PYRAMID), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_THRESHOLD), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_DISTRIBUTION), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_TYPE_LUT), VX_TYPE_ENUM, sizeof(vx_enum)},

    /* IMAGES */
//  {_STR2(FOURCC_VIRT)},
    {_STR2(FOURCC_NV12), VX_TYPE_FOURCC, sizeof(vx_fourcc)},
    {_STR2(FOURCC_NV21), VX_TYPE_FOURCC, sizeof(vx_fourcc)},
    {_STR2(FOURCC_RGB), VX_TYPE_FOURCC, sizeof(vx_fourcc)},
    {_STR2(FOURCC_RGBX), VX_TYPE_FOURCC, sizeof(vx_fourcc)},
    {_STR2(FOURCC_UYVY), VX_TYPE_FOURCC, sizeof(vx_fourcc)},
    {_STR2(FOURCC_YUYV), VX_TYPE_FOURCC, sizeof(vx_fourcc)},
    {_STR2(FOURCC_IYUV), VX_TYPE_FOURCC, sizeof(vx_fourcc)},
    {_STR2(FOURCC_YUV4), VX_TYPE_FOURCC, sizeof(vx_fourcc)},
    {_STR2(FOURCC_U8), VX_TYPE_FOURCC, sizeof(vx_fourcc)},
    {_STR2(FOURCC_U16), VX_TYPE_FOURCC, sizeof(vx_fourcc)},
    {_STR2(FOURCC_S16), VX_TYPE_FOURCC, sizeof(vx_fourcc)},
    {_STR2(FOURCC_U32), VX_TYPE_FOURCC, sizeof(vx_fourcc)},
    {_STR2(FOURCC_S32), VX_TYPE_FOURCC, sizeof(vx_fourcc)},
    {"UNKNOWN", 0, VX_TYPE_ENUM, sizeof(vx_enum)}
};

vx_type_name_t direction_names[] = {
    {_STR2(VX_INPUT), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_OUTPUT), VX_TYPE_ENUM, sizeof(vx_enum)},
    {_STR2(VX_BIDIRECTIONAL), VX_TYPE_ENUM, sizeof(vx_enum)},
};

static void vxPrintAllLog(vx_context context)
{
    vx_char entry[VX_MAX_LOG_MESSAGE_LEN];
    vx_status status = VX_SUCCESS;
    do
    {
        status = vxGetLogEntry(context, entry);
        if (status != VX_SUCCESS)
        {
            printf("[%d] %s", status, entry);
        }
    } while (status != VX_SUCCESS);
}

int main(int argc, char *argv[])
{
    vx_status status = VX_SUCCESS;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_graph graph = vxCreateGraph(context);
        vx_char implementation[VX_MAX_IMPLEMENTATION_NAME];
        vx_char *extensions = NULL;
        vx_int32 m, modules = 0;
        vx_uint32 t, targets = 0;
        vx_uint32 k, kernels = 0;
        vx_uint32 p, parameters = 0;
        vx_uint32 a = 0;
        vx_uint16 vendor, version;
        vx_size size = 0;

        // take each arg as a module name to load
        for (m = 1; m < argc; m++)
        {
            if (vxLoadKernels(context, argv[m]) != VX_SUCCESS)
                printf("Failed to load module %s\n", argv[m]);
            else
                printf("Loaded module %s\n", argv[m]);
        }

        vxPrintAllLog(context);

        vxQueryContext(context, VX_CONTEXT_ATTRIBUTE_VENDOR_ID, &vendor, sizeof(vendor));
        vxQueryContext(context, VX_CONTEXT_ATTRIBUTE_VERSION, &version, sizeof(version));
        vxQueryContext(context, VX_CONTEXT_ATTRIBUTE_IMPLEMENTATION, implementation, sizeof(implementation));
        vxQueryContext(context, VX_CONTEXT_ATTRIBUTE_NUMMODULES, &modules, sizeof(modules));
        vxQueryContext(context, VX_CONTEXT_ATTRIBUTE_NUMTARGETS, &targets, sizeof(targets));
        vxQueryContext(context, VX_CONTEXT_ATTRIBUTE_EXTENSIONS_SIZE, &size, sizeof(size));
        printf("implementation=%s (%02x:%02x) has %u targets and %u modules\n", implementation, vendor, version, targets, modules);
        extensions = malloc(size);
        if (extensions)
        {
            vx_char *line = extensions, *token = NULL;
            vxQueryContext(context, VX_CONTEXT_ATTRIBUTE_EXTENSIONS, extensions, size);
            do {
                token = strtok(line, " ");
                if (token)
                    printf("extension: %s\n", token);
                line = NULL;
            } while (token);
            free(extensions);
        }

        for (t = 0; t < targets; t++)
        {
            vx_char targetname[VX_MAX_TARGET_NAME];
            vx_target target = vxGetTargetByIndex(context, t);
            vx_target_kernel_t *table = NULL;
            vxQueryTarget(target, VX_TARGET_ATTRIBUTE_NAME, targetname, sizeof(targetname));
            vxQueryTarget(target, VX_TARGET_ATTRIBUTE_NUMKERNELS, &kernels, sizeof(kernels));
            printf("\ttarget[%u]=%s has %u kernels\n", t, targetname, kernels);
            table = calloc(kernels, sizeof(*table));
            vxQueryTarget(target, VX_TARGET_ATTRIBUTE_KERNELTABLE, table, sizeof(*table)*kernels);
            for (k = 0; k < kernels && table != NULL; k++)
            {
                vx_kernel kernel = vxGetKernelByName(context, table[k].name);
                if (kernel)
                {
                    vx_node node = 0;
                    status = vxQueryKernel(kernel, VX_KERNEL_ATTRIBUTE_NUMPARAMS, &parameters, sizeof(parameters));
                    printf("\t\tkernel[%u]=%s has %u parameters (%d)\n",
                            table[k].enumeration,
                            table[k].name,
                            parameters,
                            status);
                    node = vxCreateNode(graph, kernel);
                    for (p = 0; p < parameters; p++)
                    {
                        vx_parameter parameter = vxGetParameterByIndex(node, p);
                        vx_enum type = VX_TYPE_INVALID, dir = VX_INPUT;
                        vx_uint32 tIdx, dIdx;
                        status = VX_SUCCESS;
                        status |= vxQueryParameter(parameter, VX_PARAMETER_ATTRIBUTE_TYPE, &type, sizeof(type));
                        status |= vxQueryParameter(parameter, VX_PARAMETER_ATTRIBUTE_DIRECTION, &dir, sizeof(dir));
                        for (tIdx = 0; tIdx < dimof(parameter_names); tIdx++)
                            if (parameter_names[tIdx].tenum == type)
                                break;
                        for (dIdx = 0; dIdx < dimof(direction_names); dIdx++)
                            if (direction_names[dIdx].tenum == dir)
                                break;
                        if (status == VX_SUCCESS)
                            printf("\t\t\tparameter[%u] type:%s dir:%s\n", p,
                                parameter_names[tIdx].name,
                                direction_names[dIdx].name);
                    }
                    vxReleaseNode(&node);

                    for (a = 0; a < dimof(attribute_names); a++)
                    {
                        switch (attribute_names[a].type)
                        {
                            case VX_TYPE_SIZE:
                            {
                                vx_size value = 0;
                                if (VX_SUCCESS == vxQueryKernel(kernel, attribute_names[a].tenum, &value, sizeof(value)))
                                    printf("\t\t\tattribute[%u] %s = "VX_FMT_SIZE"\n",
                                        attribute_names[a].tenum & VX_ATTRIBUTE_ID_MASK,
                                        attribute_names[a].name,
                                        value);
                                break;
                            }
                            case VX_TYPE_UINT32:
                            {
                                vx_uint32 value = 0;
                                if (VX_SUCCESS == vxQueryKernel(kernel, attribute_names[a].tenum, &value, sizeof(value)))
                                    printf("\t\t\tattribute[%u] %s = %u\n",
                                        attribute_names[a].tenum & VX_ATTRIBUTE_ID_MASK,
                                        attribute_names[a].name,
                                        value);
                                break;
                            }
                            default:
                                break;
                        }
                    }
                    vxReleaseKernel(&kernel);
                }
                else
                {
                    printf("ERROR: target kernel %s is invalid!\n", table[k].name);
                }
            }
            vxReleaseTarget(&target);
            free(table);
        }
        vxReleaseGraph(&graph);
        vxReleaseContext(&context);
    }
    return 0;
}

