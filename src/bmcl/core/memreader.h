/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/core/reader.h"

#include <cassert>
#include <cstddef>
#include <stdint.h>
#include <cstring>

namespace bmcl {
namespace core {

class MemReader : public Reader {
public:
    MemReader(const void* ptr, std::size_t size)
    {
        _start = (uint8_t*)ptr;
        _current = _start;
        _end = _start + size;
    }

    const uint8_t* currentPtr() const
    {
        return _current;
    }

    std::size_t size() const
    {
        return _end - _start;
    }

    std::size_t sizeLeft() const
    {
        return _end - _current;
    }

    std::size_t sizeRead() const
    {
        return _current - _start;
    }

    bool isEmpty() const
    {
        return _current >= _end;
    }

    void skip(std::size_t size)
    {
        assert(sizeLeft() >= size);
        _current += size;
    }

    void peek(void* dest, std::size_t size, std::size_t offset) const
    {
        assert(sizeLeft() >= size + offset);
        std::memcpy(dest, _current + offset, size);
    }

    virtual void read(void* dest, std::size_t size)
    {
        assert(sizeLeft() >= size);
        std::memcpy(dest, _current, size);
        _current += size;
    }

private:
    const uint8_t* _start;
    const uint8_t* _current;
    const uint8_t* _end;
};
}
}
