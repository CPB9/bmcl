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

BMCL_NORETURN void assertFail(const char* msg, const char* file, int line)
{
    const char* format = "Assertion failed at %s:%d with message: %s\0";
    std::size_t maxLineNumLength = int(std::numeric_limits<int>::digits10) + 1;
    std::size_t resultSize = sizeof(format) + std::strlen(msg) + std::strlen(file) + maxLineNumLength;
    char* result = (char*)alloca(resultSize);
    result[resultSize - 1] = '\0';
    int offset = std::sprintf(result, format, file, line, msg);
    result[offset] = '\0';
    bmcl::panic(result);
    std::abort();
}
}
