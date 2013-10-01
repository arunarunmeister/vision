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

#ifndef _VX_KHR_OPENCL_H_
#define _VX_KHR_OPENCL_H_

/*! \file
 * \brief The OpenVX to OpenCL Inter-op Extension Header.
 * \author Erik Rainey <erik.rainey@ti.com>
 *
 * \defgroup group_cl_api API
 * \brief New APIs
 * \details
 *
 * \defgroup group_cl_image Images
 * \brief OpenVX Images
 * \details OpenVX vx_image objects do not convert to OpenCL Images, they are
 * instead converted to buffers.
 *
 * \defgroup group_cl_buffer Buffers
 * \brief OpenVX Buffers
 *
 * \defgroup group_cl_convolution Convolutions
 * \brief OpenVX Convolutions
 *
 * \defgroup group_cl_distribution Distributions
 * \brief OpenVX Distributions
 *
 * \defgroup group_cl_coodinates Coordinates
 * \brief OpenVX Coordinates
 *
 * \defgroup group_cl_lut LUT
 * \brief OpenVX LUT
 *
 * \defgroup group_cl_matrix Matricies
 * \brief OpenVX Matrix
 *
 * \defgroup group_cl_types OpenVX to OpenCL Atomic Types
 * \brief Atomic Types
 * \details OpenVX types map to OpenCL types through this table:
 * | VX       | OpenCL|
 * |:---------|:------|
 * |vx_uint8  |uchar  |
 * |vx_int8   |char   |
 * |vx_uint16 |ushort |
 * |vx_int16  |short  |
 * |vx_uint32 |uint   |
 * |vx_int32  |int    |
 * |vx_uint64 |ulong  |
 * |vx_int64  |long   |
 * |vx_float32|float  |
 * |vx_float64|double |
 * |vx_size   |size_t |
 */

#if defined(__APPLE__) || defined(DARWIN)
#include <OpenCL/OpenCL.h>
#else
#include <CL/cl.h>
#endif

/*! \brief The string name of this extension to match for in the extensions list */
#define OPENVX_KHR_OPENCL "vx_khr_clvx"

#ifndef VX_SCALE_UNITY
#define VX_SCALE_UNITY  (1024)
#endif

#define VX_CL_MAX_PLANES (4)

/*! \note pass -DVX_CL_KERNEL to your OpenCL Kernels to allow the OpenCL code to include
 * the correct definitions
 */

#if defined(VX_CL_KERNEL)

/*! \brief Duplicates vx_imagepatch_addressing_t from OpenVX inside OpenCL.
 * Each plane of an image in an OpenCL kernel will receive this structure then
 * a pointer to the base of the image plane.
 * \note This structure must be declared as a __global pointer.
 * \note This will be paired with a __global void *.
 * \ingroup group_cl_image
 */
typedef struct _vx_cl_imagepatch_addressing_t {
    uint dim_x;     /*!< \brief Width of patch in X dimension in pixels */
    uint dim_y;     /*!< \brief Width of patch in Y dimension in pixels */
    int  stride_x;  /*!< \brief Stride in X dimension in bytes */
    int  stride_y;  /*!< \brief Stride in Y dimension in bytes */
    uint scale_x;   /*!< \brief Scale of X dimension. For subsampled planes this will be the scaling factor of the dimension of the plane in relation to the zero plane. Use VX_SCALE_UNITY in the numerator. */
    uint scale_y;   /*!< \brief Scale of Y dimension. For subsampled planes this will be the scaling factor of the dimension of the plane in relation to the zero plane. Use VX_SCALE_UNITY in the numerator. */
    uint step_x;    /*!< \brief Step of X dimension in pixels */
    uint step_y;    /*!< \brief Step of Y dimension in pixels */
} vx_cl_imagepatch_addressing_t;

/*! \brief The vx_buffer interop structure.
 * \ingroup group_cl_buffer
 */
typedef struct _vx_cl_buffer_t {
    size_t numUnits; /*!< \brief The number of elements in the buffer */
    size_t unitSize; /*!< \brief The byte size of each element */
} vx_cl_buffer_t;

/*! \brief The vx_convolution interop structure.
 * \ingroup group_cl_convolution
 */
typedef struct _vx_cl_conv_t {
    int m;      /*!< Rows */
    int n;      /*!< Columns */
    int range;  /*!< Scalar Value */
} vx_cl_convolution_t;

/*! \brief The Type List for OpenCL from OpenVX.
 * \ingroup group_cl_types
 */
enum vx_cl_type_e {
    VX_CL_TYPE_UCHAR,   /*!< vx_uint8 */
    VX_CL_TYPE_CHAR,    /*!< vx_int8 */
    VX_CL_TYPE_USHORT,  /*!< vx_uint16 */
    VX_CL_TYPE_SHORT,   /*!< vx_int16 */
    VX_CL_TYPE_UINT,    /*!< vx_uint32 */
    VX_CL_TYPE_INT,     /*!< vx_int32 */
    VX_CL_TYPE_ULONG,   /*!< vx_uint64 */
    VX_CL_TYPE_LONG,    /*!< vx_int64 */
    VX_CL_TYPE_FLOAT,   /*!< vx_float32 */
    VX_CL_TYPE_DOUBLE,  /*!< vx_float64 */
    VX_CL_TYPE_SIZE_T,  /*!< vx_size */
};

/*! \brief The vx_matrix interop structure.
 * \ingroup group_cl_matrix
 */
typedef struct _vx_cl_matrix_t {
    int m;      /*!< Rows */
    int n;      /*!< Columns */
    int type;   /*!< Unit Type from \ref vx_cl_type_e */
} vx_cl_matrix_t;

/*! \brief The vx_distribution interop structure
 * \note This is paired with a __global int *.
 * \ingroup group_cl_distribution
 */
typedef struct _vx_cl_distribution_t {
    uint bins_x;    /*!< The number of bins in the x direction */
    uint bins_y;    /*!< The number of bins in the y direction */
    uint range_x;   /*!< The range of data in the x direction */
    uint range_y;   /*!< The range of data in the y direction */
     int offset_x;  /*!< The starting offset of data in the x direction */
     int offset_y;  /*!< The starting offset of data in the y direction */
} vx_cl_distribution_t;

/*! \brief Allows access to an image pixel as a typecast pointer deference.
 * \ingroup group_cl_image
 */
#define vxImagePixel(type, ptr, x, y, addr) \
    ((type *)ptr)[((y) * addr->stride_y) + ((x) * addr->stride_x)]

#else

/*! \brief The vx_buffer interop structure.
 * \ingroup group_cl_buffer
 */
typedef struct _vx_cl_buffer_t {
    vx_size numUnits; /*!< \brief The number of elements in the buffer */
    vx_size unitSize; /*!< \brief The byte size of each element */
} vx_cl_buffer_t;

/*! \brief The vx_convolution interop structure.
 * \ingroup group_cl_convolution
 */
typedef struct _vx_cl_conv_t {
    vx_int32 m;      /*!< Rows */
    vx_int32 n;      /*!< Columns */
    vx_int32 range;  /*!< Scalar Value */
} vx_cl_convolution_t;

/*! \brief Adds an OpenCL Kernel as source code into the OpenVX implementation.
 * \param [in] c The OpenVX Context.
 * \param [in] name The name of the kernel.
 * \param [in] enumeration The OpenVX kernel enumeration used to identify this kernel.
 * \param [in] source The array of source line pointers.
 * \param [in] num_lines the number of lines in the sources list.
 * \param [in] numParams The number of parameters to the OpenVX kernel.
 * \param [in] input The input validator.
 * \param [in] output The output validator.
 * \ingroup group_cl_api
 */
vx_kernel vxAddOpenCLAsSourceKernel(vx_context c,
                                    vx_char name[VX_MAX_KERNEL_NAME],
                                    vx_enum enumeration,
                                    char **source,
                                    size_t num_lines,
                                    char *symbol_name,
                                    vx_uint32 numParams,
                                    vx_kernel_input_validate_f input,
                                    vx_kernel_output_validate_f output);

/*! \brief Adds an OpenCL Kernel as binary program into the OpenVX implementation.
 * \param [in] c The OpenVX Context.
 * \param [in] name The name of the kernel.
 * \param [in] enumeration The OpenVX kernel enumeration used to identify this kernel.
 * \param [in] kernel The OpenCL Kernel (either pre-compiled or compiled by user).
 * \param [in] numParams The number of parameters to the OpenVX kernel.
 * \param [in] input The input validator.
 * \param [in] output The output validator.
 * \ingroup group_cl_api
 */
vx_kernel vxAddOpenCLAsBinaryKernel(vx_context c,
                                    vx_char name[VX_MAX_KERNEL_NAME],
                                    vx_enum enumeration,
                                    cl_program kernel,
                                    vx_uint32 numParams,
                                    vx_kernel_input_validate_f input,
                                    vx_kernel_output_validate_f output);

#endif

#endif

