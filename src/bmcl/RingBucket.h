/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/config.h"
#include "bmcl/RingBuffer.h"

#include <cstddef>

namespace bmcl {

typedef std::size_t RingBucketHeader;

class RingBucket {
public:
    RingBucket(void* data, std::size_t size);

#if BMCL_HAVE_MALLOC

    RingBucket(std::size_t size);

#endif

    bool isEmpty() const { return _count == 0; }

    std::size_t freeSpace() const { return _ringbuf.freeSpace(); }
    std::size_t count() const { return _count; }
    std::size_t firstSize() const;
    static std::size_t headerSize() { return sizeof(RingBucketHeader); }

    void reset();
    void removeFirst() { eraseElement(); }
    void append(const void* data, std::size_t dataSize);
    void copyFirst(void* dest) const;

private:
    std::size_t eraseElement();
    void prepareForAppend(std::size_t dataSize);
    void eraseElementsToFitSize(std::size_t size);

    RingBuffer _ringbuf;
    std::size_t _count;
};
}
