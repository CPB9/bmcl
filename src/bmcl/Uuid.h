#pragma once

#include "bmcl/Config.h"

#include <array>
#include <cstdint>

namespace bmcl {

class Uuid {
public:
    using Data = std::array<std::uint8_t, 16>;

    Uuid(const Uuid& other) = default;
    Uuid& operator=(const Uuid& other) = default;

    static Uuid create();

    bool operator==(const Uuid& other) const;
    bool operator!=(const Uuid& other) const;
    bool operator<(const Uuid& other) const;
    bool operator<=(const Uuid& other) const;
    bool operator>(const Uuid& other) const;
    bool operator>=(const Uuid& other) const;

private:
    Uuid();

    Data _data;
};

inline Uuid::Uuid()
{
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
