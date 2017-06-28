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
#include "bmcl/Writer.h"
#include "bmcl/ArrayView.h"

#include <cassert>
#include <cstddef>
#include <stdint.h>
#include <cstring>

namespace bmcl {

class BMCL_EXPORT RingBuffer : public Reader<RingBuffer>, public Writer<RingBuffer> {
public:
    struct Chunks {
        Chunks(const uint8_t* first, std::size_t firstSize, const uint8_t* second, std::size_t secondSize)
            : first(first, firstSize)
            , second(second, secondSize)
        {
        }
        Bytes first;
        Bytes second;
    };

    RingBuffer(void* data, std::size_t size);

    inline void write(Bytes data);

    void clear();
    void peek(void* dest, std::size_t size, std::size_t offset = 0) const;
    void erase(std::size_t size);

    inline uint8_t peekUint8() const;

    inline std::size_t freeSpace() const;
    inline std::size_t usedSpace() const;
    inline bool isFull() const;
    inline bool isEmpty() const;
    inline std::size_t size() const;

    inline const uint8_t* data() const;
    inline uint8_t* data();

    inline std::size_t writableSize() const;
    void write(const void* data, std::size_t size);
    inline std::size_t readableSize() const;
    void read(void* dest, std::size_t size);
    inline void skip(std::size_t size);

    Chunks readableChunks();

private:
    void init(void* data, std::size_t size);
    void extend(std::size_t size);

    std::size_t _readOffset;
    std::size_t _writeOffset;
    std::size_t _size;
    std::size_t _freeSpace;
    uint8_t* _data;
};

inline void RingBuffer::write(Bytes data)
{
    write(data.begin(), data.size());
}

inline uint8_t RingBuffer::peekUint8() const
{
    BMCL_ASSERT(_freeSpace != _size);
    return *(_data + _readOffset);
}

inline std::size_t RingBuffer::freeSpace() const
{
    return _freeSpace;
}

inline std::size_t RingBuffer::usedSpace() const
{
    return _size - _freeSpace;
}

inline bool RingBuffer::isFull() const
{
    return _freeSpace == 0;
}

inline bool RingBuffer::isEmpty() const
{
    return _freeSpace == _size;
}

inline std::size_t RingBuffer::size() const
{
    return _size;
}

inline const uint8_t* RingBuffer::data() const
{
    return _data;
}

inline uint8_t* RingBuffer::data()
{
    return _data;
}

inline std::size_t RingBuffer::writableSize() const
{
    return freeSpace();
}

inline void RingBuffer::skip(std::size_t size)
{
    erase(size);
}

inline std::size_t RingBuffer::readableSize() const
{
    return usedSpace();
}
}
