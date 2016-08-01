/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "bmcl/Assert.h"

namespace bmcl
{

template<class T>
class NonNullUniquePtr
{
public:

    NonNullUniquePtr(NonNullUniquePtr && other)
            : _ptr(std::move(other._ptr)) { BMCL_ASSERT(_ptr); }

    NonNullUniquePtr(std::unique_ptr<T> && ptr)
            : _ptr(std::move(ptr)) { BMCL_ASSERT(_ptr); }

    NonNullUniquePtr(T * ptr)
            : _ptr(ptr) { BMCL_ASSERT(_ptr); }

    NonNullUniquePtr & operator=(NonNullUniquePtr && other)
    {
        BMCL_ASSERT(other);
        _ptr = std::move(other._ptr);
    }

    T* operator->() { BMCL_ASSERT(_ptr); return _ptr.get(); }

    const T* operator->() const { BMCL_ASSERT(_ptr); return _ptr.get(); }

    T & operator*() { BMCL_ASSERT(_ptr); return *_ptr; }

    const T & operator*() const { BMCL_ASSERT(_ptr); return *_ptr; }

    operator bool() const { return static_cast<bool>(_ptr); }

    T * get() { BMCL_ASSERT(_ptr); return _ptr.get(); }

    const T * get() const { BMCL_ASSERT(_ptr); return _ptr.get(); }

    std::unique_ptr<T> & ptr() { BMCL_ASSERT(_ptr); return _ptr; }

private:

    std::unique_ptr<T> _ptr;

};

}
