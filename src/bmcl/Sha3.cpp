/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

// based on https://github.com/brainhub/SHA3IUF

/* -------------------------------------------------------------------------
 * Works when compiled for either 32-bit or 64-bit targets, optimized for
 * 64 bit.
 *
 * Canonical implementation of Init/Update/Finalize for SHA-3 byte input.
 *
 * SHA3-256, SHA3-384, SHA-512 are implemented. SHA-224 can easily be added.
 *
 * Based on code from http://keccak.noekeon.org/ .
 *
 * I place the code that I wrote into public domain, free to use.
 *
 * I would appreciate if you give credits to this work if you used it to
 * write or test * your code.
 *
 * Aug 2015. Andrey Jivsov. crypto@brainhub.org
 * ---------------------------------------------------------------------- */

#include "bmcl/Sha3.h"
#include "bmcl/ArrayView.h"
#include "bmcl/FixedArrayView.h"

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cassert>

namespace bmcl {

#define SHA3_ASSERT(x) assert(x)

#if defined(_MSC_VER)
#define SHA3_CONST(x) x
#else
#define SHA3_CONST(x) x##L
#endif

#ifndef SHA3_ROTL64
#define SHA3_ROTL64(x, y) (((x) << (y)) | ((x) >> ((sizeof(uint64_t)*8) - (y))))
#endif

#define KECCAK_ROUNDS 24

static const uint64_t keccakfRndc[KECCAK_ROUNDS] = {
    SHA3_CONST(0x0000000000000001UL), SHA3_CONST(0x0000000000008082UL),
    SHA3_CONST(0x800000000000808aUL), SHA3_CONST(0x8000000080008000UL),
    SHA3_CONST(0x000000000000808bUL), SHA3_CONST(0x0000000080000001UL),
    SHA3_CONST(0x8000000080008081UL), SHA3_CONST(0x8000000000008009UL),
    SHA3_CONST(0x000000000000008aUL), SHA3_CONST(0x0000000000000088UL),
    SHA3_CONST(0x0000000080008009UL), SHA3_CONST(0x000000008000000aUL),
    SHA3_CONST(0x000000008000808bUL), SHA3_CONST(0x800000000000008bUL),
    SHA3_CONST(0x8000000000008089UL), SHA3_CONST(0x8000000000008003UL),
    SHA3_CONST(0x8000000000008002UL), SHA3_CONST(0x8000000000000080UL),
    SHA3_CONST(0x000000000000800aUL), SHA3_CONST(0x800000008000000aUL),
    SHA3_CONST(0x8000000080008081UL), SHA3_CONST(0x8000000000008080UL),
    SHA3_CONST(0x0000000080000001UL), SHA3_CONST(0x8000000080008008UL)
};

static void keccakf(uint64_t* s)
{
    uint64_t t;
    uint64_t bc[5];

    for (std::size_t round = 0; round < KECCAK_ROUNDS; round++) {
        /* Theta */
        bc[0] = s[0] ^ s[5] ^ s[10] ^ s[15] ^ s[20];
        bc[1] = s[1] ^ s[6] ^ s[11] ^ s[16] ^ s[21];
        bc[2] = s[2] ^ s[7] ^ s[12] ^ s[17] ^ s[22];
        bc[3] = s[3] ^ s[8] ^ s[13] ^ s[18] ^ s[23];
        bc[4] = s[4] ^ s[9] ^ s[14] ^ s[19] ^ s[24];

        for (std::size_t i = 0; i < 5; i++) {
            t = bc[(i + 4) % 5] ^ SHA3_ROTL64(bc[(i + 1) % 5], 1);
            s[0 + i] ^= t;
            s[5 + i] ^= t;
            s[10 + i] ^= t;
            s[15 + i] ^= t;
            s[20 + i] ^= t;
        }

        /* Rho */
        s[ 1] = SHA3_ROTL64(s[ 1],  1);
        s[ 2] = SHA3_ROTL64(s[ 2], 62);
        s[ 3] = SHA3_ROTL64(s[ 3], 28);
        s[ 4] = SHA3_ROTL64(s[ 4], 27);
        s[ 5] = SHA3_ROTL64(s[ 5], 36);
        s[ 6] = SHA3_ROTL64(s[ 6], 44);
        s[ 7] = SHA3_ROTL64(s[ 7],  6);
        s[ 8] = SHA3_ROTL64(s[ 8], 55);
        s[ 9] = SHA3_ROTL64(s[ 9], 20);
        s[10] = SHA3_ROTL64(s[10],  3);
        s[11] = SHA3_ROTL64(s[11], 10);
        s[12] = SHA3_ROTL64(s[12], 43);
        s[13] = SHA3_ROTL64(s[13], 25);
        s[14] = SHA3_ROTL64(s[14], 39);
        s[15] = SHA3_ROTL64(s[15], 41);
        s[16] = SHA3_ROTL64(s[16], 45);
        s[17] = SHA3_ROTL64(s[17], 15);
        s[18] = SHA3_ROTL64(s[18], 21);
        s[19] = SHA3_ROTL64(s[19],  8);
        s[20] = SHA3_ROTL64(s[20], 18);
        s[21] = SHA3_ROTL64(s[21],  2);
        s[22] = SHA3_ROTL64(s[22], 61);
        s[23] = SHA3_ROTL64(s[23], 56);
        s[24] = SHA3_ROTL64(s[24], 14);

        /* Pi */
        t     = s[ 1];
        s[ 1] = s[ 6];
        s[ 6] = s[ 9];
        s[ 9] = s[22];
        s[22] = s[14];
        s[14] = s[20];
        s[20] = s[ 2];
        s[ 2] = s[12];
        s[12] = s[13];
        s[13] = s[19];
        s[19] = s[23];
        s[23] = s[15];
        s[15] = s[ 4];
        s[ 4] = s[24];
        s[24] = s[21];
        s[21] = s[ 8];
        s[ 8] = s[16];
        s[16] = s[ 5];
        s[ 5] = s[ 3];
        s[ 3] = s[18];
        s[18] = s[17];
        s[17] = s[11];
        s[11] = s[ 7];
        s[ 7] = s[10];
        s[10] = t;

        /* Chi */
        for (std::size_t j = 0; j < 25; j += 5) {
            bc[0] = s[j + 0];
            bc[1] = s[j + 1];
            s[j + 0] ^= ~bc[1] & s[j + 2];
            s[j + 1] ^= ~s[j + 2] & s[j + 3];
            s[j + 2] ^= ~s[j + 3] & s[j + 4];
            s[j + 3] ^= ~s[j + 4] & bc[0];
            s[j + 4] ^= ~bc[0] & bc[1];
        }

        /* Iota */
        s[0] ^= keccakfRndc[round];
    }
}

template <std::size_t bits>
Sha3<bits>::Sha3()
{
    reset();
}

template <std::size_t bits>
void Sha3<bits>::reset()
{
    _saved = 0;
    std::memset(_s8, 0, sizeof(_s8));
    _byteIndex = 0;
    _wordIndex = 0;
}

template <std::size_t bits>
void Sha3<bits>::update(Bytes data)
{
    update(data.begin(), data.size());
}

template <std::size_t bits>
void Sha3<bits>::update(const void* src, std::size_t len)
{
    /* 0...7 -- how much is needed to have a word */
    unsigned oldTail = (8 - _byteIndex) & 7;

    const uint8_t* buf = (const uint8_t*)src;

    SHA3_ASSERT(_byteIndex < 8);
    SHA3_ASSERT(_wordIndex < sizeof(_s64) / sizeof(_s64[0]));

    if (len < oldTail) {        /* have no complete word or haven't started
                                 * the word yet */
        /* endian-independent code follows: */
        while (len--) {
            _saved |= (uint64_t) (*(buf++)) << ((_byteIndex++) * 8);
        }
        SHA3_ASSERT(_byteIndex < 8);
        return;
    }

    if (oldTail) {              /* will have one word to process */
        /* endian-independent code follows: */
        len -= oldTail;
        while (oldTail--) {
            _saved |= (uint64_t) (*(buf++)) << ((_byteIndex++) * 8);
        }

        /* now ready to add saved to the sponge */
        _s64[_wordIndex] ^= _saved;
        SHA3_ASSERT(_byteIndex == 8);
        _byteIndex = 0;
        _saved = 0;
        if (++_wordIndex == (keccakSpongeWords - capacityWords)) {
            keccakf(_s64);
            _wordIndex = 0;
        }
    }

    /* now work in full words directly from input */

    SHA3_ASSERT(_byteIndex == 0);

    std::size_t words = len / sizeof(uint64_t);
    unsigned tail = len - words * sizeof(uint64_t);

    for (std::size_t i = 0; i < words; i++, buf += sizeof(uint64_t)) {
        const uint64_t t = (uint64_t) (buf[0]) |
                ((uint64_t) (buf[1]) << 8 * 1) |
                ((uint64_t) (buf[2]) << 8 * 2) |
                ((uint64_t) (buf[3]) << 8 * 3) |
                ((uint64_t) (buf[4]) << 8 * 4) |
                ((uint64_t) (buf[5]) << 8 * 5) |
                ((uint64_t) (buf[6]) << 8 * 6) |
                ((uint64_t) (buf[7]) << 8 * 7);
#if defined(__x86_64__ ) || defined(__i386__)
        SHA3_ASSERT(memcmp(&t, buf, 8) == 0);
#endif
        _s64[_wordIndex] ^= t;
        if (++_wordIndex == (keccakSpongeWords - capacityWords)) {
            keccakf(_s64);
            _wordIndex = 0;
        }
    }

    /* finally, save the partial word */
    SHA3_ASSERT(_byteIndex == 0 && tail < 8);
    while (tail--) {
        _saved |= (uint64_t) (*(buf++)) << ((_byteIndex++) * 8);
    }
    SHA3_ASSERT(_byteIndex < 8);
}

/* This is simply the 'update' with the padding block.
 * The padding block is 0x01 || 0x00* || 0x80. First 0x01 and last 0x80
 * bytes are always present, but they can be the same byte.
 */

template <std::size_t bits>
FixedArrayView<uint8_t, bits / 8> Sha3<bits>::finalize()
{
    /* Append 2-bit suffix 01, per SHA-3 spec. Instead of 1 for padding we
     * use 1<<2 below. The 0x02 below corresponds to the suffix 01.
     * Overall, we feed 0, then 1, and finally 1 to start padding. Without
     * M || 01, we would simply use 1 to start padding. */

    _s64[_wordIndex] ^= (_saved ^ ((uint64_t) ((uint64_t) (0x02 | (1 << 2)) << ((_byteIndex) * 8))));
    _s64[keccakSpongeWords - capacityWords - 1] ^= SHA3_CONST(0x8000000000000000UL);
    keccakf(_s64);

    /* Return first bytes of the _state64. This conversion is not needed for
     * little-endian platforms e.g. wrap with #if !defined(__BYTE_ORDER__)
     * || !defined(__ORDER_LITTLE_ENDIAN__) || \
     * __BYTE_ORDER__!=__ORDER_LITTLE_ENDIAN__ ... the conversion below ...
     * #endif */
#ifndef BMCL_LITTLE_ENDIAN
    {
        for (std::size_t i = 0; i < keccakSpongeWords; i++) {
            const unsigned t1 = (uint32_t) _s64[i];
            const unsigned t2 = (uint32_t) ((_s64[i] >> 16) >> 16);
            _s8[i * 8 + 0] = (uint8_t) (t1);
            _s8[i * 8 + 1] = (uint8_t) (t1 >> 8);
            _s8[i * 8 + 2] = (uint8_t) (t1 >> 16);
            _s8[i * 8 + 3] = (uint8_t) (t1 >> 24);
            _s8[i * 8 + 4] = (uint8_t) (t2);
            _s8[i * 8 + 5] = (uint8_t) (t2 >> 8);
            _s8[i * 8 + 6] = (uint8_t) (t2 >> 16);
            _s8[i * 8 + 7] = (uint8_t) (t2 >> 24);
        }
    }
#endif

    return FixedArrayView<uint8_t, bits / 8>::fromRawData(_s8);
}

template class Sha3<224>;
template class Sha3<256>;
template class Sha3<384>;
template class Sha3<512>;
}
