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
 * \brief The Canny Edge Detector Kernel Implementation.
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_ext_extras.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>
#include <math.h>

/*! \note Look at \ref vxCannyEdgeDetectorNode to see how this pyramid construction works */

static vx_status vxCannyEdgeKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 5)
    {
        vx_graph graph = vxGetChildGraphOfNode(node);
        status = vxProcessGraph(graph);
    }
    return status;
}

static vx_status vxCannyEdgeInitializer(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (num == 5)
    {
        vx_image input = (vx_image)parameters[0];
        vx_threshold hyst = (vx_threshold)parameters[1];
        vx_scalar gradient_size = (vx_scalar)parameters[2];
        vx_scalar snorm = (vx_scalar)parameters[3];
        vx_image output = (vx_image)parameters[4];

        vx_context context = vxGetContext(node);
        vx_graph graph = 0;
        status = vxLoadKernels(context, "openvx-extras");
        if (status != VX_SUCCESS)
        {
            return status;
        }
        graph = vxCreateGraph(context);
        if (graph)
        {
            vx_uint32 i;
            vx_convolution conv[] = {
                vxCreateConvolution(context, 3, 3),
                vxCreateConvolution(context, 3, 3),
            };
            vx_image virts[] = {
                vxCreateVirtualImage(context),  /* 0: gaussian */
                vxCreateVirtualImage(context),  /* 1: Gx */
                vxCreateVirtualImage(context),  /* 2: Gy */
                vxCreateVirtualImageWithFormat(context, FOURCC_U8),  /* 3: Mag */
                vxCreateVirtualImage(context),  /* 4: Phase */
                vxCreateVirtualImageWithFormat(context, FOURCC_U8),  /* 5: Nonmax */
            };
            /*! \bug Fix canny graph to be the actual edge tracing features! */
            vx_node nodes[] = {
                vxGaussian3x3Node(graph, input, virts[0]),
                vxConvolveNode(graph, virts[0], conv[0], virts[1]),
                vxConvolveNode(graph, virts[0], conv[1], virts[2]),
                vxMagnitudeNode(graph, virts[1], virts[2], virts[3]),
                vxPhaseNode(graph, virts[1], virts[2], virts[4]),
                vxNonMaxSuppressionNode(graph, virts[3], virts[4], virts[5]),
                vxThresholdNode(graph, virts[5], hyst, output),
            };
            vx_int16 gx[3][3] = {
                { 0, 0, 0},
                {-1, 0, 1},
                { 0, 0, 0},
            };
            vx_int16 gy[3][3] = {
                { 0,-1, 0},
                { 0, 0, 0},
                { 0, 1, 0},
            };
            status |= vxAccessConvolutionCoefficients(conv[0], NULL);
            status |= vxAccessConvolutionCoefficients(conv[1], NULL);
            status |= vxCommitConvolutionCoefficients(conv[0], (vx_int16 *)gx);
            status |= vxCommitConvolutionCoefficients(conv[1], (vx_int16 *)gy);
            status |= vxAddParameterToGraphByIndex(graph, nodes[0], 0); /* input image */
            status |= vxAddParameterToGraphByIndex(graph, nodes[6], 1); /* threshold */
            status |= vxAddParameterToGraphByIndex(graph, 0, 0);     /*! \bug add gradient size */
            status |= vxAddParameterToGraphByIndex(graph, 0, 0);     /*! \bug add norm */
            status |= vxAddParameterToGraphByIndex(graph, nodes[6], 2); /* output image */
            status |= vxVerifyGraph(graph);
            /* release our references, the graph will hold it's own */
            for (i = 0; i < dimof(nodes); i++) {
                vxReleaseNode(&nodes[i]);
            }
            for (i = 0; i < dimof(virts); i++) {
                vxReleaseImage(&virts[i]);
            }
            for (i = 0; i < dimof(conv); i++) {
                vxReleaseConvolution(&conv[i]);
            }
            status = vxSetChildGraphOfNode(node, graph);
            vxReleaseGraph(&graph);
        }
    }
    return status;
}

static vx_status vxCannyEdgeDeinitializer(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (num == 3)
    {
        vx_graph graph = vxGetChildGraphOfNode(node);
        vxReleaseGraph(&graph);
        graph = 0;
        vxSetChildGraphOfNode(node, graph);
    }
    return status;
}

static vx_status vxCannyEdgeInputValidator(vx_node node, vx_uint32 index)
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
    else if (index == 1)
    {
        vx_threshold hyst = 0;
        vx_parameter param = vxGetParameterByIndex(node, index);

        vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &hyst, sizeof(hyst));
        if (hyst)
        {
            vx_enum type = 0;
            vxQueryThreshold(hyst, VX_THRESHOLD_ATTRIBUTE_TYPE, &type, sizeof(type));
            if (type == VX_THRESHOLD_TYPE_RANGE)
            {
                status = VX_SUCCESS;
            }
        }
        vxReleaseParameter(&param);
    }
    else if (index == 2)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_scalar value = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &value, sizeof(value));
            if (value)
            {
                vx_enum stype = 0;
                vxQueryScalar(value, VX_SCALAR_ATTRIBUTE_TYPE, &stype, sizeof(stype));
                if (stype == VX_TYPE_INT32)
                {
                    vx_int32 gs = 0;
                    vxAccessScalarValue(value, &gs);
                    if ((gs == 3) || (gs == 5) || (gs == 7))
                    {
                        status = VX_SUCCESS;
                    }
                    else
                    {
                        status = VX_ERROR_INVALID_VALUE;
                    }
                }
                else
                {
                    status = VX_ERROR_INVALID_TYPE;
                }
            }
            vxReleaseParameter(&param);
        }
    }
    else if (index == 3)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if (param)
        {
            vx_scalar value = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &value, sizeof(value));
            if (value)
            {
                vx_enum norm = 0;
                vxAccessScalarValue(value, &norm);
                if ((norm == VX_NORM_L1) ||
                    (norm == VX_NORM_L2))
                {
                    status = VX_SUCCESS;
                }
            }
            vxReleaseParameter(&param);
        }
    }
    return status;
}

static vx_status vxCannyEdgeOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 4)
    {
        vx_parameter src_param = vxGetParameterByIndex(node, 0);
        if (src_param)
        {
            vx_image src = 0;
            vxQueryParameter(src_param, VX_PARAMETER_ATTRIBUTE_REF, &src, sizeof(src));
            if (src)
            {
                vx_uint32 width = 0, height = 0;
                vx_fourcc format = FOURCC_VIRT;

                vxQueryImage(src, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(height));
                vxQueryImage(src, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
                vxQueryImage(src, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
                /* fill in the meta data with the attributes so that the checker will pass */
                ptr->type = VX_TYPE_IMAGE;
                ptr->dim.image.format = format;
                ptr->dim.image.width = width;
                ptr->dim.image.height = height;
                status = VX_SUCCESS;
            }
            vxReleaseParameter(&src_param);
        }
    }
    return status;
}

static vx_param_description_t canny_kernel_params[] = {
    {VX_INPUT,  VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT,  VX_TYPE_THRESHOLD, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT,  VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT,  VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t canny_kernel = {
    VX_KERNEL_CANNY_EDGE_DETECTOR,
    "org.khronos.openvx.canny_edge_detector",
    vxCannyEdgeKernel,
    canny_kernel_params, dimof(canny_kernel_params),
    vxCannyEdgeInputValidator,
    vxCannyEdgeOutputValidator,
    vxCannyEdgeInitializer,
    vxCannyEdgeDeinitializer,
};


