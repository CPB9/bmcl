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

namespace bmcl {

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
    std::abort();
}
}
