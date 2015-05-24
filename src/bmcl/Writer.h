/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/Assert.h"
#include "bmcl/Endian.h"

#include <cstddef>
#include <stdint.h>
#include <cstdlib>
#include <limits>

namespace bmcl {

template <typename B>
class Writer {
public:
    std::size_t writableSize() const;
    void write(const void* data, std::size_t size);

    template <std::size_t n, typename R>
    void write(R(&array)[n]);

    void writeUint8(uint8_t value);
    void writeUint16(uint16_t value);
    void writeUint32(uint32_t value);
    void writeUint64(uint64_t value);

    void writeUint16Le(uint16_t value);
    void writeUint32Le(uint32_t value);
    void writeUint64Le(uint64_t value);

    void writeUint16Be(uint16_t value);
    void writeUint32Be(uint32_t value);
    void writeUint64Be(uint64_t value);

    void writeInt8(int8_t value);
    void writeInt16(int16_t value);
    void writeInt32(int32_t value);
    void writeInt64(int64_t value);

    void writeInt16Le(int16_t value);
    void writeInt32Le(int32_t value);
    void writeInt64Le(int64_t value);

    void writeInt16Be(int16_t value);
    void writeInt32Be(int32_t value);
    void writeInt64Be(int64_t value);

    void writeFloat32Le(float value);
    void writeFloat64Le(double value);

    void writeFloat32Be(float value);
    void writeFloat64Be(double value);

private:
    template <typename T>
    void writeType(T value);

    template <typename T, typename H, typename C>
    inline void writeFloat(T value, C convert);

};

template <typename B>
inline std::size_t Writer<B>::writableSize() const
{
    return static_cast<const B*>(this)->writableSizeImpl();
}

template <typename B>
inline void Writer<B>::write(const void* data, std::size_t size)
{
    static_cast<B*>(this)->writeImpl(data, size);
}

template <typename B>
template <std::size_t n, typename R>
inline void Writer<B>::write(R(&array)[n])
{
    write(array, n * sizeof(R));
}

template <typename B>
template <typename T>
inline void Writer<B>::writeType(T value)
{
    write(&value, sizeof(value));
}

template <typename B>
inline void Writer<B>::writeUint8(uint8_t value)
{
    writeType<uint8_t>(value);
}

template <typename B>
inline void Writer<B>::writeUint16(uint16_t value)
{
    writeType<uint16_t>(value);
}

template <typename B>
inline void Writer<B>::writeUint32(uint32_t value)
{
    writeType<uint32_t>(value);
}

template <typename B>
inline void Writer<B>::writeUint64(uint64_t value)
{
    writeType<uint64_t>(value);
}

template <typename B>
inline void Writer<B>::writeUint16Le(uint16_t value)
{
    writeType<uint16_t>(htole16(value));
}

template <typename B>
inline void Writer<B>::writeUint32Le(uint32_t value)
{
    writeType<uint32_t>(htole32(value));
}

template <typename B>
inline void Writer<B>::writeUint64Le(uint64_t value)
{
    writeType<uint64_t>(htole64(value));
}

template <typename B>
inline void Writer<B>::writeUint16Be(uint16_t value)
{
    writeType<uint16_t>(htobe16(value));
}

template <typename B>
inline void Writer<B>::writeUint32Be(uint32_t value)
{
    writeType<uint32_t>(htobe32(value));
}

template <typename B>
inline void Writer<B>::writeUint64Be(uint64_t value)
{
    writeType<uint64_t>(htobe64(value));
}

template <typename B>
inline void Writer<B>::writeInt8(int8_t value)
{
    writeUint8(value);
}

template <typename B>
inline void Writer<B>::writeInt16(int16_t value)
{
    writeUint16(value);
}

template <typename B>
inline void Writer<B>::writeInt32(int32_t value)
{
    writeUint32(value);
}

template <typename B>
inline void Writer<B>::writeInt64(int64_t value)
{
    writeUint64(value);
}

template <typename B>
inline void Writer<B>::writeInt16Le(int16_t value)
{
    writeUint16Le(value);
}

template <typename B>
inline void Writer<B>::writeInt32Le(int32_t value)
{
    writeUint32Le(value);
}

template <typename B>
inline void Writer<B>::writeInt64Le(int64_t value)
{
    writeUint64Le(value);
}

template <typename B>
inline void Writer<B>::writeInt16Be(int16_t value)
{
    writeUint16Be(value);
}

template <typename B>
inline void Writer<B>::writeInt32Be(int32_t value)
{
    writeUint32Be(value);
}

template <typename B>
inline void Writer<B>::writeInt64Be(int64_t value)
{
    writeUint64Be(value);
}

template <typename B>
template <typename T, typename H, typename C>
inline void Writer<B>::writeFloat(T value, C convert)
{
    BMCL_ASSERT(std::numeric_limits<T>::is_iec559);
    H swapped = convert(&value);
    write(&swapped, sizeof(H));
}

template <typename B>
inline void Writer<B>::writeFloat32Le(float value)
{
    writeFloat<float, uint32_t>(value, le32dec);
}

template <typename B>
inline void Writer<B>::writeFloat64Le(double value)
{
    writeFloat<double, uint64_t>(value, le64dec);
}

template <typename B>
inline void Writer<B>::writeFloat32Be(float value)
{
    writeFloat<float, uint32_t>(value, be32dec);
}

template <typename B>
inline void Writer<B>::writeFloat64Be(double value)
{
    writeFloat<double, uint64_t>(value, be64dec);
}
}
