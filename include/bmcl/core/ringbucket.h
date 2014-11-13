/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/core/ringbuf.h"

#include <cstdbool>
#include <cstddef>

namespace bmcl {
namespace core {

typedef std::size_t RingBucketHeaderSize;

class RingBucket {
public:
    RingBucket(void* data, std::size_t size)
        : _ringbuf(data, size)
    {
        assert(data != 0);
        assert(size != 0);
        _count = 0;
    }

#if BMCL_HAVE_MALLOC

    RingBucket(std::size_t size): _ringbuf(size)
    {
        assert(size != 0);
        _count = 0;
    }

#endif

    std::size_t freeSpace() const
    {
        return _ringbuf.freeSpace();
    }

    static std::size_t headerSize()
    {
        return sizeof(RingBucketHeaderSize);
    }

    bool isEmpty() const
    {
        return _count == 0;
    }

    void reset()
    {
        _ringbuf.clear();
        _count = 0;
    }

    std::size_t count() const
    {
        return _count;
    }

    void removeFirst()
    {
        assert(!isEmpty());
        eraseElement();
    }

    void append(const void* data, std::size_t data_size);

    std::size_t firstSize() const;

    void copyFirst(void* dest) const;


private:
    void prepareForAppend(std::size_t data_size);

    std::size_t eraseElement();

    void eraseElementsToFitSize(std::size_t size);

    RingBuf _ringbuf;
    std::size_t _count;
};
}
}
