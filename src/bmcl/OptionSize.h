/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/DefaultOption.h"

#include <cstddef>
#include <limits>

namespace bmcl {

using OptionSizeDescriptor = DefaultOptionDescriptor<std::size_t, std::size_t(-1)>;
using OptionSize = DefaultOption<std::size_t, OptionSizeDescriptor>;
}

