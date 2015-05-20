/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/Reader.h"

#include <cstddef>
#include <stdint.h>

namespace bmcl {

class MemWriter;

class MemReader : public Reader {
public:
    template <std::size_t n, typename R>
    MemReader(const R (&array)[n]);

    MemReader(const void* ptr, std::size_t size);
    MemReader(const MemWriter& memWriter);

    bool isEmpty() const { return _current >= _end; }

    const uint8_t* current() const { return _current; }
    const uint8_t* start() const { return _start; }
    const uint8_t* end() const { return _end; }

    std::size_t size() const { return _end - _start; }
    std::size_t sizeLeft() const { return _end - _current; }
    std::size_t sizeRead() const { return _current - _start; }

    void reset() { _current = _start; }
    virtual std::size_t readableSize() const;
    virtual void skip(std::size_t size);
    virtual void peek(void* dest, std::size_t size, std::size_t offset) const;
    virtual void read(void* dest, std::size_t size);

private:
    void init(const void* ptr, std::size_t size);

    const uint8_t* _start;
    const uint8_t* _current;
    const uint8_t* _end;
};

template <std::size_t n, typename R>
inline MemReader::MemReader(const R (&array)[n])
{
    init(array, sizeof(R) * n);
}

inline MemReader::MemReader(const void* ptr, std::size_t size)
{
    init(ptr, size);
}
}
