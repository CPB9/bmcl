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
#include <memory>

namespace bmcl {

template<class B, class T = B>
using enableIfVoid = typename std::enable_if<std::is_void<B>::value, T>::type;

template<class B, class T = B>
using enableIfNotVoid = typename std::enable_if<!std::is_void<B>::value, T>::type;


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
}
