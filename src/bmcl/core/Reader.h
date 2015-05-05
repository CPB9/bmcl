/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/config.h"
#include "bmcl/core/Endian.h"

#include <cstddef>
#include <stdint.h>
#include <cstdlib>
#include <cassert>
#include <limits>

namespace bmcl {
namespace core {

class Reader {
public:
    virtual ~Reader() {}

    virtual void read(void* dest, std::size_t size) = 0;

    template <typename T>
    inline T readType()
    {
        T value;
        read(&value, sizeof(value));
        return value;
    }

    uint8_t readUint8() { return readType<uint8_t>(); }
    uint16_t readUint16() { return readType<uint16_t>(); }
    uint32_t readUint32() { return readType<uint32_t>(); }
    uint64_t readUint64() { return readType<uint64_t>(); }
    uint16_t readUint16Le() { return le16toh(readType<uint16_t>()); }
    uint32_t readUint32Le() { return le32toh(readType<uint32_t>()); }
    uint64_t readUint64Le() { return le64toh(readType<uint64_t>()); }
    uint16_t readUint16Be() { return be16toh(readType<uint16_t>()); }
    uint32_t readUint32Be() { return be32toh(readType<uint32_t>()); }
    uint64_t readUint64Be() { return be64toh(readType<uint64_t>()); }

    template <typename T, typename H, typename C>
    inline T readFloat(C convert)
    {
        assert(std::numeric_limits<T>::is_iec559);
        H value = readType<H>();
        T swapped;
        convert(&swapped, value);
        return swapped;
    }

    float readFloat32Le() { return readFloat<float, uint32_t>(le32enc); }
    double readFloat64Le() { return readFloat<double, uint64_t>(le64enc); }
    float readFloat32Be() { return readFloat<float, uint32_t>(be32enc); }
    double readFloat64Be() { return readFloat<double, uint64_t>(be64enc); }
};
}
}
