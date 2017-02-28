/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#if defined(_MSC_VER) || defined(__MINGW32__)
#include <malloc.h>
#if !defined(alloca)
#define alloca _alloca
#endif
#else
#include <alloca.h>
#endif
