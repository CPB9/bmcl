/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"

#include <utility>
#include <type_traits>
#include <memory>

namespace bmcl {

template <typename T, typename... Ts>
struct ContainsType
    : std::true_type {};

template <typename T>
struct ContainsType<T>
    : std::false_type {};

template <typename T1, typename T2, typename... Ts>
struct ContainsType<T1, T2, Ts...>
    : std::conditional<std::is_same<T1, T2>::value, std::true_type, ContainsType<T1, Ts...>>::type {};

template<class B, class T = B>
using enableIfVoid = typename std::enable_if<std::is_void<B>::value, T>::type;

template<class B, class T = B>
using enableIfNotVoid = typename std::enable_if<!std::is_void<B>::value, T>::type;

template<typename T, typename... Ts>
using enableIfOneOf = typename std::enable_if<ContainsType<T, Ts...>::value, T>::type;

struct NoneType {
    inline NoneType()
    {
    }
};

const NoneType None;

struct InPlaceType {
    inline InPlaceType()
    {
    }
};

struct InPlaceFirstType {
    inline InPlaceFirstType()
    {
    }
};

struct InPlaceSecondType {
    inline InPlaceSecondType()
    {
    }
};

const InPlaceType InPlace;
const InPlaceFirstType InPlaceFirst;
const InPlaceSecondType InPlaceSecond;

template <typename T, typename... A>
inline std::unique_ptr<T> makeUnique(A&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<A>(args)...));
}

template <typename T>
bool allNull(T&& value)
{
    return value.isNull();
}

template <typename T, typename... A>
bool allNull(T&& head, A&&... tail)
{
    return head.isNull() && allNull(std::forward<A>(tail)...);
}

template <typename T>
bool anyNull(T&& value)
{
    return value.isNull();
}

template <typename T, typename... A>
bool anyNull(T&& head, A&&... tail)
{
    return head.isNull() || anyNull(std::forward<A>(tail)...);
}
}
