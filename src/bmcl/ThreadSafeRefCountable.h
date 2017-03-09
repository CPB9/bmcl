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

namespace bmcl {

template <typename T>
class ThreadSafeRefCountable {
public:
    ThreadSafeRefCountable()
        : _rc(0)
    {
    }

    virtual ~ThreadSafeRefCountable()
    {
    }

private:
    friend void bmclRcAddRef(bmcl::ThreadSafeRefCountable<T>* rc)
    {
        rc->_rc.fetch_add(1, std::memory_order_relaxed);
    }

    friend void bmclRcRelease(bmcl::ThreadSafeRefCountable<T>* rc)
    {
        if (rc->_rc.fetch_sub(1, std::memory_order_release) == 1) {
            std::atomic_thread_fence(std::memory_order_acquire);
            delete rc;
        }
    }

    std::atomic<T> _rc;
};
}
