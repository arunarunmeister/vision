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
void vx_gaussian3x3(__global vx_cl_imagepatch_addressing_t *src_addr,
                    __global void *src,
                    __global vx_cl_imagepatch_addressing_t *dst_addr,
                    __global void *dst)
{
    const int x = get_global_id(0);
    const int y = get_global_id(1);
    const size_t w = get_global_size(0);
    const size_t h = get_global_size(1);
    uint sum = 0;
    //printf("x,y={%d,%d} w=%zu h=%zu\n", x, y, w, h);
    if (y == 0 || x == 0 || x == (w - 1) || y == (h - 1))
        return;
    sum += 1*(uint)vxImagePixel(uchar, src, x-1, y-1, src_addr);
    sum += 2*(uint)vxImagePixel(uchar, src, x+0, y-1, src_addr);
    sum += 1*(uint)vxImagePixel(uchar, src, x+1, y-1, src_addr);
    sum += 2*(uint)vxImagePixel(uchar, src, x-1, y+0, src_addr);
    sum += 4*(uint)vxImagePixel(uchar, src, x+0, y+0, src_addr);
    sum += 2*(uint)vxImagePixel(uchar, src, x+1, y+0, src_addr);
    sum += 1*(uint)vxImagePixel(uchar, src, x-1, y+1, src_addr);
    sum += 2*(uint)vxImagePixel(uchar, src, x+0, y+1, src_addr);
    sum += 1*(uint)vxImagePixel(uchar, src, x+1, y+1, src_addr);
    sum /= 16;
    vxImagePixel(uchar, dst, x, y, dst_addr) = sum;
}

