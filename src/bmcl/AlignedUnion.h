/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"
#include "bmcl/Utils.h"

#include <utility>

namespace bmcl {

template <class... T>
struct AlignedUnion {
public:
    template <typename C>
    enableIfOneOf<C, T...>& as();

    template <typename C>
    const enableIfOneOf<C, T...>& as() const;

    template <typename C>
    enableIfOneOf<C, T...>* asPtr();

    template <typename C>
    const enableIfOneOf<C, T...>* asPtr() const;

    template <typename C>
    void set(const enableIfOneOf<C, T...>& value);

    template <typename C>
    void set(enableIfOneOf<C, T...>&& value);

    template <typename C, typename... A>
    void emplace(A&&... args);

    template <typename C, typename = enableIfOneOf<C, T...>>
    void destruct();

private:
    template <typename C, typename... A>
    void emplaceImpl(enableIfOneOf<C, T...>* helper, A&&... args);
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

template <class... T>
template <typename C>
inline enableIfOneOf<C, T...>& AlignedUnion<T...>::as()
{
    return *reinterpret_cast<C*>(&_data);
}

template <class... T>
template <typename C>
inline const enableIfOneOf<C, T...>& AlignedUnion<T...>::as() const
{
    return *reinterpret_cast<const C*>(&_data);
}

template <class... T>
template <typename C>
inline enableIfOneOf<C, T...>* AlignedUnion<T...>::asPtr()
{
    return reinterpret_cast<C*>(&_data);
}

template <class... T>
template <typename C>
inline const enableIfOneOf<C, T...>* AlignedUnion<T...>::asPtr() const
{
    return reinterpret_cast<const C*>(&_data);
}

template <class... T>
template <typename C>
inline void AlignedUnion<T...>::set(const enableIfOneOf<C, T...>& value)
{
    new (asPtr<C>()) C(value);
}

template <class... T>
template <typename C>
inline void AlignedUnion<T...>::set(enableIfOneOf<C, T...>&& value)
{
    new (asPtr<C>()) C(std::move(value));
}

template <class... T>
template <typename C, typename... A>
inline void AlignedUnion<T...>::emplaceImpl(enableIfOneOf<C, T...>* helper, A&&... args)
{
    (void)helper;
    new (asPtr<C>()) C(std::forward<A>(args)...);
}

template <class... T>
template <typename C, typename... A>
inline void AlignedUnion<T...>::emplace(A&&... args)
{
    emplaceImpl<C>(nullptr, std::forward<A>(args)...);
}

template <class... T>
template <typename C, typename>
inline void AlignedUnion<T...>::destruct()
{
    as<C>().~C();
}
}
