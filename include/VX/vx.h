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

#ifndef _OPENVX_H_
#define _OPENVX_H_

/*!
 * \file
 * \brief The top level Open Vision Acceleration Header.
 * \author Erik Rainey <erik.rainey@ti.com>
 */

/*! \brief Defines the maximum number of characters in a implementation string.
 * \ingroup group_target
 */
#define VX_MAX_IMPLEMENTATION_NAME (64)

/*! \brief Defines the maximum number of characters in a target string.
 * \ingroup group_target
 */
#define VX_MAX_TARGET_NAME (64)

/*! \brief The maximum length of a message buffer to copy from the log.
 * \ingroup group_log
 */
#define VX_MAX_LOG_MESSAGE_LEN (1024)

/*! \brief The maximum string length of a kernel name to be added to OpenVX.
 * \ingroup group_kernel
 */
#define VX_MAX_KERNEL_NAME (256)

#include <VX/vx_vendors.h>
#include <VX/vx_types.h>
#include <VX/vx_kernels.h>
#include <VX/vx_api.h>
#include <VX/vx_nodes.h>

/*! The major version number
 * \ingroup group_defines
 */
#define VX_VERSION_MAJOR    (0x0)

/*! The minor version number
 * \ingroup group_defines
 */
#define VX_VERSION_MINOR    (0x5)

/*! The OpenVX Version Number
 * \ingroup group_defines
 */
#define VX_VERSION          (VX_VERSION_MAJOR << 16 | VX_VERSION_MINOR)

#endif
