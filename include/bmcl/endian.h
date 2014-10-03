#pragma once

#include "bmcl/platform.h"

#if defined(BMCL_PLATFORM_LINUX)
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

#define bmcl_htobe16(value) htobe16(value)
#define bmcl_htole16(value) htole16(value)
#define bmcl_htobe32(value) htobe32(value)
#define bmcl_htole32(value) htole32(value)
#define bmcl_htobe64(value) htobe64(value)
#define bmcl_htole64(value) htole64(value)

#if defined(BMCL_PLATFORM_OPENBSD)

#define bmcl_be16toh(value) betoh16(value)
#define bmcl_le16toh(value) letoh16(value)
#define bmcl_be32toh(value) betoh32(value)
#define bmcl_le32toh(value) letoh32(value)
#define bmcl_be64toh(value) betoh64(value)
#define bmcl_le64toh(value) letoh64(value)

#else

#define bmcl_be16toh(value) be16toh(value)
#define bmcl_le16toh(value) le16toh(value)
#define bmcl_be32toh(value) be32toh(value)
#define bmcl_le32toh(value) le32toh(value)
#define bmcl_be64toh(value) be64toh(value)
#define bmcl_le64toh(value) le64toh(value)

#endif

#elif defined(BMCL_PLATFORM_MSVC)

#define bmcl_htobe16(value) _byteswap_ushort(value)
#define bmcl_htole16(value) value
#define bmcl_be16toh(value) _byteswap_ushort(value)
#define bmcl_le16toh(value) value
#define bmcl_htobe32(value) _byteswap_ulong(value)
#define bmcl_htole32(value) value
#define bmcl_be32toh(value) _byteswap_ulong(value)
#define bmcl_le32toh(value) value
#define bmcl_htobe64(value) _byteswap_uint64(value)
#define bmcl_htole64(value) value
#define bmcl_be64toh(value) _byteswap_uint64(value)
#define bmcl_le64toh(value) value

#elif defined(BMCL_PLATFORM_APPLE)

/* TODO: ConstToInt */

#define bmcl_htobe16(value) OSSwapHostToBigInt16(value)
#define bmcl_htole16(value) OSSwapHostToLittleInt16(value)
#define bmcl_be16toh(value) OSSwapBigToHostInt16(value)
#define bmcl_le16toh(value) OSSwapLittleToHostInt16(value)
#define bmcl_htobe32(value) OSSwapHostToBigInt32(value)
#define bmcl_htole32(value) OSSwapHostToLittleInt32(value)
#define bmcl_be32toh(value) OSSwapBigToHostInt32(value)
#define bmcl_le32toh(value) OSSwapLittleToHostInt32(value)
#define bmcl_htobe64(value) OSSwapHostToBigInt64(value)
#define bmcl_htole64(value) OSSwapHostToLittleInt64(value)
#define bmcl_be64toh(value) OSSwapBigToHostInt64(value)
#define bmcl_le64toh(value) OSSwapLittleToHostInt64(value)

#else
#error "byteswap not implemented for this target"
#endif
