/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#if defined(__unux) || defined(__unix__)
    #define BMCL_PLATFORM_UNIX
    #if defined(__linux) || defined(__linux__)
        #define BMCL_PLATFORM_LINUX
    #elif defined(__DragonFly__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
        #define BMCL_PLATFORM_BSD
        #if defined(__DragonFly__)
            #define BMCL_PLATFORM_DRAGONFLY
        #elif defined(__FreeBSD__)
            #define BMCL_PLATFORM_FREEBSD
        #elif defined(__OpenBSD__)
            #define BMCL_PLATFORM_OPENBSD
        #elif defined(__NetBSD__)
            #define BMCL_PLATFORM_NETBSD
        #endif
    #elif defined(_AIX)
        #define BMCL_PLATFORM_AIX
    #elif defined(__hpux)
        #define BMCL_PLATFORM_HPUX
    #elif defined(__sun) && defined(__SVR4)
        #define BMCL_PLATFORM_SOLARIS
    #elif defined(__QNX__)
        #define BMCL_PLATFORM_QNX
    #else
        #error "unsupported unix target"
    #endif
#elif defined(_WIN32)
    #define BMCL_PLATFORM_WINDOWS
    #if defined(_MSC_VER)
        #define BMCL_PLATFORM_MSVC
    #elif defined(__MINGW32__)
        #define BMCL_PLATFORM_MINGW
    #else
        #error "unsupported windows target"
    #endif
#elif defined(__APPLE__)
    #define BMCL_PLATFORM_APPLE
#elif defined(__AVR__)
    #define BMCL_PLATFORM_AVR
#else
    #error "unsupported platform"
#endif

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
#elif defined(BMCL_PLATFORM_AVR)
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        #define BMCL_LITTLE_ENDIAN
    #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        #define BMCL_BIG_ENDIAN
    #else
        #error "unsupported endianness"
    #endif
#else
    #error "endian macros not implemented for this target"
#endif

