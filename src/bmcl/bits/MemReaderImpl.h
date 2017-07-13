/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/bits/MemReaderDecl.h"
#include "bmcl/bits/ArrayViewImpl.h"

namespace bmcl {

template <std::size_t n, typename R>
inline MemReader::MemReader(const R(&array)[n])
{
    init(array, sizeof(R) * n);
}

inline MemReader::MemReader(const void* ptr, std::size_t size)
{
    init(ptr, size);
}

inline MemReader::MemReader(Bytes data)
    : MemReader(data.data(), data.size())
{
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

inline std::size_t MemReader::readableSize() const
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

inline void MemReader::read(void* dest, std::size_t size)
{
    std::memcpy(dest, _current, size);
    _current += size;
}
}
