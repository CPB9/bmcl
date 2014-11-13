/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/core/ringarray.h"

namespace bmcl {
namespace core {

void RingArray::init(void* ptr, std::size_t bufSize, std::size_t elementSize)
{
    assert(ptr != 0);
    assert(bufSize != 0);
    assert(elementSize != 0);
    assert(elementSize <= bufSize);
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
    intWritePtr();
    _count++;
}

void RingArray::incReadPtr()
{
    _readOffset++;
    if (_readOffset >= _size) {
        _readOffset -= _size;
    }
}

void RingArray::intWritePtr()
{
    _writeOffset++;
    if (_writeOffset >= _size) {
        _writeOffset -= _size;
    }
}
}
}
