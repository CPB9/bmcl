#pragma once

#include "bmcl/Config.h"

#include <chrono>

namespace bmcl {

using SystemClock = std::chrono::system_clock;
using SystemTime = SystemClock::time_point;

template <typename T>
std::chrono::seconds toSecs(const T& timePoint)
{
    return std::chrono::duration_cast<std::chrono::seconds>(timePoint);
}

template <typename T>
std::chrono::milliseconds toMsecs(const T& timePoint)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(timePoint);
}

template <typename T>
std::chrono::microseconds toUsecs(const T& timePoint)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(timePoint);
}

template <typename T>
std::chrono::nanoseconds toNsecs(const T& timePoint)
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(timePoint);
}
};
