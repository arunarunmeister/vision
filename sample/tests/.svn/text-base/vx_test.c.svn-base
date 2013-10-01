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
 * \brief The OpenVX implementation unit test code.
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vxu.h>

#include <VX/vx_ext_debug.h>
#include <VX/vx_ext_extras.h>
#include <VX/vx_ext_xyz.h>

#ifdef OPENVX_USE_NODE_MEMORY
#include <VX/vx_khr_node_memory.h>
#endif
#ifdef OPENVX_USE_LIST
#include <VX/vx_khr_list.h>
#endif

#include <VX/vx_helper.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <assert.h>

#define VX_KERNEL_FAKE_MAX  (VX_KERNEL_CHANNEL_EXTRACT) // supposed to be VX_KERNEL_MAX but until all the kernels are implemented, this will be used.

/*
 * We unfortunately have to use different macros at the call-level for
 * empty and non-empty argument lists, as C99 syntax mandates presence
 * of the first argument for variable-argument lists (as opposed to e.g.
 * the GNU C extension). This is worked around in the shared
 * implementation by "pasting" an empty string argument.
 */
#define VALARM(message, ...)  printf("[%s:%u] " message "\n", __FUNCTION__, __LINE__, __VA_ARGS__)
#define ALARM(message) VALARM("%s", message)
#define VFAIL(label, message, ...) do { \
    VALARM(message, __VA_ARGS__); \
    if (status == VX_SUCCESS) \
        status = VX_FAILURE; \
    goto label; \
 } while (0)
#define FAIL(label, message) VFAIL(label, message "%s", "")

#define CHECK_ALL_ITEMS(array, iter, status, label) { \
    status = VX_SUCCESS; \
    for ((iter) = 0; (iter) < dimof(array); (iter)++) { \
        if ((array)[(iter)] == 0) { \
            printf("Item %u in "#array" is null!\n", (iter)); \
            assert((array)[(iter)] != 0); \
            status = VX_ERROR_NOT_SUFFICIENT; \
        } \
    } \
    if (status != VX_SUCCESS) { \
        goto label; \
    } \
}

/*! \brief A local definition to point to a specific unit test */
typedef vx_status (*vx_unittest_f)(int argc, char *argv[]);

/*! \brief The structure which correlates each unit test with a result and a name. */
typedef struct _vx_unittest_t {
    vx_status status;
    vx_char name[VX_MAX_KERNEL_NAME];
    vx_unittest_f unittest;
} vx_unittest;

void vx_print_addressing(void *ptr, vx_imagepatch_addressing_t *addr)
{
    printf("ptr=%p dim={%u,%u} stride={%d,%d} scale={%u,%u} step={%u,%u}\n",
            ptr,
            addr->dim_x, addr->dim_y,
            addr->stride_x, addr->stride_y,
            addr->scale_x, addr->scale_y,
            addr->step_x, addr->step_y);
}

static void vx_print_log(vx_graph graph)
{
    char message[VX_MAX_LOG_MESSAGE_LEN];
    vx_uint32 errnum = 1;
    vx_status status = VX_SUCCESS;
    do {
        status = vxGetLogEntry(graph, message);
        if (status != VX_SUCCESS)
            printf("[%05u] error=%d %s", errnum++, status, message);
    } while (status != VX_SUCCESS);
}

#define NUM_BUFS (2)
#define NUM_IMGS (2)
#define BUF_SIZE (1024)
#define PLANES (4)
#define HEIGHT (480)
#define WIDTH (640)
#define CHANNELS (1)

uint8_t images[NUM_IMGS][PLANES][HEIGHT][WIDTH][CHANNELS];
uint8_t buffers[NUM_BUFS][BUF_SIZE];

//**********************************************************************
//
//**********************************************************************

/*!
 * \brief Test creating and releasing a kernel in a node.
 * \ingroup group_tests
 */
vx_status vx_test_framework_load_extension(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        status = vxLoadKernels(context, "xyz");
        if (status == VX_SUCCESS)
        {
            vx_kernel kernel = vxGetKernelByName(context, "org.khronos.example.xyz");
            if (kernel)
            {
                vx_uint32 num_kernels = 0;
                vx_uint32 num_modules = 0;
                vx_uint32 num_references = 0;
                status = VX_SUCCESS;
                if (vxQueryContext(context, VX_CONTEXT_ATTRIBUTE_NUMKERNELS, &num_kernels, sizeof(vx_uint32)) != VX_SUCCESS)
                    status = VX_ERROR_NOT_SUFFICIENT;
                if (vxQueryContext(context, VX_CONTEXT_ATTRIBUTE_NUMMODULES, &num_modules, sizeof(vx_uint32)) != VX_SUCCESS)
                    status = VX_ERROR_NOT_SUFFICIENT;
                if (vxQueryContext(context, VX_CONTEXT_ATTRIBUTE_NUMREFS, &num_references, sizeof(vx_uint32)) != VX_SUCCESS)
                    status = VX_ERROR_NOT_SUFFICIENT;
                printf("[VX_TEST] Kernels:%u Modules:%u Refs:%u\n", num_kernels, num_modules, num_references);
                vxReleaseKernel(&kernel);
                status = VX_SUCCESS;
            }
        }
        vxReleaseContext(&context);
    }
    return status;
}

/*!
 * \brief Loaded Kernel Node Creation Test
 */
vx_status vx_test_framework_load_kernel_node(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        status = vxLoadKernels(context, "xyz");
        if (status == VX_SUCCESS)
        {
            vx_kernel kernel = vxGetKernelByName(context, "org.khronos.example.xyz");
            if (kernel)
            {
                vx_graph graph = vxCreateGraph(context);
                if (graph)
                {
                    vx_image input = vxCreateImage(context, 640, 480, FOURCC_U8);
                    vx_image output = vxCreateImage(context, 640, 480, FOURCC_U8);
                    vx_buffer temp = vxCreateBuffer(context, 10, sizeof(vx_uint8));
                    // we're also testing the "set parameter inside the wrapper. It will check for 0 refs.
                    vx_node xyz = vxXYZNode(graph, input, 10, output, temp);
                    if (xyz)
                    {
                        vx_size size = 0;
                        void *ptr = NULL;
                        status = vxQueryNode(xyz, VX_NODE_ATTRIBUTE_LOCAL_DATA_SIZE, &size, sizeof(size));
                        status = vxQueryNode(xyz, VX_NODE_ATTRIBUTE_LOCAL_DATA_PTR, &ptr, sizeof(ptr));
                        if (status == VX_SUCCESS && size == XYZ_DATA_AREA && ptr != NULL)
                        {
                            printf("Node private size="VX_FMT_SIZE" ptr=%p\n",size,ptr);
                            status = VX_SUCCESS;
                        }
                        vxReleaseNode(&xyz);
                    }
                    vx_print_log(graph);
                    vxReleaseGraph(&graph);
                }
                vxReleaseKernel(&kernel);
            }
        }
        vxReleaseContext(&context);
    }
    return status;
}

/*!
 * \brief Test creating and releasing a kernel in a node.
 * \ingroup group_tests
 */
vx_status vx_test_framework_copy(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_graph graph = vxCreateGraph(context);
        if (graph)
        {
            vx_uint32 width = 640;
            vx_uint32 height = 480;
            vx_image input = vxCreateImage(context, width, height, FOURCC_U8);
            vx_image output = vxCreateImage(context, width, height, FOURCC_U8);
            vx_node node = 0;
            vx_uint32 errors = 0u;

            if (input == 0 || output == 0) {
                ALARM("failed to create images");
                return VX_ERROR_NOT_SUPPORTED;
            }
            status |= vxLoadKernels(context, "openvx-debug");
            node = vxCopyImageNode(graph, input, output);
            if (node)
            {
                vx_uint32 numNodes = 0;
                if (vxQueryGraph(graph, VX_GRAPH_ATTRIBUTE_NUMNODES, &numNodes, sizeof(numNodes)) != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
                else
                {
                    if (numNodes != 1)
                    {
                        status = VX_ERROR_NOT_SUFFICIENT;
                    }
                    else
                    {
                        status = vxVerifyGraph(graph);
                        if (status == VX_SUCCESS)
                        {
                            status |= vxuFillImage(0x42, input);

                            // actually run the graph
                            status |= vxProcessGraph(graph);
                            if (status != VX_SUCCESS)
                            {
                                ALARM("Failed to Process Graph!");
                            }
                            else
                            {
                                status = vxuCheckImage(output, 0x42, &errors);
                                if (status != VX_SUCCESS || errors > 0)
                                {
                                    ALARM("Image Data was not copied");
                                }
                            }
                        }
                        else
                        {
                            ALARM("Failed Graph Verification");
                        }
                    }
                }
                vxReleaseNode(&node);
            }
            else
            {
                ALARM("failed to create node");
                status = VX_ERROR_NOT_SUFFICIENT;
                goto exit;
            }
            vx_print_log(graph);
            vxReleaseGraph(&graph);
        }
        else
        {
            ALARM("failed to create graph");
            status = VX_ERROR_NOT_SUFFICIENT;
            goto exit;
        }
exit:
        vxReleaseContext(&context);
    }
    return status;
}

/*!
 * \brief Test creating and releasing a copy node with virtual object output.
 * \ingroup group_tests
 */
vx_status vx_test_framework_copy_virtual(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_graph graph = vxCreateGraph(context);
        if (graph)
        {
            vx_uint32 width = 640;
            vx_uint32 height = 480;
            vx_image input = vxCreateImage(context, width, height, FOURCC_U8);
            vx_image output = vxCreateVirtualImage(context);
            vx_node node = 0;
            vx_uint32 errors = 0u;

            if (input == 0 || output == 0)
            {
                ALARM("failed to create images");
                return VX_ERROR_NOT_SUPPORTED;
            }
            status |= vxLoadKernels(context, "openvx-debug");
            node = vxCopyImageNode(graph, input, output);
            if (node)
            {
                vx_uint32 numNodes = 0;
                if (vxQueryGraph(graph, VX_GRAPH_ATTRIBUTE_NUMNODES, &numNodes, sizeof(numNodes)) != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
                else
                {
                    if (numNodes != 1)
                    {
                        status = VX_ERROR_NOT_SUFFICIENT;
                    }
                    else
                    {
                        status = vxVerifyGraph(graph);
                        if (status == VX_SUCCESS)
                        {
                            status |= vxuFillImage(0x42, input);

                            // actually run the graph
                            status |= vxProcessGraph(graph);
                            if (status != VX_SUCCESS)
                            {
                                ALARM("Failed to Process Graph!");
                            }
                            else
                            {
                                status = vxuCheckImage(output, 0x42, &errors);
                                if (status != VX_SUCCESS || errors > 0)
                                {
                                    ALARM("Image Data was not copied");
                                    assert(errors == 0);
                                }
                            }
                        }
                        else
                        {
                            ALARM("Failed Graph Verification");
                        }
                    }
                }
                vxReleaseNode(&node);
            }
            else
            {
                ALARM("failed to create node");
                status = VX_ERROR_NOT_SUFFICIENT;
                goto exit;
            }
            vxReleaseImage(&input);
            vxReleaseImage(&output);
            vxReleaseGraph(&graph);
        }
        else
        {
            ALARM("failed to create graph");
            status = VX_ERROR_NOT_SUFFICIENT;
            goto exit;
        }
exit:
        vxReleaseContext(&context);
    }
    return status;
}

/*!
 * \brief Test creating 2 nodes with a virtual image between inputs
 * and outputs.
 * \ingroup group_tests
 */
vx_status vx_test_framework_virtualimage(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_graph graph = vxCreateGraph(context);
        if (graph)
        {
            vx_uint32 width = 640u;
            vx_uint32 height = 480u;
            vx_uint32 errors = 0u;
            vx_image input = vxCreateImage(context, width, height, FOURCC_U8);
            vx_image virt = vxCreateVirtualImage(context);
            vx_image output = vxCreateImage(context, width, height, FOURCC_U8);
            vx_node nodes[2];

            if (input == 0 || output == 0 || virt == 0) {
                ALARM("failed to create images");
                return VX_ERROR_NOT_SUPPORTED;
            }
            status |= vxLoadKernels(context, "openvx-debug");
            nodes[0] = vxCopyImageNode(graph, input, virt);
            nodes[1] = vxCopyImageNode(graph, virt, output);

            if (nodes[0] && nodes[1])
            {
                status = vxVerifyGraph(graph);
                if (status == VX_SUCCESS)
                {
                    vxuFillImage(0x42, input);

                    status = vxProcessGraph(graph);
                    if (status != VX_SUCCESS)
                    {
                        ALARM("Failed to Process Graph!");
                    }
                    else
                    {
                        status = vxuCheckImage(output, 0x42, &errors);
                        if (status != VX_SUCCESS)
                        {
                            ALARM("Image Data was not copied");
                        }
                    }
                }
                else
                {
                    ALARM("Failed Graph Verification");
                }
                vxReleaseNode(&nodes[0]);
                vxReleaseNode(&nodes[1]);
            }
            else
            {
                ALARM("failed to create node");
                status = VX_ERROR_NOT_SUFFICIENT;
                goto exit;
            }
            vx_print_log(graph);
            vxReleaseGraph(&graph);
        }
        else
        {
            ALARM("failed to create graph");
            status = VX_ERROR_NOT_SUFFICIENT;
            goto exit;
        }
exit:
        vxReleaseContext(&context);
    }
    return status;
}


/*!
 * \brief Test creating and releasing a kernel in a node.
 * \ingroup group_tests
 */
vx_status vx_test_framework_heads(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_graph graph = vxCreateGraph(context);
        if (graph)
        {
            vx_uint32 width = 640;
            vx_uint32 height = 480;
            vx_image input = vxCreateImage(context, width, height, FOURCC_U8);
            vx_image output = vxCreateImage(context, width, height, FOURCC_U8);
            vx_node nodes[2];

            if (input == 0 || output == 0)
            {
                ALARM("failed to create images");
                return VX_ERROR_NOT_SUPPORTED;
            }
            status |= vxLoadKernels(context, "openvx-debug");
            nodes[0] = vxCopyImageNode(graph, input, output);
            nodes[1] = vxCopyImageNode(graph, output, input);
            if (nodes[0] && nodes[1])
            {
                status = vxVerifyGraph(graph);
                if (status == VX_ERROR_INVALID_GRAPH)
                {
                    // we're trying to detect an error case.
                    status = VX_SUCCESS;
                }
                else
                {
                    ALARM("Failed to detect cycle in graph!");
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
                vxReleaseNode(&nodes[0]);
                vxReleaseNode(&nodes[1]);
            }
            else
            {
                ALARM("failed to create node");
                status = VX_ERROR_NOT_SUFFICIENT;
                goto exit;
            }
            vx_print_log(graph);
            vxReleaseGraph(&graph);
        }
        else
        {
            ALARM("failed to create graph");
            status = VX_ERROR_NOT_SUFFICIENT;
            goto exit;
        }
exit:
        vxReleaseContext(&context);
    }
    return status;
}


/*!
 * \brief Test creating and releasing a kernel in a node.
 * \ingroup group_tests
 */
vx_status vx_test_framework_unvisited(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_graph graph = vxCreateGraph(context);
        if (graph)
        {
            vx_uint32 width = 640;
            vx_uint32 height = 480;
            vx_buffer buf_in = vxCreateBuffer(context, 10, 10);
            vx_buffer buf_out = vxCreateBuffer(context, 10, 10);
            vx_image input = vxCreateImage(context, width, height, FOURCC_U8);
            vx_image output = vxCreateImage(context, width, height, FOURCC_U8);
            vx_node nodes[3];

            if (input == 0 || output == 0)
            {
                ALARM("failed to create images");
                return VX_ERROR_NOT_SUPPORTED;
            }
            status |= vxLoadKernels(context, "openvx-debug");
            nodes[0] = vxCopyImageNode(graph, input, output);
            nodes[1] = vxCopyImageNode(graph, output, input);
            nodes[2] = vxCopyBufferNode(graph, buf_in, buf_out);
            if (nodes[0] && nodes[1] && nodes[2])
            {
                status = vxVerifyGraph(graph);
                if (status == VX_ERROR_INVALID_GRAPH)
                {
                    // we're trying to detect an error case.
                    status = VX_SUCCESS;
                }
                else
                {
                    ALARM("Failed to detect cycle in graph!");
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
                vxReleaseNode(&nodes[0]);
                vxReleaseNode(&nodes[1]);
                vxReleaseNode(&nodes[2]);
            }
            else
            {
                ALARM("failed to create node");
                status = VX_ERROR_NOT_SUFFICIENT;
                goto exit;
            }
            vx_print_log(graph);
            vxReleaseGraph(&graph);
        }
        else
        {
            ALARM("failed to create graph");
            status = VX_ERROR_NOT_SUFFICIENT;
            goto exit;
        }
exit:
        vxReleaseContext(&context);
    }
    return status;
}

vx_status vx_test_framework_targets(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_uint32 numTargets = 0;
        vx_char implementation[VX_MAX_IMPLEMENTATION_NAME];

        status = vxQueryContext(context, VX_CONTEXT_ATTRIBUTE_IMPLEMENTATION, implementation, sizeof(implementation));
        if (status == VX_SUCCESS)
        {
            printf("[VX_TEST] Implementation: %s\n", implementation);
        }
        else
        {
            return status;
        }
        // this should show up in the kernel list
        status = vxLoadKernels(context, "xyz");
        if (status != VX_SUCCESS)
        {
            return status;
        }
        status = vxQueryContext(context, VX_CONTEXT_ATTRIBUTE_NUMTARGETS, &numTargets, sizeof(numTargets));
        if (status == VX_SUCCESS && numTargets > 0)
        {
            vx_uint32 t = 0;
            for (t = 0; t < numTargets; t++)
            {
                vx_target target = vxGetTargetByIndex(context, t);
                if (target)
                {
                    vx_char targetname[VX_MAX_TARGET_NAME];
                    vx_uint32 numKernels = 0;

                    status = vxQueryTarget(target, VX_TARGET_ATTRIBUTE_NAME, targetname, sizeof(targetname));
                    if (status != VX_SUCCESS)
                        return status;

                    status = vxQueryTarget(target, VX_TARGET_ATTRIBUTE_NUMKERNELS, &numKernels, sizeof(numKernels));
                    if (status == VX_SUCCESS && numKernels > 0)
                    {
                        vx_target_kernel_t *table = calloc(numKernels, sizeof(vx_target_kernel_t));
                        status = vxQueryTarget(target, VX_TARGET_ATTRIBUTE_KERNELTABLE, table, numKernels * sizeof(vx_target_kernel_t));
                        if (status == VX_SUCCESS)
                        {
                            vx_uint32 k;
                            for (k = 0; k < numKernels; k++)
                            {
                                printf("[VX_TEST] %s: kernel[%d]=%s\n", targetname, table[k].enumeration, table[k].name);
                            }
                        }
                    }
                }
                else
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
            }
        }
        vxReleaseContext(&context);
    }
    return status;
}

/*!
 * \brief Tests delay object creation.
 * \ingroup group_tests
 */
vx_status vx_test_framework_delay_graph(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_uint32 w = 640, h = 480;
    vx_uint32 errors = 0u;
    vx_fourcc f = FOURCC_U8;
    vx_uint32 i;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_graph graph = vxCreateGraph(context);
        if (graph)
        {
#define DEPTH_TEST (3)
            vx_image images[DEPTH_TEST];
            vx_delay delay = vxCreateImageDelay(context, w, h, f, DEPTH_TEST-1);
            vx_node nodes[DEPTH_TEST];

            memset(images, 0, sizeof(images));
            memset(nodes, 0, sizeof(nodes));

            for (i = 0; i < DEPTH_TEST; i++)
            {
                images[i] = vxCreateImage(context, w, h, f);
                if (images[i] == 0)
                {
                    status = VX_ERROR_NO_MEMORY;
                    goto exit;
                }
            }
            status |= vxLoadKernels(context, "openvx-debug");
            nodes[0] = vxCopyImageNode(graph, images[0], vxGetImageFromDelay(delay, 0));
            nodes[1] = vxCopyImageNode(graph, vxGetImageFromDelay(delay, -1), images[1]);
            nodes[2] = vxCopyImageNode(graph, vxGetImageFromDelay(delay, -1), images[2]);

            for (i = 0; i < dimof(nodes); i++)
            {
                if (nodes[i] == 0)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    goto exit;
                }
            }
            // setup the binding for auto-aging
            status = vxAssociateDelayWithNode(delay, 0, nodes[0], 1, VX_OUTPUT);
            if (status != VX_SUCCESS)
            {
                goto exit;
            }
            status = vxAssociateDelayWithNode(delay, -1, nodes[1], 0, VX_INPUT);
            if (status != VX_SUCCESS)
            {
                goto exit;
            }
            status = vxAssociateDelayWithNode(delay, -1, nodes[2], 0, VX_INPUT);
            if (status != VX_SUCCESS)
            {
                goto exit;
            }
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                vxuFillImage(0xBE, images[0]);
                vxuFillImage(0x00, images[1]);
                vxuFillImage(0x00, images[2]);
                vxuFillImage(0x00, vxGetImageFromDelay(delay, -1));
                vxuFillImage(0x00, vxGetImageFromDelay(delay, 0));

                status = vxProcessGraph(graph);
                if (status != VX_SUCCESS)
                {
                    ALARM("Failed to process graph!\n");
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
                else
                {
                    if (vxuCheckImage(vxGetImageFromDelay(delay, 0), 0xBE, &errors) == VX_SUCCESS &&
                        vxuCheckImage(vxGetImageFromDelay(delay, -1), 0x00, &errors) == VX_SUCCESS)
                    {
                        status = vxAgeDelay(delay);
                        if (status == VX_SUCCESS &&
                            vxuCheckImage(vxGetImageFromDelay(delay, 0), 0x00, &errors) == VX_SUCCESS &&
                            vxuCheckImage(vxGetImageFromDelay(delay, -1), 0xBE, &errors) == VX_SUCCESS)
                        {
                            status = vxProcessGraph(graph);
                            if (status == VX_SUCCESS)
                            {
                                if (vxuCheckImage(images[1], 0xBE, &errors) == VX_SUCCESS &&
                                    vxuCheckImage(images[2], 0xBE, &errors) == VX_SUCCESS)
                                {
                                    ALARM("Passed!");
                                    status = VX_SUCCESS;
                                }
                            }
                            else
                            {
                                ALARM("Failed to process graph!\n");
                                status = VX_ERROR_NOT_SUFFICIENT;
                            }
                        }
                        else
                        {

                            ALARM("Aging did not work correctly!\n");
                            status = VX_ERROR_NOT_SUFFICIENT;
                        }
                    }
                    else
                    {
                        status = VX_ERROR_NOT_SUFFICIENT;
                    }
                }
            }
exit:
            for (i = 0; i < DEPTH_TEST; i++)
            {
                vxReleaseNode(&nodes[i]);
            }
            vxReleaseDelay(&delay);
            for (i = 0; i < DEPTH_TEST; i++)
            {
                vxReleaseImage(&images[i]);
            }
            vxReleaseGraph(&graph);
        }
        vxReleaseContext(&context);
    }
    return status;
}

/*!
 * \brief Test usage of the asynchronous interfaces.
 * \ingroup group_tests
 */
vx_status vx_test_framework_async(int argc, char *argv[])
{
    vx_status s1,s2,status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_uint32 w = 320, h = 240;
        vx_uint8 a1 = 0x5A, a2 = 0xA5;
        vx_uint32 i = 0;
        vx_uint32 errors = 0u;
        vx_image images[4] = {
                vxCreateImage(context, w, h, FOURCC_U8),
                vxCreateImage(context, w, h, FOURCC_U8),
                vxCreateImage(context, w, h, FOURCC_U8),
                vxCreateImage(context, w, h, FOURCC_U8),
        };
        vx_graph g1 = vxCreateGraph(context);
        vx_graph g2 = vxCreateGraph(context);
        status |= vxLoadKernels(context, "openvx-debug");
        if (g1 && g2 && status == VX_SUCCESS)
        {
            vx_node nodes[] = {
                vxCopyImageNode(g1, images[0], images[1]),
                vxCopyImageNode(g2, images[2], images[3]),
            };
            CHECK_ALL_ITEMS(nodes, i, status, exit);
            s1 = vxVerifyGraph(g1);
            s2 = vxVerifyGraph(g2);
            printf("s1=%d, s2=%d\n",s1,s2);
            if (s1 == VX_SUCCESS && s2 == VX_SUCCESS)
            {
                vxuFillImage(a1, images[0]);
                vxuFillImage(a2, images[2]);

                s1 = vxScheduleGraph(g1);
                s2 = vxScheduleGraph(g2);
                printf("s1=%d, s2=%d\n",s1,s2);

                if (s1 == VX_SUCCESS && s2 == VX_SUCCESS)
                {
                    s2 = vxWaitGraph(g2);
                    s1 = vxWaitGraph(g1);
                    printf("s1=%d, s2=%d\n",s1,s2);
                    if ((s1 != VX_SUCCESS) || (s2 != VX_SUCCESS))
                    {
                        status = VX_ERROR_NOT_SUFFICIENT;
                    }
                    if ((vxuCheckImage(images[1], a1, &errors) == VX_SUCCESS) &&
                        (vxuCheckImage(images[3], a2, &errors) == VX_SUCCESS))
                    {
                        status = VX_SUCCESS;
                    }
                }
            }
            printf("s1=%d, s2=%d\n",s1,s2);
exit:
            vxReleaseGraph(&g1);
            vxReleaseGraph(&g2);
        }
        vxReleaseContext(&context);
    }
    return status;
}

/*!
 * \brief Test calling a direct copy.
 * \ingroup group_tests
 */
vx_status vx_test_direct_copy_image(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_imagepatch_addressing_t addrs[] = {
                {WIDTH, HEIGHT, sizeof(vx_uint8), WIDTH * sizeof(vx_uint8), VX_SCALE_UNITY, VX_SCALE_UNITY, 1, 1},
                {WIDTH, HEIGHT, sizeof(vx_uint8), WIDTH * sizeof(vx_uint8), VX_SCALE_UNITY, VX_SCALE_UNITY, 1, 1},
                {WIDTH, HEIGHT, sizeof(vx_uint8), WIDTH * sizeof(vx_uint8), VX_SCALE_UNITY, VX_SCALE_UNITY, 1, 1},
        };
        void *src_ptrs[] = {&images[0][0][0][0][0],
                            &images[0][1][0][0][0],
                            &images[0][2][0][0][0]
        };
        void *dst_ptrs[] = {&images[1][0][0][0][0],
                            &images[1][1][0][0][0],
                            &images[1][2][0][0][0]
        };
        vx_image input = vxCreateImageFromHandle(context, FOURCC_YUV4, addrs, src_ptrs, VX_IMPORT_TYPE_HOST);
        vx_image output = vxCreateImageFromHandle(context, FOURCC_YUV4, addrs, dst_ptrs, VX_IMPORT_TYPE_HOST);
        if (input && output)
        {
            vx_uint32 numDiffs = 0;
            status = vxLoadKernels(context, "openvx-debug");
            status |= vxuFillImage(0x42, input);
            assert(status == VX_SUCCESS);
            status |= vxuFillImage(0x00, output);
            assert(status == VX_SUCCESS);
            status |= vxuCopyImage(input, output);
            assert(status == VX_SUCCESS);
            if (status == VX_SUCCESS)
                status = vxuCompareImages(input, output, &numDiffs);
            vxReleaseImage(&input);
            vxReleaseImage(&output);
        }
        vxReleaseContext(&context);
    }
    return status;
}

/*!
 * \brief Test calling a direct copy node.
 * \ingroup group_tests
 */
vx_status vx_test_direct_copy_external_image(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
       vx_uint32 width = 640;
        vx_uint32 height = 480;
        vx_image input = vxCreateImage(context, width, height, FOURCC_RGB);
        vx_image output = vxCreateImage(context, width, height, FOURCC_RGB);
        if (input && output)
        {
            vx_uint32 value = 0x225533;
            vx_uint32 errors = 0xFFFFFFFF;
            status = vxLoadKernels(context, "openvx-debug");
            assert(status == VX_SUCCESS);
            status |= vxuFillImage(value, input);
            assert(status == VX_SUCCESS);
            status |= vxuFillImage(0x00, output);
            assert(status == VX_SUCCESS);
            status |= vxuCopyImage(input, output);
            assert(status == VX_SUCCESS);
            status |= vxuCheckImage(output, value, &errors);
            if ((status != VX_SUCCESS) && (errors > 0))
            {
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            vxReleaseImage(&input);
            vxReleaseImage(&output);
        }
        vxReleaseContext(&context);
    }
    return status;
}

/*!
 * \brief Test calling a direct copy node.
 * \ingroup group_tests
 */
vx_status vx_test_direct_copy_external_buffer(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_buffer input = vxCreateBufferFromHandle(context, sizeof(vx_uint8), BUF_SIZE, (void *)&buffers[0][0], VX_IMPORT_TYPE_HOST);
        vx_buffer output = vxCreateBufferFromHandle(context, sizeof(vx_uint8), BUF_SIZE, (void *)&buffers[1][0], VX_IMPORT_TYPE_HOST);
        if (input && output)
        {
            vx_uint32 numDiffs = 0;
            status = vxLoadKernels(context, "openvx-debug");
            assert(status == VX_SUCCESS);
            status |= vxuFillBuffer(0x42, input);
            assert(status == VX_SUCCESS);
            status |= vxuFillBuffer(0x00, output);
            assert(status == VX_SUCCESS);
            status |= vxuCopyBuffer(input, output);
            if (status == VX_SUCCESS)
                status = vxuCheckBuffer(output, 0x42, &numDiffs);
            vxReleaseBuffer(&input);
            vxReleaseBuffer(&output);
        }
        vxReleaseContext(&context);
    }
    return status;
}

/*!
 * \brief Test calling a direct copy.
 * \ingroup group_tests
 */
vx_status vx_test_direct_copy_buffer(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_uint32 len = 1024, errors = 0u;
        vx_image input = vxCreateBuffer(context, sizeof(vx_uint8), len);
        vx_image output = vxCreateBuffer(context, sizeof(vx_uint8), len);
        if ((input) && (output))
        {
            status = VX_SUCCESS;
            status |= vxLoadKernels(context, "openvx-debug");
            status |= vxuFillBuffer(0x42, input);
            status |= vxuFillBuffer(0x00, output);
            status |= vxuCopyBuffer(input, output);
            status |= vxuCheckBuffer(output, 0x42, &errors);
            if ((status != VX_SUCCESS) && (errors > 0))
            {
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            vxReleaseBuffer(&input);
            vxReleaseBuffer(&output);
        }
        vxReleaseContext(&context);
    }
    return status;
}

vx_status vx_test_graph_channels_yuv(int argc, char *argv[])
{
    vx_status status = VX_SUCCESS;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_uint32 i = 0, w = 640, h = 480;
        vx_image images[] = {
            vxCreateImage(context, w, h, FOURCC_RGB),    /* 0: rgb */
            vxCreateImage(context, w, h, FOURCC_IYUV),   /* 1: yuv */
            vxCreateImage(context, w, h, FOURCC_U8),     /* 2: luma */
            vxCreateImage(context, w/2, h/2, FOURCC_U8), /* 3: u channel */
            vxCreateImage(context, w/2, h/2, FOURCC_U8), /* 4: v channel */
        };
        CHECK_ALL_ITEMS(images, i, status, exit);
        status |= vxLoadKernels(context, "openvx-debug");
        if (status == VX_SUCCESS)
        {
            vx_graph graph = vxCreateGraph(context);
            if (graph)
            {
                vx_node nodes[] = {
                    vxFReadImageNode(graph, "colorbars_640x480_I444.rgb", images[0]),
                    vxColorConvertNode(graph, images[0], images[1]),
                    vxFWriteImageNode(graph, images[1], "oiyuv_640x480_P420.yuv"),
                    vxChannelExtractNode(graph, images[1], VX_CHANNEL_Y, images[2]),
                    vxChannelExtractNode(graph, images[1], VX_CHANNEL_U, images[3]),
                    vxChannelExtractNode(graph, images[1], VX_CHANNEL_V, images[4]),
                    vxFWriteImageNode(graph, images[2], "oy_640x480_P400.bw"),
                    vxFWriteImageNode(graph, images[3], "ou_320x240_P400.bw"),
                    vxFWriteImageNode(graph, images[4], "ov_320x240_P400.bw"),
                };
                CHECK_ALL_ITEMS(nodes, i, status, exit);
                if (status == VX_SUCCESS)
                {
                    status = vxVerifyGraph(graph);
                    if (status == VX_SUCCESS)
                    {
                        status = vxProcessGraph(graph);
                    }
                }
                vxReleaseGraph(&graph);
            }
        }
        for (i = 0; i < dimof(images); i++)
        {
            vxReleaseImage(&images[i]);
        }
exit:
        vxReleaseContext(&context);
    }
    return status;
}

vx_status vx_test_graph_bikegray(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_uint32 i = 0, w = 640, h = 480, w2 = 300, h2 = 200;
        vx_uint32 range = 256, windows = 16;
        vx_image images[] = {
            vxCreateImage(context, w, h, FOURCC_U8),                /* 0: luma */
            vxCreateImage(context, w2, h2, FOURCC_U8),              /* 1: scaled */
            vxCreateVirtualImageWithFormat(context, FOURCC_S16),    /* 2: grad_x */
            vxCreateVirtualImageWithFormat(context, FOURCC_S16),    /* 3: grad_y */
            vxCreateImage(context, w2, h2, FOURCC_U8),              /* 4: mag */
            vxCreateImage(context, w2, h2, FOURCC_U8),              /* 5: phase */
            vxCreateImage(context, w2, h2, FOURCC_U8),              /* 6: LUT out */
            vxCreateImage(context, w2, h2, FOURCC_U8),              /* 7: AbsDiff */
            vxCreateImage(context, w2, h2, FOURCC_U8),              /* 8: Threshold */
            vxCreateImage(context, w2, h2, FOURCC_U32),             /* 9: Integral */
            vxCreateImage(context, w2, h2, FOURCC_U8),              /* 10: Eroded */
            vxCreateImage(context, w2, h2, FOURCC_U8),              /* 11: Dilated */
            vxCreateImage(context, w, h, FOURCC_U8),                /* 12: Median */
            vxCreateImage(context, w, h, FOURCC_U8),                /* 13: Box */
            vxCreateImage(context, w2, h2, FOURCC_S16),             /* 14: UpDepth */
            vxCreateImage(context, w, h, FOURCC_U8),                /* 15: Canny */
            vxCreateImage(context, w, h, FOURCC_U8),                /* 16: EqualizeHistogram */
            vxCreateVirtualImageWithFormat(context, FOURCC_U8),     /* 17: DownDepth Gy */
        };
        vx_lut lut = vxCreateLUT(context, VX_TYPE_UINT8, 256);
        vx_uint8 *tmp = NULL;
        vx_int32 *histogram = NULL;
        vx_distribution dist = vxCreateDistribution(context, windows, 0, range);
        vx_float32 mean = 0.0f, stddev = 0.0f;
        vx_scalar s_mean = vxCreateScalar(context, VX_TYPE_FLOAT32, &mean);
        vx_scalar s_stddev = vxCreateScalar(context, VX_TYPE_FLOAT32, &stddev);
        vx_threshold thresh = vxCreateThreshold(context, VX_THRESHOLD_TYPE_BINARY);
        vx_uint8 lo = 140;
        vx_scalar minVal = vxCreateScalar(context, VX_TYPE_UINT8, NULL);
        vx_scalar maxVal = vxCreateScalar(context, VX_TYPE_UINT8, NULL);
        vx_coordinates minLoc = vxCreateCoordinates(context, 2);
        vx_coordinates maxLoc = vxCreateCoordinates(context, 2);
        vx_threshold hyst = vxCreateThreshold(context, VX_THRESHOLD_TYPE_RANGE);
        vx_uint8 lower = 40, upper = 250;
        vx_enum policy = VX_CONVERT_POLICY_SATURATE;
        vx_int32 shift = 7;
        vx_scalar sshift = vxCreateScalar(context, VX_TYPE_INT32, &shift);
        vx_int32 noshift = 0;
        vx_scalar snoshift = vxCreateScalar(context, VX_TYPE_INT32, &noshift);

        CHECK_ALL_ITEMS(images, i, status, exit);

        status = vxAccessLUT(lut, (void **)&tmp);
        if (status == VX_SUCCESS)
        {
            for (i = 0; i < range; i++)
            {
                vx_uint32 g = (vx_uint32)pow(i,1/0.93);
                tmp[i] = (vx_uint8)(g >= range ? (range - 1): g);
            }
            status = vxCommitLUT(lut, tmp);
        }

        status |= vxSetThresholdAttribute(thresh, VX_THRESHOLD_ATTRIBUTE_VALUE, &lo, sizeof(lo));
        status |= vxSetThresholdAttribute(hyst, VX_THRESHOLD_ATTRIBUTE_LOWER, &lower, sizeof(lower));
        status |= vxSetThresholdAttribute(hyst, VX_THRESHOLD_ATTRIBUTE_UPPER, &upper, sizeof(upper));
        status |= vxLoadKernels(context, "openvx-debug");
        if (status == VX_SUCCESS)
        {
            vx_graph graph = vxCreateGraph(context);
            if (graph)
            {
                vx_node nodes[] = {
                    vxFReadImageNode(graph, "bikegray_640x480.pgm", images[0]),
                    vxMedian3x3Node(graph, images[0], images[12]),
                    vxBox3x3Node(graph, images[0], images[13]),
                    vxScaleImageNode(graph, images[0], images[1], VX_INTERPOLATION_TYPE_BILINEAR),
                    vxTableLookupNode(graph, images[1], lut, images[6]),
                    vxHistogramNode(graph, images[6], dist),
                    vxSobel3x3Node(graph, images[1], images[2], images[3]),
                    vxMagnitudeNode(graph, images[2], images[3], images[4]),
                    vxPhaseNode(graph, images[2], images[3], images[5]),
                    vxAbsDiffNode(graph, images[6], images[1], images[7]),
                    vxConvertDepthNode(graph, images[7], images[14], policy, sshift),
                    vxMeanStdDevNode(graph, images[7], s_mean, s_stddev),
                    vxThresholdNode(graph, images[6], thresh, images[8]),
                    vxIntegralImageNode(graph, images[7], images[9]),
                    vxErode3x3Node(graph, images[8], images[10]),
                    vxDilate3x3Node(graph, images[8], images[11]),
                    vxMinMaxLocNode(graph, images[7], minVal, maxVal, minLoc, maxLoc),
                    vxCannyEdgeDetectorNode(graph, images[0], hyst, 3, VX_NORM_L1, images[15]),
                    vxEqualizeHistNode(graph, images[0], images[16]),
                    vxConvertDepthNode(graph, images[3], images[17], policy, snoshift),

                    vxFWriteImageNode(graph, images[0], "obikegray_640x480_P400.pgm"),
                    vxFWriteImageNode(graph, images[1], "obikegray_300x200_P400.pgm"),
                    vxFWriteImageNode(graph, images[2], "obikegradh_300x200_P400_-16b.bw"),
                    vxFWriteImageNode(graph, images[3], "obikegradv_300x200_P400_-16b.bw"),
                    vxFWriteImageNode(graph, images[4], "obikemag_300x200_P400.pgm"),
                    vxFWriteImageNode(graph, images[5], "obikeatan_300x200_P400.pgm"),
                    vxFWriteImageNode(graph, images[6], "obikeluty_300x200_P400.pgm"),
                    vxFWriteImageNode(graph, images[7], "obikediff_300x200_P400.pgm"),
                    vxFWriteImageNode(graph, images[8], "obikethsh_300x200_P400.pgm"),
                    vxFWriteImageNode(graph, images[9], "obikesums_600x200_P400_16b.bw"),
                    vxFWriteImageNode(graph, images[10], "obikeerod_300x200_P400.pgm"),
                    vxFWriteImageNode(graph, images[11], "obikedilt_300x200_P400.pgm"),
                    vxFWriteImageNode(graph, images[12], "obikemed_640x480_P400.pgm"),
                    vxFWriteImageNode(graph, images[13], "obikeavg_640x480_P400.pgm"),
                    vxFWriteImageNode(graph, images[14], "obikediff_300x200_P400_16b.bw"),
                    vxFWriteImageNode(graph, images[15], "obikecanny_640x480_P400.pgm"),
                    vxFWriteImageNode(graph, images[16], "obikeeqhist_640x480_P400.pgm"),
                    vxFWriteImageNode(graph, images[17], "obikegrady8_300x200_P400.pgm"),
                };
                CHECK_ALL_ITEMS(nodes, i, status, exit);
                if (status == VX_SUCCESS)
                {
                    status = vxVerifyGraph(graph);
                    if (status == VX_SUCCESS)
                    {
                        status = vxProcessGraph(graph);
                    }
                    else
                    {
                        vx_print_log(graph);
                    }
                    assert(status == VX_SUCCESS);
                    if (status == VX_SUCCESS)
                    {
                        vx_int32 min_l[2];
                        vx_int32 max_l[2];
                        vx_uint8 min_v = 255;
                        vx_uint8 max_v = 0;

                        vxAccessScalarValue(minVal, &min_v);
                        vxAccessScalarValue(maxVal, &max_v);
                        vxAccessCoordinates(minLoc, min_l);
                        vxAccessCoordinates(maxLoc, max_l);
                        printf("Min Value in AbsDiff = %u, at %d,%d\n", min_v, min_l[0], min_l[1]);
                        printf("Max Value in AbsDiff = %u, at %d,%d\n", max_v, max_l[0], max_l[1]);

                        vxAccessDistribution(dist, (void **)&histogram);
                        for (i = 0; i < windows; i++)
                        {
                            printf("histogram[%u] = %d\n", i, histogram[i]);
                        }
                        vxAccessScalarValue(s_mean, &mean);
                        vxAccessScalarValue(s_stddev, &stddev);
                        printf("AbsDiff Mean = %lf\n", mean);
                        printf("AbsDiff Stddev = %lf\n", stddev);
                        vxCommitDistribution(dist, histogram);
                    }
                    else
                    {
                        printf("Graph failed (%d)\n", status);
                        for (i = 0; i < dimof(nodes); i++)
                        {
                            status = VX_SUCCESS;
                            vxQueryNode(nodes[i], VX_NODE_ATTRIBUTE_STATUS, &status, sizeof(status));
                            if (status != VX_SUCCESS)
                            {
                                printf("nodes[%u] failed with %d\n", i, status);
                            }
                        }
                        status = VX_ERROR_NOT_SUFFICIENT;
                    }
                    for (i = 0; i < dimof(nodes); i++)
                    {
                        vxReleaseNode(&nodes[i]);
                    }
                }
                vxReleaseGraph(&graph);
            }
        }
        for (i = 0; i < dimof(images); i++)
        {
            vxReleaseImage(&images[i]);
        }
exit:
        //vx_print_log(context);
        vxReleaseScalar(&sshift);
        vxReleaseScalar(&snoshift);
        vxReleaseContext(&context);
    }
    return status;
}

vx_status vx_test_graph_opencl(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_target target = vxFindTarget(context, "pc.opencl");
        if (target)
        {
            vx_uint32 i = 0, w = 640, h = 480;
            vx_image images[] = {
                vxCreateImage(context, w, h, FOURCC_U8),                /* 0: luma */
                vxCreateImage(context, w, h, FOURCC_U8),                /* 1: Box */
                vxCreateImage(context, w, h, FOURCC_U8),                /* 2: Gaussian */
            };

            status  = VX_SUCCESS;
            status |= vxLoadKernels(context, "openvx-debug");
            if (status == VX_SUCCESS)
            {
                vx_graph graph = vxCreateGraph(context);
                if (graph)
                {
                    vx_node nodes[] = {
                        vxFReadImageNode(graph, "bikegray_640x480.pgm", images[0]),
                        vxBox3x3Node(graph, images[0], images[1]),
                        vxGaussian3x3Node(graph, images[0], images[2]),
                        vxFWriteImageNode(graph, images[1], "obikeavg_640x480_P400.pgm"),
                        vxFWriteImageNode(graph, images[2], "obikegau_640x480_P400.pgm"),
                    };
                    status = vxAssignNode(nodes[1], target);
                    if (status == VX_SUCCESS)
                    {
                        printf("Assigned Box Filter to OpenCL!\n");
                    }
                    CHECK_ALL_ITEMS(nodes, i, status, exit);
                    if (status == VX_SUCCESS)
                    {
                        status = vxVerifyGraph(graph);
                        if (status == VX_SUCCESS)
                        {
                            status = vxProcessGraph(graph);
                        }
                        if (status == VX_SUCCESS)
                        {

                        }
                        else
                        {
                            printf("Graph failed (%d)\n", status);
                            for (i = 0; i < dimof(nodes); i++)
                            {
                                status = VX_SUCCESS;
                                vxQueryNode(nodes[i], VX_NODE_ATTRIBUTE_STATUS, &status, sizeof(status));
                                if (status != VX_SUCCESS)
                                {
                                    printf("nodes[%u] failed with %d\n", i, status);
                                }
                            }
                            status = VX_ERROR_NOT_SUFFICIENT;
                        }
                        for (i = 0; i < dimof(nodes); i++)
                        {
                            vxReleaseNode(&nodes[i]);
                        }
                    }
                    vxReleaseGraph(&graph);
                }
            }
            for (i = 0; i < dimof(images); i++)
            {
                vxReleaseImage(&images[i]);
            }
        }
        else
        {
            status = VX_SUCCESS; /* not an error */
        }
exit:
        vxReleaseContext(&context);
    }
    return status;
}

vx_status vx_test_graph_lena(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_uint32 i = 0, w = 512, h = 512;
        vx_image images[] = {
            vxCreateImage(context, w, h, FOURCC_U8),                /* 0: BW */
            vxCreateImage(context, w, h, FOURCC_U8),                /* 1: Median */
            vxCreateImage(context, w, h, FOURCC_U8),                /* 2: Box */
            vxCreateImage(context, w, h, FOURCC_U8),                /* 3: Gaussian */
            vxCreateImage(context, w, h, FOURCC_S16),               /* 4: Custom */
            vxCreateImage(context, w, h, FOURCC_S16),               /* 5: Custom */
            vxCreateImage(context, w, h, FOURCC_U8),                /* 6: Mag */
            vxCreateImage(context, w, h, FOURCC_U8),                /* 7: Laplacian */
            vxCreateImage(context, w, h, FOURCC_U8),                /* 8: Affine */
            vxCreateImage(context, w, h, FOURCC_U8),                /* 9: Perspective */
        };
        vx_convolution conv[] = {
            vxCreateConvolution(context, 3, 3),
            vxCreateConvolution(context, 3, 3),
        };
        vx_pyramid pyramid = vxCreatePyramid(context, 4, VX_SCALE_PYRAMID_HALF, w, h, FOURCC_U8);
        vx_pyramid lap = vxCreatePyramid(context, 3, VX_SCALE_PYRAMID_HALF, w, h, FOURCC_U8);
        vx_matrix affine = vxCreateMatrix(context, VX_TYPE_FLOAT32, 2, 3);
        vx_matrix perspective = vxCreateMatrix(context, VX_TYPE_FLOAT32, 3, 3);
        /* just some non-sense filters */
        vx_int16 mat1[3][3] = {
            { 1, 0,-1},
            { 3, 0,-3},
            { 1, 0,-1},
        };
        vx_int16 mat2[3][3] = {
            { 1, 3, 1},
            { 0, 0, 0},
            {-1,-3,-1},
        };
        /* rotate and mirror, and scale */
       vx_float32 mat4[3][3] = {
            {0, 1, 0},
            {1, 0, 0},
            {0, 0, 0.5},
        };

        CHECK_ALL_ITEMS(images, i, status, exit);
        vxAccessConvolutionCoefficients(conv[0], NULL);
        vxCommitConvolutionCoefficients(conv[0], (vx_int16 *)mat1);
        vxAccessConvolutionCoefficients(conv[1], NULL);
        vxCommitConvolutionCoefficients(conv[1], (vx_int16 *)mat2);
        vxSetAffineRotationMatrix(affine, 45.0, 0.5, w/2, h/2);
        vxCommitMatrix(perspective, mat4);

        status |= vxLoadKernels(context, "openvx-debug");
        status |= vxLoadKernels(context, "openvx-extras");

        if (status == VX_SUCCESS)
        {
            vx_graph graph = vxCreateGraph(context);
            if (graph)
            {
                vx_node nodes[] = {
                    vxFReadImageNode(graph, "lena_512x512.pgm", images[0]),
                    vxMedian3x3Node(graph, images[0], images[1]),
                    vxBox3x3Node(graph, images[0], images[2]),
                    vxGaussian3x3Node(graph, images[0], images[3]),
                    vxConvolveNode(graph, images[3], conv[0], images[4]),
                    vxConvolveNode(graph, images[3], conv[1], images[5]),
                    vxMagnitudeNode(graph, images[4], images[5], images[6]),
                    vxLaplacian3x3Node(graph, images[0], images[7]),
                    vxPyramidNode(graph, images[0], pyramid, lap),
                    vxWarpAffineNode(graph, images[0], affine, VX_INTERPOLATION_TYPE_NEAREST_NEIGHBOR, images[8]),
                    vxWarpPerspectiveNode(graph, images[0], perspective, VX_INTERPOLATION_TYPE_NEAREST_NEIGHBOR, images[9]),
                    vxFWriteImageNode(graph, images[1], "olenamed_512x512_P400.bw"),
                    vxFWriteImageNode(graph, images[2], "olenaavg_512x512_P400.bw"),
                    vxFWriteImageNode(graph, images[3], "olenagau_512x512_P400.bw"),
                    vxFWriteImageNode(graph, images[4], "olenacus1_512x512_P400_-16b.bw"),
                    vxFWriteImageNode(graph, images[5], "olenacus2_512x512_P400_-16b.bw"),
                    vxFWriteImageNode(graph, images[6], "olenamag_512x512_P400.bw"),
                    vxFWriteImageNode(graph, images[7], "olenalapl_512x512_P400.bw"),
                    vxFWriteImageNode(graph, images[8], "olenaaffine_512x512_P400.bw"),
                    vxFWriteImageNode(graph, images[9], "olenaperspec_512x512_P400.bw"),
                };
                CHECK_ALL_ITEMS(nodes, i, status, exit);
                if (status == VX_SUCCESS)
                {
                    status = vxVerifyGraph(graph);
                    if (status == VX_SUCCESS)
                    {
                        status = vxProcessGraph(graph);
                    }
                    if (status == VX_SUCCESS)
                    {
                        vxuFWriteImage(vxGetPyramidLevel(pyramid, 0), "olenapyr_512x512_P400.bw");
                        vxuFWriteImage(vxGetPyramidLevel(pyramid, 1), "olenapyr_256x256_P400.bw");
                        vxuFWriteImage(vxGetPyramidLevel(pyramid, 2), "olenapyr_128x128_P400.bw");
                        vxuFWriteImage(vxGetPyramidLevel(pyramid, 3), "olenapyr_64x64_P400.bw");
                        vxuFWriteImage(vxGetPyramidLevel(lap, 0), "olenalap_512x512_P400.bw");
                        vxuFWriteImage(vxGetPyramidLevel(lap, 1), "olenalap_256x256_P400.bw");
                        vxuFWriteImage(vxGetPyramidLevel(lap, 2), "olenalap_128x128_P400.bw");
                    }
                    else
                    {
                        printf("Graph failed (%d)\n", status);
                        for (i = 0; i < dimof(nodes); i++)
                        {
                            status = VX_SUCCESS;
                            vxQueryNode(nodes[i], VX_NODE_ATTRIBUTE_STATUS, &status, sizeof(status));
                            if (status != VX_SUCCESS)
                            {
                                printf("nodes[%u] failed with %d\n", i, status);
                            }
                        }
                        status = VX_ERROR_NOT_SUFFICIENT;
                    }
                    for (i = 0; i < dimof(nodes); i++)
                    {
                        vxReleaseNode(&nodes[i]);
                    }
                }
                vxReleaseGraph(&graph);
            }
        }
        vxReleasePyramid(&lap);
        vxReleasePyramid(&pyramid);
        vxReleaseMatrix(&affine);
        vxReleaseMatrix(&perspective);
        for (i = 0; i < dimof(images); i++)
        {
            vxReleaseImage(&images[i]);
        }
exit:
        //vx_print_log(context);
        vxReleaseContext(&context);
    }
    return status;
}


vx_status vx_test_graph_channels_rgb(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_uint32 i = 0, w = 640, h = 480, diffs = 0u;
        vx_image images[] = {
            vxCreateImage(context, w, h, FOURCC_RGB),               /* 0: rgb */
            vxCreateVirtualImageWithFormat(context, FOURCC_U8),     /* 1: r */
            vxCreateVirtualImageWithFormat(context, FOURCC_U8),     /* 2: g */
            vxCreateVirtualImageWithFormat(context, FOURCC_U8),     /* 3: b */
            vxCreateImage(context, w, h, FOURCC_RGB),               /* 4: rgb */
        };
        status = VX_SUCCESS;
        for (i = 0; i < dimof(images); i++)
        {
            if (images[i] == 0)
            {
                status = VX_ERROR_NOT_SUFFICIENT;
            }
        }
        status |= vxLoadKernels(context, "openvx-debug");
        if (status == VX_SUCCESS)
        {
            vx_graph graph = vxCreateGraph(context);
            if (graph)
            {
                vx_node nodes[] = {
                    vxFReadImageNode(graph, "colorbars_640x480_I444.rgb", images[0]),
                    vxChannelExtractNode(graph, images[0], VX_CHANNEL_R, images[1]),
                    vxChannelExtractNode(graph, images[0], VX_CHANNEL_G, images[2]),
                    vxChannelExtractNode(graph, images[0], VX_CHANNEL_B, images[3]),
                    vxFWriteImageNode(graph, images[1], "or_640x480_P400.bw"),
                    vxFWriteImageNode(graph, images[2], "og_640x480_P400.bw"),
                    vxFWriteImageNode(graph, images[3], "ob_640x480_P400.bw"),
                    vxChannelCombineNode(graph, images[1], images[2], images[3], 0, images[4]),
                    vxFWriteImageNode(graph, images[4], "ocolorbars2_640x480_I444.rgb"),
                };
                //CHECK_ALL_ITEMS(nodes, i, status, exit);
                if (status == VX_SUCCESS)
                {
                    status = vxVerifyGraph(graph);
                    if (status == VX_SUCCESS)
                    {
                        status = vxProcessGraph(graph);
                    }
                    if (status == VX_SUCCESS)
                    {
                        // make sure images[0] == images[4]
                        status = vxuCompareImages(images[0], images[4], &diffs);
                        printf("Found %u differences between images (status=%d)\n", diffs, status);
                    }
                    for (i = 0; i < dimof(nodes); i++)
                    {
                        vxReleaseNode(&nodes[i]);
                    }
                }
                vxReleaseGraph(&graph);
            }
        }
        for (i = 0; i < dimof(images); i++)
        {
            vxReleaseImage(&images[i]);
        }
        vxReleaseContext(&context);
    }
    return status;
}

vx_status vx_test_graph_accum(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_uint32 i = 0, w = 640, h = 480;
        vx_image images[] = {
            vxCreateImage(context, w, h, FOURCC_U8),
            vxCreateImage(context, w, h, FOURCC_U16),
            vxCreateImage(context, w, h, FOURCC_U16),
            vxCreateImage(context, w, h, FOURCC_U16),
            vxCreateImage(context, w, h, FOURCC_S16),
        };
        vx_float32 alpha = 0.5f;
        vx_scalar scalar = vxCreateScalar(context, VX_TYPE_FLOAT32, &alpha);
        CHECK_ALL_ITEMS(images, i, status, exit);
        status |= vxLoadKernels(context, "openvx-debug");
        if (status == VX_SUCCESS)
        {
            vx_graph graph = vxCreateGraph(context);
            if (graph)
            {
                vx_node nodes[] = {
                    vxFReadImageNode(graph, "bikegray_640x480.pgm", images[0]),
                    vxAccumulateImageNode(graph, images[0], images[1]),
                    vxAccumulateWeightedImageNode(graph, images[0], scalar, images[2]),
                    vxAccumulateSquareImageNode(graph, images[0], images[3]),
                    vxFWriteImageNode(graph, images[1], "obikeaccu_640x480_P400_16b.bw"),
                    vxFWriteImageNode(graph, images[2], "obikeaccw_640x480_P400_16b.bw"),
                    vxFWriteImageNode(graph, images[3], "obikeaccq_640x480_P400_16b.bw"),
                };
                CHECK_ALL_ITEMS(nodes, i, status, exit);
                if (status == VX_SUCCESS)
                {
                    status = vxVerifyGraph(graph);
                    if (status == VX_SUCCESS)
                    {
                        status = vxProcessGraph(graph);
                    }
                    if (status == VX_SUCCESS)
                    {
                        /* add a node which should cause the graph to be re-verified and will cause a failure. */
                        vx_node node = vxAccumulateImageNode(graph, images[0], images[1]);
                        if (node == 0)
                        {
                            printf("Failed to create node to break graph!\n");
                        }
                        if (vxIsGraphVerified(graph) == vx_true_e)
                        {
                            return VX_ERROR_NOT_SUFFICIENT;
                        }
                        status = vxVerifyGraph(graph);
                        if (status == VX_SUCCESS)
                        {
                            printf("Failed to fail multiple writers check!\n");
                            status = VX_ERROR_NOT_SUFFICIENT;
                        }
                        else if (status == VX_ERROR_MULTIPLE_WRITERS)
                        {
                            printf("Multiple writers failed with %d!\n", status);
                            status = VX_SUCCESS;
                        }
                        else
                        {
                            /* wrong error code! */
                            status = VX_ERROR_NOT_SUFFICIENT;
                        }
                    }
                    for (i = 0; i < dimof(nodes); i++)
                    {
                        vxReleaseNode(&nodes[i]);
                    }
                }
                vxReleaseGraph(&graph);
            }
        }
        for (i = 0; i < dimof(images); i++)
        {
            vxReleaseImage(&images[i]);
        }
        vxReleaseScalar(&scalar);
exit:
        vxReleaseContext(&context);
    }
    return status;
}

static vx_uint32 b_and(vx_uint32 a, vx_uint32 b) { return a & b; }
static vx_uint32 b_or(vx_uint32 a, vx_uint32 b) { return a | b; }
static vx_uint32 b_xor(vx_uint32 a, vx_uint32 b) { return a ^ b; }
static vx_uint32 b_not_1st(vx_uint32 a, vx_uint32 b) { (void)b; return ~a; }
static vx_uint32 b_not_2nd(vx_uint32 a, vx_uint32 b) { (void)a; return ~b; }
static vx_uint32 b_nand(vx_uint32 a, vx_uint32 b) { return ~(a & b); }
static vx_uint32 b_nor(vx_uint32 a, vx_uint32 b) { return ~(a | b); }
static vx_uint32 b_xorn(vx_uint32 a, vx_uint32 b) { return a ^ ~b; }

#undef OPX
#define OPX(x) {b_ ## x, #x}

typedef struct _subtest_t {
    vx_uint32 (*op)(vx_uint32, vx_uint32);
    const char *s;
} subtest_t;

/*!
 * \brief Test for the bitwise operators.
 * \ingroup group_tests
 */
vx_status vx_test_graph_bitwise(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_uint32 i = 0, width = 640, height = 480;
        vx_graph graph = 0;
        vx_image images[] = {
            vxCreateImage(context, width, height, FOURCC_U8), /* in1 */
            vxCreateImage(context, width, height, FOURCC_U8), /* in2 */
            vxCreateImage(context, width, height, FOURCC_U8), /* and */
            vxCreateImage(context, width, height, FOURCC_U8), /* or */
            vxCreateImage(context, width, height, FOURCC_U8), /* xor */
            vxCreateImage(context, width, height, FOURCC_U8), /* not (on first image of two) */
            /*
             * The following are just trivial sequence tests; no node combination
             * can be expected, as the intermediate results are inspected.
             */
            vxCreateImage(context, width, height, FOURCC_U8), /* not (on second image of two) */
            vxCreateImage(context, width, height, FOURCC_U8), /* nand */
            vxCreateImage(context, width, height, FOURCC_U8), /* nor */
            vxCreateImage(context, width, height, FOURCC_U8), /* xorn */
        };

        CHECK_ALL_ITEMS(images, i, status, release_context);

        status = vxLoadKernels(context, "openvx-debug");
        if (status != VX_SUCCESS)
            FAIL(release_context, "can't load debug extensions");

        graph = vxCreateGraph(context);
        if (graph)
        {
            vx_node nodes[] = {
                vxAndNode(graph, images[0], images[1], images[2]),
                vxOrNode(graph, images[0], images[1], images[3]),
                vxXorNode(graph, images[0], images[1], images[4]),
                vxNotNode(graph, images[0], images[5]),

                vxNotNode(graph, images[1], images[6]),
                vxNotNode(graph, images[2], images[7]),
                vxNotNode(graph, images[3], images[8]),
                vxXorNode(graph, images[0], images[6], images[9]),
            };

            /*
             * Let's avoid putting 0 at (0, 0): let's offset the first
             * pixel-values. The start values are otherwise somewhat random.
             */
            vx_uint8 v0_start = 42, v1_start = 256 - 77;
            vx_uint8 v0 = v0_start, v1 = v1_start;

            vx_uint32 x, y;
            vx_rectangle rect = vxCreateRectangle(context, 0, 0, width, height);
            vx_imagepatch_addressing_t image0_addr;
            vx_imagepatch_addressing_t image1_addr;
            void *base0 = NULL;
            void *base1 = NULL;
            /* The images to test are now at images[2], ... images[N - 1]. */
            subtest_t subtests[] = {
                OPX(and),
                OPX(or),
                OPX(xor),
                OPX(not_1st),
                OPX(not_2nd),
                OPX(nand),
                OPX(nor),
                OPX(xorn)
            };

            status = vxAccessImagePatch(images[0], rect, 0, &image0_addr, &base0);
            status |= vxAccessImagePatch(images[1], rect, 0, &image1_addr, &base1);
            if (status != VX_SUCCESS)
                FAIL(release_rect, "couldn't get image patch");
            CHECK_ALL_ITEMS(nodes, i, status, release_graph);

            /*
             * Fill the source images. As long as each image size (i.e. width*height) is
             * >= 256*256, these tests will be exhaustive wrt. the domain of the core
             * operations.
             */
            for (y = 0; y < height; y++)
            {
                for (x = 0; x < width; x++)
                {
                    vx_uint8 *img0 = vxFormatImagePatchAddress2d(base0, x, y, &image0_addr);
                    vx_uint8 *img1 = vxFormatImagePatchAddress2d(base1, x, y, &image1_addr);

                    *img0 = v0++;
                    *img1 = v1;

                    if (v0 == v0_start)
                        v1++;
                }
            }

            status = vxVerifyGraph(graph);
            if (status != VX_SUCCESS)
                FAIL(release_rect, "graph verification failed");

            status = vxProcessGraph(graph);
            if (status != VX_SUCCESS)
                FAIL(release_rect, "graph processing failed");


            for (i = 0; i < dimof(subtests); i++)
            {
                vx_imagepatch_addressing_t image_addr;
                void *base = NULL;
                v0 = v0_start;
                v1 = v1_start;

                status = vxAccessImagePatch(images[i + 2], rect, 0, &image_addr, &base);
                if (status != VX_SUCCESS)
                    VFAIL(release_rect, "couldn't get image patch for %s-image", subtests[i].s);

                for (y = 0; y < height; y++)
                {
                    for (x = 0; x < width; x++)
                    {
                        vx_uint8 *img = vxFormatImagePatchAddress2d(base, x, y, &image_addr);
                        vx_uint8 ref = subtests[i].op(v0, v1);

                        if (ref != *img)
                            VFAIL(release_rect,
                                  "wrong result at (x, y) = (%d, %d) for bitwise %s(0x%x, 0x%x): 0x%x should be 0x%x",
                                  x, y, subtests[i].s, v0, v1, *img, ref);

                        v0++;
                        if (v0 == v0_start)
                            v1++;
                    }
                }
            }

release_rect:
            vxReleaseRectangle(&rect);

            for (i = 0; i < dimof(nodes); i++)
                vxReleaseNode(&nodes[i]);

release_graph:
            vxReleaseGraph(&graph);
        }
        for (i = 0; i < dimof(images); i++)
            vxReleaseImage(&images[i]);

release_context:
        vxReleaseContext(&context);
    }
    return status;
}

/*
 * Create an image of (width, height) in format. Fill the image,
 * covering different values, changing values no more often than a set
 * period. Use seed when initializing values.
 */
static vx_image vx_create_image_valuecovering(vx_context context, vx_fourcc format,
                                              vx_uint32 width, vx_uint32 height,
                                              vx_uint32 seed,
                                              vx_uint32 period)
{
    vx_image image = vxCreateImage(context, width, height, format);
    vx_rectangle rect = vxCreateRectangle(vxGetContext(image), 0, 0, width, height);
    vx_imagepatch_addressing_t addr;
    void *base = NULL;
    vx_status status = vxAccessImagePatch(image, rect, 0, &addr, &base);
    vx_uint32 x, y;
    vx_uint32 period_counter = 0;

    if (status != VX_SUCCESS)
        goto release_rect;

    if (format == FOURCC_U8)
    {
        /*
         * Assume we're going to iterate over all values for this image
         * and combinations together with another FOURCC_U8 image; no need
         * for randomness.
         */
        vx_uint8 val = seed & 255;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                vx_uint8 *img = vxFormatImagePatchAddress2d(base, x, y, &addr);

                *img = val;
                if (++period_counter == period)
                {
                    period_counter = 0;
                    val++;
                }
            }
        }
    }
    else if (format == FOURCC_S16)
    {
        vx_int16 val = 0;
        /*
         * There are probably not enough pixels in the image to cover
         * all combinations of FOURCC_S16 together with (even)
         * FOURCC_U8, so go for something a little more random. We don't
         * need a high-quality PRNG, whatever is delivered as part of a
         * C99 implementation is sufficient.
         */
        srand(seed);

        /* The subtraction only makes a difference if RAND_MAX < 65535. */
        val = (vx_int16)(rand() - 32768);

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                vx_int16 *img = vxFormatImagePatchAddress2d(base, x, y, &addr);

                *img = val;

                if (++period_counter == period)
                {
                    period_counter = 0;
                    val = rand();
                }
            }
        }
    }
    else /* Unsupported; missing else-if clause above. */
    {
        vxReleaseImage(&image);
        image = 0;
    }

release_rect:
    vxReleaseRectangle(&rect);
    return image;
}

static vx_int32 a_apply_policy(vx_int32 raw_result, vx_fourcc destformat, enum vx_convert_policy_e policy)
{
    vx_int32 tmp;

    if (policy == VX_CONVERT_POLICY_SATURATE)
    {
        vx_int32 max = destformat == FOURCC_U8 ? UINT8_MAX : INT16_MAX;
        vx_int32 min = destformat == FOURCC_U8 ? 0 : INT16_MIN;

        if (raw_result > max)
            tmp = max;
        else if (raw_result < min)
            tmp = min;
        else
            tmp = raw_result;
    } else
        tmp = raw_result;

    return destformat == FOURCC_U8 ? (vx_uint8)tmp : (vx_int16)tmp;
}

static vx_int32 a_mult(vx_int32 a, vx_int32 b, vx_float32 scale,
                       vx_fourcc destformat, enum vx_convert_policy_e policy)
{
    vx_int32 primary_product = a * b;
    vx_float32 scaled_raw_result = scale * primary_product;
    vx_int32 inttyped_raw_result = (vx_int32)scaled_raw_result;
    vx_int32 policy_result = a_apply_policy(inttyped_raw_result, destformat, policy);
    return policy_result;
}

static vx_int32 a_add(vx_int32 a, vx_int32 b, vx_float32 scale,
                      vx_fourcc destformat, enum vx_convert_policy_e policy)
{
    vx_int32 sum = a + b;
    vx_int32 policy_result = a_apply_policy(sum, destformat, policy);
    scale += 0.0; /* Avoid compiler warning about unused parameter. */
    return policy_result;
}

static vx_int32 a_sub(vx_int32 a, vx_int32 b, vx_float32 scale,
                      vx_fourcc destformat, enum vx_convert_policy_e policy)
{
    vx_int32 difference = a - b;
    vx_int32 policy_result = a_apply_policy(difference, destformat, policy);
    scale += 0.0; /* Avoid compiler warning about unused parameter. */
    return policy_result;
}

/*!
 * \brief Test for the pixelwise arithmetic.
 * \ingroup group_tests
 */
vx_status vx_test_graph_arit(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    /* Let's just test a single representative size here. */
    vx_uint32 width = 640, height = 480;
    vx_rectangle rect = vxCreateRectangle(context, 0, 0, width, height);

    /*
     * Iterate over all combinations of types and overflow policies;
     * skip the invalid ones (or should they be attempted with expected
     * errors verified)?  For the floating-point "scale" parameter,
     * iterate over a list of representative values.
     */
    static const vx_float32 test_scales[] = { 0, 0.125f, 0.25f, 0.5f, 1.0f, 1.25f, 1.5f, 2.0f };

    /* Testing is made somewhat simpler by using the direct calls. */
    static const struct {
        vx_int32 (*refop)(vx_int32, vx_int32, vx_float32, vx_fourcc, enum vx_convert_policy_e);
        vx_status (*operate_image)(vx_image, vx_image, vx_enum, vx_image);
        int n_scales;
        const char *name;
    } op_tests[] = {
        /*
         * A bit of simplification; using the same test-fixture despite
         * "multiply" also having a scaling parameter. The difference is
         * keyed on n_scales == 1, but only in part; see code below.
         */
        { a_add, vxuAdd, 1, "vxuAdd" },
        { a_sub, vxuSubtract, 1, "vxuSubtract" },
        { a_mult, NULL, dimof(test_scales), "vxuMult" }
    };

    /*
     * Don't iterate over any enums themselves: we can't know without
     * peeking, their individual orders or indeed whether they're
     * consecutive.
     */
    static const enum vx_convert_policy_e test_policies[] = {
        VX_CONVERT_POLICY_TRUNCATE, VX_CONVERT_POLICY_SATURATE
    };

    static const vx_fourcc test_formats[] = {
        FOURCC_U8, FOURCC_S16
    };
    vx_image src0_image = 0, src1_image = 0, dest_image = 0;
    int ipolicy;

    if (!context)
        return status;

    status = vxLoadKernels(context, "openvx-debug");
    if (status != VX_SUCCESS)
        FAIL(release_rect, "can't load debug extensions");

    for (ipolicy = 0; ipolicy < dimof(test_policies); ipolicy++)
    {
        vx_enum policy = test_policies[ipolicy];
        int isrc0_format;
        for (isrc0_format = 0; isrc0_format < dimof(test_formats); isrc0_format++)
        {
            vx_fourcc src0_format = test_formats[isrc0_format];
            vx_imagepatch_addressing_t image0_addr;
            void *base0 = NULL;
            int isrc1_format;

            src0_image = vx_create_image_valuecovering(context, src0_format, width, height, 130613, 1);
            if (!src0_image)
                VFAIL(release_rect,
                      "can't create test-image for first arithmetic operand (%d, %d)",
                      ipolicy, isrc0_format);

            status = vxAccessImagePatch(src0_image, rect, 0, &image0_addr, &base0);
            if (status != VX_SUCCESS)
                VFAIL(release_src0_image,
                      "can't create patch to test-image for first arithmetic operand (%d, %d)",
                      ipolicy, isrc0_format);

            for (isrc1_format = 0; isrc1_format < dimof(test_formats); isrc1_format++)
            {
                vx_fourcc src1_format = test_formats[isrc1_format];
                vx_imagepatch_addressing_t image1_addr;
                void *base1 = NULL;
                int idest_format;

                /* We deliberately use periodicity 256 for both FOURCC_U8 and FOURCC_S16. */
                src1_image = vx_create_image_valuecovering(context, src1_format, width, height, 20130611, 256);
                if (!src1_image)
                    VFAIL(release_src0_image, "can't create test-image for second operand (%d, %d, %d)",
                          ipolicy, isrc0_format, isrc1_format);

                status = vxAccessImagePatch(src1_image, rect, 0, &image1_addr, &base1);
                if (status != VX_SUCCESS)
                    VFAIL(release_src1_image,
                          "can't create patch to test-image for second operand (%d, %d, %d)",
                          ipolicy, isrc0_format, isrc1_format);

                for (idest_format = 0; idest_format < dimof(test_formats); idest_format++)
                {
                   vx_fourcc dest_format = test_formats[idest_format];
                   int iops;

                   if (dest_format == FOURCC_U8 && (src0_format != FOURCC_U8 || src1_format != FOURCC_U8))
                       continue;

                   for (iops = 0; iops < dimof(op_tests); iops++)
                   {
                       int iscale;
                       for (iscale = 0; iscale < op_tests[iops].n_scales; iscale++)
                       {
                           vx_float32 scale = test_scales[iscale];
                           vx_image dest_image = vxCreateImage(context, width, height, dest_format);
                           vx_imagepatch_addressing_t dest_addr;
                           void *dbase = NULL;
                           vx_uint32 x, y;

                           if (!dest_image)
                               VFAIL(release_src1_image, "can't create dest_image (%d, %d, %d, %d, %d, %d)",
                                     ipolicy, isrc0_format, isrc1_format, idest_format, iops, iscale);

                           /*
                            * Second part of test-fixture simplification: "multiply" is assumed if
                            * there's more than one "scale" iteration, otherwise "scale" is assumed
                            * not used.
                            */
                           if (op_tests[iops].n_scales > 1)
                               status = vxuMultiply(src0_image, src1_image, scale, policy, dest_image);
                           else
                               status = op_tests[iops].operate_image(src0_image, src1_image, policy, dest_image);
                           if (status != VX_SUCCESS)
                               VFAIL(release_dest_image,
                                     "can't apply %s (%d, %d, %d, %d, %d, %d)",
                                     op_tests[iops].name,
                                     ipolicy, isrc0_format, isrc1_format, idest_format, iops, iscale);

                           status = vxAccessImagePatch(dest_image, rect, 0, &dest_addr, &dbase);
                           if (status != VX_SUCCESS)
                               VFAIL(release_dest_image,
                                     "can't create patch to dest_image (%d, %d, %d, %d, %d, %d)",
                                     ipolicy, isrc0_format, isrc1_format, idest_format, iops, iscale);

                           for (y = 0; y < height; y++)
                           {
                             for (x = 0; x < width; x++)
                             {
                               void *img0 = vxFormatImagePatchAddress2d(base0, x, y, &image0_addr);
                               void *img1 = vxFormatImagePatchAddress2d(base1, x, y, &image1_addr);
                               void *imgd = vxFormatImagePatchAddress2d(dbase, x, y, &dest_addr);
                               vx_int32 v0 = src0_format == FOURCC_U8 ? *(vx_uint8 *)img0 : *(vx_int16 *)img0;
                               vx_int32 v1 = src1_format == FOURCC_U8 ? *(vx_uint8 *)img1 : *(vx_int16 *)img1;
                               vx_int32 res = dest_format == FOURCC_U8 ? *(vx_uint8 *)imgd : *(vx_int16 *)imgd;
                               vx_int32 ref = op_tests[iops].refop(v0, v1, scale, dest_format, policy);

                               if (ref != res)
                                   VFAIL(release_dest_image,
                                         "wrong result at (x, y) = (%d, %d) for %s:%s(0x%x:%s, 0x%x:%s, %f);"
                                         " 0x%x:%s should be 0x%x:%s",
                                         x, y,
                                         op_tests[iops].name,
                                         policy == VX_CONVERT_POLICY_TRUNCATE ? "TRUNC" : "SAT",
                                         v0, src0_format == FOURCC_U8 ? "U8" : "S16",
                                         v1, src1_format == FOURCC_U8 ? "U8" : "S16",
                                         scale,
                                         res, dest_format == FOURCC_U8 ? "U8" : "S16",
                                         ref, dest_format == FOURCC_U8 ? "U8" : "S16");
                             }
                           }

                           vxReleaseImage(&dest_image);
                       }
                   }
                }
                vxReleaseImage(&src1_image);
            }
            vxReleaseImage(&src0_image);
        }
    }

    /*
     * If we got here, everything checked out. We've already released
     * the node, the graph and the images, so skip those parts.
     */
    goto release_rect;

release_dest_image:
    vxReleaseImage(&dest_image);

release_src1_image:
    vxReleaseImage(&src1_image);

release_src0_image:
    vxReleaseImage(&src0_image);

release_rect:
    vxReleaseRectangle(&rect);

    vxReleaseContext(&context);

    return status;
}

vx_status vx_test_graph_corners(int argc, char *argv[])
{
    vx_status status = VX_SUCCESS;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_uint32 width = 160, height = 120, n;
        vx_float32 k = 0.15f;
        vx_float32 b = 0.47f;
        vx_float32 str = 10000.0f;
        vx_float32 min_d = 2.0f;
        vx_uint32 ws = 5, bs = 3;

        vx_scalar scalars[] = {
                vxCreateScalar(context, VX_TYPE_FLOAT32, &str),
                vxCreateScalar(context, VX_TYPE_FLOAT32, &min_d),
                vxCreateScalar(context, VX_TYPE_FLOAT32, &k),
                vxCreateScalar(context, VX_TYPE_FLOAT32, &b)
        };
        vx_image images[] = {
                vxCreateImage(context, width, height, FOURCC_U8),
                vxCreateImage(context, width, height, FOURCC_U8),
        };
        vx_list harris_list = vxCreateList(context, VX_TYPE_KEYPOINT, 1000);
        vx_list fast_list = vxCreateList(context, VX_TYPE_KEYPOINT, 1000);
        vx_graph graph = vxCreateGraph(context);
        vxLoadKernels(context, "openvx-debug");
        if (graph)
        {
            vx_node nodes[] = {
                vxFReadImageNode(graph, "shapes.pgm", images[0]),
                vxGaussian3x3Node(graph, images[0], images[1]),
                vxFWriteImageNode(graph, images[1], "oshapes_blurred.pgm"),
                vxHarrisCornersNode(graph, images[1], scalars[0], scalars[1], scalars[2], ws, bs, harris_list),
                vxFastCornersNode(graph, images[1], scalars[3], vx_false_e, fast_list),
            };
            CHECK_ALL_ITEMS(nodes, n, status, exit);
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                vx_size length = 0;
                vx_uint32 sx, sy, ex, ey;
                vx_rectangle rect = 0;

                status = vxProcessGraph(graph);
#ifdef OPENVX_KHR_LIST
                if (vxSortList(harris_list, vxHarrisScoreSorter) == VX_SUCCESS)
                {
                    vx_iterator hit = 0, lit = 0;
                    vx_keypoint hi = (vx_keypoint)vxGetListItem(harris_list, &hit, VX_LIST_LAST);
                    vx_keypoint lo = (vx_keypoint)vxGetListItem(harris_list, &lit, VX_LIST_FIRST);
                    vx_keypoint_t *hic = NULL, *loc = NULL;
                    vxAccessKeypoint(hi, &hic);
                    vxAccessKeypoint(lo, &loc);
                    printf("The highest score is %lf @ {%d,%d}, the lowest, %lf @ {%d,%d}\n",
                            hic->strength, hic->x, hic->y, loc->strength, loc->x, loc->y);
                    vxCommitKeypoint(hi, hic);
                    vxCommitKeypoint(lo, loc);
                }
#endif
                rect = vxGetValidRegionImage(images[1]);
                status = vxAccessRectangleCoordinates(rect, &sx, &sy, &ex, &ey);
                vxQueryList(harris_list, VX_LIST_ATTRIBUTE_LENGTH, &length, sizeof(length));
                printf("There are "VX_FMT_SIZE" number of points in the harris list!\n", length);
                vxQueryList(fast_list, VX_LIST_ATTRIBUTE_LENGTH, &length, sizeof(length));
                printf("There are "VX_FMT_SIZE" number of points in the fast list!\n", length);
                printf("Rectangle from Gaussian is {%d, %d, %d, %d}\n", sx, sy, ex, ey);
            }
exit:
            for (n = 0; n < dimof(nodes); n++)
            {
                vxReleaseNode(&nodes[n]);
            }
            vxReleaseGraph(&graph);
        }
        for (n = 0; n < dimof(images); n++)
        {
            vxReleaseImage(&images[n]);
        }
        for (n = 0; n < dimof(scalars); n++)
        {
            vxReleaseScalar(&scalars[n]);
        }
        vxReleaseList(&harris_list);
        vxReleaseList(&fast_list);
        vxReleaseContext(&context);
    }
    return status;
}

/*! The array of supported unit tests */
vx_unittest unittests[] = {
    {VX_FAILURE, "Framework: Load XYZ Extension",   vx_test_framework_load_extension},
    {VX_FAILURE, "Framework: Load Kernel Node", vx_test_framework_load_kernel_node},
    {VX_FAILURE, "Framework: Copy",             vx_test_framework_copy},
    {VX_FAILURE, "Framework: Copy Virtual",     vx_test_framework_copy_virtual},
    {VX_FAILURE, "Framework: Heads",            vx_test_framework_heads},
    {VX_FAILURE, "Framework: Unvisited",        vx_test_framework_unvisited},
    {VX_FAILURE, "Framework: Virtual Image",    vx_test_framework_virtualimage},
    {VX_FAILURE, "Framework: Delay",            vx_test_framework_delay_graph},
    {VX_FAILURE, "Direct: Copy Image",          vx_test_direct_copy_image},
    {VX_FAILURE, "Direct: Copy Buffer",         vx_test_direct_copy_buffer},
    {VX_FAILURE, "Direct: Copy External Image", vx_test_direct_copy_external_image},
    {VX_FAILURE, "Direct: Copy External Buffer",vx_test_direct_copy_external_buffer},
    // graphs
    {VX_FAILURE, "Graph: ColorBars YUV",        vx_test_graph_channels_yuv},
    {VX_FAILURE, "Graph: ColorBars RGB",        vx_test_graph_channels_rgb},
    {VX_FAILURE, "Graph: bikegray",             vx_test_graph_bikegray},
    {VX_FAILURE, "Graph: Lena",                 vx_test_graph_lena},
    {VX_FAILURE, "Graph: Accumulates",          vx_test_graph_accum},
    {VX_FAILURE, "Graph: OpenCL",               vx_test_graph_opencl},
    {VX_FAILURE, "Graph: Bitwise",              vx_test_graph_bitwise},
    {VX_FAILURE, "Graph: Arithmetic",           vx_test_graph_arit},
    {VX_FAILURE, "Graph: Corners",              vx_test_graph_corners},
};

/*! \brief The main unit test.
 * \param argc The number of arguments.
 * \param argv The array of arguments.
 * \return vx_status
 * \retval 0 Success.
 * \retval !0 Failure of some sort.
 */
int main(int argc, char *argv[])
{
    vx_uint32 i;
    vx_uint32 passed = 0;
    vx_bool stopOnErrors = vx_false_e;

    if (argc == 2 && ((strncmp(argv[1], "-?", 2) == 0) ||
                      (strncmp(argv[1], "--list", 6) == 0) ||
                      (strncmp(argv[1], "-l", 2) == 0) ||
                      (strncmp(argv[1], "/?", 2) == 0)))
    {
        vx_uint32 t = 0;
        for (t = 0; t < dimof(unittests); t++)
        {
            printf("%u: %s\n", t, unittests[t].name);
        }
    }
    else if (argc == 3 && strncmp(argv[1],"-t",2) == 0)
    {
        int c = atoi(argv[2]);
        if (c < (int)dimof(unittests))
        {
            unittests[c].status = unittests[c].unittest(argc, argv);
            printf("[%u][%s] %s, error = %d\n", c, (unittests[c].status == VX_SUCCESS?"PASSED":"FAILED"), unittests[c].name, unittests[c].status);
            return unittests[c].status;
        }
        else
            return -1;
    }
    else if (argc == 2 && strncmp(argv[1],"-s",2) == 0)
    {
        stopOnErrors = vx_true_e;
    }
    for (i = 0; i < dimof(unittests); i++)
    {
        unittests[i].status = unittests[i].unittest(argc, argv);
        if (unittests[i].status == VX_SUCCESS)
        {
            printf("[PASSED][%02u] %s\n", i, unittests[i].name);
            passed++;
        }
        else
        {
            printf("[FAILED][%02u] %s, error = %d\n", i, unittests[i].name, unittests[i].status);
            if (stopOnErrors == vx_true_e)
            {
                break;
            }
        }
    }
    printf("Passed %u out of "VX_FMT_SIZE"\n", passed, dimof(unittests));
    if (passed == dimof(unittests))
        return 0;
    else
        return -1;
}

