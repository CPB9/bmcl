/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/Rc.h"

#include <atomic>
#include <cstddef>

namespace bmcl {

class BMCL_EXPORT ThreadSafeRefCountableBase {
public:
    ThreadSafeRefCountableBase();
    virtual ~ThreadSafeRefCountableBase();

private:
    friend void bmclRcAddRef(const bmcl::ThreadSafeRefCountableBase* rc);
    friend void bmclRcRelease(const bmcl::ThreadSafeRefCountableBase* rc);

    mutable std::atomic<std::size_t> _rc;
};

void bmclRcAddRef(const bmcl::ThreadSafeRefCountableBase* rc);
void bmclRcRelease(const bmcl::ThreadSafeRefCountableBase* rc);

//HACK
template <typename T>
using ThreadSafeRefCountable = ThreadSafeRefCountableBase;
}
