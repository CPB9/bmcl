/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/Bytes.h"

#include <string>
#include <vector>

namespace bmcl {

BMCL_EXPORT std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems);
BMCL_EXPORT std::vector<std::string> split(const std::string& s, char delim);

BMCL_EXPORT std::string bytesToHexStringLower(const uint8_t* data, std::size_t size);
BMCL_EXPORT std::string bytesToHexStringUpper(const uint8_t* data, std::size_t size);

inline std::string bytesToHexStringLower(bmcl::Bytes bytes)
{
    return bytesToHexStringLower(bytes.data(), bytes.size());
}

inline std::string bytesToHexStringUpper(bmcl::Bytes bytes)
{
    return bytesToHexStringUpper(bytes.data(), bytes.size());
}
}
