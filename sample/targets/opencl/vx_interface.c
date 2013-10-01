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
 * \brief The OpenCL OpenVX Kernel Interfaces
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>
#include <vx_interface.h>

static const vx_char name[VX_MAX_TARGET_NAME] = "pc.opencl";

/*! \brief Prototype for assigning to kernel */
static vx_status vxclCallOpenCLKernel(vx_node node, vx_reference *parameters, vx_uint32 num);

/*! \brief Converts an OpenCL error into a string description. */
static void vxcl_error_string(char *prefix, cl_int err);

void vxclDestroyMem(vx_cl_mem_t *mem)
{
    vx_uint32 p;
    if (mem->type == VX_TYPE_IMAGE)
    {
        for (p = 0; p < mem->image.planes; p++)
        {
            if (mem->dir == VX_INPUT)
            {
                vxReleaseParameter(&mem->image.rect);
                mem->image.rect = 0;
            }
            if (vxCommitImagePatch(mem->ref, mem->image.rect, p, &mem->image.addr[p], mem->image.base[p]) == VX_SUCCESS)
            {
                clReleaseMemObject(mem->image.addr_mem[p]);
                clReleaseMemObject(mem->mem[p]);
                vxReleaseParameter(&mem->image.rect);
                mem->image.rect = 0;
            }
        }
    }
    else if (mem->type == VX_TYPE_BUFFER)
    {
        vx_size limit = 0;
        if (mem->dir != VX_INPUT)
        {
            limit = mem->buffer.buf.numUnits;
        }
        vxCommitBufferRange(mem->ref, 0, limit, mem->buffer.base);
        clReleaseMemObject(mem->mem[0]);
    }
    else if (mem->type == VX_TYPE_COORDINATES)
    {
        void *ptr = NULL;
        if (mem->dir != VX_INPUT)
        {
            ptr = mem->other.base;
        }
        vxCommitCoordinates(mem->ref, ptr);
        clReleaseMemObject(mem->mem[0]);
    }
    else if (mem->type == VX_TYPE_CONVOLUTION)
    {
        void *ptr = NULL;
        if (mem->dir != VX_INPUT)
        {
            ptr = mem->other.base;
        }
        vxCommitConvolutionCoefficients(mem->ref, ptr);
        clReleaseMemObject(mem->mem[0]);
    }
    else if (mem->type == VX_TYPE_DISTRIBUTION)
    {
        void *ptr = NULL;
        if (mem->dir != VX_INPUT)
        {
            ptr = mem->other.base;
        }
        vxCommitDistribution(mem->ref, ptr);
        clReleaseMemObject(mem->mem[0]);
    }
    else if (mem->type == VX_TYPE_LUT)
    {
        void *ptr = NULL;
        if (mem->dir != VX_INPUT)
        {
            ptr = mem->other.base;
        }
        vxCommitLUT(mem->ref, ptr);
        clReleaseMemObject(mem->mem[0]);
    }
    else if (mem->type == VX_TYPE_MATRIX)
    {
        void *ptr = NULL;
        if (mem->dir != VX_INPUT)
        {
            ptr = mem->other.base;
        }
        vxCommitMatrix(mem->ref, ptr);
        clReleaseMemObject(mem->mem[0]);
    }
    /* add other types here, currently Pyramids and Threshold and Delays don't go to OpenCL. */
}

vx_status vxclCreateMem(vx_context vxcontext,
                        cl_context clcontext,
                        vx_cl_mem_t *mem)
{
    vx_status status = VX_SUCCESS;
    cl_int err = 0;
    if (mem->type == VX_TYPE_IMAGE)
    {
        vx_uint32 p;
        vxQueryImage(mem->ref, VX_IMAGE_ATTRIBUTE_PLANES, &mem->image.planes, sizeof(mem->image.planes));
        vxQueryImage(mem->ref, VX_IMAGE_ATTRIBUTE_WIDTH, &mem->image.width, sizeof(mem->image.width));
        vxQueryImage(mem->ref, VX_IMAGE_ATTRIBUTE_HEIGHT, &mem->image.height, sizeof(mem->image.height));
        mem->image.rect = vxCreateRectangle(vxcontext, 0, 0, mem->image.width, mem->image.height);
        for (p = 0; p < mem->image.planes; p++)
        {
            if (vxAccessImagePatch(mem->ref,
                                mem->image.rect,
                                p,
                                &mem->image.addr[p],
                                &mem->image.base[p]) == VX_SUCCESS)
            {
                mem->image.addr_mem[p] = clCreateBuffer(clcontext,
                                                        CL_MEM_USE_HOST_PTR,
                                                        sizeof(vx_imagepatch_addressing_t),
                                                        &mem->image.addr[p],
                                                        &err);
                vxcl_error_string("clCreateBuffer for addr ", err);
                mem->image.size[p] = mem->image.addr[p].dim_y * mem->image.addr[p].stride_y;
                mem->mem[p] = clCreateBuffer(clcontext,
                                             CL_MEM_USE_HOST_PTR,
                                             mem->image.size[p],
                                             mem->image.base[p],
                                             &err);
                vxcl_error_string("clCreateBuffer ", err);
                if (err != CL_SUCCESS)
                {
                    status = VX_ERROR_NO_RESOURCES;
                }
            }
            else
            {
                status = VX_ERROR_NO_RESOURCES;
            }
        }
    }
    else if (mem->type == VX_TYPE_BUFFER)
    {
        vxQueryBuffer(mem->ref, VX_BUFFER_ATTRIBUTE_NUMUNITS, &mem->buffer.buf.numUnits, sizeof(mem->buffer.buf.numUnits));
        vxQueryBuffer(mem->ref, VX_BUFFER_ATTRIBUTE_UNITSIZE, &mem->buffer.buf.unitSize, sizeof(mem->buffer.buf.unitSize));
        mem->buffer.base = NULL;
        if (vxAccessBufferRange(mem->ref, 0, mem->buffer.buf.numUnits, &mem->buffer.base) == VX_SUCCESS)
        {
            vx_size size = mem->buffer.buf.numUnits * mem->buffer.buf.unitSize;
            mem->mem[0] = clCreateBuffer(clcontext, CL_MEM_USE_HOST_PTR, size, mem->buffer.base, &err);
            if (err != CL_SUCCESS)
            {
                VX_PRINT(VX_ZONE_ERROR, "clCreateBuffer returned %d\n", err);
                status = VX_ERROR_NO_RESOURCES;
            }
        }
        else
        {
            status = VX_ERROR_NO_RESOURCES;
        }
    }
    else if (mem->type == VX_TYPE_COORDINATES)
    {
        vxQueryCoordinates(mem->ref, VX_COORDINATES_ATTRIBUTE_SIZE, &mem->other.size, sizeof(mem->other.size));
        mem->other.base = malloc(mem->other.size);
        if (vxAccessCoordinates(mem->ref, (vx_int32 *)mem->other.base) == VX_SUCCESS)
        {
            mem->mem[0] = clCreateBuffer(clcontext, CL_MEM_USE_HOST_PTR, mem->other.size, mem->other.base, &err);
            if (err != CL_SUCCESS)
            {
                VX_PRINT(VX_ZONE_ERROR, "clCreateBuffer returned %d\n", err);
                status = VX_ERROR_NO_RESOURCES;
            }
        }
        else
        {
            status = VX_ERROR_NO_RESOURCES;
        }
    }
    else if (mem->type == VX_TYPE_CONVOLUTION)
    {
        vxQueryConvolution(mem->ref, VX_CONVOLUTION_ATTRIBUTE_SIZE, &mem->other.size, sizeof(mem->other.size));
        mem->other.base = malloc(mem->other.size);
        if (vxAccessConvolutionCoefficients(mem->ref, mem->other.base) == VX_SUCCESS)
        {
            mem->mem[0] = clCreateBuffer(clcontext, CL_MEM_USE_HOST_PTR, mem->other.size, mem->other.base, &err);
            if (err != CL_SUCCESS)
            {
                VX_PRINT(VX_ZONE_ERROR, "clCreateBuffer returned %d\n", err);
                status = VX_ERROR_NO_RESOURCES;
            }
        }
        else
        {
            status = VX_ERROR_NO_RESOURCES;
        }
    }
    else if (mem->type == VX_TYPE_DISTRIBUTION)
    {
        vxQueryDistribution(mem->ref, VX_DISTRIBUTION_ATTRIBUTE_SIZE, &mem->other.size, sizeof(mem->other.size));
        mem->other.base = malloc(mem->other.size);
        if (vxAccessDistribution(mem->ref, mem->other.base) == VX_SUCCESS)
        {
            mem->mem[0] = clCreateBuffer(clcontext, CL_MEM_USE_HOST_PTR, mem->other.size, mem->other.base, &err);
            if (err != CL_SUCCESS)
            {
                VX_PRINT(VX_ZONE_ERROR, "clCreateBuffer returned %d\n", err);
                status = VX_ERROR_NO_RESOURCES;
            }
        }
        else
        {
            status = VX_ERROR_NO_RESOURCES;
        }
    }
    else if (mem->type == VX_TYPE_MATRIX)
    {
        vxQueryMatrix(mem->ref, VX_MATRIX_ATTRIBUTE_SIZE, &mem->other.size, sizeof(mem->other.size));
        mem->other.base = malloc(mem->other.size);
        if (vxAccessMatrix(mem->ref, &mem->other.base) == VX_SUCCESS)
        {
            mem->mem[0] = clCreateBuffer(clcontext, CL_MEM_USE_HOST_PTR, mem->other.size, mem->other.base, &err);
            if (err != CL_SUCCESS)
            {
                VX_PRINT(VX_ZONE_ERROR, "clCreateBuffer returned %d\n", err);
                status = VX_ERROR_NO_RESOURCES;
            }
        }
        else
        {
            status = VX_ERROR_NO_RESOURCES;
        }
    }
    else if (mem->type == VX_TYPE_LUT)
    {
        vx_enum type = 0;
        vxQueryLUT(mem->ref, VX_LUT_ATTRIBUTE_COUNT, &mem->buffer.buf.numUnits, sizeof(mem->buffer.buf.numUnits));
        vxQueryLUT(mem->ref, VX_LUT_ATTRIBUTE_TYPE, &type, sizeof(type));
        if (type == VX_TYPE_UINT8)
            mem->buffer.buf.unitSize = sizeof(vx_uint8);
        else if (type == VX_TYPE_INT16 || type == VX_TYPE_UINT16)
            mem->buffer.buf.unitSize = sizeof(vx_uint16);
        mem->other.size = mem->buffer.buf.numUnits * mem->buffer.buf.unitSize;
        mem->other.base = NULL;
        if (vxAccessLUT(mem->ref, &mem->other.base) == VX_SUCCESS)
        {
            mem->mem[0] = clCreateBuffer(clcontext, CL_MEM_USE_HOST_PTR, mem->other.size, mem->other.base, &err);
            if (err != CL_SUCCESS)
            {
                VX_PRINT(VX_ZONE_ERROR, "clCreateBuffer returned %d\n", err);
                status = VX_ERROR_NO_RESOURCES;
            }
        }
        else
        {
            status = VX_ERROR_NO_RESOURCES;
        }
    }
    return status;
}

static vx_cl_kernel_description_t *cl_kernels[] = {
    &box3x3_clkernel,
    &gaussian3x3_clkernel,
    &lut_clkernel,
};

static vx_uint32 num_cl_kernels = dimof(cl_kernels);

static size_t flines(FILE *fp)
{
    size_t numLines = 0;
    char line[VX_CL_MAX_LINE_WIDTH];
    fseek(fp, 0, SEEK_SET);
    while (fgets(line, sizeof(line), fp) > 0) {
        numLines++;
    }
    fseek(fp, 0, SEEK_SET);
    return numLines;
}

static char **clLoadSources(char *filename, size_t **lengths, cl_uint *pNumLines)
{
    FILE *fp = NULL;
    cl_uint numLines = 0;
    char **lines = NULL;
    cl_int i;
    VX_PRINT(VX_ZONE_INFO, "Reading source file %s\n", filename);
    fp = fopen((char *)filename, "r");
    if (fp != NULL && lengths && pNumLines)
    {
        numLines = (cl_uint)flines(fp);
        *pNumLines = numLines;
        lines = (char **)calloc(numLines,sizeof(char *));
        *lengths = (vx_size *)calloc(numLines,sizeof(vx_size));
        if (lines != NULL && *lengths != NULL)
        {
            for (i = 0; i < (cl_int)numLines; i++)
            {
                lines[i] = (char *)calloc(VX_CL_MAX_LINE_WIDTH, sizeof(char));
                if (lines[i])
                {
                    fgets(lines[i], VX_CL_MAX_LINE_WIDTH, fp);
                    (*lengths)[i] = strlen(lines[i]);
                    VX_PRINT(VX_ZONE_INFO, "line[%05u]: %s", (*lengths)[i], lines[i]);
                }
                else
                {
                    for (i-=1; i >= 0; i--)
                        free(lines[i]);
                    free(lines);
                    lines = NULL;
                    break;
                }
            }
            VX_PRINT(VX_ZONE_INFO, "\n");
        }
    }
    return lines;
}

static void vxcl_platform_notifier(const char *errinfo,
                                const void *private_info,
                                size_t cb,
                                void *user_data)
{
    /* vx_cl_context_t *vxcl = (vx_cl_context_t *)user_data; */
    VX_PRINT(VX_ZONE_ERROR, "%s\n", errinfo);
}

static void vxcl_error_string(char *prefix, cl_int err)
{
    switch (err)
    {
        case CL_SUCCESS:
            VX_PRINT(VX_ZONE_INFO, "%s: Success\n", prefix);
            break;
        case CL_DEVICE_NOT_AVAILABLE:
            VX_PRINT(VX_ZONE_ERROR, "%s: Device not available\n", prefix);
            break;
        case CL_BUILD_PROGRAM_FAILURE:
            VX_PRINT(VX_ZONE_ERROR, "%s: Build Failure!\n", prefix);
            break;
        case CL_INVALID_CONTEXT :
            VX_PRINT(VX_ZONE_ERROR, "%s: Invalid Context\n", prefix);
            break;
        case CL_INVALID_VALUE:
            VX_PRINT(VX_ZONE_ERROR, "%s: Invalid Value\n", prefix);
            break;
        case CL_OUT_OF_HOST_MEMORY:
            VX_PRINT(VX_ZONE_ERROR, "%s: Out of Memory\n", prefix);
            break;
        case CL_INVALID_DEVICE:
            VX_PRINT(VX_ZONE_ERROR, "%s: Invalid device!\n", prefix);
            break;
        case CL_INVALID_BINARY:
            VX_PRINT(VX_ZONE_ERROR, "%s: Invalid binary!\n", prefix);
            break;
        case CL_INVALID_PROGRAM:
            VX_PRINT(VX_ZONE_ERROR, "%s: Invalid program\n", prefix);
            break;
        case CL_INVALID_BUILD_OPTIONS:
            VX_PRINT(VX_ZONE_ERROR, "%s: Invalid build options!\n", prefix);
            break;
        case CL_INVALID_COMMAND_QUEUE:
            VX_PRINT(VX_ZONE_ERROR, "%s: Invalid queue!\n", prefix);
            break;
        case CL_INVALID_MEM_OBJECT:
            VX_PRINT(VX_ZONE_ERROR, "%s: Invalid memory object!\n", prefix);
            break;
        case CL_INVALID_KERNEL_ARGS:
            VX_PRINT(VX_ZONE_ERROR, "%s: Invalid kernel args!\n", prefix);
            break;
        case CL_INVALID_ARG_INDEX:
            VX_PRINT(VX_ZONE_ERROR, "%s: Invalid arg index!\n", prefix);
            break;
        case CL_INVALID_ARG_VALUE:
            VX_PRINT(VX_ZONE_ERROR, "%s: Invalid arg value!\n", prefix);
            break;
        case CL_INVALID_ARG_SIZE:
            VX_PRINT(VX_ZONE_ERROR, "%s: Invalid arg size!\n", prefix);
            break;
        default:
            VX_PRINT(VX_ZONE_ERROR, "%s: Unknown error %d\n", prefix, err);
            break;
    }
}

static char *cl_args = VX_CL_ARGS""FILE_JOINER;

vx_status vxTargetInit(vx_target_t *target)
{
    vx_status status = VX_ERROR_NO_RESOURCES;
    cl_int err = 0;
    vx_cl_context_t *vxcl = NULL;
    cl_uint dev_types = CL_DEVICE_TYPE_ALL;

    strncpy(target->name, name, VX_MAX_TARGET_NAME);
    target->priority = VX_TARGET_PRIORITY_OPENCL;
    vxcl = VX_CALLOC(vx_cl_context_t);
    if (vxcl)
    {
        vxcl->kernels = cl_kernels;
        vxcl->num_kernels = num_cl_kernels;
        /* get the number of platforms and the ids */
        err = clGetPlatformIDs(VX_CL_MAX_PLATFORMS, vxcl->platform, &vxcl->num_platforms);
        VX_PRINT(VX_ZONE_TARGET, "%u platforms available!\n", vxcl->num_platforms);
        if (err == CL_SUCCESS)
        {
            cl_uint p = 0, d = 0;
            vx_uint32 k = 0;
            for (p = 0; p < vxcl->num_platforms; p++)
            {
                /* get the number of devices on this platform. */
                err = clGetDeviceIDs(vxcl->platform[p], dev_types, VX_CL_MAX_DEVICES, vxcl->devices[p], &vxcl->num_devices[p]);
                VX_PRINT(VX_ZONE_TARGET, "\t%u devices available!\n", vxcl->num_devices[p]);
                if (err == CL_SUCCESS)
                {
                    cl_context_properties props[] = {
                        (cl_context_properties)CL_CONTEXT_PLATFORM,
                        (cl_context_properties)vxcl->platform[p],
                        (cl_context_properties)0,
                    };

                    for (d = 0; d < vxcl->num_devices[p]; d++)
                    {
                        vx_char deviceName[VX_MAX_TARGET_NAME];
                        vx_size dNlen = 0;

                        memset(deviceName, 0, sizeof(deviceName));
                        /* get the name of the device */
                        err = clGetDeviceInfo(vxcl->devices[p][d], CL_DEVICE_NAME, VX_MAX_TARGET_NAME, deviceName, &dNlen);
                        if (err == CL_SUCCESS)
                        {
                            VX_PRINT(VX_ZONE_TARGET, "\tdevice[%u] %p = %s\n", d, vxcl->devices[p][d], deviceName);
                        }
                    }
                    /* create a context for this platform */
                    vxcl->context[p] = clCreateContext(props,
                                                       vxcl->num_devices[p],
                                                       vxcl->devices[p],
                                                       vxcl_platform_notifier,
                                                       vxcl,
                                                       &err);
                    if (err == CL_SUCCESS)
                    {
                        status = VX_SUCCESS;
                        target->reserved = vxcl;
                    }

                    /* create a queue for each device */
                    for (d = 0; d < vxcl->num_devices[p]; d++)
                    {
                        vxcl->queues[p][d] = clCreateCommandQueue(vxcl->context[p],
                                                                  vxcl->devices[p][d],
                                                                  CL_QUEUE_PROFILING_ENABLE,
                                                                  &err);
                        if (err != CL_SUCCESS)
                        {
                            VX_PRINT(VX_ZONE_ERROR, "Queue returned %d\n", err);
                        }
                    }
                    /* for each kernel */
                    for (k = 0; k < num_cl_kernels; k++)
                    {
                        char **sources   = NULL;
                        cl_uint num_lines = 0;
                        size_t *lengths  = NULL;

                        /* load the source file */
                        VX_PRINT(VX_ZONE_INFO, "Joiner: %s\n", FILE_JOINER);
                        VX_PRINT(VX_ZONE_INFO, "Path: %s\n", VX_CL_SOURCEPATH);
                        VX_PRINT(VX_ZONE_INFO, "Kernel[%u] File: %s\n", k, cl_kernels[k]->sourcepath);
                        VX_PRINT(VX_ZONE_INFO, "Kernel[%u] Name: %s\n", k, cl_kernels[k]->kernelname);
                        VX_PRINT(VX_ZONE_INFO, "Kernel[%u] ID: %s\n", k, cl_kernels[k]->description.name);
                        sources = clLoadSources(cl_kernels[k]->sourcepath, &lengths, &num_lines);
                        VX_PRINT(VX_ZONE_INFO, "Number of Lines "VX_FMT_SIZE" in file %s\n", num_lines, cl_kernels[k]->sourcepath);
                        /* create a program with this source */
                        cl_kernels[k]->program[p] = clCreateProgramWithSource(vxcl->context[p],
                                                                     num_lines,
                                                                     (const char **)sources,
                                                                     lengths,
                                                                     &err);
                        if (err == CL_SUCCESS && cl_kernels[k]->program[p])
                        {
                            /* compile for each device in the platform. */
                            VX_PRINT(VX_ZONE_INFO, "Building for %u devices, with options %s\n", vxcl->num_devices[p], cl_args);
                            for (d = 0; d < vxcl->num_devices[p]; d++)
                            {
                                cl_bool compiler = CL_FALSE;
                                clGetDeviceInfo(vxcl->devices[p][d], CL_DEVICE_COMPILER_AVAILABLE, sizeof(cl_bool), &compiler, NULL);
                                VX_PRINT(VX_ZONE_INFO, "Device %p has compiler %s\n",
                                        vxcl->devices[p][d],
                                        (compiler?"TRUE":"FALSE"));

                            }
                            err = clBuildProgram(cl_kernels[k]->program[p],
                                                 vxcl->num_devices[p],
                                                 vxcl->devices[p],
                                                 (const char *)cl_args,
                                                 NULL,
                                                 NULL);
                            if (err != CL_SUCCESS)
                            {
                                vxcl_error_string("Build Error", err);
                                if (err == CL_BUILD_PROGRAM_FAILURE)
                                {
                                    char log[10][1024];
                                    size_t logSize = 0;
                                    clGetProgramBuildInfo(cl_kernels[k]->program[p],
                                                          vxcl->devices[p][0],
                                                          CL_PROGRAM_BUILD_LOG,
                                                          sizeof(log),
                                                          log,
                                                          &logSize);
                                    VX_PRINT(VX_ZONE_ERROR, "%s",log);
                                }
                            }
                            else
                            {
                                cl_int k2 = 0;
                                cl_build_status bstatus = 0;
                                size_t bs = 0;
                                err = clGetProgramBuildInfo(cl_kernels[k]->program[p],
                                                            vxcl->devices[p][0],
                                                            CL_PROGRAM_BUILD_STATUS,
                                                            sizeof(cl_build_status),
                                                            &bstatus,
                                                            &bs);
                                VX_PRINT(VX_ZONE_INFO, "Status = %d (%d)\n", bstatus,err);
                                /* get the cl_kernels from the program */
                                err = clCreateKernelsInProgram(cl_kernels[k]->program[p],
                                                               1,
                                                               &cl_kernels[k]->kernels[p],
                                                               &cl_kernels[k]->num_kernels[p]);
                                VX_PRINT(VX_ZONE_INFO, "Found %u cl_kernels in %s (%d)\n", cl_kernels[k]->num_kernels[p], cl_kernels[k]->sourcepath, err);
                                for (k2 = 0; (err == CL_SUCCESS) && (k2 < (cl_int)cl_kernels[k]->num_kernels[p]); k2++)
                                {
                                    char kName[VX_MAX_KERNEL_NAME];
                                    size_t size = 0;
                                    err = clGetKernelInfo(cl_kernels[k]->kernels[p],
                                                    CL_KERNEL_FUNCTION_NAME,
                                                    sizeof(kName),
                                                    kName,
                                                    &size);
                                    VX_PRINT(VX_ZONE_INFO, "Kernel %s\n", kName);
                                    if (strncmp(kName, cl_kernels[k]->kernelname, VX_MAX_KERNEL_NAME) == 0)
                                    {
                                        vx_kernel_f kfunc = cl_kernels[k]->description.function;
                                        VX_PRINT(VX_ZONE_INFO, "Linked Kernel %s on target %s\n", cl_kernels[k]->kernelname, target->name);
                                        target->numKernels++;
                                        target->base.context->numKernels++;
                                        vxInitializeKernel(target->base.context,
                                                &target->kernels[k],
                                                cl_kernels[k]->description.enumeration,
                                                (kfunc == NULL ? vxclCallOpenCLKernel : kfunc),
                                                cl_kernels[k]->description.name,
                                                cl_kernels[k]->description.parameters,
                                                cl_kernels[k]->description.numParams,
                                                cl_kernels[k]->description.input_validate,
                                                cl_kernels[k]->description.output_validate,
                                                cl_kernels[k]->description.initialize,
                                                cl_kernels[k]->description.deinitialize);
                                        status = VX_SUCCESS;
                                    }
                                }
                            }
                        }
                        else
                        {
                            vxcl_error_string("Program", err);
                        }
                        free(sources);
                        free(lengths);
                    }
                }
            }
        }
    }
    if (vxcl && status != VX_SUCCESS)
    {
        free(vxcl);
    }
    return status;
}

vx_status vxTargetDeinit(vx_target_t *target)
{
    vx_cl_context_t *vxcl = (vx_cl_context_t *)target->reserved;
    if (vxcl)
    {
        cl_uint p = 0, d = 0;
        vx_uint32 k = 0;
        for (p = 0; p < vxcl->num_platforms; p++)
        {
            for (k = 0; k < num_cl_kernels; k++)
            {
                vx_kernel_t *kern = &target->kernels[k];
                vxReleaseKernel((vx_kernel *)&kern);
                clReleaseKernel(cl_kernels[k]->kernels[p]);
                clReleaseProgram(cl_kernels[k]->program[p]);

            }
            for (d = 0; d < vxcl->num_devices[p]; d++)
            {
                clReleaseCommandQueue(vxcl->queues[p][d]);
            }
            clReleaseContext(vxcl->context[p]);
        }
    }
    return VX_SUCCESS;
}

vx_status vxTargetSupports(vx_target_t *target,
                           vx_char targetName[VX_MAX_TARGET_NAME],
                           vx_char kernelName[VX_MAX_KERNEL_NAME])
{
    vx_status status = VX_FAILURE;
    if (strncmp(targetName, name, VX_MAX_TARGET_NAME) == 0 ||
        strncmp(targetName, "khronos.c_model", VX_MAX_TARGET_NAME) == 0)
    {
        vx_uint32 k = 0u;
        for (k = 0u; k < target->numKernels; k++)
        {
            if (strncmp(kernelName, target->kernels[k].name, VX_MAX_KERNEL_NAME) == 0)
            {
                status = VX_SUCCESS;
                break;
            }
        }
    }
    return status;
}

vx_action vxTargetProcess(vx_target_t *target, vx_node_t *nodes[], vx_size startIndex, vx_size numNodes)
{
    vx_action action = VX_ACTION_CONTINUE;
    vx_status status = VX_SUCCESS;
    vx_size n = 0;
    for (n = startIndex; (n < (startIndex + numNodes)) && (action == VX_ACTION_CONTINUE); n++)
    {
        VX_PRINT(VX_ZONE_GRAPH,"Executing Kernel %s:%d in Nodes[%u] on target %s\n",
            nodes[n]->kernel->name,
            nodes[n]->kernel->enumeration,
            n,
            nodes[n]->base.context->targets[nodes[n]->affinity].name);

        vxStartCapture(&nodes[n]->perf);
        status = nodes[n]->kernel->function((vx_node)nodes[n],
                                            (vx_reference *)nodes[n]->parameters,
                                            nodes[n]->kernel->signature.numParams);
        nodes[n]->executed = vx_true_e;
        nodes[n]->status = status;
        vxStopCapture(&nodes[n]->perf);

        VX_PRINT(VX_ZONE_GRAPH,"kernel %s returned %d\n", nodes[n]->kernel->name, status);

        if (status == VX_SUCCESS)
        {
            /* call the callback if it is attached */
            if (nodes[n]->callback)
            {
                action = nodes[n]->callback((vx_node)nodes[n]);
                VX_PRINT(VX_ZONE_GRAPH,"callback returned action %d\n", action);
            }
        }
        else
        {
            action = VX_ACTION_ABANDON;
            VX_PRINT(VX_ZONE_ERROR, "Abandoning Graph due to error (%d)!\n", status);
        }
    }
    return action;
}

vx_status vxTargetVerify(vx_target_t *target, vx_node_t *node)
{
    vx_status status = VX_SUCCESS;
    return status;
}

vx_kernel vxTargetAddKernel(vx_target_t *target,
                            vx_char name[VX_MAX_KERNEL_NAME],
                            vx_enum enumeration,
                            vx_kernel_f func_ptr,
                            vx_uint32 numParams,
                            vx_kernel_input_validate_f input,
                            vx_kernel_output_validate_f output,
                            vx_kernel_initialize_f initialize,
                            vx_kernel_deinitialize_f deinitialize)
{
    vx_uint32 k = 0u;
    vx_kernel_t *kernel = NULL;
    for (k = target->numKernels; k < VX_INT_MAX_KERNELS; k++)
    {
        kernel = &(target->kernels[k]);
        if ((kernel->enabled == vx_false_e) &&
            (kernel->enumeration == VX_KERNEL_INVALID))
        {
            vxInitializeKernel(target->base.context,
                               kernel,
                               enumeration, func_ptr, name,
                               NULL, numParams,
                               input, output, initialize, deinitialize);
            VX_PRINT(VX_ZONE_KERNEL, "Reserving %s Kernel[%u] for %s\n", target->name, k, kernel->name);
            target->numKernels++;
            break;
        }
        kernel = NULL;
    }
    return (vx_kernel)kernel;
}

vx_cl_kernel_description_t *vxclFindKernel(vx_enum enumeration)
{
    vx_cl_kernel_description_t *vxclk = NULL;
    vx_uint32 k;
    for (k = 0; k < num_cl_kernels; k++)
    {
        if (enumeration == cl_kernels[k]->description.enumeration)
        {
            vxclk = cl_kernels[k];
            break;
        }
    }
    return vxclk;
}

/*! \brief Calls an OpenCL kernel from OpenVX Graph.
 * Steps:
 * \arg Find the target
 * \arg Get the vxcl context
 * \arg Find the kernel (to get cl kernel information)
 * \arg for each input parameter that is an object, enqueue write
 * \arg wait for finish
 * \arg for each parameter, SetKernelArg
 * \arg call kernel
 * \arg wait for finish
 * \arg for each output parameter that is an object, enqueue read
 * \arg wait for finish
 * \note This implementation will attempt to use the External API as much as possible,
 * but will cast to internal representation when needed (due to lack of API or
 * need for secret information). This is not an optimal OpenCL invocation.
 */
vx_status vxclCallOpenCLKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    vx_node_t *pnode = (vx_node_t *)node;
    vx_target_t *target = (vx_target_t *)&pnode->base.context->targets[pnode->affinity];
    vx_cl_context_t *vxcl = (vx_cl_context_t*)target->reserved;
    vx_cl_kernel_description_t *vxclk = vxclFindKernel(pnode->kernel->enumeration);
    vx_uint32 i, p;
    vx_cl_mem_t data[VX_INT_MAX_PARAMS];
    cl_int err = 0;
    size_t off_dim[3] = {0,0,0};
    size_t work_dim[3];
    //size_t local_dim[3];
    cl_event writeEvents[VX_INT_MAX_PARAMS];
    cl_event readEvents[VX_INT_MAX_PARAMS];
    cl_event kEvent;
    cl_int we = 0, re = 0;

    if (vxcl == NULL)
        return VX_ERROR_INVALID_NODE;

    vxSemWait(&target->base.lock);
    memset(data, 0, sizeof(data));

    /* for each parameter, initialize the data array */
    VX_PRINT(VX_ZONE_TARGET, "Calling OpenCL Kernel %s with context %p\n", vxclk->kernelname, vxcl);
    for (p = 0; p < num; p++)
    {
        vx_parameter param = vxGetParameterByIndex(node, p);
        if (param)
        {
            /* get the direction and type */
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_DIRECTION, &data[p].dir, sizeof(data[p].dir));
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_TYPE, &data[p].type, sizeof(data[p].type));
            data[p].ref = parameters[p];
            VX_PRINT(VX_ZONE_TARGET, "Parameter[%u] is dir:%d type:%d ref:"VX_FMT_REF"\n", p, data[p].dir, data[p].type, data[p].ref);
            status = vxclCreateMem(vxGetContext(node), vxcl->context[0], &data[p]);
            if (status != VX_SUCCESS)
            {
                goto exit;
            }
            vxReleaseParameter(&param);
        }
    }
    /* for each input/bi data object, enqueue it */
    for (p = 0; p < num; p++)
    {
        if (data[p].dir == VX_INPUT || data[p].dir == VX_BIDIRECTIONAL)
        {
            /* create a cl_mem from the data, but it has to be tracked and closed later */
            if (data[p].type == VX_TYPE_IMAGE)
            {
                for (i = 0; i < data[p].image.planes; i++)
                {
                    err = clEnqueueWriteBuffer(vxcl->queues[0][0],
                            data[p].image.addr_mem[i],
                            CL_TRUE, 0, sizeof(vx_imagepatch_addressing_t),
                            &data[p].image.addr[i],
                            0, NULL, &writeEvents[we++]);
                    vxcl_error_string("Enqueue Write Addressing Struct", err);
                    err = clEnqueueWriteBuffer(vxcl->queues[0][0],
                            data[p].mem[i],
                            CL_TRUE, 0, data[p].image.size[i],
                            data[p].image.base[i],
                            0, NULL, &writeEvents[we++]);
                    vxcl_error_string("Enqueue Write Plane", err);
                    VX_PRINT(VX_ZONE_TARGET, "Enqueued image:"VX_FMT_REF" plane[%d] size:"VX_FMT_SIZE" ptr=%p\n", data[p].ref, i, data[p].image.size[i], data[p].image.base[i]);
                }
            }
            else if (data[p].type == VX_TYPE_BUFFER)
            {
                err = clEnqueueWriteBuffer(vxcl->queues[0][0],
                        data[p].mem[0],
                        CL_TRUE, 0, data[p].buffer.buf.numUnits * data[p].buffer.buf.unitSize,
                        data[p].buffer.base,
                        0, NULL, &writeEvents[we++]);
                vxcl_error_string("Enqueue Write Buffer", err);
                VX_PRINT(VX_ZONE_TARGET, "Enqueued buffer "VX_FMT_REF"\n", data[p].ref);
            }
            /*! \todo must include other VX types in OpenCL */
            else
            {
                err = clEnqueueWriteBuffer(vxcl->queues[0][0],
                        data[p].mem[0],
                        CL_TRUE, 0, data[p].other.size,
                        data[p].other.base,
                        0, NULL, &writeEvents[we++]);
                vxcl_error_string("Enqueue Write Object", err);
                VX_PRINT(VX_ZONE_TARGET, "Enqueued object "VX_FMT_REF"\n", data[p].ref);
            }
        }
        if (data[p].dir == VX_OUTPUT)
        {
            if (data[p].type == VX_TYPE_IMAGE)
            {
                for (i = 0; i < data[p].image.planes; i++)
                {
                    err = clEnqueueWriteBuffer(vxcl->queues[0][0],
                            data[p].image.addr_mem[i],
                            CL_TRUE, 0, sizeof(vx_imagepatch_addressing_t),
                            &data[p].image.addr[i],
                            0, NULL, &writeEvents[we++]);
                    vxcl_error_string("Enqueue Write Addressing Struct", err);
                }
            }
        }
    }
    /*****************************************/
    /* for each parameter, set as kernel arg */
    /*****************************************/
    i = 0;
    for (p = 0; p < num; p++)
    {
        VX_PRINT(VX_ZONE_TARGET, "Setting Parameter: %u\n", p);
        if (data[p].type == VX_TYPE_IMAGE)
        {
            vx_uint32 p2 = 0;
            for (p2 = 0; p2 < data[p].image.planes; p2++)
            {
                err = clSetKernelArg(vxclk->kernels[0], i, sizeof(cl_mem), &data[p].image.addr_mem[p2]);
                i++;
                vxcl_error_string("SetKernelArg for addr", err);
                err = clSetKernelArg(vxclk->kernels[0], i, sizeof(cl_mem), &data[p].mem[p2]);
                vxcl_error_string("SetKernelArg for plane", err);
            }
        }
        else if (data[p].type == VX_TYPE_BUFFER || data[p].type == VX_TYPE_LUT)
        {
            err = clSetKernelArg(vxclk->kernels[0], i, sizeof(size_t), &data[p].buffer.buf.unitSize);
            vxcl_error_string("SetKernelArg for buffer unitSize", err);
            i++;
            err = clSetKernelArg(vxclk->kernels[0], i, sizeof(size_t), &data[p].buffer.buf.numUnits);
            vxcl_error_string("SetKernelArg for buffer numUnits", err);
            i++;
            err = clSetKernelArg(vxclk->kernels[0], i, sizeof(cl_mem), &data[p].mem[0]);
            vxcl_error_string("SetKernelArg for buffer", err);
        }
        else
        {
            err = clSetKernelArg(vxclk->kernels[0], i, sizeof(cl_mem), &data[p].mem[0]);
            vxcl_error_string("SetKernelArg for other", err);
        }
        i++;
    }

    /* call the kernel */
    work_dim[0] = data[0].image.width;
    work_dim[1] = data[0].image.height;
    //local_dim[0] = 1;
    //local_dim[1] = 1;
    err = clEnqueueNDRangeKernel(vxcl->queues[0][0],
                                 vxclk->kernels[0],
                                 2,
                                 off_dim,
                                 work_dim,
                                 NULL,
                                 we, writeEvents, &kEvent);
    vxcl_error_string("clEnqueueNDRangeKernel", err);
    /* enqueue a read on all output data */
    for (p = 0; p < num; p++)
    {
        if (data[p].dir == VX_OUTPUT || data[p].dir == VX_BIDIRECTIONAL)
        {
            if (data[p].type == VX_TYPE_IMAGE)
            {
                for (i = 0; i < data[p].image.planes; i++)
                {
                    err = clEnqueueReadBuffer(vxcl->queues[0][0],
                            data[p].mem[i],
                            CL_TRUE, 0, data[p].image.size[i],
                            data[p].image.base[i],
                            1, &kEvent, &readEvents[re++]);
                    vxcl_error_string("Enqueue Read Image: ", err);
                }
            }
            else if (data[p].type == VX_TYPE_BUFFER)
            {
                err = clEnqueueReadBuffer(vxcl->queues[0][0],
                        data[p].mem[0],
                        CL_TRUE, 0, data[p].buffer.buf.numUnits * data[p].buffer.buf.unitSize,
                        data[p].buffer.base,
                        1, &kEvent, &readEvents[re++]);
                vxcl_error_string("Enqueue Write Buffer: ", err);
            }
            else
            {
                err = clEnqueueReadBuffer(vxcl->queues[0][0],
                        data[p].mem[0],
                        CL_TRUE, 0, data[p].other.size,
                        data[p].other.base,
                        1, &kEvent, &readEvents[re++]);
                vxcl_error_string("Enqueue Read Object: ", err);
            }
        }
    }
    err = clFlush(vxcl->queues[0][0]);
    vxcl_error_string("Flush", err);
    VX_PRINT(VX_ZONE_TARGET, "Waiting for read events!\n");
    clWaitForEvents(re, readEvents);
    VX_PRINT(VX_ZONE_TARGET, "Releasing Events\n");
    for (i = 0; i < (vx_uint32)we; i++)
    {
        clReleaseEvent(writeEvents[i]);
    }
    clReleaseEvent(kEvent);
    for (i = 0; i < (vx_uint32)re; i++)
    {
        clReleaseEvent(readEvents[i]);
    }
exit:
    for (p = 0; p < num; p++)
    {
        vxclDestroyMem(&data[p]);
    }
    VX_PRINT(VX_ZONE_API, "%s exiting %d\n", __FUNCTION__, status);
    vxSemPost(&target->base.lock);
    return status;
}
