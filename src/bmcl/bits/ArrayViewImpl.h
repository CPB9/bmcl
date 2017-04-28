/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/bits/ArrayViewDecl.h"
#include "bmcl/bits/ArrayViewBaseImpl.h"

namespace bmcl {

template <typename T>
template <typename R>
ArrayView<T>::ArrayView(R view, typename std::enable_if<std::is_base_of<ArrayViewBase<T, R>, R>::value>::type* a)
    : ArrayViewBase<T, ArrayView>(view.data(), view.size())
{
    (void)a;
}

template <typename T>
template <typename... A>
ArrayView<T>::ArrayView(A&&... args)
    : ArrayViewBase<T, ArrayView>(std::forward<A>(args)...)
{
}

template <typename T>
ArrayView<T>::ArrayView(std::initializer_list<T> lst)
    : ArrayViewBase<T, ArrayView>(lst)
{
}

template <typename T, typename R>
inline bool operator==(const T& lhs, ArrayView<R> rhs)
{
    return ArrayView<R>(lhs).operator==(rhs);
}

template <typename T, typename R>
inline bool operator!=(const T& lhs, ArrayView<R> rhs)
{
    return ArrayView<R>(lhs).operator!=(rhs);
}
}
