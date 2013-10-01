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
 * \brief The Graph Mode Interface for all Base Kernels.
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <vx_internal.h>

vx_node vxColorConvertNode(vx_graph graph, vx_image input, vx_image output)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph, VX_KERNEL_COLOR_CONVERT, params, dimof(params));
}

vx_node vxChannelExtractNode(vx_graph graph,
                             vx_image input,
                             vx_enum channelNum,
                             vx_image output)
{

    vx_scalar scalar = vxCreateScalar(vxGetContext(graph), VX_TYPE_ENUM, &channelNum);
    vx_parameter_item_t params[] = {
        {VX_INPUT,  input},
        {VX_INPUT,  scalar},
        {VX_OUTPUT, output},
    };
    vx_node node = vxCreateNodeByStructure(graph,
                                           VX_KERNEL_CHANNEL_EXTRACT,
                                           params,
                                           dimof(params));
    vxReleaseScalar(&scalar); // node hold reference
    return node;
}

vx_node vxChannelCombineNode(vx_graph graph,
                             vx_image plane0,
                             vx_image plane1,
                             vx_image plane2,
                             vx_image plane3,
                             vx_image output)
{
    vx_parameter_item_t params[] = {
       {VX_INPUT, plane0},
       {VX_INPUT, plane1},
       {VX_INPUT, plane2},
       {VX_INPUT, plane3},
       {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_CHANNEL_COMBINE,
                                   params,
                                   dimof(params));
}

vx_node vxSobel3x3Node(vx_graph graph, vx_image input, vx_image output_x, vx_image output_y)
{
    vx_parameter_item_t params[] = {
       {VX_INPUT, input},
       {VX_OUTPUT, output_x},
       {VX_OUTPUT, output_y},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_SOBEL_3x3,
                                   params,
                                   dimof(params));
}

vx_node vxMagnitudeNode(vx_graph graph, vx_image grad_x, vx_image grad_y, vx_image mag)
{
    vx_parameter_item_t params[] = {
       {VX_INPUT, grad_x},
       {VX_INPUT, grad_y},
       {VX_OUTPUT, mag},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_MAGNITUDE,
                                   params,
                                   dimof(params));
}

vx_node vxPhaseNode(vx_graph graph, vx_image grad_x, vx_image grad_y, vx_image orientation)
{
    vx_parameter_item_t params[] = {
       {VX_INPUT, grad_x},
       {VX_INPUT, grad_y},
       {VX_OUTPUT, orientation},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_PHASE,
                                   params,
                                   dimof(params));
}

vx_node vxScaleImageNode(vx_graph graph, vx_image src, vx_image dst, vx_enum type)
{
    vx_context context = vxGetContext(graph);
    vx_scalar stype = vxCreateScalar(context, VX_TYPE_ENUM, &type);
    vx_parameter_item_t params[] = {
        {VX_INPUT, src},
        {VX_OUTPUT, dst},
        {VX_INPUT, stype},
    };
    vx_node node = vxCreateNodeByStructure(graph,
                                           VX_KERNEL_SCALE_IMAGE,
                                           params,
                                           dimof(params));
    vxReleaseScalar(&stype);
    return node;
}

vx_node vxTableLookupNode(vx_graph graph, vx_image input, vx_lut lut, vx_image output)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_INPUT, lut},
        {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_TABLE_LOOKUP,
                                   params,
                                   dimof(params));
}

vx_node vxHistogramNode(vx_graph graph, vx_image input, vx_distribution distribution)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_OUTPUT, distribution},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_HISTOGRAM,
                                   params,
                                   dimof(params));
}

vx_node vxEqualizeHistNode(vx_graph graph, vx_image input, vx_image output)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_EQUALIZE_HISTOGRAM,
                                   params,
                                   dimof(params));
}

vx_node vxAbsDiffNode(vx_graph graph, vx_image in1, vx_image in2, vx_image out)
{
    vx_parameter_item_t params[] = {
       {VX_INPUT, in1},
       {VX_INPUT, in2},
       {VX_OUTPUT, out},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_ABSDIFF,
                                   params,
                                   dimof(params));
}

vx_node vxMeanStdDevNode(vx_graph graph, vx_image input, vx_scalar mean, vx_scalar stddev)
{
    vx_parameter_item_t params[] = {
       {VX_INPUT, input},
       {VX_OUTPUT, mean},
       {VX_OUTPUT, stddev},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_MEAN_STDDEV,
                                   params,
                                   dimof(params));
}

vx_node vxThresholdNode(vx_graph graph, vx_image input, vx_threshold thesh, vx_image output)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_INPUT, thesh},
        {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_THRESHOLD,
                                   params,
                                   dimof(params));
}

vx_node vxIntegralImageNode(vx_graph graph, vx_image input, vx_image output)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_INTEGRAL_IMAGE,
                                   params,
                                   dimof(params));
}

vx_node vxErode3x3Node(vx_graph graph, vx_image input, vx_image output)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_ERODE_3x3,
                                   params,
                                   dimof(params));
}

vx_node vxDilate3x3Node(vx_graph graph, vx_image input, vx_image output)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_DILATE_3x3,
                                   params,
                                   dimof(params));
}

vx_node vxMedian3x3Node(vx_graph graph, vx_image input, vx_image output)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_MEDIAN_3x3,
                                   params,
                                   dimof(params));
}

vx_node vxBox3x3Node(vx_graph graph, vx_image input, vx_image output)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_BOX_3x3,
                                   params,
                                   dimof(params));
}

vx_node vxGaussian3x3Node(vx_graph graph, vx_image input, vx_image output)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_GAUSSIAN_3x3,
                                   params,
                                   dimof(params));
}

vx_node vxConvolveNode(vx_graph graph, vx_image input, vx_convolution conv, vx_image output)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_INPUT, conv},
        {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_CUSTOM_CONVOLUTION,
                                   params,
                                   dimof(params));
}

vx_node vxPyramidNode(vx_graph graph, vx_image input, vx_pyramid gaussian, vx_pyramid laplacian)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_OUTPUT, gaussian},
        {VX_OUTPUT, laplacian},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_PYRAMID,
                                   params,
                                   dimof(params));
}

vx_node vxAccumulateImageNode(vx_graph graph, vx_image input, vx_image accum)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_BIDIRECTIONAL, accum},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_ACCUMULATE,
                                   params,
                                   dimof(params));
}

vx_node vxAccumulateWeightedImageNode(vx_graph graph, vx_image input, vx_scalar alpha, vx_image accum)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_INPUT, alpha},
        {VX_BIDIRECTIONAL, accum},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_ACCUMULATE_WEIGHTED,
                                   params,
                                   dimof(params));
}

vx_node vxAccumulateSquareImageNode(vx_graph graph, vx_image input, vx_image accum)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_BIDIRECTIONAL, accum},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_ACCUMULATE_SQUARE,
                                   params,
                                   dimof(params));
}

vx_node vxMinMaxLocNode(vx_graph graph,
                        vx_image input,
                        vx_scalar minVal, vx_scalar maxVal,
                        vx_coordinates minLoc, vx_coordinates maxLoc)
{
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_OUTPUT, minVal},
        {VX_OUTPUT, maxVal},
        {VX_OUTPUT, minLoc},
        {VX_OUTPUT, maxLoc},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_MINMAXLOC,
                                   params,
                                   dimof(params));
}

vx_node vxConvertDepthNode(vx_graph graph, vx_image input, vx_image output, vx_enum policy, vx_scalar shift)
{
    vx_scalar pol = vxCreateScalar(vxGetContext(graph), VX_TYPE_ENUM, &policy);
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_OUTPUT, output},
        {VX_INPUT, pol},
        {VX_INPUT, shift},
    };
    vx_node node = vxCreateNodeByStructure(graph,
                                   VX_KERNEL_CONVERTDEPTH,
                                   params,
                                   dimof(params));
    vxReleaseScalar(&pol);
    return node;
}

vx_node vxCannyEdgeDetectorNode(vx_graph graph, vx_image input, vx_threshold hyst,
                                vx_int32 gradient_size, vx_enum norm_type,
                                vx_image output)
{
    vx_scalar gs = vxCreateScalar(vxGetContext(graph), VX_TYPE_INT32, &gradient_size);
    vx_scalar nt = vxCreateScalar(vxGetContext(graph), VX_TYPE_ENUM, &norm_type);
    vx_parameter_item_t params[] = {
        {VX_INPUT, input},
        {VX_INPUT, hyst},
        {VX_INPUT, gs},
        {VX_INPUT, nt},
        {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_CANNY_EDGE_DETECTOR,
                                   params,
                                   dimof(params));
}

vx_node vxAndNode(vx_graph graph, vx_image in1, vx_image in2, vx_image out)
{
    vx_parameter_item_t params[] = {
       {VX_INPUT, in1},
       {VX_INPUT, in2},
       {VX_OUTPUT, out},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_AND,
                                   params,
                                   dimof(params));
}

vx_node vxOrNode(vx_graph graph, vx_image in1, vx_image in2, vx_image out)
{
    vx_parameter_item_t params[] = {
       {VX_INPUT, in1},
       {VX_INPUT, in2},
       {VX_OUTPUT, out},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_OR,
                                   params,
                                   dimof(params));
}

vx_node vxXorNode(vx_graph graph, vx_image in1, vx_image in2, vx_image out)
{
    vx_parameter_item_t params[] = {
       {VX_INPUT, in1},
       {VX_INPUT, in2},
       {VX_OUTPUT, out},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_XOR,
                                   params,
                                   dimof(params));
}

vx_node vxNotNode(vx_graph graph, vx_image input, vx_image output)
{
    vx_parameter_item_t params[] = {
       {VX_INPUT, input},
       {VX_OUTPUT, output},
    };
    return vxCreateNodeByStructure(graph,
                                   VX_KERNEL_NOT,
                                   params,
                                   dimof(params));
}

vx_node vxMultiplyNode(vx_graph graph, vx_image in1, vx_image in2, vx_scalar scale, vx_enum policy, vx_image out)
{
    vx_context context = vxGetContext(graph);
    vx_scalar spolicy = vxCreateScalar(context, VX_TYPE_ENUM, &policy);
    vx_parameter_item_t params[] = {
       {VX_INPUT, in1},
       {VX_INPUT, in2},
       {VX_INPUT, scale},
       {VX_INPUT, spolicy},
       {VX_OUTPUT, out},
    };
    vx_node node = vxCreateNodeByStructure(graph,
                                           VX_KERNEL_MULTIPLY,
                                           params,
                                           dimof(params));
    vxReleaseScalar(&spolicy);
    return node;
}

vx_node vxAddNode(vx_graph graph, vx_image in1, vx_image in2, vx_enum policy, vx_image out)
{
    vx_context context = vxGetContext(graph);
    vx_scalar spolicy = vxCreateScalar(context, VX_TYPE_ENUM, &policy);
    vx_parameter_item_t params[] = {
       {VX_INPUT, in1},
       {VX_INPUT, in2},
       {VX_INPUT, spolicy},
       {VX_OUTPUT, out},
    };
    vx_node node = vxCreateNodeByStructure(graph,
                                           VX_KERNEL_ADD,
                                           params,
                                           dimof(params));
    vxReleaseScalar(&spolicy);
    return node;
}

vx_node vxSubtractNode(vx_graph graph, vx_image in1, vx_image in2, vx_enum policy, vx_image out)
{
    vx_context context = vxGetContext(graph);
    vx_scalar spolicy = vxCreateScalar(context, VX_TYPE_ENUM, &policy);
    vx_parameter_item_t params[] = {
       {VX_INPUT, in1},
       {VX_INPUT, in2},
       {VX_INPUT, spolicy},
       {VX_OUTPUT, out},
    };
    vx_node node = vxCreateNodeByStructure(graph,
                                           VX_KERNEL_SUBTRACT,
                                           params,
                                           dimof(params));
    vxReleaseScalar(&spolicy);
    return node;
}

vx_node vxWarpAffineNode(vx_graph graph, vx_image input, vx_matrix matrix, vx_enum type, vx_image output)
{
    vx_context context = vxGetContext(graph);
    vx_scalar stype = vxCreateScalar(context, VX_TYPE_ENUM, &type);
    vx_parameter_item_t params[] = {
            {VX_INPUT, input},
            {VX_INPUT, matrix},
            {VX_INPUT, stype},
            {VX_OUTPUT, output},
    };
    vx_node node = vxCreateNodeByStructure(graph,
                                           VX_KERNEL_WARP_AFFINE,
                                           params,
                                           dimof(params));
    vxReleaseScalar(&stype);
    return node;
}

vx_node vxWarpPerspectiveNode(vx_graph graph, vx_image input, vx_matrix matrix, vx_enum type, vx_image output)
{
    vx_context context = vxGetContext(graph);
    vx_scalar stype = vxCreateScalar(context, VX_TYPE_ENUM, &type);
    vx_parameter_item_t params[] = {
            {VX_INPUT, input},
            {VX_INPUT, matrix},
            {VX_INPUT, stype},
            {VX_OUTPUT, output},
    };
    vx_node node = vxCreateNodeByStructure(graph,
                                           VX_KERNEL_WARP_PERSPECTIVE,
                                           params,
                                           dimof(params));
    vxReleaseScalar(&stype);
    return node;
}

vx_node vxHarrisCornersNode(vx_graph graph,
                            vx_image input,
                            vx_scalar strength_thresh,
                            vx_scalar min_distance,
                            vx_scalar sensitivity,
                            vx_int32 gradient_size,
                            vx_int32 block_size,
                            vx_list corners)
{
    vx_scalar win = vxCreateScalar(vxGetContext(graph), VX_TYPE_INT32, &gradient_size);
    vx_scalar blk = vxCreateScalar(vxGetContext(graph), VX_TYPE_INT32, &block_size);
    vx_parameter_item_t params[] = {
            {VX_INPUT, input},
            {VX_INPUT, strength_thresh},
            {VX_INPUT, min_distance},
            {VX_INPUT, sensitivity},
            {VX_INPUT, win},
            {VX_INPUT, blk},
            {VX_OUTPUT, corners},
    };
    vx_node node = vxCreateNodeByStructure(graph,
                                           VX_KERNEL_HARRIS_CORNERS,
                                           params,
                                           dimof(params));
    vxReleaseScalar(&win);
    vxReleaseScalar(&blk);
    return node;
}

vx_node vxFastCornersNode(vx_graph graph, vx_image input, vx_scalar strength_thresh, vx_bool nonmax_supression, vx_list list)
{
    vx_scalar nonmax = vxCreateScalar(vxGetContext(graph),VX_TYPE_BOOL, &nonmax_supression);
    vx_parameter_item_t params[] = {
            {VX_INPUT, input},
            {VX_INPUT, strength_thresh},
            {VX_INPUT, nonmax},
            {VX_OUTPUT, list},
    };
    vx_node node = vxCreateNodeByStructure(graph,
                                           VX_KERNEL_FAST_CORNERS,
                                           params,
                                           dimof(params));
    vxReleaseScalar(&nonmax);
    return node;
}

vx_node vxOpticalFlowPyrLKNode(vx_graph graph,
		                       vx_pyramid old_images,
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
    vx_parameter_item_t params[] = {
            {VX_INPUT, old_images},
            {VX_INPUT, new_images},
            {VX_INPUT, old_points},
            {VX_INPUT, new_points_estimates},
            {VX_OUTPUT, new_points},
            {VX_INPUT, termination},
            {VX_INPUT, epsilon},
            {VX_INPUT, num_iterations},
            {VX_INPUT, use_initial_estimate},
            {VX_INPUT, window_dimension},
    };
    return vxCreateNodeByStructure(graph,
                                    VX_KERNEL_OPTICAL_FLOW_PYR_LK,
                                    params,
                                    dimof(params));
}

