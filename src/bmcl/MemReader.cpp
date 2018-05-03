/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/Config.h"
#include "bmcl/Assert.h"
#include "bmcl/MemReader.h"
#include "bmcl/MemWriter.h"
#include "bmcl/Result.h"
#include "bmcl/ZigZag.h"

#include <cstring>

namespace bmcl {

void MemReader::init(const void* ptr, std::size_t size)
{
     _start = (uint8_t*)ptr;
    _current = _start;
    _end = _start + size;
}

MemReader::MemReader(const MemWriter& memWriter)
{
    init(memWriter.start(), memWriter.sizeUsed());
}

void MemReader::skip(std::size_t size)
{
    BMCL_ASSERT(sizeLeft() >= size);
    _current += size;
}

void MemReader::peek(void* dest, std::size_t size, std::size_t offset) const
{
    BMCL_ASSERT(sizeLeft() >= (size + offset));
    std::memcpy(dest, _current + offset, size);
}


// sqlite varuint

#define RETURN_IF_SIZE_LESS(size)   \
    if (sizeLeft() < size) {        \
        return false;               \
    }

//TODO: check if value is in range

bool MemReader::readVarUint(uint64_t* dest)
{
    RETURN_IF_SIZE_LESS(1);
    uint8_t head = *_current;
    if (head <= 240) {
        *dest = head;
        _current += 1;
        return true;
    }
    if (head <= 248) {
        RETURN_IF_SIZE_LESS(2);
        *dest = 240 + 256 * (uint64_t(head) - 241) + uint64_t(_current[1]);
        _current += 2;
        return true;
    }
    if (head == 249) {
        RETURN_IF_SIZE_LESS(3);
        *dest = 2288 + 256 * uint64_t(_current[1]) + uint64_t(_current[2]);
        _current += 3;
        return true;
    }
    if (head == 250) {
        RETURN_IF_SIZE_LESS(4);
        *dest = uint64_t(_current[1]) << 16 | uint64_t(_current[2]) << 8 | uint64_t(_current[3]);
        _current += 4;
        return true;
    }
    if (head == 251) {
        RETURN_IF_SIZE_LESS(5);
        *dest = uint64_t(_current[1]) << 24 | uint64_t(_current[2]) << 16 | uint64_t(_current[3]) << 8 | uint64_t(_current[4]);
        _current += 5;
        return true;
    }
    if (head == 252) {
        RETURN_IF_SIZE_LESS(6);
        *dest = uint64_t(_current[1]) << 32 | uint64_t(_current[2]) << 24 | uint64_t(_current[3]) << 16 | uint64_t(_current[4]) << 8 | uint64_t(_current[5]);
        _current += 6;
        return true;
    }
    if (head == 253) {
        RETURN_IF_SIZE_LESS(7);
        *dest = uint64_t(_current[1]) << 40 | uint64_t(_current[2]) << 32 | uint64_t(_current[3]) << 24 | uint64_t(_current[4]) << 16 | uint64_t(_current[5]) << 8 | uint64_t(_current[6]);
        _current += 7;
        return true;
    }
    if (head == 254) {
        RETURN_IF_SIZE_LESS(8);
        *dest = uint64_t(_current[1]) << 48 | uint64_t(_current[2]) << 40 | uint64_t(_current[3]) << 32 | uint64_t(_current[4]) << 24 | uint64_t(_current[5]) << 16 | uint64_t(_current[6]) << 8 | uint64_t(_current[7]);
        _current += 8;
        return true;
    }
    RETURN_IF_SIZE_LESS(9);
    *dest = uint64_t(_current[1]) << 56 | uint64_t(_current[2]) << 48 | uint64_t(_current[3]) << 40 | uint64_t(_current[4]) << 32 | uint64_t(_current[5]) << 24 | uint64_t(_current[6]) << 16 | uint64_t(_current[7]) << 8 | uint64_t(_current[8]);
    _current += 9;
    return true;
}

Result<uint64_t, void> MemReader::readVarUint()
{
    uint64_t res;
    if (readVarUint(&res)) {
        return res;
    }
    return Result<uint64_t, void>();
}

bool MemReader::readVarInt(int64_t* dest)
{
    uint64_t n;
    if (!readVarUint(&n)) {
        return false;
    }
    *dest = zigZagDecode(n);
    return true;
}
}
