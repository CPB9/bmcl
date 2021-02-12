/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"

#include <cstdint>

#if defined(BMCL_PLATFORM_LINUX)
    #include <endian.h>
#elif defined(BMCL_PLATFORM_BSD)
    #ifdef BMCL_PLATFORM_OPENBSD
        #include <sys/types.h>
        #include <machine/endian.h>
    #endif
    #include <sys/endian.h>
#elif defined(BMCL_PLATFORM_MSVC)
    #include <stdlib.h>
#elif defined(BMCL_PLATFORM_APPLE)
    #include <libkern/OSByteOrder.h>
    #include <machine/endian.h>
#elif defined(BMCL_PLATFORM_SOLARIS)
    #include <sys/isa_defs.h>
#elif defined(__GNUC__)
#else
    #error "endian.h not implemented for this target"
#endif

/* byteswap macros */

#if defined(BMCL_PLATFORM_LINUX) || defined(BMCL_PLATFORM_BSD)

#if defined(BMCL_PLATFORM_OPENBSD)

#define be16toh(value) betoh16(value)
#define be32toh(value) betoh32(value)
#define be64toh(value) betoh64(value)

#define le16toh(value) letoh16(value)
#define le32toh(value) letoh32(value)
#define le64toh(value) letoh64(value)

#endif

#elif defined(BMCL_PLATFORM_MSVC)

#define htobe16(value) _byteswap_ushort(value)
#define htobe32(value) _byteswap_ulong(value)
#define htobe64(value) _byteswap_uint64(value)
#define htole16(value) value
#define htole32(value) value
#define htole64(value) value

#define be16toh(value) _byteswap_ushort(value)
#define be32toh(value) _byteswap_ulong(value)
#define be64toh(value) _byteswap_uint64(value)
#define le16toh(value) value
#define le32toh(value) value
#define le64toh(value) value

#elif defined(BMCL_PLATFORM_APPLE)

#define htobe16(value) OSSwapHostToBigInt16(value)
#define htobe32(value) OSSwapHostToBigInt32(value)
#define htobe64(value) OSSwapHostToBigInt64(value)
#define htole16(value) OSSwapHostToLittleInt16(value)
#define htole32(value) OSSwapHostToLittleInt32(value)
#define htole64(value) OSSwapHostToLittleInt64(value)

#define be16toh(value) OSSwapBigToHostInt16(value)
#define be32toh(value) OSSwapBigToHostInt32(value)
#define be64toh(value) OSSwapBigToHostInt64(value)
#define le16toh(value) OSSwapLittleToHostInt16(value)
#define le32toh(value) OSSwapLittleToHostInt32(value)
#define le64toh(value) OSSwapLittleToHostInt64(value)

#elif defined(__GNUC__)

#ifdef BMCL_LITTLE_ENDIAN

#define htobe16(value) __builtin_bswap16(value)
#define htobe32(value) __builtin_bswap32(value)
#define htobe64(value) __builtin_bswap64(value)
#define htole16(value) value
#define htole32(value) value
#define htole64(value) value

#define be16toh(value) __builtin_bswap16(value)
#define be32toh(value) __builtin_bswap32(value)
#define be64toh(value) __builtin_bswap64(value)
#define le16toh(value) value
#define le32toh(value) value
#define le64toh(value) value

#else

#define htobe16(value) value
#define htobe32(value) value
#define htobe64(value) value
#define htole16(value) __builtin_bswap16(value)
#define htole32(value) __builtin_bswap32(value)
#define htole64(value) __builtin_bswap64(value)

#define be16toh(value) value
#define be32toh(value) value
#define be64toh(value) value
#define le16toh(value) __builtin_bswap16(value)
#define le32toh(value) __builtin_bswap32(value)
#define le64toh(value) __builtin_bswap64(value)

#endif

#else
#error "byteswap not implemented for this target"
#endif

#if defined(BMCL_PLATFORM_APPLE)

static inline uint16_t be16dec(const void *ptr) { return OSReadBigInt16(ptr, 0); }
static inline uint32_t be32dec(const void *ptr) { return OSReadBigInt32(ptr, 0); }
static inline uint64_t be64dec(const void *ptr) { return OSReadBigInt64(ptr, 0); }
static inline uint16_t le16dec(const void *ptr) { return OSReadLittleInt16(ptr, 0); }
static inline uint32_t le32dec(const void *ptr) { return OSReadLittleInt32(ptr, 0); }
static inline uint64_t le64dec(const void *ptr) { return OSReadLittleInt64(ptr, 0); }

static inline void be16enc(void *ptr, uint16_t value) { OSWriteBigInt16(ptr, 0, value); }
static inline void be32enc(void *ptr, uint32_t value) { OSWriteBigInt32(ptr, 0, value); }
static inline void be64enc(void *ptr, uint64_t value) { OSWriteBigInt64(ptr, 0, value); }
static inline void le16enc(void *ptr, uint16_t value) { OSWriteLittleInt16(ptr, 0, value); }
static inline void le32enc(void *ptr, uint32_t value) { OSWriteLittleInt32(ptr, 0, value); }
static inline void le64enc(void *ptr, uint64_t value) { OSWriteLittleInt64(ptr, 0, value); }

#elif !defined(BMCL_PLATFORM_FREEBSD) && !defined(BMCL_PLATFORM_NETBSD)

/* from FreeBSD <sys/endian.h> */
#include <stdint.h>

static inline uint16_t be16dec(const void* pp)
{
    const uint8_t* p = (const uint8_t*)pp;

    return ((p[0] << 8) | p[1]);
}

static inline uint32_t be32dec(const void* pp)
{
    const uint8_t* p = (const uint8_t*)pp;

    return (((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | (uint32_t)p[3]);
}

static inline uint64_t be64dec(const void* pp)
{
    const uint8_t* p = (const uint8_t*)pp;

    return (((uint64_t)be32dec(p) << 32) | be32dec(p + 4));
}

static inline uint16_t le16dec(const void* pp)
{
    const uint8_t* p = (const uint8_t*)pp;

    return ((p[1] << 8) | p[0]);
}

static inline uint32_t le32dec(const void* pp)
{
    const uint8_t* p = (const uint8_t*)pp;

    return (((uint32_t)p[3] << 24) | ((uint32_t)p[2] << 16) | ((uint32_t)p[1] << 8) | (uint32_t)p[0]);
}

static inline uint64_t le64dec(const void* pp)
{
    const uint8_t* p = (const uint8_t*)pp;

    return (((uint64_t)le32dec(p + 4) << 32) | le32dec(p));
}

static inline void be16enc(void* pp, uint16_t u)
{
    uint8_t* p = (uint8_t*)pp;

    p[0] = (u >> 8) & 0xff;
    p[1] = u & 0xff;
}

static inline void be32enc(void* pp, uint32_t u)
{
    uint8_t* p = (uint8_t*)pp;

    p[0] = (u >> 24) & 0xff;
    p[1] = (u >> 16) & 0xff;
    p[2] = (u >> 8) & 0xff;
    p[3] = u & 0xff;
}

static inline void be64enc(void* pp, uint64_t u)
{
    uint8_t* p = (uint8_t*)pp;

    be32enc(p, u >> 32);
    be32enc(p + 4, u & 0xffffffff);
}

static inline void le16enc(void* pp, uint16_t u)
{
    uint8_t* p = (uint8_t*)pp;

    p[0] = u & 0xff;
    p[1] = (u >> 8) & 0xff;
}

static inline void le32enc(void* pp, uint32_t u)
{
    uint8_t* p = (uint8_t*)pp;

    p[0] = u & 0xff;
    p[1] = (u >> 8) & 0xff;
    p[2] = (u >> 16) & 0xff;
    p[3] = (u >> 24) & 0xff;
}

static inline void le64enc(void* pp, uint64_t u)
{
    uint8_t* p = (uint8_t*)pp;

    le32enc(p, u & 0xffffffff);
    le32enc(p + 4, u >> 32);
}

namespace bmcl {

template <typename T>
T htole(T value);

template <>
inline std::uint8_t htole(std::uint8_t value)
{
    return value;
}

template <>
inline std::uint16_t htole(std::uint16_t value)
{
    return htole16(value);
}

template <>
inline std::uint32_t htole(std::uint32_t value)
{
    return htole32(value);
}

template <>
inline std::uint64_t htole(std::uint64_t value)
{
    return htole64(value);
}

template <>
inline std::int8_t htole(std::int8_t value)
{
    return value;
}

template <>
inline std::int16_t htole(std::int16_t value)
{
    return htole16(value);
}

template <>
inline std::int32_t htole(std::int32_t value)
{
    return htole32(value);
}

template <>
inline std::int64_t htole(std::int64_t value)
{
    return htole64(value);
}

template <typename T>
T letoh(T value);

template <>
inline std::uint8_t letoh(std::uint8_t value)
{
    return value;
}

template <>
inline std::uint16_t letoh(std::uint16_t value)
{
    return le16toh(value);
}

template <>
inline std::uint32_t letoh(std::uint32_t value)
{
    return le32toh(value);
}

template <>
inline std::uint64_t letoh(std::uint64_t value)
{
    return le64toh(value);
}

template <>
inline std::int8_t letoh(std::int8_t value)
{
    return value;
}

template <>
inline std::int16_t letoh(std::int16_t value)
{
    return le16toh(value);
}

template <>
inline std::int32_t letoh(std::int32_t value)
{
    return le32toh(value);
}

template <>
inline std::int64_t letoh(std::int64_t value)
{
    return le64toh(value);
}

template <typename T>
T htobe(T value);

template <>
inline std::uint8_t htobe(std::uint8_t value)
{
    return value;
}

template <>
inline std::uint16_t htobe(std::uint16_t value)
{
    return htobe16(value);
}

template <>
inline std::uint32_t htobe(std::uint32_t value)
{
    return htobe32(value);
}

template <>
inline std::uint64_t htobe(std::uint64_t value)
{
    return htobe64(value);
}

template <>
inline std::int8_t htobe(std::int8_t value)
{
    return value;
}

template <>
inline std::int16_t htobe(std::int16_t value)
{
    return htobe16(value);
}

template <>
inline std::int32_t htobe(std::int32_t value)
{
    return htobe32(value);
}

template <>
inline std::int64_t htobe(std::int64_t value)
{
    return htobe64(value);
}

template <typename T>
T betoh(T value);

template <>
inline std::uint8_t betoh(std::uint8_t value)
{
    return value;
}

template <>
inline std::uint16_t betoh(std::uint16_t value)
{
    return be16toh(value);
}

template <>
inline std::uint32_t betoh(std::uint32_t value)
{
    return be32toh(value);
}

template <>
inline std::uint64_t betoh(std::uint64_t value)
{
    return be64toh(value);
}

template <>
inline std::int8_t betoh(std::int8_t value)
{
    return value;
}

template <>
inline std::int16_t betoh(std::int16_t value)
{
    return be16toh(value);
}

template <>
inline std::int32_t betoh(std::int32_t value)
{
    return be32toh(value);
}

template <>
inline std::int64_t betoh(std::int64_t value)
{
    return be64toh(value);
}
}

#endif
