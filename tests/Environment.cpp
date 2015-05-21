#include "bmcl/Endian.h"
#include "bmcl/Alloca.h"

#include <gtest/gtest.h>

#include <cstring>

// environment sanity tests

class EnvironmentTest : public ::testing::Test {
protected:
    void SetUp()
    {
        h16 = 0x1234;
        h32 = 0x12345678;
        h64 = 0x1234567890abcdef;
        t16 = 0;
        t32 = 0;
        t64 = 0;
        if (isLittleEndian()) {
            le16 = h16;
            le32 = h32;
            le64 = h64;
            be16 = 0x3412;
            be32 = 0x78563412;
            be64 = 0xefcdab9078563412;
        } else {
            be16 = h16;
            be32 = h32;
            be64 = h64;
            le16 = 0x3412;
            le32 = 0x78563412;
            le64 = 0xefcdab9078563412;
        }
    }

    bool isLittleEndian()
    {
        union {
            uint16_t u16;
            uint8_t u8[2];
        } test = {0x1234};
        return test.u8[0] == 0x34;
    }

protected:
    // host, little-endian, big-endian, target
    uint16_t h16, le16, be16, t16;
    uint32_t h32, le32, be32, t32;
    uint64_t h64, le64, be64, t64;
};

TEST_F(EnvironmentTest, endian_defines)
{
#ifdef BMCL_LITTLE_ENDIAN
    EXPECT_TRUE(isLittleEndian());
#else
    EXPECT_FALSE(isLittleEndian());
#endif
}

TEST_F(EnvironmentTest, htobe)
{
    EXPECT_EQ(be16, htobe16(h16));
    EXPECT_EQ(be32, htobe32(h32));
    EXPECT_EQ(be64, htobe64(h64));
}

TEST_F(EnvironmentTest, htole)
{
    EXPECT_EQ(le16, htole16(h16));
    EXPECT_EQ(le32, htole32(h32));
    EXPECT_EQ(le64, htole64(h64));
}

TEST_F(EnvironmentTest, betoh)
{
    EXPECT_EQ(h16, be16toh(be16));
    EXPECT_EQ(h32, be32toh(be32));
    EXPECT_EQ(h64, be64toh(be64));
}

TEST_F(EnvironmentTest, letoh)
{
    EXPECT_EQ(h16, le16toh(le16));
    EXPECT_EQ(h32, le32toh(le32));
    EXPECT_EQ(h64, le64toh(le64));
}

TEST_F(EnvironmentTest, bedec)
{
    t16 = be16dec(&be16);
    EXPECT_EQ(h16, t16);
    t32 = be32dec(&be32);
    EXPECT_EQ(h32, t32);
    t64 = be64dec(&be64);
    EXPECT_EQ(h64, t64);
}

TEST_F(EnvironmentTest, ledec)
{
    t16 = le16dec(&le16);
    EXPECT_EQ(h16, t16);
    t32 = le32dec(&le32);
    EXPECT_EQ(h32, t32);
    t64 = le64dec(&le64);
    EXPECT_EQ(h64, t64);
}

TEST_F(EnvironmentTest, beenc)
{
    be16enc(&t16, h16);
    EXPECT_EQ(be16, t16);
    be32enc(&t32, h32);
    EXPECT_EQ(be32, t32);
    be64enc(&t64, h64);
    EXPECT_EQ(be64, t64);
}

TEST_F(EnvironmentTest, leenc)
{
    le16enc(&t16, h16);
    EXPECT_EQ(le16, t16);
    le32enc(&t32, h32);
    EXPECT_EQ(le32, t32);
    le64enc(&t64, h64);
    EXPECT_EQ(le64, t64);
}

TEST_F(EnvironmentTest, alloca)
{
    char* mem = (char*)alloca(1024);
    std::memset(mem, 0x5a, 1024);
    // should not fail
}

