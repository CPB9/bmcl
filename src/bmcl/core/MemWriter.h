/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/core/Stack.h"
#include "bmcl/core/Writer.h"

#include <cassert>
#include <cstddef>
#include <stdint.h>
#include <cstring>

namespace bmcl {
namespace core {

class MemWriter : public bmcl::core::Writer, public bmcl::core::Stack {
public:
    MemWriter(void* dest, std::size_t maxSize);

#if BMCL_HAVE_MALLOC

    MemWriter(std::size_t maxSize);
    ~MemWriter();

#endif

    bool isFull() const { return _current >= _end; }

    uint8_t* ptr() const { return _start; }
    uint8_t* currentPtr() const { return _current; }

    std::size_t sizeUsed() const { return _current - _start; }
    std::size_t maxSize() const { return _end - _start; }
    virtual std::size_t availableSize() const;

    void advance(std::size_t size);
    void fillUp(uint8_t byte);
    void fill(uint8_t byte, std::size_t size);
    virtual void write(const void* data, std::size_t size);
    virtual void push(const void* src, std::size_t size);
    virtual void pop(void* dest, std::size_t size);

private:
    void init(void* dest, std::size_t maxSize);

    uint8_t* _start;
    uint8_t* _current;
    const uint8_t* _end;
#if BMCL_HAVE_MALLOC
    bool hasAllocatedMem;
#endif
};
}
}
