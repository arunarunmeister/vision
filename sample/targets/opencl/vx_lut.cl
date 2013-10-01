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

#include <VX/vx_ext_opencl.h>

__kernel
void vx_lut(__global vx_cl_imagepatch_addressing_t *src_addr,
            __global void *src,
            __global vx_cl_buffer_t *lut_buf,
            __global void *lut_base,
            __global vx_cl_imagepatch_addressing_t *dst_addr,
            __global void *dst)
{
    const int x = get_global_id(0);
    const int y = get_global_id(1);
    size_t index = 0;
    if (src_addr->stride_x == 1 && lut_buf->unitSize == 1)
    {
        index = (size_t)vxImagePixel(uchar, src, x, y, src_addr);
        if (index < 256 && index < lut_buf->numUnits)
        {
            uchar *lut = (uchar *)lut_base;
            vxImagePixel(uchar, dst, x, y, dst_addr) = lut[index];
        }
    }
    else if (src_addr->stride_x == 2 && lut_buf->unitSize == 2)
    {
        index = (size_t)vxImagePixel(short, src, x, y, src_addr);
        if (index < 65535 && index < lut_buf->numUnits)
        {
            short *lut = (short *)lut_base;
            vxImagePixel(short, dst, x, y, dst_addr) = lut[index];
        }
    }
}


