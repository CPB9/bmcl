#include "bmcl/SharedBytes.h"

#include "BmclTest.h"

#include <gtest/gtest.h>

using namespace bmcl;

TEST(OwnedData, create)
{
    uint8_t expected[] = {1, 2, 3, 4};
    SharedBytes data = SharedBytes::create(expected, 4);
    EXPECT_EQ_MEM(expected, data.data(), 4);
}

TEST(OwnedData, createSeveral)
{
    uint8_t expected[] = {1, 2, 3, 4};
    SharedBytes data = SharedBytes::create(expected, 4);
    SharedBytes data2 = data;
    SharedBytes data3 = data2;
    data = data3;
    EXPECT_EQ_MEM(expected, data.data(), 4);
    EXPECT_EQ_MEM(expected, data2.data(), 4);
    EXPECT_EQ_MEM(expected, data3.data(), 4);
}
