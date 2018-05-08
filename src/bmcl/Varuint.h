#pragma once

#include "bmcl/Config.h"

#include <cstdint>
#include <cstddef>

namespace bmcl {

std::size_t varuintEncodedSize(std::uint64_t value);
std::size_t varintEncodedSize(std::int64_t value);
}
