/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"

#define BMCL_STRINGIFY(expr) #expr

#if BMCL_NO_ASSERTS
#define BMCL_ASSERT(expr)
#else
#define BMCL_ASSERT(expr) ((expr) ? (void)0 : bmcl::assertFail(BMCL_STRINGIFY(expr), __FILE__, __LINE__))
#endif

namespace bmcl {

BMCL_NORETURN void assertFail(const char* msg, const char* file, int line);
};
