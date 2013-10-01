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

#include <vx.hpp>

using namespace OpenVX;

Image::Image(vx_uint32 width,
             vx_uint32 height,
             vx_fourcc color) :
             Reference(vxCreateImage(Context::getContext().handle(), width, height, color))
{
}

//Image::Image(vx_image ref) : Reference(ref)
//{
//}

Image::Image() : Reference(vxCreateVirtualImage(Context::getContext().handle())) {}

Image::Image(vx_fourcc color) : Reference(vxCreateVirtualImageWithFormat(Context::getContext().handle(), color)) {}

Image::~Image()
{
    vxReleaseImage(&m_handle);
}

vx_uint32 Image::width()
{
    vx_uint32 v;
    vxQueryImage(handle(), VX_IMAGE_ATTRIBUTE_WIDTH, &v, sizeof(v));
    return v;
}

vx_uint32 Image::height()
{
    vx_uint32 v;
    vxQueryImage(handle(), VX_IMAGE_ATTRIBUTE_HEIGHT, &v, sizeof(v));
    return v;
}

vx_fourcc Image::format()
{
    vx_fourcc v;
    vxQueryImage(handle(), VX_IMAGE_ATTRIBUTE_FORMAT, &v, sizeof(v));
    return v;
}

vx_enum Image::range()
{
    vx_enum v;
    vxQueryImage(handle(), VX_IMAGE_ATTRIBUTE_RANGE, &v, sizeof(v));
    return v;
}

vx_enum Image::space()
{
    vx_enum v;
    vxQueryImage(handle(), VX_IMAGE_ATTRIBUTE_SPACE, &v, sizeof(v));
    return v;
}

vx_uint32 Image::planes()
{
    vx_uint32 v;
    vxQueryImage(handle(), VX_IMAGE_ATTRIBUTE_PLANES, &v, sizeof(v));
    return v;
}

vx_status Image::GetImagePatch(OpenVX::Rectangle *rect, vx_uint32 p, vx_imagepatch_addressing_t *addr, void **ptr)
{
    return vxAccessImagePatch(handle(), (rect!=NULL?rect->handle():0), p, addr, ptr);
}

vx_status Image::SetImagePatch(OpenVX::Rectangle *rect, vx_uint32 p, vx_imagepatch_addressing_t *addr, void *ptr)
{
    return vxCommitImagePatch(handle(), (rect!=NULL?rect->handle():0), p, addr, ptr);
}

vx_size Image::ComputePlaneSize(OpenVX::Rectangle *rect, vx_uint32 p)
{
    return vxComputeImagePatchSize(handle(), (rect!=NULL?rect->handle():0), p);
}

