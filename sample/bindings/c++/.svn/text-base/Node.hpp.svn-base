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

#ifndef _VX_NODE_HPP_
#define _VX_NODE_HPP_

#include <VX/vx_ext_debug.h>

namespace OpenVX {
    class Node : public Reference {
        friend class Context;
        friend class Graph;
    protected:
        /* only childern should create nodes directly */
        Node(vx_node);
    public:
        Node(Graph *g, Kernel *k);
        Node(Graph *g, Kernel &k);
        Node(Graph &g, Kernel *k);
        Node(Graph &g, Kernel &k);
        Node(Graph *g, vx_char name[VX_MAX_KERNEL_NAME]);
        Node(Graph *g, vx_enum kenum);
        Node(Graph &g, vx_char name[VX_MAX_KERNEL_NAME]);
        Node(Graph &g, vx_enum kenum);
    public:
        virtual ~Node();
        void GetPerf(vx_perf_t *perf);
        Parameter *GetParameter(vx_uint32 index);
        vx_status SetParameter(vx_uint32 index, vx_enum direction, Reference *object);
        vx_status SetParameter(vx_uint32 index, vx_enum direction, Reference &object);
        vx_status SetParameter(Parameter *p, vx_uint32 index);
    };

    class Sobel3x3Node : public Node {
    public:
        Sobel3x3Node(Graph *g, Image *in, Image *gx, Image *gy)
          : Node(vxSobel3x3Node(g->handle(),
                                in->handle(),
                                gx?gx->handle():0,
                                gy?gy->handle():0)) {}
    };
    class MagnitudeNode : public Node {
    public:
            MagnitudeNode(Graph *g, Image *gx, Image *gy, Image *out)
              : Node(vxMagnitudeNode(g->handle(),
                                     gx->handle(),
                                     gy->handle(),
                                     out->handle())) {}
    };
    class FReadImageNode : public Node {
    public:
        FReadImageNode(Graph *g, char *filename, Image *out)
                  : Node(vxFReadImageNode(g->handle(),
                                          filename,
                                          out->handle())) {}
    };
    class FWriteImageNode : public Node {
    public:
        FWriteImageNode(Graph *g, Image *in, char *filename)
                  : Node(vxFWriteImageNode(g->handle(),
                                           in->handle(),
                                           filename)) {}
    };
};
#endif

