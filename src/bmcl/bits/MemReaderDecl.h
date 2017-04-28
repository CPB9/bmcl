/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/Fwd.h"
#include "bmcl/Reader.h"

namespace bmcl {

class BMCL_EXPORT MemReader : public Reader<MemReader> {
public:
    template <std::size_t n, typename R>
    MemReader(const R(&array)[n]);

    inline MemReader(const void* ptr, std::size_t size);
    inline MemReader(Bytes data);
    MemReader(const MemWriter& memWriter);

    inline bool isEmpty() const;

    inline const uint8_t* current() const;
    inline const uint8_t* start() const;
    inline const uint8_t* end() const;

    inline std::size_t size() const;
    inline std::size_t sizeLeft() const;
    inline std::size_t sizeRead() const;

    inline void reset();
    void peek(void* dest, std::size_t size, std::size_t offset) const;

    inline std::size_t readableSize() const;
    void read(void* dest, std::size_t size);
    void skip(std::size_t size);

    Result<uint64_t, void> readVarUint();
    bool readVarUint(uint64_t* dest);
    bool readVarInt(int64_t* dest);

    inline uint8_t readUint8();
    inline int8_t readInt8();

private:
    void init(const void* ptr, std::size_t size);

    const uint8_t* _start;
    const uint8_t* _current;
    const uint8_t* _end;
};
}

