/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"

#include <iterator>
#include <array>
#include <cstddef>

namespace bmcl {

template <typename T, size_t S>
class FixedArrayView {
public:
    typedef std::size_t size_type;
    typedef const T* iterator;
    typedef const T* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;

    FixedArrayView(const std::array<T, S>& lst);

    static FixedArrayView<T, S> fromStaticArray(const T(&data)[S]);
    static FixedArrayView<T, S> fromRawData(const T* data);

    iterator begin() const;
    iterator end() const;

    reverse_iterator rbegin() const;
    reverse_iterator rend() const;

    const T* data() const;
    std::size_t size() const;
    bool isEmpty() const;

    const T& operator[](std::size_t index) const;

private:
    FixedArrayView(const T* data);

    const T* _data;
};
}
