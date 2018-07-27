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
#include "bmcl/Fwd.h"

#include <cstddef>
#include <cstdint>
#include <iterator>

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
    Buffer(const void* data, std::size_t size);
    Buffer(bmcl::Bytes data);
    Buffer(const Buffer& other);
    Buffer(Buffer&& other);
    ~Buffer();

    static Buffer createWithUnitializedData(std::size_t size);

    inline std::size_t size() const;
    inline std::size_t capacity() const;
    inline bool isEmpty() const;

    MemWriter dataWriter();

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
    void removeFront(std::size_t size);
    void shrink();

    void write(const void* data, std::size_t size);
    inline std::size_t writableSize() const;

    void writeVarUint(uint64_t value);
    void writeVarInt(int64_t value);

    Buffer& operator=(const Buffer& other);
    Buffer& operator=(Buffer&& other);

    inline uint8_t& operator[](std::size_t index);
    inline uint8_t operator[](std::size_t index) const;

    inline Bytes asBytes() const;
    inline operator Bytes() const;

private:
    Buffer(void* ptr, std::size_t size, std::size_t capacity);

    void extend(std::size_t additionalSize);
    void dealloc();
    void copyFrom(const Buffer& other);
    void moveFrom(Buffer&& other);
    void realloc(std::size_t size);

    uint8_t* _ptr;
    std::size_t _size;
    std::size_t _capacity;
};
}
