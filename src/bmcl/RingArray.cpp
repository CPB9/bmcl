/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/Config.h"
#include "bmcl/Assert.h"
#include "bmcl/RingArray.h"

namespace bmcl {

RingArray::RingArray(void* ptr, std::size_t bufSize, std::size_t elementSize)
{
    init(ptr, bufSize, elementSize);
#if BMCL_HAVE_MALLOC
    _hasAllocatedMem = false;
#endif
}

#if BMCL_HAVE_MALLOC

RingArray::RingArray(std::size_t numElements, std::size_t elementSize)
{
    std::size_t bufSize = numElements * elementSize;
    uint8_t* ptr = new uint8_t[bufSize];
    init(ptr, bufSize, elementSize);
    _hasAllocatedMem = true;
}

RingArray::~RingArray()
{
    if (_hasAllocatedMem) {
        delete[] _data;
    }
}

#endif

void RingArray::copyFirst(void* dest) const
{
    BMCL_ASSERT(!isEmpty());
    std::memcpy(dest, readPtr(), _elementSize);
}

void RingArray::eraseFirstElement()
{
    BMCL_ASSERT(!isEmpty());
    incReadPtr();
    _count--;
}

void RingArray::init(void* ptr, std::size_t bufSize, std::size_t elementSize)
{
    BMCL_ASSERT(ptr != 0);
    BMCL_ASSERT(bufSize != 0);
    BMCL_ASSERT(elementSize != 0);
    BMCL_ASSERT(elementSize <= bufSize);
    _data = (uint8_t*)ptr;
    _elementSize = elementSize;
    _size = bufSize / elementSize;
    _readOffset = 0;
    _writeOffset = 0;
    _count = 0;
}

void RingArray::append(const void* element)
{
    if (isFull()) {
        eraseFirstElement();
    }
    std::memcpy(writePtr(), element, _elementSize);
    incWritePtr();
    _count++;
}

void RingArray::incReadPtr()
{
    _readOffset++;
    if (_readOffset >= _size) {
        _readOffset -= _size;
    }
}

void RingArray::incWritePtr()
{
    _writeOffset++;
    if (_writeOffset >= _size) {
        _writeOffset -= _size;
    }
}
}
