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
 * \file
 * \brief The Color Convert Kernels
 * \author Erik Rainey <erik.rainey@ti.com>
 */

#include <VX/vx.h>
#include <VX/vx_helper.h>

#include <vx_internal.h>

static vx_uint8 usat8(vx_int32 a)
{
    if (a > 255)
        a = 255;
    if (a < 0)
        a = 0;
    return (vx_uint8)a;
}

static void yuv2rgb_bt601(vx_uint8 y, vx_uint8 cb, vx_uint8 cr,
                          vx_uint8 *r, vx_uint8 *g, vx_uint8 *b)
{
    /*
    R'= Y' + 0.000*U' + 1.403*V'
    G'= Y' - 0.344*U' - 0.714*V'
    B'= Y' + 1.773*U' + 0.000*V'
    */
    vx_float64 f_y = (vx_float64)y;
    vx_float64 f_u = (vx_float64)cb - 128;
    vx_float64 f_v = (vx_float64)cr - 128;
    vx_float64 f_r = f_y + 0.000f*f_u + 1.403f*f_v;
    vx_float64 f_g = f_y - 0.344f*f_u - 0.714f*f_v;
    vx_float64 f_b = f_y + 1.773f*f_u + 0.000f*f_v;
    vx_int32 i_r = (vx_int32)f_r;
    vx_int32 i_g = (vx_int32)f_g;
    vx_int32 i_b = (vx_int32)f_b;
    *r = usat8(i_r);
    *g = usat8(i_g);
    *b = usat8(i_b);
}

#if 0 /* we don't make 601 yet */
static void rgb2yuv_bt601(vx_uint8 r, vx_uint8 g, vx_uint8 b,
                          vx_uint8 *y, vx_uint8 *cb, vx_uint8 *cr)
{
    /*
    Y'= 0.299*R' + 0.587*G' + 0.114*B'
    Cb=-0.169*R' - 0.331*G' + 0.500*B'
    Cr= 0.500*R' - 0.419*G' - 0.081*B'
    */
    vx_float64 f_r = (vx_float64)r;
    vx_float64 f_g = (vx_float64)g;
    vx_float64 f_b = (vx_float64)b;
    vx_float64 f_y = 0 + 0.299f*f_r + 0.587f*f_g + 0.114f*f_b;
    vx_float64 f_u = 0 - 0.169f*f_r - 0.331f*f_g + 0.500f*f_b;
    vx_float64 f_v = 0 + 0.500f*f_r - 0.419f*f_g - 0.081f*f_b;
    vx_int32 i_y = (vx_int32)f_y;
    vx_int32 i_u = (vx_int32)f_u + 128;
    vx_int32 i_v = (vx_int32)f_v + 128;
    *y  = usat8(i_y);
    *cb = usat8(i_u);
    *cr = usat8(i_v);
}
#endif

static void yuv2rgb_bt709(vx_uint8 y, vx_uint8 cb, vx_uint8 cr,
                          vx_uint8 *r, vx_uint8 *g, vx_uint8 *b)
{
    /*
    R'= Y' + 0.0000*Cb + 1.5701*Cr
    G'= Y' - 0.1870*Cb - 0.4664*Cr
    B'= Y' - 1.8556*Cb + 0.0000*Cr
    */
    vx_float64 f_y = (vx_float64)y;
    vx_float64 f_u = (vx_float64)cb - 128;
    vx_float64 f_v = (vx_float64)cr - 128;
    vx_float64 f_r = f_y + 0.0000f*f_u + 1.5701f*f_v;
    vx_float64 f_g = f_y - 0.1870f*f_u - 0.4664f*f_v;
    vx_float64 f_b = f_y - 1.8556f*f_u + 0.0000f*f_v;
    vx_int32 i_r = (vx_int32)f_r;
    vx_int32 i_g = (vx_int32)f_g;
    vx_int32 i_b = (vx_int32)f_b;
    *r = usat8(i_r);
    *g = usat8(i_g);
    *b = usat8(i_b);
}

static void rgb2yuv_bt709(vx_uint8 r, vx_uint8 g, vx_uint8 b,
                          vx_uint8 *y, vx_uint8 *cb, vx_uint8 *cr)
{
    /*
    Y'= 0.2215*R' + 0.7154*G' + 0.0721*B'
    Cb=-0.1145*R' - 0.3855*G' + 0.5000*B'
    Cr= 0.5016*R' - 0.4556*G' - 0.0459*B'
    */
    vx_float64 f_r = (vx_float64)r;
    vx_float64 f_g = (vx_float64)g;
    vx_float64 f_b = (vx_float64)b;
    vx_float64 f_y = 0 + 0.2215f*f_r + 0.7154f*f_g + 0.0721f*f_b;
    vx_float64 f_u = 0 - 0.1145f*f_r - 0.3855f*f_g + 0.5000f*f_b;
    vx_float64 f_v = 0 + 0.5016f*f_r - 0.4556f*f_g - 0.0459f*f_b;
    vx_int32 i_y = (vx_int32)f_y;
    vx_int32 i_u = (vx_int32)f_u + 128;
    vx_int32 i_v = (vx_int32)f_v + 128;
    *y  = usat8(i_y);
    *cb = usat8(i_u);
    *cr = usat8(i_v);
}

static void yuv2yuv_601to709(vx_uint8 y0, vx_uint8 cb0, vx_uint8 cr0,
                             vx_uint8 *y1, vx_uint8 *cb1, vx_uint8 *cr1)
{
    /*
    Y' = 1.0090*Y - 0.11826430*Cb - 0.2000311*Cr
    Cb'= 0.0000*Y + 1.01911200*Cb + 0.1146035*Cr
    Cr'= 0.0001*Y + 0.07534570*Cb + 1.0290932*Cr
    */
    vx_float64 f_y0 = (vx_float64)y0;
    vx_float64 f_cb0 = (vx_float64)cb0;
    vx_float64 f_cr0 = (vx_float64)cr0;
    vx_float64 f_y1  = 1.0090*f_y0 - 0.11826430*f_cb0 - 0.2000311*f_cr0;
    vx_float64 f_cb1 = 0.0000*f_y0 + 1.01911200*f_cb0 + 0.1146035*f_cr0;
    vx_float64 f_cr1 = 0.0001*f_y0 + 0.07534570*f_cb0 + 1.0290932*f_cr0;
    vx_int32 i_y = (vx_int32)f_y1;
    vx_int32 i_cb = (vx_int32)f_cb1;
    vx_int32 i_cr = (vx_int32)f_cr1;
    *y1 = usat8(i_y);
    *cb1 = usat8(i_cb);
    *cr1 = usat8(i_cr);
}

static vx_status vxColorConvertKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (num == 2)
    {
        vx_image src = (vx_image)parameters[0];
        vx_image dst = (vx_image)parameters[1];
        vx_imagepatch_addressing_t src_addr[4], dst_addr[4];
        void *src_base[4] = {NULL};
        void *dst_base[4] = {NULL};
        vx_uint32 y, x, p;
        vx_fourcc src_format, dst_format;
        vx_uint32 src_planes, dst_planes;
        vx_enum src_space;
        vx_rectangle rect;

        status = VX_SUCCESS;
        status |= vxQueryImage(src, VX_IMAGE_ATTRIBUTE_FORMAT, &src_format, sizeof(src_format));
        status |= vxQueryImage(dst, VX_IMAGE_ATTRIBUTE_FORMAT, &dst_format, sizeof(dst_format));
        status |= vxQueryImage(src, VX_IMAGE_ATTRIBUTE_PLANES, &src_planes, sizeof(src_planes));
        status |= vxQueryImage(dst, VX_IMAGE_ATTRIBUTE_PLANES, &dst_planes, sizeof(dst_planes));
        status |= vxQueryImage(src, VX_IMAGE_ATTRIBUTE_SPACE, &src_space, sizeof(src_space));
        rect = vxGetValidRegionImage(src);
        for (p = 0; p < src_planes; p++)
        {
            status |= vxAccessImagePatch(src, rect, p, &src_addr[p], &src_base[p]);
            vxPrintImageAddressing(&src_addr[p]);
        }
        for (p = 0; p < dst_planes; p++)
        {
            status |= vxAccessImagePatch(dst, rect, p, &dst_addr[p], &dst_base[p]);
            vxPrintImageAddressing(&dst_addr[p]);
        }
        if (status != VX_SUCCESS)
        {
            VX_PRINT(VX_ZONE_ERROR, "Failed to setup images in Color Convert!\n");
        }

        if ((src_format == FOURCC_RGB) || (src_format == FOURCC_RGBX))
        {
            if (dst_format == FOURCC_NV12)
            {
                vx_uint8 cb[4];
                vx_uint8 cr[4];
                for (y = 0; y < dst_addr[0].dim_y; y+=2)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x+=2)
                    {
                        vx_uint8 *rgb[4] = {vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]),
                                            vxFormatImagePatchAddress2d(src_base[0], x+1, y, &src_addr[0]),
                                            vxFormatImagePatchAddress2d(src_base[0], x, y+1, &src_addr[0]),
                                            vxFormatImagePatchAddress2d(src_base[0], x+1, y+1, &src_addr[0])};
                        vx_uint8 *luma[4] = {vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]),
                                             vxFormatImagePatchAddress2d(dst_base[0], x+1, y, &dst_addr[0]),
                                             vxFormatImagePatchAddress2d(dst_base[0], x, y+1, &dst_addr[0]),
                                             vxFormatImagePatchAddress2d(dst_base[0], x+1, y+1, &dst_addr[0])};
                        vx_uint8 *cbcr = vxFormatImagePatchAddress2d(dst_base[1], x, y, &dst_addr[1]);
                        rgb2yuv_bt709(rgb[0][0],rgb[0][1],rgb[0][2],&luma[0][0],&cb[0],&cr[0]);
                        rgb2yuv_bt709(rgb[1][0],rgb[1][1],rgb[1][2],&luma[1][0],&cb[1],&cr[1]);
                        rgb2yuv_bt709(rgb[2][0],rgb[2][1],rgb[2][2],&luma[2][0],&cb[2],&cr[2]);
                        rgb2yuv_bt709(rgb[3][0],rgb[3][1],rgb[3][2],&luma[3][0],&cb[3],&cr[3]);

                        cbcr[0] = (cb[0] + cb[1] + cb[2] + cb[3])/4;
                        cbcr[1] = (cr[0] + cr[1] + cr[2] + cr[3])/4;
                    }
                }
            }
            else if (dst_format == FOURCC_YUV4)
            {
                for (y = 0; y < dst_addr[0].dim_y; y++)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x++)
                    {
                        vx_uint8 *rgb  = vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]);
                        vx_uint8 *luma = vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]);
                        vx_uint8 *cb   = vxFormatImagePatchAddress2d(dst_base[1], x, y, &dst_addr[1]);
                        vx_uint8 *cr   = vxFormatImagePatchAddress2d(dst_base[2], x, y, &dst_addr[2]);

                        rgb2yuv_bt709(rgb[0],rgb[1],rgb[2],luma,cb,cr);
                    }
                }
            }
            else if (dst_format == FOURCC_IYUV)
            {
                vx_uint8 cb[4];
                vx_uint8 cr[4];
                for (y = 0; y < dst_addr[0].dim_y; y+=2)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x+=2)
                    {
                        vx_uint8 *rgb[4] = {vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]),
                                            vxFormatImagePatchAddress2d(src_base[0], x+1, y, &src_addr[0]),
                                            vxFormatImagePatchAddress2d(src_base[0], x, y+1, &src_addr[0]),
                                            vxFormatImagePatchAddress2d(src_base[0], x+1, y+1, &src_addr[0])};

                        vx_uint8 *luma[4] = {vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]),
                                             vxFormatImagePatchAddress2d(dst_base[0], x+1, y, &dst_addr[0]),
                                             vxFormatImagePatchAddress2d(dst_base[0], x, y+1, &dst_addr[0]),
                                             vxFormatImagePatchAddress2d(dst_base[0], x+1, y+1, &dst_addr[0])};

                        vx_uint8 *cbp = vxFormatImagePatchAddress2d(dst_base[1], x, y, &dst_addr[1]);
                        vx_uint8 *crp = vxFormatImagePatchAddress2d(dst_base[2], x, y, &dst_addr[2]);

                        rgb2yuv_bt709(rgb[0][0],rgb[0][1],rgb[0][2],&luma[0][0],&cb[0],&cr[0]);
                        rgb2yuv_bt709(rgb[1][0],rgb[1][1],rgb[1][2],&luma[1][0],&cb[1],&cr[1]);
                        rgb2yuv_bt709(rgb[2][0],rgb[2][1],rgb[2][2],&luma[2][0],&cb[2],&cr[2]);
                        rgb2yuv_bt709(rgb[3][0],rgb[3][1],rgb[3][2],&luma[3][0],&cb[3],&cr[3]);

                        cbp[0] = (uint8_t)(((vx_uint16)cb[0] + (vx_uint16)cb[1] + (vx_uint16)cb[2] + (vx_uint16)cb[3])>>2);
                        crp[0] = (uint8_t)(((vx_uint16)cr[0] + (vx_uint16)cr[1] + (vx_uint16)cr[2] + (vx_uint16)cr[3])>>2);
                    }
                }
            }
        }
        else if (src_format == FOURCC_NV21)
        {
            if ((dst_format == FOURCC_RGB) || (dst_format == FOURCC_RGBX))
            {
                for (y = 0; y < dst_addr[0].dim_y; y++)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x++)
                    {
                        vx_uint8 *luma = vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]);
                        vx_uint8 *crcb = vxFormatImagePatchAddress2d(src_base[1], x, y, &src_addr[1]);
                        vx_uint8 *rgb = vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]);

                        if (src_space == VX_COLOR_SPACE_BT601_525 ||
                            src_space == VX_COLOR_SPACE_BT601_625)
                            yuv2rgb_bt601(luma[0],crcb[0],crcb[1],&rgb[0],&rgb[1],&rgb[2]);
                        else if (src_space == VX_COLOR_SPACE_BT709)
                            yuv2rgb_bt709(luma[0],crcb[0],crcb[1],&rgb[0],&rgb[1],&rgb[2]);
                    }
                }
            }
            else if (dst_format == FOURCC_NV12)
            {
                for (y = 0; y < dst_addr[0].dim_y; y++)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x++)
                    {
                        vx_uint8 *luma[2] = {vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]),
                                             vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0])};
                        vx_uint8 *cbcr = vxFormatImagePatchAddress2d(src_base[1], x, y, &src_addr[1]);
                        vx_uint8 *crcb = vxFormatImagePatchAddress2d(dst_base[1], x, y, &dst_addr[1]);
                        yuv2yuv_601to709(luma[0][0],cbcr[0],cbcr[1],&luma[1][0],&crcb[1],&crcb[0]);
                    }
                }
            }
            else if (dst_format == FOURCC_YUV4)
            {
                for (y = 0; y < dst_addr[0].dim_y; y++)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x++)
                    {
                        vx_uint8 *luma = vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]);
                        vx_uint8 *crcb = vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]);
                        vx_uint8 *yout = vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]);
                        vx_uint8 *cb   = vxFormatImagePatchAddress2d(dst_base[1], x, y, &dst_addr[1]);
                        vx_uint8 *cr   = vxFormatImagePatchAddress2d(dst_base[2], x, y, &dst_addr[2]);

                        yout[0] = luma[0];
                        cb[0] = crcb[1];
                        cr[0] = crcb[0];
                    }
                }
            }
            else if (dst_format == FOURCC_IYUV)
            {
                for (y = 0; y < dst_addr[0].dim_y; y++)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x++)
                    {
                        vx_uint8 *luma = vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]);
                        vx_uint8 *crcb = vxFormatImagePatchAddress2d(src_base[1], x, y, &src_addr[1]);
                        vx_uint8 *yout = vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]);
                        vx_uint8 *cb   = vxFormatImagePatchAddress2d(dst_base[1], x, y, &dst_addr[1]);
                        vx_uint8 *cr   = vxFormatImagePatchAddress2d(dst_base[2], x, y, &dst_addr[2]);

                        yout[0] = luma[0];
                        cb[0] = crcb[1];
                        cr[0] = crcb[0];
                    }
                }
            }
        }
        else if (src_format == FOURCC_YUYV)
        {
            if (dst_format == FOURCC_RGB)
            {
                for (y = 0; y < dst_addr[0].dim_y; y++)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x+=2)
                    {
                        vx_uint8 *yuyv = vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]);
                        vx_uint8 *rgb  = vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]);

                        if (src_space == VX_COLOR_SPACE_BT601_525 ||
                            src_space == VX_COLOR_SPACE_BT601_625)
                        {
                            yuv2rgb_bt601(yuyv[0],yuyv[1],yuyv[3],&rgb[0],&rgb[1],&rgb[2]);
                            yuv2rgb_bt601(yuyv[2],yuyv[1],yuyv[3],&rgb[3],&rgb[4],&rgb[5]);
                        }
                        else if (src_space == VX_COLOR_SPACE_BT709)
                        {
                            yuv2rgb_bt709(yuyv[0],yuyv[1],yuyv[3],&rgb[0],&rgb[1],&rgb[2]);
                            yuv2rgb_bt709(yuyv[2],yuyv[1],yuyv[3],&rgb[3],&rgb[4],&rgb[5]);
                        }
                    }
                }
            }
            else if (dst_format == FOURCC_RGBX)
            {
                for (y = 0; y < dst_addr[0].dim_y; y++)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x+=2)
                    {
                        vx_uint8 *yuyv = vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]);
                        vx_uint8 *rgb  = vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]);

                        if (src_space == VX_COLOR_SPACE_BT601_525 ||
                            src_space == VX_COLOR_SPACE_BT601_625)
                        {
                            yuv2rgb_bt601(yuyv[0],yuyv[1],yuyv[3],&rgb[0],&rgb[1],&rgb[2]);
                            yuv2rgb_bt601(yuyv[2],yuyv[1],yuyv[3],&rgb[4],&rgb[5],&rgb[6]);
                        }
                        else if (src_space == VX_COLOR_SPACE_BT709)
                        {
                            yuv2rgb_bt709(yuyv[0],yuyv[1],yuyv[3],&rgb[0],&rgb[1],&rgb[2]);
                            yuv2rgb_bt709(yuyv[2],yuyv[1],yuyv[3],&rgb[4],&rgb[5],&rgb[6]);
                        }
                    }
                }
            }
            else if (dst_format == FOURCC_NV12)
            {
                for (y = 0; y < dst_addr[0].dim_y; y+=2)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x+=2)
                    {
                        vx_uint8 *yuyv[2] = {vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]),
                                              vxFormatImagePatchAddress2d(src_base[0], x, y+1, &src_addr[0])};
                        vx_uint8 *luma[4] = {vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]),
                                              vxFormatImagePatchAddress2d(dst_base[0], x+1, y, &dst_addr[0]),
                                              vxFormatImagePatchAddress2d(dst_base[0], x, y+1, &dst_addr[0]),
                                              vxFormatImagePatchAddress2d(dst_base[0], x+1, y+1, &dst_addr[0])};
                        vx_uint8 *cbcr = vxFormatImagePatchAddress2d(dst_base[1], x, y, &dst_addr[1]);
                        vx_uint8 cb[4];
                        vx_uint8 cr[4];
                        yuv2yuv_601to709(yuyv[0][0], yuyv[0][1], yuyv[0][3], &luma[0][0], &cb[0], &cr[0]);
                        yuv2yuv_601to709(yuyv[0][2], yuyv[0][1], yuyv[0][3], &luma[1][0], &cb[1], &cr[1]);
                        yuv2yuv_601to709(yuyv[1][0], yuyv[1][1], yuyv[1][3], &luma[2][0], &cb[2], &cr[2]);
                        yuv2yuv_601to709(yuyv[1][2], yuyv[1][1], yuyv[1][3], &luma[3][0], &cb[3], &cr[3]);
                        cbcr[0] = (cb[0] + cb[1] + cr[2] + cr[3])/4;
                        cbcr[1] = (cr[0] + cr[1] + cr[2] + cr[3])/4;
                    }
                }
            }
            else if (dst_format == FOURCC_YUV4)
            {
                for (y = 0; y < dst_addr[0].dim_y; y++)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x+=2)
                    {
                        vx_uint8 *yuyv = vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]);
                        vx_uint8 *luma = vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]);
                        vx_uint8 *cb   = vxFormatImagePatchAddress2d(dst_base[1], x, y, &dst_addr[1]);
                        vx_uint8 *cr   = vxFormatImagePatchAddress2d(dst_base[2], x, y, &dst_addr[2]);

                        yuv2yuv_601to709(yuyv[0],yuyv[1],yuyv[3],&luma[0],&cb[0],&cr[0]);
                        yuv2yuv_601to709(yuyv[2],yuyv[1],yuyv[3],&luma[1],&cb[1],&cr[1]);
                    }
                }
            }
            else if (dst_format == FOURCC_IYUV)
            {
                for (y = 0; y < dst_addr[0].dim_y; y+=2)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x+=2)
                    {
                        vx_uint8 *yuyv[2] = {vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]),
                                              vxFormatImagePatchAddress2d(src_base[0], x, y+1, &src_addr[0])};
                        vx_uint8 *luma[4] = {vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]),
                                              vxFormatImagePatchAddress2d(dst_base[0], x+1, y, &dst_addr[0]),
                                              vxFormatImagePatchAddress2d(dst_base[0], x, y+1, &dst_addr[0]),
                                              vxFormatImagePatchAddress2d(dst_base[0], x+1, y+1, &dst_addr[0])};
                        vx_uint8 *cb = vxFormatImagePatchAddress2d(dst_base[1], x, y, &dst_addr[1]);
                        vx_uint8 *cr = vxFormatImagePatchAddress2d(dst_base[2], x, y, &dst_addr[2]);

                        yuv2yuv_601to709(yuyv[0][0],yuyv[0][1],yuyv[0][3],&luma[0][0],&cb[0],&cr[0]);
                        yuv2yuv_601to709(yuyv[0][2],yuyv[0][1],yuyv[0][3],&luma[1][0],&cb[1],&cr[1]);
                        yuv2yuv_601to709(yuyv[1][0],yuyv[1][1],yuyv[0][3],&luma[2][0],&cb[0],&cr[0]);
                        yuv2yuv_601to709(yuyv[1][2],yuyv[1][1],yuyv[0][3],&luma[3][0],&cb[1],&cr[1]);
                    }
                }
            }
        }
        else if (src_format == FOURCC_UYVY)
        {
            if (dst_format == FOURCC_RGB)
            {
                for (y = 0; y < dst_addr[0].dim_y; y++)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x+=2)
                    {
                        vx_uint8 *uyvy = vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]);
                        vx_uint8 *rgb  = vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]);

                        if (src_space == VX_COLOR_SPACE_BT601_525 ||
                            src_space == VX_COLOR_SPACE_BT601_625)
                        {
                            yuv2rgb_bt601(uyvy[1],uyvy[0],uyvy[2],&rgb[0],&rgb[1],&rgb[2]);
                            yuv2rgb_bt601(uyvy[3],uyvy[0],uyvy[2],&rgb[3],&rgb[4],&rgb[5]);
                        }
                        else if (src_space == VX_COLOR_SPACE_BT709)
                        {
                            yuv2rgb_bt709(uyvy[1],uyvy[0],uyvy[2],&rgb[0],&rgb[1],&rgb[2]);
                            yuv2rgb_bt709(uyvy[3],uyvy[0],uyvy[2],&rgb[3],&rgb[4],&rgb[5]);
                        }
                    }
                }
            }
            else if (dst_format == FOURCC_RGBX)
            {
                for (y = 0; y < dst_addr[0].dim_y; y++)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x+=2)
                    {
                        vx_uint8 *uyvy = vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]);
                        vx_uint8 *rgb  = vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]);

                        if (src_space == VX_COLOR_SPACE_BT601_525 ||
                            src_space == VX_COLOR_SPACE_BT601_625)
                        {
                            yuv2rgb_bt601(uyvy[1],uyvy[0],uyvy[2],&rgb[0],&rgb[1],&rgb[2]);
                            yuv2rgb_bt601(uyvy[3],uyvy[0],uyvy[2],&rgb[4],&rgb[5],&rgb[6]);
                        }
                        else if (src_space == VX_COLOR_SPACE_BT709)
                        {
                            yuv2rgb_bt709(uyvy[1],uyvy[0],uyvy[2],&rgb[0],&rgb[1],&rgb[2]);
                            yuv2rgb_bt709(uyvy[3],uyvy[0],uyvy[2],&rgb[4],&rgb[5],&rgb[6]);
                        }
                    }
                }
            }
            else if (dst_format == FOURCC_NV12)
            {
                for (y = 0; y < dst_addr[0].dim_y; y+=2)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x+=2)
                    {
                        vx_uint8 *uyvy[2] = {vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]),
                                             vxFormatImagePatchAddress2d(src_base[0], x, y+1, &src_addr[0])};
                        vx_uint8 *luma[4] = {vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]),
                                             vxFormatImagePatchAddress2d(dst_base[0], x+1, y, &dst_addr[0]),
                                             vxFormatImagePatchAddress2d(dst_base[0], x, y+1, &dst_addr[0]),
                                             vxFormatImagePatchAddress2d(dst_base[0], x+1, y+1, &dst_addr[0])};
                        vx_uint8 *cbcr = vxFormatImagePatchAddress2d(dst_base[1], x, y, &dst_addr[1]);
                        vx_uint8 cb[4];
                        vx_uint8 cr[4];

                        yuv2yuv_601to709(uyvy[0][1],uyvy[0][0],uyvy[0][2],&luma[0][0],&cb[0],&cr[0]);
                        yuv2yuv_601to709(uyvy[0][3],uyvy[0][0],uyvy[0][2],&luma[1][0],&cb[1],&cr[1]);
                        yuv2yuv_601to709(uyvy[1][1],uyvy[1][0],uyvy[1][2],&luma[1][0],&cb[2],&cr[2]);
                        yuv2yuv_601to709(uyvy[1][3],uyvy[1][0],uyvy[1][2],&luma[1][0],&cb[3],&cr[3]);
                        cbcr[0] = (cb[0] + cb[1] + cb[2] + cb[3])/4;
                        cbcr[1] = (cr[0] + cr[1] + cr[2] + cr[3])/4;
                    }
                }
            }
            else if (dst_format == FOURCC_YUV4)
            {
                for (y = 0; y < dst_addr[0].dim_y; y++)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x+=2)
                    {
                        vx_uint8 *uyvy = vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]);
                        vx_uint8 *luma = vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]);
                        vx_uint8 *cb   = vxFormatImagePatchAddress2d(dst_base[1], x, y, &dst_addr[1]);
                        vx_uint8 *cr   = vxFormatImagePatchAddress2d(dst_base[2], x, y, &dst_addr[2]);

                        yuv2yuv_601to709(uyvy[1],uyvy[0],uyvy[2],&luma[0],&cb[0],&cr[0]);
                        yuv2yuv_601to709(uyvy[3],uyvy[0],uyvy[2],&luma[1],&cb[1],&cr[1]);
                    }
                }
            }
            else if (dst_format == FOURCC_IYUV)
            {
                for (y = 0; y < dst_addr[0].dim_y; y+=2)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x+=2)
                    {
                        vx_uint8 *uyvy[2] = {vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]),
                                             vxFormatImagePatchAddress2d(src_base[0], x, y+1, &src_addr[0])};
                        vx_uint8 *luma[4] = {vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]),
                                             vxFormatImagePatchAddress2d(dst_base[0], x+1, y, &dst_addr[0]),
                                             vxFormatImagePatchAddress2d(dst_base[0], x, y+1, &dst_addr[0]),
                                             vxFormatImagePatchAddress2d(dst_base[0], x+1, y+1, &dst_addr[0])};
                        vx_uint8 *cb = vxFormatImagePatchAddress2d(dst_base[1], x, y, &dst_addr[1]);
                        vx_uint8 *cr = vxFormatImagePatchAddress2d(dst_base[2], x, y, &dst_addr[2]);
                        vx_uint8 cb1[4], cr1[4];

                        yuv2yuv_601to709(uyvy[0][1],uyvy[0][0],uyvy[0][2], &luma[0][0], &cb1[0], &cr1[0]);
                        yuv2yuv_601to709(uyvy[0][3],uyvy[0][0],uyvy[0][2], &luma[1][0], &cb1[1], &cr1[1]);
                        yuv2yuv_601to709(uyvy[1][1],uyvy[1][0],uyvy[1][2], &luma[2][0], &cb1[2], &cr1[2]);
                        yuv2yuv_601to709(uyvy[1][3],uyvy[1][0],uyvy[1][2], &luma[3][0], &cb1[3], &cr1[3]);
                        cb[0] = (cb1[0] + cb1[1] + cb1[2] + cb1[3])/4;
                        cr[0] = (cr1[0] + cr1[1] + cr1[2] + cr1[3])/4;
                    }
                }
            }
        }
        else if (src_format == FOURCC_IYUV)
        {
            if (dst_format == FOURCC_RGB || dst_format == FOURCC_RGBX)
            {
                for (y = 0; y < dst_addr[0].dim_y; y++)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x++)
                    {
                        vx_uint8 *luma = vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]);
                        vx_uint8 *cb   = vxFormatImagePatchAddress2d(src_base[1], x, y, &src_addr[1]);
                        vx_uint8 *cr   = vxFormatImagePatchAddress2d(src_base[2], x, y, &src_addr[2]);
                        vx_uint8 *rgb  = vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]);

                        /*! \todo restricted range 601 ? */
                        if (src_space == VX_COLOR_SPACE_BT601_525 ||
                            src_space == VX_COLOR_SPACE_BT601_625)
                            yuv2rgb_bt601(luma[0],cb[0],cr[0],&rgb[0],&rgb[1],&rgb[2]);
                        else if (src_space == VX_COLOR_SPACE_BT709)
                            yuv2rgb_bt709(luma[0],cb[0],cr[0],&rgb[0],&rgb[1],&rgb[2]);
                    }
                }
            }
            else if (dst_format == FOURCC_NV12)
            {
                for (y = 0; y < dst_addr[0].dim_y; y++)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x++)
                    {
                        vx_uint8 *luma = vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]);
                        vx_uint8 *cb   = vxFormatImagePatchAddress2d(src_base[1], x, y, &src_addr[1]);
                        vx_uint8 *cr   = vxFormatImagePatchAddress2d(src_base[2], x, y, &src_addr[2]);
                        vx_uint8 *nv12[2] = {vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0]),
                                             vxFormatImagePatchAddress2d(dst_base[1], x, y, &dst_addr[1])};
                        nv12[0][0] = luma[0];
                        nv12[1][0] = cb[0];
                        nv12[1][1] = cr[1];
                    }
                }
            }
            else if (dst_format == FOURCC_YUV4)
            {
                for (y = 0; y < dst_addr[0].dim_y; y++)
                {
                    for (x = 0; x < dst_addr[0].dim_x; x++)
                    {
                        vx_uint8 *luma[2] = {vxFormatImagePatchAddress2d(src_base[0], x, y, &src_addr[0]),
                                             vxFormatImagePatchAddress2d(dst_base[0], x, y, &dst_addr[0])};
                        vx_uint8 *cb[2]   = {vxFormatImagePatchAddress2d(src_base[1], x, y, &src_addr[1]),
                                             vxFormatImagePatchAddress2d(dst_base[1], x, y, &dst_addr[1])};
                        vx_uint8 *cr[2]   = {vxFormatImagePatchAddress2d(src_base[2], x, y, &src_addr[2]),
                                             vxFormatImagePatchAddress2d(dst_base[2], x, y, &dst_addr[2])};
                        luma[1][0] = luma[0][0];
                        cb[1][0] = cb[0][0];
                        cr[1][0] = cr[0][0];
                    }
                }
            }
        }
        status = VX_SUCCESS;
        for (p = 0; p < src_planes; p++)
        {
            status |= vxCommitImagePatch(src, 0, p, &src_addr[p], src_base[p]);
        }
        for (p = 0; p < dst_planes; p++)
        {
            status |= vxCommitImagePatch(dst, rect, p, &dst_addr[p], dst_base[p]);
        }
        if (status != VX_SUCCESS)
        {
            VX_PRINT(VX_ZONE_ERROR, "Failed to set image patches on source or destination\n");
        }
        vxReleaseRectangle(&rect);
    }
    return status;
}

static vx_status vxColorConvertInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_SUCCESS;
    if (index == 0)
    {
        vx_parameter param = vxGetParameterByIndex(node, 0);
        if (param)
        {
            vx_image image = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &image, sizeof(image));
            if (image)
            {
                vx_fourcc format = 0;
                vx_uint32 width = 0, height = 0;

                vxQueryImage(image, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
                vxQueryImage(image, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
                vxQueryImage(image, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
                // check to make sure the input format is supported.
                switch (format)
                {
                    case FOURCC_RGB:  /* 8:8:8 interleaved */
                    case FOURCC_RGBX: /* 8:8:8:8 interleaved */
                    case FOURCC_NV21: /* 4:2:0 co-planar*/
                    case FOURCC_IYUV: /* 4:2:0 planar */
                        if (height & 1)
                        {
                            status = VX_ERROR_INVALID_DIMENSION;
                            break;
                        }
                        /* no break */
                    case FOURCC_YUYV: /* 4:2:2 interleaved */
                    case FOURCC_UYVY: /* 4:2:2 interleaved */
                        if (width & 1)
                        {
                            status = VX_ERROR_INVALID_DIMENSION;
                        }
                        break;
                    default:
                        status = VX_ERROR_INVALID_FORMAT;
                        break;
                }
            }
            else
            {
                status = VX_ERROR_INVALID_PARAMETERS;
            }
            vxReleaseParameter(&param);
        }
        else
        {
            status = VX_ERROR_INVALID_PARAMETERS;
        }
    }
    else
    {
        status = VX_ERROR_INVALID_PARAMETERS;
    }
    return status;
}

static vx_fourcc color_combos[][2] = {
        /* {src, dst} */
        {FOURCC_RGB, FOURCC_NV12},
        {FOURCC_RGB, FOURCC_YUV4},
        {FOURCC_RGB, FOURCC_IYUV},
        {FOURCC_RGBX,FOURCC_NV12},
        {FOURCC_RGBX,FOURCC_YUV4},
        {FOURCC_RGBX,FOURCC_IYUV},
        {FOURCC_NV21,FOURCC_RGB},
        {FOURCC_NV21,FOURCC_RGBX},
        {FOURCC_NV21,FOURCC_NV12},
        {FOURCC_NV21,FOURCC_YUV4},
        {FOURCC_NV21,FOURCC_IYUV},
        {FOURCC_UYVY,FOURCC_RGB},
        {FOURCC_UYVY,FOURCC_RGBX},
        {FOURCC_UYVY,FOURCC_NV12},
        {FOURCC_UYVY,FOURCC_YUV4},
        {FOURCC_UYVY,FOURCC_IYUV},
        {FOURCC_YUYV,FOURCC_RGB},
        {FOURCC_YUYV,FOURCC_RGBX},
        {FOURCC_YUYV,FOURCC_NV12},
        {FOURCC_YUYV,FOURCC_YUV4},
        {FOURCC_YUYV,FOURCC_IYUV},
        {FOURCC_IYUV,FOURCC_RGB},
        {FOURCC_IYUV,FOURCC_RGBX},
        {FOURCC_IYUV,FOURCC_NV12},
        {FOURCC_IYUV,FOURCC_YUV4},
};

static vx_status vxColorConvertOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 1)
    {
        vx_parameter param0 = vxGetParameterByIndex(node, 0);
        vx_parameter param1 = vxGetParameterByIndex(node, 1);
        if (param0 && param1)
        {
            vx_image output = 0, input = 0;
            vxQueryParameter(param0, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(input));
            vxQueryParameter(param1, VX_PARAMETER_ATTRIBUTE_REF, &output, sizeof(output));
            if (input && output)
            {
                vx_fourcc src = FOURCC_VIRT;
                vx_fourcc dst = FOURCC_VIRT;
                vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &src, sizeof(src));
                vxQueryImage(output, VX_IMAGE_ATTRIBUTE_FORMAT, &dst, sizeof(dst));
                if (dst != FOURCC_VIRT) /* can't be a unspecified format */
                {
                    vx_uint32 i = 0;
                    for (i = 0; i < dimof(color_combos); i++)
                    {
                        if ((color_combos[i][0] == src) &&
                            (color_combos[i][1] == dst))
                        {
                            ptr->type = VX_TYPE_IMAGE;
                            ptr->dim.image.format = dst;
                            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_WIDTH, &ptr->dim.image.width, sizeof(ptr->dim.image.width));
                            vxQueryImage(input, VX_IMAGE_ATTRIBUTE_HEIGHT, &ptr->dim.image.height, sizeof(ptr->dim.image.height));
                            status = VX_SUCCESS;
                            break;
                        }
                    }
                }
            }
            vxReleaseParameter(&param0);
            vxReleaseParameter(&param1);
        }
    }
    VX_PRINT(VX_ZONE_API, "%s:%u returned %d\n", __FUNCTION__, index, status);
    return status;
}


/*! \brief Declares the parameter types for \ref vxuColorConvert.
 * \ingroup group_implementation
 */
static vx_param_description_t color_convert_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

/*! \brief The exported kernel table entry */
vx_kernel_description_t colorconvert_kernel = {
    VX_KERNEL_COLOR_CONVERT,
    "org.khronos.openvx.color_convert",
    vxColorConvertKernel,
    color_convert_kernel_params, dimof(color_convert_kernel_params),
    vxColorConvertInputValidator,
    vxColorConvertOutputValidator,
    NULL,
    NULL,
};

