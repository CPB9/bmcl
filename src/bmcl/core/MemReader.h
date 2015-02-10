/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/core/Reader.h"

#include <cassert>
#include <cstddef>
#include <stdint.h>
#include <cstring>

namespace bmcl {
namespace core {

class MemReader : public Reader {
public:
    MemReader(const void* ptr, std::size_t size);

    bool isEmpty() const { return _current >= _end; }

    const uint8_t* current() const { return _current; }
    const uint8_t* start() const { return _start; }
    const uint8_t* end() const { return _end; }

    std::size_t size() const { return _end - _start; }
    std::size_t sizeLeft() const { return _end - _current; }
    std::size_t sizeRead() const { return _current - _start; }

    void reset() { _current = _start; }
    void skip(std::size_t size);
    void peek(void* dest, std::size_t size, std::size_t offset) const;
    virtual void read(void* dest, std::size_t size);

private:
    const uint8_t* _start;
    const uint8_t* _current;
    const uint8_t* _end;
};
}
}
