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

#ifndef _VX_HELPER_H_
#define	_VX_HELPER_H_

/*! \file
 * \brief The OpenVX Helper Library Interface.
 * \author Erik Rainey <erik.rainey@ti.com>
 *
 * \defgroup group_helper OpenVX Helper
 * \brief The helper is an non-standardized set of convenience constructs for OpenVX.
 * \details These functions use only the OpenVX API in order to implement their
 * functionality. As such structures, objects, defines, typedefs and functions
 * defined herein are not part of the OpenVX standard, and are
 * included as EXAMPLE code only.
 */

/*! \brief A definition for TAU, or 2*PI.
 * \ingroup group_helper
 */
#define VX_TAU 6.28318530717958647692

#ifndef dimof
/*! \brief A helper macro to determine the number of elements in an array.
 * \ingroup group_helper
 */
#define dimof(x)    (sizeof(x)/sizeof(x[0]))
#endif

/*! \brief A structure used to simplify construction of nodes at run-time.
 * The order of declaration indicates the index.
 * \ingroup group_helper
 */
typedef struct _vx_parameter_item_t {
    vx_enum      direction;      /*!< \brief From \ref vx_direction_e */
    vx_reference reference;      /*!< \brief Any \ref vx_reference derived object.
                                  * No primitive data type variables may be placed here.
                                  */
} vx_parameter_item_t;

/*! \brief Contains everything needed to abstractly describe a parameter to a kernel. This is used to
 * declare kernel parameters at compile time.
 * \ingroup group_helper
 */
typedef struct _vx_param_description_t {
    vx_enum     direction;      /*!< \brief From \ref vx_direction_e */
    vx_enum     type;           /*!< \brief From \ref vx_type_e */
    vx_enum     state;          /*!< \brief From \ref vx_parameter_state_e */
} vx_param_description_t;

/*! \brief Contains everything needed to abstractly describe a kernel.
 * This is used to declare kernels at compile time.
 * \ingroup group_helper
 */
typedef struct _vx_kernel_description_t {
    /*! \brief The vx_kernel_e enum */
    vx_enum                 enumeration;
    /*! \brief The name that kernel will be used with \ref vxGetKernelByName. */
    vx_char                 name[VX_MAX_KERNEL_NAME];
    /*! \brief The pointer to the function to execute the kernel */
    vx_kernel_f             function;
    /*! \brief The pointer to the array of parameter descriptors */
    vx_param_description_t *parameters;
    /*! \brief The number of paraemeters in the array. */
    vx_uint32               numParams;
    /*! \brief The input validator */
    vx_kernel_input_validate_f input_validate;
    /*! \brief The output validator */
    vx_kernel_output_validate_f output_validate;
    /*! \brief The initialization function */
    vx_kernel_initialize_f initialize;
    /*! \brief The deinitialization function */
    vx_kernel_deinitialize_f deinitialize;
} vx_kernel_description_t;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief A method to construct a node via arbitrary parameters and an enum.
 * \param [in] graph The handle to desired graph to add the node to.
 * \param [in] kernelenum The \ref vx_kernel_e enum value used to create a node.
 * \param [in] params The array of parameter information.
 * \param [in] num The number of elements in params.
 * \return vx_node
 * \retval 0 Indicates a failure.
 * \ingroup group_helper
 */
vx_node vxCreateNodeByStructure(vx_graph graph,
                                vx_enum kernelenum,
                                vx_parameter_item_t *params,
                                vx_uint32 num);

/*! \brief A method to clear out the log for a particular graph.
 * \param [in] graph The graph handle.
 * \ingroup group_helper
 */
void vxClearLog(vx_graph graph);

/*! \brief A method to retrieve a specific target.
 * \param [in] context The handle to the overall context.
 * \param [in] name The platform specific name of the target.
 * \return Returns a \ref vx_target reference.
 * \retval 0 Target not present.
 * \retval * Target reference.
 * \ingroup group_helper
 */
vx_target vxFindTarget(vx_context context, vx_char name[VX_MAX_TARGET_NAME]);

/*! \brief This is used to connect one node parameter to another node parameter
 * when the original handles to the data objects are already lost.
 * The context determines if a buffer is necessary or can be optimized out.
 * \param [in] a The output parameter
 * \param [in] b The input parameter
 * \return Returns a status code.
 * \pre \ref vxGetParameterByIndex
 * \ingroup group_helper
 */
vx_status vxLinkParametersByReference(vx_parameter a, vx_parameter b);

/*! \brief This is used to connect one parameter to another parameter by
 * explicity indexing when the handles to the data objects are lost.
 * \param [in] node_a The source node to link from.
 * \param [in] index_a The index of the \ref vx_parameter to link from.
 * \param [in] node_b The sink node to link to.
 * \param [in] index_b The index of the \ref vx_parameter to link to.
 * \return Returns a status code.
 * \pre \ref vxCreateNode
 * \ingroup group_helper
 */
vx_status vxLinkParametersByIndex(vx_node node_a, vx_enum index_a, vx_node node_b, vx_enum index_b);

/*! \brief This helper is used to easily set the affine matrix to a rotation and scale.
 * \param [in] matrix The handle to the matrix.
 * \param [in] angle The rotation angle in degrees.
 * \param [in] scale The scaling value. Values less than one are enlarging.
 * \param [in] center_x The center pixel in the x direction.
 * \param [in] center_y The center pixel in the y direction.
 * \return Returns a \ref vx_status_e enumeration.
 * \ingroup group_helper
 */
vx_status vxSetAffineRotationMatrix(vx_matrix matrix,
                                    vx_float32 angle,
                                    vx_float32 scale,
                                    vx_float32 center_x,
                                    vx_float32 center_y);

#ifdef OPENVX_KHR_LIST
/*! \brief This helper function can be given to \ref vxSortList when dealing with
 * the output of a \ref vxHarrisCornersNode or \ref vxuHarrisCorners. This will
 * sort the list in ascending order. Use \ref VX_LIST_LAST to get the highest score.
 * \param [in] a Object A.
 * \param [in] b Object B.
 * \return A value from \ref vx_compare_e.
 * \ingroup group_helper
 */
vx_enum vxHarrisScoreSorter(vx_reference a, vx_reference b);
#endif

/*! \brief [Helper] This function changes the points of a rectangle by some
 * delta value per coordinate.
 * \param [in] rect The rectangle to modify.
 * \param [in] dsx The start x delta.
 * \param [in] dsy The start y delta.
 * \param [in] dex The end x delta.
 * \param [in] dey The end y delta.
 * \return vx_status
 * \retval VX_SUCCESS Modified rectangle.
 * \retval VX_ERROR_INVALID_REFERENCE Not a valid rectangle.
 * \ingroup group_helper
 */
vx_status vxAlterRectangle(vx_rectangle rect,
                           vx_int32 dsx,
                           vx_int32 dsy,
                           vx_int32 dex,
                           vx_int32 dey);

/*! \brief Adds a parameter to a graph by indicating the source node, and the
 * index of the parameter on the node.
 * \param [in] g The graph handle.
 * \param [in] n The node handle.
 * \param [in] index The index of the parameter on the node.
 * \return Returns a \ref vx_status_e enumeration.
 * \ingroup group_helper
 */
vx_status vxAddParameterToGraphByIndex(vx_graph g, vx_node n, vx_uint32 index);

#ifdef __cplusplus
}
#endif

#endif	/* _VX_HELPER_H_ */

