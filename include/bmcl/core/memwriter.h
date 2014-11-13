/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/core/writer.h"

#include <cassert>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace bmcl {
namespace core {

class MemWriter : public Writer {
public:
    MemWriter(void* dest, std::size_t max_size)
    {
        init(dest, max_size);
#if BMCL_HAVE_MALLOC
        hasAllocatedMem = false;
#endif
    }

#if BMCL_HAVE_MALLOC

    MemWriter(std::size_t max_size)
    {
        uint8_t* dest = new uint8_t[max_size];
        init(dest, max_size);
        hasAllocatedMem = true;
    }

    ~MemWriter()
    {
        if (hasAllocatedMem) {
            delete[] _start;
        }
    }

#endif

    uint8_t* ptr() const
    {
        return _start;
    }

    uint8_t* currentPtr() const
    {
        return _current;
    }

    std::size_t sizeUsed() const
    {
        return _current - _start;
    }

    bool isFull() const
    {
        return _current >= _end;
    }

    std::size_t maxSize() const
    {
        return _end - _start;
    }

    std::size_t sizeLeft() const
    {
        return _end - _current;
    }

    void advance(std::size_t size)
    {
        assert(sizeLeft() >= size);
        _current += size;
    }

    virtual void write(const void* data, std::size_t size)
    {
        assert(sizeLeft() >= size);
        std::memcpy(_current, data, size);
        _current += size;
    }

    void pop(void* dest, std::size_t size)
    {
        assert(sizeUsed() >= size);
        _current -= size;
        std::memcpy(dest, _current, size);
    }

    void fill(uint8_t byte, std::size_t size)
    {
        assert(sizeLeft() >= size);
        std::memset(_current, byte, size);
        _current += size;
    }

    void fillUp(uint8_t byte)
    {
        std::size_t size = _end - _current;
        std::memset(_current, byte, size);
        _current += size;
    }

private:
    void init(void* dest, std::size_t max_size)
    {
        _start = (uint8_t*)dest;
        _current = _start;
        _end = _start + max_size;
    }

    uint8_t* _start;
    uint8_t* _current;
    const uint8_t* _end;
#if BMCL_HAVE_MALLOC
    bool hasAllocatedMem;
#endif
};
}
}
