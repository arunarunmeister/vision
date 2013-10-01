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

#ifndef _VX_DELAY_HPP_
#define _VX_DELAY_HPP_

#include <vx.hpp>

namespace OpenVX {
    template <> class Delay<Buffer> : public Reference {
        friend class Context;
    protected:
        Delay<Buffer>(vx_size unitSize, vx_size numUnits, vx_size num)
         : Reference(vxCreateBufferDelay(Context::getContext().handle(), unitSize, numUnits, num)) {
        }
    public:
        ~Delay<Buffer>() {
            vxReleaseDelay(&m_handle);
        }
        Buffer *operator[](vx_uint32 index) {
            vx_buffer buf = vxGetBufferFromDelay(handle(), index);
            if (buf)
                return Buffer::wrap(buf);
            else
                return NULL;
        }
        vx_status associate(Node *node, vx_uint32 index, vx_uint32 pi, vx_enum pd)
        {
            return vxAssociateDelayWithNode(handle(), index, node->handle(), pi, pd);
        }
        vx_status dissociate(Node *node, vx_uint32 index, vx_uint32 pi)
        {
            return vxDissociateDelayFromNode(handle(), index, node->handle(), pi);
        }
    };

    template <> class Delay<Image> : public Reference {
        friend class Context;
    protected:
        Delay<Image>(vx_uint32 width, vx_uint32 height, vx_fourcc format, vx_size num)
            : Reference(vxCreateImageDelay(Context::getContext().handle(), width, height, format, num)) {
            ; // do nothing
        }
    public:
        ~Delay<Image>() {
            vxReleaseDelay(&m_handle);
        }
        Image *operator[](vx_uint32 index) {
            vx_image img = vxGetImageFromDelay(handle(), index);
            if (img)
                return Image::wrap(img);
            else
                return NULL;
        }
        vx_status associate(Node *node, vx_uint32 index, vx_uint32 pi, vx_enum pd)
        {
            return vxAssociateDelayWithNode(handle(), index, node->handle(), pi, pd);
        }
        vx_status dissociate(Node *node, vx_uint32 index, vx_uint32 pi)
        {
            return vxDissociateDelayFromNode(handle(), index, node->handle(), pi);
        }
    };
};

#endif

