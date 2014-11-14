/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/core/ringbucket.h"

#include <cassert>
#include <cstdlib>

namespace bmcl {
namespace core {

void RingBucket::append(const void* data, std::size_t data_size)
{
    prepareForAppend(data_size);
    _ringbuf.write(data, data_size);

    _count++;
}

std::size_t RingBucket::firstSize() const
{
    assert(!isEmpty());
    RingBucketHeaderSize size;
    _ringbuf.peek(&size, sizeof(size));
    return size;
}

void RingBucket::copyFirst(void* dest) const
{
    assert(!isEmpty());
    RingBucketHeaderSize first_size;
    _ringbuf.peek(&first_size, sizeof(first_size));
    _ringbuf.peek(dest, first_size, sizeof(first_size));
}

void RingBucket::prepareForAppend(std::size_t dataSize)
{
    RingBucketHeaderSize elementSize = dataSize + sizeof(elementSize);

    assert(elementSize <= _ringbuf.size());

    if (freeSpace() < elementSize)
        eraseElementsToFitSize(elementSize);

    _ringbuf.write(&dataSize, sizeof(dataSize));
}

std::size_t RingBucket::eraseElement()
{
    RingBucketHeaderSize elementSize;
    _ringbuf.peek(&elementSize, sizeof(elementSize));
    elementSize += sizeof(RingBucketHeaderSize);
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
