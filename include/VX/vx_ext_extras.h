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

#ifndef _VX_EXT_EXTRAS_H_
#define	_VX_EXT_EXTRAS_H_

/*! \file
 * \brief Extras Extension.
 * \author Erik Rainey <erik.rainey@ti.com>
 *
 * \defgroup group_extras_ext Khronos Extras Extension.
 * \brief A Set of Kernels which extend OpenVX.
 *
 * \defgroup group_kernel_nonmaxsuppression Kernel: Non-Maxima Suppression
 * \brief Suppresses Non-maximum values in a 3x3 block around the pixel.
 *
 * \defgroup group_kernel_laplacian_image Kernel: Laplacian Filter
 * \brief Computes a Laplacian filter over a window of the input image.
 * \details This filter uses the follow convolution matrix:
 \f[
 \mathbf{K}_{gaussian} = \begin{vmatrix}
   1 & 1 & 1\\
   1 &-8 & 1\\
   1 & 1 & 1
 \end{vmatrix} \times \frac{1}{1}
 \f]
 *
 * \defgroup group_kernel_scharr3x3 Kernel: Sobel 3x3
 * \brief The Scharr Image Filter Kernel
 * \details This kernel produces two output planes (one can be omitted)
 * in the x and y plane. The Scharr Operators \f$G_x, G_y\f$ are defined as:
   \f[
        \mathbf{G}_x=\begin{vmatrix}
        -3 & 0 & +3\\
        -10& 0 & +10\\
        -3 & 0 & +3
        \end{vmatrix}
        ,
        \mathbf{G}_y=\begin{vmatrix}
        -3 & -10 & -3 \\
         0 &  0 & 0  \\
        +3 & +10 & +3
        \end{vmatrix}

    \f]
 *
 */

/*! \brief The Khronos Extras Library
 * \ingroup group_extras_ext
 */
#define VX_LIBRARY_KHR_EXTRAS (0x2)

/*! \brief The Khronos Extras Kernels.
 * \ingroup group_extras_ext
 */
enum vx_kernel_extras_ext_e {
    /*! \brief The Non-Maximum Supression Kernel.
     * \note Use "org.khronos.extra.nonmaximasuppression" to \ref vxGetKernelByName.
     * \param [in] vx_image The magnitude image in FOURCC_U8.
     * \param [in] vx_image The phase image in FOURCC_U8.
     * \param [out] vx_image The edge image in FOURCC_U8.
     * \ingroup group_kernel_nonmaxsuppression
     */
    VX_KERNEL_EXTRAS_NONMAXSUPPRESSION = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_EXTRAS) + 0x0,

    /*! \brief The laplacian filter kernel.
     * \note Use "org.khronos.extras.laplacian3x3" to \ref vxGetKernelByName.
     * \param [in] vx_image The FOURCC_U8 input image.
     * \param [out] vx_image The FOURCC_U8 output image.
     * \see group_kernel_laplacian_image
     */
    VX_KERNEL_EXTRAS_LAPLACIAN_3x3 = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_EXTRAS) + 0x1,

    /*! \brief The scharr filter kernel.
     * \note Use "org.khronos.extras.scharr3x3" to \ref vxGetKernelByName.
     * \param [in] vx_image The FOURCC_U8 input image.
     * \param [out] vx_image The FOURCC_S16 output gradient x image.
     * \param [out] vx_image The FOURCC_S16 output gradient y image.
     * \see group_kernel_scharr3x3
     */
    VX_KERNEL_EXTRAS_SCHARR_3x3 = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_EXTRAS) + 0x2,

    /*! \brief The Harris Score Kernel.
     * \note use "org.khronos.extras.harris_score".
     * \param [in] vx_image A FOURCC_S16 X Gradient
     * \param [in] vx_image A FOURCC_S16 Y Gradient
     * \param [in] vx_scalar A block size.
     * \param [out] vx_image A FOURCC_S32 corner score per pixel.
     */
    VX_KERNEL_EXTRAS_HARRIS_SCORE = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_EXTRAS) + 0x3,

    /*! \brief The Sobel MxN kernel.
     * \note Use "org.khronos.extras.sobelMxN" to \ref vxGetKernelByName.
     * \param [in] vx_image The FOURCC_U8 input image.
     * \param [in] vx_scalar Window Size (3,5,7)
     * \param [out] vx_image The FOURCC_S16 output gradient x image.
     * \param [out] vx_image The FOURCC_S16 output gradient y image.
     * \see group_kernel_sobelmxn
     */
    VX_KERNEL_EXTRAS_SOBEL_MxN = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_EXTRAS) + 0x4,

    /*! \brief The image to list converter.
     * \param [in] vx_image The FOURCC_U8 or FOURCC_S32 image.
     * \param [out] vx_list The list of output
     */
    VX_KERNEL_EXTRAS_IMAGE_LISTER = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_EXTRAS) + 0x5,

    /*! \brief The Euclidean Non-Maximum Suppression Kernel.
     * \param [in] vx_image The FOURCC_S32 image.
     * \param [in] vx_scalar The minimum threshold
     * \param [in] vx_scalar The euclidean distance from the considered pixel.
     * \param [out] vx_image The FOURCC_S32 image.
     */
    VX_KERNEL_EXTRAS_EUCLIDEAN_NONMAXSUPPRESSION = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_EXTRAS) + 0x6,
};

#ifdef	__cplusplus
extern "C" {
#endif

/*! \brief [Graph] Creates a Laplacian Filter Node.
 * \param [in] graph The handle to the graph.
 * \param [in] input The input image in FOURCC_U8 format.
 * \param [out] output The output image in FOURCC_U8 format.
 * \ingroup group_kernel_laplacian_image
 */
vx_node vxNonMaxSuppressionNode(vx_graph graph, vx_image mag, vx_image phase, vx_image edge);

/*! \brief [Immediate] Creates a Laplacian Filter Node.
 * \param [in] graph The handle to the graph.
 * \param [in] input The input image in FOURCC_U8 format.
 * \param [out] output The output image in FOURCC_U8 format.
 * \ingroup group_kernel_laplacian_image
 */
vx_status vxuNonMaxSuppression(vx_image mag, vx_image phase, vx_image edge);

/*! \brief [Graph] Creates a Laplacian Filter Node.
 * \param [in] graph The handle to the graph.
 * \param [in] input The input image in FOURCC_U8 format.
 * \param [out] output The output image in FOURCC_U8 format.
 * \ingroup group_kernel_laplacian_image
 */
vx_node vxLaplacian3x3Node(vx_graph graph, vx_image input, vx_image output);

/*! \brief [Immediate] Computes a laplacian filter on the image by a 3x3 window.
 * \param [in] input The input image in FOURCC_U8 format.
 * \param [out] output The output image in FOURCC_U8 format.
 * \ingroup group_kernel_laplacian_image
 */
vx_status vxuLaplacian3x3(vx_image input, vx_image output);

/*! \brief [Graph] Creates a Scharr Filter Node.
 * \param [in] graph The handle to the graph.
 * \param [in] input The input image in FOURCC_U8 format.
 * \param [out] output The output image in FOURCC_U8 format.
 * \ingroup group_kernel_laplacian_image
 */
vx_node vxScharr3x3Node(vx_graph graph, vx_image input, vx_image output);

/*! \brief [Immediate] Computes a Scharr filter on the image by a 3x3 window.
 * \param [in] input The input image in FOURCC_U8 format.
 * \param [out] output The output image in FOURCC_U8 format.
 * \ingroup group_kernel_laplacian_image
 */
vx_status vxuScharr3x3(vx_image input, vx_image output);

vx_node vxSobelMxNNode(vx_graph graph, vx_image input, vx_scalar win, vx_image gx, vx_image gy);

vx_status vxuSobelMxN(vx_image input, vx_scalar win, vx_image gx, vx_image gy);

vx_node vxHarrisScoreNode(vx_graph graph,
                         vx_image gx,
                         vx_image gy,
                         vx_scalar sensitivity,
                         vx_scalar block_size,
                         vx_list corners);

vx_status vxuHarrisScore(vx_image gx,
                         vx_image gy,
                         vx_scalar sensitivity,
                         vx_scalar block_size,
                         vx_list corners);

vx_node vxEuclideanNonMaxNode(vx_graph graph,
                              vx_image input,
                              vx_scalar strength_thresh,
                              vx_scalar min_distance,
                              vx_image output);

vx_status vxuEuclideanNonMax(vx_image input,
                             vx_scalar strength_thresh,
                             vx_scalar min_distance,
                             vx_image output);

vx_node vxImageListerNode(vx_graph graph, vx_image input, vx_list list);

vx_status vxuImageLister(vx_image input,
                         vx_list list);

#ifdef	__cplusplus
}
#endif

#endif	/* _VX_EXT_EXTRAS_H_ */

