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

#ifndef _VX_CONTEXT_HPP_
#define _VX_CONTEXT_HPP_

#include <vx.hpp>

namespace OpenVX {
    /*! \brief A Singleton Context Object for OpenVX. */
    class Context : public Reference {
        friend class Reference;
        friend class Buffer;
        friend class Graph;
        friend class Image;
        friend class Kernel;
        friend class Node;
        friend class Parameter;
        friend class Rectangle;
        template <class T> friend class Scalar;
        friend class Target;
        template <class T> friend class Delay;
        friend class Convolution;
        template <class T> friend class Matrix;
        friend class List;
        friend class Coordinates;
        friend class Pyramid;
        template <class T> friend class LUT;
private:
        Context();
        Context(Context const& copy);
        Context& operator=(Context const &copy);
        vx_context Handle();
public:
        static Context& getContext()
        {
            static Context context;
            return context;
        }
        ~Context();
        static vx_uint32 numTargets();
        static vx_uint32 numModules();
        static vx_uint32 numKernels();
        static vx_status LoadKernels(const char *filename);
        static const char *implementation();
        static vx_uint16 vendorID();
        static vx_uint16 version();
        static const char *extensions();
    };
}
#endif
