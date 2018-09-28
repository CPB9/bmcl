#include "bmcl.h" //test compilation

#include <gtest/gtest.h>

#include <type_traits>

using namespace bmcl;

class TestRc : public RefCountable<std::size_t> {
};

TEST(misc, makeRcType)
{
    auto rc = makeRc<TestRc>();
    auto isSame = std::is_same<Rc<TestRc>, decltype(rc)>::value;
    EXPECT_TRUE(isSame);
}
