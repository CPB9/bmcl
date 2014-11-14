/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/core/endian.h"
#include "bmcl/core/ringbuf.h"

#include <cassert>
#include <stdint.h>
#include <cstring>

#define min(a, b) (((a) > (b)) ? (b) : (a))

namespace bmcl {
namespace core {

void RingBuf::write(const void* data, std::size_t size)
{
    assert(size > 0);
    assert(size <= _size);
    if (_free_space < size) {
        erase(size - _free_space);
    }

    if (_read_offset > _write_offset) { /* *********w---------------r************ */
        std::memcpy(_data + _write_offset, data, size);
    } else { /* ----------r**************w---------- */
        std::size_t rightData = _size - _write_offset;
        std::size_t firstChunkSize = min(size, rightData);
        std::memcpy(_data + _write_offset, data, firstChunkSize);
        if (size > firstChunkSize) {
            std::size_t secondChunkSize = size - firstChunkSize;
            std::memcpy(_data, (const uint8_t*)data + firstChunkSize, secondChunkSize);
        }
    }
    extend(size);
}

void RingBuf::peek(void* dest, std::size_t size, std::size_t offset) const
{
    std::size_t read_offset = _read_offset + offset;
    if (read_offset >= _size) {
        read_offset -= _size;
    }
    assert(size > 0);
    assert(size + offset <= _size - _free_space);
    if (read_offset < _write_offset) { /* ----------r**************w---------- */
        std::memcpy(dest, _data + read_offset, size);
    } else { /* *********w---------------r************ */
        std::size_t rightData = _size - read_offset;
        std::size_t firstChunkSize = min(size, rightData);
        std::memcpy(dest, _data + read_offset, firstChunkSize);
        if (size > firstChunkSize) {
            std::size_t secondChunkSize = size - firstChunkSize;
            std::memcpy((uint8_t*)dest + firstChunkSize, _data, secondChunkSize);
        }
    }
}
}
}
