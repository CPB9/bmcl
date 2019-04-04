#include "bmcl/StringView.h"
#include "bmcl/ArrayView.h"
#include "bmcl/FixedArrayView.h"
#include "bmcl/OptionSize.h"

#include "BmclTest.h"

#include <gtest/gtest.h>

#include <array>

using namespace bmcl;

static void expectStringView(StringView ref, const char* data, std::size_t size)
{
    ASSERT_EQ(size, ref.size());
    EXPECT_EQ_MEM(data, ref.data(), size);
}

static void expectStringView(StringView ref, const char* data)
{
    expectStringView(ref, data, std::strlen(data));
}

TEST(StringView, fromNothing)
{
    StringView ref;
    expectStringView(ref, "");
}

TEST(StringView, fromNullptr)
{
    StringView ref(nullptr);
    expectStringView(ref, "");
}

TEST(StringView, fromCStr)
{
    StringView ref("asd");
    expectStringView(ref, "asd");
}

TEST(StringView, fromDataSize)
{
    StringView ref("test123", 4);
    expectStringView(ref, "test");
}

TEST(StringView, fromStrString)
{
    std::string data = "test134";
    StringView ref(data);
    expectStringView(ref, "test134");
}

TEST(StringView, fromArrayView)
{
    static std::array<char, 3> arr = { 'a', 'b', 'k' };
    ArrayView<char> data = arr;
    StringView ref(data);
    expectStringView(ref, "abk");
}

TEST(StringView, fromFixedArrayView)
{
    static char arr[3] = {'a', 'b', 's'};
    auto data = FixedArrayView<char, 3>::fromStaticArray(arr);
    StringView ref(data);
    expectStringView(ref, "abs");
}

TEST(StringView, asBytes)
{
    StringView ref("bytes");
    Bytes bytes = ref.asBytes();
    ASSERT_EQ(5, bytes.size());
    EXPECT_EQ_MEM("bytes", bytes.data(), 5);
}

TEST(StringView, toStdString)
{
    StringView ref("string");
    std::string str = ref.toStdString();
    ASSERT_EQ(6, str.size());
    EXPECT_STREQ("string", str.c_str());
}

TEST(StringView, toUpper)
{
    ASSERT_EQ("ASD", StringView("asd").toUpper());
    ASSERT_EQ("A1S2D3", StringView("a1s2d3").toUpper());
    ASSERT_EQ(",A.1 S:2;D/3-", StringView(",A.1 s:2;d/3-").toUpper());
}

TEST(StringView, toLower)
{
    ASSERT_EQ("asd", StringView("ASD").toLower());
    ASSERT_EQ("a1s2d3", StringView("A1S2D3").toLower());
    ASSERT_EQ(",a.1 s:2;d/3-", StringView(",a.1 S:2;D/3-").toLower());
}

TEST(StringView, startsWith)
{
    StringView ref("test123");
    EXPECT_TRUE(ref.startsWith(""));
    EXPECT_TRUE(ref.startsWith("test"));
    EXPECT_TRUE(ref.startsWith("test123"));
    EXPECT_FALSE(ref.startsWith("test1234"));
    EXPECT_FALSE(ref.startsWith("1234"));
    EXPECT_FALSE(ref.startsWith("est12"));
}

TEST(StringView, endsWith)
{
    StringView ref("test123");
    EXPECT_TRUE(ref.endsWith(""));
    EXPECT_TRUE(ref.endsWith("123"));
    EXPECT_TRUE(ref.endsWith("test123"));
    EXPECT_FALSE(ref.endsWith("1test123"));
    EXPECT_FALSE(ref.endsWith("test"));
    EXPECT_FALSE(ref.endsWith("test12"));
}

TEST(StringView, slice)
{
    StringView ref("test123test321");
    expectStringView(ref.slice(4, 11).sliceFrom(3), "test", 4);
}

TEST(StringView, findFirstOfChar)
{
    StringView ref("0123456789asdfghjkl");
    EXPECT_EQ(2, ref.findFirstOf('2').unwrap());
    EXPECT_EQ(10, ref.findFirstOf('a').unwrap());
    EXPECT_EQ(18, ref.findFirstOf('l').unwrap());
    EXPECT_FALSE(ref.findFirstOf('z').isSome());
    EXPECT_FALSE(ref.findFirstOf('.').isSome());
    EXPECT_FALSE(ref.findFirstOf('2', 3).isSome());
}

TEST(StringView, findFirstOfStr)
{
    StringView ref("0123456789asdfghjkl");
    EXPECT_EQ(3, ref.findFirstOf("38g").unwrap());
    EXPECT_EQ(10, ref.findFirstOf("safg").unwrap());
    EXPECT_EQ(15, ref.findFirstOf("h").unwrap());
    EXPECT_FALSE(ref.findFirstOf("zxc").isSome());
    EXPECT_FALSE(ref.findFirstOf("qweert").isSome());
    EXPECT_FALSE(ref.findFirstOf("qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq").isSome());
}

TEST(StringView, findFirstNotOfChar)
{
    StringView ref("00000111111111333333");
    EXPECT_EQ(5, ref.findFirstNotOf('0').unwrap());
    EXPECT_EQ(14, ref.findFirstNotOf('1', 5).unwrap());
    EXPECT_FALSE(ref.findFirstNotOf('.', 20).isSome());
    EXPECT_FALSE(ref.findFirstNotOf('3', 15).isSome());
}

TEST(StringView, findFirstNotOfStr)
{
    EXPECT_EQ(7, StringView("122211234567890").findFirstNotOf("12").unwrap());
    EXPECT_EQ(7, StringView("122211234567890").findFirstNotOf("12", 5).unwrap());
    EXPECT_EQ(0, StringView("122211234567890").findFirstNotOf("32").unwrap());
    EXPECT_FALSE(StringView("12221121122222111").findFirstNotOf("12").isSome());
    EXPECT_FALSE(StringView("32221121122222111").findFirstNotOf("12", 1).isSome());
}

TEST(StringView, findLastOfChar)
{
    StringView ref("0123456789asdfgh2kl");
    EXPECT_EQ(0, ref.findLastOf('0').unwrap());
    EXPECT_EQ(16, ref.findLastOf('2').unwrap());
    EXPECT_EQ(16, ref.findLastOf('2', 2).unwrap());
    EXPECT_EQ(2, ref.findLastOf('2', 3).unwrap());
    EXPECT_EQ(10, ref.findLastOf('a').unwrap());
    EXPECT_EQ(18, ref.findLastOf('l').unwrap());
    EXPECT_FALSE(ref.findLastOf('z').isSome());
    EXPECT_FALSE(ref.findLastOf('.').isSome());
}

TEST(StringView, findLastOfStr)
{
    StringView ref("0123456789asdfghjkl");
    EXPECT_EQ(14, ref.findLastOf("38g").unwrap());
    EXPECT_EQ(14, ref.findLastOf("safg").unwrap());
    EXPECT_EQ(15, ref.findLastOf("h").unwrap());
    EXPECT_EQ(15, ref.findLastOf("h", 3).unwrap());
    EXPECT_EQ(0, ref.findLastOf("0", 18).unwrap());
    EXPECT_TRUE(ref.findLastOf("0", 18).isSome());
    EXPECT_FALSE(ref.findLastOf("0", 19).isSome());
    EXPECT_FALSE(ref.findLastOf("h", 4).isSome());
    EXPECT_FALSE(ref.findLastOf("zxc").isSome());
    EXPECT_FALSE(ref.findLastOf("qweert").isSome());
    EXPECT_FALSE(ref.findLastOf("qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq").isSome());
}

TEST(StringView, findLastNotOfChar)
{
    StringView ref("00000111111111333333");
    EXPECT_EQ(19, ref.findLastNotOf('0').unwrap());
    EXPECT_EQ(4, ref.findLastNotOf('1', 6).unwrap());
    EXPECT_EQ(14, ref.findLastNotOf('1', 5).unwrap());
    EXPECT_FALSE(ref.findLastNotOf('.', 20).isSome());
    EXPECT_TRUE(ref.findLastNotOf('0', 14).isSome());
    EXPECT_FALSE(ref.findLastNotOf('0', 15).isSome());
}

TEST(StringView, findLastNotOfStr)
{
    EXPECT_EQ(12, StringView("122211234567890").findLastNotOf("90").unwrap());
    EXPECT_EQ(11, StringView("122211234567890").findLastNotOf("56", 3).unwrap());
    EXPECT_EQ(8, StringView("122211234567890").findLastNotOf("56", 4).unwrap());
    EXPECT_EQ(8, StringView("122211234567890").findLastNotOf("56", 5).unwrap());
    EXPECT_EQ(8, StringView("122211234567890").findLastNotOf("56", 6).unwrap());
    EXPECT_FALSE(StringView("12221121122222111").findLastNotOf("12").isSome());
    EXPECT_TRUE(StringView("22221121122222113").findLastNotOf("12", 0).isSome());
    EXPECT_FALSE(StringView("22221121122222113").findLastNotOf("12", 1).isSome());
    EXPECT_FALSE(StringView("22221121122222113").findLastNotOf("12", 1).isSome());
}

TEST(StringView, ltrimChar)
{
    expectStringView(StringView("12345").ltrim('z'), "12345", 5);
    expectStringView(StringView("12345").ltrim('1'), "2345", 4);
    expectStringView(StringView("aazzz").ltrim('a'), "zzz", 3);
    expectStringView(StringView("zzzzz").ltrim('z'), "", 0);
}

TEST(StringView, rtrimChar)
{
    expectStringView(StringView("12345").rtrim('z'), "12345", 5);
    expectStringView(StringView("12345").rtrim('5'), "1234", 4);
    expectStringView(StringView("sssaaaa").rtrim('a'), "sss", 3);
    expectStringView(StringView("zzzzz").rtrim('z'), "", 0);
}

TEST(StringView, trimChar)
{
    StringView ref("1111111111122222221111111111");
    expectStringView(ref.trim('1'), "2222222", 7);
}


TEST(StringView, ltrimCharSet)
{
    expectStringView(StringView("12345").ltrim("67"), "12345", 5);
    expectStringView(StringView("12345").ltrim("16"), "2345", 4);
    expectStringView(StringView("aazzz").ltrim("a"), "zzz", 3);
    expectStringView(StringView("zzzzz").ltrim("az"), "", 0);
}

TEST(StringView, rtrimCharSet)
{
    expectStringView(StringView("12345").rtrim("zy"), "12345", 5);
    expectStringView(StringView("12345").rtrim("56"), "1234", 4);
    expectStringView(StringView("sssaaaa").rtrim("a"), "sss", 3);
    expectStringView(StringView("sssaaaa").rtrim("s"), "sssaaaa", 7);
    expectStringView(StringView("sssaaaa").rtrim("sa"), "", 0);
}

TEST(StringView, trimCharSet)
{
    StringView ref("11111111111222222233333333");
    expectStringView(ref.trim("13"), "2222222", 7);
    expectStringView(ref.trim("1"), "222222233333333", 15);
    expectStringView(ref.trim("3"), "111111111112222222", 18);
    expectStringView(ref.trim("567"), "11111111111222222233333333", 26);
}

TEST(StringView, assign)
{
    StringView ref = StringView::empty();
    ref = "asd";
    expectStringView(ref, "asd");
}

template <typename T>
class StringViewEq : public ::testing::Test {
};

typedef ::testing::Types<StringView, std::string, const char*, ArrayView<char>> MyTypes;

TYPED_TEST_CASE(StringViewEq, MyTypes);

TYPED_TEST(StringViewEq, typeEqView)
{
    TypeParam ref1("asd_qwe");
    StringView ref2("asd_qwe");
    EXPECT_EQ(ref1, ref2);
}

TYPED_TEST(StringViewEq, typeNeqViewSize)
{
    TypeParam ref1("asdsdasdasdasd");
    StringView ref2("ds");
    EXPECT_NE(ref1, ref2);
}

TYPED_TEST(StringViewEq, typeNeqViewContents)
{
    TypeParam ref1("asd_asd");
    StringView ref2("asd=asd");
    EXPECT_NE(ref1, ref2);
}

TYPED_TEST(StringViewEq, viewEqType)
{
    StringView ref1("123123");
    TypeParam ref2("123123");
    EXPECT_EQ(ref1, ref2);
}

TYPED_TEST(StringViewEq, vieweNeqTypeSize)
{
    StringView ref1("1234");
    TypeParam ref2("0");
    EXPECT_NE(ref1, ref2);
}

TYPED_TEST(StringViewEq, vieweNeqTypeContents)
{
    StringView ref1("1231234123123");
    TypeParam ref2("1231235123123");
    EXPECT_NE(ref1, ref2);
}
