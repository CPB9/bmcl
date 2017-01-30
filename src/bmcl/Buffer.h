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
    Buffer();
    Buffer(std::size_t size);
    Buffer(const Buffer& other);
    Buffer(Buffer&& other);
    ~Buffer();

    inline std::size_t size() const;
    inline std::size_t capacity() const;
    inline bool isEmpty() const;

    inline const uint8_t* start() const;
    inline const uint8_t* begin() const;
    inline const uint8_t* end() const;

    inline uint8_t* start();
    inline uint8_t* begin();
    inline uint8_t* end();

    void resize(std::size_t size);
    void resize(std::size_t size, uint8_t filler);
    void reserve(std::size_t size);
    void shrink();

    void writeImpl(const void* data, std::size_t size);
    inline std::size_t writableSizeImpl() const;

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

inline const uint8_t* Buffer::start() const
{
    return _ptr;
}

inline const uint8_t* Buffer::begin() const
{
    return _ptr;
}

inline const uint8_t* Buffer::end() const
{
    return _ptr + _size;
}

inline uint8_t* Buffer::start()
{
    return _ptr;
}

inline uint8_t* Buffer::begin()
{
    return _ptr;
}

inline uint8_t* Buffer::end()
{
    return _ptr + _size;
}

inline bool Buffer::isEmpty() const
{
    return _size == 0;
}

inline std::size_t Buffer::writableSizeImpl() const
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
