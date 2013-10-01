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

#ifndef _VX_LIST_HPP_
#define _VX_LIST_HPP_

namespace OpenVX {
    class List: public Reference {
        friend class Context;
    protected:
        //List(Context *c, vx_size initial);
        List(Context *c, vx_list list);
        ~List();
    public:
        vx_size length();
        Reference *GetItem(vx_enum ord);
        vx_status Empty();
        vx_status Erase(Reference *ref);
        vx_status Add(Reference *ref);
#if defined(OPENVX_KHR_LIST)
        vx_status Insert(Reference *ref, vx_ref_compare_f compare);
        vx_status Sort(vx_ref_compare_f compare);
#endif
    };
};
#endif
