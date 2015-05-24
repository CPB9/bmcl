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
class Reader {
public:
    void read(void* dest, std::size_t size);
    void skip(std::size_t size);
    std::size_t readableSize() const;

    uint8_t readUint8();
    uint16_t readUint16();
    uint32_t readUint32();
    uint64_t readUint64();

    uint16_t readUint16Le();
    uint32_t readUint32Le();
    uint64_t readUint64Le();

    uint16_t readUint16Be();
    uint32_t readUint32Be();
    uint64_t readUint64Be();

    int8_t readInt8();
    int16_t readInt16();
    int32_t readInt32();
    int64_t readInt64();

    int16_t readInt16Le();
    int32_t readInt32Le();
    int64_t readInt64Le();

    int16_t readInt16Be();
    int32_t readInt32Be();
    int64_t readInt64Be();

    float readFloat32Le();
    double readFloat64Le();

    float readFloat32Be();
    double readFloat64Be();

private:
    template <typename T>
    inline T readType();

    template <typename T, typename H, typename C>
    inline T readFloat(C convert);
};

template <typename B>
inline void Reader<B>::read(void* dest, std::size_t size)
{
    static_cast<B*>(this)->readImpl(dest, size);
}

template <typename B>
inline void Reader<B>::skip(std::size_t size)
{
    static_cast<B*>(this)->skipImpl(size);
}

template <typename B>
inline std::size_t Reader<B>::readableSize() const
{
    return static_cast<const B*>(this)->readableSizeImpl();
}

template <typename B>
template <typename T>
inline T Reader<B>::readType()
{
    T value;
    read(&value, sizeof(value));
    return value;
}

template <typename B>
inline uint8_t Reader<B>::readUint8()
{
    return readType<uint8_t>();
}

template <typename B>
inline uint16_t Reader<B>::readUint16()
{
    return readType<uint16_t>();
}

template <typename B>
inline uint32_t Reader<B>::readUint32()
{
    return readType<uint32_t>();
}

template <typename B>
inline uint64_t Reader<B>::readUint64()
{
    return readType<uint64_t>();
}

template <typename B>
inline uint16_t Reader<B>::readUint16Le()
{
    return le16toh(readType<uint16_t>());
}

template <typename B>
inline uint32_t Reader<B>::readUint32Le()
{
    return le32toh(readType<uint32_t>());
}

template <typename B>
inline uint64_t Reader<B>::readUint64Le()
{
    return le64toh(readType<uint64_t>());
}

template <typename B>
inline uint16_t Reader<B>::readUint16Be()
{
    return be16toh(readType<uint16_t>());
}

template <typename B>
inline uint32_t Reader<B>::readUint32Be()
{
    return be32toh(readType<uint32_t>());
}

template <typename B>
inline uint64_t Reader<B>::readUint64Be()
{
    return be64toh(readType<uint64_t>());
}

template <typename B>
inline int8_t Reader<B>::readInt8()
{
    return readUint8();
}

template <typename B>
inline int16_t Reader<B>::readInt16()
{
    return readUint16();
}

template <typename B>
inline int32_t Reader<B>::readInt32()
{
    return readUint32();
}

template <typename B>
inline int64_t Reader<B>::readInt64()
{
    return readUint64();
}

template <typename B>
inline int16_t Reader<B>::readInt16Le()
{
    return readUint16Le();
}

template <typename B>
inline int32_t Reader<B>::readInt32Le()
{
    return readUint32Le();
}

template <typename B>
inline int64_t Reader<B>::readInt64Le()
{
    return readUint64Le();
}

template <typename B>
inline int16_t Reader<B>::readInt16Be()
{
    return readUint16Be();
}

template <typename B>
inline int32_t Reader<B>::readInt32Be()
{
    return readUint32Be();
}

template <typename B>
inline int64_t Reader<B>::readInt64Be()
{
    return readUint64Be();
}

template <typename B>
template <typename T, typename H, typename C>
inline T Reader<B>::readFloat(C convert)
{
    BMCL_ASSERT(std::numeric_limits<T>::is_iec559);
    H value = readType<H>();
    T swapped;
    convert(&swapped, value);
    return swapped;
}

template <typename B>
inline float Reader<B>::readFloat32Le()
{
    return readFloat<float, uint32_t>(le32enc);
}

template <typename B>
inline double Reader<B>::readFloat64Le()
{
    return readFloat<double, uint64_t>(le64enc);
}

template <typename B>
inline float Reader<B>::readFloat32Be()
{
    return readFloat<float, uint32_t>(be32enc);
}

template <typename B>
inline double Reader<B>::readFloat64Be()
{
    return readFloat<double, uint64_t>(be64enc);
}
}
