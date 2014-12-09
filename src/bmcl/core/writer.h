/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/core/endian.h"
#include "bmcl/core/status.h"

#include <cstddef>
#include <stdint.h>
#include <cstdlib>

namespace bmcl {
namespace core {

class Writer {
public:
    virtual ~Writer() {}

    virtual std::size_t availableSize() const = 0;

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
};
}
}
