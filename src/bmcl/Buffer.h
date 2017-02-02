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
#include <cstring>

namespace bmcl {

class BMCL_EXPORT Buffer : public Writer<Buffer> {
public:
    typedef std::size_t size_type;
    typedef uint8_t* iterator;
    typedef const uint8_t* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    Buffer();
    Buffer(std::size_t size);
    Buffer(const Buffer& other);
    Buffer(Buffer&& other);
    ~Buffer();

    inline std::size_t size() const;
    inline std::size_t capacity() const;
    inline bool isEmpty() const;

    inline iterator begin();
    inline const_iterator cbegin() const;
    inline iterator end();
    inline const_iterator cend() const;
    inline reverse_iterator rbegin();
    inline const_reverse_iterator crbegin() const;
    inline reverse_iterator rend();
    inline const_reverse_iterator crend() const;

    const uint8_t* data() const;
    uint8_t* data();

    void resize(std::size_t size);
    void resize(std::size_t size, uint8_t filler);
    void reserve(std::size_t size);
    void shrink();

    void write(const void* data, std::size_t size);
    inline std::size_t writableSize() const;

    Buffer& operator=(const Buffer& other);
    Buffer& operator=(Buffer&& other);

    inline uint8_t& operator[](std::size_t index);
    inline uint8_t operator[](std::size_t index) const;

private:
    void dealloc();
    void copyFrom(const Buffer& other);
    void moveFrom(Buffer&& other);
    void realloc(std::size_t size);

    uint8_t* _ptr;
    std::size_t _size;
    std::size_t _capacity;
};

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
}
