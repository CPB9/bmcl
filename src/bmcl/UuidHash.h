/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/Hash.h"
#include "bmcl/Uuid.h"

namespace std {

template<>
struct hash<bmcl::Uuid>
{
    std::size_t operator()(const bmcl::Uuid& p) const
    {
        return bmcl::fnv1aHash<std::size_t>(p.data().data(), p.data().size());
    }
};
}
