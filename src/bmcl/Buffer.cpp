/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/Buffer.h"
#include "bmcl/MemWriter.h"
#include "bmcl/ZigZag.h"

#include <cstdlib>
#include <cstring>
#include <utility>

#define GROWTH_FACTOR 2

namespace bmcl {

Buffer::Buffer()
    : _ptr(0)
    , _size(0)
    , _capacity(0)
{
}

Buffer::Buffer(std::size_t size)
    : _ptr((uint8_t*)std::malloc(size))
    , _size(0)
    , _capacity(size)
{
}

Buffer::Buffer(const void* data, std::size_t size)
    : _ptr((uint8_t*)std::malloc(size))
    , _size(size)
    , _capacity(size)
{
    std::memcpy(_ptr, data, size);
}

Buffer::Buffer(bmcl::Bytes data)
    : _ptr((uint8_t*)std::malloc(data.size()))
    , _size(data.size())
    , _capacity(data.size())
{
    std::memcpy(_ptr, data.data(), data.size());
}

Buffer::Buffer(void* ptr, std::size_t size, std::size_t capacity)
    : _ptr((uint8_t*)ptr)
    , _size(size)
    , _capacity(capacity)
{
}

Buffer Buffer::createWithUnitializedData(std::size_t size)
{
    return Buffer(std::malloc(size), size, size);
}

MemWriter Buffer::dataWriter()
{
    return MemWriter(_ptr, _size);
}

void Buffer::copyFrom(const Buffer& other)
{
    if (other._ptr) {
        _size = other._size;
        _capacity = other._capacity;
        _ptr = (uint8_t*)std::malloc(_capacity);
        BMCL_ASSERT(_ptr);
        std::memcpy(_ptr, other._ptr, _size);
    } else {
        _ptr = 0;
        _size = 0;
        _capacity = 0;
    }
}

Buffer::Buffer(const Buffer& other)
{
    copyFrom(other);
}

Buffer& Buffer::operator=(const Buffer& other)
{
    dealloc();
    copyFrom(other);
    return *this;
}

void Buffer::moveFrom(Buffer&& other)
{
    _ptr = other._ptr;
    _size = other._size;
    _capacity = other._capacity;
    other._ptr = 0;
    other._size = 0;
    other._capacity = 0;
}

Buffer::Buffer(Buffer&& other)
{
    moveFrom(std::move(other));
}

Buffer& Buffer::operator=(Buffer&& other)
{
    dealloc();
    moveFrom(std::move(other));
    return *this;
}

void Buffer::dealloc()
{
    if (_ptr) {
        std::free(_ptr);
    }
}

Buffer::~Buffer()
{
    dealloc();
}

void Buffer::realloc(std::size_t capacity)
{
    if (_ptr) {
        if (capacity == 0) {
            std::free(_ptr);
            _ptr = 0;
        } else {
            _ptr = (uint8_t*)std::realloc(_ptr, capacity);
            BMCL_ASSERT(_ptr);
        }
    } else {
        _ptr = (uint8_t*)std::malloc(capacity);
    }
    _capacity = capacity;
}

void Buffer::reserve(std::size_t capacity)
{
    if (capacity > _capacity) {
        realloc(capacity);
    }
}

void Buffer::removeFront(std::size_t size)
{
    BMCL_ASSERT(size <= _size);
    std::size_t newSize = _size - size;
    std::memmove(_ptr, _ptr + size, newSize);
    _size = newSize;
}

void Buffer::resize(std::size_t size)
{
    if (size > _capacity) {
        realloc(size);
    }
    _size = size;
}

void Buffer::extend(std::size_t additionalSize)
{
    resize(_size + additionalSize);
}

void Buffer::resize(std::size_t size, uint8_t filler)
{
    if (size > _capacity) {
        realloc(size);
        std::memset(_ptr + _size, size - _size, filler);
    }
    _size = size;
}

void Buffer::shrink()
{
    realloc(_size);
}

void Buffer::write(const void* data, std::size_t size)
{
    if ((_capacity - _size) < size) {
        realloc(BMCL_MAX(_capacity + size, _size * GROWTH_FACTOR));
    }
    std::memcpy(_ptr + _size, data, size);
    _size += size;
}

void Buffer::writeVarUint(uint64_t value)
{
    std::size_t lastSize = _size;
    if (value <= 240) {
        extend(1);
        uint8_t* current = _ptr + lastSize;
        current[0] = uint8_t(value);
        return;
    }
    if (value <= 2287) {
        extend(2);
        uint8_t* current = _ptr + lastSize;
        current[0] = uint8_t((value - 240) / 256 + 241);
        current[1] = uint8_t((value - 240) % 256);
        return;
    }
    if (value <= 67823) {
        extend(3);
        uint8_t* current = _ptr + lastSize;
        current[0] = 249;
        current[1] = uint8_t((value - 2288) / 256);
        current[2] = uint8_t((value - 2288) % 256);
        return;
    }
    if (value <= 16777215) {
        extend(4);
        uint8_t* current = _ptr + lastSize;
        current[0] = 250;
        current[1] = uint8_t(value >> 16);
        current[2] = uint8_t(value >> 8);
        current[3] = uint8_t(value);
        return;
    }
    if (value <= 4294967295) {
        extend(5);
        uint8_t* current = _ptr + lastSize;
        current[0] = 251;
        current[1] = uint8_t(value >> 24);
        current[2] = uint8_t(value >> 16);
        current[3] = uint8_t(value >> 8);
        current[4] = uint8_t(value);
        return;
    }
    if (value <= 1099511627775) {
        extend(6);
        uint8_t* current = _ptr + lastSize;
        current[0] = 252;
        current[1] = uint8_t(value >> 32);
        current[2] = uint8_t(value >> 24);
        current[3] = uint8_t(value >> 16);
        current[4] = uint8_t(value >> 8);
        current[5] = uint8_t(value);
        return;
    }
    if (value <= 281474976710655) {
        extend(7);
        uint8_t* current = _ptr + lastSize;
        current[0] = 253;
        current[1] = uint8_t(value >> 40);
        current[2] = uint8_t(value >> 32);
        current[3] = uint8_t(value >> 24);
        current[4] = uint8_t(value >> 16);
        current[5] = uint8_t(value >> 8);
        current[6] = uint8_t(value);
        return;
    }
    if (value <= 72057594037927935) {
        extend(8);
        uint8_t* current = _ptr + lastSize;
        current[0] = 254;
        current[1] = uint8_t(value >> 48);
        current[2] = uint8_t(value >> 40);
        current[3] = uint8_t(value >> 32);
        current[4] = uint8_t(value >> 24);
        current[5] = uint8_t(value >> 16);
        current[6] = uint8_t(value >> 8);
        current[7] = uint8_t(value);
        return;
    }
    extend(9);
    uint8_t* current = _ptr + lastSize;
    current[0] = 255;
    current[1] = uint8_t(value >> 56);
    current[2] = uint8_t(value >> 48);
    current[3] = uint8_t(value >> 40);
    current[4] = uint8_t(value >> 32);
    current[5] = uint8_t(value >> 24);
    current[6] = uint8_t(value >> 16);
    current[7] = uint8_t(value >> 8);
    current[8] = uint8_t(value);
}

void Buffer::writeVarInt(int64_t value)
{
    return writeVarUint(zigZagEncode(value));
}
}
