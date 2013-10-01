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

#include <VX/vx.h>
#include <string.h>

vx_status example_bufferrange(vx_context context)
{
    vx_status status = VX_SUCCESS;
    vx_size totalsize = 1024, subsize;
    vx_size start = 20, end = 512;
    vx_buffer buffer = vxCreateBuffer(context, sizeof(vx_uint8), totalsize);
    void *ptr = NULL;

    /* how many bytes are in the the buffer range? */
    subsize = vxComputeBufferRangeSize(buffer, start, end);

    /* we could allocate the sub-buffer or leave it up to OpenVX */
#if defined(USER_COPY_OF_BUFFER)
    /* we want to make our own malloc'd pointer */
    ptr = malloc(subsize);
#else
    /* OpenVX will decide how to give access since ptr = NULL. */
#endif

    /* request access to the buffer data */
    status = vxAccessBufferRange(buffer, start, end, &ptr);

    /* initialize to a known value */
    memset(ptr, 42, subsize);

    /* commit the data back to the object */
    status = vxCommitBufferRange(buffer, start, end, ptr);

    return status;
}
