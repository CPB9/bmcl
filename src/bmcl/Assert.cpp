/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/Config.h"
#include "bmcl/Assert.h"
#include "bmcl/Alloca.h"
#include "bmcl/Panic.h"

#include <cstdio>
#include <cstdlib>
#include <limits>
#include <cstring>

namespace bmcl {

BMCL_NORETURN void assertFail(const char* assertion, const char* msg, const char* file, int line)
{
    const char* format;
    if (msg) {
         format = "%s:%d: Assertion '%s' failed with message: %s.\0";
    } else {
         format = "%s:%d: Assertion '%s' failed.\0";
    }
    std::size_t maxLineNumLength = int(std::numeric_limits<int>::digits10) + 1;
    std::size_t resultSize = std::strlen(format) + std::strlen(assertion) + std::strlen(file) + maxLineNumLength;
    if (msg) {
        resultSize += std::strlen(msg);
    }
    char* result = (char*)alloca(resultSize);
    result[resultSize - 1] = '\0';
    int offset;
    if (msg) {
        offset = std::sprintf(result, format, file, line, assertion, msg);
    } else {
        offset = std::sprintf(result, format, file, line, assertion);
    }
    result[offset] = '\0';
    bmcl::panic(result);
}

BMCL_NORETURN void assertFail(const char* assertion, const char* file, int line)
{
    assertFail(assertion, 0, file, line);
}
}
