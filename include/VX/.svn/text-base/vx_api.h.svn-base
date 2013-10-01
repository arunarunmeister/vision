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

#ifndef _OPENVX_API_H_
#define _OPENVX_API_H_

/*!
 * \file
 * \brief The API definition for OpenVX.
 * \author Erik Rainey <erik.rainey@ti.com>
 */

/*!
 * \defgroup group_objects OpenVX Objects
 * \brief The objects within OpenVX.
 * \details All objects in OpenVX derive from a <tt>\ref vx_reference</tt> and
 * contain a reference to the <tt>\ref vx_context</tt> from which they were made, except
 * the <tt>\ref vx_context</tt> itself.
 *
 * \defgroup group_framework OpenVX Framework
 * \brief The framework concepts and interfaces of OpenVX.
 * \details These interfaces are used to perform tasks such as performance
 * measurement on <tt>\ref vx_graph</tt> or <tt>\ref vx_node</tt> objects, obtain debugging information,
 * or set directives and hints to the implementation. While they all affect the
 * <tt>\ref vx_context</tt>, they are not considered direct methods of the context.
 *
 * \defgroup group_context Object: Context
 * \ingroup group_objects
 * \brief The Context Object Interface.
 * \details The OpenVX Context contains all other objects in OpenVX.
 *
 * \defgroup group_image Object: Image
 * \ingroup group_objects
 * \brief The Image Object interface.
 *
 * \defgroup group_buffer Object: Buffer
 * \ingroup group_objects
 * \brief The Buffer Object interface.
 *
 * \defgroup group_kernel Object: Kernel
 * \ingroup group_objects
 * \brief The list of supported kernels and the Kernel Object interface.
 *
 * \defgroup group_node Object: Node
 * \ingroup group_objects
 * \brief The Node Object interface.
 *
 * \defgroup group_graph Object: Graph
 * \ingroup group_objects
 * \brief The Graph Object interface.
 *
 * \defgroup group_parameter Object: Parameter
 * \ingroup group_objects
 * \brief The Parameter Object interface.
 *
 * \defgroup group_reference Object: Reference
 * \ingroup group_objects
 * \brief The Reference Object interface.
 *
 * \defgroup group_pyramid Object: Pyramid
 * \ingroup group_objects
 * \brief The Image Pyramid Object Interface.
 * \details A Pyramid object in OpenVX represents a collection of
 * related images.  Typically, these images are created by either
 * downscaling or upscaling a \e base \e image, contained in level zero
 * of the pyramid.  Successive levels of the pyramid increase or
 * decrease in size by a factor given by the <tt>\ref VX_PYRAMID_ATTRIBUTE_SCALE</tt> attribute.
 * For instance, in a pyramid with 3 levels and \ref VX_SCALE_PYRAMID_HALF,
 * the level one image will be one-half the width and one-half the
 * height of the level zero image and the level two image will be
 * one-quarter the width and one quarter the height of the level zero
 * image.  When downscaling or upscaling results in a non-integral
 * number of pixels at any level, fractional pixels are always rounded
 * up to the nearest integer.  (e.g., a 3 level image pyramid beginning with
 * level zero having a width of 9 and a scaling of <tt>\ref VX_SCALE_PYRAMID_HALF</tt>
 * will result in the level one image with a width of 5 = \f$ \mathbf{ceil}(9*0.5) \f$
 * and a level two image with a width of 3 = \f$ \mathbf{ceil}(5*0.5) \f$.  Position
 * \f$ (r_N,c_N) \f$ at level N corresponds to position
 * \f$ (r_{N-1}/\mathbf{scale}, c_{N-1}/\mathbf{scale}) \f$ at level N-1.
 *
 * \defgroup group_scalar Object: Scalar
 * \ingroup group_objects
 * \brief The Scalar Object interface.
 *
 * \defgroup group_delay Object: Delay
 * \ingroup group_objects
 * \brief The Delay Object interface.
 *
 * \defgroup group_target Object: Target
 * \ingroup group_objects
 * \brief The Target Object interface.
 * \details In OpenVX a Target is any configuration of devices or hardware which
 * can execute at least one vx_kernel.
 *
 * \defgroup group_log Framework: Log
 * \ingroup group_framework
 * \brief The debug logging interface.
 *
 * \defgroup group_rectangle Object: Rectangle
 * \ingroup group_objects
 * \brief The Rectangle Object interface.
 *
 * \defgroup group_coordinates Object: Coordinates
 * \ingroup group_objects
 * \brief The Image Pixel Coordinates Object interface.
 *
 * \defgroup group_convolution Object: Convolution
 * \ingroup group_objects
 * \brief The Image Convolution Object interface.
 *
 * \defgroup group_distribution Object: Distribution
 * \ingroup group_objects
 * \brief The Distribution Object Interface.
 *
 * \defgroup group_matrix Object: Matrix
 * \ingroup group_objects
 * \brief The Matrix Object Interface.
 *
 * \defgroup group_threshold Object: Threshold
 * \ingroup group_objects
 * \brief The Threshold Object Interface.
 *
 * \defgroup group_keypoint Object: Keypoint
 * \ingroup group_objects
 * \brief The Keypoint Object Interface.
 * \details Keypoints are frequently the output of corner detectors and the
 * inputs of trackers.
 *
 * \defgroup group_lut Object: LUT
 * \ingroup group_objects
 * \brief The Look-Up Table Interface.
 * \details A lookup table is an array that simplifies runtime computation
 * by replacing computation with a simpler array indexing operation.
 *
 * \defgroup group_list Object: List
 * \ingroup group_objects
 * \brief The List Object Interface
 * \details A list is a generic, iterable storage structure for other data objects.
 * \if OPENVX_STRICT_1_0
 * For OpenVX 1.0 <tt>\ref vx_list</tt> should only a constructed with <tt>\ref VX_TYPE_COORDINATES</tt> or <tt>\ref VX_TYPE_RECTANGLE</tt> or \ref VX_TYPE_KEYPOINT.
 * \endif
 * This is an example "for" loop over a list in the "forwards" direction
 * \code
 * vx_reference r = 0;
 * for (r  = vxGetListItem(list, VX_LIST_FRONT);
 *      r != 0;
 *      r  = vxGetListItem(list, VX_LIST_NEXT))
 * {
 *     // cast the "r" to another vx_<type> and do something
 * }
 * \endcode
 * This is an example "for" loop over a list in the "backwards" direction
 * \code
 * vx_reference r = 0;
 * for (r  = vxGetListItem(list, VX_LIST_BACK);
 *      r != 0;
 *      r  = vxGetListItem(list, VX_LIST_PREVIOUS))
 * {
 *     // cast the "r" to another vx_<type> and do something
 * }
 * \endcode
 *
 * \defgroup group_remap Object: Remap
 * \ingroup group_objects
 * \brief The Remap Object Interface.
 *
 * \defgroup group_hint Framework: Hint
 * \ingroup group_framework
 * \brief The Hints Interface.
 * \details <i>Hints</i> are messages given to the OpenVX implementation
 * which are optional.
 *
 * \defgroup group_directive Framework: Directive
 * \ingroup group_framework
 * \brief The Directives Interface.
 * \details <i>Directives</i> are messages given the OpenVX implementation
 * which are non-optional.
 *
 * \defgroup group_user_nodes Framework: Users Supplied Nodes
 * \ingroup group_framework
 * \brief User Extensions are a method to extend OpenVX with User defined kernels which can be loaded by OpenVX and included as nodes in the graph or immediate functions.
 * \details User Extensions can be loaded and executed on HLOS/CPU compatible platforms.
 * This specification does not mandate what constitutes compatible platforms.
 *
 * \defgroup group_performance Framework: Performance Measurement
 * \ingroup group_framework
 * \brief The Performance measurement and reporting interfaces.
 * \details In OpenVX, both <tt>\ref vx_graph</tt> objects and <tt>\ref vx_node</tt> objects
 * track performance information. A user can query either object type using their
 * respective <tt>vxQuery&lt;Object&gt;</tt> function with their attribute enumeration
 * <tt>VX_&lt;OBJECT&gt;_ATTRIBUTE_PERFORMANCE</tt> along with a <tt>\ref vx_perf_t</tt>
 * structure to obtain the performance information.
 *
 */

#ifdef  __cplusplus
extern "C" {
#endif

/*! \brief Creates a <tt>\ref vx_context</tt>
 * \details This creates a top level object context for OpenVX.
 * \note This is required to do anything else.
 * \returns The reference to the implementation context.
 * \retval 0 No context was created.
 * \retval * A context reference.
 * \ingroup group_context
 * \post <tt>\ref vxReleaseContext</tt>
 */
vx_context vxCreateContext();

/*! \brief Releases the OpenVX object context.
 * \details All reference counted objects are garbage collected by the return of this call.
 * No calls are possible using the parameter context after the context has been
 * released until a new reference from <tt>\ref vxCreateContext</tt> is returned.
 * All outstanding references to OpenVX objects from this context are invalid
 * after this call.
 * \param [in] context The pointer to the reference to the context.
 * \note After returning from this function the reference will be zeroed.
 * \ingroup group_context
 * \pre <tt>\ref vxCreateContext</tt>
 */
void vxReleaseContext(vx_context *context);

/*! \brief Retrieves the context from any reference from within a context.
 * \param [in] reference The reference to the extract the context from.
 * \ingroup group_context
 * \return Returns the overall context which created the particular
 * reference.
 */
vx_context vxGetContext(vx_reference reference);

/*! \brief Queries the context for some specific information.
 * \param [in] context The reference to the context.
 * \param [in] attr The attribute to query. Use a <tt>\ref vx_context_attribute_e</tt>.
 * \param [out] ptr The output pointer where the value will be sent.
 * \param [in] size The size of the objects pointed to by out.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS No errors
 * \retval VX_ERROR_INVALID_REFERENCE if the context is not a <tt>\ref vx_context</tt>.
 * \retval VX_ERROR_INVALID_PARAMETERS if any of the other parameters are incorrect.
 * \retval VX_ERROR_NOT_SUPPORTED if the attribute is not supported on this implementation.
 * \ingroup group_context
 * \pre <tt>\ref vxCreateContext</tt>
 * \post <tt>\ref vxReleaseContext</tt>
 */
vx_status vxQueryContext(vx_context context, vx_enum attr, void *ptr, vx_size size);

/*! \brief Creates an opaque reference to an image buffer.
 * \details Not guaranteed to exist until the <tt>\ref vx_graph</tt> containing it has been verified
 * \param [in] context The reference to the implementation context.
 * \param [in] width The image width in pixels.
 * \param [in] height The image height in pixels.
 * \param [in] color The FOURCC (<tt>\ref vx_fourcc_e</tt>) code which represents the format of the image and the color space.
 * \return Returns an image reference or zero when an error is encountered.
 * \see vxAccessImagePatch to obtain direct memory access to the image data.
 * \pre <tt>\ref vxCreateContext</tt>
 * \ingroup group_image
 */
vx_image vxCreateImage(vx_context context, vx_uint32 width, vx_uint32 height, vx_fourcc color);

/*! \brief Creates an image from another image given a rectangle. This second
 * reference refers to the data in the original image. Updates to this image will
 * update the parent image. The rectangle must be defined within the pixel space
 * of the parent image.
 * \param [in] img The reference to the parent image.
 * \param [in] rect The region of interest rectangle. Must contain points within
 * the parent image pixel space.
 * \pre <tt>\ref vxCreateImage</tt>
 * \return Returns the reference to the sub-image or zero if the rectangle was invalid.
 * \ingroup group_image
 */
vx_image vxCreateImageFromROI(vx_image img, vx_rectangle rect);

/*! \brief Creates an reference to an image object which has a singular,
 * uniform value in all pixels.
 * \details The value pointer must reflect the specific format of the desired
 * image. For example:
 * | Color       | Value Ptr  |
 * |:------------|:-----------|
 * | <tt>\ref FOURCC_U8</tt>   | vx_uint8 * |
 * | <tt>\ref FOURCC_S16</tt>  | vx_int16 * |
 * | <tt>\ref FOURCC_U16</tt>  | vx_uint16 *|
 * | <tt>\ref FOURCC_S32</tt>  | vx_int32 * |
 * | <tt>\ref FOURCC_U32</tt>  | vx_uint32 *|
 * | <tt>\ref FOURCC_RGB</tt>  | vx_uint8 pixel[3] in R, G, B order |
 * | <tt>\ref FOURCC_RGBX</tt> | vx_uint8 pixels[4] |
 * | Any YUV     | vx_uint8 pixel[3] in Y, U, V order |
 *
 * \param [in] context The reference to the implementation context.
 * \param [in] width The image width in pixels.
 * \param [in] height The image height in pixels.
 * \param [in] color The FOURCC (\ref vx_fourcc_e) code which represents the format of the image and the color space.
 * \param [in] value The pointer to the pixel value to set all pixels to.
 * \return Returns an image reference or zero when an error is encountered.
 * <tt>\see vxAccessImagePatch</tt> to obtain direct memory access to the image data.
 * \note <tt>\ref vxAccessImagePatch</tt> and <tt>\ref vxCommitImagePatch</tt> may be called with
 * a uniform image reference.
 * \pre <tt>\ref vxCreateContext</tt>
 * \ingroup group_image
 */
vx_image vxCreateUniformImage(vx_context context, vx_uint32 width, vx_uint32 height, vx_fourcc color, void *value);

/*! \brief Creates an opaque reference to an image buffer with no direct
 * user access however allows setting the image format.
 * \details Virtual data objects allow users to connect various nodes within a
 * graph via data references within access to that data but they also permit the
 * implementation to take maximum advantage of possible optimizations. Use this
 * API to create a data reference to link two or more nodes together when the
 * intermediate data is not required to be accessed by outside entities. This API
 * in particular allows the user to define the image format of the data without
 * specifying the exact dimensions.
 * \param [in] context The reference to the implementation context.
 * \param [in] color The FOURCC (<tt>\ref vx_fourcc_e</tt>) code which represents the format of the image and the color space.
 * \return Returns an image reference or zero when an error is encountered.
 * \pre <tt>\ref vxCreateContext</tt>
 * \post <tt>\ref vxReleaseImage</tt>
 * \ingroup group_image
 */
vx_image vxCreateVirtualImageWithFormat(vx_context context, vx_fourcc color);

/*! \brief Creates an opaque reference to an image buffer with no direct
 * user access.
 * \details Virtual data objects allow users to connect various nodes within a
 * graph via data references within access to that data but they also permit the
 * implementation to take maximum advantage of possible optimizations. Use this
 * API to create a data reference to link two or more nodes together when the
 * intermediate data is not required to be accessed by outside entities.
 * \param [in] context The reference to the implementation context.
 * \return Returns an image reference or zero when an error is encountered.
 * \pre <tt>\ref vxCreateContext</tt>
 * \post <tt>\ref vxReleaseImage</tt>
 * \ingroup group_image
 */
vx_image vxCreateVirtualImage(vx_context context);

/*! \brief Creates a reference to an image object which was externally allocated.
 * \param [in] context The reference to the implementation context.
 * \param [in] color See the <tt>\ref vx_fourcc_e</tt> codes. This mandates the
 * number planes needed to be valid in the addrs and ptrs arrays based on the format given.
 * \param [in] addrs[] The array of image patch addressing structures which
 * defines the dimension and stride of the array of pointers.
 * \param [in] ptrs[] The array of platform defined references to each plane.
 * \param [in] type <tt>\ref vx_import_type_e</tt>. When giving <tt>\ref VX_IMPORT_TYPE_HOST</tt>
 * the ptrs[] is assumed to be HOST accessible pointers to memory.
 * \return Returns <tt>\ref vx_image</tt>.
 * \retval 0 Image could not be created.
 * \retval * Valid Image reference.
 * \pre <tt>\ref vxCreateContext</tt>
 * \post <tt>\ref vxReleaseImage</tt>
 * \ingroup group_image
 */
vx_image vxCreateImageFromHandle(vx_context context, vx_fourcc color, vx_imagepatch_addressing_t addrs[], void *ptrs[], vx_enum type);

/*! \brief Retrieves various attributes of an image.
 * \param [in] image The reference to the image to query.
 * \param [in] attribute The attribute to query. Use a <tt>\ref vx_image_attribute_e</tt>.
 * \param [out] ptr The output pointer where the value will be sent.
 * \param [in] size The size of the objects pointed to by out.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS No errors
 * \retval VX_ERROR_INVALID_REFERENCE if the image is not a <tt>\ref vx_image</tt>.
 * \retval VX_ERROR_INVALID_PARAMETERS if any of the other parameters are incorrect.
 * \retval VX_ERROR_NOT_SUPPORTED if the attribute is not supported on this implementation.
 * \ingroup group_image
 * \pre <tt>\ref vxCreateImage</tt>
 */
vx_status vxQueryImage(vx_image image, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Allows setting attributes on the image.
 * \param [in] image The reference to the image to set the attribute on.
 * \param [in] attr The attribute to set. Use a <tt>\ref vx_image_attribute_e</tt> enumeration.
 * \param [in] out The pointer to the where the value will be read from.
 * \param [in] size The size of the object pointed to by out.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS No errors
 * \retval VX_ERROR_INVALID_REFERENCE if the image is not a <tt>\ref vx_image</tt>.
 * \retval VX_ERROR_INVALID_PARAMETERS if any of the other parameters are incorrect.
 * \ingroup group_image
 * \pre <tt>\ref vxCreateImage</tt>
 */
vx_status vxSetImageAttribute(vx_image image, vx_enum attr, void *out, vx_size size);

/*! \brief Releases a reference to an image object.
 * The object may not be garbage collected until its total reference count is zero..
 * \param [in] image The pointer to the image to release
 * \note After returning from this function the reference will be zeroed.
 * \ingroup group_image
 * \pre <tt>\ref vxCreateImage</tt>
 */
void vxReleaseImage(vx_image *image);

/*! \brief Creates an opaque reference to a buffer in memory
 * \param [in] context The reference to the implementation context.
 * \param [in] unitSize The size of the unit in bytes.
 * \param [in] numUnits The number of units in the buffer.
 * \return Returns a buffer reference.
 * \retval 0 when an error is encountered.
 * \ingroup group_buffer
 * \pre <tt>\ref vxCreateContext</tt>
 * \post <tt>\ref vxReleaseBuffer</tt>
 */
vx_buffer vxCreateBuffer(vx_context context, vx_size unitSize, vx_size numUnits);

/*! \brief Creates an opaque reference to a buffer in memory with no
 * direct user access.
 * \details Virtual Buffers are useful when buffer ranges or unit sizes are unknown
 * ahead of time.
 * \param [in] context The reference to the implementation context.
 * \return Returns an buffer reference
 * \retval 0 when an error is encountered.
 * \ingroup group_buffer
 * \pre <tt>\ref vxCreateContext</tt>
 * \post <tt>\ref vxReleaseBuffer</tt>
 */
vx_buffer vxCreateVirtualBuffer(vx_context context);

/*! \brief Creates a buffer object backed by externally allocated memory.
 * \param [in] context The reference to implementation context.
 * \param [in] unitSize The size of the units in bytes.
 * \param [in] numUnits The number of units in the buffer.
 * \param [in] ptr The handle to the external memory.
 * \param [in] type The <tt>\ref vx_import_type_e</tt>. When using <tt>\ref VX_IMPORT_TYPE_HOST</tt> the
 * ptr is assumed to be a pointer to the buffer in HOST memory.
 * \ingroup group_buffer
 * \return Returns <tt>\ref vx_buffer</tt>.
 * \retval 0 Buffer could not be created.
 * \retval * Valid buffer object reference.
 * \pre <tt>\ref vxCreateContext</tt>
 * \post <tt>\ref vxReleaseBuffer</tt>
 */
vx_buffer vxCreateBufferFromHandle(vx_context context, vx_size unitSize, vx_size numUnits, void *ptr, vx_enum type);

/*! \brief Queries the buffer for some specific information.
 * \param [in] buffer The reference to the buffer.
 * \param [in] attribute The attribute to query. Use a <tt>\ref vx_buffer_attribute_e</tt>.
 * \param [out] ptr The output pointer where the value will be sent.
 * \param [in] size The size of the objects pointed to by out.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS No errors
 * \retval VX_ERROR_INVALID_REFERENCE if the buffer is not a <tt>\ref vx_buffer</tt>.
 * \retval VX_ERROR_INVALID_PARAMETERS if any of the other parameters are incorrect.
 * \retval VX_ERROR_NOT_SUPPORTED if the attribute is not a value supported on
 * this implementation.
 * \ingroup group_buffer
 * \pre <tt>\ref vxCreateBuffer</tt> or <tt>\ref vxCreateVirtualBuffer</tt>
 * \post vxReleaseBuffer
 */
vx_status vxQueryBuffer(vx_buffer buffer, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Releases a reference to a buffer object. The object may not be garbage collected until its
 * total reference count is zero.
 * \param [in] buffer The pointer to a buffer to release.
 * \note After returning from this function the reference will be zeroed.
 * \ingroup group_buffer
 * \pre <tt>\ref vxCreateBuffer</tt>
 */
void vxReleaseBuffer(vx_buffer *buffer);

/*! \brief Loads one or more kernels into the OpenVX context. This is the interface
 * by which OpenVX is extensible. Once the set of kernels is loaded new kernels
 * and their parameters can be queried.
 * \note When all references to loaded kernels are released, the module
 * may be automatically unloaded.
 * \param [in] context The reference to the implementation context.
 * \param [in] module The short name of the module to load. On systems where
 * there are specific naming conventions for modules, the name passed
 * should ignore such conventions. For example: "libxyz.so" should be
 * passed as just "xyz" and the implementation will "do the right thing" that
 * the platform requires.
 * \note This API will use the system pre-defined paths for modules.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS No errors
 * \retval VX_ERROR_INVALID_REFERENCE if the context is not a <tt>\ref vx_context</tt>.
 * \retval VX_ERROR_INVALID_PARAMETERS if any of the other parameters are incorrect.
 * \ingroup group_user_nodes
 * \pre <tt>\ref vxCreateContext</tt>
 * \see vxGetKernelByName
 */
vx_status vxLoadKernels(vx_context context, vx_char *module);

/*! \brief This allows the user to query a kernel by name once the
 * kernel is loaded from a module.
 * \param [in] context The reference to the implementation context.
 * \param [in] name The string of the name of the kernel to get.
 * \return Returns a kernel reference or zero if an error occurred.
 * \retval 0 The kernel name was not found in the context.
 * \ingroup group_kernel
 * \see vxLoadKernels
 * \pre <tt>\ref vxCreateContext</tt>
 * \pre <tt>\ref vxLoadKernels</tt> (optional)
 * \post <tt>\ref vxReleaseKernel</tt>
 * \note User Kernels should follow a "dotted" heirarchical syntax. For example:
 * "org.khronos.example.xyz".
 */
vx_kernel vxGetKernelByName(vx_context context, vx_char *name);

/*! \brief This call uses the <tt>\ref vx_kernel_e</tt> to retrieve the kernels.
 * \details Enum values above the standard set are assumed to apply to
 * loaded modules.
 * \param [in] context The reference to the implementation context.
 * \param [in] kernel
 * \return Returns a kernel reference or zero if an error occurred.
 * \retval 0 The kernel enumeration was not found in the context.
 * \ingroup group_kernel
 * \pre <tt>\ref vxCreateContext</tt>
 * \pre <tt>\ref vxLoadKernels</tt> (optional)
 * \post <tt>\ref vxReleaseKernel</tt>
 * \see vxGetKernelByName
 */
vx_kernel vxGetKernelByEnum(vx_context context, vx_enum kernel);

/*! \brief This allows the client to query the kernel to get information about
 * the number of parameters, enum values, etc.
 * \param [in] kernel The kernel reference to query.
 * \param [in] attribute The attribute to query. Use a <tt>\ref vx_kernel_attribute_e</tt>.
 * \param [out] ptr The output pointer where the value will be sent.
 * \param [in] size The size of the objects pointed to by out.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS No errors
 * \retval VX_ERROR_INVALID_REFERENCE if the kernel is not a <tt>\ref vx_kernel</tt>.
 * \retval VX_ERROR_INVALID_PARAMETERS if any of the other parameters are incorrect.
 * \retval VX_ERROR_NOT_SUPPORTED if the attribute value is not supported in this implementation.
 * \ingroup group_kernel
 * \pre <tt>\ref vxGetKernelByName</tt> or <tt>\ref vxGetKernelByEnum</tt>
 */
vx_status vxQueryKernel(vx_kernel kernel, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Release the reference to the kernel.
 * The object may not be garbage collected until its total reference count is zero..
 * \param [in] kernel The pointer to the kernel reference to release.
 * \note After returning from this function the reference will be zeroed.
 * \ingroup group_kernel
 * \pre <tt>\ref vxGetKernelByName</tt> or <tt>\ref vxGetKernelByEnum</tt>
 */
void vxReleaseKernel(vx_kernel *kernel);

/*! \brief Creates an empty graph.
 * \param [in] context The reference to the implementation context.
 * \return Return a graph reference
 * \retval 0 if an error occurred.
 * \ingroup group_graph
 * \pre <tt>\ref vxCreateContext</tt>
 * \post <tt>\ref vxReleaseGraph</tt>
 */
vx_graph vxCreateGraph(vx_context context);

/*! \brief Releases a reference to a graph.
 * The object may not be garbage collected until its total reference count is zero..
 * Once the reference count is zero, all node references in the graph will be automatically
 * released as well. Data referenced by those nodes may not be released as
 * the user may have external references to the data.
 * \param [in] graph The pointer to the graph to release.
 * \note After returning from this function the reference will be zeroed.
 * \ingroup group_graph
 * \pre <tt>\ref vxCreateGraph</tt>
 */
void vxReleaseGraph(vx_graph *graph);

/*! \brief This call verifies the state of the graph before it is executed.
 * This is useful to catch programmer errors and contract errors. If not verified,
 * the graph will verify before being processed.
 * \note Memory for data objects is not guarenteed to exist before
 * this call. After this call data objects will exist unless
 * the implementation optimized them out.
 * \param [in] graph The reference to the graph to verify.
 * return Returns a status code for graphs with more than one error, it is
 * undefined which error will be returned. Use <tt>\ref vxGetLogEntry</tt> to retreive all
 * errors from the graph verification.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS No errors.
 * \retval VX_ERROR_INVALID_REFERENCE if graph is not a <tt>\ref vx_graph</tt>.
 * \retval VX_ERROR_MULTIPLE_WRITERS if the graph contains more than one writer
 * to any data object.
 * \retval VX_ERROR_INVALID_NODE if
 * \retval VX_ERROR_INVALID_GRAPH if the graph contains cycles or other invalid topology.
 * \retval VX_ERROR_INVALID_TYPE if any parameter on a node was given the wrong type.
 * \retval VX_ERROR_INVALID_VALUE if any value of any parameter is out of bounds of specification.
 * \retval VX_ERROR_INVALID_FORMAT if the image format was not compatible.
 * \ingroup group_graph
 * \pre <tt>\ref vxCreateGraph</tt>
 * \see vxConvertReference
 * \see vxProcessGraph
 */
vx_status vxVerifyGraph(vx_graph graph);

/*! \brief The call causes the processing of the graph. If the graph
 * has not been verified that will internal take place before processing.
 * If verification fails this function will return a status identical to
 * what <tt>\ref vxVerifyGraph</tt> would return.
 * \param [in] graph The graph to execute.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS Graph has been processed.
 * \retval VX_ERROR_INVALID_GRAPH Graph has not been verified.
 * \ingroup group_graph
 * \pre <tt>\ref vxCreateGraph</tt>
 * \see vxVerifyGraph
 */
vx_status vxProcessGraph(vx_graph graph);

/*! \brief Schedules a graph for future execution.
 * \param [in] graph The graph to schedule.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_ERROR_NO_RESOURCES The graph can not be scheduled now.
 * \retval VX_ERROR_NOT_SUFFICIENT The graph was not verified and has failed
forced verification.
 * \retval VX_SUCCESS The graph has been scheduled.
 * \ingroup group_graph
 */
vx_status vxScheduleGraph(vx_graph graph);

/*! \brief Waits for a specific graph to complete.
 * \param [in] graph The graph to wait on.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS The graph has completed.
 * \retval VX_FAILURE The graph has not completed yet
 * \ingroup group_graph
 */
vx_status vxWaitGraph(vx_graph graph);

/*! \brief Allows the user to query the graph.
 * \param [in] graph The reference to the created graph.
 * \param [in] attribute The <tt>\ref vx_graph_attribute_e</tt> type needed.
 * \param [out] ptr The output pointer where the value will be sent.
 * \param [in] size The size of the objects pointed to by out.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \pre <tt>\ref vxCreateGraph</tt>
 * \ingroup group_graph
 */
vx_status vxQueryGraph(vx_graph graph, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Creates a reference to a node object.
 * \param [in] graph The reference to the graph in which this node will exist.
 * \param [in] kernel The kernel reference which will be associated with this new node.
 * \return vx_node
 * \retval 0 The node failed to create.
 * \retval * A node was created.
 * \ingroup group_node
 * \pre <tt>\ref vxCreateGraph</tt>
 * \pre <tt>\ref vxGetKernelByName</tt>
 * \pre <tt>\ref vxGetKernelByEnum</tt>
 * \post <tt>\ref vxReleaseNode</tt>
 */
vx_node vxCreateNode(vx_graph graph, vx_kernel kernel);

/*! \brief Allows a user to query information out of a node.
 * \param [in] node The reference to the node to query.
 * \param [in] attribute Use <tt>\ref vx_node_attribute_e</tt> value to query for information.
 * \param [out] ptr The output pointer where the value will be sent.
 * \param [in] size The size of the objects pointed to by out.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS Successful
 * \retval VX_ERROR_INVALID_PARAMETERS The type or size was incorrect.
 * \ingroup group_node
 * \pre <tt>\ref vxCreateNode</tt>
 */
vx_status vxQueryNode(vx_node node, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Allows a user to set attribute of a node before Graph Validation.
 * \param [in] node The reference to the node to set.
 * \param [in] attribute Use <tt>\ref vx_node_attribute_e</tt> value to query for information.
 * \param [out] ptr The output pointer where the value will be sent.
 * \param [in] size The size of the objects pointed to by out.
 * \note Some attributes are inherited from the <tt>\ref vx_kernel</tt> which was used
 * to create the node. Some of these can be overridden using this API, notable
 * \ref VX_NODE_ATTRIBUTE_LOCAL_DATA_SIZE, \ref VX_NODE_ATTRIBUTE_LOCAL_DATA_PTR.
 * \ingroup group_node
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS The attribute was set.
 * \retval VX_ERROR_INVALID_REFERENCE node was not a vx_node.
 * \retval VX_ERROR_INVALID_PARAMETER size was not correct for the type needed.
 * \pre <tt>\ref vxCreateNode</tt>
 */
vx_status vxSetNodeAttribute(vx_node node, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Releases a reference to a node object.
 * The object may not be garbage collected until its total reference count is zero.
 * \param [in] node The pointer to the reference of the node to release.
 * \ingroup group_node
 * \note After returning from this function the reference will be zeroed.
 * \pre <tt>\ref vxCreateNode</tt>
 */
void vxReleaseNode(vx_node *node);

/*! \brief Assigns a callback to a node.
 * If a callback already exists in this node, this function must return an error
 * and the user may clear the callback by passing a NULL pointer as the callback.
 * \param [in] node The reference to the node.
 * \param [in] callback The callback to associate with completion of this specific node
 * \note This should be used with extreme caution as it can ruin potential
 * optimizations in the power/performance efficiency of a graph.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS Callback assigned.
 * \retval VX_ERROR_INVALID_REFERENCE The value passed as node was not a vx_node.
 * \ingroup group_node
 * \pre <tt>\ref vxCreateNode</tt>
 */
vx_status vxAssignNodeCallback(vx_node node, vx_nodecomplete_f callback);

/*! \brief Retrieves the current node callback function pointer set on the node.
 * \param [in] node The reference to the <tt>\ref vx_node</tt> object.
 * \ingroup group_node
 * \return vx_nodecomplete_f The pointer to the callback function.
 * \retval NULL No callback has been set.
 * \retval * The node callback function.
 */
vx_nodecomplete_f vxRetrieveNodeCallback(vx_node node);

/*! \brief This is used to extract a single, particular parameter from a kernel node by specifing its attributes.
 * \param [in] node The node to extract the parameter from.
 * \param [in] index The index of the parameter to get a reference to.
 * \return <tt>\ref vx_parameter</tt>
 * \ingroup group_parameter
 * \pre <tt>\ref vxCreateNode</tt>
 * \see vxGetParameterByName
 * \post <tt>\ref vxReleaseParameter</tt>
 */
vx_parameter vxGetParameterByIndex(vx_node node, vx_uint32 index);

/*! \brief Releases a reference to a parameter object.
 * The object may not be garbage collected until its total reference count is zero.
 * \param [in] param The pointer to the parameter to release.
 * \ingroup group_parameter
 * \note After returning from this function the reference will be zeroed.
 * \pre <tt>\ref vxGetParameterByIndex</tt>
 */
void vxReleaseParameter(vx_parameter *param);

/*! \brief Sets the specified parameter data for a kernel on the node.
 * \param [in] node The node which contains the kernel.
 * \param [in] index The index of the parameter desired.
 * \param [in] direction The direction of the kernel parameter. \see vx_direction_e.
 * \param [in] value The reference to the parameter.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_parameter
 * \pre <tt>\ref vxCreateNode</tt>
 * \see vxSetParameterByReference
 * \post <tt>\ref vxReleaseParameter</tt>
 */
vx_status vxSetParameterByIndex(vx_node node, vx_uint32 index, vx_enum direction, vx_reference value);

/*! \brief Associates a parameter reference and a data reference with a kernel
 * on a node.
 * \param [in] parameter The reference to the kernel parameter.
 * \param [in] value The value to associate with the kernel parameter.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_parameter
 * \pre <tt>\ref vxCreateNode</tt>
 * \see vxGetParameterByIndex
 * \see vxGetParameterByName
 * \post <tt>\ref vxReleaseParameter</tt>
 */
vx_status vxSetParameterByReference(vx_parameter parameter, vx_reference value);

/*! \brief This allows the client to query a parameter to determine its meta-information.
 * \param [in] param The reference to the parameter.
 * \param [in] attribute The attribute to query. Use a <tt>\ref vx_parameter_attribute_e</tt>.
 * \param [out] ptr The output pointer where the value will be sent.
 * \param [in] size The size of the objects pointed to by out.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_parameter
 * \pre <tt>\ref vxGetParameterByIndex</tt>
 */
vx_status vxQueryParameter(vx_parameter param, vx_enum attribute, void *ptr, vx_size size);

/*! \brief A generic API to give platform specific hints to the implementation.
 * \param [in] context The reference to the implementation context.
 * \param [in] reference The reference to the object to hint at.
 * This could be <tt>\ref vx_context</tt>, <tt>\ref vx_graph</tt>, <tt>\ref vx_node</tt>, <tt>\ref vx_image</tt>, <tt>\ref vx_buffer</tt>, or any other reference.
 * \param [in] hint A <tt>\ref vx_hint_e</tt> "hint" to give the OpenVX context. This is a platform specific optimization or implementation mechanism.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS No error.
 * \retval VX_ERROR_INVALID_REFERENCE if context or reference are invalid.
 * \retval VX_ERROR_NOT_SUPPORTED if the hint is not supported.
 * \ingroup group_hint
 * \pre <tt>\ref vxCreateContext</tt>
 */
vx_status vxHint(vx_context context, vx_reference reference, vx_enum hint);

/*! \brief A generic API to give platform specific directives to the implementations.
 * \param [in] context The reference to the implementation context.
 * \param [in] reference The reference to the object to set the directive on.
 * This could be <tt>\ref vx_context</tt>, <tt>\ref vx_graph</tt>, <tt>\ref vx_node</tt>, <tt>\ref vx_image</tt>, <tt>\ref vx_buffer</tt>, or any other reference.
 * \param [in] directive The directive to set.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS No error.
 * \retval VX_ERROR_INVALID_REFERENCE if context or reference are invalid.
 * \retval VX_ERROR_NOT_SUPPORTED if the directive is not supported.
 * \pre <tt>\ref vxCreateContext</tt>
 * \ingroup group_directive
 */
vx_status vxDirective(vx_context context, vx_reference reference, vx_enum directive);

/*! \brief This allows access to a sub-range of the a buffer.
 * \param [in] buffer The buffer to access.
 * \param [in] start The start index.
 * \param [in] end The end index.
 * \param [in] ptr The user supplied pointer to a pointer.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_buffer
 * \include vx_bufferrange.c
 */
vx_status vxAccessBufferRange(vx_buffer buffer, vx_size start, vx_size end, void **ptr);

/*! \brief Commits data back to the buffer object.
 * \details This allows a user to commit data to a sub-range of a buffer.
 * \param [in] buffer The buffer to access.
 * \param [in] start The start index.
 * \param [in] end The end index.
 * \param [in] ptr The user supplied pointer.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_buffer
 */
vx_status vxCommitBufferRange(vx_buffer buffer, vx_size start, vx_size end, void *ptr);

/*! \brief Computes the byte size of a buffer range.
 * \details This computes the size of the buffer needed to hold the range
 * defined by the start and end.
 * \param [in] buffer The buffer reference.
 * \param [in] start The start index
 * \param [in] end The end index
 * \return vx_size
 * \ingroup group_buffer
 */
vx_size vxComputeBufferRangeSize(vx_buffer buffer, vx_uint32 start, vx_uint32 end);

/*! \brief This computes the size needs to retrieve an image patch from an image.
 * \param [in] image The reference to the image to extract the patch from.
 * \param [in] rect The coordinates. Must be 0 <= start < end <= dimension where
 * dimension is width for x and height for y.
 * \param [in] plane_index The plane index to get the data from.
 * \return vx_size
 * \ingroup group_image
 */
vx_size vxComputeImagePatchSize(vx_image image,
                                vx_rectangle rect,
                                vx_uint32 plane_index);

/*! \brief This allows the User to extract a rectangular patch (subset) of an image from a single plane.
 * \param [in] image The reference to the image to extract the patch from.
 * \param [in] rect The coordinates to get the patch from. Must be 0 <= start < end.
 * \param [in] plane_index The plane index to get the data from.
 * \param [out] addr The addressing information for the image patch will be written into the data structure.
 * \param [in,out] ptr The pointer to a pointer of a location to store the data.
 * If the user passes in NULL, the function will map or allocate a buffer and return it.
 * If the user passes in an non-NULL pointer, the function will attempt to
 * copy to the location provided by the user.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_ERROR_OPTIMIZED_AWAY The image is not present in memory.
 * \retval VX_ERROR_INVALID_PARAMETERS The start, end, plane index, stride_x or stride_y pointer was incorrect.
 * \retval VX_ERROR_INVALID_REFERENCE The image reference was not actually an image reference.
 * \note The user may ask for data outside the bounds of the valid region, but
 * such data has an undefined value.
 * \ingroup group_image
 * \include vx_imagepatch.c
 *
 */
vx_status vxAccessImagePatch(vx_image image,
                             vx_rectangle rect,
                             vx_uint32 plane_index,
                             vx_imagepatch_addressing_t *addr,
                             void **ptr);

/*! \brief This allows the User to commit a rectangular patch (subset) of an image from a single plane.
 * \param [in] image The reference to the image to extract the patch from.
 * \param [in] rect The coordinates to set the patch to. Must be 0 <= start <= end.
 * This may be 0 or a rectangle of zero area in order to indicate that the commit
 * should only decrement the reference count.
 * \param [in] plane_index The plane index to set the data to.
 * \param [in] addr The addressing information for the image patch.
 * \param [in] ptr The pointer of a location to read the data from. If the
 * user allocated the pointer they must free it. If the pointer
 * was set by <tt>\ref vxAccessImagePatch</tt>, the user may not access the pointer after
 * this call is complete.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_ERROR_OPTIMIZED_AWAY The image is not present in memory.
 * \retval VX_ERROR_INVALID_PARAMETERS The start, end, plane index, stride_x or stride_y pointer was incorrect.
 * \retval VX_ERROR_INVALID_REFERENCE The image reference was not actually an image reference.
 * \ingroup group_image
 * \include vx_imagepatch.c
 * \note If the implementation gave the client a pointer from
 * <tt>\ref vxAccessImagePatch</tt> then implementation specific behavior may occur.
 * If not, then a copy occurs from the users pointer to the internal data of the object.
 * \note If the rectangle is intersects bounds of the current valid region, the
 * valid region grows to the union of the two rectangles as long as they occur
 * within the bounds of the original image dimensions.
 */
vx_status vxCommitImagePatch(vx_image image,
                             vx_rectangle rect,
                             vx_uint32 plane_index,
                             vx_imagepatch_addressing_t *addr,
                             void *ptr);

/*!
 * \brief Used to access a specific indexed pixel in an image patch.
 * \param [in] ptr The base pointer of the patch as returned from <tt>\ref vxAccessImagePatch</tt>.
 * \param [in] index The 0 based index of the pixel count in the patch. Indexes increase horizontally by 1 then wrap around to the next row.
 * \param [in] addr The pointer to the addressing mode information returned from <tt>\ref vxAccessImagePatch</tt>.
 * \return void * Returns the pointer to the specified pixel.
 * \pre <tt>\ref vxAccessImagePatch</tt>
 * \include vx_imagepatch.c
 * \ingroup group_image
 */
void *vxFormatImagePatchAddress1d(void *ptr, vx_uint32 index, vx_imagepatch_addressing_t *addr);

/*!
 * \brief Used to access a specific pixel at a 2d coordinate in an image patch.
 * \param [in] ptr The base pointer of the patch as returned from <tt>\ref vxAccessImagePatch</tt>.
 * \param [in] x The x dimension within the patch.
 * \param [in] y The y dimension within the patch.
 * \param [in] addr The pointer to the addressing mode information returned from <tt>\ref vxAccessImagePatch</tt>.
 * \return void * Returns the pointer to the specified pixel.
 * \pre <tt>\ref vxAccessImagePatch</tt>
 * \include vx_imagepatch.c
 * \ingroup group_image
 */
void *vxFormatImagePatchAddress2d(void *ptr, vx_uint32 x, vx_uint32 y, vx_imagepatch_addressing_t *addr);

/*! \brief This API allows users to add custom kernels to the known kernel
 * database in OpenVX at runtime. This would primarily be used by the module function
 * vxPublishKernels.
 * \param [in] context The reference to the implementation context.
 * \param [in] name The string which is to be used to match the kernel.
 * \param [in] enumeration The enumerated value of the kernel to be used by clients.
 * \param [in] func_ptr The process-local function pointer to be invoked
 * \param [in] numParams The number of parameters for this kernel.
 * \param [in] input The pointer to a function which will validate the
 * input parameters to this kernel.
 * \param [in] output The pointer to a function which will validate the
 * output parameters to this kernel.
 * \param [in] init The kernel initialization function.
 * \param [in] deinit The kernel de-initialization function.
 * \ingroup group_user_nodes
 * \return <tt>\ref vx_kernel</tt>
 * \retval 0 Indicates that an error occurred when adding the kernel.
 * \retval * Kernel added to OpenVX.
 * \pre vxCreateContext vxLoadKernels
 */
vx_kernel vxAddKernel(vx_context context,
                      vx_char name[VX_MAX_KERNEL_NAME],
                      vx_enum enumeration,
                      vx_kernel_f func_ptr,
                      vx_uint32 numParams,
                      vx_kernel_input_validate_f input,
                      vx_kernel_output_validate_f output,
                      vx_kernel_initialize_f init,
                      vx_kernel_deinitialize_f deinit);

/*! \brief This API is called after all parameters have been added to the
 * kernel and the kernel is "ready" to be used.
 * \param [in] kernel The reference to the loaded kernel from <tt>\ref vxAddKernel</tt>.
 * \return A <tt>\ref vx_status_e</tt> enumeration. If an error occurs, the kernel will not be available
 * for usage by the clients of OpenVX. Typically this is due to a mismatch
 * between the number of parameter requested and given.
 * \pre vxAddKernel vxAddParameterToKernel
 * \ingroup group_user_nodes
 */
vx_status vxFinalizeKernel(vx_kernel kernel);

/*! \brief This API allows users to set the signatures of the custom kernel.
 * \param [in] kernel The reference to the kernel added with <tt>\ref vxAddKernel</tt>.
 * \param [in] index The index of the parameter to add.
 * \param [in] dir The direction of the parameter. This must be a value from <tt>\ref vx_direction_e</tt>.
 * \param [in] type The type of parameter. This must be a value from <tt>\ref vx_type_e</tt>.
 * \param [in] state The state of the parameter (Required or not). This must be a value from <tt>\ref vx_parameter_state_e</tt>.
 * \return A <tt>\ref vx_status_e</tt> enumerated value.
 * \retval VX_SUCCESS Parameter set on kernel.
 * \retval VX_ERROR_INVALID_REFERENCE The value passed as kernel was not a vx_kernel.
 * \pre vxAddKernel
 * \ingroup group_user_nodes
 */
vx_status vxAddParameterToKernel(vx_kernel kernel, vx_uint32 index, vx_enum dir, vx_enum type, vx_enum state);

/*! \brief Removes a non-finalized vx_kernel from the vx_context.
 * Once a <tt>\ref vx_kernel</tt> has been finalized it can not be removed.
 * \param [in] kernel The reference to the kernel to be removed. Returned from <tt>\ref vxAddKernel</tt>.
 * \note Any kernel enumerated in the base standard
 * can not be removed. Only kernels added through <tt>\ref vxAddKernel</tt> can
 * be removed.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_ERROR_INVALID_REFERENCE if an invalid kernel was passed in.
 * \retval VX_ERROR_INVALID_PARAMETER if a base kernel was passed in.
 * \ingroup group_user_nodes
 */
vx_status vxRemoveKernel(vx_kernel kernel);

/*! \brief The interface to set kernel attributes.
 * \param [in] kernel The reference to the kernel.
 * \param [in] attribute The enumeration of the attributes. See <tt>\ref vx_kernel_attribute_e</tt>.
 * \param [in,out] ptr The pointer to the location to read or write the attribute.
 * \param [in] size The size of the data area indicated by ptr.
 * \note After a kernel has been passed to <tt>\ref vxFinalizeKernel</tt>, no attributes
 * can be altered.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_user_nodes
 */
vx_status vxSetKernelAttribute(vx_kernel kernel, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Creates a reference to a scalar object.
 * \param [in] context The reference to the system context.
 * \param [in] type The type of the scalar.
 * \param [in] ptr The pointer to the initial value of the scalar.
 * \ingroup group_scalars
 * \post vxReleaseScalar
 */
vx_scalar vxCreateScalar(vx_context context, vx_enum type, void *ptr);

/*! \brief Releases a reference to a scalar object.
 * The object may not be garbage collected until its total reference count is zero.
 * \param [in] scalar The pointer to the scalar to release.
 * \ingroup group_scalars
 * \note After returning from this function the reference will be zeroed.
 * \pre vxCreateScalar
 */
void vxReleaseScalar(vx_scalar *scalar);

/*! \brief Queries attributes from a scalar.
 * \param [in] scalar The scalar object.
 * \param [in] attribute The enumeration to query. Use a <tt>\ref vx_rectangle_attribute_e</tt> enumeration.
 * \param [in] ptr The address of the variable to hold the attribute.
 * \param [in] size The byte size that ptr points to.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_scalar
 */
vx_status vxQueryScalar(vx_scalar scalar, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Gets the scalar value out of a reference.
 * \note Use this in conjunction with Query APIs which return references which
 * should be converted into values.
 * \ingroup group_scalars
 * \param [in] ref The reference to get the scalar value from.
 * \param [out] ptr An appropriate typed pointer which points to a location to copy
 * the scalar value to.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_ERROR_INVALID_REFERENCE Will be returned if the ref is not a valid
 * reference.
 * \retval VX_ERROR_INVALID_PARAMETERS will be returned if ptr is NULL.
 * \retval VX_ERROR_INVALID_TYPE will be returned if the type does not match the type in the reference or is a bad value.
 */
vx_status vxAccessScalarValue(vx_scalar ref, void *ptr);

/*! \brief Sets the scalar value in a reference.
 * \note Use this in conjunction with Parameter APIs which return references
 * to parameters which need to be altered.
 * \ingroup group_scalars
 * \param [in] ref The reference to get the scalar value from.
 * \param [in] ptr An appropriately typed pointer which points to a location to copy
 * the scalar value to.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_ERROR_INVALID_REFERENCE Will be returned if the ref is not a valid
 * reference.
 * \retval VX_ERROR_INVALID_PARAMETERS will be returned if ptr is NULL.
 * \retval VX_ERROR_INVALID_TYPE will be returned if the type does not match the type in the reference or is a bad value.
 */
vx_status vxCommitScalarValue(vx_scalar ref, void *ptr);

/*! \brief Used to query any reference type for some basic information (count, type)
 * \param [in] ref The reference to query.
 * \param [in] attribute The value to query for. Use <tt>\ref vx_reference_attribute_e</tt>.
 * \param [out] ptr The location to store the value.
 * \param [in] size The size of the location pointed to by ptr.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_reference
 */
vx_status vxQueryReference(vx_reference ref, vx_enum attribute, void *ptr, vx_size size);


/*! \brief Creates a reference to a delay object of <tt>\ref vx_image</tt> types.
 * \param [in] context The reference to the system context.
 * \param [in] width The width in pixels.
 * \param [in] height The height in pixels.
 * \param [in] format The FOURCC code of the image.
 * \param [in] count The number of images in the delay ring.
 * \return <tt>\ref vx_delay</tt>
 * \ingroup group_delay
 * \post <tt>\ref vxReleaseDelay</tt>
 */
vx_delay vxCreateImageDelay(vx_context context,
                            vx_uint32 width,
                            vx_uint32 height,
                            vx_fourcc format,
                            vx_size count);

/*! \brief Queries a <tt>\ref vx_delay</tt> object attribute.
 * \param [in] delay The coordinates object to set.
 * \param [in] attribute The attribute to query. Use a <tt>\ref vx_delay_attribute_e</tt> enumeration.
 * \param [out] ptr The pointer to the location to set the queried value to.
 * \param [in] size The size of the data pointed to by ptr.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_delay
 */
vx_status vxQueryDelay(vx_delay delay, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Releases a reference to a delay object.
 * The object may not be garbage collected until its total reference count is zero..
 * \param [in] delay The pointer to the delay to release.
 * \note After returning from this function the reference will be zeroed.
 * \pre <tt>\ref vxCreateImageDelay</tt> <tt>\ref vxCreateBufferDelay</tt>
 * \ingroup group_delay
 */
void vxReleaseDelay(vx_delay *delay);

/*! Creates a reference to a delay object of <tt>\ref vx_buffer</tt> types.
 * \param [in] context The reference to the system context.
 * \param [in] unitSize The number of bytes per unit.
 * \param [in] numUnits The number of units in the buffer.
 * \param [in] count The number of buffers in the delay ring.
 * \return <tt>\ref vx_delay</tt>
 * \post <tt>\ref vxReleaseDelay</tt>
 * \ingroup group_delay
 */
vx_delay vxCreateBufferDelay(vx_context context,
                             vx_size unitSize,
                             vx_size numUnits,
                             vx_size count);

/*! \brief Retrieves an image from an image delay object.
 * \param [in] delay The reference to the delay object.
 * \param [in] index An index into the delay from which to extract the
 * reference to the image.
 * \return <tt>\ref vx_image</tt>
 * \note The delay index is in the range \f$ [-count+1,0] \f$. 0 is always the
 * "current" object.
 * \pre <tt>\ref vxCreateImageDelay</tt>
 * \ingroup group_delay
 * \note A reference from a delay object should not be given to its associated
 * release API. Use the <tt>\ref vxReleaseDelay</tt> only.
 */
vx_image vxGetImageFromDelay(vx_delay delay, vx_int32 index);

/*! \brief Retrieves a buffer from an buffer delay object.
 * \param [in] delay The reference to the delay object.
 * \param [in] index An index into the delay from which to extract the
 * reference to the image.
 * \return <tt>\ref vx_buffer</tt>
 * \note The delay index is in the range \f$ [-count+1,0] \f$. 0 is always the
 * "current" object.
 * \pre <tt>\ref vxCreateBufferDelay</tt>
 * \ingroup group_delay
 * \note A reference from a delay object should not be given to its associated
 * release API. Use the <tt>\ref vxReleaseDelay</tt> only.
 */
vx_image vxGetBufferFromDelay(vx_delay delay, vx_int32 index);

/*! \brief Ages the internal delay ring by one. This means that once this API is
 * called the reference from index 0 will go to index -1 and so forth until
 * \f$ -count+1 \f$ is reached. This last object will become 0. Once the delay
has
 * been aged, it will update the reference in any associated nodes.
 * \param [in] delay
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS Delay was aged.
 * \retval VX_ERROR_INVALID_REFERENCE The value passed as delay was not a vx_delay.
 * \pre <tt>\ref vxCreateImageDelay</tt>
 * \pre <tt>\ref vxCreateBufferDelay</tt>
 * \pre <tt>\ref vxAssociateDelayWithNode</tt>
 * \ingroup group_delay
 */
vx_status vxAgeDelay(vx_delay delay);

/*! \brief Associates a delay index with a particular node's parameter.
 * \param [in] delay The reference to the delay object.
 * \param [in] delay_index The index of the object in the delay object. This is
 * in the range of \f$ [-count+1,0] \f$.
 * \param [in] node The reference to the node.
 * \param [in] param_index The index of the parameter on the node. This is in
 * the range of \f$ [0,numParams-1] \f$.
 * \param [in] param_direction The directionality of the parameter. See <tt>\ref vx_direction_e</tt>.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \pre <tt>\ref vxCreateImageDelay</tt> or <tt>\ref vxCreateBufferDelay</tt>
 * \ingroup group_delay
 * \post <tt>\ref vxAgeDelay</tt>
 * \include vx_delaygraph.c
 */
vx_status vxAssociateDelayWithNode(vx_delay delay,
                                   vx_int32 delay_index,
                                   vx_node node,
                                   vx_uint32 param_index,
                                   vx_enum param_direction);

/*! \brief Dissociates a delay index from a particular node parameter.
 * \param [in] delay The reference to the delay object.
 * \param [in] delay_index The relative index of the object in the delay.
 * \param [in] node The reference to the node.
 * \param [in] param_index The index to the parameter on the node.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \pre vxAssociateDelayWithNode
 * \ingroup group_delay
 */
vx_status vxDissociateDelayFromNode(vx_delay delay,
                                    vx_int32 delay_index,
                                    vx_node node,
                                    vx_uint32 param_index);

/*! \brief Used to retrieve a target reference by the index of the target.
 * \param [in] context The reference to the overall context.
 * \param [in] index The index of the target to get a reference to.
 * \return <tt>\ref vx_target</tt>
 * \retval 0 Invalid index.
 * \retval * A target reference.
 * \note Use <tt>\ref vxQueryContext</tt> with <tt>\ref VX_CONTEXT_ATTRIBUTE_NUMTARGETS</tt> to retrieve the upper limit of targets.
 * \ingroup group_target
 */
vx_target vxGetTargetByIndex(vx_context context, vx_uint32 index);

/*! \brief Used to get a reference to named target when the name is known beforehand.
 * \param [in] context The reference to the overall context.
 * \param [in] name The target name which is of the format "<vendor>.<vendor_string>"
 * \return <tt>\ref vx_target</tt>
 * \retval 0 Invalid index.
 * \retval * A target reference.
 * \ingroup group_target
 */
vx_target vxGetTargetByName(vx_context context, const vx_char *name);

/*! \brief Releases a reference to a target object.
 * The object may not be garbage collected until its total reference count is zero.
 * \param [in] target The pointer to the target to release.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \note After returning from this function the reference will be zeroed.
 * \ingroup group_target
 */
void vxReleaseTarget(vx_target *target);

/*! \brief Used to query the target about it's properties.
 * \param [in] target The reference to the target.
 * \param [in] attribute The <tt>\ref vx_target_attribute_e</tt> value to query for.
 * \param [out] ptr The pointer to the location to store the value.
 * \param [in] len The length of the area pointed to by ptr.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \pre <tt>\ref vxGetTargetByName</tt> or <tt>\ref vxGetTargetByIndex</tt>
 * \ingroup group_target
 */
vx_status vxQueryTarget(vx_target target, vx_enum attribute, void *ptr, size_t len);

/*! \brief Used to assign target affinity to a node.
 * \param [in] node The node reference to assign affinity to.
 * \param [in] target The reference to the target.
 * \pre <tt>\ref vxGetTargetByName</tt> or <tt>\ref vxGetTargetByIndex</tt>
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_target
 */
vx_status vxAssignNode(vx_node node, vx_target target);

/*! \brief Adds a line to the log.
 * \param [in] ref The reference to add the log entry against. Some valid value must be provided.
 * \param [in] status The status code. <tt>\ref VX_SUCCESS</tt> status entries will be ignored and not added.
 * \param [in] message The human readable message to add to the log.
 * \param [in] ... a list of variable arguments to the message.
 * \note Messages may not exceed <tt>\ref VX_MAX_LOG_MESSAGE_LEN</tt> bytes and will be truncated in the log if they exceed this limit.
 * \ingroup group_log
 */
void vxAddLogEntry(vx_reference ref, vx_status status, const char *message, ...);

/*! \brief Returns the previous entry of the log. When called consecutively it
 * will return the entire log. The log will be cleared by reading it.
 * \param [in] ref The reference to filter the log entries against.
 * If the context is given, the next entry will be returned.
 * \param [out] message A predefined location to store a copy of the log's
 * message value.
 * This must point to at least <tt>\ref VX_MAX_LOG_MESSAGE_LEN</tt> bytes of characters.
 * \return Returns the status of the log entry from <tt>\ref vx_status_e</tt>.
 * \ingroup group_log
 * \note The API returns errors oldest to newest order.
 * When VX_SUCCESS is returned, the log reading is complete.
 * \include vx_clearlog.c
 */
vx_status vxGetLogEntry(vx_reference ref, char message[VX_MAX_LOG_MESSAGE_LEN]);

/*! \brief Creates a reference to a rectangle object.
 * \param [in] context The context reference.
 * \param [in] sx The start x coordinate.
 * \param [in] sy The start y coordinate.
 * \param [in] ex The end x coordinate.
 * \param [in] ey The end y coordinate.
 * \return <tt>\ref vx_rectangle</tt>
 * \ingroup group_rectangle
 */
vx_rectangle vxCreateRectangle(vx_context context,
                               vx_uint32 sx,
                               vx_uint32 sy,
                               vx_uint32 ex,
                               vx_uint32 ey);

/*! \brief Release a reference to a rectangle object.
 * The object may not be garbage collected until its total reference count is zero.
 * \param [in] rect The pointer to the rectangle to release.
 * \note After returning from this function the reference will be zeroed.
 * \ingroup group_rectangle
 */
void vxReleaseRectangle(vx_rectangle *rect);

/*! \brief Queries attributes from a rectangle.
 * \param [in] rect The rectangle object.
 * \param [in] attribute The enumeration to query. Use a <tt>\ref vx_rectangle_attribute_e</tt> enumeration.
 * \param [in] ptr The address of the variable to hold the attribute.
 * \param [in] size The byte size that ptr points to.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_rectangle
 */
vx_status vxQueryRectangle(vx_rectangle rect, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Set an attributes on a rectangle.
 * \param [in] rect The rectangle object.
 * \param [in] attribute The enumeration to set. Use a <tt>\ref vx_rectangle_attribute_e</tt> enumeration.
 * \param [in] ptr The address of the variable to read the attribute.
 * \param [in] size The byte size that ptr points to.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_rectangle
 */
vx_status vxSetRectangleAttribute(vx_rectangle rect, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Creates LUT object of a given type.
 * \param [in] context The reference to the context.
 * \param [in] type The type of LUT.
 * \param [in] count The number of entries desired.
 * \if OPENVX_STRICT_1_0
 * \note For OpenVX 1.0 count must be equal to 256 and the type can only be \ref VX_TYPE_UINT8.
 * \endif
 * \return <tt>\ref vx_lut</tt>
 * \post <tt>\ref vxReleaseLUT</tt>
 * \ingroup group_lut
 */
vx_lut vxCreateLUT(vx_context context, vx_enum type, vx_size count);

/*! \brief Release a reference to a LUT object.
 * The object may not be garbage collected until its total reference count is zero..
 * \param [in] lut The pointer to the LUT to release.
 * \note After returning from this function the reference will be zeroed.
 * \pre <tt>\ref vxCreateLUT</tt>
 * \ingroup group_lut
 */
void vxReleaseLUT(vx_lut *lut);

/*! \brief Queries attributes from a LUT.
 * \param [in] lut The LUT to query.
 * \param [in] attribute The attribute to query. Use a <tt>\ref vx_lut_attribute_e</tt> enumeration.
 * \param [out] ptr The address of the variable to hold the attribute.
 * \param [in] size The byte size that ptr points to.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_lut
 */
vx_status vxQueryLUT(vx_lut lut, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Gets direct access to the LUT table data.
 * \details There are several variants of call methodology:
 * \arg If ptr is NULL (which means the current data of the LUT is not desired),
 * the LUT reference count is incremented.
 * \arg If ptr is not NULL but (*ptr) is NULL, (*ptr) will contain the address of the LUT data when the function returns and
 * the reference count will be incremented. Whether the (*ptr) address is mapped
 * or allocated is undefined. (*ptr) must be returned to <tt>\ref vxCommitLUT</tt>.
 * \arg If ptr is not NULL and (*ptr) is not NULL, the user is signalling the implementation to copy the LUT data into the location specified
 * by (*ptr). Users must use <tt>\ref vxQueryLUT</tt> with <tt>\ref VX_LUT_ATTRIBUTE_SIZE</tt> to
 * determine how much memory to allocate for the LUT data.
 *
 * In any case, <tt>\ref vxCommitLUT</tt> must be called after LUT access is complete.
 * \param [in] lut The LUT to get the data from.
 * \param [in,out] ptr The address of the location to store the pointer to the LUT memory.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \post <tt>\ref vxCommitLUT</tt>
 * \ingroup group_lut
 */
vx_status vxAccessLUT(vx_lut lut, void **ptr);

/*! \brief Commits the Lookup Table.
 * \details Commits the data back to the LUT object and decrements the reference count.
 * There are several variants of call methodology:
 * \arg If a user should allocated their own memory for the LUT data copy, the user is
 * obligated to free this memory.
 * \arg If ptr is not NULL and the (*ptr) for <tt>\ref vxAccessLUT</tt> was NULL,
 * it is undefined whether the implementation will unmap or copy and free the memory.
 * \param [in] lut The LUT to modify.
 * \param [in] ptr The pointer used with <tt>\ref vxAccessLUT</tt>. This may not be NULL.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \pre <tt>\ref vxAccessLUT</tt>.
 * \ingroup group_lut
 */
vx_status vxCommitLUT(vx_lut lut, void *ptr);

/*! \brief Creates a reference to a 1D Distribution with a start offset, valid range, and number of equally weighted bins.
 * \param [in] context The reference to the overall context.
 * \param [in] numBins The number of bins in the distribution.
 * \param [in] offset The offset into the range value.
 * \param [in] range The total range of the values.
 * \return <tt>\ref vx_distribution</tt>
 * \ingroup group_distribution
 */
vx_distribution vxCreateDistribution(vx_context context, vx_size numBins, vx_size offset, vx_size range);

/*! \brief Releases a reference to a distribution object.
 * The object may not be garbage collected until its total reference count is zero..
 * \param [in] distribution The reference to the distribution to release.
 * \note After returning from this function the reference will be zeroed.
 * \pre <tt>\ref vxCreateDistribution</tt>
 * \ingroup group_distribution
 */
void vxReleaseDistribution(vx_distribution *distribution);

/*! \brief Queries a Distribution object.
 * \param [in] distribution The reference to the distribution to query.
 * \param [in] attribute The attribute to query. Use a <tt>\ref vx_distribution_attribute_e</tt> enumeration.
 * \param [out] ptr The pointer to store the attribute
 * \param [in] size The size of the area which pointer references.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \pre <tt>\ref vxCreateDistribution</tt>
 * \ingroup group_distribution
 */
vx_status vxQueryDistribution(vx_distribution distribution, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Gets direct access to a Distribution in memory.
 * \param [in] distribution The reference to the distribution to access.
 * \param [in,out] ptr The address of the location to store the pointer to the
 * Distribution memory. If the pointer is not NULL, the Distribution will be copied to that
 * address. If the pointer is NULL, the pointer will be mapped to the Distribution memory.
 * In either case, <tt>\ref vxCommitDistribution</tt> must be called after Distribution access is complete.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \pre <tt>\ref vxCreateDistribution</tt>
 * \post <tt>\ref vxCommitDistribution</tt>
 * \ingroup group_distribution
 */
vx_status vxAccessDistribution(vx_distribution distribution, void * * ptr);

/*! \brief Sets the Distribution back to the memory. The memory is the array must be
 * a vx_uint32 array of a value at least as big as the value returned via VX_DISTRIBUTION_ATTRIBUTE_RANGE.
 * \param [in] distribution The Distribution to modify.
 * \param [in] ptr The pointer returned from (or not modified by) <tt>\ref vxAccessLUT</tt>.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \pre <tt>\ref vxAccessDistribution</tt>.
 * \ingroup group_distribution
 */
vx_status vxCommitDistribution(vx_distribution distribution, void * ptr);

/*! \brief Releases a reference to a threshold object.
 * The object may not be garbage collected until its total reference count is zero.
 * \param [in] thresh The pointer to the threshold to release.
 * \note After returning from this function the reference will be zeroed.
 * \ingroup group_threshold
 */
void vxReleaseThreshold(vx_threshold *thresh);

/*! \brief Creates a reference to a threshold object of a given type.
 * \param [in] c The reference to the overall context.
 * \param [in] type The type of threshold to create.
 * \return <tt>\ref vx_threshold</tt>
 * \ingroup group_threshold
 */
vx_threshold vxCreateThreshold(vx_context c, vx_enum type);

/*! \brief Sets attributes on the threshold object.
 * \param [in] thresh The threshold object to set.
 * \param [in] attribute The attribute to modify. Use a <tt>\ref vx_threshold_attribute_e</tt> enumeration.
 * \param [in] ptr The pointer to the value to set the attribute to.
 * \param [in] size The size of the data pointed to by ptr.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_threshold
 */
vx_status vxSetThresholdAttribute(vx_threshold thresh, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Queries an attribute on the threshold object.
 * \param [in] thresh The threshold object to set.
 * \param [in] attribute The attribute to query. Use a <tt>\ref vx_threshold_attribute_e</tt> enumeration.
 * \param [out] ptr The pointer to the location to set the queried value to.
 * \param [in] size The size of the data pointed to by ptr.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_threshold
 */
vx_status vxQueryThreshold(vx_threshold thresh, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Releases a reference to a matrix object.
 * The object may not be garbage collected until its total reference count is zero.
 * \param [in] mat The matrix reference to release.
 * \note After returning from this function the reference will be zeroed.
 * \ingroup group_matrix
 */
void vxReleaseMatrix(vx_matrix *mat);

/*! \brief Creates a reference to a matrix object.
 * \param [in] c The reference to the overall context.
 * \param [in] type The unit format of the matrix. VX_TYPE_INT32 or VX_TYPE_FLOAT32.
 * \param [in] columns The first dimensionality.
 * \param [in] rows The second dimensionality.
 * \return <tt>\ref vx_matrix</tt>
 * \ingroup group_matrix
 */
vx_matrix vxCreateMatrix(vx_context c, vx_enum type, vx_size columns, vx_size rows);

/*! \brief Queries an attribute on the matrix object.
 * \param [in] mat The matrix object to set.
 * \param [in] attribute The attribute to query. Use a <tt>\ref vx_matrix_attribute_e</tt> enumeration.
 * \param [out] ptr The pointer to the location to set the queried value to.
 * \param [in] size The size of the data pointed to by ptr.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_matrix
 */
vx_status vxQueryMatrix(vx_matrix mat, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Gets the matrix data (copy)
 * \param [in] mat The reference to the matrix.
 * \param [out] array The array to place the matrix.
 * \see vxQueryMatrix and <tt>\ref VX_MATRIX_ATTRIBUTE_COLUMNS</tt> and <tt>\ref VX_MATRIX_ATTRIBUTE_ROWS</tt>
 * to get the needed number of elements of the array.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_matrix
 */
vx_status vxAccessMatrix(vx_matrix mat, void *array);

/*! \brief Sets the matrix data (copy)
 * \param [in] mat The reference to the matrix.
 * \param [out] array The array to read the matrix.
 * \see vxQueryMatrix and <tt>\ref VX_MATRIX_ATTRIBUTE_COLUMNS</tt> and <tt>\ref VX_MATRIX_ATTRIBUTE_ROWS</tt>
 * to get the needed number of elements of the array.'
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_matrix
 */
vx_status vxCommitMatrix(vx_matrix mat, void *array);

/*! \brief Releases a reference to a coordinates object.
 * The object may not be garbage collected until its total reference count is zero..
 * \param [in] coord The pointer to the coordinates to release.
 * \note After returning from this function the reference will be zeroed.
 * \ingroup group_coordinates
 */
void vxReleaseCoordinates(vx_coordinates *coord);

/*! \brief Creates a reference to a coordinates object with unintialized values of the specified dimensions.
 * \param [in] c The reference to the overall context.
 * \param [in] dims The number of dimensions of the coordinate.
 * \return <tt>\ref vx_coordinates</tt>
 * \ingroup group_coordinates
 */
vx_coordinates vxCreateCoordinates(vx_context c, vx_size dims);

/*! \brief Queries an attribute on the coordinates object.
 * \param [in] coord The coordinates object to set.
 * \param [in] attribute The attribute to query. Use a <tt>\ref vx_coordinates_attribute_e</tt> enumeration.
 * \param [out] ptr The pointer to the location to set the queried value to.
 * \param [in] size The size of the data pointed to by ptr.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_coordinates
 */
vx_status vxQueryCoordinates(vx_coordinates coord, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Sets attributes on the coordinates object.
 * \param [in] coord The coordinates object to set.
 * \param [in] attr The attribute to modify. Use a <tt>\ref vx_coordinates_attribute_e</tt> enumeration.
 * \param [in] ptr The pointer to the value to set the attribute to.
 * \param [in] size The size of the data pointed to by ptr.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_coordinates
 */
vx_status vxSetCoordinatesAttributes(vx_coordinates coord, vx_enum attr, void *ptr, vx_size size);

/*! \brief Gets the coordinate data (copy)
 * \param [in] coord The reference to the coordinates.
 * \param [out] array The array to place the coordinates.
 * \see vxQueryCoordinates and <tt>\ref VX_COORDINATES_ATTRIBUTE_DIMENSIONS</tt> to get the
 * needed number of elements of the array.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_coordinates
 */
vx_status vxAccessCoordinates(vx_coordinates coord, vx_int32 *array);

/*! \brief Sets the coordinate data (copy)
 * \param [in] coord The reference to the coordinates.
 * \param [out] array The array to read the coordinates.
 * \see vxQueryCoordinates and <tt>\ref VX_COORDINATES_ATTRIBUTE_DIMENSIONS</tt> to get the
 * needed number of elements of the array.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_coordinates
 */
vx_status vxCommitCoordinates(vx_coordinates coord, vx_int32 *array);

/*! \brief Releases the reference to a convolution matrix.
 * The object may not be garbage collected until its total reference count is zero..
 * \param [in] conv The pointer to the convolution matrix to release.
 * \note After returning from this function the reference will be zeroed.
 * \ingroup group_convolution
 */
void vxReleaseConvolution(vx_convolution *conv);

/*! \brief Creates a reference to a convolution matrix object.
 * \param [in] context The reference to the overall context.
 * \param [in] columns The columns dimension of the convolution.
 * Must be odd and greater than or equal to 3 and less than the value returned
 * from \ref VX_CONTEXT_ATTRIBUTE_CONVOLUTION_MAXIMUM_DIMENSION.
 * \param [in] rows The rows dimension of the convolution.
 * Must be odd and greater than or equal to 3 and less than the value returned
 * from \ref VX_CONTEXT_ATTRIBUTE_CONVOLUTION_MAXIMUM_DIMENSION.
 * \return <tt>\ref vx_convolution</tt>
 * \ingroup group_convolution
 */
vx_convolution vxCreateConvolution(vx_context context, vx_size columns, vx_size rows);

/*! \brief Queries an attribute on the convolution matrix object.
 * \param [in] conv The convolution matrix object to set.
 * \param [in] attribute The attribute to query. Use a <tt>\ref vx_convolution_attribute_e</tt> enumeration.
 * \param [out] ptr The pointer to the location to set the queried value to.
 * \param [in] size The size of the data pointed to by ptr.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_convolution
 */
vx_status vxQueryConvolution(vx_convolution conv, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Sets attributes on the convolution object.
 * \param [in] conv The coordinates object to set.
 * \param [in] attr The attribute to modify. Use a <tt>\ref vx_convolution_attribute_e</tt> enumeration.
 * \param [in] ptr The pointer to the value to set the attribute to.
 * \param [in] size The size of the data pointed to by ptr.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_convolution
 */
vx_status vxSetConvolutionAttribute(vx_convolution conv, vx_enum attr, void *ptr, vx_size size);

/*! \brief Gets the convolution data (copy)
 * \param [in] conv The reference to the convolution.
 * \param [out] array The array to place the convolution.
 * \see vxQueryConvolution and <tt>\ref VX_CONVOLUTION_ATTRIBUTE_SIZE</tt> to get the
 * needed number of bytes of the array.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_convolution
 */
vx_status vxAccessConvolutionCoefficients(vx_convolution conv, vx_int16 *array);

/*! \brief Sets the convolution data (copy)
 * \param [in] conv The reference to the convolution.
 * \param [out] array The array to read the convolution.
 * \see vxQueryConvolution and <tt>\ref VX_CONVOLUTION_ATTRIBUTE_SIZE</tt> to get the
 * needed number of bytes of the array.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_convolution
 */
vx_status vxCommitConvolutionCoefficients(vx_convolution conv, vx_int16 *array);

/*! \brief Releases a reference to a pyramid object.
 * The object may not be garbage collected until its total reference count is zero.
 * \param [in] pyr The pointer to the pyramid to release.
 * \ingroup group_pyramid
 * \note After returning from this function the reference will be zeroed.
 * \pre <tt>\ref vxCreatePyramid</tt>
 */
void vxReleasePyramid(vx_pyramid *pyr);

/*! \brief Creates a reference to a pyramid object of the supplied number of levels.
 * \param [in] c The overall context.
 * \param [in] levels The number of levels desired.
 * \param [in] scale Used to indicate the scale between pyramid levels.
 * \param [in] width The width of the 0th level image in pixels.
 * \param [in] height The height of the 0th level image in pixels.
 * \param [in] format The format of all images in the pyramid.
 * \return <tt>\ref vx_pyramid</tt>
 * \retval 0 No pyramid was created.
 * \retval * A pyramid reference.
 * \ingroup group_pyramid
 */
vx_pyramid vxCreatePyramid(vx_context c, vx_size levels, vx_float32 scale, vx_uint32 width, vx_uint32 height, vx_fourcc format);

/*! \brief Queries an attribute from an image pyramid.
 * \param [in] pyr The pyramid to query.
 * \param [in] attribute The attribute to query for. Use a <tt>\ref vx_pyramid_attribute_e</tt> enumeration.
 * \param [out] ptr The pointer to the location to store the value.
 * \param [in] size The size of the location pointed to by ptr.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_pyramid
 */
vx_status vxQueryPyramid(vx_pyramid pyr, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Retrieves a level of the pyramid as a vx_image, which can be used
 * elsewhere in OpenVX.
 * \param [in] pyr The pyramid object.
 * \param [in] index The index of the level, such that index is less than levels.
 * \return vx_image
 * \retval 0 Indicates that the index or the object was invalid.
 * \ingroup group_pyramid
 */
vx_image vxGetPyramidLevel(vx_pyramid pyr, vx_uint32 index);

/*! \brief Retrieves the valid region of the image as a rectangle.
 * \details After the image is allocated but has not been written to this will
 * return the full rectangle of the image so that functions do not have to manage
 * a case for uninitialized data. The image will still retain an uninitialized
 * value but once the image is written to via any means such as <tt>\ref vxCommitImagePatch</tt>,
 * the valid region will be altered to contain the maximum bounds of the written
 * area.
 * \param [in] image The image to retrieve the valid region from.
 * \return vx_rectangle
 * \retval 0 Invalid image.
 * \retval * Valid image.
 * \note This rectangle can be passed directly to <tt>\ref vxAccessImagePatch</tt> to get
 * the full valid region of the image. Modifications from <tt>\ref vxCommitImagePatch</tt>
 * will grow the valid region.
 * \post <tt>\ref vxReleaseRectangle</tt>
 * \ingroup group_image
 */
vx_rectangle vxGetValidRegionImage(vx_image image);

/*! \brief Retrieves the start and end, x and y coordinates from the rectangle.
 * \param [in] rectangle The rectangle to query.
 * \param [out] psx The pointer to the start x coordinate.
 * \param [out] psy The pointer to the start y coordinate.
 * \param [out] pex The pointer to the end x coordinate.
 * \param [out] pey The pointer to the end y coordinate.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_rectangle
 */
vx_status vxAccessRectangleCoordinates(vx_rectangle rectangle,
                                       vx_uint32 *psx,
                                       vx_uint32 *psy,
                                       vx_uint32 *pex,
                                       vx_uint32 *pey);

/*! \brief Retrieves the start and end, x and y coordinates from the rectangle.
 * \param [in] rectangle The rectangle to query.
 * \param [out] sx The start x coordinate. Must be sx <= ex.
 * \param [out] sy The start y coordinate. Must be sy <= ey.
 * \param [out] ex The end x coordinate. Must be sx <= ex.
 * \param [out] ey The end y coordinate. Must be sy <= ey.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_rectangle
 */
vx_status vxCommitRectangleCoordinates(vx_rectangle rectangle,
                                       vx_uint32 sx,
                                       vx_uint32 sy,
                                       vx_uint32 ex,
                                       vx_uint32 ey);

/*! \brief Creates a reference to a list object, capable of holding many references.
 * \param [in] context The reference to the overall context.
 * \param [in] type The type of references to hold.
 * \param [in] initial The number of initial references to hold.
 * \if OVX_STRICT_1_0
 * \note Only <tt>\ref VX_TYPE_COORDINATES</tt> or <tt>\ref VX_TYPE_RECTANGLE</tt> is supported.
 * \endif
 * \return vx_list
 * \retval 0 No list was created.
 * \retval * A list was created.
 * \ingroup group_list
 */
vx_list vxCreateList(vx_context context, vx_enum type, vx_size initial);

/*! \brief Releases a reference of a list object.
 * The object may not be garbage collected until its total reference count is zero.
 * Once zero, all entries in the list and the list itself are released.
 * \param [in] list The pointer to the list to release.
 * \note After returning from this function the reference will be zeroed.
 * \ingroup group_list
 */
void vxReleaseList(vx_list *list);

/*! \brief Queries attributes from a list.
 * \param [in] list The list object.
 * \param [in] attribute The enumeration to query. Use a <tt>\ref vx_list_attribute_e</tt> enumeration.
 * \param [out] ptr The address of the variable to hold the attribute.
 * \param [in] size The byte size that ptr points to.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_list
 */
vx_status vxQueryList(vx_list list, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Retrieves an item from the list from named positions, but does not remove it.
 * \param [in] list The list to access.
 * \param [in,out] it The pointer to the iterator object. This object is internally
 * "reset" when <tt>\ref VX_LIST_FRONT</tt> or <tt>\ref VX_LIST_BACK</tt> are used and only updated when
 * <tt>\ref VX_LIST_NEXT</tt> or <tt>\ref VX_LIST_PREVIOUS</tt> are used. <tt>\ref VX_LIST_FIRST</tt> and <tt>\ref VX_LIST_LAST</tt>
 * do not modify the iterator.
 * \param [in] iter The <tt>\ref vx_list_iterator_e</tt> enumeration.
 * \note The specification makes no guarantees about the thread-safety of this function. If
 * <tt>\ref VX_LIST_FRONT</tt> or <tt>\ref VX_LIST_BACK</tt> has not been called, then <tt>\ref VX_LIST_NEXT</tt>
 * will map to <tt>\ref VX_LIST_FIRST</tt> for one call and <tt>\ref VX_LIST_PREVIOUS</tt> will map to
 * <tt>\ref VX_LIST_LAST</tt> for one call.
 * \ingroup group_list
 * \return vx_reference
 * \retval 0 The list is empty or the enumeration fell off the list.
 */
vx_reference vxGetListItem(vx_list list, vx_iterator *it, vx_enum iter);

/*! \brief Adds an item to the list, incrementing the reference count of the item.
 * \note No ordering of list is defined. Once added the list may become unsorted.
 * \param [in] list The list object.
 * \param [in] ref The reference to add to the list.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_FAILURE the list is full.
 * \retval VX_ERROR_INVALID_TYPE The supplied object was not of the type created for this list.
 * \ingroup group_list
 */
vx_status vxAddListItem(vx_list list, vx_reference ref);

/*! \brief Removes all items from the list.
 * \param [in] list The list to empty.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS The list is empty.
 * \retval VX_ERROR_INVALID_REFERENCE The list was not a vx_list.
 * \ingroup group_list
 */
vx_status vxEmptyList(vx_list list);

/*! \brief Sets the border mode for immediate mode functions.
 * \details Graph mode functions are unaffected by this function. If the
 * immediate mode function does not support the border mode, it must return
 * an error (\ref VX_ERROR_NOT_SUPPORTED).
 * \param [in] context A reference to the overall context.
 * \param [in] config A border structure which contains the value from
 * <tt>\ref vx_border_mode_e</tt> and a constant value for \ref VX_BORDER_MODE_CONSTANT.
 * \return vx_status
 * \retval VX_SUCCESS Success.
 * \retval VX_ERROR_INVALID_VALUE The mode value was not valid (or out of range).
 * \retval VX_ERROR_INVALID_REFERENCE The context was invalid.
 * \retval VX_ERROR_INVALID_PARAMETERS If config is NULL.
 * \retval VX_ERROR_NOT_SUPPORTED Returned if the border mode is not supported.
 * \note This is not guaranteed to be a thread-safe function with respect
 * to immediate mode execution. The assumed default value for immediate mode
 * functions is \ref VX_BORDER_MODE_UNDEFINED.
 * \ingroup group_context
 */
vx_status vxSetImmediateBorderMode(vx_context context, vx_border_mode_t *config);

/*! \brief Add the given parameter from a <tt>\ref vx_node</tt> to the graph.
 * \param [in] graph The graph reference which contains the node.
 * \param [in] parameter The parameter reference to add to the graph from the node.
 * \return Returns a <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS Parameter added to Graph.
 * \retval VX_ERROR_INVALID_REFERENCE The parameter was not a valid <tt>\ref vx_parameter</tt>
 * \retval VX_ERROR_INVALID_PARAMETER The parameter was of a node not in this
 * graph.
 * \ingroup group_graph
 */
vx_status vxAddParameterToGraph(vx_graph graph, vx_parameter parameter);

/*! \brief Sets a reference to the parameter on the graph. The implementation
 * must set this parameter on the originating node as well.
 * \param [in] graph The graph reference.
 * \param [in] index The parameter index.
 * \param [in] dir The <tt>\ref vx_direction_e</tt> of the parameter.
 * \param [in] value The reference to set to the parameter.
 * \return Returns a <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS Parameter set to Graph.
 * \retval VX_ERROR_INVALID_REFERENCE The value was not a valid <tt>\ref vx_reference</tt>
 * \retval VX_ERROR_INVALID_PARAMETER The parameter index was out of bounds or the
 * dir parameter was incorrect
 * \ingroup group_graph
 */
vx_status vxSetGraphParameterByIndex(vx_graph graph, vx_uint32 index, vx_enum dir, vx_reference value);

/*! \brief Retrieves the parameter object which describes the attributes of the
 * parameter and it's current value.
 * \param [in] graph The graph.
 * \param [in] index The index of the parameter.
 * \return Returns <tt>\ref vx_parameter</tt> reference.
 * \retval 0 if the index was out of bounds.
 * \retval * The parameter reference.
 * \post <tt>\ref vxReleaseParameter</tt>
 * \ingroup group_graph
 */
vx_parameter vxGetGraphParameterByIndex(vx_graph graph, vx_uint32 index);

/*! \brief Creates a reference to a keypoint object.
 * \param [in] c The reference to the overall context.
 * \post <tt>\ref vxReleaseKeypoint</tt>
 * \return Returns a <tt>\ref vx_keypoint</tt> reference or zero.
 * \ingroup group_keypoint
 */
vx_keypoint vxCreateKeypoint(vx_context c);

/*! \brief Gives the user access to the keypoint data.
 * \param [in] kp The reference to the keypoint.
 * \param [out] ptr The pointer to the location to store the pointer to a \ref
 * vx_keypoint_t structure. If NULL is passed, the reference count is still incremented.
 * If the value that ptr points to is NULL, the implementation will set ptr so that
 * the user can access the data. If the value that ptr points to is not NULL
 * the the implementation will copy the data to the supplied location.
 * \note This will cause the reference count to increment.
 * \post <tt>\ref vxCommitKeypoint</tt> must be called after access.
 * \return Returns a <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS User was given access to the keypoint.
 * \retval VX_ERROR_INVALID_REFERENCE The kp parameter was not a <tt>\ref vx_keypoint</tt>.
 * \ingroup group_keypoint
 */
vx_status vxAccessKeypoint(vx_keypoint kp, vx_keypoint_t **ptr);

/*! \brief Commits a keypoint's data back to the object.
 * \param [in] kp The reference to the keypoint object.
 * \param [in] ptr The pointer to the <tt>\ref vx_keypoint_t</tt> data. If NULL is
 * supplied, the reference count is still decremented.
 * \pre <tt>\ref vxAccessKeypoint</tt> must have been called before calling this function.
 * \return Returns a <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS User data was committed to the keypoint.
 * \retval VX_ERROR_INVALID_REFERENCE The kp parameter was not a <tt>\ref vx_keypoint</tt>.
 * \ingroup group_keypoint
 */
vx_status vxCommitKeypoint(vx_keypoint kp, vx_keypoint_t *ptr);

/*! \brief Releases a reference to a keypoint object. The object may not be garbage
 * collected until its total reference count is zero.
 * \param [in] kp The pointer to the keypoint to release.
 * \note After returning from this function the reference will be zeroed.
 * \ingroup group_keypoint
 */
void vxReleaseKeypoint(vx_keypoint *kp);

/*! \brief Creates a remap table object.
 * \param [in] context The reference to the overall context.
 * \param [in] src_width Width of the source image in pixel.
 * \param [in] src_height Height of the source image in pixels.
 * \param [in] dst_width Width of the destination image in pixels.
 * \param [in] dst_height Height of the destination image in pixels.
 * \ingroup group_remap
 * \return Returns <tt>\ref vx_remap</tt>
 * \retval 0 Object could not be created.
 * \retval * Object was created.
 */
vx_remap vxCreateRemap(vx_context context,
                       vx_uint32 src_width,
                       vx_uint32 src_height,
                       vx_uint32 dst_width,
                       vx_uint32 dst_height);

/*! \brief Release a reference to a remap table object. The object may not be
 * garbage collected until its total reference count is zero.
 * \param [in] table The pointer to the remap table to release.
 * \note After returning from this function the reference will be zeroed.
 * \ingroup group_remap
 */
void vxReleaseRemap(vx_remap *table);

/*! \brief Assigns a destination pixel mapping to the source pixel.
 * \param [in] table The remap table reference.
 * \param [in] dst_x The destination x coordinate.
 * \param [in] dst_y The destination y coordinate.
 * \param [in] src_x The source x coordinate in float representation to allow interpolation.
 * \param [in] src_y The source y coordinate in float representation to allow interpolation.
 * \ingroup group_remap
 * \return Returns a <tt>\ref vx_status_e</tt> enumeration.
 */
vx_status vxSetRemapPoint(vx_remap table,
                          vx_uint32 dst_x, vx_uint32 dst_y,
                          vx_float32 src_x, vx_float32 src_y);


/*! \brief Retrieves the source pixel point from a destination pixel.
 * \param [in] table The remap table reference.
 * \param [in] dst_x The destination x coordinate.
 * \param [in] dst_y The destination y coordinate.
 * \param [out] src_x The pointer to the location to store the source x coordinate in float representation to allow interpolation.
 * \param [out] src_y The pointer to the location to store the source y coordinate in float representation to allow interpolation.
 * \ingroup group_remap
 * \return Returns a <tt>\ref vx_status_e</tt> enumeration.
 */
vx_status vxGetRemapPoint(vx_remap table,
                          vx_uint32 dst_x, vx_uint32 dst_y,
                          vx_float32 *src_x, vx_float32 *src_y);

/*! \brief Queries attributes from a Remap table.
 * \param [in] r The remap to query.
 * \param [in] attribute The attribute to query. Use a <tt>\ref vx_remap_attribute_e</tt> enumeration.
 * \param [out] ptr The address of the variable to hold the attribute.
 * \param [in] size The byte size that ptr points to.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \ingroup group_remap
 */
vx_status vxQueryRemap(vx_remap r, vx_enum attribute, void *ptr, vx_size size);

/*! \brief Returns a boolean to indicate the state of graph verification.
 * \param [in] graph The reference to the graph to check.
 * \return vx_bool
 * \retval vx_true_e The graph is verified.
 * \retval vx_false_e The graph not verified. It must be verified before
 * execution either through <tt>\ref vxVerifyGraph</tt> or automatically through
 * <tt>\ref vxProcessGraph</tt>, <tt>\ref vxScheduleGraph</tt>.
 * \ingroup group_graph
 */
vx_bool vxIsGraphVerified(vx_graph graph);

#ifdef  __cplusplus
}
#endif

#endif
