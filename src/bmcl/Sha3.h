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

#include <cstdint>
#include <cstddef>
#include <array>

namespace bmcl {

BMCL_EXPORT void keccakf(uint64_t* state);

/* 'Words' here refers to uint64_t */

template <std::size_t bits>
class BMCL_EXPORT Sha3 {
public:
    using HashView = FixedArrayView<uint8_t, bits / 8>;
    using HashContainer = std::array<uint8_t, bits / 8>;

    /* the double size of the hash output in
     * words (e.g. 16 for Keccak 512) */
    static constexpr unsigned capacityWords =  2 * bits / (8 * sizeof(std::uint64_t));
    static constexpr unsigned keccakSpongeWords = 1600 / 8 / sizeof(std::uint64_t);
    static constexpr unsigned keccakSpongeBytes = keccakSpongeWords * 8;

    Sha3();

    void reset();
    void update(const void* src, std::size_t len);
    void update(Bytes data);
    HashView finalize();

    static HashContainer calcInOneStep(const void* src, std::size_t len);
    static HashContainer calcInOneStep(Bytes data);

private:
    uint64_t _saved;            /* the portion of the input message that we
                                 * didn't consume yet */
    union {                     /* Keccak's state */
        std::uint64_t _s64[keccakSpongeWords];
        std::uint8_t _s8[keccakSpongeBytes];
    };
    unsigned _byteIndex;        /* 0..7--the next byte after the set one
                                 * (starts from 0; 0--none are buffered) */
    unsigned _wordIndex;        /* 0..24--the next word to integrate input
                                 * (starts from 0) */
};

extern template class Sha3<224>;
extern template class Sha3<256>;
extern template class Sha3<384>;
extern template class Sha3<512>;
}
