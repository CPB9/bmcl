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
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace bmcl {
namespace core {

class RingBuf: public Reader, public Writer {
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
        _write_offset = 0;
        _read_offset = 0;
        _free_space = _size;
    }

    std::size_t freeSpace() const
    {
        return _free_space;
    }

    std::size_t usedSpace() const
    {
        return _size - _free_space;
    }

    bool isFull() const
    {
        return _free_space == 0;
    }

    bool isEmpty() const
    {
        return _free_space == _size;
    }

    void erase(std::size_t size)
    {
        assert(_size - _free_space >= size);
        _free_space += size;
        _read_offset += size;
        if (_read_offset >= _size) {
            _read_offset -= _size;
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
        _free_space = size;
        _read_offset = 0;
        _write_offset = 0;
    }

    void extend(std::size_t size)
    {
        _write_offset += size;
        if (_write_offset >= _size) {
            _write_offset -= _size;
        }
        _free_space -= size;
    }

    std::size_t _read_offset;
    std::size_t _write_offset;
    std::size_t _size;
    std::size_t _free_space;
    uint8_t* _data;
#if BMCL_HAVE_MALLOC
    bool hasAllocatedMem;
#endif
};
}
}
