#include "bmcl/Varuint.h"
#include "bmcl/ZigZag.h"

namespace bmcl {

std::size_t varuintEncodedSize(std::uint64_t value)
{
    if (value <= 240) {
        return 1;
    }
    if (value <= 2287) {
        return 2;
    }
    if (value <= 67823) {
        return 3;
    }
    if (value <= 16777215) {
        return 4;
    }
    if (value <= 4294967295) {
        return 5;
    }
    if (value <= 1099511627775) {
        return 6;
    }
    if (value <= 281474976710655) {
        return 7;
    }
    if (value <= 72057594037927935) {
        return 8;
    }
    return 9;
}

std::size_t varintEncodedSize(std::int64_t value)
{
    return varuintEncodedSize(zigZagEncode(value));
}
}
