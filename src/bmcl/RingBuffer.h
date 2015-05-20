/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/config.h"
#include "bmcl/Reader.h"
#include "bmcl/Writer.h"

#include <cassert>
#include <cstddef>
#include <stdint.h>
#include <cstring>

namespace bmcl {

class RingBuffer : public Reader, public Writer {
public:
    RingBuffer(void* data, std::size_t size);

#if BMCL_HAVE_MALLOC

    RingBuffer(std::size_t size);
    ~RingBuffer();

#endif

    void clear();
    void peek(void* dest, std::size_t size, std::size_t offset = 0) const;
    virtual std::size_t readableSize() const;
    virtual std::size_t writableSize() const;
    virtual void read(void* dest, std::size_t size);
    virtual void skip(std::size_t size);
    virtual void write(const void* data, std::size_t size);
    void erase(std::size_t size);

    std::size_t freeSpace() const { return _freeSpace; }
    std::size_t usedSpace() const { return _size - _freeSpace; }
    bool isFull() const { return _freeSpace == 0; }
    bool isEmpty() const { return _freeSpace == _size; }
    std::size_t size() const { return _size; }

private:
    void init(void* data, std::size_t size);
    void extend(std::size_t size);

    std::size_t _readOffset;
    std::size_t _writeOffset;
    std::size_t _size;
    std::size_t _freeSpace;
    uint8_t* _data;
#if BMCL_HAVE_MALLOC
    bool _hasAllocatedMem;
#endif
};
}
