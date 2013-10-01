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

#include <vx_data.h>

/* the following images are declared with [p][y][x][c] indexing, or
 * planes, height, width, channels order.
 */

/* 1 plane of 4x4 pixels of 3 channels (FOURCC_RGB) */
uint8_t rgb_image[1][4][4][3] = {
    {
        {{255,0,0}, {128,0,0}, {64,0,0}, {32,0,0}},
        {{0,255,0}, {0,128,0}, {0,64,0}, {0,32,0}},
        {{0,0,255}, {0,0,128}, {0,0,64}, {0,0,32}},
        {{255,255,255}, {128,128,128}, {64,64,64}, {32,32,32}},
    },
};

/* 1 plane of 4x4 pixels of YUV422i */
uint8_t uyvy_image[1][4][4][2] = {
    {
        {{0x80, 0x10}, {0x80, 0x10}, {0x80, 0x10}, {0x80, 0x10}},
        {{0x80, 0x10}, {0x80, 0x10}, {0x80, 0x10}, {0x80, 0x10}},
        {{0x80, 0x10}, {0x80, 0x10}, {0x80, 0x10}, {0x80, 0x10}},
        {{0x80, 0x10}, {0x80, 0x10}, {0x80, 0x10}, {0x80, 0x10}},
    },
};

/* really: 2 planes, [4][4][1] and [2][2][2], which is 24 total */
uint8_t nv12_image[1][6][4][1] = {
    {
        {{0x00},{0x01},{0x02},{0x03}},
        {{0x04},{0x05},{0x06},{0x07}},
        {{0x08},{0x09},{0x0A},{0x0B}},
        {{0x0C},{0x0D},{0x0E},{0x0F}},
        /* uv plane */
        {{0x10},{0x20},{0x11},{0x21}},
        {{0x12},{0x22},{0x13},{0x23}},
    },
};

uint8_t multichannel[4][4][4][1] = {
    {
        {{0x00}, {0x01}, {0x02}, {0x03}},
        {{0x04}, {0x05}, {0x06}, {0x07}},
        {{0x08}, {0x09}, {0x0A}, {0x0B}},
        {{0x0C}, {0x0D}, {0x0E}, {0x0F}},
    },
    {
        {{0x10}, {0x11}, {0x12}, {0x13}},
        {{0x14}, {0x15}, {0x16}, {0x17}},
        {{0x18}, {0x19}, {0x1A}, {0x1B}},
        {{0x1C}, {0x1D}, {0x1E}, {0x1F}},
    },
    {
        {{0x20}, {0x21}, {0x22}, {0x23}},
        {{0x24}, {0x25}, {0x26}, {0x27}},
        {{0x28}, {0x29}, {0x2A}, {0x2B}},
        {{0x2C}, {0x2D}, {0x2E}, {0x2F}},
    },
    {
        {{0x30}, {0x31}, {0x32}, {0x33}},
        {{0x34}, {0x35}, {0x36}, {0x37}},
        {{0x38}, {0x39}, {0x3A}, {0x3B}},
        {{0x3C}, {0x3D}, {0x3E}, {0x3F}},
    },
};

uint8_t sobel3x3_input[1][5][5][1] = {
    {
        { {1},  {2},  {3},  {2},  {1}},
        { {6},  {7},  {8},  {7},  {6}},
        {{11}, {12}, {13}, {12}, {11}},
        { {6},  {7},  {8},  {7},  {6}},
        { {1},  {2},  {3},  {2},  {1}},
    },
};

int16_t sobel3x3_grad_x[1][5][5][1] = {
    {
        {{0},  {0},  {0},  {0},  {0}},
        {{0},  {8},  {0}, {-8},  {0}},
        {{0},  {8},  {0}, {-8},  {0}},
        {{0},  {8},  {0}, {-8},  {0}},
        {{0},  {0},  {0},  {0},  {0}},
    },
};

int16_t sobel3x3_grad_y[1][5][5][1] = {
    {
        {{0},  {0},  {0},  {0},  {0}},
        {{0}, {40}, {40}, {40},  {0}},
        {{0},  {0},  {0},  {0},  {0}},
        {{0},{-40},{-40},{-40},  {0}},
        {{0},  {0},  {0},  {0},  {0}},
    },
};

uint8_t grad_magnitude[1][5][5][1] = {
    {
        {{0},  {0},  {0},  {0},  {0}},
        {{0}, {41}, {40}, {41},  {0}},
        {{0},  {8},  {0},  {8},  {0}},
        {{0}, {41}, {40}, {41},  {0}},
        {{0},  {0},  {0},  {0},  {0}},
    },
};

uint8_t grad_phase[1][5][5][1] = {
    {
        {{0},  {0},  {0},  {0},  {0}},
        {{0},{183},{159},{183},  {0}},
        {{0},  {0},  {0},  {0},  {0}},
        {{0}, {72}, {96}, {72},  {0}},
        {{0},  {0},  {0},  {0},  {0}},
    }
};
