#include "bmcl/Panic.h"
#include "bmcl/Assert.h"

#include <iostream>

#include <gtest/gtest.h>

#if GTEST_HAS_DEATH_TEST && !BMCL_ASAN

TEST(Panic, panic)
{
    ASSERT_DEATH(bmcl::panic("Death message"), "Death message");
}

TEST(Panic, assertFail)
{
    ASSERT_DEATH(bmcl::assertFail("Assert message", __FILE__, __LINE__), "Assertion failed at.*Assert message");
}

static bool assertTest()
{
    return false;
}

TEST(Panic, assert)
{
    EXPECT_DEATH(BMCL_ASSERT(9 > 10), "Assertion failed at.*9 > 10");
}

static void customHandler(const char* msg)
{
    std::cerr << "Custom " << msg;
}

TEST(Panic, custom)
{
    bmcl::setPanicHandler(customHandler);
    ASSERT_DEATH(bmcl::panic("Message"), "Custom Message");
}

#endif
