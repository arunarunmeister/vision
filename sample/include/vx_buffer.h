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

#ifndef _OPENVX_INT_BUFFER_H_
#define _OPENVX_INT_BUFFER_H_

/*!
 * \file
 * \brief The internal buffer implementation
 * \author Erik Rainey <erik.rainey@ti.com>
 *
 * \defgroup group_int_buffer Internal Buffer API
 * \ingroup group_internal
 * \brief The Internal Buffer API.
 */

#ifdef __cplusplus
    extern "C" {
#endif

/*! \brief Used to validate vx_buffer types.
 * \param [in] buf The vx_buffer to validate.
 * \ingroup group_int_buffer
 */
vx_bool vxIsValidBuffer(vx_buffer_t *buf);

/*! \brief Used to free a buffer object.
 * \param [in] buffer The buffer to free. The metadata structure is not freed, just the data.
 * \ingroup group_int_buffer
 */
void vxFreeBuffer(vx_buffer_t *buffer);

/*! \brief Used to allocate a buffer object.
 * \param [in,out] buffer The buffer object.
 * \ingroup group_int_buffer
 */
vx_bool vxAllocateBuffer(vx_buffer_t *buffer);

/*! \brief Used in conjunction with the \ref vxReleaseReference to get rid of objects.
 * \param [in,out] ref The base reference pointer.
 * \ingroup group_int_buffer
 */
void vxDestructBuffer(vx_reference_t *ref);

/*! \brief Internal refcount method to release a buffer.
 * \param [in] buf The buffer to release.
 * \ingroup group_int_buffer
 */
void vxReleaseBufferInt(vx_buffer_t *buf);

#ifdef __cplusplus
}
#endif

#endif
