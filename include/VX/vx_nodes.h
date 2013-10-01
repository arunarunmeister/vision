/*******************************************************************************
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
******************************************************************************/

#ifndef _OPENVX_NODES_H_
#define _OPENVX_NODES_H_

/*!
 * \file vx_nodes.h
 * \brief The "Simple" API interface for OpenVX. These APIs are just
 * wrappers around the more verbose functions defined in <tt>\ref vx_api.h</tt>.
 * \author Erik Rainey <erik.rainey@ti.com>
 *
 * \defgroup group_kernels OpenVX Kernels
 * \brief The kernels supported in OpenVX.
 *
 * \defgroup group_kernel_colorconvert Kernel: Color Convert
 * \ingroup group_kernels
 * \brief The Color Conversion Kernel
 * \details This kernel converts an image of a designated <tt>\ref vx_fourcc_e</tt> format to another
 * <tt>\ref vx_fourcc_e</tt> format for those combinations listed in this table, where the columns are output
 * types and rows are input types; the API version first supporting the conversion is listed:
 *
 * | I/O  | RGB  | RGBX | NV12 | NV21 | UYVY | YUYV | IYUV | YUV4 |
 * |:-----|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|
 * | RGB  |      | 1.0  | 1.0  |      |      |      | 1.0  | 1.0  |
 * | RGBX | 1.0  |      | 1.0  |      |      |      | 1.0  | 1.0  |
 * | NV12 | 1.0  | 1.0  |      |      |      |      | 1.0  | 1.0  |
 * | NV21 | 1.0  | 1.0  |      |      |      |      | 1.0  | 1.0  |
 * | UYVY | 1.0  | 1.0  | 1.0  |      |      |      | 1.0  |      |
 * | YUYV | 1.0  | 1.0  | 1.0  |      |      |      | 1.0  |      |
 * | IYUV | 1.0  | 1.0  | 1.0  |      |      |      |      | 1.0  |
 * | YUV4 |      |      |      |      |      |      |      |      |
 *
 * The <tt>\ref vx_fourcc_e</tt> encoding, held in the <tt>\ref VX_IMAGE_ATTRIBUTE_FORMAT</tt> attribute,
 * describes the data layout.
 * The interpretation of the colors is determined by the <tt>\ref VX_IMAGE_ATTRIBUTE_SPACE</tt>
 * (see <tt>\ref vx_color_space_e</tt>) and <tt>\ref VX_IMAGE_ATTRIBUTE_RANGE</tt>
 * (see <tt>\ref vx_channel_range_e</tt>) attributes of the image.
 *
 * \defgroup group_kernel_channelextract Kernel: Channel Extract
 * \ingroup group_kernels
 * \brief The Channel Extraction Kernel
 * \details This kernel removes a single <tt>\ref FOURCC_U8</tt> channel (plane) from a multi-planar
 * or interleaved image format from <tt>\ref vx_fourcc_e</tt>.
 *
 * \defgroup group_kernel_channelcombine Kernel: Channel Combine
 * \ingroup group_kernels
 * \brief The Channel Combine Kernel.
 * \details This kernel takes multiple <tt>\ref FOURCC_U8</tt> planes to recombine them into
 * a multi-planar or interleaved format from <tt>\ref vx_fourcc_e</tt>.
 *
 * \defgroup group_kernel_sobel3x3 Kernel: Sobel 3x3
 * \ingroup group_kernels
 * \brief The Sobel Image Filter Kernel
 * \details This kernel produces two output planes (one can be omitted)
 * in the x and y plane. The Sobel Operators \f$ G_x, G_y \f$ are defined as:
 * \f[
 *      \mathbf{G}_x=\begin{vmatrix}
 *      -1 & 0 & +1\\
 *      -2 & 0 & +2\\
 *      -1 & 0 & +1
 *      \end{vmatrix}
 *      ,
 *      \mathbf{G}_y=\begin{vmatrix}
 *      -1 & -2 & -1 \\
 *       0 &  0 & 0  \\
 *      +1 & +2 & +1
 *      \end{vmatrix}
 * \f]
 *
 * \defgroup group_kernel_magnitude Kernel: Magnitude
 * \ingroup group_kernels
 * \brief The Gradient Magnitude Computation Kernel.
 * \details This kernel takes two gradients in <tt>\ref FOURCC_S16</tt> format and computes
 * the <tt>\ref FOURCC_U8</tt> normalized magnitude. Magnitude is computed as:
 * \f[
 * mag(x,y) = \sqrt{grad_x(x,y)^2 + grad_y(x,y)^2}
 * \f]
 *
 * \defgroup group_kernel_phase Kernel: Phase
 * \ingroup group_kernels
 * \brief The Gradient Phase Computation Kernel.
 * \details This kernel takes two gradients in <tt>\ref FOURCC_S16</tt> format and computes
 * the angles for each pixel and store this in a <tt>\ref FOURCC_U8</tt> image.
 * \f[
 * \phi = \tan^{-1}{\frac{grad_y(x,y)}{grad_x(x,y)}}
 * \f]
 * Where \f$ \phi \f$ is then translated to \f$ 0 \le \phi \le \tau \f$. Each \f$ \phi \f$ value is then mapped to the range 0 to 255 inclusive.
 *
 * \defgroup group_kernel_scale_image Kernel: Scale Image
 * \ingroup group_kernels
 * \brief The Image Resizing Kernel
 * \details This kernel resizes an image from the source to the destination
 * dimensions. The only format supported is <tt>\ref FOURCC_U8</tt>.
 * The supported interpolation types are currently:
 * \arg <tt>\ref VX_INTERPOLATION_TYPE_NEAREST_NEIGHBOR</tt>
 * \arg <tt>\ref VX_INTERPOLATION_TYPE_AREA</tt>
 * \arg <tt>\ref VX_INTERPOLATION_TYPE_BILINEAR</tt>
 *
 * The sample positions used to determine output pixel values are generated by
 * scaling the outside edges of the source image pixels to the outside edges of
 * the destination image pixels. As described in the documentation for
 * <tt>\ref vx_interpolation_type_e</tt>, samples are taken at pixel centers.
 * This means that, unless the scale is 1:1, the sample position for the top left
 * destination pixel typically does not fall exactly on the top left source pixel,
 * but will be generated by interpolation.
 * \defgroup group_kernel_lut Kernel: TableLookup
 * \ingroup group_kernels
 * \brief The Table Lookup Image Kernel
 * \details This kernel uses each pixel in an image to index into a LUT and put
 * the indexed LUT value into the output image. The format supported is <tt>\ref FOURCC_U8</tt> or <tt>\ref FOURCC_U16</tt>
 *
 * \defgroup group_kernel_histogram Kernel: Histogram
 * \ingroup group_kernels
 * \brief Generates a distribution from an image.
 * \details This kernel counts the number of occurrences of each pixel value within the
 * window size of a pre-calculated number of bins.
 *
 * \defgroup group_kernel_equalize_hist Kernel: Equalize Histogram
 * \ingroup group_kernels
 * \brief Equalizes the histogram of a grayscale image.
 * \details This kernel modifies the values of a grayscale image so that it will
 * automatically have a standardized brightness and contrast, using Histogram Equalization.
 *
 * \defgroup group_kernel_absdiff Kernel: Absolute Difference
 * \ingroup group_kernels
 * \brief Computes the absolute difference between two images.
 * \details Absolute Difference is computed by:
 * \f[
 * out(x,y) = |in_1(x,y) - in_2(x,y)|
 * \f]
 *
 * \defgroup group_kernel_meanstddev Kernel: Mean and Standard Deviation.
 * \ingroup group_kernels
 * \brief Computes the mean pixel value and the standard deviation of the pixels
 * in the input image (which has a dimension width and height).
 * \details The mean value is computed as
 * \f[
 * \mu = \frac{\left(\sum_{y=0}^h \sum_{x=0}^w src(x,y) \right)} {(width \times height)}
 * \f]
 * The standard deviation is computed as
 * \f[
 * \sigma = \sqrt{\frac{\left(\sum_{y=0}^h \sum_{x=0}^w (\mu - src(x,y))^2 \right)} {(width \times height)}}
 * \f]
 *
 * \defgroup group_kernel_threshold Kernel: Thresholding
 * \ingroup group_kernels
 * \brief Thresholds an input image and produces an output boolean image.
 * \details In \ref VX_THRESHOLD_TYPE_BINARY, the output is determined by
 * \f[
 * dst(x,y) = \begin{cases}
 * 255 & \text{if } src(x,y) > threshold \cr
 * 0 & \text{otherwise }
 * \end{cases}
 * \f]
 * In \ref VX_THRESHOLD_TYPE_RANGE, the output is determined by:
 * \f[
 * dst(x,y) = \begin{cases}
 * 0 & \text{if } src(x,y) > upper \cr
 * 0 & \text{if } src(x,y) < lower \cr
 * 255 & \text{otherwise }
 * \end{cases}
 * \f]
 *
 * \defgroup group_kernel_integral_image Kernel: Integral Image
 * \ingroup group_kernels
 * \brief Computes the integral image of the input.
 * \details Each output pixel is the sum of all pixels above and to the left of itself.
 * \f[
 * dst(x,y) = sum(x,y) = src(x,y) + sum(x-1,y) + sum(x,y-1) - sum(x-1,y-1)
 * \f]
 *
 * \defgroup group_kernel_erode_image Kernel: Erode Image
 * \ingroup group_kernels
 * \brief Erosion "shrinks" the white space in a <tt>\ref FOURCC_U8</tt> "bool" image.
 * \details This kernel uses a 3x3 box around the output pixel used to determine value.
 * \f[
 * dst(x,y) = \min_{
 * \begin{array}{c}
 *  x-1 \le x' \le x+1 \\
 *  y-1 \le y' \le y+1
 * \end{array}
 * } src(x',y')
 * \f]
 *
 * \defgroup group_kernel_dilate_image Kernel: Dilate Image
 * \ingroup group_kernels
 * \brief Dilation "grows" the white space in a <tt>\ref FOURCC_U8</tt> "bool" image.
 * \details This kernel uses a 3x3 box around the output pixel used to determine value.
 * \f[
 * dst(x,y) = \max_{
 * \begin{array}{c}
 *  x-1 \le x' \le x+1 \\
 *  y-1 \le y' \le y+1
 * \end{array}
 * } src(x',y')
 * \f]
 *
 * \defgroup group_kernel_median_image Kernel: Median Filter
 * \ingroup group_kernels
 * \brief Compute a median pixel value over a window of the input image.
 * \details The median is the middle value over an odd numbered sorted range of values.
 *
 * \defgroup group_kernel_box_image Kernel: Box Filter
 * \ingroup group_kernels
 * \brief Compute a Box filter over a window of the input image.
 * \details This filter uses the follow convolution matrix:
 * \f[
 * \mathbf{K}_{box} = \begin{vmatrix}
 *   1 & 1 & 1\\
 *   1 & 1 & 1\\
 *   1 & 1 & 1
 * \end{vmatrix} \times \frac{1}{9}
 * \f]
 *
 * \defgroup group_kernel_gaussian_image Kernel: Gaussian Filter
 * \ingroup group_kernels
 * \brief Computes a Gaussian filter over a window of the input image.
 * \details This filter uses the follow convolution matrix:
 * \f[
 * \mathbf{K}_{gaussian} = \begin{vmatrix}
 *   1 & 2 & 1\\
 *   2 & 4 & 2\\
 *   1 & 2 & 1
 * \end{vmatrix} \times \frac{1}{16}
 * \f]
 *
 * \defgroup group_kernel_custom_convolution Kernel: Custom Convolution
 * \ingroup group_kernels
 * \brief Convolves the input with the user supplied convolution matrix.
 * \details The user can supply a vx_int16 typed convolution matrix \f$ C_{m,n} \f$. Outputs will
 * be in the <tt>\ref FOURCC_S16</tt> format unless a <tt>\ref FOURCC_U8</tt> image is explicitly provided.
 * If values would have been out of range of U8, the values are clamped to 0 or 255.
 * \f{eqnarray}{
 * k_0 &=& \frac{m}{2} + 1 \\
 * l_0 &=& \frac{n}{2} + 1 \\
 * sum &=& \sum_{k=0,l=0}^{k=m,l=n} input(x+k-k_0, y+l-l_0) * C_{k,l}
 * \f}
 * This translates into the "C" declaration:
 * \code
 * // A horizontal Scharr gradient operator
 * vx_int16 gx[3][3] = {
 *     {  3, 0, -3},
 *     { 10, 0,-10},
 *     {  3, 0, -3},
 * };
 * vx_convolution scharr_x = vxCreateConvolution(context, 3, 3);
 * vxSetConvolutionCoefficients(scharr_x, gx);
 * \endcode
 *
 * For <tt>\ref FOURCC_U8</tt> output, an additional step is taken:
 * \f[
 * output(x,y) = \begin{cases}  \cr
 * 0 & \text{if } sum < 0 \cr
 * 255 & \text{if } sum / scale > 255 \cr
 * sum / scale & \text{otherwise}
 * \end{cases}
 * \f]
 * For <tt>\ref FOURCC_S16</tt> output, the summation is simply set to the output
 * \f[
 * output(x,y) = sum / scale
 * \f]
 *
 * \defgroup group_kernel_pyramid Kernel: Image Pyramid
 * \ingroup group_kernels
 * \brief Computes a Gaussian and/or Laplacian filter over each step or level of
 * the pyramid then scales it.
 * \details This uses the \ref group_kernel_gaussian_image on the gaussian pyramid.
 * Level 0 is always equivalent to the input image.
 * \if OPENVX_STRICT_1_0
 * The pyramids must be configured with one of the following level scaling:
 * \arg <tt>\ref VX_SCALE_PYRAMID_DOUBLE</tt>
 * \arg <tt>\ref VX_SCALE_PYRAMID_HALF</tt>
 * \arg <tt>\ref VX_SCALE_PYRAMID_ORB</tt>
 * \endif
 *
 * \defgroup group_kernel_accumulate Kernel: Accumulate
 * \ingroup group_kernels
 * \brief Accumulates an input image into output image.
 * \details Accumulation is computed by:
 * \f[
 * accum(x,y) = accum(x,y) + input(x,y)
 * \f]
 *
 * \defgroup group_kernel_accumulate_weighted Kernel: Accumulate Weighted
 * \ingroup group_kernels
 * \brief Accumulates a weighted value from an input image to an output image.
 * \details Weighted accumulation is computed by:
 * \f[
 * accum(x,y) = (1 - \alpha)*accum(x,y) + \alpha*input(x,y)
 * \f]
 * Where \f$ 0 \le \alpha \le 1 \f$
 *
 * \defgroup group_kernel_accumulate_square Kernel: Accumulate Squared
 * \ingroup group_kernels
 * \brief Accumulates a squared value from an input image to an output image.
 * \details Accumulate squares is computed by:
 * \f[
 * accum(x,y) = accum(x,y) + scale*input(x,y)^2
 * \f]
 * Where \f$ 0 \le scale \f$
 *
 * \defgroup group_kernel_minmaxloc Kernel: Min, Max Location
 * \ingroup group_kernels
 * \brief Finds the minimum and maximum values in an image and a location for each.
 * \details The locations are not guaranteed to be found in any particular scanning
 * method.
 * \f[
 * minVal = \min_{
 * \begin{array}{c}
 * 0 \le x' \le width \\
 * 0 \le y' \le height
 * \end{array}
 * } src(x',y')
 * \f]
 *
 * \f[
 * maxVal = \max_{
 * \begin{array}{c}
 * 0 \le x' \le width \\
 * 0 \le y' \le height
 * \end{array}
 * } src(x',y')
 * \f]
 *
 * \defgroup group_kernel_and Kernel: Bitwise And
 * \ingroup group_kernels
 * \brief Performs bitwise "and" between two <tt>\ref FOURCC_U8</tt> images.
 * \details Bitwise "and" is computed by, for each bit in each pixel
 * in the input images:
 * \f[
 * out(x,y) = in_1(x,y) \wedge in_2(x,y)
 * \f]
 * Or expressed as "C" code:
 * \code
 * out(x,y) = in_1(x,y) & in_2(x,y)
 * \endcode
 *
 * \defgroup group_kernel_or Kernel: Bitwise Inclusive Or
 * \ingroup group_kernels
 * \brief Performs bitwise "inclusive or" between two <tt>\ref FOURCC_U8</tt> images.
 * \details Bitwise "inclusive or" is computed by, for each bit in each
 * pixel in the input images:
 * \f[
 * out(x,y) = in_1(x,y) \vee in_2(x,y)
 * \f]
 * Or expressed as "C" code:
 * \code
 * out(x,y) = in_1(x,y) | in_2(x,y)
 * \endcode
 *
 * \defgroup group_kernel_xor Kernel: Bitwise Exclusive Or
 * \ingroup group_kernels
 * \brief Performs bitwise "exclusive or" between two <tt>\ref FOURCC_U8</tt> images.
 * \details Bitwise "exclusive or" is computed by, for each bit in each
 * pixel in the input images:
 * \f[
 * out(x,y) = in_1(x,y) \oplus in_2(x,y)
 * \f]
 * Or expressed as "C" code:
 * \code
 * out(x,y) = in_1(x,y) ^ in_2(x,y)
 * \endcode
 *
 * \defgroup group_kernel_not Kernel: Bitwise Not
 * \ingroup group_kernels
 * \brief Performs bitwise "not" on a <tt>\ref FOURCC_U8</tt> input image.
 * \details Bitwise "not" is computed by, for each bit in each
 * pixel in the input image:
 * \f[
 * out(x,y) = \sim in(x,y)
 * \f]
 *
 * \defgroup group_kernel_mult Kernel: Pixel-wise Multiplication
 * \ingroup group_kernels
 * \brief Performs element-wise multiplication between two images and a scalar value.
 * \details Pixel-wise multiplication is performed between the pixel values in two
 * <tt>\ref FOURCC_U8</tt> or <tt>\ref FOURCC_S16</tt> images and a scalar floating-point number
 * ``scale''. The output image can be <tt>\ref FOURCC_U8</tt> only if both source images are
 * <tt>\ref FOURCC_U8</tt> and the output image is explicitly set to <tt>\ref FOURCC_U8</tt>. It is otherwise
 * <tt>\ref FOURCC_S16</tt>. If one of the input images is of type <tt>\ref FOURCC_S16</tt>, all values are
 * converted to <tt>\ref FOURCC_S16</tt>. The overflow handling is controlled by an overflow-policy
 * parameter. For each pixel value in the two input images:
 * \f[
 * out(x,y) = in_1(x,y) * in_2(x,y) * scale
 * \f]
 *
 * \defgroup group_kernel_add Kernel: Arithmetic Addition
 * \ingroup group_kernels
 * \brief Performs addition between two images.
 * \details Arithmetic addition is performed between the pixel values in two
 * <tt>\ref FOURCC_U8</tt> or <tt>\ref FOURCC_S16</tt> images. The output image can be <tt>\ref FOURCC_U8</tt> only
 * if both source images are <tt>\ref FOURCC_U8</tt> and the output image is explicitly set to
 * <tt>\ref FOURCC_U8</tt>. It is otherwise <tt>\ref FOURCC_S16</tt>. If one of the input images is of type
 * <tt>\ref FOURCC_S16</tt>, all values are converted to <tt>\ref FOURCC_S16</tt>. The overflow handling is
 * controlled by an overflow-policy parameter. For each pixel value in the two input
 * images:
 * \f[
 * out(x,y) = in_1(x,y) + in_2(x,y)
 * \f]
 *
 * \defgroup group_kernel_sub Kernel: Arithmetic Subtraction
 * \ingroup group_kernels
 * \brief Performs subtraction between two images.
 * \details Arithmetic subtraction is performed between the pixel values in two
 * <tt>\ref FOURCC_U8</tt> or <tt>\ref FOURCC_S16</tt> images. The output image can be <tt>\ref FOURCC_U8</tt> only
 * if both source images are <tt>\ref FOURCC_U8</tt> and the output image is explicitly set to
 * <tt>\ref FOURCC_U8</tt>. It is otherwise <tt>\ref FOURCC_S16</tt>. If one of the input images is of type
 * <tt>\ref FOURCC_S16</tt>, all values are converted to <tt>\ref FOURCC_S16</tt>. The overflow handling is
 * controlled by an overflow-policy  parameter. For each pixel value in the two input
 * images:
 * \f[
 * out(x,y) = in_1(x,y) - in_2(x,y)
 * \f]
 *
 * \defgroup group_kernel_convertdepth Kernel: Convert Bit depth
 * \ingroup group_kernels
 * \brief Converts image bit depth.
 * \details This kernel converts an image from some source bit-depth to
 * another bit-depth as described by the table below. The columns are
 * output types and rows are input types and the conversion has the API version
 * on which it is supported listed (An 'X' implies an invalid operation).
 *
 * | I/O |  U8 | U16 | S16 | U32 | S32 |
 * |:----|:---:|:---:|:---:|:---:|:---:|
 * | U8  |  X  |     | 1.0 |     |     |
 * | U16 |     |  X  |  X  |     |     |
 * | S16 | 1.0 |  X  |  X  |     |     |
 * | U32 |     |     |     |  X  |  X  |
 * | S32 |     |     |     |  X  |  X  |
 *
 * Down-conversions with <tt>\ref VX_CONVERT_POLICY_TRUNCATE</tt> follow this equation:
 * \code
 * output(x,y) = (OUTTYPE)input(x,y) >> shift
 * \endcode
 * Down-conversions with <tt>\ref VX_CONVERT_POLICY_SATURATE</tt> follow this equation:
 * \code
 * INTYPE value = input(x,y) >> shift;
 * value = (value < OUTTYPE_MIN ? OUTTYPE_MIN : value);
 * value = (value > OUTTYPE_MAX ? OUTTYPE_MAX : value);
 * output(x,y) = (OUTTYPE)value;
 * \endcode
 * Up-conversions ignore the policy and perform this operation:
 * \code
 * output(x,y) = (OUTTYPE)input(x,y) << shift;
 * \endcode
 *
 * \defgroup group_kernel_canny Kernel: Canny Edge Detector
 * \ingroup group_kernels
 * \brief Canny Edge detection kernel
 * \details This function implements an edge detection algorithm similar to that described in
 * \cite Canny1986. The main components of the algorithm are
 * \arg Gradient magnitude and orientation computation using a noise resistant operator (Sobel)
 * \arg Non-maximum suppression of the gradient magnitude, using the gradient orientation information
 * \arg Tracing edges in the modified gradient image using hysteresis thresholding to produce a binary result
 *
 * The details of each of these steps are described below.
 *
 * \arg <b>Gradient Computation:</b>
 * Conceptually, the input image is convolved with vertical and
 * horizontal Sobel kernels of the size indicated by the gradient_size
 * parameter.  The two resulting directional gradient images (\f$ dx \f$ and
 * \f$ dy \f$) are then used to compute a gradient magnitude image and a
 * gradient orientation image.  The norm used to compute the gradient
 * magnitude is indicated by the norm_type parameter, so the magnitude
 * may be \f$ |dx| + |dy| \f$ for <tt>\ref VX_NORM_L1</tt> or \f$ \sqrt{dx^{2} + dy^{2}} \f$ for
 * <tt>\ref VX_NORM_L2</tt>.  The gradient orientation image is quantized into 4
 * values: 0, 45, 90, and 135 degrees.
 * \arg <b>Non-Maximum Suppression:</b>
 * This is then applied such that a pixel is
 * retained as a potential edge pixel if an only if its magnitude is
 * greater than the pixels in the direction perpendicular to its edge
 * orientation.  For example, if the pixel's orientation is 0 degrees,
 * it is only retained if its gradient magnitude is larger than that
 * of the pixels at 90 and 270 degrees to it.  If a pixel is
 * suppressed via this condition, it must not appear as an edge pixel
 * in the final output, i.e., its value must be 0 in the final output.
 * \arg <b>Edge Tracing:</b>
 * The final edge pixels in the output are identified via a double
 * thresholded hysteresis procedure.  All retained pixels with
 * magnitude above the "high: threshold are marked as known edge
 * pixels (valued 255) in the final output image.  All pixels with
 * magnitudes below the "low" threshold must not be marked as edge
 * pixels in the final output.  For the pixels in between the
 * thresholds, edges are traced and marked as edges (255) in the
 * output.  This can be done by starting at the known edge pixels and
 * moving along the edge using the orientation information calculated
 * above until the gradient magnitude falls below the low threshold.
 * \arg <b>Caveats:</b>
 * The intermediate results described above are conceptual only, so
 * for example the implementation may not actually construct the
 * gradient images and non-maximum-suppressed images.  Only the final
 * binary (0 or 255 valued) output image must be computed so that it
 * matches the result of a final image constructed as described above.
 *
 * \defgroup group_kernel_warp_affine Kernel: Warp Affine
 * \ingroup group_kernels
 * \brief Performs an affine transform on an image.
 * \details This kernel performs an affine transform with a 2x3 Matrix \f$ M \f$ with this method of pixel coordinate translation:
 * \f{eqnarray}{
 * x0 &=& M_{1,1}*x + M_{1,2}*y + M_{1,3} \\
 * y0 &=& M_{2,1}*x + M_{2,2}*y + M_{2,3} \\
 * output(x,y) &=& input(x0,y0)
 * \f}
 * This translates into the "C" declaration:
 * \code
 * // x0 = a * x + b * y + c;
 * // y0 = d * x + e * y + f;
 * vx_float32 mat[3][2] = {
 *     {a, d}, // 'x' coefficients
 *     {b, e}, // 'y' coefficients
 *     {c, f}, // 'offsets'
 * };
 * vx_matrix matrix = vxCreateMatrix(context, 2, 3);
 * vxAccessMatrix(matrix, NULL);
 * vxCommitMatrix(matrix, mat);
 * \endcode
 *
 * \defgroup group_kernel_warp_perspective Kernel: Warp Perspective
 * \ingroup group_kernels
 * \brief Performs a perspective transform on an image.
 * \details This kernel performs an perspective transform with a 3x3 Matrix \f$ M \f$ with this method of pixel coordinate translation:
 * \f{eqnarray}{
 * x0 &=& M_{1,1}*x + M_{1,2}*y + M_{1,3} \\
 * y0 &=& M_{2,1}*x + M_{2,2}*y + M_{2,3} \\
 * z0 &=& M_{3,1}*x + M_{3,2}*y + M_{3,3} \\
 * output(x,y) &=& input(\frac{x0}{z0},\frac{y0}{z0})
 * \f}
 * This translates into the "C" declaration:
 * \code
 * // x0 = a * x + b * y + c;
 * // y0 = d * x + e * y + f;
 * // z0 = g * x + h * y + i;
 * vx_float32 mat[3][3] = {
 *     {a, d, g}, // 'x' coefficients
 *     {b, e, h}, // 'y' coefficients
 *     {c, f, i}, // 'offsets'
 * };
 * vx_matrix matrix = vxCreateMatrix(context, 3, 3);
 * vxAccessMatrix(matrix, NULL);
 * vxCommitMatrix(matrix, mat);
 * \endcode
 *
 * \defgroup group_kernel_harris Kernel: Harris Corners
 * \ingroup group_kernels
 * \brief Computes the Harris Corners of an image.
 * \details The Harris Corners are computed with several parameters
 * \f{eqnarray}{
 * I &=& \text{input image} \\
 * T_c &=& \text{corner strength threshold} \\
 * r &=& \text{euclidean radius} \\
 * k &=& \text{sensitivity threshold} \\
 * w &=& \text{window size} \\
 * b &=& \text{block size} \\
 * \f}
 * The computation to find the corner values or scores can be summarized as:
 * \f{eqnarray}{
 * G_x &=& Sobel_x(w, I) \\
 * G_y &=& Sobel_y(w, I) \\
 * A &=& window_{G_{x,y}}(x-b/2,y-b/2,x+b/2,y+b/2) \\
 * trace(A) &=& \sum^{A}{G_x^{2}} + \sum^{A}{G_y^{2}} \\
 * det(A) &=& \sum^{A}{G_x^{2}} * \sum^{A}{G_y^{2}} - \sum^{A}{(G_x * G_y)^{2}} \\
 * M_c(x,y) &=& det(A) - k*trace(A)^{2} \\
 * V_c(x,y) &=& \begin{cases}
 *                 M_c(x,y) \text{ if } M_c(x,y) > T_c \\
 *                 0 \text{ otherwise} \\
 *              \end{cases}
 * \f}
 * \f[
 * \text{where } V_c \text{ is the thresholded corner value}.
 * \f]
 * \f$ V_c \f$ is then non-maximally suppressed within the Euclidean distance \f$ r \f$
 * and returned as a \ref vx_list of \ref vx_keypoint objects.
 *
 * \defgroup group_kernel_fast Kernel: Fast Corners
 * \ingroup group_kernels
 * \brief Computes the corners in an image using FAST algorithm.
 * \details
 * The FAST (features from accelerated segment test) algorithm based on the
 * FAST9 algorithm described in
 * \cite rosten_2006_machine and with some updates from \cite rosten_2008_faster .
 * It extracts corners by evaluating pixels on the Bresenham circle around a candidate point.
 * If \f$ N \f$ contiguous pixels are brighter than the candidate point by at least a threshold value \f$ t \f$
 * or darker by at least \f$ t \f$, then the candidate point is considered to be a corner.
 * For each detected corner, its strength is computed.
 * Optionally, a non-maxima suppression step is applied on all detected corners
 * to remove multiple or spurious responses.
 *
 * \section sec_fast_detector Segment Test Detector
 *
 * The FAST corner detector uses the pixels on a Bresenham circle of radius 3 (16 pixels) to classify
 * whether a candidate point \f$ p \f$ is actually a corner.
 *
 * Given the following definitions:
 *
 * \f{eqnarray}{
 *    I   &=& \text{input image}                                                                   \\
 *    p   &=& \text{candidate point position for a corner}                                         \\
 *    I_p &=& \text{image intensity of the candidate point in image \f$ I \f$ }                    \\
 *    x   &=& \text{pixel on the Bresenham circle around the candidate point \f$ p \f$ }           \\
 *    I_x &=& \text{image intensity of the candidate point}                                        \\
 *    t   &=& \text{intensity difference threshold for a corner }                                  \\
 *    N   &=& \text{minimum number of contiguous pixel to detect a corner }                        \\
 *    S   &=& \text{set of contiguous pixel on the Bresenham circle around the candidate point  }  \\
 *    C_p &=& \text{corner response at corner location \f$ p \f$ }
 * \f}
 *
 * \n The two conditions for FAST corner detection can be expressed as:
 * \arg  C1: A set of \f$ N \f$ contiguous pixels \f$ S \f$, \f$ \forall x \f$ in \f$ S \f$, \f$ I_{x} > I_{p} + t \f$
 * \arg  C2: A set of \f$ N \f$ contiguous pixels \f$ S \f$, \f$ \forall x \f$ in \f$ S \f$, \f$ I_{x} < I_{p} - t \f$
 *
 * \n
 * So when either of these two conditions is met, the candidate \f$ p \f$ is classified as a corner.
 *
 * In this version of the FAST algorithm, the minimum number of contiguous pixels \f$ N \f$ is 9 (FAST9).
 *
 * The value of the intensity difference threshold <tt>strength_thresh</tt>
 * of type <tt>\ref VX_TYPE_FLOAT32</tt>  must be within:
 * \f[
 * {UINT8_{MIN}} <  t < {UINT8_{MAX}}
 * \f]
 * These limits are established due to the input data type <tt>\ref FOURCC_U8</tt>.
 *
 * <H3> Corner Strength Computation </H3>
 *
 * Once a corner has been detected, its strength (response, saliency or score) is computed.
 * The corner response \f$ C_p \f$ function is defined as the largest threshold \f$ t \f$
 * for which the pixel \f$ p \f$ remains a corner.
 *
 * <H3> Non-maximum suppression </H3>
 *
 * If the <tt>nonmax_suppression</tt> flag is true,
 * a non-maxima suppression step is applied on the detected corners.
 * This step is only keeping the corner which has a response greater than
 * the ones of its neighboring corners within the Bresenham circle of radius 3.
 *
 * \see http://www.edwardrosten.com/work/fast.html
 * \see http://en.wikipedia.org/wiki/Features_from_accelerated_segment_test
 *
 * \defgroup group_kernel_opticalflowpyrlk Kernel: Optical Flow Pyramid (LK)
 * \ingroup group_kernels
 * \brief Computes the optical flow using the Lucas-Kanade method between two pyramid images.
 * \details The function is an implementation of the algorithm described in \cite Bouguet2000 .
 * The function inputs are two <tt>\ref vx_pyramid</tt> objects, old and new,
 * along with a <tt>\ref vx_list</tt> of <tt>\ref vx_keypoint</tt> objects
 * to track from the old <tt>\ref vx_pyramid</tt>. The function outputs a <tt>\ref vx_list</tt>
 * of <tt>\ref vx_keypoint</tt> objects that were tracked from the old <tt>\ref vx_pyramid</tt> to
 * the new <tt>\ref vx_pyramid</tt>. Each element in the <tt>\ref vx_list</tt> of <tt>\ref vx_keypoint</tt> objects
 * in the new list may be valid or not. The implementation shall return the same
 * number of <tt>\ref vx_keypoint</tt> objects in the new <tt>\ref vx_list</tt> that
 * were in the older <tt>\ref vx_list</tt>.
 *
 * In more detail:
 * The Lucas-Kanade method finds the affine motion vector \f$ V \f$ for each
 * point in the old image tracking points list, using the following equation:
 * \f[
 * \begin{bmatrix}
 * V_x \\
 * V_y
 * \end{bmatrix} =
 * \begin{bmatrix}
 * \sum_{i}{I_x}^2 & \sum_{i}{I_x*I_y} \\
 * \sum_{i}{I_x*I_y} & \sum_{i}{I_y}^2
 * \end{bmatrix}^{-1}
 * \begin{bmatrix}
 * -\sum_{i}{I_x*I_t} \\
 * -\sum_{i}{I_y*I_t}
 * \end{bmatrix}
 * \f]
 * Where \f$ I_x \f$ and \f$ I_y \f$ are obtained using the Scharr gradients on the input image:
 * \f[
 * G_x = \begin{bmatrix}
 *  +3 & 0 & -3 \\
 * +10 & 0 & -10 \\
 *  +3 & 0 & -3
 * \end{bmatrix}
 * \f]
 *
 * \f[
 * G_y = \begin{bmatrix}
 * +3 & +10 & +3 \\
 *  0 &   0 &  0 \\
 * -3 & -10 & -3
 * \end{bmatrix}
 * \f]
 *
 * \f$ I_t \f$ is obtained by a simple difference between the same pixel in
 * both images. \f$ i \f$ is defined as the adjacent pixels to the point
 * \f$ p(x,y) \f$ under consideration. With a given window size of \f$ M \f$,
 * \f$ i \f$ is \f$ M^2 \f$ points.
 * The pixel \f$ p(x,y) \f$ is centered in the window.
 * In practice, to get an accurate solution, it is necessary to iterate
 * multiple times on this scheme (in a Newton-Raphson fashion). until the residual
 * of the affine motion vector is smaller than a threshold. And/or maximum number of iteration achieved.
 * Each iteration, the estimation of the previous iteration is used. By changing \f$ I_t \f$ to be the difference
 * between the old image and the pixel with the estimated coordinates in the new image.
 * Each iteration the function check if the pixel to track was lost. The criteria for lost tracking is
 * that the matrix above is invertible. (The determinant of the matrix is less then a threshold : \f$ 10^{-7} \f$ ).
 * Or the minimum eigenvalue of the matrix is smaller then a threshold ( \f$ 10^{-4} \f$ ).
 * Also lost tracking happen when the point tracked coordinate is outside the image coordinates.
 * When <tt>\ref vx_true_e</tt> is given as the input to <tt>use_initial_estimates</tt>,
 * the algorithm starts by calculating \f$ I_t \f$ as the difference between the
 * old image and the pixel with the initial estimated coordinates in the new image.
 * The input <tt>\ref vx_list</tt> of <tt>\ref vx_keypoint</tt> objects with
 * <tt>tracking_status</tt> set to zero (lost) are copied to the new <tt>\ref vx_list</tt>.
 *
 * Users are responsible for editing the output <tt>\ref vx_list</tt> of
 * <tt>\ref vx_keypoint</tt> objects list before applying it as the
 * input <tt>\ref vx_list</tt> of <tt>\ref vx_keypoint</tt> objects for the next frame.
 * For example, <tt>\ref vx_keypoint</tt> objects with <tt>tracking_status</tt> set to zero
 * may be removed by a user for efficiency.
 *
 * This function changes just the <tt>x</tt>, <tt>y</tt> and <tt>tracking_status</tt>
 * members of the <tt>\ref vx_keypoint_t</tt> structure and behaves as if it copied the rest from the
 * old tracking <tt>\ref vx_keypoint</tt> to new image <tt>\ref vx_keypoint</tt>.
 *
 * \defgroup group_kernel_remap Kernel: Remap
 * \ingroup group_kernels
 * \brief Maps output pixels in an image from input pixels in an image.
 * \details Remap takes a remap table object \ref vx_remap to map a set of output pixels back
 * to source input pixels. A remap is typically defined as:
 * \f[
 * output(x1,y1) = input(map_x(x0,y0),map_y(x0,y0))
 * \f]
 * However, the mapping functions are contained in the \ref vx_remap object.
 *
 * \defgroup group_borders Node: Border Modes
 * \ingroup group_node
 * \brief The border mode behaviors.
 * \details Border Mode behavior is set as an attribute of the node, not as a
 * direct parameter to the kernel. This allows users to "set-and-forget" the modes
 * of any particular node which supports border modes. Most Nodes do not
 * support any explicit border modes beyond \ref VX_BORDER_MODE_UNDEFINED.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief [Graph] Creates a color conversion node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input image to convert from.
 * \param [out] output The output image to convert into.
 * \see VX_KERNEL_COLOR_CONVERT
 * \ingroup group_kernel_colorconvert
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxColorConvertNode(vx_graph graph, vx_image input, vx_image output);

/*! \brief [Graph] Creates a channel extract node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input image. Must be one of the defined \ref vx_fourcc_e multi-planar formats.
 * \param [in] channel The <tt>\ref vx_channel_e</tt> channel to extract.
 * \param [out] output The output image. Must be <tt>\ref FOURCC_U8</tt>.
 * <tt>\see VX_KERNEL_CHANNEL_EXTRACT</tt>
 * \ingroup group_kernel_channelextract
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxChannelExtractNode(vx_graph graph,
                             vx_image input,
                             vx_enum channel,
                             vx_image output);

/*! \brief [Graph] Creates a channel combine node.
 * \param [in] graph The graph reference.
 * \param [in] plane0 The plane which will form channel 0. Must be <tt>\ref FOURCC_U8</tt>.
 * \param [in] plane1 The plane which will form channel 1. Must be <tt>\ref FOURCC_U8</tt>.
 * \param [in] plane2 [optional] The plane which will form channel 2. Must be <tt>\ref FOURCC_U8</tt>.
 * \param [in] plane3 [optional] The plane which will form channel 3. Must be <tt>\ref FOURCC_U8</tt>.
 * \param [out] output The output image.
 * \see VX_KERNEL_CHANNEL_COMBINE
 * \ingroup group_kernel_channelcombine
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxChannelCombineNode(vx_graph graph,
                             vx_image plane0,
                             vx_image plane1,
                             vx_image plane2,
                             vx_image plane3,
                             vx_image output);

/*! \brief [Graph] Create a Magnitude node.
 * \param [in] graph The reference to the graph.
 * \param [in] grad_x The input x image. This should be in <tt>\ref FOURCC_S16</tt> format.
 * \param [in] grad_y The input y image. This should be in <tt>\ref FOURCC_S16</tt> format.
 * \param [out] orientation The phase image. This will be in <tt>\ref FOURCC_U8</tt> format.
 * \see VX_KERNEL_PHASE
 * \ingroup group_kernel_phase
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxPhaseNode(vx_graph graph, vx_image grad_x, vx_image grad_y, vx_image orientation);

/*! \brief [Graph] Create a Sobel3x3 node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input image in <tt>\ref FOURCC_U8</tt> format.
 * \param [out] output_x [optional] The output gradient in the x direction in <tt>\ref FOURCC_S16</tt>.
 * \param [out] output_y [optional] The output gradient in the y direction in <tt>\ref FOURCC_S16</tt>.
 * \see VX_KERNEL_SOBEL_3x3
 * \ingroup group_kernel_sobel3x3
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxSobel3x3Node(vx_graph graph, vx_image input, vx_image output_x, vx_image output_y);


/*! \brief [Graph] Create a Magnitude node.
 * \param [in] graph The reference to the graph.
 * \param [in] grad_x The input x image. This should be in <tt>\ref FOURCC_S16</tt> format.
 * \param [in] grad_y The input y image. This should be in <tt>\ref FOURCC_S16</tt> format.
 * \param [out] mag The magnitude image. This will be in <tt>\ref FOURCC_U8</tt> format.
 * \see VX_KERNEL_MAGNITUDE
 * \ingroup group_kernel_magnitude
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxMagnitudeNode(vx_graph graph, vx_image grad_x, vx_image grad_y, vx_image mag);

/*! \brief [Graph] Create a Scale Image Node.
 * \param [in] graph The reference to the graph.
 * \param [in] src The source image.
 * \param [out] dst The destination image.
 * \param [in] type The interpolation type to use. \see vx_interpolation_type_e.
 * \note The destination image must have a defined size and format.
 * \ingroup group_kernel_scale_image
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxScaleImageNode(vx_graph graph, vx_image src, vx_image dst, vx_enum type);

/*! \brief [Graph] Creates a Table Lookup node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input image in <tt>\ref FOURCC_U8</tt>.
 * \param [in] lut The LUT which is of type VX_TYPE_UINT8.
 * \param [out] output The output image of type <tt>\ref FOURCC_U8</tt>.
 * \ingroup group_kernel_lut
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxTableLookupNode(vx_graph graph, vx_image input, vx_lut lut, vx_image output);

/*! \brief [Graph] Creates a Histogram node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input image in <tt>\ref FOURCC_U8</tt>.
 * \param [out] distribution The output distribution.
 * \ingroup group_kernel_histogram
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxHistogramNode(vx_graph graph, vx_image input, vx_distribution distribution);

/*! \brief [Graph] Creates a Histogram Equalization node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The grayscale input image in <tt>\ref FOURCC_U8</tt>.
 * \param [out] output The grayscale output image of type <tt>\ref FOURCC_U8</tt> with equalized brightness and contrast.
 * \ingroup group_kernel_equalize_hist
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxEqualizeHistNode(vx_graph graph, vx_image input, vx_image output);

/*! \brief [Graph] Creates an AbsDiff node.
 * \param [in] graph The reference to the graph.
 * \param [in] in1 An input image
 * \param [in] in2 An input image
 * \param [out] out The output image.
 * \ingroup group_kernel_absdiff
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxAbsDiffNode(vx_graph graph, vx_image in1, vx_image in2, vx_image out);

/*! \brief [Graph] Creates a mean value and standard deviation node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input image.
 * \param [out] mean The <tt>\ref VX_TYPE_FLOAT32</tt> average pixel value.
 * \param [out] stddev The <tt>\ref VX_TYPE_FLOAT32</tt> standard deviation of the pixel values.
 * \ingroup group_kernel_meanstddev
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxMeanStdDevNode(vx_graph graph, vx_image input, vx_scalar mean, vx_scalar stddev);

/*! \brief [Graph] Creates a Threshold node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input image. <tt>\ref FOURCC_U8</tt> is supported.
 * \param [in] thresh The thresholding object which defines the parameters of
 * the operation.
 * \param [out] output The output boolean image. Values are either 0 or 255.
 * \ingroup group_kernel_threshold
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxThresholdNode(vx_graph graph, vx_image input, vx_threshold thresh, vx_image output);

/*! \brief [Graph] Creates an Integral Image Node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input image in <tt>\ref FOURCC_U8</tt> format.
 * \param [out] output The output image in <tt>\ref FOURCC_U32</tt> format.
 * \ingroup group_kernel_integral_image
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxIntegralImageNode(vx_graph graph, vx_image input, vx_image output);

/*! \brief [Graph] Creates an Erosion Image Node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input image in <tt>\ref FOURCC_U8</tt> format.
 * \param [out] output The output image in <tt>\ref FOURCC_U8</tt> format.
 * \ingroup group_kernel_erode_image
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxErode3x3Node(vx_graph graph, vx_image input, vx_image output);

/*! \brief [Graph] Creates an Dilation Image Node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input image in <tt>\ref FOURCC_U8</tt> format.
 * \param [out] output The output image in <tt>\ref FOURCC_U8</tt> format.
 * \ingroup group_kernel_dilate_image
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxDilate3x3Node(vx_graph graph, vx_image input, vx_image output);

/*! \brief [Graph] Creates a Median Image Node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input image in <tt>\ref FOURCC_U8</tt> format.
 * \param [out] output The output image in <tt>\ref FOURCC_U8</tt> format.
 * \ingroup group_kernel_median_image
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxMedian3x3Node(vx_graph graph, vx_image input, vx_image output);

/*! \brief [Graph] Creates a Box Filter Node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input image in <tt>\ref FOURCC_U8</tt> format.
 * \param [out] output The output image in <tt>\ref FOURCC_U8</tt> format.
 * \ingroup group_kernel_box_image
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxBox3x3Node(vx_graph graph, vx_image input, vx_image output);

/*! \brief [Graph] Creates a Gaussian Filter Node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input image in <tt>\ref FOURCC_U8</tt> format.
 * \param [out] output The output image in <tt>\ref FOURCC_U8</tt> format.
 * \ingroup group_kernel_gaussian_image
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxGaussian3x3Node(vx_graph graph, vx_image input, vx_image output);

/*! \brief [Graph] Creates a custom convolution node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input image in <tt>\ref FOURCC_U8</tt> format.
 * \param [in] conv The vx_int16 convolution matrix.
 * \param [out] output The output image in <tt>\ref FOURCC_S16</tt> format.
 * \ingroup group_kernel_custom_convolution
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxConvolveNode(vx_graph graph, vx_image input, vx_convolution conv, vx_image output);

/*! \brief [Graph] Creates a pyramid node for both a gaussian pyramid and/or a laplacian pyramid.
 * \details This function creates the gaussian and laplacian pyramids from the input image
 * using the particular 5x5 Gaussian Kernel:
 * \f[
 *      \mathbf{G}=1/256 * \begin{vmatrix}
 *      1 & 4  & 6  & 4  & 1 \\
 *      4 & 16 & 24 & 16 & 4 \\
 *      6 & 24 & 36 & 24 & 6 \\
 *      4 & 16 & 24 & 16 & 4 \\
 *      1 & 4  & 6  & 4  & 1 \\
 *      \end{vmatrix}
 * \f]
 * Note that both gaussian and laplacian are optional.  However, if both are specified,
 * they must have the same scale and images sizes.  If both are specified, the
 * number of levels can be different.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input image in <tt>\ref FOURCC_U8</tt> format.
 * \param [out] gaussian [optional] The gaussian pyramid to construct.
 * Level zero of gaussian is the same as the input image. If scale < 1, then  Level N of gaussian is created
 * by filtering level N-1 with the Gaussian Kernel \f$ \mathbf{G} \f$ and then downsampling using
 * \ref VX_INTERPOLATION_TYPE_BILINEAR.  If scale > 1, then Level N of gaussian is created by
 * upsampling level N-1 and filtering with the Gaussian Kernel \f$ \mathbf{G} \f$.
 * \param [out] laplacian [optional] The laplacian pyramid to construct.  Level N of laplacian is
 * created by upsampling level N+1 of gaussian, filtering with the Gaussian Kernel \f$ \mathbf{G} \f$ and subtracting
 * the result from level N of gaussian.  Level N of gaussian can be reconstructed by upsampling level N+1 of
 * gaussian, filtering with the Gaussian Kernel \f$ \mathbf{G} \f$ and adding Level N of laplacian.
 * \ingroup group_kernel_pyramid
 * \ref group_pyramid
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxPyramidNode(vx_graph graph, vx_image input, vx_pyramid gaussian, vx_pyramid laplacian);

/*! \brief [Graph] Creates an accumulate node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input <tt>\ref FOURCC_U8</tt> image.
 * \param [in,out] accum The accumulation image in <tt>\ref FOURCC_U16</tt>.
 * \ingroup group_kernel_accumulate
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxAccumulateImageNode(vx_graph graph, vx_image input, vx_image accum);

/*! \brief [Graph] Creates a weighted accumulate node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input <tt>\ref FOURCC_U8</tt> image.
 * \param [in] alpha The alpha value. Must be a <tt>\ref VX_TYPE_FLOAT32</tt>.
 * \param [in,out] accum The accumulation image.
 * \ingroup group_kernel_accumulate_weighted
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxAccumulateWeightedImageNode(vx_graph graph, vx_image input, vx_scalar alpha, vx_image accum);

/*! \brief [Graph] Creates an accumulate square node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input <tt>\ref FOURCC_U8</tt> image.
 * \param [in,out] accum The accumulation image in <tt>\ref FOURCC_U16</tt>.
 * \ingroup group_kernel_accumulate_square
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxAccumulateSquareImageNode(vx_graph graph, vx_image input, vx_image accum);

/*! \brief [Graph] Creates a min,max,loc node.
 * \param [in] graph The reference to create the graph.
 * \param [in] input The input image in FOURCC_U8, FOURCC_U16, FOURCC_S16, FOURCC_U32, or <tt>\ref FOURCC_S32</tt> format.
 * \param [out] minVal The minimum value in the image.
 * \param [out] maxVal The maximum value in the image.
 * \param [out] minLoc The location of the first minimum value.
 * \param [out] maxLoc The locaiton of the first maximum value.
 * \ingroup group_kernel_minmaxloc
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxMinMaxLocNode(vx_graph graph,
                        vx_image input,
                        vx_scalar minVal, vx_scalar maxVal,
                        vx_coordinates minLoc, vx_coordinates maxLoc);

/*! \brief [Graph] Creates a bitwise-and node.
 * \param [in] graph The reference to the graph.
 * \param [in] in1 A <tt>\ref FOURCC_U8</tt> input image
 * \param [in] in2 A <tt>\ref FOURCC_U8</tt> input image
 * \param [out] out The <tt>\ref FOURCC_U8</tt> output image.
 * \ingroup group_kernel_and
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxAndNode(vx_graph graph, vx_image in1, vx_image in2, vx_image out);

/*! \brief [Graph] Creates a bitwise inclusive-or node.
 * \param [in] graph The reference to the graph.
 * \param [in] in1 A <tt>\ref FOURCC_U8</tt> input image
 * \param [in] in2 A <tt>\ref FOURCC_U8</tt> input image
 * \param [out] out The <tt>\ref FOURCC_U8</tt> output image.
 * \ingroup group_kernel_or
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxOrNode(vx_graph graph, vx_image in1, vx_image in2, vx_image out);

/*! \brief [Graph] Creates a bitwise exclusive-or node.
 * \param [in] graph The reference to the graph.
 * \param [in] in1 A <tt>\ref FOURCC_U8</tt> input image
 * \param [in] in2 A <tt>\ref FOURCC_U8</tt> input image
 * \param [out] out The <tt>\ref FOURCC_U8</tt> output image.
 * \ingroup group_kernel_xor
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxXorNode(vx_graph graph, vx_image in1, vx_image in2, vx_image out);

/*! \brief [Graph] Creates a bitwise-not node.
 * \param [in] graph The reference to the graph.
 * \param [in] input A <tt>\ref FOURCC_U8</tt> input image
 * \param [out] output The <tt>\ref FOURCC_U8</tt> output image.
 * \ingroup group_kernel_not
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxNotNode(vx_graph graph, vx_image input, vx_image output);

/*! \brief [Graph] Creates an pixelwise-multiplication node.
 * \param [in] graph The reference to the graph.
 * \param [in] in1 An input image, <tt>\ref FOURCC_U8</tt> or <tt>\ref FOURCC_S16</tt>.
 * \param [in] in2 An input image, <tt>\ref FOURCC_U8</tt> or <tt>\ref FOURCC_S16</tt>.
 * \param [in] scale A non-negative <tt>\ref VX_TYPE_FLOAT32</tt> multiplied to each product before overflow handling
 * \param [in] policy A <tt>\ref VX_TYPE_ENUM</tt> of the \ref vx_convert_policy_e enumeration.
 * \param [out] out The output image, a <tt>\ref FOURCC_U8</tt> or <tt>\ref FOURCC_S16</tt> image.
 * \ingroup group_kernel_mult
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxMultiplyNode(vx_graph graph,
                       vx_image in1, vx_image in2,
                       vx_scalar scale,
                       vx_enum policy,
                       vx_image out);

/*! \brief [Graph] Creates an arithmetic addition node.
 * \param [in] graph The reference to the graph.
 * \param [in] in1 An input image, <tt>\ref FOURCC_U8</tt> or <tt>\ref FOURCC_S16</tt>.
 * \param [in] in2 An input image, <tt>\ref FOURCC_U8</tt> or <tt>\ref FOURCC_S16</tt>.
 * \param [in] policy A <tt>\ref VX_TYPE_ENUM</tt> of the \ref vx_convert_policy_e enumeration.
 * \param [out] out The output image, a <tt>\ref FOURCC_U8</tt> or <tt>\ref FOURCC_S16</tt> image.
 * \ingroup group_kernel_add
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxAddNode(vx_graph graph,
                  vx_image in1, vx_image in2,
                  vx_enum policy,
                  vx_image out);

/*! \brief [Graph] Creates an arithmetic subtraction node.
 * \param [in] graph The reference to the graph.
 * \param [in] in1 An input image, <tt>\ref FOURCC_U8</tt> or FOURCC_S16, the minuend.
 * \param [in] in2 An input image, <tt>\ref FOURCC_U8</tt> or FOURCC_S16, the subtrahend.
 * \param [in] policy A <tt>\ref VX_TYPE_ENUM</tt> of the \ref vx_convert_policy_e enumeration.
 * \param [out] out The output image, a <tt>\ref FOURCC_U8</tt> or <tt>\ref FOURCC_S16</tt> image.
 * \ingroup group_kernel_sub
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxSubtractNode(vx_graph graph,
                       vx_image in1, vx_image in2,
                       vx_enum policy,
                       vx_image out);

/*! \brief [Graph] Creates a bit-depth conversion node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input image.
 * \param [out] output The output image.
 * \param [in] policy A scalar containing a <tt>\ref VX_TYPE_ENUM</tt> of the \ref vx_convert_policy_e enumeration.
 * \param [in] shift A scalar containing a <tt>\ref VX_TYPE_INT32</tt> of the shift value.
 * \ingroup group_kernel_convertdepth
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxConvertDepthNode(vx_graph graph, vx_image input, vx_image output, vx_enum policy, vx_scalar shift);

/*! \brief [Graph] Creates a Canny Edge Detection Node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input <tt>\ref FOURCC_U8</tt> image.
 * \param [in] hyst The double threshold for hysteresis.
 * \param [in] gradient_size The size of the Sobel filter window, must support at least 3, 5 and 7.
 * \param [in] norm_type A flag indicating the norm used to compute the gradient, <tt>\ref VX_NORM_L1</tt> or VX_NORM_L2.
 * \param [out] output The output image in <tt>\ref FOURCC_U8</tt> format with values either 0 or 255.
 * \ingroup group_kernel_canny
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxCannyEdgeDetectorNode(vx_graph graph, vx_image input, vx_threshold hyst,
                                vx_int32 gradient_size, vx_enum norm_type,
                                vx_image output);

/*! \brief [Graph] Creates a Affine Warp Node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input <tt>\ref FOURCC_U8</tt> image.
 * \param [in] matrix The affine matrix. Must be 2x3 of type \ref VX_TYPE_FLOAT32.
 * \param [in] type The interpolation type from <tt>\ref vx_interpolation_type_e</tt>.
 * \param [out] output The output <tt>\ref FOURCC_U8</tt> image.
 * \ingroup group_kernel_warp_affine
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxWarpAffineNode(vx_graph graph, vx_image input, vx_matrix matrix, vx_enum type, vx_image output);

/*! \brief [Graph] Creates a Perspective Warp Node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input <tt>\ref FOURCC_U8</tt> image.
 * \param [in] matrix The perspective matrix. Must be 3x3 of type \ref VX_TYPE_FLOAT32.
 * \param [in] type The interpolation type from <tt>\ref vx_interpolation_type_e</tt>.
 * \param [out] output The output <tt>\ref FOURCC_U8</tt> image.
 * \ingroup group_kernel_warp_perspective
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxWarpPerspectiveNode(vx_graph graph, vx_image input, vx_matrix matrix, vx_enum type, vx_image output);

/*! \brief [Graph] Creates a Harris Corners Node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input <tt>\ref FOURCC_U8</tt> image.
 * \param [in] strength_thresh The minimum threshold which to eliminate Harris Corner scores.
 * \param [in] min_distance The radial Euclidean distance for non-maximum suppression.
 * \param [in] sensitivity The <tt>\ref VX_TYPE_FLOAT32</tt> scalar sensitivity threshold \f$ k \f$ from the Harris-Stephens equation.
 * \param [in] gradient_size The gradient window size to use on the input. The
 * implementation must support at least 3, 5, and 7.
 * \param [in] block_size The block window size used to compute the harris corner score.
 * The implementation must support at least 3, 5, and 7.
 * \param [out] corners The list of \ref vx_keypoint objects.
 * \ingroup group_kernel_harris
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxHarrisCornersNode(vx_graph graph,
                            vx_image input,
                            vx_scalar strength_thresh,
                            vx_scalar min_distance,
                            vx_scalar sensitivity,
                            vx_int32 gradient_size,
                            vx_int32 block_size,
                            vx_list corners);

/*! \brief [Graph] Creates a FAST Corners Node.
 * \param [in] graph The reference to the graph.
 * \param [in] input The input <tt>FOURCC_U8</tt> image.
 * \param [in] strength_thresh Threshold on difference between intensity of the central pixel and pixels on Bresenham's circle of radius 3 (<tt>\ref VX_TYPE_FLOAT32</tt> scalar)
 * \param [in] nonmax_supression If true, non-maximum suppression is applied to
 * detected corners before being places in the <tt>\ref vx_list</tt> of <tt>\ref vx_keypoint</tt> objects.
 * \param [out] corners Output corner <tt>\ref vx_list</tt> of <tt>\ref vx_keypoint</tt>.
 * \ingroup group_kernel_fast
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
vx_node vxFastCornersNode(vx_graph graph, vx_image input, vx_scalar strength_thresh, vx_bool nonmax_supression, vx_list corners);

/*! \brief [Graph] Creates a Lucas Kanade Tracking Node.
 * \param [in] graph The reference to the graph.
 * \param [in] old_images Input of first (old) image pyramid
 * \param [in] new_images Input of destination (new) image pyramid
 * \param [in] old_points a list of key points in a vx_list of vx_keypoint those key points are defined at
 *  the old_images high resolution pyramid
 * \param [in] new_points_estimates a list of estimation on what is the output key points in a vx_list of
 *  vx_keypoint those keypoints are defined at the new_images high resolution pyramid
 * \param [out] new_points a output list of key points in a vx_list of vx_keypoint those key points are
 *  defined at the new_images high resolution pyramid
 * \param [in] termination termination can be <tt>\ref VX_TERM_CRITERIA_ITERATIONS</tt> or <tt>\ref VX_TERM_CRITERIA_EPSILON</tt> or
 * <tt>\ref VX_TERM_CRITERIA_BOTH</tt>
 * \param [in] epsilon is the vx_uint32 error for terminating the algorithm
 * \param [in] num_iterations is the number of iterations
 * \param [in] use_initial_estimate Can be set to either vx_false_e or vx_true_e.
 * \param [in] window_dimension is the window on which to perform the algorithm.
 *
 * \ingroup group_kernel_opticalflowpyrlk
 * \return vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle
 */
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
                               vx_size window_dimension);

/*! \brief [Graph] Creates a Remap Node.
 * \param [in] graph The reference to the graph which will contain the node.
 * \param [in] input The input <tt>\ref FOURCC_U8</tt> image.
 * \param [in] table The remap table object.
 * \param [in] policy An interpolation type from <tt>\ref vx_interpolation_type_e</tt>.
 * <tt>\ref VX_INTERPOLATION_TYPE_AREA</tt> is not supported.
 * \param [out] output The output <tt>\ref FOURCC_U8</tt> image.
 * \note Only VX_NODE_ATTRIBUTE_BORDER value VX_BORDER_MODE_UNDEFINED or
 * VX_BORDER_MODE_CONSTANT is supported.
 * \return Returns a \ref vx_node
 * \retval 0 Node could not be created.
 * \retval * Node Handle.
 * \ingroup group_kernel_remap
 */
vx_node vxRemapNode(vx_graph graph,
                    vx_image input,
                    vx_remap table,
                    vx_enum policy,
                    vx_image output);

#ifdef __cplusplus
}
#endif

#endif
