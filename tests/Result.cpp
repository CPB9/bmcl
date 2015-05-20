#include "bmcl/Result.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

using namespace bmcl;

TEST(ResultTest, test1)
{
    Result<int, std::string> value(123);
    Result<int, std::string> error("error");
    EXPECT_TRUE(value.isOk());
    EXPECT_TRUE(error.isErr());
    EXPECT_EQ(123, value.unwrap());
    EXPECT_EQ("error", error.unwrapErr());

    value = error;
    EXPECT_TRUE(value.isErr());
    EXPECT_EQ("error", value.unwrapErr());

    value = Result<int, std::string>(321);
    EXPECT_TRUE(value.isOk());
    EXPECT_EQ(321, value.unwrap());
}
