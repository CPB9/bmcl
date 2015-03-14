/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/config.h"
#include "bmcl/core/MemReader.h"
#include "bmcl/core/MemWriter.h"

#include <cassert>
#include <cstring>

namespace bmcl {
namespace core {

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
    assert(sizeLeft() >= size);
    _current += size;
}

void MemReader::peek(void* dest, std::size_t size, std::size_t offset) const
{
    assert(sizeLeft() >= size + offset);
    std::memcpy(dest, _current + offset, size);
}

void MemReader::read(void* dest, std::size_t size)
{
    assert(sizeLeft() >= size);
    std::memcpy(dest, _current, size);
    _current += size;
}
}
}
