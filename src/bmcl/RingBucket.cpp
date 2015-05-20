/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/Config.h"
#include "bmcl/Assert.h"
#include "bmcl/RingBucket.h"

#include <cstdlib>

namespace bmcl {

RingBucket::RingBucket(void* data, std::size_t size)
    : _ringbuf(data, size)
{
    BMCL_ASSERT(data != 0);
    BMCL_ASSERT(size != 0);
    _count = 0;
}

#if BMCL_HAVE_MALLOC

RingBucket::RingBucket(std::size_t size)
    : _ringbuf(size)
{
    BMCL_ASSERT(size != 0);
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
    BMCL_ASSERT(!isEmpty());
    RingBucketHeader size;
    _ringbuf.peek(&size, sizeof(size));
    return size;
}

void RingBucket::copyFirst(void* dest) const
{
    BMCL_ASSERT(!isEmpty());
    RingBucketHeader firstSize;
    _ringbuf.peek(&firstSize, sizeof(firstSize));
    _ringbuf.peek(dest, firstSize, sizeof(firstSize));
}

void RingBucket::prepareForAppend(std::size_t dataSize)
{
    RingBucketHeader elementSize = dataSize + sizeof(elementSize);
    BMCL_ASSERT(elementSize <= _ringbuf.size());

    if (freeSpace() < elementSize)
        eraseElementsToFitSize(elementSize);

    _ringbuf.write(&dataSize, sizeof(dataSize));
}

std::size_t RingBucket::eraseElement()
{
    BMCL_ASSERT(!isEmpty());
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
