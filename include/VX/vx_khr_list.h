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

#ifndef _VX_KHR_LIST_H_
#define _VX_KHR_LIST_H_

#define OPENVX_KHR_LIST     "vx_khr_list"

/*! \file
 * \brief The OpenVX List Sorting Extension
 * \author Erik Rainey <erik.rainey@ti.com>
 * \defgroup group_list Extension: List Sorting
 * \brief An extension to support arbitrary list sorting mechanism on HOST.
 */

/*! \brief The comparison enumeration. These are numbered such that you could use:
 * \ref VX_ENUM_MASK to remove the vendor and then bit-compare against other
 * enums.
 * \code
 * vx_enum comp = VX_COMPARE_GE;
 * vx_uint32 masked = VX_ENUM_MASK & comp,
 *           equalto = VX_ENUM_MASK & VX_COMPARE_EQ,
 *           greathan = VX_ENUM_MASK & VX_COMPARE_GT;
 * if ((masked & equalto) || (masked & greathan))
 * {
 *     // do something for the greater than or equal to case.
 * }
 * \endcode
 * \ingroup group_list
 */
enum vx_compare_e {
    /*! \brief Unknown */
    VX_COMPARE_UNKNOWN = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_COMPARISON) + 0x0,
    /*! \brief Less than */
    VX_COMPARE_LT = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_COMPARISON) + 0x1,
    /*! \brief Equal to */
    VX_COMPARE_EQ = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_COMPARISON) + 0x2,
    /*! \brief Less than or equal to (LT|EQ)*/
    VX_COMPARE_LE = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_COMPARISON) + 0x3,
    /*! \brief Greater than */
    VX_COMPARE_GT = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_COMPARISON) + 0x4,
    /*! \brief Greater than or equal to (EQ|GT) */
    VX_COMPARE_GE = VX_ENUM_BASE(VX_ID_KHRONOS, VX_ENUM_COMPARISON) + 0x6,
};

/*! \brief A host-side comparison function typedef for two vx_reference derived objects.
 * \param [in] a Comparison item
 * \param [in] b Baseline item
 * \return vx_enum This function must return only one of the following:
 * \retval VX_COMPARE_LT a logically less than b
 * \retval VX_COMPARE_EQ a equivalent to b
 * \retval VX_COMPARE_GT a logically greater than b
 * \ingroup group_list
 */
typedef vx_enum (*vx_ref_compare_f)(vx_reference a, vx_reference b);

/*! \brief Sorts the list based on the comparison function.
 * \param [in] list The list object.
 * \param [in] sorter The sorting function.
 * \return A \ref vx_status_e enumeration.
 * \retval VX_SUCCESS The list was sorted. For lists of size zero or 1, this is automatically true.
 * \retval VX_ERROR_INVALID_VALUE The sorting function did not return meaningful information to sort the list.
 * \retval VX_ERROR_INVALID_REFERENCE The list or the ref was not a reference.
 * \ingroup group_list
 */
vx_status vxSortList(vx_list list, vx_ref_compare_f sorter);

/*! \brief Inserts an item into a pre-sorted list in a sorted manner.
 * \param [in] list The list object.
 * \param [in] ref The reference to add to the list in a sorted manner.
 * \param [in] sorter The sorting function.
 * \return A \ref vx_status_e enumeration.
 * \retval VX_ERROR_INVALID_TYPE The supplied object was not of the type created for this list.
 * \retval VX_ERROR_INVALID_VALUE If the sorting function did not properly sort the reference.
 * \retval VX_ERROR_INVALID_REFERENCE if the ref was not valid.
 * \ingroup group_list
 */
vx_status vxInsertListItem(vx_list list, vx_reference ref, vx_ref_compare_f sorter);

/*! \brief Removes the item from the list, decrementing its reference count.
 * \param [in] list The list object.
 * \param [in] it The pointer to a location to store the iterator.
 * \param [in] ref The reference to remove from the list and decrement it's reference count.
 * \return A \ref vx_status_e enumeration.
 * \retval VX_FAILURE item was not in the list.
 * \ingroup group_list
 */
vx_status vxEraseListItem(vx_list list, vx_iterator *it, vx_reference ref);

#endif
