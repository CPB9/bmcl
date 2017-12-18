/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/bits/FixedArrayViewDecl.h"
#include "bmcl/Assert.h"

#include <cassert>

namespace bmcl {

template <typename T, size_t S>
inline FixedArrayView<T, S>::FixedArrayView(const std::array<T, S>& lst)
    : _data(lst.data())
{
}

template <typename T, size_t S>
constexpr inline FixedArrayView<T, S>::FixedArrayView(const T* data)
    : _data(data)
{
}

template <typename T, size_t S>
constexpr inline FixedArrayView<T, S> FixedArrayView<T, S>::fromRawData(const T* data)
{
    return FixedArrayView<T, S>(data);
}

template <typename T, size_t S>
constexpr inline FixedArrayView<T, S> FixedArrayView<T, S>::fromStaticArray(const T(&data)[S])
{
    return FixedArrayView<T, S>(data);
}

template <typename T, size_t S>
constexpr inline typename FixedArrayView<T, S>::iterator FixedArrayView<T, S>::begin() const
{
    return _data;
}

template <typename T, size_t S>
constexpr inline typename FixedArrayView<T, S>::iterator FixedArrayView<T, S>::end() const
{
    return _data + S;
}

template <typename T, size_t S>
constexpr inline typename FixedArrayView<T, S>::reverse_iterator FixedArrayView<T, S>::rbegin() const
{
    return reverse_iterator(rend());
}

template <typename T, size_t S>
constexpr inline typename FixedArrayView<T, S>::reverse_iterator FixedArrayView<T, S>::rend() const
{
    return reverse_iterator(rbegin());
}

template <typename T, size_t S>
constexpr inline const T* FixedArrayView<T, S>::data() const
{
    return _data;
}

template <typename T, size_t S>
constexpr inline std::size_t FixedArrayView<T, S>::size() const
{
    return S;
}

template <typename T, size_t S>
constexpr inline bool FixedArrayView<T, S>::isEmpty() const
{
    return S == 0;
}

template <typename T, size_t S>
inline const T& FixedArrayView<T, S>::operator[](std::size_t index) const
{
    BMCL_ASSERT(index < S);
    return _data[index];
}
}
