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

#include <vx.hpp>
#include <stdlib.h>

using namespace OpenVX;

Target::Target(vx_uint32 idx) : Reference(vxGetTargetByIndex(Context::getContext().handle(), idx))
{
    table = NULL;
    if (handle())
    {
        vxQueryTarget(handle(), VX_TARGET_ATTRIBUTE_INDEX, &target_index, sizeof(target_index));
        vxQueryTarget(handle(), VX_TARGET_ATTRIBUTE_NAME,  &target_name, sizeof(target_name));
        vxQueryTarget(handle(), VX_TARGET_ATTRIBUTE_NUMKERNELS, &target_numKernels, sizeof(target_numKernels));
    }
}

Target::~Target()
{
    vxReleaseTarget(&m_handle);
}

vx_char *Target::name()
{
    return target_name;
}

vx_uint32 Target::index()
{
    return target_index;
}

vx_uint32 Target::numKernels()
{
    return target_numKernels;
}

vx_target_kernel_t *Target::kernelTable()
{
    vx_uint32 newnum = 0;
    vxQueryTarget(handle(), VX_TARGET_ATTRIBUTE_NUMKERNELS, &newnum, sizeof(newnum));
    if (newnum != target_numKernels)
    {
        free(table);
        table = NULL;
        target_numKernels = 0;
    }
    if (table == NULL)
    {
        target_numKernels = newnum;
        table = (vx_target_kernel_t *)calloc(target_numKernels, sizeof(vx_target_kernel_t));
        if (table)
        {
            /* this fills in the values in the table */
            vxQueryTarget(handle(), VX_TARGET_ATTRIBUTE_KERNELTABLE, table, sizeof(vx_target_kernel_t) * target_numKernels);
        }
    }
    return table;
}

