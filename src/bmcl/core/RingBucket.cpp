/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/config.h"
#include "bmcl/core/RingBucket.h"

#include <cassert>
#include <cstdlib>

namespace bmcl {
namespace core {

RingBucket::RingBucket(void* data, std::size_t size)
    : _ringbuf(data, size)
{
    assert(data != 0);
    assert(size != 0);
    _count = 0;
}

#if BMCL_HAVE_MALLOC

RingBucket::RingBucket(std::size_t size)
    : _ringbuf(size)
{
    assert(size != 0);
    _count = 0;
}

#endif

void RingBucket::reset()

{
    _ringbuf.clear();
    _count = 0;
}

void RingBucket::append(const void* data, std::size_t dataSize)
{
    prepareForAppend(dataSize);
    _ringbuf.write(data, dataSize);

    _count++;
}

std::size_t RingBucket::firstSize() const
{
    assert(!isEmpty());
    RingBucketHeader size;
    _ringbuf.peek(&size, sizeof(size));
    return size;
}

void RingBucket::copyFirst(void* dest) const
{
    assert(!isEmpty());
    RingBucketHeader firstSize;
    _ringbuf.peek(&firstSize, sizeof(firstSize));
    _ringbuf.peek(dest, firstSize, sizeof(firstSize));
}

void RingBucket::prepareForAppend(std::size_t dataSize)
{
    RingBucketHeader elementSize = dataSize + sizeof(elementSize);
    assert(elementSize <= _ringbuf.size());

    if (freeSpace() < elementSize)
        eraseElementsToFitSize(elementSize);

    _ringbuf.write(&dataSize, sizeof(dataSize));
}

std::size_t RingBucket::eraseElement()
{
    assert(!isEmpty());
    RingBucketHeader elementSize;
    _ringbuf.peek(&elementSize, sizeof(elementSize));
    elementSize += sizeof(RingBucketHeader);
    _ringbuf.erase(elementSize);
    _count--;
    return elementSize;
}

void RingBucket::eraseElementsToFitSize(std::size_t size)
{
    std::size_t freeSpace = this->freeSpace();
    while (freeSpace < size) {
        freeSpace += eraseElement();
    }
}
}
}
