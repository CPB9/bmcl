/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/Config.h"
#include "bmcl/Assert.h"
#include "bmcl/MemWriter.h"
#include "bmcl/ZigZag.h"

namespace bmcl {

MemWriter::MemWriter(void* dest, std::size_t maxSize)
{
    init(dest, maxSize);
}

void MemWriter::advance(std::size_t size)
{
    BMCL_ASSERT(writableSize() >= size);
    _current += size;
}

void MemWriter::fill(uint8_t byte, std::size_t size)
{
    BMCL_ASSERT(writableSize() >= size);
    std::memset(_current, byte, size);
    _current += size;
}

void MemWriter::fillUp(uint8_t byte)
{
    std::size_t size = _end - _current;
    std::memset(_current, byte, size);
    _current += size;
}

void MemWriter::init(void* dest, std::size_t maxSize)
{
    _start = (uint8_t*)dest;
    _current = _start;
    _end = _start + maxSize;
}

//sqlite varuint

#define RETURN_IF_SIZE_LESS(size)   \
    if (sizeLeft() < size) {        \
        return false;               \
    }

bool MemWriter::writeVarUint(uint64_t value)
{
    if (value <= 240) {
        RETURN_IF_SIZE_LESS(1);
        _current[0] = uint8_t(value);
        _current += 1;
        return true;
    }
    if (value <= 2287) {
        RETURN_IF_SIZE_LESS(2);
        _current[0] = uint8_t((value - 240) / 256 + 241);
        _current[1] = uint8_t((value - 240) % 256);
        _current += 2;
        return true;
    }
    if (value <= 67823) {
        RETURN_IF_SIZE_LESS(3);
        _current[0] = 249;
        _current[1] = uint8_t((value - 2288) / 256);
        _current[2] = uint8_t((value - 2288) % 256);
        _current += 3;
        return true;
    }
    if (value <= 16777215) {
        RETURN_IF_SIZE_LESS(4);
        _current[0] = 250;
        _current[1] = uint8_t(value >> 16);
        _current[2] = uint8_t(value >> 8);
        _current[3] = uint8_t(value);
        _current += 4;
        return true;
    }
    if (value <= 4294967295) {
        RETURN_IF_SIZE_LESS(5);
        _current[0] = 251;
        _current[1] = uint8_t(value >> 24);
        _current[2] = uint8_t(value >> 16);
        _current[3] = uint8_t(value >> 8);
        _current[4] = uint8_t(value);
        _current += 5;
        return true;
    }
    if (value <= 1099511627775) {
        RETURN_IF_SIZE_LESS(6);
        _current[0] = 252;
        _current[1] = uint8_t(value >> 32);
        _current[2] = uint8_t(value >> 24);
        _current[3] = uint8_t(value >> 16);
        _current[4] = uint8_t(value >> 8);
        _current[5] = uint8_t(value);
        _current += 6;
        return true;
    }
    if (value <= 281474976710655) {
        RETURN_IF_SIZE_LESS(7);
        _current[0] = 253;
        _current[1] = uint8_t(value >> 40);
        _current[2] = uint8_t(value >> 32);
        _current[3] = uint8_t(value >> 24);
        _current[4] = uint8_t(value >> 16);
        _current[5] = uint8_t(value >> 8);
        _current[6] = uint8_t(value);
        _current += 7;
        return true;
    }
    if (value <= 72057594037927935) {
        RETURN_IF_SIZE_LESS(8);
        _current[0] = 254;
        _current[1] = uint8_t(value >> 48);
        _current[2] = uint8_t(value >> 40);
        _current[3] = uint8_t(value >> 32);
        _current[4] = uint8_t(value >> 24);
        _current[5] = uint8_t(value >> 16);
        _current[6] = uint8_t(value >> 8);
        _current[7] = uint8_t(value);
        _current += 8;
        return true;
    }
    RETURN_IF_SIZE_LESS(9);
    _current[0] = 255;
    _current[1] = uint8_t(value >> 56);
    _current[2] = uint8_t(value >> 48);
    _current[3] = uint8_t(value >> 40);
    _current[4] = uint8_t(value >> 32);
    _current[5] = uint8_t(value >> 24);
    _current[6] = uint8_t(value >> 16);
    _current[7] = uint8_t(value >> 8);
    _current[8] = uint8_t(value);
    _current += 9;
    return true;
}

bool MemWriter::writeVarInt(int64_t n)
{
    return writeVarUint(zigZagEncode(n));
}
}
