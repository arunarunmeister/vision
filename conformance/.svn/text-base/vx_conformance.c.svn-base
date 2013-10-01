/*
 * Copyright (c) 2012-2013 The Khronos Group Inc.
 * All Rights Reserved.  This code is protected by copyright laws and contains
 * material proprietary to the Khronos Group, Inc.  This is UNPUBLISHED
 * PROPRIETARY SOURCE CODE that may not be disclosed in whole or in part to
 * third parties, and may not be reproduced, republished, distributed,
 * transmitted, displayed, broadcast or otherwise exploited in any manner
 * without the express prior written permission of Khronos Group.
 *
 * The receipt or possession of this code does not convey any rights to
 * reproduce, disclose, or distribute its contents, or to manufacture, use, or
 * sell anything that it may describe, in whole or in part other than under the
 * terms of the Khronos Adopters Agreement or Khronos Conformance Test Source
 * License Agreement as executed between Khronos and the recipient.
 *
 * For the avoidance of doubt, this code when provided: a) under the Khronos
 * Conformance Test Source License Agreement is for the sole purpose of creating
 * Conformance tests for delivery to Khronos and does not provide for formally
 * testing products or use of Khronos trademarks on conformant products; b)
 * under the Khronos Adopters Agreement is for the sole purpose of formally
 * administering tests to products pursuant to the Khronos Conformance
 * Process Document.
 *
 * Khronos, OpenKODE, OpenVG, OpenWF, glFX, OpenMAX and OpenSL ES are trademarks
 * of the Khronos Group Inc.  COLLADA is a trademark of Sony Computer
 * Entertainment Inc. used by permission by Khronos.  OpenGL and OpenML are
 * registered trademarks and the OpenGL ES logo is a trademark of Silicon
 * Graphics Inc. used by permission by Khronos.
 *
 * Use, duplication or disclosure by the Government is subject to restrictions
 * as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data and
 * Computer Software clause at DFARS 252.227-7013, and/or in similar or
 * successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished
 * rights reserved under the Copyright Laws of the United States and other
 * countries.
 */

#include <vx_conformance.h>

static vx_int32  chan_s32_pixels[] = { -INT32_MAX/2 + 1 };
static vx_uint32 chan_u32_pixels[] = { UINT32_MAX/2 + 1 };
static vx_int16  chan_s16_pixels[] = { -INT16_MAX/2 - 1 };
static vx_uint16 chan_u16_pixels[] = { UINT16_MAX/2 + 1 };
static vx_uint8  chan_u08_pixels[] = { 0x01, 0x02, 0x03, 0x4 };

#include "images/shapes.h"
#include "images/lena.h"
#include "images/bikegray.h"
#include "images/colorbars.h"

static vx_fourcc supported_formats[] = {
    0, // invalid format.
    FOURCC_RGB,
    FOURCC_RGBX,
    FOURCC_NV12,
    FOURCC_NV21,
    FOURCC_UYVY,
    FOURCC_YUYV,
    FOURCC_IYUV,
    FOURCC_YUV4,
    FOURCC_U8,
    FOURCC_U16,
    FOURCC_S16,
    FOURCC_U32,
    FOURCC_S32,
};

static void *pixels[] = {
    NULL,
    (void *)chan_u08_pixels,
    (void *)chan_u08_pixels,
    (void *)chan_u08_pixels,
    (void *)chan_u08_pixels,
    (void *)chan_u08_pixels,
    (void *)chan_u08_pixels,
    (void *)chan_u08_pixels,
    (void *)chan_u08_pixels,
    (void *)chan_u08_pixels,
    (void *)chan_u16_pixels,
    (void *)chan_s16_pixels,
    (void *)chan_u32_pixels,
    (void *)chan_s32_pixels,
};

/* the master list of base nodes and their interfaces */
vx_node_check_t base_nodes[] = {
        {VX_KERNEL_INVALID, "org.khronos.openvx.invalid", 0, {{0}}},
        {VX_KERNEL_COLOR_CONVERT, "org.khronos.openvx.color_convert", 2,
                {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                 {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_CHANNEL_EXTRACT, "org.khronos.openvx.channel_extract", 3,
                {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                 {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
                 {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_CHANNEL_COMBINE, "org.khronos.openvx.channel_combine", 5,
                {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                 {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                 {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_OPTIONAL},
                 {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_OPTIONAL},
                 {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_SOBEL_3x3, "org.khronos.openvx.sobel3x3", 3,
                {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                 {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_OPTIONAL},
                 {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_OPTIONAL}}},
        {VX_KERNEL_MAGNITUDE, "org.khronos.openvx.magnitude", 3,
                {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                 {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                 {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_PHASE, "org.khronos.openvx.phase", 3,
                {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                 {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                 {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_SCALE_IMAGE, "org.khronos.openvx.scale_image", 3,
                {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                 {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                 {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_OPTIONAL}}},
        {VX_KERNEL_TABLE_LOOKUP, "org.khronos.openvx.table_lookup", 3,
                {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                 {VX_INPUT, VX_TYPE_LUT, VX_PARAMETER_STATE_REQUIRED},
                 {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_HISTOGRAM, "org.khronos.openvx.histogram", 2,
                 {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                  {VX_OUTPUT, VX_TYPE_DISTRIBUTION, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_EQUALIZE_HISTOGRAM, "org.khronos.openvx.equalize_histogram", 2,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_ABSDIFF, "org.khronos.openvx.absdiff", 3,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_MEAN_STDDEV, "org.khronos.openvx.mean_stddev", 3,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_THRESHOLD, "org.khronos.openvx.threshold", 3,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_THRESHOLD, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_INTEGRAL_IMAGE, "org.khronos.openvx.integral_image", 2,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_DILATE_3x3, "org.khronos.openvx.dilate3x3", 2,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_ERODE_3x3, "org.khronos.openvx.erode3x3", 2,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_MEDIAN_3x3, "org.khronos.openvx.median3x3", 2,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_BOX_3x3, "org.khronos.openvx.box3x3", 2,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_GAUSSIAN_3x3, "org.khronos.openvx.gaussian3x3", 2,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_CUSTOM_CONVOLUTION, "org.khronos.openvx.custom_convolution", 3,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_CONVOLUTION, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_PYRAMID, "org.khronos.openvx.pyramid", 3,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_PYRAMID, VX_PARAMETER_STATE_OPTIONAL},
                {VX_OUTPUT, VX_TYPE_PYRAMID, VX_PARAMETER_STATE_OPTIONAL}}},
        {VX_KERNEL_ACCUMULATE, "org.khronos.openvx.accumulate", 2,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_BIDIRECTIONAL, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_ACCUMULATE_SQUARE, "org.khronos.openvx.accumulate_square", 2,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_BIDIRECTIONAL, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_ACCUMULATE_WEIGHTED, "org.khronos.openvx.accumulate_weighted", 3,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
                {VX_BIDIRECTIONAL, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_MINMAXLOC, "org.khronos.openvx.min_max_loc", 5,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_OPTIONAL},
                {VX_OUTPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_OPTIONAL},
                {VX_OUTPUT, VX_TYPE_COORDINATES, VX_PARAMETER_STATE_OPTIONAL},
                {VX_OUTPUT, VX_TYPE_COORDINATES, VX_PARAMETER_STATE_OPTIONAL}}},
        {VX_KERNEL_CONVERTDEPTH, "org.khronos.openvx.convert_depth", 4,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_CANNY_EDGE_DETECTOR, "org.khronos.openvx.canny_edge_detector", 5,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_THRESHOLD, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_AND, "org.khronos.openvx.and", 3,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_OR, "org.khronos.openvx.or", 3,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_XOR, "org.khronos.openvx.xor", 3,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_NOT, "org.khronos.openvx.not", 2,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_MULTIPLY, "org.khronos.openvx.multiply", 5,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_ADD, "org.khronos.openvx.add", 4,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_SUBTRACT, "org.khronos.openvx.subtract", 4,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_WARP_AFFINE, "org.khronos.openvx.warp_affine", 4,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_MATRIX, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_OPTIONAL},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_WARP_PERSPECTIVE, "org.khronos.openvx.warp_perspective", 4,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_MATRIX, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_OPTIONAL},
                {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_HARRIS_CORNERS, "org.khronos.openvx.harris_corners", 7,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_LIST, VX_PARAMETER_STATE_REQUIRED}}},
        {VX_KERNEL_FAST_CORNERS, "org.khronos.openvx.fast_corners", 4,
               {{VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
                {VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED},
                {VX_OUTPUT, VX_TYPE_LIST, VX_PARAMETER_STATE_REQUIRED}}},
        /*! \todo when Optical Flow is finalized, put it here. */
};

vx_image vxCreateTestImage(vx_context context,
                          vx_uint8 *ptr,
                          vx_uint32 channels,
                          vx_uint32 width,
                          vx_uint32 height,
                          vx_uint32 planes)
{
    vx_status status = VX_SUCCESS;
    void *base = NULL;
    vx_imagepatch_addressing_t addr;
    vx_uint32 p, y, x, c, i;
    vx_fourcc format = FOURCC_U8;
    vx_image image = 0;
    vx_rectangle rect = vxCreateRectangle(context, 0, 0, width, height);
    image = vxCreateImage(context, width, height, format);
    i = 0;
    for (p = 0; p < planes; p++)
    {
        status = vxAccessImagePatch(image, rect, p, &addr, &base);
        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                vx_uint8 *img = vxFormatImagePatchAddress2d(base, x, y, &addr);
                for (c = 0; c < channels; c++)
                {
                    i = (p * height * width * channels) +
                        (y * width * channels) +
                        (x * channels) +
                        (c);
                    img[c] = ptr[i];
                }
            }
        }
        status = vxCommitImagePatch(image, rect, p, &addr, base);
    }
    return image;
}

void conf_print(char *message, const char *function, vx_uint32 line, ...)
{
    vx_char format[1024];
    va_list ap;

    va_start(ap, line);
#if defined(WIN32)
    _snprintf(format, sizeof(format), "%20s@%u %s", function, line, message);
#else
    snprintf(format, sizeof(format), "%20s@%u %s", function, line, message);
#endif
    vprintf(format, ap);
    va_end(ap);
}

void conf_print_log(vx_context context)
{
    vx_char message[VX_MAX_LOG_MESSAGE_LEN];
    vx_status status = VX_SUCCESS;
    do {
        status = vxGetLogEntry(context, message);
        if (status != VX_SUCCESS)
        {
            CONF_PRINT("[status=%d] %s", status, message);
        }
    } while (status != VX_SUCCESS);
}

vx_status vxStatisticsOnImages(vx_context c, vx_image a, vx_image b,
                               vx_scalar mean, vx_scalar stddev,
                               vx_scalar minVal, vx_scalar maxVal)
{
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(c);
    if (graph)
    {
        vx_uint32 n = 0u;
        vx_image v = vxCreateVirtualImage(c);
        vx_node nodes[] = {
            vxAbsDiffNode(graph, a, b, v),
            vxMeanStdDevNode(graph, v, mean, stddev),
            vxMinMaxLocNode(graph, v, minVal, maxVal, 0, 0),
        };
        status = vxVerifyGraph(graph);
        if (status == VX_SUCCESS)
        {
            status = vxProcessGraph(graph);
        }
        for (n = 0u; n < dimof(nodes); n++)
        {
            vxReleaseNode(&nodes[n]);
        }
        vxReleaseGraph(&graph);
    }
    return status;
}

vx_status vxQuery(vx_reference ref, vx_attribute_query_t attr[], vx_size numAttr)
{
    vx_status status = VX_FAILURE;
    vx_size a = 0ul;
    for (a = 0ul; a < numAttr; a++)
    {
        if ((attr[a].attribute == VX_REF_ATTRIBUTE_COUNT) ||
            (attr[a].attribute == VX_REF_ATTRIBUTE_TYPE))
        {
            status = vxQueryReference((vx_reference)ref, attr[a].attribute, attr[a].out, attr[a].size);
        }
        else
        {
            vx_enum type = 0;
            status = vxQueryReference(ref, VX_REF_ATTRIBUTE_TYPE, &type, sizeof(type));
            if (status == VX_SUCCESS)
            {
                switch (type)
                {
                    case VX_TYPE_CONTEXT:
                        status = vxQueryContext((vx_context)ref, attr[a].attribute, attr[a].out, attr[a].size);
                        break;
                    case VX_TYPE_GRAPH:
                        status = vxQueryGraph((vx_graph)ref, attr[a].attribute, attr[a].out, attr[a].size);
                        break;
                    case VX_TYPE_KERNEL:
                        status = vxQueryKernel((vx_kernel)ref, attr[a].attribute, attr[a].out, attr[a].size);
                        break;
                    case VX_TYPE_NODE:
                        status = vxQueryNode((vx_node)ref, attr[a].attribute, attr[a].out, attr[a].size);
                        break;
                    case VX_TYPE_PARAMETER:
                        status = vxQueryParameter((vx_parameter)ref, attr[a].attribute, attr[a].out, attr[a].size);
                        break;
                    case VX_TYPE_DELAY:
                        status = VX_ERROR_NOT_SUPPORTED; // vxQueryDelay((vx_delay)ref, attr[a].attribute, attr[a].out, attr[a].size);
                        break;
                    case VX_TYPE_TARGET:
                        status = vxQueryTarget((vx_target)ref, attr[a].attribute, attr[a].out, attr[a].size);
                        break;
                    case VX_TYPE_RECTANGLE:
                        status = vxQueryRectangle((vx_rectangle)ref, attr[a].attribute, attr[a].out, attr[a].size);
                        break;
                    case VX_TYPE_LUT:
                        status = vxQueryLUT((vx_lut)ref, attr[a].attribute, attr[a].out, attr[a].size);
                        break;
                    case VX_TYPE_DISTRIBUTION:
                        status = vxQueryDistribution((vx_distribution)ref, attr[a].attribute, attr[a].out, attr[a].size);
                        break;
                    case VX_TYPE_PYRAMID:
                        status = vxQueryPyramid((vx_pyramid)ref, attr[a].attribute, attr[a].out, attr[a].size);
                        break;
                    case VX_TYPE_THRESHOLD:
                        status = vxQueryThreshold((vx_threshold)ref, attr[a].attribute, attr[a].out, attr[a].size);
                        break;
                    case VX_TYPE_MATRIX:
                        status = vxQueryMatrix((vx_matrix)ref, attr[a].attribute, attr[a].out, attr[a].size);
                        break;
                    case VX_TYPE_CONVOLUTION:
                        status = vxQueryConvolution((vx_convolution)ref, attr[a].attribute, attr[a].out, attr[a].size);
                        break;
                    case VX_TYPE_COORDINATES:
                        status = vxQueryCoordinates((vx_coordinates)ref, attr[a].attribute, attr[a].out, attr[a].size);
                        break;
                    case VX_TYPE_SCALAR:
                        status = VX_ERROR_NOT_SUPPORTED; //vxQueryScalar((vx_scalar)ref, attr[a].attribute, attr[a].out, attr[a].size);
                        break;
                    case VX_TYPE_IMAGE:
                    default: // some FOURCC code or otherwise
                        status = vxQueryImage((vx_image)ref, attr[a].attribute, attr[a].out, attr[a].size);
                        break;
                }
                if (status != VX_SUCCESS)
                {
                    CONF_PRINT("Failed (status %d) to query attribute 0x%x on type 0x%x\n", status, attr[a].attribute, type);
                    break;
                }
            }
            else
            {
                CONF_PRINT("Failed to query type of reference %x\n", ref);
                break;
            }
        }
    }
    return status;
}


/******************************************************************************/

/*!
 * \brief Test creating and releasing an environment.
 * \ingroup group_tests
 */
vx_status vx_conformance_framework_context(void)
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_uint32 num_kernels = 0u;
        vx_uint32 num_modules = 0u;
        vx_uint32 num_targets = 0u;
        vx_uint32 num_references = 0u;
        vx_uint16 version = 0u;
        vx_uint16 vendor_id = 0u;
        vx_char implementation[VX_MAX_IMPLEMENTATION_NAME];
        vx_size size = 0ul;
        vx_size dim = 0ul;
        vx_attribute_query_t queries[] = {
            VX_ATTR(VX_CONTEXT_ATTRIBUTE_VENDOR_ID, vendor_id),
            VX_ATTR(VX_CONTEXT_ATTRIBUTE_VERSION, version),
            VX_ATTR(VX_CONTEXT_ATTRIBUTE_NUMKERNELS, num_kernels),
            VX_ATTR(VX_CONTEXT_ATTRIBUTE_NUMMODULES, num_modules),
            VX_ATTR(VX_CONTEXT_ATTRIBUTE_NUMTARGETS, num_targets),
            VX_ATTR(VX_CONTEXT_ATTRIBUTE_NUMREFS, num_references),
            VX_ATTR(VX_CONTEXT_ATTRIBUTE_IMPLEMENTATION, implementation),
            VX_ATTR(VX_CONTEXT_ATTRIBUTE_EXTENSIONS_SIZE, size),
            VX_ATTR(VX_CONTEXT_ATTRIBUTE_CONVOLUTION_MAXIMUM_DIMENSION, dim),
        };
        status = vxQuery(context, queries, dimof(queries));
        if (status == VX_SUCCESS)
        {
            vx_char *extensions = malloc(size);
            CONF_PRINT("VENDOR: 0x%04x\n", vendor_id);
            CONF_PRINT("VERSION: 0x%04x\n", version);
            CONF_PRINT("IMPLEMENTATION: %s\n", implementation);
            status = vxQueryContext(context, VX_CONTEXT_ATTRIBUTE_EXTENSIONS, extensions, size);
            if (status == VX_SUCCESS)
            {
                CONF_PRINT("EXTENSIONS: %s\n", extensions);
            }
            CONF_PRINT("Kernels:%u Modules:%u Refs:%u\n", num_kernels, num_modules, num_references);
            free(extensions);
        }
        vxReleaseContext(&context);
    }
    return status;
}

/*! \brief Tests the initialization of images from the supported formats.
 * \ingroup group_tests
 */
vx_status vx_conformance_object_images(void)
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_uint32 i = 0;
        for (i = 0; i < dimof(supported_formats); i++)
        {
            vx_uint32 w,width = 640;
            vx_uint32 h,height = 480;
            vx_image image = vxCreateImage(context, width, height, supported_formats[i]);
            if (image != 0 && supported_formats[i] == 0)
            {
                CONF_PRINT("ERROR: Failed to not make invalid image\n");
                status = VX_ERROR_NOT_SUFFICIENT;
                break;
            }
            else if (image == 0 && supported_formats[i] != 0)
            {
                CONF_PRINT("ERROR: image index %u, type 0x%08x\n", i, supported_formats[i]);
                status = VX_ERROR_NOT_SUFFICIENT;
                break;
            }
            else
            {
                vx_int32 stride = 0;
                vx_uint32 planes = 0;
                vx_fourcc color = FOURCC_VIRT;
                vx_enum space = 0, range = 0;
                vx_attribute_query_t queries[] = {
                    VX_ATTR(VX_IMAGE_ATTRIBUTE_WIDTH, w),
                    VX_ATTR(VX_IMAGE_ATTRIBUTE_HEIGHT, h),
                    VX_ATTR(VX_IMAGE_ATTRIBUTE_FORMAT, color),
                    VX_ATTR(VX_IMAGE_ATTRIBUTE_PLANES, planes),
                    VX_ATTR(VX_IMAGE_ATTRIBUTE_SPACE, space),
                    VX_ATTR(VX_IMAGE_ATTRIBUTE_RANGE, range),
                };
                status = vxQuery(image, queries, dimof(queries));

                CONF_PRINT("images[%u]="VX_FMT_REF" wxh=%u,%u FOURCC=%x stride=%d planes=%u (%d)\n",
                    __FUNCTION__, __LINE__,
                    i, image, w, h, color, stride, planes, status);
                if (image == 0 && supported_formats[i] == 0 && status == VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                if (i == 0) // when we're creating a bad file, the rest is bad.
                {
                    continue;
                }
                /*! @todo add more checks? */
                if (planes == 0 || color != supported_formats[i] || w != width || h != height)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
            }
            vxReleaseImage(&image);
        }
        vxReleaseContext(&context);
    }
    return status;
}


/*! \brief Tests the initialization of images from the supported formats.
 * \ingroup group_tests
 */
vx_status vx_conformance_object_image_const(void)
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_uint32 i = 0;
        for (i = 0; i < dimof(supported_formats); i++)
        {
            vx_uint32 w,width = 320;
            vx_uint32 h,height = 240;
            vx_image image = vxCreateUniformImage(context, width, height, supported_formats[i], pixels[i]);
            if (image != 0 && supported_formats[i] == 0)
            {
                CONF_PRINT("ERROR: Failed to not make invalid image\n");
                vxReleaseImage(&image);
                status = VX_ERROR_NOT_SUFFICIENT;
                break;
            }
            else if (image == 0 && supported_formats[i] != 0)
            {
                CONF_PRINT("ERROR: image index %u, type 0x%08x\n", i, supported_formats[i]);
                status = VX_ERROR_NOT_SUFFICIENT;
                break;
            }
            else
            {
                vx_uint32 stride;
                vx_uint32 planes;
                vx_fourcc color;
                vx_enum space, range;
                vx_rectangle rect = 0;
                vx_imagepatch_addressing_t addr = VX_IMAGEPATCH_ADDR_INIT;
                void *base = NULL;
                vx_uint8 img[4][240][320]; /* large enough to handle a vx_uint32 single plane too */

                status = VX_SUCCESS;
                w = h = color = stride = planes = space = range = 0;
                if (vxQueryImage(image, VX_IMAGE_ATTRIBUTE_WIDTH, &w, sizeof(w)) != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
                if (vxQueryImage(image, VX_IMAGE_ATTRIBUTE_HEIGHT, &h, sizeof(h)) != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
                if (vxQueryImage(image, VX_IMAGE_ATTRIBUTE_FORMAT, &color, sizeof(color)) != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
                if (vxQueryImage(image, VX_IMAGE_ATTRIBUTE_PLANES, &planes, sizeof(planes)) != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
                if (vxQueryImage(image, VX_IMAGE_ATTRIBUTE_SPACE, &space, sizeof(space)) != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
                if (vxQueryImage(image, VX_IMAGE_ATTRIBUTE_RANGE, &range, sizeof(range)) != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
                CONF_PRINT("images[%u]="VX_FMT_REF" wxh=%u,%u FOURCC=%x stride=%d planes=%u (%d)\n",
                    __FUNCTION__, __LINE__,
                    i, image, w, h, color, stride, planes, status);
                if (image == 0 && supported_formats[i] == 0 && status == VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                if (i == 0) // when we're creating a bad file, the rest is bad.
                {
                    continue;
                }
                if (planes == 0 || color != supported_formats[i] || w != width || h != height)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                rect = vxCreateRectangle(context, 0, 0 , width, height);
                if (vxAccessImagePatch(image, rect, 0, &addr, &base) == VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                base = img; /* just make sure it's not NULL */
                if (vxAccessImagePatch(image, rect, 0, &addr, &base) != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                if (vxCommitImagePatch(image, rect, 0, &addr, base) != VX_ERROR_NOT_SUPPORTED)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                if (vxCommitImagePatch(image, 0, 0, &addr, base) != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                vxReleaseRectangle(&rect);
            }
            vxReleaseImage(&image);
        }
        vxReleaseContext(&context);
    }
    return status;
}


/*! \brief Tests the initialization of buffers.
 * \ingroup group_tests
 */
vx_status vx_conformance_object_buffers(void)
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_buffer buffers[10];
        vx_uint32 i = 0;
        for (i = 0; i < dimof(buffers); i++)
        {
            vx_size u = 0,unitSize = (vx_size)(1ul << (rand()%5));
            vx_size n = 0,numUnits = (vx_size)rand() % 1024;

            buffers[i] = vxCreateBuffer(context, unitSize, numUnits);
            if (buffers[i] != 0)
            {
                vx_size size;
                status = VX_SUCCESS;
                if (vxQueryBuffer(buffers[i], VX_BUFFER_ATTRIBUTE_UNITSIZE, &u, sizeof(u)) != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                if (vxQueryBuffer(buffers[i], VX_BUFFER_ATTRIBUTE_NUMUNITS, &n, sizeof(n)) != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                if (vxQueryBuffer(buffers[i], VX_BUFFER_ATTRIBUTE_SIZE, &size, sizeof(size)) != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                if (u != unitSize || n != numUnits)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                CONF_PRINT("buffers[%u]=" VX_FMT_REF " uxn=%u,%u size=%zu\n",
                    __FUNCTION__, __LINE__,
                    i, buffers[i], u, n, size);
            }
            else
            {
                status = VX_ERROR_INVALID_REFERENCE;
                break;
            }
        }
        for (i = 0; i < dimof(buffers); i++)
        {
            vxReleaseBuffer(&buffers[i]);
        }
        vxReleaseContext(&context);
    }
    return status;
}

/*!
 * \brief Test creating and destroy one of each of the base kernels.
 * \ingroup group_tests
 */
vx_status vx_conformance_framework_kernel(void)
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_kernel kernel = 0;
        vx_enum k;

        // invalid should always fail.
        kernel = vxGetKernelByEnum(context, VX_KERNEL_INVALID);
        if (kernel != 0)
        {
            CONF_PRINT("Invalid kernel did not fail!");
            status = VX_ERROR_NOT_SUFFICIENT;
            goto exit;
        }
        // all other base kernels should be "retrievable".
        for (k = VX_KERNEL_INVALID + 1; k < VX_KERNEL_MAX_1_0; k++)
        {
            kernel = vxGetKernelByEnum(context, k);
            if (kernel)
            {
                vx_uint32 numParams = 0;
                vx_char name[VX_MAX_KERNEL_NAME];
                vx_enum enumeration = 0;
                vx_size size = 0ul;
                void *ptr = NULL;

                status = vxQueryKernel(kernel, VX_KERNEL_ATTRIBUTE_NUMPARAMS, &numParams, sizeof(numParams));
                if (status != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                if (numParams == 0)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                status = vxQueryKernel(kernel, VX_KERNEL_ATTRIBUTE_NAME, name, sizeof(name));
                if (status != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                if (strcmp(name, "") == 0)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                status = vxQueryKernel(kernel, VX_KERNEL_ATTRIBUTE_ENUM, &enumeration, sizeof(enumeration));
                if (status != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                if (enumeration != k)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                status = vxQueryKernel(kernel, VX_KERNEL_ATTRIBUTE_LOCAL_DATA_SIZE, &size, sizeof(size));
                if (status != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
#if defined(OPENVX_KHR_NODE_MEMORY)
                status = vxQueryKernel(kernel, VX_KERNEL_ATTRIBUTE_GLOBAL_DATA_SIZE, &size, sizeof(size));
                if (status != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                status = vxQueryKernel(kernel, VX_KERNEL_ATTRIBUTE_GLOBAL_DATA_PTR, &ptr, sizeof(ptr));
                if (status != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                if ((size > 0 && ptr == NULL) || (ptr && size == 0))
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
#endif
                vxReleaseKernel(&kernel);
                status = VX_SUCCESS;
            }
            else
            {
                CONF_PRINT("[ERROR] Failed to retrieve kernel %d\n", k);
                status = VX_ERROR_NOT_SUFFICIENT;
                goto exit;
            }
        }
exit:
        vxReleaseContext(&context);
    }
    return status;
}

/*!
 * \brief Test creating and releasing a kernel in a node.
 * \ingroup group_tests
 */
vx_status vx_conformance_framework_graph(void)
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_graph graph = vxCreateGraph(context);
        if (graph)
        {
            vx_uint32 numNodes = 0xFFFFFFFF;
            vx_perf_t perf;
            vx_status graph_status = VX_FAILURE;
            do {
                status = vxQueryGraph(graph, VX_GRAPH_ATTRIBUTE_NUMNODES, &numNodes, sizeof(numNodes));
                if (status != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                if (numNodes != 0)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                status = vxQueryGraph(graph, VX_GRAPH_ATTRIBUTE_PERFORMANCE, &perf, sizeof(perf));
                if (status != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                status = vxQueryGraph(graph, VX_GRAPH_ATTRIBUTE_STATUS, &graph_status, sizeof(graph_status));
                if (status != VX_SUCCESS)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
                /* \todo what should an un-run graph status be? */

                /* check verify to make sure it returns an error with no nodes */
                status = vxVerifyGraph(graph);
                if (status != VX_ERROR_INVALID_GRAPH)
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
                else
                {
                    /* make sure a null graph is invalid */
                    status = vxProcessGraph(0);
                    if (!(status == VX_ERROR_INVALID_GRAPH ||
                          status == VX_ERROR_INVALID_REFERENCE))
                    {
                        status = VX_ERROR_NOT_SUFFICIENT;
                        goto failure;
                    }

                    /* check process to make sure it returns an error with no nodes */
                    status = vxProcessGraph(graph);
                    if (status != VX_ERROR_INVALID_GRAPH)
                    {
                        status = VX_ERROR_NOT_SUFFICIENT;
                    }
                    else
                    {
                        /* also check schedule/wait */
                        status = vxScheduleGraph(graph);
                        /*! \todo should schedule return an error? */

                        status = vxWaitGraph(graph);
                        if (status != VX_ERROR_INVALID_GRAPH)
                        {
                            status = VX_ERROR_NOT_SUFFICIENT;
                        }
                        else
                        {
                            status = VX_SUCCESS;
                        }
                    }
                }
            } while (0);
            vxReleaseGraph(&graph);
        }
        else
        {
            status = VX_ERROR_NOT_SUFFICIENT;
        }
failure:
        vxReleaseContext(&context);
    }
    return status;
}

vx_action conformance_callback(vx_node node)
{
    return VX_ACTION_ABANDON;
}

/*!
 * \brief Test creating and releasing a kernel in a node.
 * \ingroup group_tests
 */
vx_status vx_conformance_framework_node(void)
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_graph graph = vxCreateGraph(context);
        if (graph)
        {
            vx_enum k = 0;

            // for all valid base kernels... create a node with it.
            status = VX_SUCCESS;
            for (k = 0; (status == VX_SUCCESS) && (k < dimof(base_nodes)); k++)
            {
                vx_kernel kernel = vxGetKernelByEnum(context, base_nodes[k].kernel_enum);
                vx_kernel kernel2 = vxGetKernelByName(context, base_nodes[k].kernel_name);
                if (kernel && kernel2)
                {
                    vx_uint32 num = 0;
                    vx_char name[VX_MAX_KERNEL_NAME];
                    vx_enum kenum = VX_KERNEL_INVALID;
                    vx_size kgsize = 0, klsize = 0;
                    vx_attribute_query_t kern_attr[] = {
                            VX_ATTR(VX_KERNEL_ATTRIBUTE_NUMPARAMS, num),
                            VX_ATTR(VX_KERNEL_ATTRIBUTE_NAME, name),
                            VX_ATTR(VX_KERNEL_ATTRIBUTE_ENUM, kenum),
#if defined(OPENVX_KHR_NODE_MEMORY)
                            VX_ATTR(VX_KERNEL_ATTRIBUTE_GLOBAL_DATA_SIZE, kgsize),
#endif
                            VX_ATTR(VX_KERNEL_ATTRIBUTE_LOCAL_DATA_SIZE, klsize),
                    };
                    status = vxQuery(kernel, kern_attr, dimof(kern_attr));
                    if ((status == VX_SUCCESS) &&
                        (num == base_nodes[k].num) &&
                        (kenum == base_nodes[k].kernel_enum) &&
                        (strncmp(name, base_nodes[k].kernel_name, VX_MAX_KERNEL_NAME) == 0))
                    {
                        vx_node node = vxCreateNode(graph, kernel);
                        if (node)
                        {
                            vx_perf_t perf;
                            vx_status node_status = VX_FAILURE;
                            vx_size lsize = 0, gsize = 0;
                            void *lptr = NULL, *gptr = NULL;
                            vx_border_mode_t border;
                            vx_uint32 p = 0;
                            vx_attribute_query_t queries[] = {
                                    VX_ATTR(VX_NODE_ATTRIBUTE_PERFORMANCE, perf),
                                    VX_ATTR(VX_NODE_ATTRIBUTE_STATUS, node_status),
                                    VX_ATTR(VX_NODE_ATTRIBUTE_LOCAL_DATA_SIZE, lsize),
                                    VX_ATTR(VX_NODE_ATTRIBUTE_LOCAL_DATA_PTR, lptr),
#if defined(OPENVX_KHR_NODE_MEMORY)
                                    VX_ATTR(VX_NODE_ATTRIBUTE_GLOBAL_DATA_SIZE, gsize),
                                    VX_ATTR(VX_NODE_ATTRIBUTE_GLOBAL_DATA_PTR, gptr),
#endif
                                    VX_ATTR(VX_NODE_ATTRIBUTE_BORDER_MODE, border),
                            };
                            status = vxQuery(node, queries, dimof(queries));
                            if (status != VX_SUCCESS)
                            {
                                status = VX_ERROR_NOT_SUFFICIENT;
                                break;
                            }
                            /*! \todo check the default value of all attributes... */
                            status = vxAssignNodeCallback(node, conformance_callback);
                            if (status != VX_SUCCESS)
                            {
                                status = VX_ERROR_NOT_SUFFICIENT;
                                break;
                            }
                            for (p = 0; p < num; p++)
                            {
                                vx_parameter param = vxGetParameterByIndex(node, p);
                                if (param)
                                {
                                    vx_enum dir = 0, type = 0, state = 0;
                                    vx_attribute_query_t param_attr[] = {
                                            VX_ATTR(VX_PARAMETER_ATTRIBUTE_DIRECTION, dir),
                                            VX_ATTR(VX_PARAMETER_ATTRIBUTE_TYPE, type),
                                            VX_ATTR(VX_PARAMETER_ATTRIBUTE_STATE, state),
                                    };
                                    status = vxQuery(param, param_attr, dimof(param_attr));
                                    if ((status != VX_SUCCESS) ||
                                        (dir != base_nodes[k].parameters[p].direction) ||
                                        (type != base_nodes[k].parameters[p].type) ||
                                        (state != base_nodes[k].parameters[p].state))
                                    {
                                        CONF_PRINT("Invalid %s.parameter[%u]\n", name, p);
                                        status = VX_ERROR_NOT_SUFFICIENT;
                                        break;
                                    }
                                }
                                else
                                {
                                    CONF_PRINT("Invalid parameter[%u]\n", p);
                                    status = VX_ERROR_NOT_SUFFICIENT;
                                    break;
                                }
                            }
                            vxReleaseNode(&node);
                        }
                        else
                        {
                            CONF_PRINT("Failed to create node for %s\n", name);
                            status = VX_ERROR_NOT_SUFFICIENT;
                        }
                    }
                    else if (status != VX_SUCCESS)
                    {
                        CONF_PRINT("Failed to query kernel attributes (%d)!\n", status);
                        status = VX_ERROR_NOT_SUFFICIENT;
                    }
                    else
                    {
                        CONF_PRINT("Failed to match kernel attributes for kernel %s(0x%x)[%d]! reported:%s(0x%x)[%d]\n", base_nodes[k].kernel_name, base_nodes[k].kernel_enum, base_nodes[k].num, name, kenum, num);
                        status = VX_ERROR_NOT_SUFFICIENT;
                    }
                    vxReleaseKernel(&kernel);
                }
                else if (base_nodes[k].kernel_enum != VX_KERNEL_INVALID)
                {
                    CONF_PRINT("Failed to get kernel enum 0x%x\n", base_nodes[k].kernel_enum);
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
                else
                {
                    status = VX_SUCCESS;
                }
            }
            vxReleaseGraph(&graph);
        }
        else
        {
            CONF_PRINT("Failed to create graph!\n");
            status = VX_ERROR_NOT_SUFFICIENT;
        }
        vxReleaseContext(&context);
    }
    else
    {
        CONF_PRINT("Failed to open context!\n");
        status = VX_ERROR_NOT_SUFFICIENT;
    }
    return status;
}

/*!
 * \brief Test creating and releasing a kernel in a node.
 * \ingroup group_tests
 */
vx_status vx_conformance_framework_noload_extension(void)
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        status = vxLoadKernels(context, "zyx");
        if (status == VX_SUCCESS)
        {
            CONF_PRINT("Loaded non-existant module!");
            status = VX_ERROR_NOT_SUFFICIENT; // should not have passed!
        }
        else
        {
            vx_kernel kernel = vxGetKernelByName(context, "org.khronos.example.zyx");
            if (kernel)
            {
                vxReleaseKernel(&kernel);
                status = VX_ERROR_NOT_SUFFICIENT; // this should have failed!
            }
            else
            {
                status = VX_SUCCESS;
            }
        }
        vxReleaseContext(&context);
    }
    return status;
}

vx_status vx_conformance_framework_targets(void)
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_uint32 numTargets = 0;
        vx_char implementation[VX_MAX_IMPLEMENTATION_NAME];

        status = vxQueryContext(context, VX_CONTEXT_ATTRIBUTE_IMPLEMENTATION, implementation, sizeof(implementation));
        if (status == VX_SUCCESS)
        {
            CONF_PRINT("Implementation: %s\n", implementation);
        }
        else
        {
            return status;
        }
        // this should show up in the kernel list
        status = vxLoadKernels(context, "xyz");
        if (status != VX_SUCCESS)
        {
            return status;
        }
        status = vxQueryContext(context, VX_CONTEXT_ATTRIBUTE_NUMTARGETS, &numTargets, sizeof(numTargets));
        if (status == VX_SUCCESS && numTargets > 0)
        {
            vx_uint32 t = 0;
            CONF_PRINT("There are %u targets\n", numTargets);
            for (t = 0; t < numTargets; t++)
            {
                vx_target target = vxGetTargetByIndex(context, t);
                if (target)
                {
                    vx_char targetname[VX_MAX_TARGET_NAME];
                    vx_uint32 numKernels = 0;

                    status = vxQueryTarget(target, VX_TARGET_ATTRIBUTE_NAME, targetname, sizeof(targetname));
                    if (status != VX_SUCCESS)
                        return status;
                    status = vxQueryTarget(target, VX_TARGET_ATTRIBUTE_NUMKERNELS, &numKernels, sizeof(numKernels));
                    CONF_PRINT("Target[%u] %s has %u kernels (%d)\n", t, targetname, numKernels, status);
                    if (status == VX_SUCCESS && numKernels > 0)
                    {
                        vx_target_kernel_t *table = calloc(numKernels, sizeof(vx_target_kernel_t));
                        status = vxQueryTarget(target, VX_TARGET_ATTRIBUTE_KERNELTABLE, table, numKernels * sizeof(vx_target_kernel_t));
                        if (status == VX_SUCCESS)
                        {
                            vx_uint32 k;
                            for (k = 0; k < numKernels; k++)
                            {
                                CONF_PRINT("%s: kernel[%d]=%s\n", targetname, table[k].enumeration, table[k].name);
                            }
                        }
                    }
                }
                else
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                    break;
                }
            }
        }
        vxReleaseContext(&context);
    }
    return status;
}

vx_status vx_conformance_framework_req_param(void)
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_graph graph = vxCreateGraph(context);
        if (graph)
        {
            vx_kernel kernel = vxGetKernelByEnum(context, VX_KERNEL_INVALID + 1);
            vx_node node = vxCreateNode(graph, kernel);
            if (node)
            {
                /*! check to make sure at least one is required */
                vx_parameter parameter = vxGetParameterByIndex(node, 0);
                vx_enum state = -1;
                vxQueryParameter(parameter, VX_PARAMETER_ATTRIBUTE_STATE, &state, sizeof(state));
                if (state == VX_PARAMETER_STATE_REQUIRED)
                {
                    /*! don't set the parameters on a kernel that requires them,
                     * this is a negative test.
                     */
                    status = vxVerifyGraph(graph);
                    if (status != VX_ERROR_NOT_SUFFICIENT)
                        status = VX_ERROR_NOT_SUFFICIENT;
                    else
                        status = VX_SUCCESS;
                }
                vxReleaseNode(&node);
                vxReleaseKernel(&kernel);
            }
            vxReleaseGraph(&graph);
        }
        vxReleaseContext(&context);
    }
    return status;
}

/*!
 * \brief Test usage of the asynchronous interfaces.
 * \ingroup group_tests
 */
vx_status vx_conformance_framework_async(void)
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_uint32 i = 0, w = 640, h = 480;
        vx_image images[] = {
            vxCreateImage(context, w, h, FOURCC_RGB),
            vxCreateImage(context, w, h, FOURCC_U8),
            vxCreateImage(context, w, h, FOURCC_U8),
            vxCreateImage(context, w, h, FOURCC_U8),
        };
        vx_graph graphs[] = {
            vxCreateGraph(context),
            vxCreateGraph(context),
            vxCreateGraph(context),
        };
        if (graphs[0] && graphs[1] && graphs[2])
        {
            vx_node nodes[] = {
                vxChannelExtractNode(graphs[0], images[0], VX_CHANNEL_R, images[1]),
                vxChannelExtractNode(graphs[1], images[0], VX_CHANNEL_G, images[2]),
                vxChannelExtractNode(graphs[2], images[0], VX_CHANNEL_B, images[3]),
            };
            status = VX_SUCCESS;
            for (i = 0; i < dimof(graphs); i++)
            {
                status |= vxVerifyGraph(graphs[i]);
            }
            if (status == VX_SUCCESS)
            {
                for (i = 0; i < dimof(graphs); i++)
                {
                    status |= vxScheduleGraph(graphs[i]);
                }
                if (status == VX_SUCCESS)
                {
                    for (i = 0; i < dimof(graphs); i++)
                    {
                        status |= vxWaitGraph(graphs[i]);
                    }
                }
            }
            if (status != VX_SUCCESS)
            {
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            for (i = 0; i < dimof(nodes); i++)
            {
                vxReleaseNode(&nodes[i]);
            }
            for (i = 0; i < dimof(graphs); i++)
            {
                vxReleaseGraph(&graphs[i]);
            }
        }
        conf_print_log(context);
        vxReleaseContext(&context);
    }
    return status;
}

static vx_uint32 counter;

static vx_action counter_callback(vx_node node)
{
    vx_status status = VX_FAILURE;
    vx_action action = VX_ACTION_CONTINUE;

    if (vxQueryNode(node, VX_NODE_ATTRIBUTE_STATUS, &status, sizeof(status)) == VX_SUCCESS)
    {
        if (status == VX_SUCCESS)
        {
            counter++;
            if (counter < 2)
            {
                printf("Restarting Graph! (%u)\n", counter);
                action = VX_ACTION_RESTART;
            }
            else if (2 <= counter && counter < 3)
            {
                printf("Abandoning Graph! (%u)\n", counter);
                action = VX_ACTION_ABANDON;
            }
            else
            {
                printf("Continuing Graph! (%u)\n", counter);
            }
        }
        else
        {
            printf("Node failed to execute, status = %d\n", status);
            action = VX_ACTION_ABANDON;
        }
    }
    else
    {
        printf("Can't query node!\n");
    }
    return action;
}

/*!
 * \brief Test which uses callbacks control graph execution.
 * \ingroup group_tests
 */
vx_status vx_conformance_framework_callback(void)
{
    vx_status status = VX_FAILURE;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_graph graph = vxCreateGraph(context);
        if (graph)
        {
            vx_uint32 w = 640, h = 480;
            vx_image images[] = {
                vxCreateImage(context, w, h, FOURCC_RGB),
                vxCreateImage(context, w, h, FOURCC_IYUV),
            };
            vx_node node = vxColorConvertNode(graph, images[0], images[1]);
            if (node)
            {
                counter = 0;
                status = vxAssignNodeCallback(node, counter_callback);
                if (status == VX_SUCCESS)
                {
                    status = vxVerifyGraph(graph);
                    if (status == VX_SUCCESS)
                    {
                        do {
                            status = vxProcessGraph(graph);
                        } while (status != VX_SUCCESS);

                        if (counter != 3)
                            status = VX_ERROR_NOT_SUFFICIENT;
                    }
                }
            }
        }
    }
    return status;
}

/*!
 * \brief Test for logging features.
 * \ingroup group_tests
 */
vx_status vx_conformance_framework_log(void)
{
    vx_status status = VX_SUCCESS;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_graph graph = vxCreateGraph(context);
        if (graph)
        {
            vx_uint32 errnum = 0;
            vx_uint32 logs = 0;
            vx_char message[VX_MAX_LOG_MESSAGE_LEN];

            /* add a log entry starting from VX_MIN_ERROR all the way to */
            // (but not including) success.
            for (status = VX_STATUS_MIN, logs = 0; status != VX_SUCCESS; status++,logs++)
            {
                vxAddLogEntry(graph, status, "%s[%u][%d] this is a log entry\n", __FUNCTION__, logs, status);
            }

            do {
                // pass either context or graph, both should work.
                status = vxGetLogEntry(rand()&1?context:graph, message);
                // this makes sure that the status' are changing
                if (((vx_int32)status - errnum) != VX_STATUS_MIN)
                {
                    break;
                }
                if (status != VX_SUCCESS)
                {
                    CONF_PRINT("[%05u] status=%d %s", errnum++, status, message);
                }
                else
                {
                    break;
                }
            } while (status != VX_SUCCESS);
            if (errnum != logs)
            {
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            vxReleaseGraph(&graph);
        }
        vxReleaseContext(&context);
    }
    return status;
}

/*! \brief Tests the initialization of LUTs.
 * \ingroup group_tests
 */
vx_status vx_conformance_object_distribution(void)
{
    vx_status status = VX_ERROR_NOT_SUFFICIENT;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_distribution dist = vxCreateDistribution(context, 16, 0, 256);
        if (dist)
        {
            vx_uint32 range = 0;
            vx_uint32 bins = 0;
            vx_uint32 window_size = 0;
            vx_uint32 offset = 0;
            vx_size dims = 0;

            status  = VX_SUCCESS;
            if (vxQueryDistribution(dist, VX_DISTRIBUTION_ATTRIBUTE_DIMENSIONS, &dims, sizeof(dims)) != VX_SUCCESS)
            {
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            if (vxQueryDistribution(dist, VX_DISTRIBUTION_ATTRIBUTE_BINS, &bins, sizeof(bins)) != VX_SUCCESS)
            {
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            if (vxQueryDistribution(dist, VX_DISTRIBUTION_ATTRIBUTE_RANGE, &range, sizeof(range)) != VX_SUCCESS)
            {
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            if (vxQueryDistribution(dist, VX_DISTRIBUTION_ATTRIBUTE_OFFSET, &offset, sizeof(offset)) != VX_SUCCESS)
            {
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            if (vxQueryDistribution(dist, VX_DISTRIBUTION_ATTRIBUTE_WINDOW, &window_size, sizeof(window_size)) != VX_SUCCESS)
            {
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            if ((status != VX_SUCCESS) || (range != 256) || (dims != 1) || (bins != 16) || (window_size != 16))
            {
                CONF_PRINT("Failed to get values! range=%lu dims=%lu bins=%lu windows_size=%lu offset=%u\n",
                        __FUNCTION__, __LINE__, range, dims, bins, window_size, offset);
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            else
            {
                status = VX_SUCCESS;
            }
            vxReleaseDistribution(&dist);
        }
        vxReleaseContext(&context);
    }
    return status;
}

#ifdef OPENVX_KHR_LIST
vx_enum vxScoreSorter(vx_reference a, vx_reference b)
{
    vx_coordinates ca = (vx_coordinates)a;
    vx_coordinates cb = (vx_coordinates)b;
    vx_int32 cav[3];
    vx_int32 cbv[3];
    vx_enum cmp = VX_COMPARE_UNKNOWN;
    if ((vxAccessCoordinates(ca, cav) == VX_SUCCESS) &&
        (vxAccessCoordinates(cb, cbv) == VX_SUCCESS))
    {
        if (cav[2] < cbv[2])
            cmp = VX_COMPARE_LT;
        else if (cav[2] == cbv[2])
            cmp = VX_COMPARE_EQ;
        else if (cav[2] > cbv[2])
            cmp = VX_COMPARE_GT;
        vxCommitCoordinates(ca, NULL);
        vxCommitCoordinates(cb, NULL);
    }
    CONF_PRINT("Compare "VX_FMT_REF", "VX_FMT_REF" (%d ? %d) yields 0x%x\n", ca, cb, cav[2], cbv[2], cmp);
    if (cmp == VX_COMPARE_UNKNOWN)
    {
        CONF_PRINT("FATAL: References given to list sorter are not desired type!\n");
        exit(-1);
    }
    return cmp;
}
#endif

vx_status vx_conformance_object_list(void)
{
    vx_status status = VX_ERROR_NOT_SUFFICIENT;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_size c, count = 10ul;
        vx_uint32 w = 640, h = 480, top = 1000;
        vx_reference ref = 0;
        vx_int32 score = 0;
        vx_enum type = 0;
        vx_iterator it = 0;
        vx_list list = vxCreateList(context, VX_TYPE_COORDINATES, count);
        if (list)
        {
            c = count + 1;
            status = VX_SUCCESS;
            status |= vxQueryList(list, VX_LIST_ATTRIBUTE_LENGTH, &c, sizeof(c));
            status |= vxQueryList(list, VX_LIST_ATTRIBUTE_TYPE, &type, sizeof(type));
            if ((status != VX_SUCCESS) || (type != VX_TYPE_COORDINATES) || (c != 0))
            {
                CONF_PRINT("Failed to create list correctly! (status=%d, type=%d, c=%lu)\n", status, type, c);
                status = VX_ERROR_NOT_SUFFICIENT;
                goto exit;
            }
            for (c = 0ul; c < count; c++)
            {
                vx_coordinates coord = vxCreateCoordinates(context, 3);
                if (coord)
                {
                    vx_int32 v[3] = {rand() % w, rand() % h, rand() % top};
                    vxAccessCoordinates(coord, NULL);
                    vxCommitCoordinates(coord, v);
                    CONF_PRINT("Created coordinates v[3]={%d, %d, %d}\n", v[0], v[1], v[2]);
                    status = vxAddListItem(list, coord);
                    if (status != VX_SUCCESS)
                    {
                        CONF_PRINT("Failed to add item to list!\n");
                        status = VX_ERROR_NOT_SUFFICIENT;
                        goto exit;
                    }
                    vxReleaseCoordinates(&coord);
                }
            }
            c = count + 1;
            status = vxQueryList(list, VX_LIST_ATTRIBUTE_LENGTH, &c, sizeof(c));
            if ((status != VX_SUCCESS) || (c != count))
            {
                CONF_PRINT("Failed to add to list correctly!\n");
                status = VX_ERROR_NOT_SUFFICIENT;
                goto exit;
            }
#if defined(OPENVX_KHR_LIST)
            status = vxSortList(list, vxScoreSorter);
            if (status != VX_SUCCESS)
            {
                CONF_PRINT("Failed to sort list correctly!\n");
                status = VX_ERROR_NOT_SUFFICIENT;
                goto exit;
            }
            else
            {
                CONF_PRINT("Sorted list!\n");
            }
#endif
            for (ref  = vxGetListItem(list, &it, VX_LIST_FRONT), c = 0ul;
                 ref != 0;
                 ref  = vxGetListItem(list, &it, VX_LIST_NEXT))
            {
                if (ref)
                {
                    vx_int32 v[3] = {0,0,0};
                    if (ref == vxGetListItem(list, &it, VX_LIST_FIRST))
                    {
                        CONF_PRINT("Forwards order:\n");
                        c++;
                    }
                    vxAccessCoordinates((vx_coordinates)ref, v);
                    CONF_PRINT("v[3] = {%d, %d, %d}\n", v[0], v[1], v[2]);
                    vxCommitCoordinates((vx_coordinates)ref, NULL);
#if defined(OPENVX_KHR_LIST)
                    if (score > v[2])
                    {
                        CONF_PRINT("Scores were sorted incorrectly!\n");
                        status = VX_ERROR_NOT_SUFFICIENT;
                        goto exit;
                    }
                    score = v[2];
#endif
                }
                else
                {
                    CONF_PRINT("Failed to get list item!\n");
                    status = VX_ERROR_NOT_SUFFICIENT;
                    goto exit;
                }
            }
            if (c != 1)
            {
                CONF_PRINT("Failed to get first list item!\n");
                status = VX_ERROR_NOT_SUFFICIENT;
                goto exit;
            }
            c = 0ul;
            score = top;
            CONF_PRINT("Backwards order:\n");
            for (ref  = vxGetListItem(list, &it, VX_LIST_BACK);
                 ref != 0;
                 ref  = vxGetListItem(list, &it, VX_LIST_PREVIOUS))
            {
                if (ref)
                {
                    vx_int32 v[3] = {0,0,0};
                    if (ref == vxGetListItem(list, &it, VX_LIST_LAST))
                    {
                        /* since we are erasing too,
                         * this should get hit every time!
                         */
                        c++;
                    }
                    vxAccessCoordinates((vx_coordinates)ref, v);
                    CONF_PRINT("v[3] = {%d, %d, %d}\n", v[0], v[1], v[2]);
                    vxCommitCoordinates((vx_coordinates)ref, NULL);
#ifdef OPENVX_KHR_LIST
                    if (score < v[2])
                    {
                        CONF_PRINT("Scores were sorted incorrectly!\n");
                        status = VX_ERROR_NOT_SUFFICIENT;
                        goto exit;
                    }
                    score = v[2];
                    /* remove the item */
                    status = vxEraseListItem(list, &it, ref);
                    if (status != VX_SUCCESS)
                    {
                        CONF_PRINT("Failed to erase ref "VX_FMT_REF"!\n", ref);
                        status = VX_ERROR_NOT_SUFFICIENT;
                        goto exit;
                    }
#endif
                }
                else
                {
                    CONF_PRINT("Failed to get list item!\n");
                    status = VX_ERROR_NOT_SUFFICIENT;
                    goto exit;
                }
            }
#ifdef OPENVX_KHR_LIST
            if (c != count)
            {
                CONF_PRINT("Failed to get last item!\n");
                status = VX_ERROR_NOT_SUFFICIENT;
                goto exit;
            }
            c = count + 1;
            status = vxQueryList(list, VX_LIST_ATTRIBUTE_LENGTH, &c, sizeof(c));
            if ((status != VX_SUCCESS) && (c != 0))
            {
                CONF_PRINT("List is not empty!\n");
                status = VX_ERROR_NOT_SUFFICIENT;
                goto exit;
            }
            CONF_PRINT("Recreating list for inserting!\n");
            /* add back a bunch of stuff again, but inserted now */
            for (c = 0ul; c < count; c++)
            {
                vx_coordinates coord = vxCreateCoordinates(context, 3);
                if (coord)
                {
                    vx_int32 v[3] = {rand() % w, rand() % h, rand() % top};
                    vxAccessCoordinates(coord, NULL);
                    vxCommitCoordinates(coord, v);
                    CONF_PRINT("Created coordinates v[3]={%d, %d, %d}\n", v[0], v[1], v[2]);
                    status = vxInsertListItem(list, coord, vxScoreSorter);
                    if (status != VX_SUCCESS)
                    {
                        CONF_PRINT("Failed to add item to list!\n");
                        status = VX_ERROR_NOT_SUFFICIENT;
                        goto exit;
                    }
                    vxReleaseCoordinates(&coord);
                }
            }
#endif
            score = 0;
            CONF_PRINT("Forwards order (2):\n");
            for (ref  = vxGetListItem(list, &it, VX_LIST_FRONT);
                 ref != 0;
                 ref  = vxGetListItem(list, &it, VX_LIST_NEXT))
            {
                vx_int32 v[3] = {0,0,0};
                vxAccessCoordinates((vx_coordinates)ref, v);
                CONF_PRINT("v[3] = {%d, %d, %d}\n", v[0], v[1], v[2]);
                vxCommitCoordinates((vx_coordinates)ref, NULL);
#ifdef OPENVX_KHR_LIST
                if (score > v[2])
                {
                    CONF_PRINT("Scores were sorted incorrectly!\n");
                    status = VX_ERROR_NOT_SUFFICIENT;
                    goto exit;
                }
                score = v[2];
                status = vxEraseListItem(list, &it, ref);
                if (status != VX_SUCCESS)
                {
                    CONF_PRINT("Failed to erase ref "VX_FMT_REF"!\n", ref);
                    status = VX_ERROR_NOT_SUFFICIENT;
                    goto exit;
                }
            }
            c = count + 1;
            status = vxQueryList(list, VX_LIST_ATTRIBUTE_LENGTH, &c, sizeof(c));
            if ((status != VX_SUCCESS) && (c != 0))
            {
                CONF_PRINT("List is not empty!\n");
                status = VX_ERROR_NOT_SUFFICIENT;
                goto exit;
            }
#endif
exit:
            /* this should remove all items */
            vxReleaseList(&list);
        }
        vxReleaseContext(&context);
    }
    return status;
}


/*! \brief Tests the initialization of LUTs.
 * \ingroup group_tests
 */
vx_status vx_conformance_object_lut(void)
{
    vx_status status = VX_ERROR_NOT_SUFFICIENT;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_lut lut = vxCreateLUT(context, VX_TYPE_UINT8, 256);
        if (lut)
        {
            vx_uint8 mylut[256];
            vx_uint8 *tmp = NULL;
            vx_uint32 i;

            tmp = &mylut[0];
            status = vxAccessLUT(lut, (void **)&tmp);
            if (status == VX_SUCCESS)
            {
                if (tmp != mylut)
                {
                    CONF_PRINT("Failed to reuse given pointer!\n");
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
                else
                {
                    for (i = 0; i < dimof(mylut); i++)
                    {
                        tmp[i] = i;
                    }
                    status = vxCommitLUT(lut, tmp);
                    if (status == VX_SUCCESS)
                    {
                        tmp = NULL;
                        status = vxAccessLUT(lut, (void **)&tmp);
                        if (status == VX_SUCCESS)
                        {
                            if (memcmp(tmp, mylut, sizeof(mylut)) == 0)
                            {
                                status = VX_SUCCESS;
                            }
                            else
                            {
                                CONF_PRINT("LUTs are not equal!\n");
                                status = VX_ERROR_NOT_SUFFICIENT;
                            }
                            if (vxCommitLUT(lut, tmp) != VX_SUCCESS)
                            {
                                CONF_PRINT("Failed to set LUT (2)!\n");
                                status = VX_ERROR_NOT_SUFFICIENT;
                            }
                        }
                        else
                        {
                            CONF_PRINT("Failed to get LUT (2)!\n");
                            status = VX_ERROR_NOT_SUFFICIENT;
                        }
                    }
                    else
                    {
                        CONF_PRINT("Failed to set LUT (1)!\n");
                        status = VX_ERROR_NOT_SUFFICIENT;
                    }
                }
            }
            else
            {
                CONF_PRINT("Failed to get LUT (1)!\n");
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            vxReleaseLUT(&lut);
        }
        else
        {
            CONF_PRINT("Failed to create LUT!\n");
        }
        vxReleaseContext(&context);
    }
    return status;
}

vx_status vx_conformance_object_matrix(void)
{
    vx_status status = VX_SUCCESS;
    vx_context context = vxCreateContext();
    if (context)
    {
#define COLUMNS 3
#define ROWS 4
        vx_matrix matrix = vxCreateMatrix(context, VX_TYPE_FLOAT32, COLUMNS, ROWS);
        if (matrix)
        {
            vx_enum type = 0;
            vx_size columns = 0ul, rows = 0ul, size = 0ul, c = 0ul, r = 0ul;
            vx_float32 mat[ROWS][COLUMNS];
            vx_attribute_query_t queries[] = {
                    VX_ATTR(VX_MATRIX_ATTRIBUTE_COLUMNS, columns),
                    VX_ATTR(VX_MATRIX_ATTRIBUTE_ROWS, rows),
                    VX_ATTR(VX_MATRIX_ATTRIBUTE_TYPE, type),
                    VX_ATTR(VX_MATRIX_ATTRIBUTE_SIZE, size),
            };
            status = vxQuery(matrix, queries, dimof(queries));
            if ((COLUMNS != columns) || (ROWS != rows) || (type != VX_TYPE_FLOAT32))
            {
                CONF_PRINT("Matrix was not able to set attributes!\n",__FUNCTION__,__LINE__);
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            if (vxAccessMatrix(matrix, NULL) == VX_SUCCESS)
            {
                for (r = 0; r < rows; r++)
                {
                    for (c = 0; c < columns; c++)
                    {
                        mat[r][c] = (vx_float32)((r * columns) + c);
                    }
                }
                if (vxCommitMatrix(matrix, mat) == VX_SUCCESS)
                {
                    memset(mat, 0, sizeof(mat));
                    if (vxAccessMatrix(matrix, mat) == VX_SUCCESS)
                    {
                        for (r = 0; r < rows; r++)
                        {
                            for (c = 0; c < columns; c++)
                            {
                                if (mat[r][c] != (r * columns) + c)
                                {
                                    status = VX_ERROR_NOT_SUFFICIENT;
                                }
                            }
                        }
                        if (vxCommitMatrix(matrix, NULL) != VX_SUCCESS)
                        {
                            CONF_PRINT("Could not set matrix 2!\n",__FUNCTION__,__LINE__);
                            status = VX_ERROR_NOT_SUFFICIENT;
                        }
                    }
                    else
                    {
                        CONF_PRINT("Could not get matrix 2!\n",__FUNCTION__,__LINE__);
                        status = VX_ERROR_NOT_SUFFICIENT;
                    }
                }
                else
                {
                    CONF_PRINT("Could not set matrix!\n",__FUNCTION__,__LINE__);
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
            }
            else
            {
                CONF_PRINT("Could not get matrix!\n",__FUNCTION__,__LINE__);
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            vxReleaseMatrix(&matrix);
        }
        vxReleaseContext(&context);
    }
    return status;
}

vx_status vx_conformance_object_coordinates(void)
{
    vx_status status = VX_SUCCESS;
    vx_context context = vxCreateContext();
    if (context)
    {
#define M 3
        vx_coordinates coord = vxCreateCoordinates(context, M);
        if (coord)
        {
            vx_int32 array[M];
            vx_size dims = 0;
            if (vxQueryCoordinates(coord, VX_COORDINATES_ATTRIBUTE_DIMENSIONS, &dims, sizeof(dims)) != VX_SUCCESS)
            {
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            if (M != dims)
            {
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            if (vxAccessCoordinates(coord, NULL) == VX_SUCCESS)
            {
                array[0] = 1;
                array[1] = 2;
                array[2] = 3;
                if (vxCommitCoordinates(coord, array) == VX_SUCCESS)
                {
                    memset(array, 0, sizeof(array));
                    if (vxAccessCoordinates(coord, array) == VX_SUCCESS)
                    {
                        if ((array[0] == 1) &&
                            (array[1] == 2) &&
                            (array[2] == 3))
                        {
                            if (vxCommitCoordinates(coord, NULL) != VX_SUCCESS)
                            {
                                status = VX_ERROR_NOT_SUFFICIENT;
                            }
                        }
                        else
                        {
                            status = VX_ERROR_NOT_SUFFICIENT;
                        }
                    }
                    else
                    {
                        status = VX_ERROR_NOT_SUFFICIENT;
                    }
                }
                else
                {
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
            }
            else
            {
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            vxReleaseCoordinates(&coord);
        }
        vxReleaseContext(&context);
    }
    return status;
}

vx_status vx_conformance_object_convolution(void)
{
    vx_status status = VX_SUCCESS;
    vx_context context = vxCreateContext();
    if (context)
    {
#define M 3
        vx_coordinates conv = vxCreateConvolution(context, M, M);
        if (conv)
        {
            vx_int16 sobel[M][M] = {
                {-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1},
            };
            vx_size dims = 0;
            if (vxQueryConvolution(conv, VX_CONVOLUTION_ATTRIBUTE_ROWS, &dims, sizeof(dims)) != VX_SUCCESS)
            {
                CONF_PRINT("Failed to query convolution\n",__FUNCTION__,__LINE__);
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            if (M != dims)
            {
                CONF_PRINT("Failed to get convolution rows\n",__FUNCTION__,__LINE__);
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            if (vxQueryConvolution(conv, VX_CONVOLUTION_ATTRIBUTE_COLUMNS, &dims, sizeof(dims)) != VX_SUCCESS)
            {
                CONF_PRINT("Failed to query convolution\n",__FUNCTION__,__LINE__);
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            if (M != dims)
            {
                CONF_PRINT("Failed to get convolution columns\n",__FUNCTION__,__LINE__);
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            if (vxAccessConvolutionCoefficients(conv, NULL) == VX_SUCCESS)
            {
                if (vxCommitConvolutionCoefficients(conv, (vx_int16 *)sobel) == VX_SUCCESS)
                {
                    memset(sobel, 0, sizeof(sobel));
                    if (vxAccessConvolutionCoefficients(conv, (vx_int16 *)sobel) == VX_SUCCESS)
                    {
                        if ((sobel[0][0] == -1) && (sobel[0][1] == 0) && (sobel[0][2] == 1) &&
                            (sobel[1][0] == -2) && (sobel[1][1] == 0) && (sobel[1][2] == 2) &&
                            (sobel[2][0] == -1) && (sobel[2][1] == 0) && (sobel[2][2] == 1))
                        {
                            if (vxCommitConvolutionCoefficients(conv, NULL) != VX_SUCCESS)
                            {
                                CONF_PRINT("Failed to release convolution\n",__FUNCTION__,__LINE__);
                                status = VX_ERROR_NOT_SUFFICIENT;
                            }
                        }
                        else
                        {
                            CONF_PRINT("Failed to change convolution\n",__FUNCTION__,__LINE__);
                            status = VX_ERROR_NOT_SUFFICIENT;
                        }
                    }
                    else
                    {
                        CONF_PRINT("Failed to get convolution\n",__FUNCTION__,__LINE__);
                        status = VX_ERROR_NOT_SUFFICIENT;
                    }
                }
                else
                {
                    CONF_PRINT("Failed to set convolution\n",__FUNCTION__,__LINE__);
                    status = VX_ERROR_NOT_SUFFICIENT;
                }
            }
            else
            {
                CONF_PRINT("Failed to acquire convolution\n",__FUNCTION__,__LINE__);
                status = VX_ERROR_NOT_SUFFICIENT;
            }
            vxReleaseConvolution(&conv);
        }
        vxReleaseContext(&context);
    }
    return status;
}

/*!
 * \brief Tests delay object creation.
 * \ingroup group_tests
 */
vx_status vx_test_framework_delay_graph(int argc, char *argv[])
{
    vx_status status = VX_FAILURE;
    vx_uint32 w = 640, h = 480;
    vx_uint32 i;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_image images[] = {
            vxCreateImage(context, w, h, FOURCC_RGB),
            vxCreateImage(context, w, h, FOURCC_RGB),
            vxCreateImage(context, w, h, FOURCC_RGB),
        };
        vx_delay delay = vxCreateImageDelay(context, w, h, FOURCC_IYUV, dimof(images)-1);
        vx_graph graph = vxCreateGraph(context);
        if (graph)
        {
            vx_node nodes[] = {
                vxColorConvertNode(graph, images[0], vxGetImageFromDelay(delay, 0)),
                vxColorConvertNode(graph, vxGetImageFromDelay(delay, -1), images[1]),
                vxColorConvertNode(graph, vxGetImageFromDelay(delay, -1), images[2]),
            };

            /* setup the binding for auto-aging */
            status = vxAssociateDelayWithNode(delay, 0, nodes[0], 1, VX_OUTPUT);
            if (status != VX_SUCCESS)
            {
                goto exit;
            }
            status = vxAssociateDelayWithNode(delay, -1, nodes[1], 0, VX_INPUT);
            if (status != VX_SUCCESS)
            {
                goto exit;
            }
            status = vxAssociateDelayWithNode(delay, -1, nodes[2], 0, VX_INPUT);
            if (status != VX_SUCCESS)
            {
                goto exit;
            }
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
                if (status == VX_SUCCESS)
                {
                    /*! \todo need to add some logic for testing delay */
                    status = vxAgeDelay(delay);
                }
            }
exit:
            for (i = 0; i < dimof(nodes); i++)
            {
                vxReleaseNode(&nodes[i]);
            }
            vxReleaseGraph(&graph);
        }
        vxReleaseDelay(&delay);
        for (i = 0; i < dimof(images); i++)
        {
            vxReleaseImage(&images[i]);
        }
        vxReleaseContext(&context);
    }
    return status;
}

#if 0
vx_status vx_conformance_node_channels(void)
{
    vx_status status = VX_SUCCESS;
    vx_context context = vxCreateContext();
    if (context)
    {
        vx_image images[] = {
            vxCreateTestImage(context, (void *)multichannel, 1, 4, 4, 4),
            vxCreateTestImage(context, (void *)&multichannel[0], 1, 4, 4, 1),
            vxCreateTestImage(context, (void *)&multichannel[1], 1, 4, 4, 1),
            vxCreateTestImage(context, (void *)&multichannel[2], 1, 4, 4, 1),
            vxCreateImage(context, 4, 4, FOURCC_U8),
            vxCreateImage(context, 4, 4, FOURCC_U8),
            vxCreateImage(context, 4, 4, FOURCC_U8),
            vxCreateImage(context, 4, 4, FOURCC_YUV4),
        };
        vx_graph graph = vxCreateGraph(context);
        if (graph)
        {
            vx_node nodes[] = {
                vxChannelExtractNode(graph, images[0], VX_CHANNEL_0, images[5]),
                vxChannelExtractNode(graph, images[0], VX_CHANNEL_1, images[6]),
                vxChannelExtractNode(graph, images[0], VX_CHANNEL_2, images[7]),
                vxChannelExtractNode(graph, images[0], VX_CHANNEL_3, images[8]),
                vxChannelCombineNode(graph, images[5], images[6], images[7], images[8],)
            };
            vxReleaseGraph(&graph);
        }
        vxReleaseContext(&context);
    }
    return status;
}
#endif

/*! The array of supported unit tests */
vx_conformance_t conformance_tests[] = {
    {VX_FAILURE, "Framework: Context",          VX_CONFORMANCE_GROUP_FRAMEWORK_OBJECTS, VX_TYPE_CONTEXT, vx_conformance_framework_context},
    {VX_FAILURE, "Framework: Log",              VX_CONFORMANCE_GROUP_FRAMEWORK_MECHANISM, VX_FRAMEWORK_MECHANISM_LOG, vx_conformance_framework_log},
    {VX_FAILURE, "Object: Images",              VX_CONFORMANCE_GROUP_DATA_OBJECTS, VX_TYPE_IMAGE, vx_conformance_object_images},
    {VX_FAILURE, "Object: Const Images",        VX_CONFORMANCE_GROUP_DATA_OBJECTS, VX_TYPE_IMAGE, vx_conformance_object_image_const},
    {VX_FAILURE, "Object: Buffers",             VX_CONFORMANCE_GROUP_DATA_OBJECTS, VX_TYPE_BUFFER, vx_conformance_object_buffers},
    {VX_FAILURE, "Object: LUT",                 VX_CONFORMANCE_GROUP_DATA_OBJECTS, VX_TYPE_LUT, vx_conformance_object_lut},
    {VX_FAILURE, "Object: Distribution",        VX_CONFORMANCE_GROUP_DATA_OBJECTS, VX_TYPE_DISTRIBUTION, vx_conformance_object_distribution},
    {VX_FAILURE, "Object: Matrix",              VX_CONFORMANCE_GROUP_DATA_OBJECTS, VX_TYPE_MATRIX, vx_conformance_object_matrix},
    {VX_FAILURE, "Object: Coordinates",         VX_CONFORMANCE_GROUP_DATA_OBJECTS, VX_TYPE_COORDINATES, vx_conformance_object_coordinates},
    {VX_FAILURE, "Object: Convolution",         VX_CONFORMANCE_GROUP_DATA_OBJECTS, VX_TYPE_CONVOLUTION, vx_conformance_object_convolution},
    {VX_FAILURE, "Object: List",                VX_CONFORMANCE_GROUP_DATA_OBJECTS, VX_TYPE_LIST, vx_conformance_object_list},
    {VX_FAILURE, "Framework: Kernel",           VX_CONFORMANCE_GROUP_FRAMEWORK_OBJECTS, VX_TYPE_KERNEL, vx_conformance_framework_kernel},
    {VX_FAILURE, "Framework: Noload Extension", VX_CONFORMANCE_GROUP_FRAMEWORK_MECHANISM, VX_TYPE_CONTEXT, vx_conformance_framework_noload_extension},
    {VX_FAILURE, "Framework: Targets",          VX_CONFORMANCE_GROUP_FRAMEWORK_OBJECTS, VX_TYPE_TARGET, vx_conformance_framework_targets},
    {VX_FAILURE, "Framework: Node",             VX_CONFORMANCE_GROUP_FRAMEWORK_OBJECTS, VX_TYPE_NODE, vx_conformance_framework_node},
    {VX_FAILURE, "Framework: Parameters",       VX_CONFORMANCE_GROUP_FRAMEWORK_OBJECTS, VX_TYPE_PARAMETER, vx_conformance_framework_req_param},
    {VX_FAILURE, "Framework: Graph",            VX_CONFORMANCE_GROUP_FRAMEWORK_OBJECTS, VX_TYPE_GRAPH, vx_conformance_framework_graph},
    {VX_FAILURE, "Framework: Async",            VX_CONFORMANCE_GROUP_FRAMEWORK_OBJECTS, VX_TYPE_GRAPH, vx_conformance_framework_async},
    {VX_FAILURE, "Framework: Callbacks",        VX_CONFORMANCE_GROUP_FRAMEWORK_MECHANISM, VX_TYPE_CONTEXT, vx_conformance_framework_callback},
#if 0
    {VX_FAILURE, "Framework: Delay",            VX_CONFORMANCE_GROUP_FRAMEWORK_OBJECTS, VX_TYPE_CONTEXT, vx_conformance_framework_delay_graph},
    {VX_FAILURE, "Node: ChannelExtract/Combine",vVX_CONFORMANCE_GROUP_FRAMEWORK_OBJECTS, VX_TYPE_CONTEXT, x_conformance_node_channels},
#endif
};

/*! \brief The main unit test.
 * \param argc The number of arguements.
 * \param argv The array of arguments.
 * \return vx_status
 * \retval 0 Success.
 * \retval !0 Failure of some sort.
 */
int main(int argc, char *argv[])
{
    vx_uint32 i;
    vx_uint32 passed = 0;

    if (argc == 2 && ((strncmp(argv[1], "-?", 2) == 0) ||
                      (strncmp(argv[1], "--list", 6) == 0) ||
                      (strncmp(argv[1], "-l", 2) == 0) ||
                      (strncmp(argv[1], "/?", 2) == 0)))
    {
        vx_uint32 t = 0;
        for (t = 0; t < dimof(conformance_tests); t++)
        {
            CONF_PRINT("%u: %s\n", t, conformance_tests[t].name);
        }
    }
    else if (argc == 3 && strncmp(argv[1],"-t",2) == 0)
    {
        int c = atoi(argv[2]);
        if (c < (int)dimof(conformance_tests))
        {
            conformance_tests[c].status = conformance_tests[c].test();
            CONF_PRINT("[%u][%s] %s\n", c, (conformance_tests[c].status == VX_SUCCESS?"PASSED":"FAILED"), conformance_tests[c].name);
            return conformance_tests[c].status;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        for (i = 0; i < dimof(conformance_tests); i++)
        {
            conformance_tests[i].status = conformance_tests[i].test();
            switch (conformance_tests[i].status)
            {
                case VX_SUCCESS:
                    CONF_PRINT("[PASSED][%u] %s\n", i, conformance_tests[i].name);
                    passed++;
                    break;
                default:
                    CONF_PRINT("[FAILED][%u] %s, error = %d\n", i, conformance_tests[i].name, conformance_tests[i].status);
                    break;
            }
        }
        CONF_PRINT("Passed %u out of %lu\n", passed, dimof(conformance_tests));
        if (passed == dimof(conformance_tests))
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

