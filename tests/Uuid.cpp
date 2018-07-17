#include "bmcl/Uuid.h"
#include "bmcl/Result.h"
#include "bmcl/StringView.h"

#ifdef BMCL_HAVE_QT
# include <QString>
#endif

#include "BmclTest.h"

using namespace bmcl;

TEST(Uuid, fromStringNormal)
{
    bmcl::StringView uuid = "01234567-89ab-cdef-0123-456789ABCDEF";
    Uuid::Data expected = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    auto rv = Uuid::createFromString(uuid);
    ASSERT_TRUE(rv.isOk());
    EXPECT_EQ(expected, rv.unwrap().data());
}

TEST(Uuid, fromStringMicrosoft)
{
    bmcl::StringView uuid = "{01234667-89ab-cdef-0123-456789ABCDEF}";
    Uuid::Data expected = {0x01, 0x23, 0x46, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    auto rv = Uuid::createFromString(uuid);
    ASSERT_TRUE(rv.isOk());
    EXPECT_EQ(expected, rv.unwrap().data());
}

TEST(Uuid, fromStringInvalidSize)
{
    bmcl::StringView uuid = "01234667-89ab-cdef-0123-456789ABCDE";
    auto rv = Uuid::createFromString(uuid);
    ASSERT_FALSE(rv.isOk());
}

TEST(Uuid, fromStringInvalidFormat)
{
    bmcl::StringView uuid = "01234667-89ab-cdef-0123_456789ABCDEF";
    auto rv = Uuid::createFromString(uuid);
    ASSERT_FALSE(rv.isOk());
}

TEST(Uuid, fromToString)
{
    Uuid u = Uuid::create();

    std::string str = u.toStdString();

    auto rv = Uuid::createFromString(str);
    ASSERT_TRUE(rv.isOk());
    EXPECT_EQ(u, rv.unwrap());
}

#ifdef BMCL_HAVE_QT

TEST(Uuid, fromToQString)
{
    Uuid u = Uuid::create();

    QString str = u.toQString();

    auto rv = Uuid::createFromString(str);
    ASSERT_TRUE(rv.isOk());
    EXPECT_EQ(u, rv.unwrap());
}

#endif
