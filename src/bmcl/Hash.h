/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"

#include <cstdint>

namespace bmcl {

template <typename R>
struct FnvHashParams;

template <>
struct FnvHashParams<std::uint32_t>
{
    static constexpr std::uint32_t prime = 16777619u;
    static constexpr std::uint32_t offsetBias = 2166136261u;
};

template <>
struct FnvHashParams<std::uint64_t>
{
    static constexpr std::uint64_t prime = 1099511628211u;
    static constexpr std::uint64_t offsetBias = 14695981039346656037u;
};

template <typename R, typename T>
constexpr R fnv1aHash(const T* data, std::size_t size, R value = FnvHashParams<R>::offsetBias)
{
    return (size == 0) ? value : fnv1aHash<R, T>(data + 1, size - 1, (value ^ R(*data)) * FnvHashParams<R>::prime);
}

template <typename R>
constexpr R fnv1aHashCString(const char* data, R value = FnvHashParams<R>::offsetBias)
{
    return (*data == '\0') ? value : fnv1aHashCString<R>(data + 1, (value ^ R(*data)) * FnvHashParams<R>::prime);
}
}
