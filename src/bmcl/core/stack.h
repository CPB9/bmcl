/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstddef>

namespace bmcl {
namespace core {

class Stack {
public:
    virtual ~Stack() {}

    virtual void pop(void* dest, std::size_t size) = 0;
    virtual void push(const void* src, std::size_t size) = 0;

    template <typename T>
    inline T popType()
    {
        T value;
        pop(&value, sizeof(value));
        return value;
    }

    template <typename T>
    inline void pushType(T value)
    {
        push(&value, sizeof(value));
    }
};
}
}
