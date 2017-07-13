#include <bmcl/Variant.h>

#include "BmclTest.h"

#include <gtest/gtest.h>

using namespace bmcl;

enum class Vkind {
    Empty,
    Int,
    Double,
    Float,
    Voidp,
    String,
};

using Vtype = Variant<Vkind, Vkind::Empty,
                VariantElementDesc<Vkind, int, Vkind::Int>,
                VariantElementDesc<Vkind, double, Vkind::Double>,
                VariantElementDesc<Vkind, float, Vkind::Float>,
                VariantElementDesc<Vkind, void*, Vkind::Voidp>,
                VariantElementDesc<Vkind, std::string, Vkind::String>
                >;

TEST(Variant, constructFromValue)
{
    Vtype var0;
    Vtype var1(int(1));
    Vtype var2(double(2.2));
    Vtype var3(float(3.3));
    Vtype var4((void*)4);
    Vtype var5(std::string("balalaika"));

    EXPECT_TRUE(var0.isEmpty());
    EXPECT_FALSE(var1.isEmpty());
    EXPECT_FALSE(var2.isEmpty());
    EXPECT_FALSE(var3.isEmpty());
    EXPECT_FALSE(var4.isEmpty());
    EXPECT_FALSE(var5.isEmpty());
    EXPECT_EQ(Vkind::Empty, var0.kind());
    EXPECT_EQ(Vkind::Int, var1.kind());
    EXPECT_EQ(1, var1.as<int>());
    EXPECT_EQ(Vkind::Double, var2.kind());
    EXPECT_DOUBLE_EQ(2.2, var2.as<double>());
    EXPECT_EQ(Vkind::Float, var3.kind());
    EXPECT_FLOAT_EQ(3.3, var3.as<float>());
    EXPECT_EQ(Vkind::Voidp, var4.kind());
    EXPECT_EQ((void*)4, var4.as<void*>());
    EXPECT_EQ(Vkind::String, var5.kind());
    EXPECT_EQ("balalaika", var5.as<std::string>());
}

TEST(Variant, moveConstructFromValue)
{
    std::string value = std::string("balalaika");
    Vtype var(std::move(value));

    EXPECT_EQ(Vkind::String, var.kind());
    EXPECT_EQ("balalaika", var.as<std::string>());
    EXPECT_TRUE(value.empty());
}

TEST(Variant, constructFromOther)
{
    std::string value = std::string("balalaika");
    Vtype var1(value);
    Vtype var2(var1);

    EXPECT_EQ(Vkind::String, var2.kind());
    EXPECT_EQ("balalaika", var2.as<std::string>());
}

TEST(Variant, moveConstructFromOther)
{
    std::string value = std::string("balalaika");
    Vtype var1(value);
    Vtype var2(std::move(var1));

    EXPECT_TRUE(var1.isEmpty());
    EXPECT_EQ(Vkind::String, var2.kind());
    EXPECT_EQ("balalaika", var2.as<std::string>());
}

TEST(Variant, constructFromEmpty)
{
    Vtype var1;
    Vtype var2(var1);

    EXPECT_EQ(Vkind::Empty, var2.kind());
    EXPECT_TRUE(var2.isEmpty());
}

TEST(Variant, moveConstructFromEmpty)
{
    Vtype var1;
    Vtype var2(std::move(var1));

    EXPECT_EQ(Vkind::Empty, var1.kind());
    EXPECT_EQ(Vkind::Empty, var2.kind());
    EXPECT_TRUE(var2.isEmpty());
}

TEST(Variant, reset)
{
    Vtype var1(int(1));

    EXPECT_EQ(Vkind::Int, var1.kind());
    var1.reset();
    EXPECT_TRUE(var1.isEmpty());
}

TEST(Variant, operatorEqValue)
{
    Vtype var1;
    EXPECT_EQ(Vkind::Empty, var1.kind());

    var1 = int(12);
    EXPECT_EQ(Vkind::Int, var1.kind());
    EXPECT_EQ(12, var1.as<int>());

    std::string asd("asd");
    var1 = asd;
    EXPECT_EQ(Vkind::String, var1.kind());
    EXPECT_EQ("asd", asd);
    EXPECT_EQ("asd", var1.as<std::string>());

    var1 = double(2.3);
    EXPECT_EQ(Vkind::Double, var1.kind());
    EXPECT_DOUBLE_EQ(2.3, var1.as<double>());
}

TEST(Variant, operatorEqMoveValue)
{
    Vtype var1;
    EXPECT_EQ(Vkind::Empty, var1.kind());

    var1 = int(12);
    EXPECT_EQ(Vkind::Int, var1.kind());
    EXPECT_EQ(12, var1.as<int>());

    std::string asd("asd");
    var1 = std::move(asd);
    EXPECT_TRUE(asd.empty());
    EXPECT_EQ(Vkind::String, var1.kind());
    EXPECT_EQ("asd", var1.as<std::string>());

    var1 = double(2.3);
    EXPECT_EQ(Vkind::Double, var1.kind());
    EXPECT_DOUBLE_EQ(2.3, var1.as<double>());
}

TEST(Variant, operatorEqOther)
{
    Vtype var0;
    Vtype var1(int(1));
    Vtype var2(double(2.2));
    Vtype var3(float(3.3));
    Vtype var4((void*)4);
    Vtype var5(std::string("balalaika"));

    var2 = var0;
    EXPECT_EQ(Vkind::Empty, var2.kind());
    var0 = var3;
    EXPECT_EQ(Vkind::Float, var0.kind());
    EXPECT_FLOAT_EQ(3.3, var0.as<float>());
    var3 = var5;
    EXPECT_EQ(Vkind::String, var3.kind());
    var5 = var4;
    EXPECT_EQ(Vkind::Voidp, var5.kind());
    var4 = var1;
    EXPECT_EQ(Vkind::Int, var4.kind());
}

TEST(Variant, operatorEqMoveOther)
{
    Vtype var0;
    Vtype var2(double(2.2));
    Vtype var4(std::string("fdsa"));
    Vtype var5(std::string("balalaika"));
    Vtype var6(float(54.1));

    var2 = std::move(var0);
    EXPECT_EQ(Vkind::Empty, var2.kind());
    EXPECT_EQ(Vkind::Empty, var0.kind());

    var0 = std::move(var4);
    EXPECT_EQ(Vkind::Empty, var4.kind());
    EXPECT_EQ(Vkind::String, var0.kind());
    EXPECT_EQ("fdsa", var0.as<std::string>());

    var5 = std::move(var6);
    EXPECT_EQ(Vkind::Empty, var6.kind());
    EXPECT_EQ(Vkind::Float, var5.kind());
    EXPECT_FLOAT_EQ(54.1, var5.as<float>());

}

TEST(Variant, operatorEqEqValue)
{
    Vtype var0;
    Vtype var4(std::string("fdsa"));
    Vtype var5(std::string("balalaika"));

    EXPECT_NE(0, var0);
    EXPECT_NE(1.1, var0);
    EXPECT_NE(std::string("balalaika"), var0); //FIXME: allow const char*

    EXPECT_NE(0, var4);
    EXPECT_NE(1.1, var4);
    EXPECT_NE(std::string("balalaika"), var4);
    EXPECT_EQ(std::string("fdsa"), var4);

    EXPECT_NE(0, var5);
    EXPECT_NE(1.1, var5);
    EXPECT_EQ(std::string("balalaika"), var5);
    EXPECT_NE(std::string("fdsa"), var5);
}

TEST(Variant, operatorEqEqOther)
{
    Vtype var0;
    Vtype var1(int(1));
    Vtype var4((void*)4);
    Vtype var5(std::string("balalaika"));
    Vtype var6(std::string("fgh"));
    Vtype var7(int(1));
    Vtype var8(std::string("fgh"));

    EXPECT_NE(var0, var1);
    EXPECT_NE(var1, var4);
    EXPECT_NE(var4, var5);
    EXPECT_NE(var6, var5);
    EXPECT_NE(var6, var0);
    EXPECT_EQ(var1, var7);
    EXPECT_EQ(var8, var6);
}
