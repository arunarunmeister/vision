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

vx_bool vxFreeMemory(vx_context_t *context, vx_memory_t *memory)
{
    if (memory->allocated == vx_true_e)
    {
        vx_int32 p = 0u;
        vxPrintMemory(memory);
        for (p = 0; p < memory->nptrs; p++)
        {
            if (memory->ptrs[p])
            {
                VX_PRINT(VX_ZONE_INFO, "Freeing %p\n", memory->ptrs[p]);
                free(memory->ptrs[p]);
                memory->ptrs[p] = NULL;
            }
        }
        memory->allocated = vx_false_e;
    }
    return memory->allocated;
}


vx_bool vxAllocateMemory(vx_context_t *context, vx_memory_t *memory)
{
    if (memory->allocated == vx_false_e)
    {
        vx_int32 d = 0, p = 0;
        VX_PRINT(VX_ZONE_INFO, "Allocating %u pointers of %u dimensions each.\n", memory->nptrs, memory->ndims);
        memory->allocated = vx_true_e;
        for (p = 0; p < memory->nptrs; p++)
        {
            vx_size size = 1ul;
            for (d = 0; d < memory->ndims; d++)
            {
                memory->strides[p][d] = (vx_int32)size;
                size *= (vx_size)abs(memory->dims[p][d]);
            }
            memory->ptrs[p] = calloc(1ul, size);
            if (memory->ptrs[p] == NULL)
            {
                VX_PRINT(VX_ZONE_ERROR, "Failed to allocated "VX_FMT_SIZE" bytes\n", size);
                /* unroll */
                memory->allocated = vx_false_e;
                for (p = p - 1; p >= 0; p--)
                {
                    VX_PRINT(VX_ZONE_INFO, "Freeing %p\n", memory->ptrs[p]);
                    free(memory->ptrs[p]);
                    memory->ptrs[p] = NULL;
                }
                break;
            }
            else
            {
                VX_PRINT(VX_ZONE_INFO, "Allocated %p for "VX_FMT_SIZE" bytes\n", memory->ptrs[p], size);
            }
        }
        vxPrintMemory(memory);
    }
    return memory->allocated;
}

void vxPrintMemory(vx_memory_t *mem)
{
    vx_int32 d = 0, p = 0;
    for (p = 0; p < mem->nptrs; p++)
    {
        VX_PRINT(VX_ZONE_INFO, "ptr[%u]=%p\n", p, mem->ptrs[p]);
        for (d = 0; d < mem->ndims; d++)
        {
            VX_PRINT(VX_ZONE_INFO, "\tdim[%u][%u]=%d strides[%u][%u]=%d\n", p, d, mem->dims[p][d], p, d, mem->strides[p][d]);
        }
    }
}

