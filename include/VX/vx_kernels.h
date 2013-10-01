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

#ifndef _OPENVX_KERNELS_H_
#define _OPENVX_KERNELS_H_

/*!
 * \file
 * \brief The list of supported kernels in the OpenVX standard.
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#ifdef  __cplusplus
extern "C" {
#endif

/*! \brief The standard list of available libraries */
enum vx_library_e {
    /*! \brief The base set of kernels as defined by Khronos. */
    VX_LIBRARY_KHR_BASE = 0x0,
};

/*!
 * \brief The standard list of available vision kernels.
 *
 * Each kernel listed here can be used with the <tt>\ref vxGetKernelByEnum</tt> call.
 * When programming the parameters, use
 * \arg <tt>\ref VX_INPUT</tt> for [in]
 * \arg <tt>\ref VX_OUTPUT</tt> for [out]
 * \arg <tt>\ref VX_BIDIRECTIONAL</tt> for [in,out]
 *
 * When programming the parameters, use
 * \arg <tt>\ref VX_TYPE_IMAGE</tt> for a <tt>\ref vx_image</tt> in the size field of <tt>\ref vxGetParameterByIndex</tt> or <tt>\ref vxSetParameterByIndex</tt>  * \arg <tt>\ref VX_TYPE_BUFFER</tt> for a <tt>\ref vx_buffer</tt> in the size field of <tt>\ref vxGetParameterByIndex</tt> or <tt>\ref vxSetParameterByIndex</tt>  * \arg or other appropriate types in \ref vx_type_e.
 * \note All kernels in the lower level specification would be reflected here.
 * These names are prone to changing before the specification is complete.
 * \ingroup group_kernel
 */
enum vx_kernel_e {

    /*!
     * \brief The invalid kernel is used to for conformance failure in relation to
     * some kernel operation (Get/Release).
     * \details If the kernel is executed it shall always return an error.
     * The kernel has no parameters. To address by name use "org.khronos.openvx.invalid".
     */
    VX_KERNEL_INVALID = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x0,

    /*!
     * \brief The Color Space conversion kernel.
     * \details The conversions are based on the <tt>\ref vx_fourcc_e</tt> code in the images.
     * \param  [in] vx_image The input image.
     * \param [out] vx_image The output image.
     * \see group_kernel_colorconvert
     */
    VX_KERNEL_COLOR_CONVERT = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x1,

    /*!
     * \brief The Generic Channel Extraction Kernel.
     * \details This kernel can remove individual color channels from an interleaved
     * or semi-planar, planar, sub-sampled planar image. A client could extract
     * a red channel from an interleaved RGB image or do a Luma extract from a
     * YUV format.
     * \param  [in] vx_image The input image.
     * \param [in]  vx_enum The channel index. This is not dependant on the input channel order or packing. See \see vx_channel_e.
     * \param [out] vx_image The output image. Must be <tt>\ref FOURCC_U8</tt> formatted.
     * \see group_kernel_channelextract
     */
    VX_KERNEL_CHANNEL_EXTRACT = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x2,

    /*!
     * \brief The Generic Channel Combine Kernel.
     * \details This kernel combine multiple individual planes into a single
     * multiplanar image of the type specified in the output image.
     * \param [in] vx_image Plane 0, Must be FOURCC_U8
     * \param [in] vx_image Plane 1, Must be FOURCC_U8
     * \param [in] vx_image Plane 2, [optional] Must be FOURCC_U8
     * \param [in] vx_image Plane 3, [optional] Must be FOURCC_U8
     * \param [out] vx_image Output Image. FOURCC_RGB, FOURCC_NV12, or FOURCC_IYUV.
     * \see group_kernel_channelcombine
     */
    VX_KERNEL_CHANNEL_COMBINE = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x3,

    /*! \brief The Sobel 3x3 Filter Kernel.
     * \param [in] vx_image Input Image. Must be FOURCC_U8
     * \param [out] vx_image Output Gradient X image. Must be FOURCC_S16.
     * \param [out] vx_image Output Gradient Y image. Must be FOURCC_S16.
     * \see group_kernel_sobel3x3
     */
    VX_KERNEL_SOBEL_3x3 = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x4,

    /*!
     * \brief The Magnitude Kernel.
     * \details This kernel produces a magnitude plane from two input gradients.
     * \param [in] vx_image The input x image in FOURCC_S16
     * \param [in] vx_image The input y image in FOURCC_S16
     * \param [out] vx_image The output magnitude plane in FOURCC_U8
     * \see group_kernel_magnitude
     */
    VX_KERNEL_MAGNITUDE = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x5,

    /*!
     * \brief The Magnitude Kernel.
     * \details This kernel produces a phase plane from two input gradients.
     * \param [in] vx_image The input x image in FOURCC_S16
     * \param [in] vx_image The input y image in FOURCC_S16
     * \param [out] vx_image The output phase plane in FOURCC_U8. 0-255 map to
     * 0 to 2*PI.
     * \see group_kernel_phase
     */
    VX_KERNEL_PHASE = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x6,

    /*!
     * \brief The Scale Image Kernel.
     * \details This kernel provides resizing of an input image to an output image.
     * The scaling factor is determined but the relative sizes of the input and
     * output.
     * \param [in] vx_image The input image.
     * \param [out] vx_image The output image. This must not be a virtual image.
     * \param [in] vx_enum The filtering type. VX_FILTER_DEFAULT is the default.
     * \see group_kernel_scale_image
     */
    VX_KERNEL_SCALE_IMAGE = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x7,

    /*! \brief The Table Lookup kernel
     * \param [in] vx_image The input image in FOURCC_U8.
     * \param [in] vx_lut The LUT which is of type VX_TYPE_UINT8.
     * \param [out] vx_image The output image of type FOURCC_U8.
     * \see group_kernel_lut
     */
    VX_KERNEL_TABLE_LOOKUP = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x8,

    /*! \brief The Histogram Kernel.
     * \param [in] vx_image The input image.
     * \param [out] vx_distribution The distribution.
     * \see group_kernel_histogram
     */
    VX_KERNEL_HISTOGRAM = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x9,

    /*! \brief The Histogram Equalization Kernel.
     * \param [in] vx_image The grayscale input image in FOURCC_U8.
     * \param [out] vx_image The grayscale output image of type FOURCC_U8 with equalized brightness and contrast.
     * \see group_kernel_equalize_hist
     */
    VX_KERNEL_EQUALIZE_HISTOGRAM = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0xA,

    /*! \brief The Absolute Difference Kernel.
     * \param [in] vx_image An input image.
     * \param [in] vx_image An input image.
     * \param [out] vx_image The output image.
     * \see group_kernel_absdiff
     */
    VX_KERNEL_ABSDIFF = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0xB,

    /*! \brief The Mean and Standard Deviation Kernel.
     * \param [in] vx_image The input image.
     * \param [out] vx_scalar A VX_TYPE_FLOAT32 value outputting the mean.
     * \param [out] vx_scalar A VX_TYPE_FLOAT32 value outputting the standard deviation.
     * \see group_kernel_meanstddev
     */
    VX_KERNEL_MEAN_STDDEV = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0xC,

    /*! \brief The Threshold Kernel.
     * \param [in] vx_image A FOURCC_U8 image.
     * \param [in] vx_threshold A VX_THRESHOLD_TYPE_BINARY
     * \param [out] vx_image A FOURCC_U8 with either 0 or 255 as values.
     * \see group_kernel_threshold
     */
    VX_KERNEL_THRESHOLD = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0xD,

    /*! \brief The Integral Image Kernel.
     * \param [in] vx_image A FOURCC_U8 image.
     * \param [out] vx_image A FOURCC_U32 image.
     * \see group_kernel_integral_image
     */
    VX_KERNEL_INTEGRAL_IMAGE = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0xE,

    /*! \brief The dilate kernel.
     * \param [in] vx_image The FOURCC_U8 input image.
     * \param [out] vx_image The FOURCC_U8 output image.
     * \see group_kernel_dilate_image
     */
    VX_KERNEL_DILATE_3x3 = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0xF,

    /*! \brief The erode kernel.
     * \param [in] vx_image The FOURCC_U8 input image.
     * \param [out] vx_image The FOURCC_U8 output image.
     * \see group_kernel_dilate_image
     */
    VX_KERNEL_ERODE_3x3 = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x10,

    /*! \brief The median image filter.
     * \param [in] vx_image The FOURCC_U8 input image.
     * \param [out] vx_image The FOURCC_U8 output image.
     * \see group_kernel_median_image
     */
    VX_KERNEL_MEDIAN_3x3 = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x11,

    /*! \brief The box filter kernel.
     * \param [in] vx_image The FOURCC_U8 input image.
     * \param [out] vx_image The FOURCC_U8 output image.
     * \see group_kernel_box_image
     */
    VX_KERNEL_BOX_3x3 = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x12,

    /*! \brief The gaussian filter kernel.
     * \param [in] vx_image The FOURCC_U8 input image.
     * \param [out] vx_image The FOURCC_U8 output image.
     * \see group_kernel_gaussian_image
     */
    VX_KERNEL_GAUSSIAN_3x3 = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x13,

    /*! \brief The custom convolution kernel.
     * \param [in] vx_image The FOURCC_U8 input image.
     * \param [in] vx_convolution The vx_int16 symmetric matrix.
     * \param [out] vx_image The FOURCC_S16 output image.
     * \see group_kernel_custom_convolution
     */
    VX_KERNEL_CUSTOM_CONVOLUTION = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x14,

    /*! \brief The image pyramid kernel.
     * \param [in] vx_image The input FOURCC_U8 image.
     * \param [out] vx_pyramid The pyramid object with the defined number of levels.
     * \see group_kernel_pyramid
     */
    VX_KERNEL_PYRAMID = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x15,

    /*! \brief The accumulation kernel.
     * \param [in] vx_image The input FOURCC_U8 image.
     * \param [in,out] vx_image The accumulation image.
     * \see group_kernel_accumulate
     */
    VX_KERNEL_ACCUMULATE = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x16,

    /*! \brief The weigthed accumulation kernel.
     * \param [in] vx_image The input FOURCC_U8 image.
     * \param [in] vx_scalar The alpha value.
     * \param [in,out] vx_image The accumulation image.
     * \see group_kernel_accumulate_weighted
     */
    VX_KERNEL_ACCUMULATE_WEIGHTED = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x17,

    /*! \brief The squared accumulation kernel.
     * \param [in] vx_image The input FOURCC_U8 image.
     * \param [in,out] vx_image The accumulation image.
     * \see group_kernel_accumulate_square
     */
    VX_KERNEL_ACCUMULATE_SQUARE = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x18,

    /*! \brief The min and max location kernel.
     * \param [in] vx_image The input image.
     * \param [out] vx_scalar The minimum value.
     * \param [out] vx_scalar The maximum value.
     * \param [out] vx_coordinates The minimum location
     * \param [out] vx_coordinates The maximum location
     * \see group_kernel_minmaxloc
     */
    VX_KERNEL_MINMAXLOC = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x19,

    /*! \brief The bit-depth conversion kernel.
     * \param [in] vx_image The input image. Formats include FOURCC_U8, FOURCC_U16.
     * \param [out] vx_image The output image. Formats include FOURCC_U8, FOURCC_U16.
     * \param [in] vx_scalar The enumeration of the \ref vx_convert_policy_e.
     * \param [in] vx_scalar The vx_int32 shift value.
     * \see group_kernel_convertdepth
     */
    VX_KERNEL_CONVERTDEPTH = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x1A,

    /*! \brief The Canny Edge Detector.
     * \param [in] vx_image The input FOURCC_U8 image.
     * \param [in] vx_threshold The double threshold for hysteresis.
     * \param [out] vx_image The output image in FOURCC_U8 format.
     * \see group_kernel_canny
     */
    VX_KERNEL_CANNY_EDGE_DETECTOR = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x1B,

    /*! \brief The Bitwise And Kernel.
     * \param [in] vx_image Input image used as an operand.
     * \param [in] vx_image Input image used as an operand.
     * \param [out] vx_image The output image containing the result of the operation.
     * \see group_kernel_and
     */
    VX_KERNEL_AND = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x1C,

    /*! \brief The Bitwise Inclusive Or Kernel.
     * \param [in] vx_image Input image used as an operand.
     * \param [in] vx_image Input image used as an operand.
     * \param [out] vx_image The output image containing the result of the operation.
     * \see group_kernel_or
     */
    VX_KERNEL_OR = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x1D,

    /*! \brief The Bitwise Exclusive Or Kernel.
     * \param [in] vx_image Input image used as an operand.
     * \param [in] vx_image Input image used as an operand.
     * \param [out] vx_image The output image containing the result of the operation.
     * \see group_kernel_xor
     */
    VX_KERNEL_XOR = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x1E,

    /*! \brief The Bitwise Not Kernel.
     * \param [in] vx_image Input image used as the operand.
     * \param [out] vx_image The output image containing the result of the operation.
     * \see group_kernel_not
     */
    VX_KERNEL_NOT = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x1F,

    /*! \brief The Pixelwise Multiplication Kernel.
     * \param [in] vx_image Input image used as an operand.
     * \param [in] vx_image Input image used as an operand.
     * \param [in] vx_scalar A non-negative <tt>\ref VX_TYPE_FLOAT32</tt> scale multiplied to each product before overflow handling.
     * \param [in] vx_enum Overflow policy, an enumeration of the \ref vx_convert_policy_e.
     * \param [out] vx_image The output image containing the result of the operation.
     * \see group_kernel_mult
     */
    VX_KERNEL_MULTIPLY = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x20,

    /*! \brief The Addition Kernel.
     * \param [in] vx_image Input image used as an operand.
     * \param [in] vx_image Input image used as an operand.
     * \param [in] vx_enum Overflow policy, an enumeration of the \ref vx_convert_policy_e.
     * \param [out] vx_image The output image containing the result of the operation.
     * \see group_kernel_add
     */
    VX_KERNEL_ADD = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x21,

    /*! \brief The Subtraction Kernel.
     * \param [in] vx_image Input image used as an operand.
     * \param [in] vx_image Input image used as an operand.
     * \param [in] vx_enum Overflow policy, an enumeration of the \ref vx_convert_policy_e.
     * \param [out] vx_image The output image containing the result of the operation.
     * \see group_kernel_sub
     */
    VX_KERNEL_SUBTRACT = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x22,

    /*! \brief The Warp Affine Kernel.
     * \param [in] vx_image The input image.
     * \param [in] vx_matrix The 2x3 affine matrix.
     * \param [in] vx_enum The Interpolation type from \ref vx_interpolation_type_e.
     * \param [out] vx_image The output image.
     * \see group_kernel_warp_affine
     */
    VX_KERNEL_WARP_AFFINE = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x23,

    /*! \brief The Warp Perspective Kernel.
	 * \param [in] vx_image The input image.
	 * \param [in] vx_matrix The 3x3 perspective matrix.
	 * \param [in] vx_enum The Interpolation type from \ref vx_interpolation_type_e.
	 * \param [out] vx_image The output image.
	 * \see group_kernel_warp_perspective
	 */
    VX_KERNEL_WARP_PERSPECTIVE = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_BASE) + 0x24,

    /*! \brief The Harris Corners Kernel.
     * \param [in] vx_image The input image.
     * \param [in] vx_scalar The sensitivity factor.
     * \param [in] vx_scalar The minimum threshold which to eliminate Harris Corner scores.
     * \param [in] vx_scalar The radial Euclidean distance for non-maximum suppression.
     * \param [in] vx_scalar The <tt>\ref VX_TYPE_FLOAT32</tt> scalar sensitivity threshold \f$ k \f$ from the Harris-Stephens equation.
     * \param [in] vx_scalar The gradient window size to use on the input. The
     * implementation must support at least 3, 5, and 7.
     * \param [in] vx_scalar The block window size used to compute the harris corner score.
     * The implementation must support at least 3, 5, and 7.
     * \param [out] vx_list The list of output coordinates.
     * \see group_kernel_harris
     */
    VX_KERNEL_HARRIS_CORNERS,

    /*! \brief The FAST Corners Kernel.
     * \param [in]    vx_image   input                The input grayscale image (<tt>FOURCC_U8</tt>).
     * \param [in]    vx_scalar  strength_thresh      Threshold on difference between intensity of the central pixel and pixels on Bresenham�s circle of radius 3 (<tt>\ref VX_TYPE_FLOAT32 scalar</tt>)
     * \param [in]    vx_bool    nonmax_supression    If true, non-maximum suppression is applied to detected corners (keypoints)
     * \param [out]   vx_list    corners              Output corner list (<tt>\ref vx_list</tt> of <tt>\ref vx_keypoint</tt>)
     * \see group_kernel_fast
     */
    VX_KERNEL_FAST_CORNERS,

    /*! \brief The Optical Flow Pyramid (LK) Kernel.
     * \see group_kernel_opticalflowpyrlk
     */
    VX_KERNEL_OPTICAL_FLOW_PYR_LK,

    /* insert new kernels here */

    VX_KERNEL_MAX_1_0, /*!< \internal Used for bounds checking in the conformance test. */
};

#ifdef  __cplusplus
}
#endif

#endif  /* _OPEN_VISION_LIBRARY_KERNELS_H_ */
