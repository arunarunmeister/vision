/*
 * Copyright (c) 2012-2013 The Khronos Group Inc.
 * All Rights Reserved.  This code is protected by copyright laws and contains
 * material proprietary to the Khronos Group, Inc.  This is UNPUBLISHED
 * PROPRIETARY SOURCE CODE that may not be disclosed in whole or in part to
 * third parties, and may not be reproduced, republished, distributed,
 * transmitted, displayed, broadcast or otherwise exploited in any manner
 * without the express prior written permission of Khronos Group.
 *
 * The receipt or possession of this code does not convey any rights to
 * reproduce, disclose, or distribute its contents, or to manufacture, use, or
 * sell anything that it may describe, in whole or in part other than under the
 * terms of the Khronos Adopters Agreement or Khronos Conformance Test Source
 * License Agreement as executed between Khronos and the recipient.
 *
 * For the avoidance of doubt, this code when provided: a) under the Khronos
 * Conformance Test Source License Agreement is for the sole purpose of creating
 * Conformance tests for delivery to Khronos and does not provide for formally
 * testing products or use of Khronos trademarks on conformant products; b)
 * under the Khronos Adopters Agreement is for the sole purpose of formally
 * administering tests to products pursuant to the Khronos Conformance
 * Process Document.
 *
 * Khronos, OpenKODE, OpenVG, OpenWF, glFX, OpenMAX and OpenSL ES are trademarks
 * of the Khronos Group Inc.  COLLADA is a trademark of Sony Computer
 * Entertainment Inc. used by permission by Khronos.  OpenGL and OpenML are
 * registered trademarks and the OpenGL ES logo is a trademark of Silicon
 * Graphics Inc. used by permission by Khronos.
 *
 * Use, duplication or disclosure by the Government is subject to restrictions
 * as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data and
 * Computer Software clause at DFARS 252.227-7013, and/or in similar or
 * successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished
 * rights reserved under the Copyright Laws of the United States and other
 * countries.
 */

#ifndef VX_CONFORMANCE_H
#define VX_CONFORMANCE_H

/*!
 * \file
 * \brief The OpenVX Conformance Test Framework.
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <VX/vx.h>
#include <VX/vxu.h>
#include <VX/vx_helper.h>
#if defined(OPENVX_USE_NODE_MEMORY)
#include <VX/vx_khr_node_memory.h>
#endif
#if defined(OPENVX_USE_LIST)
#include <VX/vx_khr_list.h>
#endif

/*! \brief A local definition of a function pointer for a conformance test
 * \ingroup group_conformance
 */
typedef vx_status (*vx_conformance_f)(void);

/*! \brief A definition of the group to which a test belongs for ease of identification.
 * \ingroup group_conformance
 */
typedef enum _vx_conformance_group_e {
    VX_CONFORMANCE_GROUP_FRAMEWORK_OBJECTS,
    VX_CONFORMANCE_GROUP_DATA_OBJECTS,
    VX_CONFORMANCE_GROUP_FRAMEWORK_MECHANISM,
    VX_CONFORMANCE_GROUP_KERNELS,
    VX_CONFORMANCE_GROUP_GRAPHS,
} vx_conformance_group_e;

/*! \brief A definition of the sub-group to which a framework mechanism test belongs for ease of identification.
 * \ingroup group_conformance
 */
enum _vx_framework_mechanism_e {
    VX_FRAMEWORK_MECHANISM_LOG,
    VX_FRAMEWORK_MECHANISM_USER_NODE,
    VX_FRAMEWORK_MECHANISM_CALLBACK,
};

/*! \brief The structure which correlates each unit test with a result and a name.
 * \ingroup group_conformance
 */
typedef struct _vx_conformance_t {
    vx_status               status;     /*!< \brief The result of the test */
    vx_char                 name[256];  /*!< \brief The name of the test */
    vx_conformance_group_e  group;      /*!< \brief The conformance group that the test is in. */
    vx_enum                 subgroup;   /*!< \brief The subgroup that the test is in. */
    vx_conformance_f        test;       /*!< \brief The pointer to the HOST side function */
} vx_conformance_t;

/*! \brief Used to set the the \ref vx_attribute_query_t structure.
 * \ingroup group_conformance
 */
#define VX_ATTR(attr, var)     {attr, #attr, &var, sizeof(var)}

/*! \brief The Attribute query structure.
 * \ingroup group_conformance
 */
typedef struct _vx_attribute_query_t {
    vx_enum attribute;  /*!< \brief The Attribute to query for */
    vx_char name[256];  /*!< \brief The name of the attribute */
    void   *out;        /*!< \brief The location to store the output */
    vx_size size;       /*!< \brief The sizeof the output location */
} vx_attribute_query_t;

#define VX_MAX_PARAMETERS (10)

typedef struct _vx_node_check_t {
    vx_enum kernel_enum;
    vx_char kernel_name[VX_MAX_KERNEL_NAME];
    vx_uint32 num; /*!< \brief number of parameters */
    vx_param_description_t parameters[VX_MAX_PARAMETERS];
} vx_node_check_t;

#if defined(WIN32) || defined(UNDER_CE)
#define CONF_PRINT(message, ...)    conf_print(message, __FUNCTION__, __LINE__, __VA_ARGS__)
#else
#define CONF_PRINT(message, ...)    conf_print(message, __FUNCTION__, __LINE__, ## __VA_ARGS__)
#endif

/*! \brief Creates an image from a predefined buffer.
 * \ingroup group_conformance
 */
vx_image vxCreateTestImage(vx_context context,
                          vx_uint8 *ptr,
                          vx_uint32 channels,
                          vx_uint32 width,
                          vx_uint32 height,
                          vx_uint32 planes);

/*! \brief Printing Function.
 * \ingroup group_conformance
 */
void conf_print(char *message, const char *function, vx_uint32 line, ...);

/*! \brief The Log Printing Function.
 * \ingroup group_conformance
 */
void conf_print_log(vx_context context);

/*! \brief A graph to compute average and standard deviation on the difference of two images.
 * \ingroup group_conformance
 */
vx_status vxStatisticsOnImages(vx_context c, vx_image a, vx_image b,
                               vx_scalar mean, vx_scalar stddev,
                               vx_scalar minVal, vx_scalar maxVal);

/*! \brief A wrapper to call any object's query function with a table of attributes.
 * \ingroup group_conformance
 */
vx_status vxQuery(vx_reference ref, vx_attribute_query_t attr[], vx_size numAttr);

#endif
