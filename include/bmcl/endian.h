#pragma once

#include "bmcl/platform.h"

#if defined(BMCL_PLATFORM_LINUX)
    #define _DEFAULT_SOURCE
    #include <endian.h>
#elif defined(BMCL_PLATFORM_BSD)
    #include <sys/endian.h>
#elif defined(BMCL_PLATFORM_MSVC)
    #include <stdlib.h>
#elif defined(BMCL_PLATFORM_APPLE)
    #include <libkern/OSByteOrder.h>
    #include <machine/endian.h>
#elif defined(BMCL_PLATFORM_SOLARIS)
    #include <sys/isa_defs.h>
#else
    #error "endian.h not implemented for this target"
#endif

/* endian macros */

#if defined(BMCL_PLATFORM_LINUX)
    #if __BYTE_ORDER == __LITTLE_ENDIAN
        #define BMCL_LITTLE_ENDIAN
    #elif __BYTE_ORDER == __BIG_ENDIAN
        #define BMCL_BIG_ENDIAN
    #else
        #error "unsupported endianness"
    #endif
#elif defined(BMCL_PLATFORM_BSD)
    #if _BYTE_ORDER == _LITTLE_ENDIAN
        #define BMCL_LITTLE_ENDIAN
    #elif _BYTE_ORDER == _BIG_ENDIAN
        #define BMCL_BIG_ENDIAN
    #else
        #error "unsupported endianness"
    #endif
#elif defined(BMCL_PLATFORM_WINDOWS)
    #define BMCL_LITTLE_ENDIAN
#elif defined(BMCL_PLATFORM_APPLE)
    #if __DARWIN_BYTE_ORDER == __DARWIN_LITTLE_ENDIAN
        #define BMCL_LITTLE_ENDIAN
    #elif __DARWIN_BYTE_ORDER == __DARWIN_BIG_ENDIAN
        #define BMCL_BIG_ENDIAN
    #else
        #error "unsupported endianness"
    #endif
#elif defined(BMCL_PLATFORM_SOLARIS)
    #ifdef _LITTLE_ENDIAN
        #define BMCL_LITTLE_ENDIAN
    #elif _BIG_ENDIAN
        #define BMCL_BIG_ENDIAN
    #else
        #error "unsupported endianness"
    #endif
#else
    #error "endian macros not implemented for this target"
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

#else
#error "byteswap not implemented for this target"
#endif

#if defined(BMCL_PLATFORM_APPLE)

#define be16dec(ptr) OSReadBigInt16(ptr, 0)
#define be32dec(ptr) OSReadBigInt32(ptr, 0)
#define be64dec(ptr) OSReadBigInt64(ptr, 0)
#define le16dec(ptr) OSReadLittleInt16(ptr, 0)
#define le32dec(ptr) OSReadLittleInt32(ptr, 0)
#define le64dec(ptr) OSReadLittleInt64(ptr, 0)

#define be16enc(ptr, value) OSWriteBigInt16(ptr, 0, value)
#define be32enc(ptr, value) OSWriteBigInt32(ptr, 0, value)
#define be64enc(ptr, value) OSWriteBigInt64(ptr, 0, value)
#define le16enc(ptr, value) OSWriteLittleInt16(ptr, 0, value)
#define le32enc(ptr, value) OSWriteLittleInt32(ptr, 0, value)
#define le64enc(ptr, value) OSWriteLittleInt64(ptr, 0, value)

#elif !defined(BMCL_PLATFORM_FREEBSD)

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

    return ((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3]);
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

    return ((p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0]);
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

#endif

