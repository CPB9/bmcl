#include "bmcl/CString.h"

#include "BmclTest.h"

#include <cstring>

using namespace bmcl;

TEST(CString, null)
{
    CString str;
    ASSERT_TRUE(str.isNull());

    CString str2(nullptr);
    ASSERT_TRUE(str2.isNull());
}

TEST(CString, fromChar)
{
    CString str("asdasd");
    ASSERT_FALSE(str.isNull());
    ASSERT_STREQ("asdasd", str.data());
    ASSERT_EQ(6, str.size());
}

TEST(CString, constructOther)
{
    CString str1("dsadsa");
    CString str2(str1);

    ASSERT_STREQ("dsadsa", str1.data());
    ASSERT_STREQ("dsadsa", str2.data());
}

TEST(CString, moveConstructOther)
{
    CString str1("dsadsa");
    CString str2(std::move(str1));

    ASSERT_TRUE(str1.isNull());
    ASSERT_STREQ("dsadsa", str2.data());
}

TEST(CString, assignOther)
{
    CString str1("4444444");
    CString str2("555555");

    str2 = str1;

    ASSERT_STREQ("4444444", str1.data());
    ASSERT_STREQ("4444444", str2.data());
}

TEST(CString, moveAssignOther)
{
    CString str1("1111");
    CString str2("6666");

    str2 = std::move(str1);

    ASSERT_TRUE(str1.isNull());
    ASSERT_STREQ("1111", str2.data());
}

TEST(CString, assignOtherIntoNull)
{
    CString str1("01010101");
    CString str2;

    str2 = str1;

    ASSERT_STREQ("01010101", str1.data());
    ASSERT_STREQ("01010101", str2.data());
}

TEST(CString, moveAssignOtherIntoNull)
{
    CString str1("ggaa");
    CString str2;

    str2 = std::move(str1);

    ASSERT_TRUE(str1.isNull());
    ASSERT_STREQ("ggaa", str2.data());
}

TEST(CString, assignOtherFromNull)
{
    CString str1;
    CString str2("0000000000");

    str2 = str1;

    ASSERT_TRUE(str1.isNull());
    ASSERT_TRUE(str2.isNull());
}

TEST(CString, moveAssignOtherFromNull)
{
    CString str1;
    CString str2("rrrr");

    str2 = std::move(str1);

    ASSERT_TRUE(str1.isNull());
    ASSERT_TRUE(str2.isNull());
}

TEST(CString, fromOwned)
{
    char* data = (char*)std::malloc(11);
    std::memcpy(data, "1234567890", 10);
    data[10] = '\0';

    CString str1 = CString::fromOwned(data);

    ASSERT_STREQ("1234567890", str1.data());
}
