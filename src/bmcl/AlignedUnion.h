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

namespace bmcl {

template <class... T>
struct AlignedUnion {
private:
#ifdef _MSC_VER
    typename std::aligned_union<0, T...>::type _data;
#else
    template <typename S>
    static constexpr S max(S t)
    {
        return t;
    }

    template <typename S, typename... A>
    static constexpr S max(S t, A... args)
    {
        return t > max(args...) ? t : max(args...);
    }

    alignas(max(alignof(T)...)) char _data[max(sizeof(T)...)];
#endif
};
}
