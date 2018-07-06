#pragma once

#include "bmcl/Config.h"

#include <cstdint>

namespace bmcl {

inline std::uint8_t zigZagEncode(std::int8_t n)
{
    return (n << 1) ^ (n >> 7);
}

inline std::uint16_t zigZagEncode(std::int16_t n)
{
    return (n << 1) ^ (n >> 15);
}

inline std::uint32_t zigZagEncode(std::int32_t n)
{
    return (n << 1) ^ (n >> 31);
}

inline std::uint64_t zigZagEncode(std::int64_t n)
{
    return (n << 1) ^ (n >> 63);
}

inline std::int8_t zigZagDecode(std::uint8_t n)
{
    return (n >> 1) ^ -(std::int8_t)(n & 1);
}

inline std::int16_t zigZagDecode(std::uint16_t n)
{
    return (n >> 1) ^ -(std::int16_t)(n & 1);
}

inline std::int32_t zigZagDecode(std::uint32_t n)
{
    return (n >> 1) ^ -(std::int32_t)(n & 1);
}

inline std::int64_t zigZagDecode(std::uint64_t n)
{
    return (n >> 1) ^ -(std::int64_t)(n & 1);
}
}
