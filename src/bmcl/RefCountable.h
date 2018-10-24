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
    RefCountable()
        : _rc(0)
    {
    }

    virtual ~RefCountable()
    {
    }

protected:
    friend void bmclRcAddRef(const bmcl::RefCountable<T>* rc)
    {
        rc->_rc++;
    }

    friend void bmclRcRelease(const bmcl::RefCountable<T>* rc)
    {
        rc->_rc--;
        if (rc->_rc == 0) {
            delete rc;
        }
    }

private:
    mutable T _rc;
};
}

