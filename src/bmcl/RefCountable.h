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

namespace bmcl {

template <typename T>
class RefCountable {
public:
    RefCountable();
    virtual ~RefCountable();

    T refCount() const;

    void incref();
    T decref();
private:
    T _rc;
};

template <typename T>
inline RefCountable<T>::RefCountable()
    : _rc(0)
{
}

template <typename T>
inline RefCountable<T>::~RefCountable()
{
}

template <typename T>
inline T RefCountable<T>::refCount() const
{
    return _rc;
}

template <typename T>
inline T RefCountable<T>::decref()
{
    return --_rc;
}

template <typename T>
inline void RefCountable<T>::incref()
{
    ++_rc;
}

template <typename T>
inline void bmclRcAddRef(bmcl::RefCountable<T>* rc)
{
    rc->incref();
}

template <typename T>
inline void bmclRcRelease(bmcl::RefCountable<T>* rc)
{
    if (rc->decref() == 0) {
        delete rc;
    }
}
}

