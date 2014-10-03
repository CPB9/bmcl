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
#else
    #error "unsupported platform"
#endif
