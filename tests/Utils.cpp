#include "bmcl/Utils.h"
#include "bmcl/Result.h"
#include "bmcl/Buffer.h"

#include "BmclTest.h"

#include <gtest/gtest.h>

#include <cstring>

TEST(Utils, readFile)
{
    const char* path = DATA_DIR"/test1";
    bmcl::Result<std::string, int> res = bmcl::readFileIntoString(path);
    ASSERT_TRUE(res.isOk());
    ASSERT_EQ(11, res.unwrap().size());
    EXPECT_EQ_MEM(res.unwrap().data(), "1234567890\n", 11);
}

TEST(Utils, readFileError)
{
    const char* path = DATA_DIR"/_te_st1_";
    bmcl::Result<std::string, int> res = bmcl::readFileIntoString(path);
    ASSERT_TRUE(res.isErr());
}
