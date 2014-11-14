/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cassert>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace bmcl {
namespace core {

class RingArray {
public:
    RingArray(void* ptr, std::size_t bufSize, std::size_t elementSize)
    {
        init(ptr, bufSize, elementSize);
#if BMCL_HAVE_MALLOC
        hasAllocatedMem = false;
#endif
    }

#if BMCL_HAVE_MALLOC

    RingArray(std::size_t numElements, std::size_t elementSize)
    {
        std::size_t bufSize = numElements * elementSize;
        uint8_t* ptr = new uint8_t[bufSize];
        init(ptr, bufSize, elementSize);
        hasAllocatedMem = true;
    }

    ~RingArray()
    {
        if (hasAllocatedMem) {
            delete[] _data;
        }
    }

#endif

    void reset()
    {
        init(_data, _size, _elementSize);
    }

    void append(const void* element);

    std::size_t count() const
    {
        return _count;
    }

    std::size_t size() const
    {
        return _size;
    }

    bool isEmpty() const
    {
        return _count == 0;
    }

    bool isFull() const
    {
        return !isEmpty() && (_readOffset == _writeOffset);
    }

    std::size_t elementSize() const
    {
        return _elementSize;
    }

    void copyFirst(void* dest) const
    {
        assert(!isEmpty());
        std::memcpy(dest, readPtr(), _elementSize);
    }

    void removeFirst()
    {
        assert(!isEmpty());
        eraseFirstElement();
    }

private:
    void init(void* ptr, std::size_t bufSize, std::size_t elementSize);

    void eraseFirstElement()
    {
        incReadPtr();
        _count--;
    }

    void* writePtr() const
    {
        return _data + _writeOffset * _elementSize;
    }

    void* readPtr() const
    {
        return _data + _readOffset * _elementSize;
    }

    void incReadPtr();

    void incWritePtr();

    uint8_t* _data;
    std::size_t _elementSize;
    std::size_t _size;
    std::size_t _readOffset;
    std::size_t _writeOffset;
    std::size_t _count;
#if BMCL_HAVE_MALLOC
    bool hasAllocatedMem;
#endif
};
}
}
