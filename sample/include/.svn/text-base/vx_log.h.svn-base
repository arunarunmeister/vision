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

#ifndef _OPENVX_INT_LOG_H_
#define _OPENVX_INT_LOG_H_

/*!
 * \file
 * \brief The internal log implementation
 * \author Erik Rainey <erik.rainey@ti.com>
 *
 * \defgroup group_int_log Internal Log API
 * \ingroup group_internal
 * \brief The Internal Log API
 */

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Initializes the log object.
 * \param [in] log The log object.
 * \ingroup group_int_log
 */
void vxInitLog(vx_log_t *log);

 /*! \brief Resets the log. All entries are forgotten.
  * \param [in] log The log object
  * \ingroup group_int_log
  */
void vxResetLog(vx_log_t *log);

/*! \brief Prints a log entry.
 * \param [in] log The log object.
 * \param [in] i The index of the log entries to print.
 * \ingroup group_int_log
 */
void vxPrintLogEntry(vx_log_t *log, vx_int32 i, vx_uint32 zone);

/*! \brief Prints Log Info
 * \param [in] log The log object.
 * \ingroup group_int_log
 */
void vxPrintLog(vx_log_t *log, vx_uint32 zone);

/*! \brief Forces a log print of all current entries.
 * \param [in] log The log object.
 * \ingroup group_int_log
 */
void vxPrintAllLogEntries(vx_log_t *log, vx_uint32 zone);

#ifdef __cplusplus
}
#endif

 #endif
