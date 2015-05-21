/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"

#include <cstddef>
#include <stdint.h>
#include <cstring>

namespace bmcl {

class RingArray {
public:
    RingArray(void* ptr, std::size_t bufSize, std::size_t elementSize);

#if BMCL_HAVE_MALLOC

    RingArray(std::size_t numElements, std::size_t elementSize);
    ~RingArray();

#endif

    bool isEmpty() const;
    bool isFull() const;

    std::size_t count() const;
    std::size_t size() const;
    std::size_t elementSize() const;

    void reset();
    void append(const void* element);
    void copyFirst(void* dest) const;
    void removeFirst();

private:
    void* writePtr() const;
    void* readPtr() const;

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

inline bool RingArray::isEmpty() const
{
    return _count == 0;
}

inline bool RingArray::isFull() const
{
    return !isEmpty() && (_readOffset == _writeOffset);
}

inline std::size_t RingArray::count() const
{
    return _count;
}

inline std::size_t RingArray::size() const
{
    return _size;
}

inline std::size_t RingArray::elementSize() const
{
    return _elementSize;
}

inline void RingArray::reset()
{
    init(_data, _size, _elementSize);
}

inline void RingArray::removeFirst()
{
    eraseFirstElement();
}

inline void* RingArray::writePtr() const
{
    return _data + _writeOffset * _elementSize;
}

inline void* RingArray::readPtr() const
{
    return _data + _readOffset * _elementSize;
}
}
