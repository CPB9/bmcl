#include <bmcl/AlignedUnion.h>

#include "BmclTest.h"

#include <gtest/gtest.h>
#include <type_traits>

using namespace bmcl;

TEST(AlignedUnion, set)
{
    AlignedUnion<int, double, std::string> alignedUnion;
    alignedUnion.set<int>(1);
    EXPECT_EQ(1, alignedUnion.as<int>());

    alignedUnion.set<std::string>(std::string("asd"));
    EXPECT_EQ("asd", alignedUnion.as<std::string>());
    alignedUnion.destruct<std::string>();

    alignedUnion.set<int>(4);
    alignedUnion.set<double>(4.1);
    EXPECT_DOUBLE_EQ(4.1, alignedUnion.as<double>());
}

TEST(AlignedUnion, emplace)
{
    AlignedUnion<int, double, std::string> alignedUnion;
    alignedUnion.emplace<int>(2);
    EXPECT_EQ(2, alignedUnion.as<int>());

    alignedUnion.emplace<int>(5);
    alignedUnion.emplace<double>(4.2);
    EXPECT_DOUBLE_EQ(4.2, alignedUnion.as<double>());

    alignedUnion.emplace<std::string>("asd");
    EXPECT_EQ("asd", alignedUnion.as<std::string>());
    alignedUnion.destruct<std::string>();
}
