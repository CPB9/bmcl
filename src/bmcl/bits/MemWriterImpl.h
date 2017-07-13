/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/bits/MemWriterDecl.h"
#include "bmcl/bits/ArrayViewImpl.h"

#include <cstring>

namespace bmcl {

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

inline Bytes MemWriter::writenData() const
{
    return Bytes(_start, sizeUsed());
}

inline void MemWriter::reset()
{
    _current = _start;
};

inline std::size_t MemWriter::writableSize() const
{
    return _end - _current;
}

inline void MemWriter::write(Bytes data)
{
    write(data.data(), data.size());
}

inline void MemWriter::write(const void* data, std::size_t size)
{
    BMCL_ASSERT(writableSize() >= size);
    std::memcpy(_current, data, size);
    _current += size;
}

}
