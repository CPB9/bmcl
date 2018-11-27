/*
 * Copyright (c) 2014-2015 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstddef>

#include "bmcl/Config.h"
#include "bmcl/DefaultOption.h"

namespace bmcl {

using OptionPtrDescriptor = DefaultOptionDescriptor<std::nullptr_t, nullptr>;

template <typename T>
using OptionPtr = DefaultOption<T*, OptionPtrDescriptor>;
}
