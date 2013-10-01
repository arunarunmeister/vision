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

Graph::Graph() : Reference(vxCreateGraph(Context::getContext().handle()))
{
    ; // do nothing
}

Graph::~Graph()
{
    vxReleaseGraph(&m_handle);
}

Node *Graph::CreateNode(Kernel *k)
{
    return new Node(this, k);
}

Node *Graph::CreateNode(Kernel &k)
{
    return new Node(this, k);
}

vx_uint32 Graph::numNodes()
{
    vx_uint32 nodes;
    vxQueryGraph(handle(), VX_GRAPH_ATTRIBUTE_NUMNODES, &nodes, sizeof(nodes));
    return nodes;
}

vx_status Graph::Verify()
{
    return vxVerifyGraph(handle());
}

vx_status Graph::Process()
{
    return vxProcessGraph(handle());
}

vx_status Graph::Schedule()
{
    return vxScheduleGraph(handle());
}

vx_status Graph::Wait()
{
    return vxWaitGraph(handle());
}


