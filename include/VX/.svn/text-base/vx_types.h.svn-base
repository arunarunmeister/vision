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

#ifndef _OPENVX_TYPES_H_
#define _OPENVX_TYPES_H_

/*!
 * \file vx_types.h
 * \brief The type definitions required by Open Vision Library.
 * \author Erik Rainey <erik.rainey@ti.com>
 *
 * \defgroup group_types OpenVX Types
 * \brief The types associated with OpenVX.
 *
 * \defgroup group_macros OpenVX Macros
 * \brief The macros associated with OpenVX.
 *
 * \defgroup group_defines OpenVX Defines
 * \brief The defines associated with OpenVX.
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#if defined(WIN32) || defined(UNDER_CE)
#include <windows.h>
#endif

/*! \brief An 8 bit ASCII character
 * \ingroup group_types
 */
typedef char     vx_char;

/*! \brief An 8-bit unsigned value
 * \ingroup group_types
 */
typedef uint8_t  vx_uint8;

/*! \brief A 16-bit unsigned value
 * \ingroup group_types
 */
typedef uint16_t vx_uint16;

/*! \brief A 32-bit unsigned value
 * \ingroup group_types
 */
typedef uint32_t vx_uint32;

/*! \brief A 64-bit unsigned value
 * \ingroup group_types
 */
typedef uint64_t vx_uint64;

/*! \brief An 8-bit signed value
 * \ingroup group_types
 */
typedef int8_t   vx_int8;

/*! \brief A 16-bit signed value
 * \ingroup group_types
 */
typedef int16_t  vx_int16;

/*! \brief A 32-bit signed value
 * \ingroup group_types
 */
typedef int32_t  vx_int32;

/*! \brief A 64-bit signed value
 * \ingroup group_types
 */
typedef int64_t  vx_int64;

#if defined(OVX_PLATFORM_SUPPORTS_16_FLOAT)

/*! \brief A 16-bit float value
 * \ingroup group_types
 */
typedef hfloat   vx_float16;
#endif

/*! \brief A 32-bit float value
 * \ingroup group_types
 */
typedef float    vx_float32;

/*! \brief A 64-bit float value (aka double)
 * \ingroup group_types
 */
typedef double   vx_float64;

/*! \brief A generic opaque reference.
 * \details A user should not assume that this can be casted directly to anything, however, sub-typed units can be cast back to this.
 * \ingroup group_reference
 */
#if (defined(WIN32) || defined(UNDER_CE)) && !defined(CYGWIN)
typedef HANDLE vx_reference;
#else
typedef uintptr_t vx_reference;
#endif

/*! \brief Sets the standard enumeration type size to be a fixed quantity.
 * All enumerable fields should use this type as the container to
 * enforce enumeration ranges and sizeof()s.
 * \ingroup group_types
 */
typedef int32_t vx_enum;

/*! \brief A wrapper of size_t to keep the naming convention uniform.
 * \ingroup group_types
 */
typedef size_t vx_size;

/*! \brief Used to hold a FOURCC code to describe the pixel format and color space.
 * \ingroup group_types
 */
typedef uint32_t vx_fourcc;

/*! \brief An opaque reference to a scalar.
 * \details A scalar can be up to 64 bits wide.
 * \see vxCreateScalar
 * \ingroup group_scalar
 * \extends vx_reference
 */
typedef vx_reference vx_scalar;

/*! \brief An opaque reference to a buffer.
 * \details Buffers are typically an array of scalars or defined data structures.
 * \see vxCreateBuffer
 * \ingroup group_buffer
 * \extends vx_reference
 */
typedef vx_reference vx_buffer;

/*! \brief An opaque reference to an image.
 * \see vxCreateImage
 * \ingroup group_image
 * \extends vx_reference
 */
typedef vx_reference vx_image;

/*! \brief An opaque reference to the descriptor of a kernel.
 * \see vxGetKernelByName
 * \see vxGetKernelByEnum
 * \ingroup group_kernel
 * \extends vx_reference
 */
typedef vx_reference vx_kernel;

/*! \brief An opaque reference to a single parameter.
 * \see vxGetParameterByName
 * \see vxGetParameterByIndex
 * \ingroup group_parameter
 * \extends vx_reference
 */
typedef vx_reference vx_parameter;

/*! \brief An opaque reference to a kernel node.
 * \see vxCreateNode
 * \ingroup group_node
 * \extends vx_reference
 */
typedef vx_reference vx_node;

/*! \brief An opaque reference to a graph
 * \see vxCreateGraph
 * \ingroup group_graph
 * \extends vx_reference
 */
typedef vx_reference vx_graph;

/*! \brief An opaque reference to the implementation context.
 * \see vxCreateContext
 * \ingroup group_context
 * \extends vx_reference
 */
typedef vx_reference vx_context;

/*! \brief The delay object. This is like a ring buffer of objects which is
 * maintained by the OpenVX implementation.
 * \see vxCreateImageDelay
 * \see vxCreateBufferDelay
 * \extends vx_reference
 * \ingroup group_delay
 */
typedef vx_reference vx_delay;

/*! \brief The rectangle object. This is an opaque set of 2 image coordinates.
 * \extends vx_reference
 * \ingroup group_rectangle
 */
typedef vx_reference vx_rectangle;

/*! \brief The Look-Up Table (LUT) Object.
 * \extends vx_reference
 * \ingroup group_lut
 */
typedef vx_reference vx_lut;

/*! \brief The Distribution object. This has a user defined number of bins over
 * a user defined range (within a uint32_t range)
 * \extends vx_reference
 * \ingroup group_distribution
 */
typedef vx_reference vx_distribution;

/*! \brief The Matrix Object. An MxN matrix of some unit type.
 * \extends vx_reference
 * \ingroup group_matrix
 */
typedef vx_reference vx_matrix;

/*! \brief The Image Pyramid object. A set of scaled images.
 * \extends vx_reference
 * \ingroup group_pyramid
 */
typedef vx_reference vx_pyramid;

/*! \brief The Threshold Object. A thresholding object contains the types and
 * limit values of the thresholding required.
 * \extends vx_reference
 * \ingroup group_threshold
 */
typedef vx_reference vx_threshold;

/*! \brief The Convolution Object. A user defined convolution kernel of MxM elements.
 * \extends vx_reference
 * \ingroup group_convolution
 */
typedef vx_reference vx_convolution;

/*! \brief The Coordinates Object. A set of pixel space coordinates.
 * \extends vx_reference
 * \ingroup group_coordinates
 */
typedef vx_reference vx_coordinates;

/*! \brief The List Object. A List can contain an arbitrary number of data objects.
 * \ingroup group_list
 */
typedef vx_reference vx_list;

/*! \brief An iterator for Lists.
 * \ingroup group_list
 */
typedef vx_reference vx_iterator;

/*! \brief The keypoint Object. An keypoint from a detector fed into a tracker or flow prediction.
 * \ingroup group_keypoint
 */
typedef vx_reference vx_keypoint;

/*! \brief The remap table Object. A remap table contains per pixel mapping of
 * output pixels to input pixels.
 * \ingroup group_remap
 */
typedef vx_reference vx_remap;

/*! \brief A boolean value.
 * This allows 0 to be false, as it is in C, and any non-zero to be true.
 * \code
 * vx_bool ret = vx_true_e;
 * if (ret) printf("true!\n");
 * ret = vx_false_e;
 * if (!ret) printf("false!\n");
 * \endcode
 * This would print both strings.
 * \ingroup group_types
 */
typedef enum _vx_bool_e {
    /*! \brief The "false" value. */
    vx_false_e = 0,
    /*! \brief The "true" value. */
    vx_true_e,
} vx_bool;

/*! \brief An abstract handle to a target.
 * \ingroup group_target
 */
typedef vx_reference vx_target;

/*! \brief The type enumeration lists all the known types in OpenVX.
 * \ingroup group_types
 */
enum vx_type_e {
    VX_TYPE_INVALID         = 0x000,/*!< \internal An invalid type value. When passed an error must be returned. */
    VX_TYPE_CHAR            = 0x001,/*!< \brief A <tt>\ref vx_char</tt> */
    VX_TYPE_INT8            = 0x002,/*!< \brief A <tt>\ref vx_int8</tt> */
    VX_TYPE_UINT8           = 0x003,/*!< \brief A <tt>\ref vx_uint8</tt> */
    VX_TYPE_INT16           = 0x004,/*!< \brief A <tt>\ref vx_int16</tt> */
    VX_TYPE_UINT16          = 0x005,/*!< \brief A <tt>\ref vx_uint16</tt> */
    VX_TYPE_INT32           = 0x006,/*!< \brief A <tt>\ref vx_int32</tt> */
    VX_TYPE_UINT32          = 0x007,/*!< \brief A <tt>\ref vx_uint32</tt> */
    VX_TYPE_INT64           = 0x008,/*!< \brief A <tt>\ref vx_int64</tt> */
    VX_TYPE_UINT64          = 0x009,/*!< \brief A <tt>\ref vx_uint64</tt> */
    VX_TYPE_FLOAT32         = 0x00A,/*!< \brief A <tt>\ref vx_float32</tt> */
    VX_TYPE_FLOAT64         = 0x00B,/*!< \brief A <tt>\ref vx_float64</tt> */
    VX_TYPE_ENUM            = 0x00C,/*!< \brief A <tt>\ref vx_enum</tt>. Equivalent in size to a <tt>\ref vx_int32</tt> */
    VX_TYPE_SIZE            = 0x00D,/*!< \brief A <tt>\ref vx_size</tt> */
    VX_TYPE_FOURCC          = 0x00E,/*!< \brief A <tt>\ref vx_fourcc</tt> */
#if defined(OVX_PLATFORM_SUPPORTS_16_FLOAT)
    VX_TYPE_FLOAT16         = 0x00F,/*!< \brief A <tt>\ref vx_float16</tt> */
#endif
    VX_TYPE_BOOL            = 0x010,/*!< \brief A <tt>\ref vx_bool</tt> */

    /* add new scalar types here */

    VX_TYPE_SCALAR_MAX,     /*!< \internal A floating value for comparison between scalars and objects */

    VX_TYPE_REFERENCE       = 0x020,/*!< \brief A <tt>\ref vx_reference</tt> */
    VX_TYPE_CONTEXT         = 0x021,/*!< \brief A <tt>\ref vx_context</tt> */
    VX_TYPE_GRAPH           = 0x022,/*!< \brief A <tt>\ref vx_graph</tt> */
    VX_TYPE_NODE            = 0x023,/*!< \brief A <tt>\ref vx_node</tt> */
    VX_TYPE_KERNEL          = 0x024,/*!< \brief A <tt>\ref vx_kernel</tt> */
    VX_TYPE_PARAMETER       = 0x025,/*!< \brief A <tt>\ref vx_parameter</tt> */
    VX_TYPE_DELAY           = 0x026,/*!< \brief A <tt>\ref vx_delay</tt> */
    VX_TYPE_TARGET          = 0x027,/*!< \brief A <tt>\ref vx_target</tt> */
    VX_TYPE_RECTANGLE       = 0x028,/*!< \brief A <tt>\ref vx_rectangle</tt> */
    VX_TYPE_LUT             = 0x029,/*!< \brief A <tt>\ref vx_lut</tt> */
    VX_TYPE_DISTRIBUTION    = 0x02A,/*!< \brief A <tt>\ref vx_distribution</tt> */
    VX_TYPE_PYRAMID         = 0x02B,/*!< \brief A <tt>\ref vx_pyramid</tt> */
    VX_TYPE_THRESHOLD       = 0x02C,/*!< \brief A <tt>\ref vx_threshold</tt> */
    VX_TYPE_MATRIX          = 0x02D,/*!< \brief A <tt>\ref vx_matrix</tt> */
    VX_TYPE_CONVOLUTION     = 0x02E,/*!< \brief A <tt>\ref vx_convolution</tt> */
    VX_TYPE_COORDINATES     = 0x02F,/*!< \brief A <tt>\ref vx_coordinates</tt> */
    VX_TYPE_SCALAR          = 0x030,/*!< \brief A <tt>\ref vx_scalar</tt> when needed to be completely generic for kernel validation. */
    VX_TYPE_LIST            = 0x031,/*!< \brief A <tt>\ref vx_list</tt> */
    VX_TYPE_BUFFER          = 0x032,/*!< \brief A <tt>\ref vx_buffer</tt> */
    VX_TYPE_IMAGE           = 0x033,/*!< \brief A <tt>\ref vx_image</tt> */
    VX_TYPE_KEYPOINT        = 0x034,/*!< \brief A <tt>\ref vx_keypoint</tt> */
    VX_TYPE_REMAP           = 0x035,/*!< \brief A <tt>\ref vx_remap</tt> */
    /* \todo add new object types here */

    VX_TYPE_OBJECT_MAX,     /*!< \internal A value used for bound checking the object types */
};

/*! \brief The enumeration of all status codes.
 * \see vx_status.
 * \ingroup group_types
 */
enum vx_status_e {
    VX_STATUS_MIN                       = -24,/*!< \brief Indicates the lower bound of status codes in VX. Used for bounds checks only */
    /* add new codes here */
    VX_ERROR_REFERENCE_NONZERO          = -23,/*!< \brief Indicates that an operation could not complete due to a reference count being non-zero. */
    VX_ERROR_MULTIPLE_WRITERS           = -22,/*!< \brief Indicates that the graph had more than one node outputting to the same data object. This is an invalid graph structure. */
    VX_ERROR_GRAPH_ABANDONED            = -21,/*!< \brief Indicates that the graph was stopped due to an error or a callback which abandoned execution */
    VX_ERROR_GRAPH_SCHEDULED            = -20,/*!< \brief Indicates that the supplied graph already has been scheduled and may be currently executing */
    VX_ERROR_INVALID_NODE               = -19,/*!< \brief Indicates that the supplied node could not be created.*/
    VX_ERROR_INVALID_GRAPH              = -18,/*!< \brief Indicates that the supplied graph had invalid connections (cycles) */
    VX_ERROR_INVALID_TYPE               = -17,/*!< \brief Indicates that the supplied type parameter was incorrect. */
    VX_ERROR_INVALID_VALUE              = -16,/*!< \brief Indicates that the supplied parameter had an incorrect value */
    VX_ERROR_INVALID_DIMENSION          = -15,/*!< \brief Indicates that the supplied parameter was too big or too small in dimension. */
    VX_ERROR_INVALID_FORMAT             = -14,/*!< \brief Indicates that the supplied parameter was in an invalid format. */
    VX_ERROR_INVALID_LINK               = -13,/*!< \brief Indicates that the link was not possible as specified. The parameters were incompatible. */
    VX_ERROR_INVALID_REFERENCE          = -12,/*!< \brief Indicates that the reference provided was not valid. */
    VX_ERROR_INVALID_MODULE             = -11,/*!< \brief This is returned from <tt>\ref vxLoadKernels</tt> when the module did not contain the entry point */
    VX_ERROR_INVALID_PARAMETERS         = -10,/*!< \brief The supplied parameter information did not match the kernel contract */
    VX_ERROR_OPTIMIZED_AWAY             = -9,/*!< \brief This code indicates that the object refered to has been optimized out of existence. */
    VX_ERROR_NO_MEMORY                  = -8,/*!< \brief An internal or implicit allocation failed. Typically catastrophic. After detection, deconstruct the context. \see vxVerifyGraph */
    VX_ERROR_NO_RESOURCES               = -7,/*!< \brief An internal or implicit resource could not be acquired (not memory). This is typically catastrophic. After detection, deconstruct the context. \see vxVerifyGraph */
    VX_ERROR_NOT_COMPATIBLE             = -6,/*!< \brief The attempt to link two parameters together failed due to type incompatibilty */
    VX_ERROR_NOT_ALLOCATED              = -5,/*!< \brief Indicates to the system that the parameter must be allocated by the system.  */
    VX_ERROR_NOT_SUFFICIENT             = -4,/*!< \brief The given graph has failed verification due to an insufficient number of required parameters which can not be automatically created. Typically this indicate required atomic parameters. \see vxVerifyGraph */
    VX_ERROR_NOT_SUPPORTED              = -3,/*!< \brief The requested set of parameter produce a configuration which can not be supported. Refer to the supplied documentation on the configured kernels. \see vx_kernel_e */
    VX_ERROR_NOT_IMPLEMENTED            = -2,/*!< \brief The requested kernel is missing. \see vx_kernel_e vxGetKernelByName */
    VX_FAILURE                          = -1,/*!< \brief The generic error code, used when no other will describe the error. */
    VX_SUCCESS                          = 0,/*!< \brief No error. */
};

/*! \brief A formal status type with known fixed size.
 * \see vx_status_e
 * \ingroup group_types
 */
typedef vx_enum vx_status;

/*! \brief The formal typedef of the response from the callback.
 * \see vx_action_e
 * \ingroup group_graph
 */
typedef vx_enum vx_action;

/*! \brief A callback to the client once a particular node has completed.
 * \see vx_action
 * \see vxAddCallbackToNode
 * \param [in] node The node which the callback was attached.
 * \return Returns an action code from <tt>\ref vx_action_e</tt>.
 * \ingroup group_node
 */
typedef vx_action (*vx_nodecomplete_f)(vx_node node);

/*! \brief Vendor ID's are 2 nibbles in size and are located in the upper byte of
 * the 4 bytes of an enumeration.
 * \ingroup group_defines
 */
#define VX_VENDOR_MASK                      (0xFFF00000)

/*! \brief A type mask removes the scalar/object type from the attribute.
 * It is 3 nibbles in size and in contained between the third and second byte.
 * \see vx_type_e
 * \ingroup group_defines
 */
#define VX_TYPE_MASK                        (0x000FFF00)

/*! \brief A library is a set of vision kernels with its own id supplied by a vendor.
 * The vendor defines the library ID. The range is \f$ [0,2^{8}-1] \f$ inclusive.
 * \ingroup group_defines
 */
#define VX_LIBRARY_MASK                     (0x000FF000)

/*! \brief An individual kernel in a library has its own unique ID within \f$ [0,2^{12}-1] \f$ (inclusive).
 * \ingroup group_defines
 */
#define VX_KERNEL_MASK                      (0x00000FFF)

/*! \brief An object's attribute ID is within the range of \f$ [0,2^{8}-1] \f$ (inclusive).
 * \ingroup group_defines
 */
#define VX_ATTRIBUTE_ID_MASK                (0x000000FF)

/*! \brief A type of enumeration. The valid range is between \f$ [0,2^{8}-1] \f$ (inclusive).
 * \ingroup group_defines
 */
#define VX_ENUM_TYPE_MASK                   (0x000FF000)

/*! \brief A generic enumeration list can have values between \f$ [0,2^{12}-1] \f$ (inclusive).
 * \ingroup group_defines
 */
#define VX_ENUM_MASK                        (0x00000FFF)

/*! \brief A macro to extract the vendor ID from the enumerated value.
 * \ingroup group_macros
 */
#define VX_VENDOR(e)                        (((vx_uint32)e & VX_VENDOR_MASK) >> 20)

/*! \brief A macro to extract the type from an enumerated attribute value.
 * \ingroup group_macros
 */
#define VX_TYPE(e)                          (((vx_uint32)e & VX_TYPE_MASK) >> 8)

/*! \brief A macro to extract the enum type from an enumerated value.
 * \ingroup group_macros
 */
#define VX_ENUM_TYPE(e)                     (((vx_uint32)e & VX_ENUM_TYPE_MASK) >> 12)

/*! \brief A macro to extract the kernel library enumeration from a enumerated kernel value.
 * \ingroup group_macros
 */
#define VX_LIBRARY(e)                       (((vx_uint32)e & VX_LIBRARY_MASK) >> 12)

#if defined(_LITTLE_ENDIAN_) || (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || defined(_WIN32) || defined(WIN32)
#define VX_FOURCC(a,b,c,d)                  ((a) | (b << 8) | (c << 16) | (d << 24))
#define VX_ATTRIBUTE_BASE(vendor, object)   (((vendor) << 20) | (object << 8))
#define VX_KERNEL_BASE(vendor, lib)         (((vendor) << 20) | (lib << 12))
#define VX_ENUM_BASE(vendor, id)            (((vendor) << 20) | (id << 12))
#elif defined(_BIG_ENDIAN_) || (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define VX_FOURCC(a,b,c,d)                  ((d) | (c << 8) | (b << 16) | (a << 24))
#define VX_ATTRIBUTE_BASE(vendor, object)   ((vendor) | (object << 12))
#define VX_KERNEL_BASE(vendor, lib)         ((vendor) | (lib << 12))
#define VX_ENUM_BASE(vendor, id)            ((vendor) | (id << 12))
#else
#error "Endian-ness must be defined!"
#endif

/*! \def VX_FOURCC
 * \brief Converts a set of four chars into a uint32_t container of a FOURCC code.
 * \note Use a <tt>\ref vx_fourcc</tt> variable to hold the value.
 * \ingroup group_macros
 */

/*! \def VX_ATTRIBUTE_BASE
 * \brief Defines the manner in which to combine the Vendor and Object IDs to get
 * the base value of the enumeration.
 * \ingroup group_macros
 */

/*! \def VX_KERNEL_BASE
 * \brief Defines the manner in which to combine the Vendor and Library IDs to get
 * the base value of the enumeration.
 * \ingroup group_macros
 */

/*! \def VX_ENUM_BASE
 * \brief Defines the manner in which to combine the Vendor and Object IDs to get
 * the base value of the enumeration.
 * \details From any enumerated value (with exceptions), the vendor, and enumeration
 * type should be extractable. Those types which are exceptions are
 * <tt>\ref vx_vendor_id_e</tt>, <tt>\ref vx_type_e</tt>, <tt>\ref vx_enum_e</tt>, <tt>\ref vx_fourcc_e</tt>, and vx_bool.
 * \ingroup group_macros
 */

/*! \brief The set of supported enumerations in OpenVX.
 * \details These can be extracted from enumerated values using <tt>\ref VX_ENUM_TYPE</tt>.
 * \ingroup group_types
 */
enum vx_enum_e {
    VX_ENUM_DIRECTION       = 0x00, /*!< \brief Parameter Direction */
    VX_ENUM_ACTION          = 0x01, /*!< \brief Action Codes */
    VX_ENUM_HINT            = 0x02, /*!< \brief Hint Values */
    VX_ENUM_DIRECTIVE       = 0x03, /*!< \brief Directive Values */
    VX_ENUM_INTERPOLATION   = 0x04, /*!< \brief Interpolation Types */
    VX_ENUM_OVERFLOW        = 0x05, /*!< \brief Overflow Policies */
    VX_ENUM_COLOR_SPACE     = 0x06, /*!< \brief Color Space */
    VX_ENUM_COLOR_RANGE     = 0x07, /*!< \brief Color Space Range */
    VX_ENUM_PARAMETER_STATE = 0x08, /*!< \brief Parameter State */
    VX_ENUM_CHANNEL         = 0x09, /*!< \brief Channel Name */
    VX_ENUM_CONVERT_POLICY  = 0x0A, /*!< \brief Convert Policy */
    VX_ENUM_THRESHOLD_TYPE  = 0x0B, /*!< \brief Threshold Type List */
    VX_ENUM_BORDER_MODE     = 0x0C, /*!< \brief Border Mode List */
    VX_ENUM_LIST_ITERATORS  = 0x0D, /*!< \brief List Iteration Names */
    VX_ENUM_COMPARISON      = 0x0E, /*!< \brief Comparison Values */
    VX_ENUM_IMPORT_MEM      = 0x0F, /*!< \brief The memory import enumeration */
    VX_ENUM_TERM_CRITERIA   = 0x10, /*!< \brief A termination criteria */
    VX_ENUM_NORM_TYPE       = 0x11, /*!< \brief A norm type. */
};

/*! \brief A return code enumeration from a <tt>\ref vx_nodecomplete_f</tt> during execution.
 * \see vxAssignNodeCallback
 * \ingroup group_graph
 */
enum vx_action_e {
    /*! \brief Continue executing the graph with no changes. */
    VX_ACTION_CONTINUE = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_ACTION) + 0x0,
    /*! \brief Stop executing the graph at the current point and restart from the beginning. */
    VX_ACTION_RESTART  = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_ACTION) + 0x1,
    /*! \brief Stop executing the graph. */
    VX_ACTION_ABANDON  = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_ACTION) + 0x2,
};

/*! \brief An indication of how a kernel will treat the given parameter.
 * \ingroup group_parameter
 */
enum vx_direction_e {
    /*! \brief The parameter is an input only. */
    VX_INPUT = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_DIRECTION) + 0x0,
    /*! \brief The parameter is an output only. */
    VX_OUTPUT = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_DIRECTION) + 0x1,
    /*! \brief The parameter is both an input and output. */
    VX_BIDIRECTIONAL = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_DIRECTION) + 0x2,

    /*! \brief A synonym for <tt>\ref VX_INPUT</tt> */
    VX_READ_ONLY = VX_INPUT,
    /*! \brief A synonym for <tt>\ref VX_OUTPUT</tt> */
    VX_WRITE_ONLY = VX_OUTPUT,
    /*! \brief A synonym for <tt>\ref VX_BIDIRECTIONAL</tt> */
    VX_READ_AND_WRITE = VX_BIDIRECTIONAL,
};

/*! \brief These enumerations are given to the vxHint API to enable/disable platform
 * optimizations and/or features. Hints are optional and usually will be vendor
 * specific.
 * \see vxHint
 * \ingroup group_hint
 */
enum vx_hint_e {
    /*! \brief This indicates to the implementation that the user wants to disable
     * any parallelization techniques. Implementations may not be parallelized,
     * so this is a hint only.
     */
    VX_HINT_SERIALIZE = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_HINT) + 0x0,
};

/*! \brief These enumerations are given to the vxDirective API to enable/disable
 * platform optimizations and/or features. Directives are not optional and
 * usually will be vendor specific, by defining a vendor range of directives and
 * starting their enumeration from there.
 * \see vxDirective
 * \ingroup group_directive
 */
enum vx_directive_e {
    /*! \brief Disables recording information for graph debugging */
    VX_DIRECTIVE_DISABLE_LOGGING = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_DIRECTIVE) + 0x0,
    /*! \brief Enables recording information for graph debugging */
    VX_DIRECTIVE_ENABLE_LOGGING = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_DIRECTIVE) + 0x1,
};

/*! \brief The Conversion Policy Enumeration.
 * \ingroup group_types
 */
enum vx_convert_policy_e {
    /*! \brief Results are the least significant bits of the output operand, as if
     * stored in two's complement binary format in the size of its bit-depth.
     */
    VX_CONVERT_POLICY_TRUNCATE = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_CONVERT_POLICY) + 0x0,
    /*! \brief Results are saturated to the bit depth of the output operand. */
    VX_CONVERT_POLICY_SATURATE = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_CONVERT_POLICY) + 0x1,
};

/*! \brief Based on the FOURCC definition referenced on http://fourcc.org.
 * \note Use <tt>\ref vx_fourcc</tt> to contain these values.
 * \ingroup group_types
 */
enum vx_fourcc_e {
    /*! \brief A virtual image of no defined type */
    FOURCC_VIRT = VX_TYPE_IMAGE,
    /*! \brief A single plane of 24 bit pixel as 3 interleaved 8 bit units of
     * R then G then B data. This uses the BT709 full range by default.
     */
    FOURCC_RGB  = VX_FOURCC('R','G','B','2'),
    /*! \brief A single plane of 32 bit pixel as 4 interleaved 8 bit units of
     * R then G then B data, then a "don't care" byte.
     * This uses the BT709 full range by default.
     */
    FOURCC_RGBX = VX_FOURCC('R','G','B','A'),
    /*! \brief A 2 plane YUV format of Luma (Y) and interleaved UV data at
     * 4:2:0 sampling. This uses the BT709 full range by default.
     */
    FOURCC_NV12 = VX_FOURCC('N','V','1','2'),
    /*! \brief A 2 plane YUV format of Luma (Y) and interleaved VU data at
     * 4:2:0 sampling. This uses the BT709 full range by default.
     */
    FOURCC_NV21 = VX_FOURCC('N','V','2','1'),
    /*! \brief A single plane of 32 bit macro pixel of U0, Y0, V0, Y1 bytes.
     * This uses the BT709 full range by default.
     */
    FOURCC_UYVY = VX_FOURCC('U','Y','V','Y'),
    /*! \brief A single plane of 32 bit macro pixel of Y0, U0, Y1, V0 bytes.
     * This uses the BT709 full range by default.
     */
    FOURCC_YUYV = VX_FOURCC('Y','U','Y','V'),
    /*! \brief A 3 plane of 8 bit 4:2:0 sampled Y, U, V planes.
     * This uses the BT709 full range by default.
     */
    FOURCC_IYUV = VX_FOURCC('I','Y','U','V'),
    /*! \brief A 3 plane of 8 bit 4:4:4 sampled Y, U, V planes.
     * This uses the BT709 full range by default.
     */
    FOURCC_YUV4 = VX_FOURCC('Y','U','V','4'),
    /*! \brief A single plane of unsigned 8 bit data.
     * The range of data is not specified, as it may be extracted from a YUV or
     * generated.
     */
    FOURCC_U8 = VX_FOURCC('U','0','0','8'),
    /*! \brief A single place of unsigned 16 bit data.
     * The range of data is not specified, as it may be extracted from a YUV or
     * generated.
     */
    FOURCC_U16  = VX_FOURCC('U','0','1','6'),
    /*! \brief A single place of signed 16 bit data.
     * The range of data is not specified, as it may be extracted from a YUV or
     * generated.
     */
    FOURCC_S16  = VX_FOURCC('S','0','1','6'),
    /*! \brief A single place of unsigned 32 bit data.
     * The range of data is not specified, as it may be extracted from a YUV or
     * generated.
     */
    FOURCC_U32  = VX_FOURCC('U','0','3','2'),
    /*! \brief A single place of unsigned 32 bit data.
     * The range of data is not specified, as it may be extracted from a YUV or
     * generated.
     */
    FOURCC_S32  = VX_FOURCC('S','0','3','2'),
};

/*! \brief The reference attributes list
 * \ingroup group_reference
 */
enum vx_reference_attribute_e {
    /*! \brief Returns the reference count of the object. Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_REF_ATTRIBUTE_COUNT = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_REFERENCE) + 0x0,
    /*! \brief Returns the <tt>\ref vx_type_e</tt> of the reference. Use a <tt>\ref vx_enum</tt> parameter. */
    VX_REF_ATTRIBUTE_TYPE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_REFERENCE) + 0x1,
};

/*! \brief A list of context attributes
 * \ingroup group_context
 */
enum vx_context_attribute_e {
    /*! \brief Used to query the unique vendor ID. Use a <tt>\ref vx_uint16</tt>. */
    VX_CONTEXT_ATTRIBUTE_VENDOR_ID = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_CONTEXT) + 0x0,
    /*! \brief Used to query the OpenVX Version Number. Use a <tt>\ref vx_uint16</tt> */
    VX_CONTEXT_ATTRIBUTE_VERSION = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_CONTEXT) + 0x1,
    /*! \brief Used to query the context for the number of active kernels. Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_CONTEXT_ATTRIBUTE_NUMKERNELS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_CONTEXT) + 0x2,
    /*! \brief Used to query the context for the number of active modules. Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_CONTEXT_ATTRIBUTE_NUMMODULES = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_CONTEXT) + 0x3,
    /*! \brief Used to query the context for the number of active references. Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_CONTEXT_ATTRIBUTE_NUMREFS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_CONTEXT) + 0x4,
    /*! \brief Used to query the context for the number of active targets. Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_CONTEXT_ATTRIBUTE_NUMTARGETS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_CONTEXT) + 0x5,
    /*! \brief Used to query the context for it's implementation name. Use a <tt>\ref vx_char</tt>[\ref VX_MAX_IMPLEMENTATION_NAME] array */
    VX_CONTEXT_ATTRIBUTE_IMPLEMENTATION = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_CONTEXT) + 0x6,
    /*! \brief Used to query the number of bytes in the extensions string. Use a <tt>\ref vx_size</tt> parameter. */
    VX_CONTEXT_ATTRIBUTE_EXTENSIONS_SIZE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_CONTEXT) + 0x7,
    /*! \brief Used to query the number of bytes in the extensions string. Use a <tt>\ref vx_char</tt> pointer allocated to the size returned from <tt>\ref VX_CONTEXT_ATTRIBUTE_EXTENSIONS_SIZE</tt>. */
    VX_CONTEXT_ATTRIBUTE_EXTENSIONS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_CONTEXT) + 0x8,
    /*! \brief The maximum range of either dimension that a convolution matrix
     * may be created with. Use a <tt>\ref vx_size</tt> parameter.
     * Each vendor will have to support centered kernels of size w X h, where both w
     * and h are odd numbers, 3 <= w <= n and 3 <= h <= n, where n is the value of the
     * <tt>\ref VX_CONTEXT_ATTRIBUTE_CONVOLUTION_MAXIMUM_DIMENSION</tt> attribute. n is an odd
     * number that should not be smaller than 9. w and h may or may not be equal to
     * each other. All combinations of w and h meeting the conditions above should be
     * supported. The behavior of <tt>\ref vxCreateConvolution</tt> is undefined for values
     * larger than the value returned by this attribute.
     */
    VX_CONTEXT_ATTRIBUTE_CONVOLUTION_MAXIMUM_DIMENSION = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_CONTEXT) + 0x9,
};

/*! \brief The kernel attributes list
 * \ingroup group_kernel
 */
enum vx_kernel_attribute_e {
    /*! \brief Used to query a kernel for the number of parameters the kernel
     * supports. Use a <tt>\ref vx_uint32</tt> parameter.
     */
    VX_KERNEL_ATTRIBUTE_NUMPARAMS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_KERNEL) + 0x0,
    /*! \brief Used to query the name of the kernel. Not settable.
     * Use a <tt>\ref vx_char</tt>[\ref VX_MAX_KERNEL_NAME] array
     */
    VX_KERNEL_ATTRIBUTE_NAME = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_KERNEL) + 0x1,
    /*! \brief Used to query the enum of the kernel. Not settable.
     * Use a <tt>\ref vx_enum</tt> parameter.
     */
    VX_KERNEL_ATTRIBUTE_ENUM = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_KERNEL) + 0x2,
    /*! \brief The local data area allocated with each kernel when it becomes a
     * node. Use a <tt>\ref vx_size</tt> parameter.
     * \note If not set it will default to zero.
     */
    VX_KERNEL_ATTRIBUTE_LOCAL_DATA_SIZE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_KERNEL) + 0x3,
    /*! \brief The local data pointer allocate with each kernel when it becomes
     * a node.Use a void pointer parameter.
     * Use a <tt>\ref vx_size</tt> parameter.
     */
    VX_KERNEL_ATTRIBUTE_LOCAL_DATA_PTR = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_KERNEL) + 0x4,
};

/*! \brief The node attributes list
 * \ingroup group_node
 */
enum vx_node_attribute_e {
    /*! \brief Used to query the status of node execution. Use a <tt>\ref vx_status</tt> parameter. */
    VX_NODE_ATTRIBUTE_STATUS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_NODE) + 0x0,
    /*! \brief Used to query the performance of the node execution. Use a <tt>\ref vx_perf_t</tt> parameter. */
    VX_NODE_ATTRIBUTE_PERFORMANCE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_NODE) + 0x1,
    /*! \brief Used to get or set the border mode of the node.
     * Use a <tt>\ref vx_border_mode_t</tt> structure.
     */
    VX_NODE_ATTRIBUTE_BORDER_MODE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_NODE) + 0x2,
    /*! \brief Used to indicate the size of the kernel local memory area.
     * Use a <tt>\ref vx_size</tt> parameter.
     */
    VX_NODE_ATTRIBUTE_LOCAL_DATA_SIZE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_NODE) + 0x3,
    /*! \brief Used to indicate the pointer kernel local memory area.
     * Use a void * parameter.
     */
    VX_NODE_ATTRIBUTE_LOCAL_DATA_PTR = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_NODE) + 0x4,
};

/*! \brief The parameter attributes list
 * \ingroup group_parameter
 */
enum vx_parameter_attribute_e {
    /*! \brief Used to query a parameter for its index value on the kernel it is associated with. Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_PARAMETER_ATTRIBUTE_INDEX = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_PARAMETER) + 0x0,
    /*! \brief Used to query a parameter for its direction value on the kernel it is associated with. Use a <tt>\ref vx_enum</tt> parameter. */
    VX_PARAMETER_ATTRIBUTE_DIRECTION = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_PARAMETER) + 0x1,
    /*!< \brief Used to query a parameter for its size in bytes or if it is a <tt>\ref vx_image</tt> or <tt>\ref vx_buffer</tt> its <tt>\ref vx_type_e</tt> will be returned. Use a <tt>\ref vx_enum</tt> parameter. */
    VX_PARAMETER_ATTRIBUTE_TYPE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_PARAMETER) + 0x2,
    /*! \brief Used to query a parameter for its state. A value in <tt>\ref vx_parameter_state_e</tt> will be returned. Use a <tt>\ref vx_enum</tt> parameter. */
    VX_PARAMETER_ATTRIBUTE_STATE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_PARAMETER) + 0x3,
    /*!< \brief Used to extract the reference contained in the parameter. Use a <tt>\ref vx_reference</tt> parameter.  */
    VX_PARAMETER_ATTRIBUTE_REF = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_PARAMETER) + 0x4,
};

/*! \brief The image attributes list
 * \ingroup group_image
 */
enum vx_image_attribute_e {
    /*! \brief Used to query an image for its height. Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_IMAGE_ATTRIBUTE_WIDTH = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_IMAGE) + 0x0,
    /*! \brief Used to query an image for its width. Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_IMAGE_ATTRIBUTE_HEIGHT = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_IMAGE) + 0x1,
    /*! \brief Used to query an image for its format. Use a <tt>\ref vx_fourcc</tt> parameter. */
    VX_IMAGE_ATTRIBUTE_FORMAT = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_IMAGE) + 0x2,
    /*! \brief Used to query an image for its number of planes. Use a <tt>\ref vx_size</tt> parameter. */
    VX_IMAGE_ATTRIBUTE_PLANES = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_IMAGE) + 0x3,
    /*! \brief Used to query an image for its color space (see <tt>\ref vx_color_space_e</tt>). Use a <tt>\ref vx_enum</tt> parameter. */
    VX_IMAGE_ATTRIBUTE_SPACE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_IMAGE) + 0x4,
    /*! \brief Used to query an image for its channel range (see <tt>\ref vx_channel_range_e</tt>). Use a <tt>\ref vx_enum</tt> parameter. */
    VX_IMAGE_ATTRIBUTE_RANGE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_IMAGE) + 0x5,
    /*! \brief Used to query an image for its total number of bytes. Use a <tt>\ref vx_size</tt> parameter. */
    VX_IMAGE_ATTRIBUTE_SIZE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_IMAGE) + 0x6,
};

/*! \brief The scalar attributes list.
 * \ingroup group_scalar
 */
enum vx_scalar_attribute_e {
    /*! \brief Used to query the type of atomic is contained in the scalar. Use a <tt>\ref vx_enum</tt> parameter.*/
    VX_SCALAR_ATTRIBUTE_TYPE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_SCALAR) + 0x0,
};

/*! \brief The buffer attributes list
 * \ingroup group_buffer
 */
enum vx_buffer_attribute_e {
    /*! \brief Used to query a buffers unit size. Use a <tt>\ref vx_size</tt> parameter. */
    VX_BUFFER_ATTRIBUTE_UNITSIZE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_BUFFER) + 0x0,
    /*! \brief Used to query a buffers number of units. Use a <tt>\ref vx_size</tt> parameter. */
    VX_BUFFER_ATTRIBUTE_NUMUNITS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_BUFFER) + 0x1,
    /*! \brief Used to query a buffer total bytes size. Use a <tt>\ref vx_size</tt> parameter. */
    VX_BUFFER_ATTRIBUTE_SIZE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_BUFFER) + 0x2,
};

/*! \brief The graph attributes list
 * \ingroup group_graph
 */
enum vx_graph_attribute_e {
    /*! \brief Returns the number of nodes in a graph. Use a <tt>\ref vx_uint32</tt> parameter.*/
    VX_GRAPH_ATTRIBUTE_NUMNODES = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_GRAPH) + 0x0,
    /*! \brief Returns the overall status of the graph. Use a <tt>\ref vx_status</tt> parameter.*/
    VX_GRAPH_ATTRIBUTE_STATUS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_GRAPH) + 0x1,
    /*! \brief Returns the overall performance of the graph. Use a <tt>\ref vx_perf_t</tt> parameter. */
    VX_GRAPH_ATTRIBUTE_PERFORMANCE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_GRAPH) + 0x2,
    /*! \brief Returns the number of explicitly declared parameters on the graph. Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_GRAPH_ATTRIBUTE_NUMPARAMETERS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_GRAPH) + 0x3,
};

/*! \brief The target attributes list
 * \ingroup group_target
 */
enum vx_target_attribute_e {
    /*! \brief Returns the index of the given target. Use a <tt>\ref vx_uint32</tt> parameter.*/
    VX_TARGET_ATTRIBUTE_INDEX = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_TARGET) + 0x0,
    /*! \brief Returns the name of the given target in the format "vendor.vendor_string".
     * Use a <tt>\ref vx_char</tt>[\ref VX_MAX_KERNEL_NAME] array
     */
    VX_TARGET_ATTRIBUTE_NAME = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_TARGET) + 0x1,
    /*! \brief Returns the number of kernels that the target is capable of processing.
     * This is then used to allocate a table which is then filled.
     * Use a <tt>\ref vx_uint32</tt> parameter.
     */
    VX_TARGET_ATTRIBUTE_NUMKERNELS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_TARGET) + 0x2,
    /*! \brief Returns the table of all the kernels that a given target can execute.
     *  Use a <tt>\ref vx_target_kernel_t</tt> array.
     * \pre You must call <tt>\ref vxQueryTarget</tt> with <tt>\ref VX_TARGET_ATTRIBUTE_NUMKERNELS</tt>
     * to compute the necessary size of the array.
     */
    VX_TARGET_ATTRIBUTE_KERNELTABLE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_TARGET) + 0x3,
};

/*! \brief The rectangle attribute list.
 * \ingroup group_rectangle
 */
enum vx_rectangle_attribute_e {
    /*! \brief Indicates start x value (inclusive). Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_RECTANGLE_ATTRIBUTE_START_X = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_RECTANGLE) + 0x0,
    /*! \brief Indicates start y value (inclusive). Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_RECTANGLE_ATTRIBUTE_START_Y = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_RECTANGLE) + 0x1,
    /*! \brief Indicates end x value (exclusive). Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_RECTANGLE_ATTRIBUTE_END_X = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_RECTANGLE) + 0x2,
    /*! \brief Indicates end y value (exclusive). Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_RECTANGLE_ATTRIBUTE_END_Y = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_RECTANGLE) + 0x3,
};

/*! \brief The LUT attribute list.
 * \ingroup group_lut
 */
enum vx_lut_attribute_e {
    /*! \brief Indicates the value type of the LUT. Use a <tt>\ref vx_enum</tt>. */
    VX_LUT_ATTRIBUTE_TYPE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS,VX_TYPE_LUT) + 0x0,
    /*! \brief Indicates the number of elements in the LUT. Use a <tt>\ref vx_size</tt>. */
    VX_LUT_ATTRIBUTE_COUNT = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS,VX_TYPE_LUT) + 0x1,
    /*! \brief The total size of the LUT in bytes. Uses a <tt>\ref vx_size</tt>. */
    VX_LUT_ATTRIBUTE_SIZE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS,VX_TYPE_LUT) + 0x2,
};

/*! \brief The distribution attribute list.
 * \ingroup group_distribution
 */
enum vx_distribution_attribute_e {
    /*! \brief Indicates the number of dimensions in the distribution. Use a <tt>\ref vx_size</tt> parameter. */
    VX_DISTRIBUTION_ATTRIBUTE_DIMENSIONS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_DISTRIBUTION) + 0x0,
    /*! \brief Indicates the start of the values to use (inclusive). Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_DISTRIBUTION_ATTRIBUTE_OFFSET = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_DISTRIBUTION) + 0x1,
    /*! \brief Indicates end value to use as the range (exclusive). Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_DISTRIBUTION_ATTRIBUTE_RANGE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_DISTRIBUTION) + 0x2,
    /*! \brief Indicates the number of bins. Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_DISTRIBUTION_ATTRIBUTE_BINS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_DISTRIBUTION) + 0x3,
    /*! \brief Indicates the range of a bin. Use a <tt>\ref vx_uint32</tt> parameter.  */
    VX_DISTRIBUTION_ATTRIBUTE_WINDOW = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_DISTRIBUTION) + 0x4,
    /*! \brief The total size of the distribution in bytes. Use a <tt>\ref vx_size</tt> parameter. */
    VX_DISTRIBUTION_ATTRIBUTE_SIZE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_DISTRIBUTION) + 0x5,
};

/*! \brief The Threshold types.
 * \ingroup group_threshold
 */
enum vx_threshold_type_e {
    /*! \brief A threshold with only 1 value. */
    VX_THRESHOLD_TYPE_BINARY = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_THRESHOLD_TYPE) + 0x0,
    /*! \brief A threshold with 2 values (upper/lower). Used with canny edge detection. */
    VX_THRESHOLD_TYPE_RANGE = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_THRESHOLD_TYPE) + 0x1,
};

/*! \brief The threshold attributes.
 * \ingroup group_threshold
 */
enum vx_threshold_attribute_e {
    /*! \brief The value type of the threshold. Use a <tt>\ref vx_enum</tt> parameter. Will contain a <tt>\ref vx_threshold_type_e</tt>. */
    VX_THRESHOLD_ATTRIBUTE_TYPE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_THRESHOLD) + 0x0,
    /*! \brief The value of the single threshold for functions that use that. Use a <tt>\ref vx_uint8</tt> parameter. */
    VX_THRESHOLD_ATTRIBUTE_VALUE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_THRESHOLD) + 0x1,
    /*! \brief The value of the lower threshold for functions that use that. Use a <tt>\ref vx_uint8</tt> parameter. */
    VX_THRESHOLD_ATTRIBUTE_LOWER = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_THRESHOLD) + 0x2,
    /*! \brief The value of the higher threshold for functions that use that. Use a <tt>\ref vx_uint8</tt> parameter. */
    VX_THRESHOLD_ATTRIBUTE_UPPER = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_THRESHOLD) + 0x3,
};

/*! \brief The matrix attributes.
 * \ingroup group_matrix
 */
enum vx_matrix_attribute_e {
    /*! \brief The value type of the matrix. Use a <tt>\ref vx_enum</tt> parameter. */
    VX_MATRIX_ATTRIBUTE_TYPE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_MATRIX) + 0x0,
    /*! \brief The M dimension of the matrix. Use a <tt>\ref vx_size</tt> parameter. */
    VX_MATRIX_ATTRIBUTE_ROWS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_MATRIX) + 0x1,
    /*! \brief The N dimension of the matrix. Use a <tt>\ref vx_size</tt> parameter. */
    VX_MATRIX_ATTRIBUTE_COLUMNS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_MATRIX) + 0x2,
    /*! \brief The total size of the matrix in bytes. Use a <tt>\ref vx_size</tt> parameter. */
    VX_MATRIX_ATTRIBUTE_SIZE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_MATRIX) + 0x3,
};

/*! \brief The coordinates attributes.
 * \ingroup group_coordinates
 */
enum vx_coordinates_attribute_e {
    /*! \brief The dimension of the coordinate matrix. Coordinate are DIMSx1. Use a <tt>\ref vx_size</tt> parameter. */
    VX_COORDINATES_ATTRIBUTE_DIMENSIONS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_COORDINATES) + 0x0,
    /*! \brief The total size of the coordinate matrix. Use a <tt>\ref vx_size</tt> parameter. */
    VX_COORDINATES_ATTRIBUTE_SIZE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_COORDINATES) + 0x1,
};

/*! \brief The convolution attributes.
 * \ingroup group_convolution
 */
enum vx_convolution_attribute_e {
    /*! \brief The number of rows of the convolution matrix. Use a <tt>\ref vx_size</tt> parameter. */
    VX_CONVOLUTION_ATTRIBUTE_ROWS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_CONVOLUTION) + 0x0,
    /*! \brief The number of columns of the convolution matrix. Use a <tt>\ref vx_size</tt> parameter. */
    VX_CONVOLUTION_ATTRIBUTE_COLUMNS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_CONVOLUTION) + 0x1,
    /*! \brief The scale of the convolution matrix. Use a <tt>\ref vx_uint32</tt> parameter.
     * \if OPENVX_STRICT_1_0
     * \note For 1.0, only powers of 2 are supported up to 2^31.
     * \endif
     */
    VX_CONVOLUTION_ATTRIBUTE_SCALE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_CONVOLUTION) + 0x2,
    /*! \brief The total size of the convolution matrix in bytes. Use a <tt>\ref vx_size</tt> parameter. */
    VX_CONVOLUTION_ATTRIBUTE_SIZE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_CONVOLUTION) + 0x3,
};

/*! \brief The pyramid object attributes.
 * \ingroup group_pyramid
 */
enum vx_pyramid_attribute_e {
    /*! \brief The number of levels of the pyramid. Use a <tt>\ref vx_size</tt> parameter. */
    VX_PYRAMID_ATTRIBUTE_LEVELS = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_PYRAMID) + 0x0,
    /*! \brief The scale factor between each level of the pyramid. Use a <tt>\ref vx_float32</tt> parameter. */
    VX_PYRAMID_ATTRIBUTE_SCALE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_PYRAMID) + 0x1,
};

/*! \brief The list object attributes.
 * \ingroup group_list
 */
enum vx_list_attribute_e {
    /*! \brief The type with which the list was created. Use a <tt>\ref vx_enum</tt> parameter.*/
    VX_LIST_ATTRIBUTE_TYPE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_LIST) + 0x0,
    /*! \brief The number of active items in the list. Use a <tt>\ref vx_size</tt> parameter. */
    VX_LIST_ATTRIBUTE_LENGTH = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_LIST) + 0x1,
};

/*! \brief The remap object attributes.
 * \ingroup group_remap
 */
enum vx_remap_attribute_e {
    /*! \brief The source width. Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_REMAP_ATTRIBUTE_SOURCE_WIDTH = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_REMAP) + 0x0,
    /*! \brief The source height. Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_REMAP_ATTRIBUTE_SOURCE_HEIGHT = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_REMAP) + 0x1,
    /*! \brief The destination width. Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_REMAP_ATTRIBUTE_DESTINATION_WIDTH = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_REMAP) + 0x2,
    /*! \brief The destination height. Use a <tt>\ref vx_uint32</tt> parameter. */
    VX_REMAP_ATTRIBUTE_DESTINATION_HEIGHT = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_REMAP) + 0x3,
};

/*! \brief The channel enumerations for channel extractions.
 * \see vxChannelExtractNode
 * \see vxuChannelExtract
 * \see VX_KERNEL_CHANNEL_EXTRACT
 * \ingroup group_types
 */
enum vx_channel_e {
    /*! \brief Used by formats with unknown channel types */
    VX_CHANNEL_0 = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_CHANNEL) + 0x0,
    /*! \brief Used by formats with unknown channel types */
    VX_CHANNEL_1 = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_CHANNEL) + 0x1,
    /*! \brief Used by formats with unknown channel types */
    VX_CHANNEL_2 = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_CHANNEL) + 0x2,
    /*! \brief Used by formats with unknown channel types */
    VX_CHANNEL_3 = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_CHANNEL) + 0x3,

    /*! \brief Used to extract the RED channel, no matter the byte or packing order */
    VX_CHANNEL_R = VX_CHANNEL_0,
    /*! \brief Used to extract the GREEN channel, no matter the byte or packing order */
    VX_CHANNEL_G = VX_CHANNEL_1,
    /*! \brief Used to extract the BLUE channel, no matter the byte or packing order */
    VX_CHANNEL_B = VX_CHANNEL_2,
    /*! \brief Used to extract the ALPHA channel, no matter the byte or packing order */
    VX_CHANNEL_A = VX_CHANNEL_3,
    /*! \brief Used to extract the LUMA channel, no matter the byte or packing order */
    VX_CHANNEL_Y = VX_CHANNEL_0,
    /*! \brief Used to extract the Cb/U channel, no matter the byte or packing order */
    VX_CHANNEL_U = VX_CHANNEL_1,
    /*! \brief Used to extract the Cr/V/Value channel, no matter the byte or packing order */
    VX_CHANNEL_V = VX_CHANNEL_2,
};

/*! \brief An enumeration of memory import types.
 * \ingroup group_context
 */
enum vx_import_type_e {
    /*! \brief For memory allocated through OpenVX, this is the import type. */
    VX_IMPORT_TYPE_NONE = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_IMPORT_MEM) + 0x0,

    /*! \brief The default memory type to import from the Host. */
    VX_IMPORT_TYPE_HOST = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_IMPORT_MEM) + 0x1,
};

/*! \brief The image reconstruction filters supported by image resampling operations.
 *
 * The edge of a pixel is interpreted as being aligned to the edge of the image.
 * The values for an output pixel is evaluated at the center of that pixel.
 *
 * This means, for example, that an even enlargement of a factor of two in nearest-neighbor
 * interpolation will replicate every source pixel into a 2x2 quad in the destination, and that
 * an even shrink by a factor of two in bilinear interpolation will create each destination pixel
 * by average a 2x2 quad of source pixels.
 *
 * Samples which cross the boundary of the source image have values determined by the border
 * mode - see <tt>\ref vx_border_mode_e</tt> and <tt>\ref VX_NODE_ATTRIBUTE_BORDER_MODE</tt>.
 * \see vxuScaleImage
 * \see vxScaleImageNode
 * \see VX_KERNEL_SCALE_IMAGE
 * \see vxuWarpAffine
 * \see vxWarpAffineNode
 * \see VX_KERNEL_WARP_AFFINE
 * \see vxuWarpPerspective
 * \see vxWarpPerspectiveNode
 * \see VX_KERNEL_WARP_PERSPECTIVE
 * \ingroup group_types
 */
enum vx_interpolation_type_e {
    /*! \brief Output values are defined to match the source pixel whose center is nearest to the sample position. */
    VX_INTERPOLATION_TYPE_NEAREST_NEIGHBOR = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_INTERPOLATION) + 0x0,
    /*! \brief Output values are defined by bilinear interpolation between the pixels whose centers are closest
     * to the sample position, weighted linearly by the distance of the sample from the pixel centers. */
    VX_INTERPOLATION_TYPE_BILINEAR = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_INTERPOLATION) + 0x1,
    /*! \brief Output values are determined by averaging the source pixels whose areas fall under the
     * area of the destination pixel, projected onto the source image. */
    VX_INTERPOLATION_TYPE_AREA = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_INTERPOLATION) + 0x2,
};

/*! \brief The image color space list used by the <tt>\ref VX_IMAGE_ATTRIBUTE_SPACE</tt> attribute of a <tt>\ref vx_image</tt>.
 * \ingroup group_image
 */
enum vx_color_space_e {
    /*! \brief Used to indicate that no color space is used. */
    VX_COLOR_SPACE_NONE = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_COLOR_SPACE) + 0x0,
    /*! \brief Used to indicate that the BT.601 coefficients and SMPTE C primaries are used for conversions. */
    VX_COLOR_SPACE_BT601_525 = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_COLOR_SPACE) + 0x1,
    /*! \brief Used to indicate that the BT.601 coefficients and BTU primaries are used for conversions. */
    VX_COLOR_SPACE_BT601_625 = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_COLOR_SPACE) + 0x2,
    /*! \brief Used to indicate that the BT.709 coefficients are used for conversions. */
    VX_COLOR_SPACE_BT709 = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_COLOR_SPACE) + 0x3,

    /*! \brief All images in VX are by default BT.709 */
    VX_COLOR_SPACE_DEFAULT = VX_COLOR_SPACE_BT709,
};

/*! \brief The image channel range list used by the <tt>\ref VX_IMAGE_ATTRIBUTE_RANGE</tt> attribute of a <tt>\ref vx_image</tt>.
 *  \ingroup group_image
 */
enum vx_channel_range_e {
    /*! \brief Full range of the unit of the channel */
    VX_CHANNEL_RANGE_FULL = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_COLOR_RANGE) + 0x0,
    /*! \brief Restricted range of the unit of the channel based on the space given */
    VX_CHANNEL_RANGE_RESTRICTED = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_COLOR_RANGE) + 0x1,
};

/*! \brief The parameter state type
 * \ingroup group_parameter
 */
enum vx_parameter_state_e {
    /*! \brief Default. The parameter must be supplied. If not set, during
     * Verify, an error will be returned.
     */
    VX_PARAMETER_STATE_REQUIRED = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_PARAMETER_STATE) + 0x0,
    /*! \brief The parameter may be unspecified. The kernel will take care not
     * to deference optional parameters until is is certain they valid.
     */
    VX_PARAMETER_STATE_OPTIONAL = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_PARAMETER_STATE) + 0x1,
};

/*! \brief The border mode list.
 * \ingroup group_borders
 */
enum vx_border_mode_e {
    /*! \brief No defined border mode behavior is given. */
    VX_BORDER_MODE_UNDEFINED = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_BORDER_MODE) + 0x0,
    /*! \brief For nodes which support this behavior, a constant value is
     * "filled-in" when accessing out-of-bounds pixels
     */
    VX_BORDER_MODE_CONSTANT = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_BORDER_MODE) + 0x1,
    /*! \brief For nodes which support this behavior, a replication of the nearest
     * edge pixels value is given for out-of-bounds pixels.
     */
    VX_BORDER_MODE_REPLICATE = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_BORDER_MODE) + 0x2,
};

/*! \brief The list iterator names.
 * \ingroup group_list
 */
enum vx_list_iterator_e {
    /*! \brief The logically first item in the list. Resets the <tt>\ref VX_LIST_NEXT</tt>, and <tt>\ref VX_LIST_PREVIOUS</tt> */
    VX_LIST_FRONT = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_LIST_ITERATORS) + 0x0,
    /*! \brief The logically last item in the list. Resets the <tt>\ref VX_LIST_NEXT</tt> and <tt>\ref VX_LIST_PREVIOUS</tt> */
    VX_LIST_BACK = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_LIST_ITERATORS) + 0x1,
    /*! \brief The logically first item in the list. */
    VX_LIST_FIRST = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_LIST_ITERATORS) + 0x2,
    /*! \brief The logically last item in the list. */
    VX_LIST_LAST = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_LIST_ITERATORS) + 0x3,
    /*! \brief The logically next item in the list. Updates <tt>\ref VX_LIST_NEXT</tt> and <tt>\ref VX_LIST_PREVIOUS</tt>. */
    VX_LIST_NEXT = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_LIST_ITERATORS) + 0x4,
    /*! \brief The logically previous item in the list. Updates <tt>\ref VX_LIST_NEXT</tt> and <tt>\ref VX_LIST_PREVIOUS</tt>. */
    VX_LIST_PREVIOUS = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_LIST_ITERATORS) + 0x5,
};

/*! \brief The termination criteria list.
 * \see group_kernel_opticalflowpyrlk
 * \ingroup group_context
 */
enum vx_termination_criteria_e {
    /*! \brief Indicates a termination after a set number of iterations */
    VX_TERM_CRITERIA_ITERATIONS = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_TERM_CRITERIA) + 0x0,
    /*! \brief Indicates a termination after matching against the value of eplison provided to the function. */
    VX_TERM_CRITERIA_EPSILON = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_TERM_CRITERIA) + 0x1,
    /*! \brief Indicates that both an iterations and eplison method are employed. Whichever one matches first, will
     * cause the termination.
     */
    VX_TERM_CRITERIA_BOTH = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_TERM_CRITERIA) + 0x2,
};

/*! \brief A normalization type.
 * \see group_kernel_canny
 * \ingroup group_kernel_canny
 */
enum vx_norm_type_e {
    /*! \brief The L1 normalization */
    VX_NORM_L1 = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_NORM_TYPE) + 0x0,
    /*! \brief The L2 normalization */
    VX_NORM_L2 = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_NORM_TYPE) + 0x1,
};

/*! \brief The delay attribute list.
 * \ingroup group_delay
 */
enum vx_delay_attribute_e {
    /*! \brief The type of reference contained in the delay. Use a <tt>\ref vx_enum</tt> parameter. */
    VX_DELAY_ATTRIBUTE_TYPE = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_DELAY) + 0x0,
    /*! \brief The number of items in the delay. Use a <tt>\ref vx_uint32</tt> parameter.*/
    VX_DELAY_ATTRIBUTE_COUNT = VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_DELAY) + 0x1,
};

/*!
 * \brief The entry point into modules loaded by vxLoadKernels.
 * \param [in] context The handle to the implementation context.
 * \note The symbol exported from the user module must be "vxPublishKernels" in extern "C" format.
 * \ingroup group_user_nodes
 */
typedef vx_status (*vx_publish_kernels_f)(vx_context context);

/*!
 * \brief The pointer to the Host side kernel.
 * \param [in] node The handle to the node which contains this kernel.
 * \param [in] parameters The array of parameter references.
 * \param [in] num The number of parameters.
 * \ingroup group_user_nodes
 */
typedef vx_status (*vx_kernel_f)(vx_node node, vx_reference *parameters, vx_uint32 num);

/*!
 * \brief The pointer to the kernel initializer. If the host code requires a call
 * to initialize data once all the parameters have been validated, this function will be called,
 * if not NULL.
 * \param [in] node The handle to the node which contains this kernel.
 * \param [in] parameters The array of parameter references.
 * \param [in] num The number of parameters.
 * \ingroup group_user_nodes
 */
typedef vx_status (*vx_kernel_initialize_f)(vx_node node, vx_reference *parameters, vx_uint32 num);

/*!
 * \brief The pointer to the kernel deinitializer. If the host code requires a call
 * to deinitialize data during a node garbage collection, this function will be called,
 * if not NULL.
 * \param [in] node The handle to the node which contains this kernel.
 * \param [in] parameters The array of parameter references.
 * \param [in] num The number of parameters.
 * \ingroup group_user_nodes
 */
typedef vx_status (*vx_kernel_deinitialize_f)(vx_node node, vx_reference *parameters, vx_uint32 num);

/*!
 * \brief This structure is used to extract meta data from a validation
 * function. If the data object between nodes is virtual, this will allow the
 * framework to automatically create the data object, if needed.
 * \ingroup group_user_nodes
 */
typedef struct _vx_meta_format_t
{
    vx_enum type;               /*!< The <tt>\ref vx_type_e</tt> or <tt>\ref vx_fourcc_e</tt> code */
    /*! \brief A union of the configuration types needed by all object types. */
    struct dim {
        /*! \brief When a VX_TYPE_IMAGE or fourcc code. */
        struct image {
            vx_uint32 width;    /*!< \brief The width of the image in pixels */
            vx_uint32 height;   /*!< \brief The height of the image in pixels */
            vx_fourcc format;   /*!< \brief The format of the image. */
        } image;
        /*! \brief When a VX_TYPE_BUFFER. */
        struct buffer {
            vx_size unitSize;   /*!< \brief The unit size of the buffer */
            vx_size numUnits;   /*!< \brief The number of the units in the buffer */
        } buffer;
        /*! \brief When a VX_TYPE_PYRAMID is specified */
        struct pyramid {
            vx_uint32 width;    /*!< \brief The width of the 0th image in pixels. */
            vx_uint32 height;   /*!< \brief The height of the 0th image in pixels. */
            vx_fourcc format;   /*!< \brief The <tt>\ref vx_fourcc_e</tt> format of the image. */
            vx_uint32 levels;   /*!< \brief The number of scale levels */
            vx_float32 scale;   /*!< \brief The ratio between each level */
        } pyramid;
        /*! \brief When a VX_TYPE_LIST is specified */
        struct list {
            vx_enum type;       /*!< \brief The type contained in the list */
            vx_size initial;    /*!< \brief The initial size of the list, optional */
        } list;
        /*! \brief When a VX_TYPE_SCALAR is specified */
        struct scalar {
            vx_enum type;       /*!< \brief The type of the scalar */
        } scalar;
    } dim;
} vx_meta_format_t;

/*!
 * \brief The user defined kernel node input parameter validation function.
 * \note This function will be called once for each VX_INPUT or VI_BIDIRECTIONAL
 * parameter index.
 * \param [in] node The handle to the node which is being validated.
 * \param [in] index The index of the parameter being validated.
 * \return Returns an error code describing the validation status on this
 * parameter.
 * \retval VX_ERROR_INVALID_FORMAT The parameter format was incorrect.
 * \retval VX_ERROR_INVALID_VALUE The value of the parameter was incorrect.
 * \retval VX_ERROR_INVALID_DIMENSION The dimensionality of the parameter was incorrect.
 * \retval VX_ERROR_INVALID_PARAMETERS The index was out of bounds.
 * \ingroup group_user_nodes
 */
typedef vx_status (*vx_kernel_input_validate_f)(vx_node node, vx_uint32 index);

/*!
 * \brief The user defined kernel node output parameter validation function. The function only
 * needs to fill in the meta data structure.
 * \note This function will be called once for each VX_OUTPUT parameter index.
 * \param [in] node The handle to the node which is being validated.
 * \param [in] index The index of the parameter being validated.
 * \param [in] ptr A pointer to a preallocated structure that the system holds.
 * The validation function will fill in the correct type, format and dimensionality for
 * the system to use either create memory or check against existing memory.
 * \return Returns an error code describing the validation status on this
 * parameter.
 * \retval VX_ERROR_INVALID_PARAMETERS The index was out of bounds.
 * \ingroup group_user_nodes
 */
typedef vx_status (*vx_kernel_output_validate_f)(vx_node node, vx_uint32 index, vx_meta_format_t *ptr);

#if defined(WIN32) || defined(UNDER_CE)
/*! Used to aid in debugging values in OpenVX.
 * \ingroup group_macros
 */
#if defined(ARCH_32)
#define VX_FMT_REF  "%lu"
/*! Used to aid in debugging values in OpenVX.
 * \ingroup group_macros
 */
#define VX_FMT_SIZE "%lu"
#elif defined(ARCH_64)
/*! Used to aid in debugging values in OpenVX.
 * \ingroup group_macros
 */
#define VX_FMT_REF  "%I64u"
/*! Used to aid in debugging values in OpenVX.
 * \ingroup group_macros
 */
#define VX_FMT_SIZE "%I64u"
#endif
#else
/*! Used to aid in debugging values in OpenVX.
 * \ingroup group_macros
 */
#define VX_FMT_REF  "%zu"
/*! Used to aid in debugging values in OpenVX.
 * \ingroup group_macros
 */
#define VX_FMT_SIZE "%zu"
#endif
/*! Used to indicate the 1:1 ratio in Q22.10 format.
 * \ingroup group_macros
 */
#define VX_SCALE_UNITY (1024u)

/*!
 * \brief The addressing image patch structure is used by the Host only
 * to address pixels in an image patch. The fields of the structure are defined as:
 * \arg dim - The dimensions of the image in logical pixel units in the x & y direction.
 * \arg stride - The physical byte distance from a logical pixel to the next
 * logically adjacent pixel in the positive x or y direction.
 * \arg scale - The relationship of scaling from the primary plane (typically
 * the zero indexed plane) to this plane. An integer down-scaling factor of \f$ f \f$ shall be
 * set to a value equal to \f$ scale = \frac{unity}{f} \f$ and an integer up-scaling factor of \f$ f \f$
 * shall be set to a value of \f$ scale = unity \times f \f$. \f$ unity \f$ is defined as <tt>\ref VX_SCALE_UNITY</tt>.
 * \arg step - The step is the number of logical pixel units to skip in order to
 * arrive at the next physically unique pixel. For example, on a plane which is
 * half-scaled in a dimension, the step in that dimension is 2 to indicate that
 * every other pixel in that dimension is an alias. This is useful in situations
 * where iteration over unique pixels is required such as in serializing
 * or de-serializing the image patch information.
 * \see <tt>\ref vxAccessImagePatch</tt>
 * \ingroup group_image
 * \include vx_imagepatch.c
 */
typedef struct _vx_imagepatch_addressing_t {
    vx_uint32 dim_x;        /*!< \brief Width of patch in X dimension in pixels */
    vx_uint32 dim_y;        /*!< \brief Height of patch in Y dimension in pixels */
    vx_int32  stride_x;     /*!< \brief Stride in X dimension in bytes */
    vx_int32  stride_y;     /*!< \brief Stride in Y dimension in bytes */
    vx_uint32 scale_x;      /*!< \brief Scale of X dimension. For sub-sampled planes this will be the scaling factor of the dimension of the plane in relation to the zero plane. Used <tt>\ref VX_SCALE_UNITY</tt> in the numerator. */
    vx_uint32 scale_y;      /*!< \brief Scale of Y dimension. For sub-sampled planes this will be the scaling factor of the dimension of the plane in relation to the zero plane. Used <tt>\ref VX_SCALE_UNITY</tt> in the numerator.  */
    vx_uint32 step_x;       /*!< \brief Step of X dimension in pixels */
    vx_uint32 step_y;       /*!< \brief Step of Y dimension in pixels */
} vx_imagepatch_addressing_t;

/*! \brief Used to initialize a <tt>\ref vx_imagepatch_addressing_t</tt> structure on the stack.
 * \ingroup group_image
 */
#define VX_IMAGEPATCH_ADDR_INIT {0u, 0u, 0, 0, 0u, 0u, 0u, 0u}

/*! \brief The performance measurement structure.
 * \ingroup group_performance
 */
typedef struct _vx_perf_t {
    vx_uint64 tmp;          /*!< \brief Used to hold the last measurement. */
    vx_uint64 beg;          /*!< \brief Used to hold the first measurement in a set */
    vx_uint64 end;          /*!< \brief Used to hold the last measurement in a set */
    vx_uint64 sum;          /*!< \brief Used to hold the summation of durations */
    vx_uint64 avg;          /*!< \brief Used to hold the average of the durations */
    vx_uint64 num;          /*!< \brief Used to hold the number of measurements */
} vx_perf_t;

/*! \brief Used to initialize a <tt>\ref vx_perf_t</tt> on the stack.
 * \ingroup group performance
 */
#define VX_PERF_INIT    {0ul, 0ul, 0ul, 0ul, 0ul, 0ul}

/*! \brief The target to kernel correlation table entry definition.
 * \ingroup group_target
 */
typedef struct _vx_target_kernel_t {
    /*! \brief The kernel enumeration value from <tt>\ref vx_kernel_e</tt> (or an
     * extension thereof)
     */
    vx_enum enumeration;

    /*! \brief The kernel name in dotted hierarchical format.
     * e.g. "org.khronos.openvx.invalid"
     */
    vx_char name[VX_MAX_KERNEL_NAME];
} vx_target_kernel_t;

/*! \brief Used to indicate a double scale pyramid.
 * \ingroup group_pyramid
 */
#define VX_SCALE_PYRAMID_DOUBLE     (2.0f)

/*! \brief Used to indicate a half-scale pyramid.
 * \ingroup group_pyramid
 */
#define VX_SCALE_PYRAMID_HALF       (0.5f)

/*! \brief Used to indicate a ORB scaled pyramid whose scaling factor is \f$ \frac{1}{\root 4 \of {2}} \f$.
 * \ingroup group_pyramid
 */
#define VX_SCALE_PYRAMID_ORB        ((vx_float32)0.8408964f)

/*! \brief Used with the enumeration <tt>\ref VX_NODE_ATTRIBUTE_BORDER_MODE</tt> to set the
 * border mode behavior of a node which supports borders.
 * \ingroup group_borders
 */
typedef struct _vx_border_mode_t {
    /*! \brief See <tt>\ref vx_border_mode_e</tt>. */
    vx_enum mode;
    /*! \brief For the mode <tt>\ref VX_BORDER_MODE_CONSTANT</tt>, this value will be
     * filled into each pixel. If there are sub-channels in the pixel then this
     * value will be divided up accordingly.
     */
    vx_uint32 constant_value;
} vx_border_mode_t;

/*! \brief The keypoint data structure which is shared with the users.
 * \details Use <tt>\ref vxAccessKeypoint</tt> to access this data.
 * \ingroup group_keypoint
 */
typedef struct _vx_keypoint_t {
    vx_int32 x;                 /*!< \brief The x coordinate. */
    vx_int32 y;                 /*!< \brief The y coordinate. */
    vx_float32 strength;        /*!< \brief The strength of the keypoint. */
    vx_float32 scale;           /*!< \brief Unused field reserved for future use. */
    vx_float32 orientation;     /*!< \brief Unused field reserved for future use. */
    vx_int32 tracking_status;   /*!< \brief A zero indicates a lost point. */
    vx_float32 error;           /*!< \brief An tracking method specific error. */
} vx_keypoint_t;

#endif
