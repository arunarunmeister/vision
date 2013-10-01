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

/*!
 * \file
 * \brief The Image Pyramid Kernel
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_ext_debug.h>
#include <VX/vx_ext_extras.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>
#include <math.h>

/*! \note Look at \ref vxPyramidNode to see how this pyramid construction works */

static vx_status vxPyramidKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 3)
    {
        vx_graph graph = 0;
        vx_size size = 0;
        status  = VX_SUCCESS;
        status |= vxQueryNode(node, VX_NODE_ATTRIBUTE_LOCAL_DATA_PTR, &graph, sizeof(graph));
        status |= vxQueryNode(node, VX_NODE_ATTRIBUTE_LOCAL_DATA_SIZE, &size, sizeof(size));
        if (graph && size == sizeof(graph))
        {
            status = vxProcessGraph(graph);
        }
    }
    return status;
}

static vx_status vxPyramidInitializer(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (num == 3)
    {
        vx_size lev, levels = 1;
        vx_image input = (vx_image)parameters[0];
        vx_pyramid gaussian = (vx_pyramid)parameters[1];
        vx_pyramid laplacian = (vx_pyramid)parameters[2];
        vx_context context = vxGetContext(node);
        vx_graph graph = vxCreateGraph(context);
        vx_enum interp = VX_INTERPOLATION_TYPE_BILINEAR; // VX_INTERPOLATION_TYPE_NEAREST_NEIGHBOR
        if (graph)
        {
            if (gaussian)
            {
                vx_node copy = vxCopyImageNode(graph, input, vxGetPyramidLevel(gaussian, 0));
                vxQueryPyramid(gaussian, VX_PYRAMID_ATTRIBUTE_LEVELS, &levels, sizeof(levels));
                for (lev = 1; lev < levels; lev++)
                {
                    vx_image tmp0 = (lev-1==0 ? input : vxGetPyramidLevel(gaussian, (vx_uint32)lev-1));
                    vx_image tmp1 = vxGetPyramidLevel(gaussian, (vx_uint32)lev);
                    vx_image virt = vxCreateVirtualImage(context);
                    vx_node  gtmp = vxGaussian3x3Node(graph, tmp0, virt);
                    vx_node  stmp = vxScaleImageNode(graph, virt, tmp1, interp);
                    /* decrements the references */
                    vxReleaseImage(&virt);
                    vxReleaseNode(&gtmp);
                    vxReleaseNode(&stmp);
                }
                vxReleaseNode(&copy);
            }
            if (laplacian)
            {
                /*! \todo Implement Laplacian pyramids correctly */
                vx_node copy = vxCopyImageNode(graph, input, vxGetPyramidLevel(laplacian, 0));
                vxQueryPyramid(laplacian, VX_PYRAMID_ATTRIBUTE_LEVELS, &levels, sizeof(levels));
                for (lev = 1; lev < levels; lev++)
                {
                    vx_image tmp0 = (lev-1==0 ? input : vxGetPyramidLevel(laplacian, (vx_uint32)lev-1));
                    vx_image tmp1 = vxGetPyramidLevel(laplacian, (vx_uint32)lev);
                    vx_image virt = vxCreateVirtualImage(context);
                    vx_node  ltmp = vxLaplacian3x3Node(graph, tmp0, virt);
                    vx_node  stmp = vxScaleImageNode(graph, virt, tmp1, interp);
                    /* decrements the references */
                    vxReleaseImage(&virt);
                    vxReleaseNode(&ltmp);
                    vxReleaseNode(&stmp);
                }
                vxReleaseNode(&copy);
            }
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                vx_size size = sizeof(vx_graph);

                /* remember the graph */
                status |= vxSetNodeAttribute(node, VX_NODE_ATTRIBUTE_LOCAL_DATA_SIZE, &size, sizeof(size));
                status |= vxSetNodeAttribute(node, VX_NODE_ATTRIBUTE_LOCAL_DATA_PTR, &graph, sizeof(graph));
            }
        }
    }
    return status;
}

static vx_status vxPyramidDeinitializer(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (num == 3)
    {
        vx_graph graph = 0;
        vx_size size = 0;
        status  = VX_SUCCESS;
        status |= vxQueryNode(node, VX_NODE_ATTRIBUTE_LOCAL_DATA_PTR, &graph, sizeof(graph));
        status |= vxQueryNode(node, VX_NODE_ATTRIBUTE_LOCAL_DATA_SIZE, &size, sizeof(size));
        if (graph && size == sizeof(graph))
        {
            vxReleaseGraph(&graph);
            graph = 0;
            size = 0;
            status |= vxSetNodeAttribute(node, VX_NODE_ATTRIBUTE_LOCAL_DATA_PTR, &graph, sizeof(graph));
            status |= vxSetNodeAttribute(node, VX_NODE_ATTRIBUTE_LOCAL_DATA_SIZE, &size, sizeof(size));
        }
    }
    return status;
}

static vx_status vxPyramidInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0)
    {
        vx_image input = 0;
        vx_parameter param = vxGetParameterByIndex(node, index);

        vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
        if (input)
        {
            vx_fourcc format = 0;
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
            if (format == FOURCC_U8)
            {
                status = VX_SUCCESS;
            }
        }
        vxReleaseParameter(&param);
    }
    return status;
}

static vx_status vxPyramidOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if ((index == 1) || (index == 2))
    {
        vx_image src = 0;
        vx_parameter src_param = vxGetParameterByIndex(node, 0);

        vxQueryParameter(src_param, VX_PARAMETER_ATTRIBUTE_REF, &src, sizeof(src));
        if (src)
        {
            vx_uint32 width = 0, height = 0;
            vx_fourcc format;

            vxQueryImage(src, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
            vxQueryImage(src, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
            vxQueryImage(src, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
            /* fill in the meta data with the attributes so that the checker will pass */
            ptr->type = VX_TYPE_PYRAMID;
            ptr->dim.pyramid.width = width;
            ptr->dim.pyramid.height = height;
            ptr->dim.pyramid.format = format;
            status = VX_SUCCESS;
        }
        vxReleaseParameter(&src_param);
    }
    return status;
}

static vx_param_description_t pyramid_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_PYRAMID,   VX_PARAMETER_STATE_OPTIONAL},
    {VX_OUTPUT, VX_TYPE_PYRAMID,   VX_PARAMETER_STATE_OPTIONAL},
};

vx_kernel_description_t pyramid_kernel = {
    VX_KERNEL_PYRAMID,
    "org.khronos.openvx.pyramid",
    vxPyramidKernel,
    pyramid_kernel_params, dimof(pyramid_kernel_params),
    vxPyramidInputValidator,
    vxPyramidOutputValidator,
    vxPyramidInitializer,
    vxPyramidDeinitializer,
};


