/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/bits/BufferDecl.h"
#include "bmcl/bits/ArrayViewDecl.h"
#include "bmcl/bits/ArrayViewImpl.h"
#include "bmcl/Assert.h"

namespace bmcl {

inline std::size_t Buffer::size() const
{
    return _size;
}

inline std::size_t Buffer::capacity() const
{
    return _capacity;
}

inline uint8_t* Buffer::data()
{
    return _ptr;
}

inline const uint8_t* Buffer::data() const
{
    return _ptr;
}

inline Buffer::iterator Buffer::begin()
{
    return _ptr;
}

inline Buffer::const_iterator Buffer::cbegin() const
{
    return _ptr;
}

inline Buffer::iterator Buffer::end()
{
    return _ptr + _size;
}

inline Buffer::const_iterator Buffer::cend() const
{
    return _ptr + _size;
}

inline Buffer::reverse_iterator Buffer::rbegin()
{
    return reverse_iterator(end());
}

inline Buffer::const_reverse_iterator Buffer::crbegin() const
{
    return const_reverse_iterator(cend());
}

inline Buffer::reverse_iterator Buffer::rend()
{
    return reverse_iterator(begin());
}

inline Buffer::const_reverse_iterator Buffer::crend() const
{
    return const_reverse_iterator(cbegin());
}

inline bool Buffer::isEmpty() const
{
    return _size == 0;
}

inline std::size_t Buffer::writableSize() const
{
    return _capacity - _size;
}

inline uint8_t& Buffer::operator[](std::size_t index)
{
    BMCL_ASSERT(index < _size);
    return _ptr[index];
}

inline uint8_t Buffer::operator[](std::size_t index) const
{
    BMCL_ASSERT(index < _size);
    return _ptr[index];
}

inline Bytes Buffer::asBytes() const
{
    return Bytes(_ptr, _size);
}

inline Buffer::operator Bytes() const
{
    return asBytes();
}
}
