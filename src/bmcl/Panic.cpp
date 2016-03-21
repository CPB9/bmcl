/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/Config.h"
#include "bmcl/Panic.h"
#include "bmcl/Alloca.h"

#include <cstdlib>
#include <cstdio>

#if defined(BMCL_PLATFORM_LINUX)
#include <execinfo.h>
#include <cxxabi.h>
#endif

namespace bmcl {

void printStackTrace()
{
#if defined(BMCL_PLATFORM_LINUX)
    std::FILE* out = stderr;
    unsigned int max_frames = 63;

    void* addrlist[max_frames + 1];

    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

    if (addrlen == 0) {
        std::fprintf(out, "  <empty, possibly corrupt>\n");
        return;
    }

    char** symbollist = backtrace_symbols(addrlist, addrlen);
    size_t funcnamesize = 256;
    char* funcname = (char*)malloc(funcnamesize);

    for (int i = 1; i < addrlen; i++) {
        char* begin_name = 0;
        char* begin_offset = 0;
        char* end_offset = 0;

        for (char* p = symbollist[i]; *p; ++p) {
            if (*p == '(') {
                begin_name = p;
            } else if (*p == '+') {
                begin_offset = p;
            } else if (*p == ')' && begin_offset) {
                end_offset = p;
                break;
            }
        }

        if (begin_name && begin_offset && end_offset && begin_name < begin_offset) {
            *begin_name++ = '\0';
            *begin_offset++ = '\0';
            *end_offset = '\0';

            int status;
            char* ret = abi::__cxa_demangle(begin_name, funcname, &funcnamesize, &status);
            if (status == 0) {
                funcname = ret;
                std::fprintf(out, "  %s : %s+%s\n", symbollist[i], funcname, begin_offset);
            } else {
                std::fprintf(out, "  %s : %s()+%s\n", symbollist[i], begin_name, begin_offset);
            }
        } else {
            std::fprintf(out, "  %s\n", symbollist[i]);
        }
    }

    free(funcname);
    free(symbollist);
#endif
}

static inline void print_stacktrace(FILE *out = stderr, unsigned int max_frames = 63)
{

}

static void defaultHandler(const char* msg)
{
    std::fputs(msg, stderr);
}

static PanicHandler currentHandler = defaultHandler;

void setPanicHandler(PanicHandler handler)
{
    currentHandler = handler;
}

PanicHandler panicHandler()
{
    return currentHandler;
}

PanicHandler defaultPanicHandler()
{
    return defaultHandler;
}

BMCL_NORETURN void panic(const char* msg)
{
    currentHandler(msg);
    printStackTrace();
    std::abort();
}
}
