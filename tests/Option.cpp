#include "bmcl/Option.h"

#include <gtest/gtest.h>

#include <string>

using namespace bmcl;

TEST(OptionTest, createNone)
{
    Option<int> option;
    EXPECT_FALSE(option.isSome());
    EXPECT_TRUE(option.isNone());
}

static Option<double> noneTestFunc()
{
    return None;
}

TEST(OptionTest, createFromTag)
{
    Option<double> option = noneTestFunc();
    EXPECT_FALSE(option.isSome());
    EXPECT_TRUE(option.isNone());
}


TEST(OptionTest, createInt)
{
    Option<int> option(5);
    EXPECT_TRUE(option.isSome());
    EXPECT_FALSE(option.isNone());
    EXPECT_EQ(5, option.unwrap());
}

TEST(OptionTest, createStringInPlace)
{
    std::string src("test string");
    Option<std::string> option(InPlace, src, 1, 9);
    EXPECT_TRUE(option.isSome());
    EXPECT_EQ("est strin", option.unwrap());
}


TEST(OptionTest, createCopyString)
{
    std::string test = "copy string test";
    Option<std::string> option(test);
    EXPECT_TRUE(option.isSome());
    EXPECT_FALSE(option.isNone());
    EXPECT_EQ("copy string test", option.unwrap());
}

TEST(OptionTest, createMoveString)
{
    std::string test = "move string test";
    Option<std::string> option(std::move(test));
    EXPECT_TRUE(option.isSome());
    EXPECT_FALSE(option.isNone());
    EXPECT_EQ("move string test", option.unwrap());
}

TEST(OptionTest, unwrapMutable)
{
    Option<std::string> option("string test");
    option.unwrap() = "123";
    EXPECT_TRUE(option.isSome());
    EXPECT_EQ("123", option.unwrap());
}

TEST(OptionTest, unwrapOrLvalue)
{
    Option<std::string> option1;
    Option<std::string> option2("test");
    EXPECT_EQ("result", option1.unwrapOr("result"));
    EXPECT_EQ("test", option2.unwrapOr("result"));
}

TEST(OptionTest, unwrapOrRvalue)
{
    EXPECT_EQ("result", Option<std::string>().unwrapOr("result"));
    EXPECT_EQ("test", Option<std::string>("test").unwrapOr("result"));
}

TEST(OptionTest, takeString)
{
    Option<std::string> option("take string test");
    std::string newString = option.take();
    EXPECT_FALSE(option.isSome());
    EXPECT_TRUE(option.isNone());
    EXPECT_EQ("take string test", newString);
}

TEST(OptionTest, copyConstructor)
{
    Option<std::string> oldOption("copy constructor test");
    Option<std::string> newOption(oldOption);
    EXPECT_TRUE(oldOption.isSome());
    EXPECT_FALSE(oldOption.isNone());
    EXPECT_EQ("copy constructor test", oldOption.unwrap());
    EXPECT_TRUE(newOption.isSome());
    EXPECT_FALSE(newOption.isNone());
    EXPECT_EQ("copy constructor test", newOption.unwrap());
}

TEST(OptionTest, moveConstructor)
{
    Option<std::string> oldOption("move constructor test");
    Option<std::string> newOption(std::move(oldOption));
    EXPECT_FALSE(oldOption.isSome());
    EXPECT_TRUE(oldOption.isNone());
    EXPECT_TRUE(newOption.isSome());
    EXPECT_FALSE(newOption.isNone());
    EXPECT_EQ("move constructor test", newOption.unwrap());
}

TEST(OptionTest, operatorEqNoneNone)
{
    Option<std::string> oldOption;
    Option<std::string> newOption;
    newOption = oldOption;
    EXPECT_TRUE(oldOption.isNone());
    EXPECT_TRUE(newOption.isNone());
}

TEST(OptionTest, operatorEqNoneSome)
{
    Option<std::string> oldOption;
    Option<std::string> newOption("some");
    newOption = oldOption;
    EXPECT_TRUE(oldOption.isNone());
    EXPECT_TRUE(newOption.isNone());
}

TEST(OptionTest, operatorEqSomeNone)
{
    Option<std::string> oldOption("some");
    Option<std::string> newOption;
    newOption = oldOption;
    EXPECT_TRUE(oldOption.isSome());
    EXPECT_TRUE(newOption.isSome());
    EXPECT_EQ("some", oldOption.unwrap());
    EXPECT_EQ("some", newOption.unwrap());
}

TEST(OptionTest, operatorEqSomeSome)
{
    Option<std::string> oldOption("some1");
    Option<std::string> newOption("some2");
    newOption = oldOption;
    EXPECT_TRUE(oldOption.isSome());
    EXPECT_TRUE(newOption.isSome());
    EXPECT_EQ("some1", oldOption.unwrap());
    EXPECT_EQ("some1", newOption.unwrap());
}

TEST(OptionTest, operatorEqNoneValue)
{
    Option<std::string> option;
    std::string value("test value");
    option = value;
    EXPECT_TRUE(option.isSome());
    EXPECT_EQ("test value", option.unwrap());
}

TEST(OptionTest, operatorEqSomeValue)
{
    Option<std::string> option("previous");
    std::string value("next");
    option = value;
    EXPECT_TRUE(option.isSome());
    EXPECT_EQ("next", option.unwrap());
}

TEST(OptionTest, operatorEqMoveNoneNone)
{
    Option<std::string> oldOption;
    Option<std::string> newOption;
    newOption = std::move(oldOption);
    EXPECT_TRUE(oldOption.isNone());
    EXPECT_TRUE(newOption.isNone());
}

TEST(OptionTest, operatorEqMoveNoneSome)
{
    Option<std::string> oldOption;
    Option<std::string> newOption("some");
    newOption = std::move(oldOption);
    EXPECT_TRUE(oldOption.isNone());
    EXPECT_TRUE(newOption.isNone());
}

TEST(OptionTest, operatorEqMoveSomeNone)
{
    Option<std::string> oldOption("some");
    Option<std::string> newOption;
    newOption = std::move(oldOption);
    EXPECT_TRUE(oldOption.isNone());
    EXPECT_TRUE(newOption.isSome());
    EXPECT_EQ("some", newOption.unwrap());
}

TEST(OptionTest, operatorEqMoveSomeSome)
{
    Option<std::string> oldOption("some1");
    Option<std::string> newOption("some2");
    newOption = std::move(oldOption);
    EXPECT_TRUE(oldOption.isNone());
    EXPECT_TRUE(newOption.isSome());
    EXPECT_EQ("some1", newOption.unwrap());
}

TEST(OptionTest, operatorEqMoveNoneValue)
{
    Option<std::string> option;
    std::string value("test value");
    option = std::move(value);
    EXPECT_TRUE(option.isSome());
    EXPECT_EQ("test value", option.unwrap());
}

TEST(OptionTest, operatorEqMoveSomeValue)
{
    Option<std::string> option("previous");
    std::string value("next");
    option = std::move(value);
    EXPECT_TRUE(option.isSome());
    EXPECT_EQ("next", option.unwrap());
}

TEST(OptionTest, operatorEqEq)
{
    Option<std::string> none1;
    Option<std::string> none2;
    Option<std::string> some1("some");
    Option<std::string> some2("some");
    Option<std::string> some3("emos");
    EXPECT_TRUE(none1 == none2);
    EXPECT_TRUE(none2 == none1);
    EXPECT_TRUE(some1 == some2);
    EXPECT_TRUE(some2 == some1);
    EXPECT_FALSE(none1 == some1);
    EXPECT_FALSE(some2 == none2);
    EXPECT_FALSE(some1 == some3);
    EXPECT_FALSE(some3 == some2);
}

TEST(OptionTest, operatorDeref)
{
    Option<std::string> option("some");
    EXPECT_TRUE(option.isSome());
    EXPECT_STREQ("some", option->c_str());
    option->append(" test");
    EXPECT_STREQ("some test", option->c_str());
}

TEST(OptionTest, operatorInd)
{
    Option<std::string> option("some");
    EXPECT_TRUE(option.isSome());
    EXPECT_STREQ("some", (*option).c_str());
    (*option).append(" test");
    EXPECT_STREQ("some test", (*option).c_str());
}

TEST(OptionTest, emplace)
{
    Option<std::string> option;
    EXPECT_FALSE(option.isSome());
    option.emplace("some1");
    EXPECT_TRUE(option.isSome());
    EXPECT_EQ("some1", option.unwrap());
    option.emplace("2some");
    EXPECT_TRUE(option.isSome());
    EXPECT_EQ("2some", option.unwrap());
}
