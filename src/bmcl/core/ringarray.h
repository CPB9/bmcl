/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cassert>
#include <cstddef>
#include <stdint.h>
#include <cstring>

namespace bmcl {
namespace core {

class RingArray {
public:
    RingArray(void* ptr, std::size_t bufSize, std::size_t elementSize);

#if BMCL_HAVE_MALLOC

    RingArray(std::size_t numElements, std::size_t elementSize);
    ~RingArray();

#endif

    bool isEmpty() const { return _count == 0; }
    bool isFull() const { return !isEmpty() && (_readOffset == _writeOffset); }

    std::size_t count() const { return _count; }
    std::size_t size() const { return _size; }
    std::size_t elementSize() const { return _elementSize; }

    void reset() { init(_data, _size, _elementSize); }
    void append(const void* element);
    void copyFirst(void* dest) const;
    void removeFirst() { eraseFirstElement(); }

private:
    void* writePtr() const { return _data + _writeOffset * _elementSize; }
    void* readPtr() const { return _data + _readOffset * _elementSize; }

    void init(void* ptr, std::size_t bufSize, std::size_t elementSize);
    void eraseFirstElement();
    void incReadPtr();
    void incWritePtr();

    uint8_t* _data;
    std::size_t _elementSize;
    std::size_t _size;
    std::size_t _readOffset;
    std::size_t _writeOffset;
    std::size_t _count;
#if BMCL_HAVE_MALLOC
    bool _hasAllocatedMem;
#endif
};
}
}
