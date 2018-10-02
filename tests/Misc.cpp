#include "bmcl.h" //test compilation

#include <gtest/gtest.h>

#include <type_traits>

using namespace bmcl;

class TestRc : public RefCountable<std::size_t> {
public:
    TestRc(int, const char*, int)
    {
    }
};

TEST(misc, makeRcType)
{
    auto rc = makeRc<TestRc>(1, "", 4);
    auto isSame = std::is_same<Rc<TestRc>, decltype(rc)>::value;
    EXPECT_TRUE(isSame);
}

TEST(misc, wrapRc)
{
    auto ptr = new TestRc(1, "asd", 5);
    auto rc = wrapRc(ptr);
    auto isSame = std::is_same<Rc<TestRc>, decltype(rc)>::value;
    EXPECT_TRUE(isSame);
}
