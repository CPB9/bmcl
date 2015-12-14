#include "bmcl/ArrayView.h"

#include "BmclTest.h"

#include <gtest/gtest.h>

using namespace bmcl;

template <typename T>
static void expectArrayView(const ArrayView<T>& ref, const std::initializer_list<T>& lst)
{
    ASSERT_EQ(lst.size(), ref.size());
    EXPECT_TRUE(std::equal(ref.begin(), ref.end(), lst.begin()));
}

TEST(ArrayView, fromStaticArray)
{
    int array[5] = {1, 2, 3, 4, 5};
    ArrayView<int> ref(array);
    expectArrayView(ref, {1, 2, 3, 4, 5});
}

TEST(ArrayView, fromDataSize)
{
    int array[4] = {2, 3, 4, 5};
    ArrayView<int> ref(&array[0], 4);
    expectArrayView(ref, {2, 3, 4, 5});
}

TEST(ArrayView, fromStartEnd)
{
    int array[3] = {3, 4, 5};
    ArrayView<int> ref(&array[0], &array[0] + 3);
    expectArrayView(ref, {3, 4, 5});
}

TEST(ArrayView, fromStdVector)
{
    std::vector<std::string> vec{"test", "asd", "dsa"};
    ArrayView<std::string> ref(vec);
    expectArrayView(ref, std::initializer_list<std::string>{"test", "asd", "dsa"});
}

TEST(ArrayView, fromStdInitializerList)
{
    std::initializer_list<std::string> lst{"test", "asd", "dsa"};
    ArrayView<std::string> ref(lst);
    expectArrayView(ref, std::initializer_list<std::string>{"test", "asd", "dsa"});
}

TEST(ArrayView, fromStdArray)
{
    std::array<int, 3> array = {{3, 4, 5}};
    ArrayView<int> ref(array);
    expectArrayView(ref, {3, 4, 5});
}

TEST(ArrayView, iterator)
{
    ArrayView<int> ref{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int expected = 0;
    for (int i : ref) {
        ASSERT_EQ(expected, i);
        expected++;
    }
}

TEST(ArrayView, reverseIterator)
{
    ArrayView<int> ref{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int expected = 10;
    for (ArrayView<int>::reverse_iterator i = ref.rbegin(); i < ref.rend(); i++) {
        ASSERT_EQ(expected, *i);
        expected--;
    }
}

TEST(ArrayView, isEmpty)
{
    int* value = nullptr;
    ArrayView<int> ref(value, std::size_t(0));
    EXPECT_TRUE(ref.isEmpty());
}

TEST(ArrayView, isNotEmpty)
{
    int array[1] = {1};
    ArrayView<int> ref(array);
    EXPECT_FALSE(ref.isEmpty());
}

TEST(ArrayView, toStdVector)
{
    uint8_t array[5] = {5, 4, 3, 2, 1};
    ArrayView<uint8_t> ref(array);
    std::vector<uint8_t> vec = ref.toStdVector();
    ASSERT_EQ(5, vec.size());
    EXPECT_TRUE(std::equal(vec.begin(), vec.end(), &array[0]));
}

TEST(ArrayView, subscriptOperator)
{
    ArrayView<uint8_t> ref{3, 2, 1};
    EXPECT_EQ(3, ref[0]);
    EXPECT_EQ(2, ref[1]);
    EXPECT_EQ(1, ref[2]);
}

TEST(ArrayView, sliceFrom)
{
    ArrayView<int> ref{6, 5, 4, 3, 2, 1};
    expectArrayView(ref.sliceFrom(0), {6, 5, 4, 3, 2, 1});
    expectArrayView(ref.sliceFrom(2), {4, 3, 2, 1});
    EXPECT_TRUE(ref.sliceFrom(6).isEmpty());
}

TEST(ArrayView, sliceTo)
{
    ArrayView<int> ref{6, 5, 4, 3, 2, 1};
    expectArrayView(ref.sliceTo(6), {6, 5, 4, 3, 2, 1});
    expectArrayView(ref.sliceTo(3), {6, 5, 4});
    EXPECT_TRUE(ref.sliceTo(0).isEmpty());
}

TEST(ArrayView, slice)
{
    ArrayView<int> ref{6, 5, 4, 3, 2, 1};
    expectArrayView(ref.slice(0, 6), {6, 5, 4, 3, 2, 1});
    expectArrayView(ref.slice(1, 5), {5, 4, 3, 2});
    EXPECT_TRUE(ref.slice(3, 3).isEmpty());
}
