/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/Stack.h"
#include "bmcl/Writer.h"

#include <cstddef>
#include <stdint.h>
#include <cstring>

namespace bmcl {

class MemWriter : public Writer, public Stack {
public:
    template <std::size_t n, typename R>
    MemWriter(R (&array)[n]);

    MemWriter(void* dest, std::size_t maxSize);

#if BMCL_HAVE_MALLOC

    MemWriter(std::size_t maxSize);
    ~MemWriter();

#endif

    bool isFull() const { return _current >= _end; }
    bool isEmpty() const { return _current == _start; }

    uint8_t* current() const { return _current; }
    uint8_t* start() const { return _start; }
    uint8_t* end() const { return _end; }

    std::size_t sizeUsed() const { return _current - _start; }
    std::size_t maxSize() const { return _end - _start; }
    virtual std::size_t writableSize() const;

    void reset() { _current = _start; };
    void advance(std::size_t size);
    void fillUp(uint8_t byte);
    void fill(uint8_t byte, std::size_t size);
    virtual void write(const void* data, std::size_t size);
    virtual void push(const void* src, std::size_t size);
    virtual void pop(void* dest, std::size_t size);

    template <std::size_t n, typename R>
    void write(R (&array)[n]);

private:
#if BMCL_HAVE_MALLOC
    void init(void* dest, std::size_t maxSize, bool hasAllocatedMem = false);
#else
    void init(void* dest, std::size_t maxSize);
#endif

    uint8_t* _start;
    uint8_t* _current;
    uint8_t* _end;
#if BMCL_HAVE_MALLOC
    bool _hasAllocatedMem;
#endif
};

template <std::size_t n, typename R>
inline MemWriter::MemWriter(R (&array)[n])
{
    init(array, n * sizeof(R));
}

template <std::size_t n, typename R>
inline void MemWriter::write(R (&array)[n])
{
    write(array, sizeof(R) * n);
}
}
