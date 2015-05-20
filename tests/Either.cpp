#include "bmcl/Either.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

using namespace bmcl;

TEST(EitherTest, createCopySecond)
{
    std::string errString = "second";
    Either<int, std::string> either(errString);
    EXPECT_FALSE(either.isFirst());
    EXPECT_TRUE(either.isSecond());
    EXPECT_EQ("second", either.unwrapSecond());
}

TEST(EitherTest, createMoveSecond)
{
    std::string errString = "second";
    Either<int, std::string> either(std::move(errString));
    EXPECT_FALSE(either.isFirst());
    EXPECT_TRUE(either.isSecond());
    EXPECT_EQ("second", either.unwrapSecond());
}

TEST(EitherTest, createCopyEither)
{
    std::vector<int> data({1, 2, 3, 4, 6});
    Either<std::vector<int>, std::string> either(data);
    EXPECT_TRUE(either.isFirst());
    EXPECT_FALSE(either.isSecond());
    EXPECT_EQ(std::vector<int>({1, 2, 3, 4, 6}), either.unwrapFirst());
}

TEST(EitherTest, createMoveEither)
{
    std::vector<int> data({1, 2, 3, 4, 5});
    Either<std::vector<int>, std::string> either(std::move(data));
    EXPECT_TRUE(either.isFirst());
    EXPECT_FALSE(either.isSecond());
    EXPECT_EQ(std::vector<int>({1, 2, 3, 4, 5}), either.unwrapFirst());
}

TEST(EitherTest, copyConstructSecond)
{
    std::vector<int> err{3, 2, 1};
    Either<std::string, std::vector<int>> oldEither(err);
    Either<std::string, std::vector<int>> newEither(oldEither);
    EXPECT_FALSE(newEither.isFirst());
    EXPECT_TRUE(newEither.isSecond());
    EXPECT_EQ(std::vector<int>({3, 2, 1}), newEither.unwrapSecond());
}

TEST(EitherTest, copyConstructEither)
{
    Either<std::string, int> oldEither("either");
    Either<std::string, int> newEither(oldEither);
    EXPECT_TRUE(newEither.isFirst());
    EXPECT_FALSE(newEither.isSecond());
    EXPECT_EQ("either", newEither.unwrapFirst());
}

TEST(EitherTest, moveConstructSecond)
{
    std::vector<int> err{3, 2, 1};
    Either<std::string, std::vector<int>> oldEither(err);
    Either<std::string, std::vector<int>> newEither(std::move(oldEither));
    EXPECT_FALSE(newEither.isFirst());
    EXPECT_TRUE(newEither.isSecond());
    EXPECT_EQ(std::vector<int>({3, 2, 1}), newEither.unwrapSecond());
}

TEST(EitherTest, moveConstructEither)
{
    Either<std::string, int> oldEither("either");
    Either<std::string, int> newEither(std::move(oldEither));
    EXPECT_TRUE(newEither.isFirst());
    EXPECT_FALSE(newEither.isSecond());
    EXPECT_EQ("either", newEither.unwrapFirst());
}

TEST(EitherTest, operatorEqFirstFirst)
{
    Either<std::string, int> first1("asdasd");
    Either<std::string, int> first2("qweqwe");
    first1 = first2;
    EXPECT_TRUE(first1.isFirst());
    EXPECT_TRUE(first2.isFirst());
    EXPECT_EQ("qweqwe", first1.unwrapFirst());
    EXPECT_EQ("qweqwe", first2.unwrapFirst());
}

TEST(EitherTest, operatorEqFirstSecond)
{
    Either<std::string, int> first("first");
    Either<std::string, int> second(2);
    first = second;
    EXPECT_TRUE(first.isSecond());
    EXPECT_TRUE(second.isSecond());
    EXPECT_EQ(2, first.unwrapSecond());
    EXPECT_EQ(2, second.unwrapSecond());
}

TEST(EitherTest, operatorEqSecondFirst)
{
    Either<std::string, int> second(2);
    Either<std::string, int> first("first");
    second = first;
    EXPECT_TRUE(first.isFirst());
    EXPECT_TRUE(second.isFirst());
    EXPECT_EQ("first", first.unwrapFirst());
    EXPECT_EQ("first", second.unwrapFirst());
}

TEST(EitherTest, operatorEqSecondSecond)
{
    Either<std::string, int> second1(123);
    Either<std::string, int> second2(768);
    second1 = second2;
    EXPECT_TRUE(second1.isSecond());
    EXPECT_TRUE(second2.isSecond());
    EXPECT_EQ(768, second1.unwrapSecond());
    EXPECT_EQ(768, second2.unwrapSecond());
}

TEST(EitherTest, takeFirst)
{
    Either<std::string, std::vector<int>> first("value");
    Either<std::string, std::vector<int>> second = std::vector<int>({1, 2, 3, 4});
    second = first.takeFirst();
    EXPECT_TRUE(first.isFirst());
    EXPECT_TRUE(second.isFirst());
    EXPECT_EQ("value", second.unwrapFirst());
    EXPECT_EQ("", first.unwrapFirst());
}

TEST(EitherTest, takeSecond)
{
    Either<std::string, std::vector<int>> first("value");
    Either<std::string, std::vector<int>> second = std::vector<int>({1, 2, 3, 4});
    first = second.takeSecond();
    EXPECT_TRUE(first.isSecond());
    EXPECT_TRUE(second.isSecond());
    EXPECT_EQ(std::vector<int>({1, 2, 3, 4}), first.unwrapSecond());
    EXPECT_TRUE(second.unwrapSecond().empty());
}
