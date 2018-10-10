#pragma once

#include "bmcl/Config.h"

#include <utility>

namespace bmcl {

template <typename T>
bool allSome(T&& value)
{
    return value.isSome();
}

template <typename T, typename... A>
bool allSome(T&& head, A&&... tail)
{
    return head.isSome() && allSome(std::forward<A>(tail)...);
}

template <typename T>
bool anySome(T&& value)
{
    return value.isSome();
}

template <typename T, typename... A>
bool anySome(T&& head, A&&... tail)
{
    return head.isSome() || anySome(std::forward<A>(tail)...);
}

template <typename T>
bool allNone(T&& value)
{
    return value.isNone();
}

template <typename T, typename... A>
bool allNone(T&& head, A&&... tail)
{
    return head.isNone() && allNone(std::forward<A>(tail)...);
}

template <typename T>
bool anyNone(T&& value)
{
    return value.isNone();
}

template <typename T, typename... A>
bool anyNone(T&& head, A&&... tail)
{
    return head.isNone() || anyNone(std::forward<A>(tail)...);
}
}
