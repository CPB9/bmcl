/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/Config.h"

#include <array>
#include <cstdint>

namespace bmcl {

class BMCL_EXPORT Uuid {
public:
    using Data = std::array<std::uint8_t, 16>;

    Uuid(const Uuid& other) = default;
    Uuid& operator=(const Uuid& other) = default;

    static Uuid create();

    const Data& data() const;

    bool operator==(const Uuid& other) const;
    bool operator!=(const Uuid& other) const;
    bool operator<(const Uuid& other) const;
    bool operator<=(const Uuid& other) const;
    bool operator>(const Uuid& other) const;
    bool operator>=(const Uuid& other) const;

private:
    Uuid() = default;

    Data _data;
};

inline const Uuid::Data& Uuid::data() const
{
    return _data;
}

inline bool Uuid::operator==(const Uuid& other) const
{
    return _data == other._data;
}

inline bool Uuid::operator!=(const Uuid& other) const
{
    return _data != other._data;
}

inline bool Uuid::operator<(const Uuid& other) const
{
    return _data < other._data;
}

inline bool Uuid::operator<=(const Uuid& other) const
{
    return _data <= other._data;
}

inline bool Uuid::operator>(const Uuid& other) const
{
    return _data > other._data;
}

inline bool Uuid::operator>=(const Uuid& other) const
{
    return _data >= other._data;
}
}
