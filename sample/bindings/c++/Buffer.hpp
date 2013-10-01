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

#ifndef _VX_BUFFER_HPP_
#define _VX_BUFFER_HPP_

namespace OpenVX {
    class Buffer: public Reference {
        friend class Context;
        friend class Delay<Buffer>;
    protected:
        static Buffer *wrap(vx_buffer ref) {
            Reference *r = new Reference(ref);
            return dynamic_cast<Buffer *>(r);
        }
    public:
        Buffer();
        Buffer(vx_size unitSize, vx_size numUnits);
        ~Buffer();

        vx_size size();
        vx_size numUnits();
        vx_size unitSize();

        vx_status GetBufferRange(vx_uint32 su, vx_uint32 eu, void **ptr);
        vx_status SetBufferRange(vx_uint32 su, vx_uint32 eu, void *ptr);
    };
};
#endif

