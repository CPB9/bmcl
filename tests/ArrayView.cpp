#include "bmcl/ArrayView.h"
#include "bmcl/StringView.h"
#include "bmcl/Buffer.h"

#include "BmclTest.h"

#include <gtest/gtest.h>

using namespace bmcl;

template <typename T>
static void expectArrayView(ArrayView<T> ref, std::initializer_list<T> lst)
{
    ASSERT_EQ(lst.size(), ref.size());
    EXPECT_TRUE(std::equal(ref.begin(), ref.end(), lst.begin()));
}

TEST(ArrayView, fromStaticArray)
{
    int array[5] = {1, 2, 3, 4, 5};
    auto ref = ArrayView<int>::fromStaticArray(array);
    expectArrayView(ref, {1, 2, 3, 4, 5});
}

TEST(ArrayView, fromStdArray)
{
    std::array<int, 3> array = {{3, 4, 5}};
    ArrayView<int> ref(array);
    expectArrayView(ref, {3, 4, 5});
}

TEST(ArrayView, fromFixedArrayView)
{
    FixedArrayView<int, 3> view = {{3, 4, 5}};
    ArrayView<int> ref(view);
    expectArrayView(ref, {3, 4, 5});
}

TEST(ArrayView, fromBuffer)
{
    Buffer buf;
    buf.writeUint8(9);
    buf.writeUint8(8);
    buf.writeUint8(7);
    ArrayView<uint8_t> ref(buf);
    expectArrayView<uint8_t>(ref, {9, 8, 7});
    EXPECT_EQ(buf.data(), ref.data());
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

TEST(ArrayView, fromStringView)
{
    StringView strView("qwer");
    ArrayView<char> ref(strView);
    expectArrayView(ref, {'q', 'w', 'e', 'r'});
}

TEST(ArrayView, empty)
{
    auto view = ArrayView<uint16_t>::empty();
    EXPECT_EQ(0, view.size());
    EXPECT_TRUE(view.isEmpty());
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
    ArrayView<int> ref = {1};
    EXPECT_FALSE(ref.isEmpty());
}

TEST(ArrayView, toStdVector)
{
    uint8_t array[5] = {5, 4, 3, 2, 1};
    auto ref = ArrayView<uint8_t>::fromStaticArray(array);
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

TEST(ArrayView, sliceFromBack)
{
    ArrayView<int> ref{6, 5, 4, 3, 2, 1};
    expectArrayView(ref.sliceFromBack(0), {6, 5, 4, 3, 2, 1});
    expectArrayView(ref.sliceFromBack(2), {6, 5, 4, 3});
    EXPECT_TRUE(ref.sliceFromBack(6).isEmpty());
}

TEST(ArrayView, assign)
{
    ArrayView<char> ref = ArrayView<char>::empty();
    ref = {'a', 's', 'd'};
    expectArrayView(ref, {'a', 's', 'd'});

    std::vector<char> vec{'t', 's', 'd'};
    ref = vec;
    expectArrayView(ref, {'t', 's', 'd'});


    std::array<char, 4> arr{{'t', 'r', 'd', 'f'}};
    ref = arr;
    expectArrayView(ref, {'t', 'r', 'd', 'f'});
}
