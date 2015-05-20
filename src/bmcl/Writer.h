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

class Writer {
public:
    virtual ~Writer() {}

    virtual std::size_t writableSize() const = 0;

    template <std::size_t n, typename R>
    inline void write(R(&array)[n])
    {
        write(array, n * sizeof(R));
    }

    virtual void write(const void* data, std::size_t size) = 0;

    template <typename T>
    inline void writeType(T value)
    {
        write(&value, sizeof(value));
    }

    void writeUint8(uint8_t value) { writeType<uint8_t>(value); }
    void writeUint16(uint16_t value) { writeType<uint16_t>(value); }
    void writeUint32(uint32_t value) { writeType<uint32_t>(value); }
    void writeUint64(uint64_t value) { writeType<uint64_t>(value); }
    void writeUint16Le(uint16_t value) { writeType<uint16_t>(htole16(value)); }
    void writeUint32Le(uint32_t value) { writeType<uint32_t>(htole32(value)); }
    void writeUint64Le(uint64_t value) { writeType<uint64_t>(htole64(value)); }
    void writeUint16Be(uint16_t value) { writeType<uint16_t>(htobe16(value)); }
    void writeUint32Be(uint32_t value) { writeType<uint32_t>(htobe32(value)); }
    void writeUint64Be(uint64_t value) { writeType<uint64_t>(htobe64(value)); }

    template <typename T, typename H, typename C>
    inline void writeFloat(T value, C convert)
    {
        BMCL_ASSERT(std::numeric_limits<T>::is_iec559);
        H swapped = convert(&value);
        write(&swapped, sizeof(H));
    }

    void writeFloat32Le(float value) { writeFloat<float, uint32_t>(value, le32dec); }
    void writeFloat64Le(double value) { writeFloat<double, uint64_t>(value, le64dec); }
    void writeFloat32Be(float value) { writeFloat<float, uint32_t>(value, be32dec); }
    void writeFloat64Be(double value) { writeFloat<double, uint64_t>(value, be64dec); }
};
}
