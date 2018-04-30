#include "bmcl/BitArray.h"

#include "BmclTest.h"

#include <gtest/gtest.h>

using namespace bmcl;

TEST(BitArray, dataSize)
{
    EXPECT_EQ(BitArray<7>::dataSizeInBytes(), 1);
    EXPECT_EQ(BitArray<8>::dataSizeInBytes(), 1);
    EXPECT_EQ(BitArray<9>::dataSizeInBytes(), 2);
    EXPECT_EQ(BitArray<13>::dataSizeInBytes(), 2);
    EXPECT_EQ(BitArray<15>::dataSizeInBytes(), 2);
    EXPECT_EQ(BitArray<16>::dataSizeInBytes(), 2);
    EXPECT_EQ(BitArray<17>::dataSizeInBytes(), 3);
    EXPECT_EQ(BitArray<1345>::dataSizeInBytes(), 169);
}

TEST(BitArray, init)
{
    constexpr std::size_t size = 45;
    BitArray<size> arr;
    for (std::size_t i = 0; i < size; i++) {
        ASSERT_FALSE(arr.isSet(i));
    }
}

TEST(BitArray, setAll)
{
    constexpr std::size_t size = 91;
    BitArray<size> arr;
    arr.setAll();
    for (std::size_t i = 0; i < size; i++) {
        ASSERT_TRUE(arr.isSet(i));
    }
}

TEST(BitArray, setClearAll)
{
    constexpr std::size_t size = 75;
    BitArray<size> arr;
    arr.setAll();
    for (std::size_t i = 0; i < size; i++) {
        ASSERT_TRUE(arr.isSet(i));
    }
    arr.clearAll();
    for (std::size_t i = 0; i < size; i++) {
        ASSERT_FALSE(arr.isSet(i));
    }
}

TEST(BitArray, set)
{
    constexpr std::size_t size = 13;
    BitArray<size> arr;
    arr.set(1);
    arr.set(6);
    arr.set(10);
    EXPECT_FALSE(arr.isSet(0));
    EXPECT_TRUE(arr.isSet(1));
    EXPECT_FALSE(arr.isSet(2));
    EXPECT_FALSE(arr.isSet(3));
    EXPECT_FALSE(arr.isSet(4));
    EXPECT_FALSE(arr.isSet(5));
    EXPECT_TRUE(arr.isSet(6));
    EXPECT_FALSE(arr.isSet(7));
    EXPECT_FALSE(arr.isSet(8));
    EXPECT_FALSE(arr.isSet(9));
    EXPECT_TRUE(arr.isSet(10));
    EXPECT_FALSE(arr.isSet(11));
    EXPECT_FALSE(arr.isSet(12));
}

TEST(BitArray, setClear)
{
    constexpr std::size_t size = 4;
    BitArray<size> arr;
    arr.set(0);
    arr.set(1);
    arr.set(2);
    arr.set(3);
    EXPECT_TRUE(arr.isSet(0));
    EXPECT_TRUE(arr.isSet(1));
    EXPECT_TRUE(arr.isSet(2));
    EXPECT_TRUE(arr.isSet(3));
    arr.clear(1);
    EXPECT_TRUE(arr.isSet(0));
    EXPECT_FALSE(arr.isSet(1));
    EXPECT_TRUE(arr.isSet(2));
    EXPECT_TRUE(arr.isSet(3));
}

TEST(BitArray, setClearTwoTimes)
{
    constexpr std::size_t size = 4;
    BitArray<size> arr;
    arr.set(1);
    arr.set(1);
    EXPECT_FALSE(arr.isSet(0));
    EXPECT_TRUE(arr.isSet(1));
    EXPECT_FALSE(arr.isSet(2));
    EXPECT_FALSE(arr.isSet(3));
    arr.clear(1);
    arr.clear(1);
    EXPECT_FALSE(arr.isSet(0));
    EXPECT_FALSE(arr.isSet(1));
    EXPECT_FALSE(arr.isSet(2));
    EXPECT_FALSE(arr.isSet(3));
}

TEST(BitArray, assignAfterClear)
{
    constexpr std::size_t size = 5;
    BitArray<size> arr;
    arr.assign(0, true);
    arr.assign(1, false);
    arr.assign(2, true);
    arr.assign(3, false);
    arr.assign(4, true);
    EXPECT_TRUE(arr.isSet(0));
    EXPECT_FALSE(arr.isSet(1));
    EXPECT_TRUE(arr.isSet(2));
    EXPECT_FALSE(arr.isSet(3));
    EXPECT_TRUE(arr.isSet(4));
}

TEST(BitArray, assignAfterSet)
{
    constexpr std::size_t size = 5;
    BitArray<size> arr;
    arr.setAll();
    arr.assign(1, false);
    arr.assign(2, true);
    arr.assign(3, false);
    arr.assign(4, true);
    EXPECT_TRUE(arr.isSet(0));
    EXPECT_FALSE(arr.isSet(1));
    EXPECT_TRUE(arr.isSet(2));
    EXPECT_FALSE(arr.isSet(3));
    EXPECT_TRUE(arr.isSet(4));
}

TEST(BitArray, setAllIndividually)
{
    constexpr std::size_t size = 80;
    BitArray<size> arr;
    for (std::size_t i = 0; i < size; i++) {
        arr.set(i);
    }
    for (std::size_t i = 0; i < size; i++) {
        ASSERT_TRUE(arr.isSet(i));
    }
}

TEST(BitArray, setClearAllIndividually)
{
    constexpr std::size_t size = 45;
    BitArray<size> arr;
    for (std::size_t i = 0; i < size; i++) {
        arr.set(i);
    }
    for (std::size_t i = 0; i < size; i++) {
        ASSERT_TRUE(arr.isSet(i));
    }
    for (std::size_t i = 0; i < size; i++) {
        arr.clear(i);
    }
    for (std::size_t i = 0; i < size; i++) {
        ASSERT_FALSE(arr.isSet(i));
    }
}
