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
 * \file xyz_module.c
 * \example xyz_module.c
 * \brief An example implementation of a User Extension to OpenVX.
 * \author Erik Rainey <erik.rainey@ti.com>
 * \defgroup group_example_kernel Example: User Defined Kernel
 * \ingroup group_example
 * \brief An example of a user defined kernel to OpenVX and it's required parts.
 * \details Users Defined Kernels are a method by which to insert User defined
 * code to execute inside the graph concurrently with other nodes in the Graph.
 * The benefit of User Defined Kernels is that code which would otherwise have
 * to stop all other graph processing and then begin graph processing again,
 * can be run, potentially in parallel, with other nodes in the graph, with no
 * other interrupt of the graph. User Defined Kernels execute on the host
 * processor by default. OpenVX does not define a language to describe kernels,
 * but relies on the existing OS constructs of modules and symbols to be able to
 * reference local functions on the host to validate input and output data as
 * well as the invoke the kernel itself.
 */

#include <VX/vx.h>
#include <VX/vx_ext_xyz.h>
#include <stdarg.h>

/*! An internal definition of the order of the parameters to the function.
 * This list must match the parameter list in the function and in the
 * publish kernel list.
 * \ingroup group_example_kernel
 */
typedef enum _xyz_params_e {
    XYZ_PARAM_INPUT = 0,
    XYZ_PARAM_VALUE,
    XYZ_PARAM_OUTPUT,
    XYZ_PARAM_TEMP
} xyz_params_e;

/*! \brief An example input parameter validator.
 * \param [in] node The handle to the node.
 * \param [in] index The index of the parameter to validate.
 * \return A \ref vx_status_e enumeration.
 * \ingroup group_example_kernel
 */
vx_status XYZInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    vx_parameter param = vxGetParameterByIndex(node, index);
    if (index == XYZ_PARAM_INPUT)
    {
        vx_image image;
        vx_fourcc fourcc = 0;
        if (vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &image, sizeof(vx_image)) == VX_SUCCESS &&
            vxQueryImage(image, VX_IMAGE_ATTRIBUTE_FORMAT, &fourcc, sizeof(fourcc)) == VX_SUCCESS)
        {
            if (fourcc == FOURCC_U8)
                status = VX_SUCCESS;
            else
                status = VX_ERROR_INVALID_VALUE;
        }
    }
    else if (index == XYZ_PARAM_VALUE)
    {
        vx_scalar scalar = 0;
        vx_enum type = 0;
        vx_int32 value = 0;
        if (vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &scalar, sizeof(scalar)) == VX_SUCCESS &&
            vxQueryScalar(scalar, VX_SCALAR_ATTRIBUTE_TYPE, &type, sizeof(type)) == VX_SUCCESS &&
            type == VX_TYPE_INT32 &&
            vxAccessScalarValue(scalar, &value) == VX_SUCCESS)
        {
            if (XYZ_VALUE_MIN < value && value < XYZ_VALUE_MAX)
            {
                status = VX_SUCCESS;
            }
            else
            {
                status = VX_ERROR_INVALID_VALUE;
            }
        }
    }
    else if (index == XYZ_PARAM_TEMP)
    {
        vx_buffer temp;
        vx_size size = 0;
        if (vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &temp, sizeof(temp)) == VX_SUCCESS &&
            vxQueryBuffer(temp, VX_BUFFER_ATTRIBUTE_SIZE, &size, sizeof(size)) == VX_SUCCESS)
        {
            if (size >= XYZ_TEMP_SIZE)
                status = VX_SUCCESS;
            else
                status = VX_ERROR_INVALID_DIMENSION;
        }
    }
    vxReleaseParameter(&param);
    return status;
}

/*! \brief An example output parameter validator.
 * \param [in] node The handle to the node.
 * \param [in] index The index of the parameter to validate.
 * \param [out] ptr The metadata used to check the parameter.
 * \return A \ref vx_status_e enumeration.
 * \ingroup group_example_kernel
 */
vx_status XYZOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == XYZ_PARAM_OUTPUT)
    {
        vx_parameter in0 = vxGetParameterByIndex(node, XYZ_PARAM_INPUT);
        vx_image input;
        if (vxQueryParameter(in0, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(vx_image)) == VX_SUCCESS)
        {
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &ptr->type, sizeof(ptr->type));
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_WIDTH, &ptr->dim.image.width, sizeof(ptr->dim.image.width));
            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_HEIGHT, &ptr->dim.image.height, sizeof(ptr->dim.image.height));
            status = VX_SUCCESS;
        }
        vxReleaseParameter(&in0);
    }
    return status;
}

/*!
 * \brief The private kernel function for XYZ.
 * \note This is not called directly by users.
 * \param [in] node The handle to the node this kernel is instanced into.
 * \param [in] parameters The array of \ref vx_reference references.
 * \param [in] num The number of parameters in the array.
 * functions.
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Successful return.
 * \retval VX_ERROR_INVALID_PARAMETER The input or output image were
 * of the incorrect dimensions.
 * \ingroup group_example_kernel
 */
vx_status XYZKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (num == 4)
    {
        vx_image input  = (vx_image)parameters[0];
        vx_scalar scalar = (vx_scalar)parameters[1];
        vx_image output = (vx_image)parameters[2];
        vx_buffer temp  = (vx_buffer)parameters[3];
        void *buf, *in, *out;
        vx_uint32 us = 0, nu = 0, y, x;
        vx_int32 value = 0;
        vx_imagepatch_addressing_t addr1, addr2;
        vx_rectangle rect = 0;

        status = VX_SUCCESS;

        status |= vxAccessScalarValue(scalar, &value);
        rect = vxGetValidRegionImage(input);
        status |= vxAccessImagePatch(input, rect, 0, &addr1, &in);
        status |= vxAccessImagePatch(output, rect, 0, &addr2, &out);
        status |= vxQueryBuffer(temp, VX_BUFFER_ATTRIBUTE_NUMUNITS, &nu, sizeof(nu));
        status |= vxQueryBuffer(temp, VX_BUFFER_ATTRIBUTE_UNITSIZE, &us, sizeof(us));
        status |= vxAccessBufferRange(temp, 0, nu, &buf);
        for (y = 0; y < addr1.dim_y; y+=addr1.step_y)
        {
            for (x = 0; x < addr1.dim_x; x+=addr1.step_x)
            {
                // do some operation...
            }
        }
        // write back and release
        status |= vxCommitBufferRange(temp, 0, nu, buf);
        status |= vxCommitImagePatch(output, rect, 0, &addr2, out);
        status |= vxCommitImagePatch(input, 0, 0, &addr1, in); // don't write back into the input
        vxReleaseRectangle(&rect);
    }
    return status;
}

/*! \brief An initializer function.
 * \param [in] node The handle to the node this kernel is instanced into.
 * \param [in] parameters The array of \ref vx_reference references.
 * \param [in] num The number of parameters in the array.
 * functions.
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Successful return.
 * \retval VX_ERROR_INVALID_PARAMETER The input or output image were
 * of the incorrect dimensions.
 * \ingroup group_example_kernel
 */
vx_status XYZInitialize(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    /* XYZ requires no initialization of memory or resources */
    return VX_SUCCESS;
}

/*! \brief A deinitializer function.
 * \param [in] node The handle to the node this kernel is instanced into.
 * \param [in] parameters The array of \ref vx_reference references.
 * \param [in] num The number of parameters in the array.
 * functions.
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS Successful return.
 * \retval VX_ERROR_INVALID_PARAMETER The input or output image were
 * of the incorrect dimensions.
 * \ingroup group_example_kernel
 */
vx_status XYZDeinitialize(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    /* XYZ requires no de-initialization of memory or resources */
    return VX_SUCCESS;
}

//**********************************************************************
//  PUBLIC FUNCTION
//**********************************************************************

/*! \brief The entry point into this module to add the extensions to OpenVX.
 * \param [in] context The handle to the implementation context.
 * \return A \ref vx_status_e enumeration. Returns errors if some or all kernels were not added
 * correctly.
 * \note This follows the function pointer definition of a \ref vx_publish_kernels_f
 * and uses the predefined name for the entry point, "vxPublishKernels".
 * \ingroup group_example_kernel
 */
vx_status vxPublishKernels(vx_context context)
{
    vx_status status = VX_SUCCESS;
    vx_kernel kernel = vxAddKernel(context,
                                    "org.khronos.example.xyz",
                                    VX_KERNEL_KHR_XYZ,
                                    XYZKernel,
                                    4,
                                    XYZInputValidator,
                                    XYZOutputValidator,
                                    XYZInitialize,
                                    XYZDeinitialize);
    if (kernel)
    {
        vx_size size = XYZ_DATA_AREA;
        status = vxAddParameterToKernel(kernel, 0, VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED);
        if (status != VX_SUCCESS) goto exit;
        status = vxAddParameterToKernel(kernel, 1, VX_INPUT, VX_TYPE_INT32, VX_PARAMETER_STATE_REQUIRED);
        if (status != VX_SUCCESS) goto exit;
        status = vxAddParameterToKernel(kernel, 2, VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED);
        if (status != VX_SUCCESS) goto exit;
        status = vxAddParameterToKernel(kernel, 3, VX_BIDIRECTIONAL, VX_TYPE_BUFFER, VX_PARAMETER_STATE_REQUIRED);
        if (status != VX_SUCCESS) goto exit;
        status = vxSetKernelAttribute(kernel, VX_KERNEL_ATTRIBUTE_LOCAL_DATA_SIZE, &size, sizeof(size));
        if (status != VX_SUCCESS) goto exit;
        status = vxFinalizeKernel(kernel);
        if (status != VX_SUCCESS) goto exit;
    }
exit:
    if (status != VX_SUCCESS) {
        vxRemoveKernel(kernel);
    }
    return status;
}

