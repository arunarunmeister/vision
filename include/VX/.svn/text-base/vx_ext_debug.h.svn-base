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

#ifndef _OPENVX_EXT_DEBUG_H_
#define _OPENVX_EXT_DEBUG_H_

#include <VX/vx.h>

/*!
 * \file
 * \brief The OpenVX Debugging Extension.
 * \author Erik Rainey <erik.rainey@ti.com>
 * \defgroup group_debug_ext Debugging Extension
 * \defgroup group_kernel_copy_image Kernel: Copy Image
 * \defgroup group_kernel_copy_buffer Kernel: Copy Buffer
 * \defgroup group_kernel_fwrite_image Kernel: File Write Image
 * \defgroup group_kernel_fwrite_buffer Kernel: File Write Buffer
 * \defgroup group_kernel_plus1 Kernel: Plus One Image
 * \defgroup group_kernel_fill_image Kernel: Fill Image
 * \defgroup group_kernel_check_image Kernel: Check Image
 * \defgroup group_kernel_fill_buffer Kernel: Fill Buffer
 * \defgroup group_kernel_check_buffer Kernel: Check Buffer
 * \defgroup group_kernel_compare_images Kernel: Comapare Images
 */

/*! \brief The maximum filepath name length.
 * \ingroup group_debug_ext
 */
#define VX_MAX_FILE_NAME    (256)

/*! \brief The library value for the extension
 * \ingroup group_debug_ext
 */
#define VX_LIBRARY_KHR_DEBUG (0x1)

/*! \brief The list of extensions to OpenVX from the Sample Implementation.
 * \ingroup group_debug_ext
 */
enum vx_kernel_debug_ext_e {

    /*!
     * \brief The Copy kernel. Output = Input.
     * \param  [in] vx_image The input image.
     * \param [out] vx_image The output image.
     * \see group_kernel_copy_image
     */
    VX_KERNEL_DEBUG_COPY_IMAGE = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_DEBUG) + 0x0,

    /*!
     * \brief The Copy Kernel, Output = Input.
     * \param [in] vx_buffer The input buffer.
     * \param [out] vx_buffer The output buffer.
     * \see group_kernel_copy_buffer
     */
     VX_KERNEL_DEBUG_COPY_BUFFER = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_DEBUG) + 0x1,

    /*!
     * \brief The File Writing Kernel for Images.
     * \param [in] vx_image The input image.
     * \param [in] vx_buffer The name of the file.
     * \see group_kernel_fwrite_image
     */
    VX_KERNEL_DEBUG_FWRITE_IMAGE = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_DEBUG) + 0x2,

    /*!
     * \brief The File Writing Kernel for Buffers
     * \param [in] vx_buffer The input buffer.
     * \param [in] vx_buffer The name of the file.
     * \see group_kernel_fwrite_buffer
     */
     VX_KERNEL_DEBUG_FWRITE_BUFFER = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_DEBUG) + 0x3,

     /*!
      * \brief The File Reading Kernel for images.
      * \param [in] vx_buffer The name of the file to read.
      * \param [out] vx_image The output image.
      * \see group_kernel_fread_image
      */
     VX_KERNEL_DEBUG_FREAD_IMAGE = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_DEBUG) + 0x4,

     /*!
      * \brief The File Reading Kernel for Buffers.
      * \param [in] vx_buffer The name of the file to read.
      * \param [out] vx_image The output image.
      * \see group_kernel_fread_buffer
      */
     VX_KERNEL_DEBUG_FREAD_BUFFER = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_DEBUG) + 0x5,

     /*!
      * \brief Fills the image with a given value.
      * \param [in] vx_uint32
      * \param [out] vx_image
      */
     VX_KERNEL_FILL_IMAGE = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_DEBUG) + 0x6,

     /*!
      * \brief Checks an image against a known value and returns a number of
      * errors.
      * \param [in] vx_image
      * \param [in] vx_uint32
      * \param [out] vx_scalar
      */
     VX_KERNEL_CHECK_IMAGE = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_DEBUG) + 0x7,

     /*!
      * \brief Fills the buffer with a given value.
      * \param [in] vx_uint8
      * \param [out] vx_buffer
      */
     VX_KERNEL_FILL_BUFFER = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_DEBUG) + 0x8,

     /*!
      * \brief Checks a buffer against a known value and returns a number of
      * errors.
      * \param [in] vx_buffer
      * \param [in] vx_uint8
      * \param [out] vx_scalar
      */
     VX_KERNEL_CHECK_BUFFER = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_DEBUG) + 0x9,

     /*!
      * \brief Compares two images and returns the number of differences.
      * \param [in] vx_image
      * \param [in] vx_image
      * \param [out] vx_scalar
      */
     VX_KERNEL_COMPARE_IMAGE = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_DEBUG) + 0xA,

     /*!
      * \brief Copies an image from a memory area.
      * \param [in] void *
      * \param [out] vx_image
      * \see group_kernel_copy_ptr
      */
     VX_KERNEL_COPY_IMAGE_FROM_PTR = VX_KERNEL_BASE(VX_ID_KHRONOS, VX_LIBRARY_KHR_DEBUG) + 0xB,
};

/******************************************************************************/
// GRAPH MODE FUNCTIONS
/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
/*!
 * \brief [Graph] Creates a Copy Image Node.
 * \param [in] graph The handle to the graph.
 * \param [in] input The input image.
 * \param [out] output The output image.
 * \see VX_KERNEL_COPY_IMAGE
 * \note Graph Mode Function.
 * \ingroup group_kernel_copy_image
 */
vx_node vxCopyImageNode(vx_graph graph, vx_image input, vx_image output);

/*!
 * \brief [Graph] Creates a Copy Buffer Node.
 * \param [in] graph The handle to the graph.
 * \param [in] input The input buffer.
 * \param [out] output The output buffer.
 * \see VX_KERNEL_COPY_BUFFER
 * \note Graph Mode Function.
 * \ingroup group_kernel_copy_buffer
 */
vx_node vxCopyBufferNode(vx_graph graph, vx_buffer input, vx_buffer output);

/*! \brief [Graph] Writes the source image to the file.
 * \param [in] graph The handle to the graph.
 * \param [in] image The input buffer.
 * \param [in] name The name of the file.
 * \note Graph Mode Function.
 * \ingroup group_kernel_fwrite_image
 */
vx_node vxFWriteImageNode(vx_graph graph, vx_image image, vx_char name[VX_MAX_FILE_NAME]);

/*! \brief [Graph] Writes the source buffer to the file.
 * \param [in] graph The handle to the graph.
 * \param [in] buffer The input buffer.
 * \param [in] name The name of the file.
 * \note Graph Mode Function.
 * \ingroup group_kernel_fwrite_buffer
 */
vx_node vxFWriteBufferNode(vx_graph graph, vx_buffer buffer, vx_char name[VX_MAX_FILE_NAME]);

/*! \brief [Graph] Writes the source image to the file.
 * \param [in] graph The handle to the graph.
 * \param [in] name The name of the file.
 * \param [out] image The output image.
 * \note Graph Mode Function.
 * \ingroup group_kernel_fread_image
 */
vx_node vxFReadImageNode(vx_graph graph, vx_char name[VX_MAX_FILE_NAME], vx_image image);

/*! \brief [Graph] Writes the source buffer to the file.
 * \param [in] graph The handle to the graph.
 * \param [in] name The name of the file.
 * \param [out] buffer The output buffer.
 * \note Graph Mode Function.
 * \ingroup group_kernel_fread_buffer
 */
vx_node vxFReadBufferNode(vx_graph graph, vx_char name[VX_MAX_FILE_NAME], vx_buffer buffer);

/*! \brief [Graph] Adds 1 to each uint8 pixel. This will clamp at 255.
 * \param [in] graph The handle to the graph.
 * \param [in,out] image The image to increment.
 * \note Graph Mode Function
 * \ingroup group_kernel_plus1
 */
vx_node vxPlusOneNode(vx_graph graph, vx_image image);

/*!
 * \brief [Graph] Fills an image with a known value.
 * \param [in] graph The handle to the graph.
 * \param [in] value The known value to fill the image with.
 * \param [out] output The image to fill.
 * \note Graph Mode Function
 * \ingroup group_kernel_fill_image
 */
vx_node vxFillImageNode(vx_graph graph, vx_uint32 value, vx_image output);

/*!
 * \brief [Graph] Checks an image against a known value.
 * \param [in] graph The handle to the graph.
 * \param [in] input The image to check.
 * \param [in] value The known value to check the image against.
 * \param [out] errs The handle to the number of errors found.
 * \note Graph Mode Function
 * \ingroup group_kernel_check_image
 */
vx_node vxCheckImageNode(vx_graph graph, vx_image input, vx_uint32 value, vx_scalar errs);

/*!
 * \brief [Graph] Fills an buffer with a known value.
 * \param [in] graph The handle to the graph.
 * \param [in] value The known value to fill the buffer with.
 * \param [out] output The buffer to fill.
 * \note Graph Mode Function
 * \ingroup group_kernel_fill_buffer
 */
vx_node vxFillBufferNode(vx_graph graph, vx_uint8 value, vx_buffer output);

/*!
 * \brief [Graph] Checks a buffer for a known value.
 * \param [in] graph The handle to the graph.
 * \param [in] input The buffer to check.
 * \param [in] value The known value to check against.
 * \param [out] errs An output of the number of errors.
 * \note Graph Mode Function
 * \ingroup group_kernel_check_buffer
 */
vx_node vxCheckBufferNode(vx_graph graph, vx_image input, vx_uint8 value, vx_scalar errs);

/*!
 * \brief [Graph] Compares two images and returns the number of pixel sub-channels
 * which are different.
 * \param [in] graph The handle to the graph.
 * \param [in] a The first image.
 * \param [in] b The second image.
 * \param [out] diffs The handle to scalar to hold the number of differences.
 * \note Graph Mode Function
 * \ingroup group_kernel_compare_image
 */
vx_node vxCompareImagesNode(vx_graph graph, vx_image a, vx_image b, vx_scalar diffs);

/*! \brief [Graph] Copies a HOST memory area into an image.
 * \param [in] graph The handle to the graph.
 * \param [in] ptr The input pointer to the memory area to copy.
 * \param [out] output The output image.
 * \note Graph Mode Function
 * \ingroup group_kernel_copy_ptr
 */
vx_node vxCopyImageFromPtrNode(vx_graph graph, void *ptr, vx_image output);

/******************************************************************************/
// IMMEDIATE MODE FUNCTION
/******************************************************************************/

/*! \brief [Immediate] Copies the source image to the destination image.
 * \param [in] src The input image.
 * \param [in] dst The output image.
 * \note Immediate Mode Function.
 * \ingroup group_kernel_copy_image
 */
vx_status vxuCopyImage(vx_image src, vx_image dst);

/*! \brief [Immediate] Copies the source buffer to the destination buffer.
 * \param [in] src The input buffer.
 * \param [in] dst The output buffer.
 * \note Immediate Mode Function.
 * \ingroup group_kernel_copy_buffer
 */
vx_status vxuCopyBuffer(vx_buffer src, vx_buffer dst);

/*! \brief [Immediate] Writes the source image to the file.
 * \param [in] image The input buffer.
 * \param [in] name The name of the file.
 * \note Immediate Mode Function.
 * \ingroup group_kernel_fwrite_image
 */
vx_status vxuFWriteImage(vx_image image, vx_char name[VX_MAX_FILE_NAME]);

/*! \brief [Immediate] Writes the source buffer to the file.
 * \param [in] buffer The input buffer.
 * \param [in] name The name of the file.
 * \note Immediate Mode Function.
 * \ingroup group_kernel_fwrite_buffer
 */
vx_status vxuFWriteBuffer(vx_buffer buffer, vx_char name[VX_MAX_FILE_NAME]);

/*! \brief [Immediate] Reads the source image from the file.
 * \param [in] name The name of the file.
 * \param [out] image The output image.
  * \note Immediate Mode Function.
 * \ingroup group_kernel_fread_image
 */
vx_status vxuFReadImage(vx_char name[VX_MAX_FILE_NAME], vx_image image);

/*! \brief [Immediate] Reads the source buffer from the file.
 * \param [in] name The name of the file.
 * \param [out] buffer The output buffer.
 * \note Immediate Mode Function.
 * \ingroup group_kernel_fread_buffer
 */
vx_status vxuFReadBuffer(vx_char name[VX_MAX_FILE_NAME], vx_buffer buffer);

/*! \brief [Immediate] Adds 1 to each uint8 pixel. This will clamp at 255.
 * \param [in,out] image The image to increment.
 * \note Immediate Mode Function
 * \ingroup group_kernel_plus1
 */
vx_node vxuPlusOneNode(vx_image image);

/*!
 * \brief [Immediate] Fills an image with a known value.
 * \param [in] value The known value to fill the image with.
 * \param [out] output The image to fill.
 * \note Immediate Mode Function
 * \ingroup group_kernel_fill_image
 */
vx_status vxuFillImage(vx_uint32 value, vx_image output);

/*!
 * \brief [Immediate] Checks an image against a known value.
 * \param [in] output The image to check.
 * \param [in] value The known value to check the image against.
 * \param [out] numErrors The handle to the number of errors found.
 * \note Immediate Mode Function
 * \ingroup group_kernel_check_image
 */
vx_status vxuCheckImage(vx_image input, vx_uint32 value, vx_uint32 *numErrors);

/*!
 * \brief [Immediate] Fills an buffer with a known value.
 * \param [in] value The known value to fill the buffer with.
 * \param [out] output The buffer to fill.
 * \note Immediate Mode Function
 * \ingroup group_kernel_fill_buffer
 */
vx_status vxuFillBuffer(vx_uint8 value, vx_buffer output);

/*!
 * \brief [Immediate] Checks a buffer for a known value.
 * \param [in] input The buffer to check.
 * \param [in] value The known value to check against.
 * \param [out] numErrors An output of the number of errors.
 * \note Immediate Mode Function
 * \ingroup group_kernel_check_buffer
 */
vx_status vxuCheckBuffer(vx_buffer input, vx_uint8 value, vx_uint32 *numErrors);

/*!
 * \brief [Immediate] Compares two images and returns the number of pixel sub-channels
 * which are different.
 * \param [in] a The first image.
 * \param [in] b The second image.
 * \param [out] numDiffs The handle to scalar to hold the number of differences.
 * \note Immediate Mode Function
 * \ingroup group_kernel_compare_image
 */
vx_status vxuCompareImages(vx_image a, vx_image b, vx_uint32 *numDiffs);

/*! \brief [Immediate] Copies a HOST memory area into an image.
 * \param [in] ptr The input pointer to the memory area to copy.
 * \param [out] output The output image.
 * \note Immediate Mode Function
 * \ingroup group_kernel_copy_ptr
 */
vx_status vxuCopyImageFromPtr(void *ptr, vx_image output);

#ifdef __cplusplus
}
#endif

#endif

