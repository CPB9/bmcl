/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/config.h"
#include "bmcl/core/MemWriter.h"

namespace bmcl {
namespace core {

MemWriter::MemWriter(void* dest, std::size_t maxSize)
{
    init(dest, maxSize);
}

#if BMCL_HAVE_MALLOC

MemWriter::MemWriter(std::size_t maxSize)
{
    uint8_t* dest = new uint8_t[maxSize];
    init(dest, maxSize, true);
}

MemWriter::~MemWriter()
{
    if (_hasAllocatedMem) {
        delete[] _start;
    }
}

#endif

void MemWriter::advance(std::size_t size)
{
    assert(writableSize() >= size);
    _current += size;
}

void MemWriter::write(const void* data, std::size_t size)
{
    assert(writableSize() >= size);
    std::memcpy(_current, data, size);
    _current += size;
}

std::size_t MemWriter::writableSize() const { return _end - _current; }

void MemWriter::push(const void* src, std::size_t size) { write(src, size); }

void MemWriter::pop(void* dest, std::size_t size)
{
    assert(sizeUsed() >= size);
    _current -= size;
    std::memcpy(dest, _current, size);
}

void MemWriter::fill(uint8_t byte, std::size_t size)
{
    assert(writableSize() >= size);
    std::memset(_current, byte, size);
    _current += size;
}

void MemWriter::fillUp(uint8_t byte)
{
    std::size_t size = _end - _current;
    std::memset(_current, byte, size);
    _current += size;
}

#if BMCL_HAVE_MALLOC
void MemWriter::init(void* dest, std::size_t maxSize, bool hasAllocatedMem)
#else
void MemWriter::init(void* dest, std::size_t maxSize)
#endif
{
    _start = (uint8_t*)dest;
    _current = _start;
    _end = _start + maxSize;
#if BMCL_HAVE_MALLOC
    _hasAllocatedMem = hasAllocatedMem;
#endif
}
}
}
