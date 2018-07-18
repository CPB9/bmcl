#include "bmcl/Uuid.h"
#include "bmcl/Result.h"
#include "bmcl/StringView.h"

#ifdef BMCL_HAVE_QT
# include <QString>
# include <QUuid>
#endif

#include "BmclTest.h"

using namespace bmcl;

TEST(Uuid, nil)
{
    Uuid::Data expected = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    Uuid u = Uuid::createNil();

    ASSERT_EQ(expected, u.data());
    EXPECT_TRUE(u.isNil());
}

TEST(Uuid, fromValues)
{
    std::uint32_t d1 = 0x4d2f9a30;
    std::uint16_t d2 = 0x8c74;
    std::uint16_t d3 = 0x9012;
    std::uint64_t d4 = 0x9ed9f03d34df7a58;

    Uuid::Data expected = {0x4d, 0x2f, 0x9a, 0x30, 0x8c, 0x74, 0x90, 0x12, 0x9e, 0xd9, 0xf0, 0x3d, 0x34, 0xdf, 0x7a, 0x58};

    Uuid u(d1, d2, d3, d4);

    ASSERT_EQ(expected, u.data());
    EXPECT_EQ(d1, u.part1());
    EXPECT_EQ(d2, u.part2());
    EXPECT_EQ(d3, u.part3());
    EXPECT_EQ(d4, u.part4());
}

TEST(Uuid, fromStringNoBracesDashes)
{
    bmcl::StringView uuid = "01234567-89ab-cdef-0123-456789ABCDEF";
    Uuid::Data expected = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    auto rv = Uuid::createFromString(uuid);
    ASSERT_TRUE(rv.isOk());
    EXPECT_EQ(expected, rv.unwrap().data());
}

TEST(Uuid, fromStringBracesDahes)
{
    bmcl::StringView uuid = "{01234667-89ab-cdef-0123-456789ABCDEF}";
    Uuid::Data expected = {0x01, 0x23, 0x46, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    auto rv = Uuid::createFromString(uuid);
    ASSERT_TRUE(rv.isOk());
    EXPECT_EQ(expected, rv.unwrap().data());
}

TEST(Uuid, fromStringNoBracesNoDashes)
{
    bmcl::StringView uuid = "a123456789abcdef0123456789ABCDEF";
    Uuid::Data expected = {0xa1, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    auto rv = Uuid::createFromString(uuid);
    ASSERT_TRUE(rv.isOk());
    EXPECT_EQ(expected, rv.unwrap().data());
}

TEST(Uuid, fromStringBracesNoDashes)
{
    bmcl::StringView uuid = "{0123456789abcdef0153456789ABCDEF}";
    Uuid::Data expected = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x53, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
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

TEST(Uuid, fromStringOrNilInvalid)
{
    bmcl::StringView uuid = "a1234";
    auto rv = Uuid::createFromStringOrNil(uuid);
    ASSERT_TRUE(rv.isNil());
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

TEST(Uuid, fromToQUuid)
{
    QUuid from = QUuid::createUuid();
    Uuid u(from);
    QUuid to = u.toQUuid();

    EXPECT_EQ(from, to);
}


#endif
