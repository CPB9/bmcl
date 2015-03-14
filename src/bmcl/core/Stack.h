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

namespace bmcl {
namespace core {

class Stack {
public:
    virtual ~Stack() {}

    virtual void pop(void* dest, std::size_t size) = 0;
    virtual void push(const void* src, std::size_t size) = 0;

    template <typename T>
    inline T popType()
    {
        T value;
        pop(&value, sizeof(value));
        return value;
    }

    template <typename T>
    inline void pushType(T value)
    {
        push(&value, sizeof(value));
    }

    void pushUint8(uint8_t value) { pushType<uint8_t>(value); }
    void pushUint16(uint16_t value) { pushType<uint16_t>(value); }
    void pushUint32(uint32_t value) { pushType<uint32_t>(value); }
    void pushUint64(uint64_t value) { pushType<uint64_t>(value); }
    void pushUint16Le(uint16_t value) { pushType<uint16_t>(htole16(value)); }
    void pushUint32Le(uint32_t value) { pushType<uint32_t>(htole32(value)); }
    void pushUint64Le(uint64_t value) { pushType<uint64_t>(htole64(value)); }
    void pushUint16Be(uint16_t value) { pushType<uint16_t>(htobe16(value)); }
    void pushUint32Be(uint32_t value) { pushType<uint32_t>(htobe32(value)); }
    void pushUint64Be(uint64_t value) { pushType<uint64_t>(htobe64(value)); }

    uint8_t popUint8() { return popType<uint8_t>(); }
    uint16_t popUint16() { return popType<uint16_t>(); }
    uint32_t popUint32() { return popType<uint32_t>(); }
    uint64_t popUint64() { return popType<uint64_t>(); }
    uint16_t popUint16Le() { return le16toh(popType<uint16_t>()); }
    uint32_t popUint32Le() { return le32toh(popType<uint32_t>()); }
    uint64_t popUint64Le() { return le64toh(popType<uint64_t>()); }
    uint16_t popUint16Be() { return be16toh(popType<uint16_t>()); }
    uint32_t popUint32Be() { return be32toh(popType<uint32_t>()); }
    uint64_t popUint64Be() { return be64toh(popType<uint64_t>()); }
};
}
}
