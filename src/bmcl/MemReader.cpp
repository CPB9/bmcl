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

void MemReader::skipImpl(std::size_t size)
{
    BMCL_ASSERT(sizeLeft() >= size);
    _current += size;
}

void MemReader::peek(void* dest, std::size_t size, std::size_t offset) const
{
    BMCL_ASSERT(sizeLeft() >= (size + offset));
    std::memcpy(dest, _current + offset, size);
}

void MemReader::readImpl(void* dest, std::size_t size)
{
    BMCL_ASSERT(sizeLeft() >= size);
    std::memcpy(dest, _current, size);
    _current += size;
}
}
