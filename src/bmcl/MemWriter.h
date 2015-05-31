/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/Writer.h"

#include <cstddef>
#include <stdint.h>
#include <cstring>

namespace bmcl {

class MemWriter : public Writer<MemWriter> {
public:
    template <std::size_t n, typename R>
    MemWriter(R(&array)[n]);

    MemWriter(void* dest, std::size_t maxSize);

    bool isFull() const;
    bool isEmpty() const;

    uint8_t* current() const;
    uint8_t* start() const;
    uint8_t* end() const;

    std::size_t sizeUsed() const;
    std::size_t sizeLeft() const;
    std::size_t maxSize() const;

    void reset();
    void advance(std::size_t size);
    void fillUp(uint8_t byte);
    void fill(uint8_t byte, std::size_t size);

    void writeImpl(const void* data, std::size_t size);
    std::size_t writableSizeImpl() const;

private:
    void init(void* dest, std::size_t maxSize);

    uint8_t* _start;
    uint8_t* _current;
    uint8_t* _end;
};

template <std::size_t n, typename R>
inline MemWriter::MemWriter(R(&array)[n])
{
    init(array, n * sizeof(R));
}

inline bool MemWriter::isFull() const
{
    return _current >= _end;
}

inline bool MemWriter::isEmpty() const
{
    return _current == _start;
}

inline uint8_t* MemWriter::current() const
{
    return _current;
}

inline uint8_t* MemWriter::start() const
{
    return _start;
}

inline uint8_t* MemWriter::end() const
{
    return _end;
}

inline std::size_t MemWriter::sizeUsed() const
{
    return _current - _start;
}

inline std::size_t MemWriter::sizeLeft() const
{
    return _end - _current;
}

inline std::size_t MemWriter::maxSize() const
{
    return _end - _start;
}

inline void MemWriter::reset()
{
    _current = _start;
};

inline std::size_t MemWriter::writableSizeImpl() const
{
    return _end - _current;
}
}
