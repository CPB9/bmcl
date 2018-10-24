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
#include "bmcl/Fwd.h"

#include <atomic>
#include <cstddef>

namespace bmcl {

class BMCL_EXPORT ThreadSafeRefCountableBase {
public:
    ThreadSafeRefCountableBase();
    virtual ~ThreadSafeRefCountableBase();

protected:
    BMCL_EXPORT friend void bmclRcAddRef(const bmcl::ThreadSafeRefCountableBase* rc);
    BMCL_EXPORT friend void bmclRcRelease(const bmcl::ThreadSafeRefCountableBase* rc);

private:
    mutable std::atomic<std::size_t> _rc;
};

BMCL_EXPORT void bmclRcAddRef(const bmcl::ThreadSafeRefCountableBase* rc);
BMCL_EXPORT void bmclRcRelease(const bmcl::ThreadSafeRefCountableBase* rc);
}
