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
#include <cstdint>

namespace bmcl {

template <typename T>
class BMCL_EXPORT ThreadSafeRefCountable {
public:
    ThreadSafeRefCountable()
        : _rc(0)
    {
    }

    virtual ~ThreadSafeRefCountable()
    {
    }

private:
    friend void bmclRcAddRef(const bmcl::ThreadSafeRefCountable<T>* rc)
    {
        rc->_rc.fetch_add(1, std::memory_order_relaxed);
    }

    friend void bmclRcRelease(const bmcl::ThreadSafeRefCountable<T>* rc)
    {
        if (rc->_rc.fetch_sub(1, std::memory_order_release) == 1) {
            std::atomic_thread_fence(std::memory_order_acquire);
            delete rc;
        }
    }

    mutable std::atomic<T> _rc;
};

extern template class ThreadSafeRefCountable<std::uint8_t>;
extern template class ThreadSafeRefCountable<std::uint16_t>;
extern template class ThreadSafeRefCountable<std::uint32_t>;
extern template class ThreadSafeRefCountable<std::uint64_t>;
}
