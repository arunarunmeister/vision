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

using namespace OpenVX;

Node::Node(vx_node node)
{
    m_handle = node;
}

Node::Node(Graph *g, Kernel *k) : Reference(vxCreateNode(g->handle(), k->handle())) {}
Node::Node(Graph *g, Kernel &k) : Reference(vxCreateNode(g->handle(), k.handle())) {}
Node::Node(Graph &g, Kernel *k) : Reference(vxCreateNode(g.handle(), k->handle())) {}
Node::Node(Graph &g, Kernel &k) : Reference(vxCreateNode(g.handle(), k.handle())) {}
Node::Node(Graph *g, vx_char name[VX_MAX_KERNEL_NAME]) : Reference(0) {
    Kernel k(name);
    m_handle = vxCreateNode(g->handle(), k.handle());
}

Node::Node(Graph *g, vx_enum kenum) : Reference(0) {
    Kernel k(kenum);
    m_handle = vxCreateNode(g->handle(), k.handle());
}

Node::Node(Graph &g, vx_char name[VX_MAX_KERNEL_NAME]) : Reference(0) {
    Kernel k(name);
    m_handle = vxCreateNode(g.handle(), k.handle());
}

Node::Node(Graph &g, vx_enum kenum) : Reference(0) {
    Kernel k(kenum);
    m_handle = vxCreateNode(g.handle(), k.handle());
}

Node::~Node()
{
    vxReleaseNode(&m_handle);
}

void Node::GetPerf(vx_perf_t *perf)
{
    vxQueryNode(handle(), VX_NODE_ATTRIBUTE_PERFORMANCE, perf, sizeof(*perf));
}

Parameter *Node::GetParameter(vx_uint32 index)
{
    return new Parameter(this, index);
}

vx_status Node::SetParameter(vx_uint32 index, vx_enum direction, Reference *object)
{
    return vxSetParameterByIndex(handle(), index, direction, object->handle());
}

vx_status Node::SetParameter(vx_uint32 index, vx_enum direction, Reference &object)
{
    return vxSetParameterByIndex(handle(), index, direction, object.handle());
}
