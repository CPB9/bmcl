#include "bmcl/String.h"

#include "BmclTest.h"

#include <gtest/gtest.h>

using namespace bmcl;

TEST(BytesToHex, lowerEmpty)
{
    uint8_t data[] = {1};
    std::string expected = "";
    EXPECT_EQ(expected, bytesToHexStringLower(data, 0));
}

TEST(BytesToHex, lower)
{
    uint8_t data[] = {0xa0, 0xb1, 0xc2, 0xd3, 0xe4, 0xf5, 0x67, 0x98};
    std::string expected = "a0b1c2d3e4f56798";
    EXPECT_EQ(expected, bytesToHexStringLower(data, 8));
}

TEST(BytesToHex, upperEmpty)
{
    uint8_t data[] = {1};
    std::string expected = "";
    EXPECT_EQ(expected, bytesToHexStringUpper(data, 0));
}

TEST(BytesToHex, upper)
{
    uint8_t data[] = {0xfa, 0xdc, 0xbe, 0x09, 0x12, 0x54, 0x36, 0x87, 0xa6};
    std::string expected = "FADCBE0912543687A6";
    EXPECT_EQ(expected, bytesToHexStringUpper(data, 9));
}

