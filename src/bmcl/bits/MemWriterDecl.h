/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/Writer.h"
#include "bmcl/Fwd.h"

namespace bmcl {

class BMCL_EXPORT MemWriter : public Writer<MemWriter> {
public:
    template <std::size_t n, typename R>
    MemWriter(R(&array)[n]);

    MemWriter(void* dest, std::size_t maxSize);

    inline bool isFull() const;
    inline bool isEmpty() const;

    inline uint8_t* current() const;
    inline uint8_t* start() const;
    inline uint8_t* end() const;

    inline std::size_t sizeUsed() const;
    inline std::size_t sizeLeft() const;
    inline std::size_t maxSize() const;

    inline Bytes writenData() const;

    inline void reset();
    void advance(std::size_t size);
    void fillUp(uint8_t byte);
    void fill(uint8_t byte, std::size_t size);

    void write(const void* data, std::size_t size);
    inline void write(Bytes data);
    inline std::size_t writableSize() const;

    bool writeVarUint(uint64_t value);
    bool writeVarInt(int64_t value);

private:
    void init(void* dest, std::size_t maxSize);

    uint8_t* _start;
    uint8_t* _current;
    uint8_t* _end;
};
}
