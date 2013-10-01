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

#ifndef _OPENVX_INT_REF_H_
#define _OPENVX_INT_REF_H_

 /*!
 * \file
 * \brief The Internal Reference API
 * \author Erik Rainey <erik.rainey@ti.com>
 * \defgroup group_int_reference Internal Reference API
 * \ingroup group_internal
 * \brief The Internal Reference API
 */

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Used to initialize any vx_reference.
 * \param [in] ref The pointer to the reference object.
 * \param [in] context The system context to put in the reference.
 * \param [in] type The type of the vx_<object>
 * \ingroup group_int_reference
 */
void vxInitReference(vx_reference_t *ref, vx_context_t *context, vx_enum type);

/*! \brief Used to add a reference to the context.
 * \param [in] context The system context.
 * \param [in] ref The pointer to the reference object.
 * \ingroup group_int_reference
 */
vx_bool vxAddReference(vx_context_t *context, vx_reference_t *ref);

/*! \brief Used to create a reference.
 * \note This does not add the reference to the system context yet.
 * \param [in] context The system context.
 * \param [in] type The \ref vx_type_e type desired.
 * \ingroup group_int_reference
 */
vx_reference_t *vxCreateReference(vx_context_t *context, vx_enum type);

/*! \brief Used to destroy an object in a generic way. */
typedef void (*vx_destructor_f)(vx_reference_t *ref);

/*! \brief Used to destroy a reference.
 * \param [in] ref The reference to release.
 * \param [in] type The \ref vx_type_e to check against.
 * \param [in] internal If true, the internal count is decremented, else the external
 * \param [in] destructor The function to call after the total count has reached zero.
 * \ingroup group_int_reference
 */
void vxReleaseReference(vx_reference_t *ref,
                        vx_enum type,
                        vx_bool internal,
                        vx_destructor_f destructor);

/*! \brief Used to validate everything but vx_context, vx_image and vx_buffer.
 * \param [in] ref The reference to validate.
 * \ingroup group_implementation
 */
vx_bool vxIsValidReference(vx_reference_t * ref);

/*! \brief Used to validate everything but vx_context, vx_image and vx_buffer.
 * \param [in] ref The reference to validate.
 * \param [in] type The \ref vx_type_e to check for.
 * \ingroup group_implementation
 */
vx_bool vxIsValidSpecificReference(vx_reference_t * ref, vx_enum type);

/*! \brief Used to remove a reference from the context.
 * \param [in] context The system context.
 * \param [in] ref The pointer to the reference object.
 * \ingroup group_int_reference
 */
vx_bool vxRemoveReference(vx_context_t *context, vx_reference_t *ref);

/*! \brief Prints the values of a reference.
 * \param [in] ref The reference to print.
 * \ingroup group_int_reference
 */
void vxPrintReference(vx_reference_t *ref);

/*! \brief Increments the ref count.
 * \param [in] ref The reference to print.
 * \ingroup group_int_reference
 */
void vxIncrementReference(vx_reference_t *ref);

/*! \brief Decrements the ref count.
 * \param [in] ref The reference to print.
 * \ingroup group_int_reference
 */
vx_bool vxDecrementReference(vx_reference_t *ref);

/*! \brief Increments the internal ref count.
 * \param [in] ref The reference to print.
 * \ingroup group_int_reference
 */
void vxIncrementIntReference(vx_reference_t *ref);

/*! \brief Decrements the internal ref count.
 * \param [in] ref The reference to print.
 * \ingroup group_int_reference
 */
vx_bool vxDecrementIntReference(vx_reference_t *ref);

/*! \brief Returns the total reference count of the object.
 * \param [in] ref The reference to print.
 * \ingroup group_int_reference
 */
vx_uint32 vxTotalReferenceCount(vx_reference_t *ref);

/*! \brief A tracking function used to increment the write usage counter and track
 * its side-effects.
 * \param [in] ref The reference to print.
 * \ingroup group_int_reference
 */
void vxWroteToReference(vx_reference_t *ref);

/*! \brief A tracking function used to increment the read usage counter and track
 * its side-effects.
 * \param [in] ref The reference to print.
 * \ingroup group_int_reference
 */
void vxReadFromReference(vx_reference_t *ref);
#ifdef __cplusplus
}
#endif

#endif