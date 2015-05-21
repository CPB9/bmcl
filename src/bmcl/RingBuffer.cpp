/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/Config.h"
#include "bmcl/Assert.h"
#include "bmcl/Endian.h"
#include "bmcl/RingBuffer.h"

#include <stdint.h>
#include <cstring>

namespace bmcl {

RingBuffer::RingBuffer(void* data, std::size_t size)
{
    init(data, size);
#if BMCL_HAVE_MALLOC
    _hasAllocatedMem = false;
#endif
}

#if BMCL_HAVE_MALLOC

RingBuffer::RingBuffer(std::size_t size)
{
    uint8_t* data = new uint8_t[size];
    init(data, size);
    _hasAllocatedMem = true;
}

RingBuffer::~RingBuffer()
{
    if (_hasAllocatedMem) {
        delete[] _data;
    }
}

#endif

void RingBuffer::clear()
{
    _writeOffset = 0;
    _readOffset = 0;
    _freeSpace = _size;
}

void RingBuffer::erase(std::size_t size)
{
    BMCL_ASSERT(_size - _freeSpace >= size);
    _freeSpace += size;
    _readOffset += size;
    if (_readOffset >= _size) {
        _readOffset -= _size;
    }
}

void RingBuffer::readImpl(void* dest, std::size_t size)
{
    peek(dest, size, 0);
    erase(size);
}

void RingBuffer::init(void* data, std::size_t size)
{
    BMCL_ASSERT(size > 0);
    _data = (uint8_t*)data;
    _size = size;
    _freeSpace = size;
    _readOffset = 0;
    _writeOffset = 0;
}

void RingBuffer::extend(std::size_t size)
{
    _writeOffset += size;
    if (_writeOffset >= _size) {
        _writeOffset -= _size;
    }
    _freeSpace -= size;
}

void RingBuffer::writeImpl(const void* data, std::size_t size)
{
    BMCL_ASSERT(size > 0);
    BMCL_ASSERT(size <= _size);
    if (_freeSpace < size) {
        erase(size - _freeSpace);
    }
    if (_readOffset > _writeOffset) { /* *********w---------------r************ */
        std::memcpy(_data + _writeOffset, data, size);
    } else { /* ----------r**************w---------- */
        std::size_t rightData = _size - _writeOffset;
        std::size_t firstChunkSize = BMCL_MIN(size, rightData);
        std::memcpy(_data + _writeOffset, data, firstChunkSize);
        if (size > firstChunkSize) {
            std::size_t secondChunkSize = size - firstChunkSize;
            std::memcpy(_data, (const uint8_t*)data + firstChunkSize, secondChunkSize);
        }
    }
    extend(size);
}

void RingBuffer::peek(void* dest, std::size_t size, std::size_t offset) const
{
    std::size_t readOffset = _readOffset + offset;
    if (readOffset >= _size) {
        readOffset -= _size;
    }
    BMCL_ASSERT(size > 0);
    BMCL_ASSERT(size + offset <= _size - _freeSpace);
    if (readOffset < _writeOffset) { /* ----------r**************w---------- */
        std::memcpy(dest, _data + readOffset, size);
    } else { /* *********w---------------r************ */
        std::size_t rightData = _size - readOffset;
        std::size_t firstChunkSize = BMCL_MIN(size, rightData);
        std::memcpy(dest, _data + readOffset, firstChunkSize);
        if (size > firstChunkSize) {
            std::size_t secondChunkSize = size - firstChunkSize;
            std::memcpy((uint8_t*)dest + firstChunkSize, _data, secondChunkSize);
        }
    }
}
}
