#include "bmcl/MmapOpener.h"

#include "BmclTest.h"

#include <gtest/gtest.h>

TEST(MmapOpener, readFile1)
{
    const char* path = DATA_DIR"/test1";
    bmcl::MmapOpener mmapFile;
    ASSERT_TRUE(mmapFile.open(path));
    ASSERT_TRUE(mmapFile.isOpen());
    ASSERT_EQ(mmapFile.size(), 11u);
    EXPECT_EQ_MEM(mmapFile.data(), "1234567890\n", 11);
    ASSERT_TRUE(mmapFile.close());
    ASSERT_FALSE(mmapFile.isOpen());
}

TEST(MmapOpener, missingFile)
{
    const char* path = DATA_DIR"/_invali_123123_";
    bmcl::MmapOpener mmapFile;
    ASSERT_FALSE(mmapFile.open(path));
    ASSERT_FALSE(mmapFile.isOpen());
}

TEST(MmapOpener, readOnes)
{
    const char* path = DATA_DIR"/ones";
    bmcl::MmapOpener mmapFile;
    ASSERT_TRUE(mmapFile.open(path));
    ASSERT_TRUE(mmapFile.isOpen());

    std::string expected(1024 * 1024, '1');

    ASSERT_EQ(mmapFile.size(), expected.size());
    EXPECT_EQ_MEM(mmapFile.data(), expected.data(), expected.size());
    ASSERT_TRUE(mmapFile.close());
    ASSERT_FALSE(mmapFile.isOpen());
}
