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

#ifndef _OPENVX_UTILITY_H_
#define _OPENVX_UTILITY_H_

/*!
 * \file
 * \brief The OpenVX Utility Library.
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief [Immediate] Invokes an immediate Color Conversion.
 * \param [in] input The input image.
 * \param [out] output The output image.
 * \ingroup group_kernel_colorconvert
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuColorConvert(vx_image input, vx_image output);

/*! \brief [Immediate] Invokes an immediate Channel Extract.
 * \param [in] input The input image. Must be one of the defined \ref vx_fourcc_e multiplanar formats.
 * \param [in] channel The \ref vx_channel_e channel to extract.
 * \param [out] output The output image. Must be \ref FOURCC_U8.
 * \ingroup group_kernel_channelextract
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuChannelExtract(vx_image input, vx_enum channel, vx_image output);

/*! \brief [Immediate] Invokes an immediate Channel Combine.
 * \param [in] plane0 The plane which will form channel 0. Must be FOURCC_U8.
 * \param [in] plane1 The plane which will form channel 1. Must be FOURCC_U8.
 * \param [in] plane2 [optional] The plane which will form channel 2. Must be FOURCC_U8.
 * \param [in] plane3 [optional] The plane which will form channel 3. Must be FOURCC_U8.
 * \param [out] output The output image.
 * \ingroup group_kernel_channelcombine
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuChannelCombine(vx_image plane0, vx_image plane1, vx_image plane2, vx_image plane3, vx_image output);

/*! \brief [Immediate] Invokes an immediate Sobel 3x3.
 * \param [in] input The input image in FOURCC_U8 format.
 * \param [out] output_x [optional] The output gradient in the x direction in FOURCC_S16.
 * \param [out] output_y [optional] The output gradient in the y direction in FOURCC_S16.
 * \ingroup group_kernel_sobel3x3
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuSobel3x3(vx_image input, vx_image output_x, vx_image output_y);

/*! \brief [Immediate] Invokes an immediate Magnitude.
 * \param [in] grad_x The input x image. This should be in FOURCC_S16 format.
 * \param [in] grad_y The input y image. This should be in FOURCC_S16 format.
 * \param [out] output The magnitude image. This will be in FOURCC_U8 format.
 * \ingroup group_kernel_magnitude
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuMagnitude(vx_image grad_x, vx_image grad_y, vx_image output);

/*! \brief [Immediate] Invokes an immediate Phase.
 * \param [in] grad_x The input x image. This should be in FOURCC_S16 format.
 * \param [in] grad_y The input y image. This should be in FOURCC_S16 format.
 * \param [out] output The phase image. This will be in FOURCC_U8 format.
 * \ingroup group_kernel_phase
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuPhase(vx_image grad_x, vx_image grad_y, vx_image output);

/*! \brief [Immediate] Scales an input image to an output image.
 * \param [in] src The source image.
 * \param [out] dst The destintation image.
 * \param [in] type The interpolation type. \see vx_interpolation_type_e.
 * \ingroup group_kernel_scale_image
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuScaleImage(vx_image src, vx_image dst, vx_enum type);

/*! \brief [Immediate] Processes the image through the LUT.
 * \param [in] input The input image in FOURCC_U8.
 * \param [in] lut The LUT which is of type VX_TYPE_UINT8, or VX_TYPE_UINT16.
 * \param [out] output The output image of type FOURCC_U8.
 * \ingroup group_kernel_lut
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuTableLookup(vx_image input, vx_lut lut, vx_image output);

/*! \brief [Immediate] Generates a distribution from an image.
 * \param [in] input The input image in FOURCC_U8.
 * \param [out] distribution The output distribution.
 * \ingroup group_kernel_histogram
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuHistogram(vx_image input, vx_distribution distribution);

/*! \brief [Immediate] Equalizes the Histogram of a grayscale image.
 * \param [in] input The grayscale input image in FOURCC_U8.
 * \param [out] output The grayscale output image of type FOURCC_U8 with equalized brightness and contrast.
 * \ingroup group_kernel_equalize_hist
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuEqualizeHist(vx_image input, vx_image output);

/*! \brief [Immediate] Computes the absolute difference between two images.
 * \param [in] in1 An input image
 * \param [in] in2 An input image
 * \param [out] out The output image.
 * \ingroup group_kernel_absdiff
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuAbsDiff(vx_image in1, vx_image in2, vx_image out);

/*! \brief [Immediate] Computes the mean value and standard deviation.
 * \param [in] input The input image.
 * \param [out] mean The average pixel value.
 * \param [out] stddev The standard deviation of the pixel values.
 * \ingroup group_kernel_meanstddev
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuMeanStdDev(vx_image input, vx_float32 *mean, vx_float32 *stddev);

/*! \brief [Immediate] Threshold's an input image and produces a FOURCC_U8
 * boolean image.
 * \param [in] input The input image. FOURCC_U8 is supported.
 * \param [in] thresh The thresholding object which defines the parameters of
 * the operation.
 * \param [out] output The output boolean image. Values are either 0 or 255.
 * \ingroup group_kernel_threshold
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuThreshold(vx_image input, vx_threshold thresh, vx_image output);

/*! \brief [Immediate] Computes the integral image of the input.
 * \param [in] input The input image in FOURCC_U8 format.
 * \param [out] output The output image in FOURCC_U32 format.
 * \ingroup group_kernel_integral_image
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuIntegralImage(vx_image input, vx_image output);

/*! \brief [Immediate] Erodes an image by a 3x3 window.
 * \param [in] input The input image in FOURCC_U8 format.
 * \param [out] output The output image in FOURCC_U8 format.
 * \ingroup group_kernel_erode_image
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuErode3x3(vx_image input, vx_image output);

/*! \brief [Immediate] Dilates an image by a 3x3 window.
 * \param [in] input The input image in FOURCC_U8 format.
 * \param [out] output The output image in FOURCC_U8 format.
 * \ingroup group_kernel_dilate_image
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuDilate3x3(vx_image input, vx_image output);

/*! \brief [Immediate] Computes a median filter on the image by a 3x3 window.
 * \param [in] input The input image in FOURCC_U8 format.
 * \param [out] output The output image in FOURCC_U8 format.
 * \ingroup group_kernel_median_image
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuMedian3x3(vx_image input, vx_image output);

/*! \brief [Immediate] Computes a box filter on the image by a 3x3 window.
 * \param [in] input The input image in FOURCC_U8 format.
 * \param [out] output The output image in FOURCC_U8 format.
 * \ingroup group_kernel_box_image
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuBox3x3(vx_image input, vx_image output);

/*! \brief [Immediate] Computes a gaussian filter on the image by a 3x3 window.
 * \param [in] input The input image in FOURCC_U8 format.
 * \param [out] output The output image in FOURCC_U8 format.
 * \ingroup group_kernel_gaussian_image
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuGaussian3x3(vx_image input, vx_image output);

/*! \brief [Immediate] Computes a convolution on the input image with the supplied
 * matrix.
 * \param [in] input The input image in FOURCC_U8 format.
 * \param [in] matrix The convolution matrix.
 * \param [out] output The output image in FOURCC_S16 format.
 * \ingroup group_kernel_custom_convolution
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuConvolve(vx_image input, vx_convolution matrix, vx_image output);

/*! \brief [Immediate] Computes a gaussian and/or laplacian pyramid.
 * \param [in] input The input image in FOURCC_U8.
 * \param [out] gaussian [optional] The gaussian pyramid to construct.
 * Level zero of gaussian is the same as the input image. If scale < 1, then  Level N of gaussian is created
 * by filtering level N-1 with the Gaussian Kernel \f$ \mathbf{G} \f$ and then downsampling using
 * \ref VX_INTERPOLATION_TYPE_BILINEAR.  If scale > 1, then Level N of gaussian is created by
 * upsampling level N-1 and filtering with the Gaussian Kernel \f$ \mathbf{G} \f$.
 * \param [out] laplacian [optional] The laplacian pyramid to construct.  Level N of laplacian is
 * created by upsampling level N+1 of gaussian, filtering with the Gaussian Kernel \f$ \mathbf{G} \f$ and subtracting
 * the result from level N of gaussian.  Level N of gaussian can be reconstructed by upsampling level N+1 of
 * gaussian, filtering with the Gaussian Kernel \f$ \mathbf{G} \f$ and adding Level N of laplacian.
 * * \ingroup group_kernel_pyramid
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuPyramid(vx_image input, vx_pyramid gaussian, vx_pyramid laplacian);

/*! \brief [Immediate] Creates an accumulate node.
 * \param [in] input The input FOURCC_U8 image.
 * \param [in,out] accum The accumulation image in FOURCC_U16.
 * \ingroup group_kernel_accumulate
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuAccumulateImage(vx_image input, vx_image accum);

/*! \brief [Immediate] Creates a weighted accumulate node.
 * \param [in] input The input FOURCC_U8 image.
 * \param [in] alpha The alpha value. Must be between 0 and 1 exclusive.
 * \param [in,out] accum The accumulation image.
 * \ingroup group_kernel_accumulate_weighted
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuAccumulateWeightedImage(vx_image input, vx_float32 alpha, vx_image accum);

/*! \brief [Immediate] Creates an accumulate square node.
 * \param [in] input The input FOURCC_U8 image.
 * \param [in,out] accum The accumulation image in FOURCC_U16.
 * \ingroup group_kernel_accumulate_square
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuAccumulateSquareImage(vx_image input, vx_image accum);

/*! \brief [Immediate] Computes the minimum and maximum values of the image.
 * \param [in] input The input image in FOURCC_U8, FOURCC_U16, FOURCC_S16, FOURCC_U32, or FOURCC_S32 format.
 * \param [out] minVal The minimum value in the image.
 * \param [out] maxVal The maximum value in the image.
 * \param [out] minLoc The location of the first minimum value.
 * \param [out] maxLoc The locaiton of the first maximum value.
 * \ingroup group_kernel_minmaxloc
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuMinMaxLoc(vx_image input,
                        vx_scalar minVal, vx_scalar maxVal,
                        vx_coordinates minLoc, vx_coordinates maxLoc);

/*! \brief [Immediate] Converts the input images bit-depth into the output image.
 * \param [in] input The input image.
 * \param [out] output The output image.
 * \param [in] policy A \ref vx_convert_policy_e enumeration.
 * \param [in] shift The shift value.
 * \ingroup group_kernel_convertdepth
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuConvertDepth(vx_image input, vx_image output, vx_enum policy, vx_int32 shift);

/*! \brief [Immediate] Computes Canny Edges on the input image into the output image.
 * \param [in] input The input FOURCC_U8 image.
 * \param [in] hyst The double threshold for hysteresis.
 * \param [in] gradient_size The size of the Sobel filter window, must support at least 3, 5 and 7.
 * \param [in] norm_type A flag indicating the norm used to compute the gradient, VX_NORM_L1 or VX_NORM_L2.
 * \param [out] output The output image in FOURCC_U8 format.
 * \ingroup group_kernel_canny
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuCannyEdgeDetector(vx_image input, vx_threshold hyst,
                               vx_int32 gradient_size, vx_enum norm_type,
                               vx_image output);

/*! \brief [Immediate] Performs a Gaussian Blur (3x3) on an image then half-scales it.
 * \details The output image size is determined by:
 * \f[
 * W_{output} = \frac{W_{input} + 1}{2} \\
 * H_{output} = \frac{H_{input} + 1}{2}
 * \f]
 * \param [in] input The input FOURCC_U8 image.
 * \param [out] output The output FOURCC_U8 image.
 * \ingroup group_kernel_scale_image
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuHalfScaleGaussian3x3(vx_image input, vx_image output);

/*! \brief [Immediate] Computes the bitwise and between two images.
 * \param [in] in1 A FOURCC_U8 input image
 * \param [in] in2 A FOURCC_U8 input image
 * \param [out] out The FOURCC_U8 output image.
 * \ingroup group_kernel_and
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuAnd(vx_image in1, vx_image in2, vx_image out);

/*! \brief [Immediate] Computes the bitwise inclusive-or between two images.
 * \param [in] in1 A FOURCC_U8 input image
 * \param [in] in2 A FOURCC_U8 input image
 * \param [out] out The FOURCC_U8 output image.
 * \ingroup group_kernel_or
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuOr(vx_image in1, vx_image in2, vx_image out);

/*! \brief [Immediate] Computes the bitwise exclusive-or between two images.
 * \param [in] in1 A FOURCC_U8 input image
 * \param [in] in2 A FOURCC_U8 input image
 * \param [out] out The FOURCC_U8 output image.
 * \ingroup group_kernel_xor
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuXor(vx_image in1, vx_image in2, vx_image out);

/*! \brief [Immediate] Computes the bitwise not of an image.
 * \param [in] input The FOURCC_U8 input image
 * \param [out] output The FOURCC_U8 output image.
 * \ingroup group_kernel_not
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuNot(vx_image input, vx_image output);

/*! \brief [Immediate] Performs elementwise multiplications on pixel values in the input images and a scale.
 * \param [in] in1 A FOURCC_U8 or FOURCC_S16 input image.
 * \param [in] in2 A FOURCC_U8 or FOURCC_S16 input image.
 * \param [in] scale The scale value.
 * \param [in] policy A \ref vx_convert_policy_e enumeration.
 * \param [out] out The output image in FOURCC_U8 or FOURCC_S16 format.
 * \ingroup group_kernel_mult
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuMultiply(vx_image in1, vx_image in2, vx_float32 scale, vx_enum policy, vx_image out);

/*! \brief [Immediate] Performs arithmetic addition on pixel values in the input images.
 * \param [in] in1 A FOURCC_U8 or FOURCC_S16 input image.
 * \param [in] in2 A FOURCC_U8 or FOURCC_S16 input image.
 * \param [in] policy A \ref vx_convert_policy_e enumeration.
 * \param [out] out The output image in FOURCC_U8 or FOURCC_S16 format.
 * \ingroup group_kernel_add
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuAdd(vx_image in1, vx_image in2, vx_enum policy, vx_image out);

/*! \brief [Immediate] Performs arithmetic subtraction on pixel values in the input images.
 * \param [in] in1 A FOURCC_U8 or FOURCC_S16 input image, the minuend.
 * \param [in] in2 A FOURCC_U8 or FOURCC_S16 input image, the subtrahend.
 * \param [in] policy A \ref vx_convert_policy_e enumeration.
 * \param [out] out The output image in FOURCC_U8 or FOURCC_S16 format.
 * \ingroup group_kernel_sub
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuSubtract(vx_image in1, vx_image in2, vx_enum policy, vx_image out);

/*! \brief [Immediate] Performs an Affine warp on an image.
 * \param [in] input The input FOURCC_U8 image.
 * \param [in] matrix The affine matrix. Must be 2x3 of type \ref VX_TYPE_FLOAT32.
 * \param [in] type The interpolation type from \ref vx_interpolation_type_e.
 * \param [out] output The output FOURCC_U8 image.
 * \ingroup group_kernel_warp_affine
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuWarpAffine(vx_image input, vx_matrix matrix, vx_enum type, vx_image output);

/*! \brief [Immediate] Performs an Perspective warp on an image.
 * \param [in] input The input FOURCC_U8 image.
 * \param [in] matrix The perspective matrix. Must be 3x3 of type \ref VX_TYPE_FLOAT32.
 * \param [in] type The interpolation type from \ref vx_interpolation_type_e.
 * \param [out] output The output FOURCC_U8 image.
 * \ingroup group_kernel_warp_perspective
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuWarpPerspective(vx_image input, vx_matrix matrix, vx_enum type, vx_image output);

/*! \brief [Immediate] Computes the Harris Corners over an image and produces the list of scored points.
 * \param [in] input The input FOURCC_U8 image.
 * \param [in] strength_thresh The minimum threshold which to eliminate Harris Corner scores.
 * \param [in] min_distance The radial Euclidean distance for non-maximum suppression.
 * \param [in] sensitivity The \ref VX_TYPE_FLOAT32 scalar sensitivity threshold \f$ k \f$ from the Harris-Stephens equation.
 * \param [in] gradient_size The gradient window size to use on the input. The
 * implementation must support at least 3, 5, and 7.
 * \param [in] block_size The block window size used to compute the harris corner score.
 * The implementation must support at least 3, 5, and 7.
 * \param [out] corners The list of \ref vx_keypoint objects.
 * \ingroup group_kernel_harris
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuHarrisCorners(vx_image input,
        vx_scalar strength_thresh,
        vx_scalar min_distance,
        vx_scalar sensitivity,
        vx_int32 gradient_size,
        vx_int32 block_size,
        vx_list corners);


/*! \brief [Immediate] Computes corners on an image using FAST algorithm and produces the list of feature points.
  \param [in] input The input <tt>FOURCC_U8</tt> image.
 * \param [in] strength_thresh Threshold on difference between intensity of the central pixel and pixels on Bresenham's circle of radius 3 (<tt>\ref VX_TYPE_FLOAT32</tt> scalar)
 * \param [in] nonmax_supression If true, non-maximum suppression is applied to
 * detected corners before being places in the <tt>\ref vx_list</tt> of <tt>\ref vx_keypoint</tt> objects.
 * \param [out] corners Output corner <tt>\ref vx_list</tt> of <tt>\ref vx_keypoint</tt>.
  \ingroup group_kernel_fast
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS Success
 * \retval *          An error occurred. See <tt>\ref vx_status_e</tt>.
 */
vx_status vxuFastCorners(vx_image input, vx_scalar strength_thresh, vx_bool nonmax_supression, vx_list corners);

/*! \brief [Immediate] Computes an optical flow on two images.
 * \param [in] old_images Input of first (old) image pyramid
 * \param [in] new_images Input of destination (new) image pyramid
 * \param [in] old_points a list of key points in a vx_list of vx_keypoint those key points are defined at
 *  the old_images high resolution pyramid
 * \param [in] new_points_estimates a list of estimation on what is the output key points in a vx_list of
 *  vx_keypoint those keypoints are defined at the new_images high resolution pyramid
 * \param [out] new_points a output list of key points in a vx_list of vx_keypoint those key points are
 *  defined at the new_images high resolution pyramid
 * \param [in] termination termination can be VX_TERM_CRITERIA_ITERATIONS or VX_TERM_CRITERIA_EPSILON or
 * VX_TERM_CRITERIA_BOTH
 * \param [in] epsilon is the vx_uint32 error for terminating the algorithm
 * \param [in] num_iterations is the number of iterations
 * \param [in] use_initial_estimate Can be set to either vx_false_e or vx_true_e.
 * \param [in] window_dimension is the window on which to perform the algorithm.
 *
 * \ingroup group_kernel_opticalflowpyrlk
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Success
 * \retval * An error occurred. See \ref vx_status_e.
 */
vx_status vxuOpticalFlowPyrLK(vx_pyramid old_images,
                              vx_pyramid new_images,
                              vx_list old_points,
                              vx_list new_points_estimates,
                              vx_list new_points,
                              vx_enum termination,
                              vx_scalar epsilon,
                              vx_scalar num_iterations,
                              vx_scalar use_initial_estimate,
                              vx_size window_dimension);

/*! \brief [Immediate] Remaps an output image from an input image.
 * \param [in] input The input \ref FOURCC_U8 image.
 * \param [in] table The remap table object.
 * \param [in] policy The interpolation policy from \ref vx_interpolation_type_e.
 * \ref VX_INTERPOLATION_TYPE_AREA is not supported.
 * \param [out] output The output \ref FOURCC_U8 image.
 * \return Returns a \ref vx_status_e enumeration.
 * \ingroup group_kernel_remap
 */
vx_status vxuRemap(vx_image input,
                  vx_remap table,
                  vx_enum policy,
                  vx_image output);

#ifdef __cplusplus
}
#endif

#endif
