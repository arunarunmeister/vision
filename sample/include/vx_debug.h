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

#ifndef _OPENVX_INT_DEBUG_H_
#define _OPENVX_INT_DEBUG_H_

/*!
 * \file
 * \author Erik Rainey <erik.rainey@ti.com>
 *
 * \defgroup group_int_debug Internal Debugging API
 * \ingroup group_internal
 * \brief The Internal Debugging API
 */

/*! \brief These are the bit flags for debugging.
 * \ingroup group_int_debug
 */
enum vx_debug_zone_e {
    VX_ZONE_ERROR       = 0,    /*!< Used for most errors */
    VX_ZONE_WARNING     = 1,    /*!< Used to warning developers of possible issues */
    VX_ZONE_API         = 2,    /*!< Used to trace API calls and return values */
    VX_ZONE_INFO        = 3,    /*!< Used to show run-time processing debug */

    VX_ZONE_PERF        = 4,    /*!< Used to show performance information */
    VX_ZONE_CONTEXT     = 5,
    VX_ZONE_OSAL        = 6,
    VX_ZONE_REFERENCE   = 7,

    VX_ZONE_BUFFER      = 8,
    VX_ZONE_IMAGE       = 9,
    VX_ZONE_SCALAR      = 10,
    VX_ZONE_KERNEL      = 11,

    VX_ZONE_GRAPH       = 12,
    VX_ZONE_NODE        = 13,
    VX_ZONE_PARAMETER   = 14,
    VX_ZONE_DELAY       = 15,

    VX_ZONE_TARGET      = 16,
    VX_ZONE_LOG         = 17,
    VX_ZONE_LIST        = 18,

    VX_ZONE_MAX         = 32
};

#if defined(WIN32) || defined(UNDER_CE)
/*! \brief Debug Facility.
 * \note the do-while loop allows us to use single line conditionals around the print
 */
#define VX_PRINT(zone, message, ...) do { vx_print(zone, "[%s:%u] "message, __FUNCTION__, __LINE__, __VA_ARGS__); } while (0)
#else
/*! \brief Debug Facility.
 * \note the do-while loop allows us to use single line conditionals around the print
 */
#define VX_PRINT(zone, message, ...) do { vx_print(zone, "[%s:%u] "message, __FUNCTION__, __LINE__, ## __VA_ARGS__); } while (0)
#endif

/*! \brief A debugging macro for entering kernels
 * \ingroup group_int_debug
 */
#define VX_KERNEL_ENTRY(params, num) { \
    vx_uint32 p; \
    VX_PRINT(VX_ZONE_API, "Entered Kernel! Parameters:\n"); \
    for (p = 0; p < num; p++) { \
        VX_PRINT(VX_ZONE_API, "\tparameter[%u]="VX_FMT_REF"\n", p, params[p]); \
    }\
}

/*! \brief A debugging macro for leaving kernels
 * \ingroup group_int_debug
 */
#define VX_KERNEL_RETURN(status) VX_PRINT(VX_ZONE_API, "returning %d\n", status);

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Internal Printing Function.
 * \param [in] zone The debug zone from \ref vx_debug_zone_e.
 * \param [in] format The format string to print.
 * \param [in] ... The variable list of arguments.
 * \ingroup group_int_debug
 */
void vx_print(vx_enum zone, char *format, ...);

/*! \brief Sets a zone bit in the debug mask
 * \param [in] zone The debug zone from \ref vx_debug_zone_e.
 * \ingroup group_int_debug
 */
void vx_set_debug_zone(vx_enum zone);

/*! \brief Clears the zone bit in the mask.
 * \param [in] zone The debug zone from \ref vx_debug_zone_e.
 * \ingroup group_int_debug
 */
void vx_clr_debug_zone(vx_enum zone);

/*! \brief Returns true or false if the zone bit is set or cleared.
 * \param [in] zone The debug zone from \ref vx_debug_zone_e.
 * \ingroup group_int_debug
 */
vx_bool vx_get_debug_zone(vx_enum zone);

/*! \brief Pulls the debug zone mask from the environment variables.
 * \ingroup group_int_debug
 */
void vx_set_debug_zone_from_env(void);

#ifdef __cplusplus
}
#endif

#endif

