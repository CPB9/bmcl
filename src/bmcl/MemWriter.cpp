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

namespace bmcl {

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
    BMCL_ASSERT(writableSize() >= size);
    _current += size;
}

void MemWriter::writeImpl(const void* data, std::size_t size)
{
    BMCL_ASSERT(writableSize() >= size);
    std::memcpy(_current, data, size);
    _current += size;
}

std::size_t MemWriter::writableSizeImpl() const
{
    return _end - _current;
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
