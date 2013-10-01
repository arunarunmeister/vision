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

#ifndef _VX_IMAGE_HPP_
#define _VX_IMAGE_HPP_

namespace OpenVX {
    class Image : public Reference {
        friend class Context;
        friend class Delay<Image>;
    protected:
        static Image *wrap(vx_image ref) {
            Reference *r = new Reference(ref);
            return dynamic_cast<Image *>(r);
        }
    public:
        Image();
        Image(vx_uint32 width, vx_uint32 height, vx_fourcc color);
        Image(vx_fourcc color);
        ~Image();
        vx_uint32 width();
        vx_uint32 height();
        vx_fourcc format();
        vx_uint32 planes();
        vx_enum space();
        vx_enum range();

        vx_status GetImagePatch(Rectangle *rect, vx_uint32 p, vx_imagepatch_addressing_t *addr, void **ptr);
        vx_status SetImagePatch(Rectangle *rect, vx_uint32 p, vx_imagepatch_addressing_t *addr, void *ptr);
        vx_size ComputePlaneSize(Rectangle *rect, vx_uint32 p);
    };
}
#endif

