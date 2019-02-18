#include "bmcl/SharedBytes.h"

#include "BmclTest.h"

#include <gtest/gtest.h>

using namespace bmcl;

TEST(OwnedData, createNull)
{
    SharedBytes data;
    EXPECT_EQ(0, data.size());
    EXPECT_TRUE(data.isEmpty());
    EXPECT_TRUE(data.isNull());
}

TEST(OwnedData, createEmpty)
{
    uint8_t expected[] = {1, 2, 3, 4};
    SharedBytes data = SharedBytes::create(expected, 0);
    EXPECT_EQ(0, data.size());
    EXPECT_TRUE(data.isEmpty());
    EXPECT_FALSE(data.isNull());
}

TEST(OwnedData, createOne)
{
    uint8_t expected[] = {1, 2, 3, 4};
    SharedBytes data = SharedBytes::create(expected, 4);
    EXPECT_EQ(4, data.size());
    EXPECT_EQ_MEM(expected, data.data(), 4);
    EXPECT_FALSE(data.isEmpty());
    EXPECT_FALSE(data.isNull());
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

    SharedBytes data4 = std::move(data2);
    EXPECT_EQ_MEM(expected, data4.data(), 4);
    EXPECT_TRUE(data2.isNull());
}
