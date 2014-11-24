/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/core/reader.h"
#include "bmcl/core/writer.h"

#include <cassert>
#include <cstddef>
#include <stdint.h>
#include <cstring>

namespace bmcl {
namespace core {

class RingBuf : public bmcl::core::Reader, public bmcl::core::Writer {
public:
    RingBuf(void* data, std::size_t size)
    {
        init(data, size);
#if BMCL_HAVE_MALLOC
        hasAllocatedMem = false;
#endif
    }

#if BMCL_HAVE_MALLOC

    RingBuf(std::size_t size)
    {
        uint8_t* data = new uint8_t[size];
        init(data, size);
        hasAllocatedMem = true;
    }

    ~RingBuf()
    {
        if (hasAllocatedMem) {
            delete[] _data;
        }
    }

#endif

    void clear()
    {
        _writeOffset = 0;
        _readOffset = 0;
        _freeSpace = _size;
    }

    std::size_t freeSpace() const
    {
        return _freeSpace;
    }

    std::size_t usedSpace() const
    {
        return _size - _freeSpace;
    }

    virtual std::size_t sizeLeft() const
    {
        return freeSpace();
    }

    bool isFull() const
    {
        return _freeSpace == 0;
    }

    bool isEmpty() const
    {
        return _freeSpace == _size;
    }

    void erase(std::size_t size)
    {
        assert(_size - _freeSpace >= size);
        _freeSpace += size;
        _readOffset += size;
        if (_readOffset >= _size) {
            _readOffset -= _size;
        }
    }

    virtual void write(const void* data, std::size_t size);

    void peek(void* dest, std::size_t size, std::size_t offset = 0) const;

    virtual void read(void* dest, std::size_t size)
    {
        peek(dest, size, 0);
        erase(size);
    }

    std::size_t size() const
    {
        return _size;
    }

private:
    void init(void* data, std::size_t size)
    {
        assert(size > 0);
        _data = (uint8_t*)data;
        _size = size;
        _freeSpace = size;
        _readOffset = 0;
        _writeOffset = 0;
    }

    void extend(std::size_t size)
    {
        _writeOffset += size;
        if (_writeOffset >= _size) {
            _writeOffset -= _size;
        }
        _freeSpace -= size;
    }

    std::size_t _readOffset;
    std::size_t _writeOffset;
    std::size_t _size;
    std::size_t _freeSpace;
    uint8_t* _data;
#if BMCL_HAVE_MALLOC
    bool hasAllocatedMem;
#endif
};
}
}
