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
 * \brief The File IO Object Kernels.
 * \author Erik Rainey <erik.rainey@ti.com>
 * \defgroup group_debug_ext Debugging Extension
 */

#include <VX/vx.h>
#include <VX/vx_ext_debug.h>
#include <VX/vx_helper.h>
#include <stdio.h>

static vx_status vxFWriteImageKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 2)
    {
        vx_image input = (vx_image)parameters[0];
        vx_buffer file = (vx_buffer)parameters[1];
        vx_char *filename = NULL;
        vx_uint8 *src[4] = {NULL, NULL, NULL, NULL};
        vx_uint32 p, y, planes, width, height;
        vx_imagepatch_addressing_t addr[4];
        vx_fourcc format;
        FILE *fp = NULL;
        vx_rectangle rect;
        vx_char *ext = NULL;

        status = VX_SUCCESS; // assume success until it fails
        vxAccessBufferRange(file, 0, VX_MAX_FILE_NAME, (void **)&filename);
        //VX_PRINT(VX_ZONE_INFO, "filename=%s\n",filename);
        fp = fopen(filename, "wb+");
        if (fp == NULL) {
            vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Failed to open file %s\n",filename);
            return VX_FAILURE;
        }
        vxQueryImage(input, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
        vxQueryImage(input, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
        vxQueryImage(input, VX_IMAGE_ATTRIBUTE_PLANES, &planes, sizeof(planes));
        vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
        rect = vxCreateRectangle(vxGetContext(node), 0, 0, width, height);
        status = VX_SUCCESS;
        ext = strrchr(filename, '.');
        if (ext && (strcmp(ext, ".pgm") == 0 || strcmp(ext, ".PGM") == 0))
        {
            fprintf(fp, "P5\n# %s\n",filename);
            fprintf(fp, "%u %u\n", width, height);
            if (format == FOURCC_U8)
                fprintf(fp, "255\n");
            else if (format == FOURCC_S16)
                fprintf(fp, "65535\n");
            else if (format == FOURCC_U16)
                fprintf(fp, "65535\n");
        }
        for (p = 0u; p < planes; p++)
        {
            status |= vxAccessImagePatch(input, rect, p, &addr[p], (void **)&src[p]);
        }
        for (p = 0u; (p < planes) && (status == VX_SUCCESS); p++)
        {
            size_t len = addr[p].stride_x * (addr[p].dim_x * addr[p].scale_x)/VX_SCALE_UNITY;
            for (y = 0u; y < addr[p].dim_y; y+=addr[p].step_y)
            {
                vx_uint8 *ptr = vxFormatImagePatchAddress2d(src[p], 0, y, &addr[p]);
                size_t wrote = fwrite(ptr, 1, len, fp);
                if (wrote != len)
                {
                    vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Failed to write len %u units to file!\n");
                    status = VX_FAILURE;
                    break;
                }
            }
            if (status == VX_FAILURE)
            {
                vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Failed to write image to file correctly\n");
                break;
            }
        }
        for (p = 0u; p < planes; p++)
        {
            status |= vxCommitImagePatch(input, 0, p, &addr[p], src[p]);
        }
        if (status != VX_SUCCESS)
        {
            vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Failed to write image to file correctly\n");
        }
        fflush(fp);
        fclose(fp);
        vxReleaseRectangle(&rect);
        vxCommitBufferRange(file, 0, 0, filename);
    }
    return status;
}

static vx_status vxFWriteBufferKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 2)
    {
        vx_buffer buf = (vx_buffer)parameters[0];
        vx_buffer name = (vx_buffer)parameters[1];
        vx_size nu, us, nu2, us2;
        vx_uint8 *bufptr = NULL;
        vx_char *filename = NULL;
        FILE *fp = NULL;
        vxQueryBuffer(buf, VX_BUFFER_ATTRIBUTE_NUMUNITS, &nu, sizeof(nu));
        vxQueryBuffer(buf, VX_BUFFER_ATTRIBUTE_UNITSIZE, &us, sizeof(us));
        vxQueryBuffer(name, VX_BUFFER_ATTRIBUTE_NUMUNITS, &nu2, sizeof(nu2));
        vxQueryBuffer(name, VX_BUFFER_ATTRIBUTE_UNITSIZE, &us2, sizeof(us2));
        vxAccessBufferRange(name, 0, nu2, (void **)&filename);
        fp = fopen(filename, "wb+");
        if (fp)
        {
            vxAccessBufferRange(buf, 0, nu, (void **)&bufptr);
            if (fwrite(bufptr, us, nu, fp) == nu*us)
                status = VX_SUCCESS;
            vxCommitBufferRange(buf, 0, 0, bufptr);
            fclose(fp);
        }
        else
        {
            vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Failed to open file to write\n");
        }
        vxCommitBufferRange(name, 0, 0, filename);
    }
    return status;
}

static vx_status vxFReadImageKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 2)
    {
        vx_buffer file = (vx_buffer)parameters[0];
        vx_image input = (vx_image)parameters[1];
        vx_char *filename = NULL;
        vx_uint8 *src = NULL;
        vx_uint32 p, y, planes, width, height;
        vx_imagepatch_addressing_t addr;
        vx_fourcc format;
        FILE *fp = NULL;
        vx_char tmp[VX_MAX_FILE_NAME];
        vx_char *ext;
        vx_rectangle rect;

        status = VX_SUCCESS; // assume success until it fails
        vxAccessBufferRange(file, 0, VX_MAX_FILE_NAME, (void **)&filename);
        fp = fopen(filename, "rb");
        if (fp == NULL) {
            vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Failed to open file %s\n",filename);
            return VX_FAILURE;
        }
        vxQueryImage(input, VX_IMAGE_ATTRIBUTE_WIDTH, &width, sizeof(width));
        vxQueryImage(input, VX_IMAGE_ATTRIBUTE_HEIGHT, &height, sizeof(height));
        vxQueryImage(input, VX_IMAGE_ATTRIBUTE_PLANES, &planes, sizeof(planes));
        vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
        rect = vxCreateRectangle(vxGetContext(node), 0, 0, width, height);
        ext = strrchr(filename, '.');
        if (ext && (strcmp(ext, ".pgm") == 0 || strcmp(ext, ".PGM") == 0))
        {
            fgets(tmp, sizeof(tmp), fp); // PX
            fgets(tmp, sizeof(tmp), fp); // comment
            fgets(tmp, sizeof(tmp), fp); // W H
            fgets(tmp, sizeof(tmp), fp); // BPP
            /*! \todo double check image size? */
        }
        for (p = 0; p < planes; p++)
        {
            status = vxAccessImagePatch(input, rect, p, &addr, (void **)&src);
            if (status == VX_SUCCESS)
            {
                for (y = 0; y < addr.dim_y; y+=addr.step_y)
                {
                    vx_uint8 *srcp = vxFormatImagePatchAddress2d(src, 0, y, &addr);
                    vx_size len = ((addr.dim_x * addr.scale_x)/VX_SCALE_UNITY);
                    if (fread(srcp, addr.stride_x, len, fp) != len)
                    {
                        status = VX_FAILURE;
                        break;
                    }
                }
                if (status == VX_SUCCESS)
                {
                    status = vxCommitImagePatch(input, rect, p, &addr, src);
                }
                if (status != VX_SUCCESS)
                {
                    break;
                }
            }
        }
        fclose(fp);
        vxReleaseRectangle(&rect);
        vxCommitBufferRange(file, 0, 0, filename);
    }
    return status;
}

static vx_status vxFReadBufferKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    vx_status status = VX_FAILURE;
    if (num == 2)
    {
        vx_buffer name = (vx_buffer)parameters[0];
        vx_buffer buf = (vx_buffer)parameters[1];
        vx_size nu, us, nu2, us2;
        vx_uint8 *bufptr = NULL;
        vx_char *filename = NULL;
        FILE *fp = NULL;
        vxQueryBuffer(buf, VX_BUFFER_ATTRIBUTE_NUMUNITS, &nu, sizeof(nu));
        vxQueryBuffer(buf, VX_BUFFER_ATTRIBUTE_UNITSIZE, &us, sizeof(us));
        vxQueryBuffer(name, VX_BUFFER_ATTRIBUTE_NUMUNITS, &nu2, sizeof(nu2));
        vxQueryBuffer(name, VX_BUFFER_ATTRIBUTE_UNITSIZE, &us2, sizeof(us2));
        vxAccessBufferRange(name, 0, nu2, (void **)&filename);
        fp = fopen(filename, "rb");
        if (fp)
        {
            vxAccessBufferRange(buf, 0, nu, (void **)&bufptr);
            if (fread(bufptr, us, nu, fp) == (nu*us))
            {
                status = VX_SUCCESS;
            }
            vxCommitBufferRange(buf, 0, 0, bufptr);
            fclose(fp);
        }
        else
        {
            vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Failed to open file to write\n");
        }
        vxCommitBufferRange(name, 0, 0, filename);
        status = VX_SUCCESS;
    }
    return status;
}

static vx_status vxFWriteImageInputValidator(vx_node node, vx_uint32 index)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if (index == 0)
    {
        vx_parameter param = vxGetParameterByIndex(node, index);
        if(param)
        {
            vx_image img = 0;
            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &img, sizeof(img));
            if (img)
            {
                vx_fourcc formats[] = {FOURCC_U8, FOURCC_S16, FOURCC_U16, FOURCC_U32, FOURCC_UYVY, FOURCC_YUYV, FOURCC_RGB};
                vx_fourcc format = 0;
                vx_uint32 i = 0;
                vxQueryImage(img, VX_IMAGE_ATTRIBUTE_FORMAT, &format, sizeof(format));
                for (i = 0; i < dimof(formats); i++)
                {
                    if (formats[i] == format)
                    {
                        status = VX_SUCCESS;
                        break;
                    }
                }
            }
            vxReleaseParameter(&param);
        }
    }
    else if(index == 1)
    {
        vx_parameter param = vxGetParameterByIndex(node, 1);
        if(param)
        {
            vx_buffer file = 0;
            vx_char *filename = NULL;

            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &file, sizeof(file));
            if(file)
            {
                vxAccessBufferRange(file, 0, VX_MAX_FILE_NAME, (void **)&filename);
                if(!strcmp(filename, ""))
                {
                    vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Empty file name. %s\n",filename);
                    status = VX_FAILURE;
                }
                else
                {
                    status = VX_SUCCESS;
                }
                vxCommitBufferRange(file, 0, VX_MAX_FILE_NAME, filename);
            }
            vxReleaseParameter(&param);
        }
    }
    return status;
}

static vx_status vxReadImageInputValidator(vx_node node, vx_uint32 index)
{
    if(index == 0)
    {
        vx_parameter param = vxGetParameterByIndex(node, 0);
        if(param)
        {
            vx_buffer file = 0;
            vx_char *filename = NULL;

            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &file, sizeof(file));
            if(file)
            {
                vxAccessBufferRange(file, 0, VX_MAX_FILE_NAME, (void **)&filename);
                if(!strcmp(filename, ""))
                {
                    vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Empty file name. %s\n",filename);
                    return VX_FAILURE;
                }
                vxCommitBufferRange(file, 0, VX_MAX_FILE_NAME, filename);
            }
            vxReleaseParameter(&param);
        }
    }
    return VX_SUCCESS;
}

static vx_status vxReadImageOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    vx_status status = VX_ERROR_INVALID_PARAMETERS;
    if(index == 1)
    {
        vx_parameter param = vxGetParameterByIndex(node, 0);
        if (param)
        {
            vx_buffer file = 0;
            vx_char *filename = NULL;
            vx_char *ext = NULL;
            FILE *fp = NULL;
            vx_char tmp[VX_MAX_FILE_NAME];
            vx_uint32 width = 0, height = 0;
            vx_fourcc format = FOURCC_VIRT;

            vxQueryParameter(param, VX_PARAMETER_ATTRIBUTE_REF, &file, sizeof(file));

            if (file)
            {
                //find metadata
                vxAccessBufferRange(file, 0, VX_MAX_FILE_NAME, (void **)&filename);
                fp = fopen(filename, "rb");
                if (fp == NULL) {
                    vxAddLogEntry(vxGetContext(node), VX_FAILURE, "Failed to open file %s\n",filename);
                    return VX_FAILURE;
                }
                ext = strrchr(filename, '.');
                if (ext)
                {
                    vx_uint32 depth = 0;
                    if ((strcmp(ext, ".pgm") == 0 || strcmp(ext, ".PGM") == 0))
                    {
                        fgets(tmp, sizeof(tmp), fp); // PX
                        fgets(tmp, sizeof(tmp), fp); // comment
                        fgets(tmp, sizeof(tmp), fp); // W H
                        sscanf(tmp, "%u %u", &width, &height);
                        fgets(tmp, sizeof(tmp), fp); // BPP
                        sscanf(tmp, "%u", &depth);
                        if (UINT8_MAX == depth)
                            format = FOURCC_U8;
                        else if (INT16_MAX == depth)
                            format = FOURCC_S16;
                        else if (UINT16_MAX == depth)
                            format = FOURCC_U16;
                    }
                    else if (strcmp(ext, ".bw") == 0)
                    {
                        vx_char shortname[256] = {0};
                        vx_char fmt[5] = {0};
                        vx_char cbps[3]= {0};
                        sscanf(filename, "%s_%ux%u_%s_%s.bw", shortname, &width, &height, fmt, cbps);
                        if (strcmp(fmt,"P400") == 0)
                        {
                            format = FOURCC_U8;
                        }
                    }
                    else if (strcmp(ext, ".yuv") == 0)
                    {
                        vx_char shortname[256] = {0};
                        vx_char fmt[5] = {0};
                        vx_char cbps[3]= {0};
                        sscanf(filename, "%s_%ux%u_%s_%s.bw", shortname, &width, &height, fmt, cbps);
                        if (strcmp(fmt,"IYUV") == 0)
                        {
                            format = FOURCC_IYUV;
                        }
                        if (strcmp(fmt,"UYVY") == 0)
                        {
                            format = FOURCC_UYVY;
                        }
                        if (strcmp(fmt,"P444") == 0)
                        {
                            format = FOURCC_YUV4;
                        }
                    }
                    else if (strcmp(ext, ".rgb") == 0)
                    {
                        vx_char shortname[256] = {0};
                        vx_char fmt[5] = {0};
                        vx_char cbps[3]= {0};
                        sscanf(filename, "%s_%ux%u_%s_%s.bw", shortname, &width, &height, fmt, cbps);
                        if (strcmp(fmt,"I444") == 0)
                        {
                            format = FOURCC_RGB;
                        }
                    }
                }

                status = VX_SUCCESS;

                ptr->type = VX_TYPE_IMAGE;
                ptr->dim.image.format = format;
                ptr->dim.image.width = width;
                ptr->dim.image.height = height;
                fclose(fp);
                vxCommitBufferRange(file, 0, 0, filename);
            }
            vxReleaseParameter(&param);
        }
    }
    return status;
}

static vx_status vxAllPassInputValidator(vx_node node, vx_uint32 index)
{
    return VX_SUCCESS;
}

static vx_status vxAllPassOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t *ptr)
{
    return VX_SUCCESS;
}

/*! \brief Declares the parameter types for \ref vxFWriteImageNode.
  * \ingroup group_debug_ext
  */
static vx_param_description_t fwriteimage_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_BUFFER, VX_PARAMETER_STATE_REQUIRED},
};

/*! \brief Declares the parameter types for \ref vxFWriteBufferNode.
 * \ingroup group_debug_ext
 */
static vx_param_description_t fwritebuffer_kernel_params[] = {
    {VX_INPUT, VX_TYPE_BUFFER, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_BUFFER, VX_PARAMETER_STATE_REQUIRED},
};

/*! \brief Declares the parameter types for \ref vxFReadImageNode.
  * \ingroup group_debug_ext
  */
static vx_param_description_t freadimage_kernel_params[] = {
    {VX_INPUT, VX_TYPE_BUFFER, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
};

/*! \brief Declares the parameter types for \ref vxFReadBufferNode.
 * \ingroup group_debug_ext
 */
static vx_param_description_t freadbuffer_kernel_params[] = {
    {VX_INPUT, VX_TYPE_BUFFER, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_BUFFER, VX_PARAMETER_STATE_REQUIRED},
};

vx_kernel_description_t fwriteimage_kernel = {
    VX_KERNEL_DEBUG_FWRITE_IMAGE,
    "org.khronos.debug.fwrite_image",
    vxFWriteImageKernel,
    fwriteimage_kernel_params, dimof(fwriteimage_kernel_params),
    vxFWriteImageInputValidator,
    vxAllPassOutputValidator,
    NULL, NULL,
};

vx_kernel_description_t fwritebuffer_kernel = {
    VX_KERNEL_DEBUG_FWRITE_BUFFER,
    "org.khronos.debug.fwrite_buffer",
    vxFWriteBufferKernel,
    fwritebuffer_kernel_params, dimof(fwritebuffer_kernel_params),
    vxAllPassInputValidator,
    vxAllPassOutputValidator,
    NULL, NULL,
};

vx_kernel_description_t freadimage_kernel = {
    VX_KERNEL_DEBUG_FREAD_IMAGE,
    "org.khronos.debug.fread_image",
    vxFReadImageKernel,
    freadimage_kernel_params, dimof(freadimage_kernel_params),
    vxReadImageInputValidator,
    vxReadImageOutputValidator,
    NULL, NULL,
};

vx_kernel_description_t freadbuffer_kernel = {
    VX_KERNEL_DEBUG_FREAD_BUFFER,
    "org.khronos.debug.fread_buffer",
    vxFReadBufferKernel,
    freadbuffer_kernel_params, dimof(freadbuffer_kernel_params),
    vxAllPassInputValidator,
    vxAllPassOutputValidator,
    NULL, NULL,
};
