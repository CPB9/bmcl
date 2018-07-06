/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/Fwd.h"

#include <string>
#include <cstdint>

namespace bmcl {

template <typename T, typename E>
class Result;

class Buffer;
class SharedBytes;

BMCL_EXPORT Result<std::string, int> readFileIntoString(const char* path);
BMCL_EXPORT Result<Buffer, int> readFileIntoBuffer(const char* path);
BMCL_EXPORT Result<SharedBytes, int> readFileIntoBytes(const char* path);

BMCL_EXPORT const bmcl::Option<std::string>& applicationFilePath();
BMCL_EXPORT const bmcl::Option<std::string>& applicationDirPath();
BMCL_EXPORT std::uint64_t applicationPid();
}
