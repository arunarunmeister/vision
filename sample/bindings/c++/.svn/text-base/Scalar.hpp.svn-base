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

#ifndef _VX_SCALAR_HPP_
#define _VX_SCALAR_HPP_

#include <vx.hpp>
#include <assert.h>

namespace OpenVX {
    template <> class Scalar<vx_char> : public Reference {
    public:
        Scalar<vx_char>(vx_char v) : Reference(vxCreateScalar(Context::getContext().handle(), VX_TYPE_CHAR, &v)) {
        }
        ~Scalar<vx_char>() {
            vxReleaseScalar(&m_handle);
        }
        vx_char get() {
            vx_char v = 0;
            vxAccessScalarValue(m_handle, &v);
            return v;
        }
        void set(vx_char v) {
            vxCommitScalarValue(m_handle, &v);
        }
        friend class Context;
    };

    template <> class Scalar<vx_uint8> : public Reference {
    public:
        Scalar<vx_uint8>(vx_uint8 v) : Reference(vxCreateScalar(Context::getContext().handle(), VX_TYPE_UINT8, &v)) {
        }
        ~Scalar<vx_uint8>() {
            vxReleaseScalar(&m_handle);
        }
        vx_uint8 get() {
            vx_uint8 v = 0;
            vxAccessScalarValue(m_handle, &v);
            return v;
        }
        void set(vx_uint8 v) {
            vxCommitScalarValue(m_handle, &v);
        }
        friend class Context;
    };

    template <> class Scalar<vx_int8> : public Reference {
    public:
        Scalar<vx_int8>(vx_uint8 v) : Reference(vxCreateScalar(Context::getContext().handle(), VX_TYPE_INT8, &v)) {
        }
        ~Scalar<vx_int8>() {
            vxReleaseScalar(&m_handle);
        }
    public:
        vx_int8 get() {
            vx_int8 v = 0;
            vxAccessScalarValue(m_handle, &v);
            return v;
        }
        void set(vx_int8 v) {
            vxCommitScalarValue(m_handle, &v);
        }
        friend class Context;
    };

    template <> class Scalar<vx_uint16> : public Reference {
    public:
        Scalar<vx_uint16>(vx_uint16 v) : Reference(vxCreateScalar(Context::getContext().handle(), VX_TYPE_UINT16, &v)) {
        }
        ~Scalar<vx_uint16>() {
            vxReleaseScalar(&m_handle);
        }
    public:
        vx_uint16 get() {
            vx_uint16 v = 0;
            vxAccessScalarValue(m_handle, &v);
            return v;
        }
        void set(vx_uint16 v) {
            vxCommitScalarValue(m_handle, &v);
        }
        friend class Context;
    };

    template <> class Scalar<vx_int16> : public Reference {
    public:
        Scalar<vx_int16>(vx_uint16 v) : Reference(vxCreateScalar(Context::getContext().handle(), VX_TYPE_INT16, &v)) {
        }
        ~Scalar<vx_int16>() {
            vxReleaseScalar(&m_handle);
        }
    public:
        vx_int16 get() {
            vx_int16 v = 0;
            vxAccessScalarValue(m_handle, &v);
            return v;
        }
        void set(vx_int16 v) {
            vxCommitScalarValue(m_handle, &v);
        }
        friend class Context;
    };

    template <> class Scalar<vx_uint32> : public Reference {
    public:
        Scalar<vx_uint32>(vx_uint32 v) : Reference(vxCreateScalar(Context::getContext().handle(), VX_TYPE_UINT32, &v)) {
        }
        ~Scalar<vx_uint32>() {
            vxReleaseScalar(&m_handle);
        }
    public:
        vx_uint32 get() {
            vx_uint32 v = 0;
            vxAccessScalarValue(m_handle, &v);
            return v;
        }
        void set(vx_uint32 v) {
            vxCommitScalarValue(m_handle, &v);
        }
        friend class Context;
    };

    template <> class Scalar<vx_int32> : public Reference {
    public:
        Scalar<vx_int32>(vx_uint32 v) : Reference(vxCreateScalar(Context::getContext().handle(), VX_TYPE_INT32, &v)) {
        }
        ~Scalar<vx_int32>() {
            vxReleaseScalar(&m_handle);
        }
    public:
        vx_int32 get() {
            vx_int32 v = 0;
            vxAccessScalarValue(m_handle, &v);
            return v;
        }
        void set(vx_int32 v) {
            vxCommitScalarValue(m_handle, &v);
        }
        friend class Context;
    };

    template <> class Scalar<vx_uint64> : public Reference {
    public:
        Scalar<vx_uint64>(vx_uint64 v) : Reference(vxCreateScalar(Context::getContext().handle(), VX_TYPE_UINT64, &v)) {
        }
        ~Scalar<vx_uint64>() {
            vxReleaseScalar(&m_handle);
        }
    public:
        vx_uint64 get() {
            vx_uint64 v = 0;
            vxAccessScalarValue(m_handle, &v);
            return v;
        }
        void set(vx_uint64 v) {
            vxCommitScalarValue(m_handle, &v);
        }
        friend class Context;
    };

    template <> class Scalar<vx_int64> : public Reference {
    public:
        Scalar<vx_int64>(vx_uint64 v) : Reference(vxCreateScalar(Context::getContext().handle(), VX_TYPE_INT64, &v)) {
        }
        ~Scalar<vx_int64>() {
            vxReleaseScalar(&m_handle);
        }
    public:
        vx_int64 get() {
            vx_int64 v = 0;
            vxAccessScalarValue(m_handle, &v);
            return v;
        }
        void set(vx_int64 v) {
            vxCommitScalarValue(m_handle, &v);
        }
        friend class Context;
    };

    template <> class Scalar<vx_float32> : public Reference {
    public:
        Scalar<vx_float32>(vx_float32 v) : Reference(vxCreateScalar(Context::getContext().handle(), VX_TYPE_FLOAT32, &v)) {
        }
        ~Scalar<vx_float32>() {
            vxReleaseScalar(&m_handle);
        }
    public:
        vx_float32 get() {
            vx_float32 v = 0;
            vxAccessScalarValue(m_handle, &v);
            return v;
        }
        void set(vx_float32 v) {
            vxCommitScalarValue(m_handle, &v);
        }
        friend class Context;
    };

    template <> class Scalar<vx_float64> : public Reference {
    public:
        Scalar<vx_float64>(vx_float64 v) : Reference(vxCreateScalar(Context::getContext().handle(), VX_TYPE_FLOAT64, &v)) {
        }
        ~Scalar<vx_float64>() {
            vxReleaseScalar(&m_handle);
        }
    public:
        vx_float64 get() {
            vx_float64 v = 0;
            vxAccessScalarValue(m_handle, &v);
            return v;
        }
        void set(vx_float64 v) {
            vxCommitScalarValue(m_handle, &v);
        }
        friend class Context;
    };

#if 0
    template <> class Scalar<vx_size> : public Reference {
    public:
        Scalar<vx_size>(vx_size v) : Reference(vxCreateScalar(Context::getContext().handle(), VX_TYPE_SIZE, &v)) {
        }
        ~Scalar<vx_size>() {
            vxReleaseScalar(&m_handle);
        }
    public:
        vx_size get() {
            vx_size v = 0;
            vxAccessScalarValue(m_handle, &v);
            return v;
        }
        void set(vx_size v) {
            vxCommitScalarValue(m_handle, &v);
        }
        friend class Context;
    };

    template <> class Scalar<vx_enum> : public Reference {
    public:
        Scalar<vx_enum>(vx_enum v) : Reference(vxCreateScalar(Context::getContext().handle(), VX_TYPE_ENUM, &v)) {
        }
        ~Scalar<vx_enum>() {
            vxReleaseScalar(&m_handle);
        }
    public:
        vx_enum get() {
            vx_enum v = 0;
            vxAccessScalarValue(m_handle, &v);
            return v;
        }
        void set(vx_enum v) {
            vxCommitScalarValue(m_handle, &v);
        }
        friend class Context;
    };

    template <> class Scalar<vx_fourcc> : public Reference {
    public:
        Scalar<vx_fourcc>(vx_fourcc v) : Reference(vxCreateScalar(Context::getContext().handle(), VX_TYPE_FOURCC, &v)) {
        }
        ~Scalar<vx_fourcc>() {
            vxReleaseScalar(&m_handle);
        }
    public:
        vx_fourcc get() {
            vx_fourcc v = 0;
            vxAccessScalarValue(m_handle, &v);
            return v;
        }
        void set(vx_fourcc v) {
            vxCommitScalarValue(m_handle, &v);
        }
        friend class Context;
    };
#endif
}

#endif

