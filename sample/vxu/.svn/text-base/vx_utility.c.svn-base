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
 * \brief The sample implementation of the immediate mode calls.
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

vx_status vxuColorConvert(vx_image src, vx_image dst)
{
    vx_context context = vxGetContext(src);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxColorConvertNode(graph, src, dst);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuChannelExtract(vx_image src, vx_enum channel, vx_image dst)
{
    vx_context context = vxGetContext(src);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxChannelExtractNode(graph, src, channel, dst);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuChannelCombine(vx_image plane0,
                            vx_image plane1,
                            vx_image plane2,
                            vx_image plane3,
                            vx_image output)
{
    vx_context context = vxGetContext(plane0);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxChannelCombineNode(graph, plane0, plane1, plane2, plane3, output);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuSobel3x3(vx_image src, vx_image output_x, vx_image output_y)
{
    vx_context context = vxGetContext(src);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxSobel3x3Node(graph, src, output_x, output_y);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuMagnitude(vx_image grad_x, vx_image grad_y, vx_image dst)
{
    vx_context context = vxGetContext(grad_x);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxMagnitudeNode(graph, grad_x, grad_y, dst);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuPhase(vx_image grad_x, vx_image grad_y, vx_image dst)
{
    vx_context context = vxGetContext(grad_x);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxPhaseNode(graph, grad_x, grad_y, dst);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuScaleImage(vx_image src, vx_image dst, vx_enum type)
{
    vx_context context = vxGetContext(src);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxScaleImageNode(graph, src, dst, type);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuTableLookup(vx_image input, vx_lut lut, vx_image output)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxTableLookupNode(graph, input, lut, output);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuHistogram(vx_image input, vx_distribution distribution)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxHistogramNode(graph, input, distribution);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuEqualizeHist(vx_image input, vx_image output)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxEqualizeHistNode(graph, input, output);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuAbsDiff(vx_image in1, vx_image in2, vx_image out)
{
    vx_context context = vxGetContext(in1);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxAbsDiffNode(graph, in1, in2, out);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuMeanStdDev(vx_image input, vx_float32 *mean, vx_float32 *stddev)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_scalar s_mean = vxCreateScalar(context, VX_TYPE_FLOAT32, NULL);
        vx_scalar s_stddev = vxCreateScalar(context, VX_TYPE_FLOAT32, NULL);
        vx_node node = vxAbsDiffNode(graph, input, s_mean, s_stddev);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
                vxAccessScalarValue(s_mean, mean);
                vxAccessScalarValue(s_stddev, stddev);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuThreshold(vx_image input, vx_threshold thresh, vx_image output)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxThresholdNode(graph, input, thresh, output);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuIntegralImage(vx_image input, vx_image output)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxIntegralImageNode(graph, input, output);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuErode3x3(vx_image input, vx_image output)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxErode3x3Node(graph, input, output);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuDilate3x3(vx_image input, vx_image output)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxDilate3x3Node(graph, input, output);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuMedian3x3(vx_image input, vx_image output)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxMedian3x3Node(graph, input, output);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuBox3x3(vx_image input, vx_image output)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxBox3x3Node(graph, input, output);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuGaussian3x3(vx_image input, vx_image output)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxGaussian3x3Node(graph, input, output);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuConvolve(vx_image input, vx_convolution conv, vx_image output)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxConvolveNode(graph, input, conv, output);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuPyramid(vx_image input, vx_pyramid gaussian, vx_pyramid laplacian)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxPyramidNode(graph, input, gaussian, laplacian);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuAccumulateImage(vx_image input, vx_image accum)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxAccumulateImageNode(graph, input, accum);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuAccumulateWeightedImage(vx_image input, vx_float32 alpha, vx_image accum)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_scalar scalar = vxCreateScalar(context, VX_TYPE_FLOAT32, &alpha);
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxAccumulateWeightedImageNode(graph, input, scalar, accum);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuAccumulateSquareImage(vx_image input, vx_image accum)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxAccumulateSquareImageNode(graph, input, accum);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuMinMaxLoc(vx_image input,
                        vx_scalar minVal, vx_scalar maxVal,
                        vx_coordinates minLoc, vx_coordinates maxLoc)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxMinMaxLocNode(graph, input, minVal, maxVal, minLoc, maxLoc);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuConvertDepth(vx_image input, vx_image output, vx_enum policy, vx_int32 shift)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    vx_scalar sshift = vxCreateScalar(context, VX_TYPE_INT32, &shift);
    if (graph)
    {
        vx_node node = vxConvertDepthNode(graph, input, output, policy, sshift);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxReleaseScalar(&sshift);
    vxClearLog(context);
    return status;
}

vx_status vxuCannyEdgeDetector(vx_image input, vx_threshold hyst,
                               vx_int32 gradient_size, vx_enum norm_type,
                               vx_image output)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxCannyEdgeDetectorNode(graph, input, hyst, gradient_size, norm_type, output);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuHalfScaleGaussian3x3(vx_image input, vx_image output)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_image virt = vxCreateVirtualImage(context);
        vx_node nodes[] = {
                vxGaussian3x3Node(graph, input, virt),
                vxScaleImageNode(graph, virt, output, VX_INTERPOLATION_TYPE_NEAREST_NEIGHBOR),
        };
        if (nodes[0] && nodes[1])
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&nodes[0]);
            vxReleaseNode(&nodes[1]);
        }
        vxReleaseImage(&virt);
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuAnd(vx_image in1, vx_image in2, vx_image out)
{
    vx_context context = vxGetContext(in1);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxAndNode(graph, in1, in2, out);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuOr(vx_image in1, vx_image in2, vx_image out)
{
    vx_context context = vxGetContext(in1);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxOrNode(graph, in1, in2, out);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuXor(vx_image in1, vx_image in2, vx_image out)
{
    vx_context context = vxGetContext(in1);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxXorNode(graph, in1, in2, out);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuNot(vx_image input, vx_image out)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxNotNode(graph, input, out);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuMultiply(vx_image in1, vx_image in2, vx_float32 scale, vx_enum policy, vx_image out)
{
    vx_context context = vxGetContext(in1);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    vx_scalar sscale = vxCreateScalar(context, VX_TYPE_FLOAT32, &scale);
    if (graph)
    {
        vx_node node = vxMultiplyNode(graph, in1, in2, sscale, policy, out);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxReleaseScalar(&sscale);
    vxClearLog(context);
    return status;
}

vx_status vxuAdd(vx_image in1, vx_image in2, vx_enum policy, vx_image out)
{
    vx_context context = vxGetContext(in1);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxAddNode(graph, in1, in2, policy, out);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuSubtract(vx_image in1, vx_image in2, vx_enum policy, vx_image out)
{
    vx_context context = vxGetContext(in1);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxSubtractNode(graph, in1, in2, policy, out);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuWarpAffine(vx_image input, vx_matrix matrix, vx_enum type, vx_image output)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxWarpAffineNode(graph, input, matrix, type, output);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuWarpPerspective(vx_image input, vx_matrix matrix, vx_enum type, vx_image output)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxWarpPerspectiveNode(graph, input, matrix, type, output);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuHarrisCorners(vx_image input,
        vx_scalar strength_thresh,
        vx_scalar min_distance,
        vx_scalar sensitivity,
        vx_int32 gradient_size,
        vx_int32 block_size,
        vx_list corners)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxHarrisCornersNode(graph, input, strength_thresh, min_distance, sensitivity, gradient_size, block_size, corners);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuFastCorners(vx_image input, vx_scalar sens, vx_bool nonmax, vx_list points)
{
    vx_context context = vxGetContext(input);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxFastCornersNode(graph, input, sens, nonmax, points);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}

vx_status vxuOpticalFlowPyrLK(vx_pyramid old_images,
                              vx_pyramid new_images,
                              vx_list old_points,
                              vx_list new_points_estimates,
                              vx_list new_points,
                              vx_enum termination,
                              vx_scalar epsilon,
                              vx_scalar num_iterations,
                              vx_scalar use_initial_estimate,
                              vx_size window_dimension)
{
    vx_context context = vxGetContext(old_images);
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxOpticalFlowPyrLKNode(graph, old_images, new_images, old_points,new_points_estimates, new_points,
        		termination,epsilon,num_iterations,use_initial_estimate,window_dimension);
        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    vxClearLog(context);
    return status;
}
