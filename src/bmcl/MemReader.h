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

class MemReader : public Reader<MemReader> {
public:
    template <std::size_t n, typename R>
    MemReader(const R(&array)[n]);

    MemReader(const void* ptr, std::size_t size);
    MemReader(const MemWriter& memWriter);

    bool isEmpty() const;

    const uint8_t* current() const;
    const uint8_t* start() const;
    const uint8_t* end() const;

    std::size_t size() const;
    std::size_t sizeLeft() const;
    std::size_t sizeRead() const;

    void reset();
    void peek(void* dest, std::size_t size, std::size_t offset) const;

    std::size_t readableSizeImpl() const;
    void readImpl(void* dest, std::size_t size);
    void skipImpl(std::size_t size);

    uint8_t readUint8();
    int8_t readInt8();

private:
    void init(const void* ptr, std::size_t size);

    const uint8_t* _start;
    const uint8_t* _current;
    const uint8_t* _end;
};

template <std::size_t n, typename R>
inline MemReader::MemReader(const R(&array)[n])
{
    init(array, sizeof(R) * n);
}

inline MemReader::MemReader(const void* ptr, std::size_t size)
{
    init(ptr, size);
}

inline bool MemReader::isEmpty() const
{
    return _current >= _end;
}

inline const uint8_t* MemReader::current() const
{
    return _current;
}

inline const uint8_t* MemReader::start() const
{
    return _start;
}

inline const uint8_t* MemReader::end() const
{
    return _end;
}

inline std::size_t MemReader::size() const
{
    return _end - _start;
}

inline std::size_t MemReader::sizeLeft() const
{
    return _end - _current;
}

inline std::size_t MemReader::sizeRead() const
{
    return _current - _start;
}

inline void MemReader::reset()
{
    _current = _start;
}

inline std::size_t MemReader::readableSizeImpl() const
{
    return sizeLeft();
}

inline uint8_t MemReader::readUint8()
{
    BMCL_ASSERT(sizeLeft() > 0);
    uint8_t value = *_current;
    _current++;
    return value;
}

inline int8_t MemReader::readInt8()
{
    return readUint8();
}
}
