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
#include <vx.hpp>

using namespace OpenVX;

void list_kernels(void)
{
    vx_uint32 numTargets = OpenVX::Context::numTargets();
    vx_uint32 numModules = OpenVX::Context::numModules();
    vx_uint32 numKernels = OpenVX::Context::numKernels();

    printf("Implementation: %s\n", OpenVX::Context::implementation());
    printf("Vendor: %hu\n", OpenVX::Context::vendorID());
    printf("Version: %04x\n", OpenVX::Context::version());
    printf("Extensions: %s\n", OpenVX::Context::extensions());
    printf("Num Modules = %u\n", numModules);
    printf("Num Targets = %u\n", numTargets);
    printf("Num Kernels = %u\n", numKernels);

    for (vx_uint32 t = 0; t < numTargets; t++)
    {
        OpenVX::Target target(t);
        vx_target_kernel_t *table = target.kernelTable();
        printf("Target %s\n", target.name());
        for (vx_uint32 k = 0; table && k < target.numKernels(); k++)
        {
            OpenVX::Kernel kernel(table[k].enumeration); // or by name
            printf("enum %08x == %s params:%u\n", table[k].enumeration, table[k].name, kernel.numParameters());
        }
    }
}

int main(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    OpenVX::Context::LoadKernels("openvx-debug");
    if (argc == 2 && strcmp("-l",argv[1]) == 0)
        list_kernels();

    OpenVX::Graph graph;
    OpenVX::Image input, gx, gy, output(FOURCC_U8);

    OpenVX::FReadImageNode fr(&graph, "lena_512x512.pgm", &input);
    OpenVX::Sobel3x3Node sobel(&graph, &input, &gx, &gy);
    OpenVX::MagnitudeNode mag(&graph, &gx, &gy, &output);
    OpenVX::FWriteImageNode fw(&graph, &output, "ocpplena.pgm");

    status = graph.Verify();
    if (status == VX_SUCCESS)
    {
        status = graph.Process();
        if (status == VX_SUCCESS)
        {
            printf("Ran graph!\n");
        }
    }
    printf("Returning status %d\n", status);
    return status;
}

