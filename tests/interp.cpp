#include "bmcl/interp/interp.h"

#include "bmcl-test.h"

#include <algorithm>

using namespace bmcl::core;
using namespace bmcl::interp;

class InterpTest : public ::testing::Test {
protected:
    void SetUp()
    {
        _interp = 0;
    }

    void TearDown()
    {
        if (_interp) {
            delete _bcWriter;
            delete _interp;
            delete[] _bytecode;
        }
    }

    void newInterp(std::size_t bytecodeSize, std::size_t stackSize = 1000)
    {
        assert(_interp == 0);
        _bytecode = new uint8_t[bytecodeSize];
        _bcWriter = new MemWriter(_bytecode, bytecodeSize);
        _interp = new Interpreter(_bytecode, bytecodeSize, stackSize);
    }

    void execNextCmd()
    {
        auto status = _interp->execNext();
        EXPECT_EQ(Status::Success, status) << Status::toCString(status);
    }

    template <typename T>
    void appendBytecode(T value)
    {
        _bcWriter->writeType<T>(value);
    }

    void appendInstrHeader(Interpreter::Instruction type)
    {
        _bcWriter->writeUint8((uint8_t)type);
    }

    template <typename T>
    void appendStack(T value)
    {
        _interp->stack()->writeType<T>(value);
    }

    void appendStackBytes(uint8_t byte, std::size_t count)
    {
        _interp->stack()->fill(byte, count);
    }

    void expectNextCmdError(Status::Msg err)
    {
        auto status = _interp->execNext();
        EXPECT_EQ(err, status);
    }

    template <typename T>
    void expectStack(T expected)
    {
        T value = _interp->stack()->popType<T>();
        EXPECT_EQ(expected, value);
    }

private:
    MemWriter* _bcWriter;
    uint8_t* _bytecode;
    Interpreter* _interp;
};

#define PUSH_TEST(name, type, instr, value)                                                                            \
    TEST_F(InterpTest, name)                                                                                           \
    {                                                                                                                  \
        newInterp(1 + sizeof(type));                                                                                   \
        appendInstrHeader(Interpreter::instr);                                                                         \
        appendBytecode<type>(value);                                                                                   \
        execNextCmd();                                                                                                 \
        expectStack<type>(value);                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    TEST_F(InterpTest, name##_stack_overflow)                                                                          \
    {                                                                                                                  \
        newInterp(1 + sizeof(type), sizeof(type) - 1);                                                                 \
        appendInstrHeader(Interpreter::instr);                                                                         \
        appendBytecode<type>(value);                                                                                   \
        expectNextCmdError(Status::StackOverflow);                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    TEST_F(InterpTest, name##_unexpected_end_of_bytecode)                                                              \
    {                                                                                                                  \
        newInterp(1 + sizeof(type) - 1, sizeof(type));                                                                 \
        appendInstrHeader(Interpreter::instr);                                                                         \
        expectNextCmdError(Status::UnexpectedEndOfBytecode);                                                           \
    }

PUSH_TEST(push8, uint8_t, Push8, 0xba);
PUSH_TEST(push16, uint16_t, Push16, 0x1234);
PUSH_TEST(push32, uint32_t, Push32, 0xaabbccdd);
PUSH_TEST(push64, uint64_t, Push64, 0x9988776655443322);

#define CONV_TEST(name, type1, type2, instr, val, res)                                                                 \
    TEST_F(InterpTest, convert_##name)                                                                                 \
    {                                                                                                                  \
        newInterp(1, std::max(sizeof(type1), sizeof(type2)));                                                          \
        appendInstrHeader(Interpreter::instr);                                                                         \
        appendStack<type1>(val);                                                                                       \
        execNextCmd();                                                                                                 \
        expectStack<type2>(res);                                                                                       \
    }                                                                                                                  \
                                                                                                                       \
    TEST_F(InterpTest, convert_##name##_invalid_stack_size)                                                            \
    {                                                                                                                  \
        if (sizeof(type2) > sizeof(type1)) {                                                                           \
            newInterp(1, sizeof(type2) - 1);                                                                           \
            appendInstrHeader(Interpreter::instr);                                                                     \
            appendStack<type1>(val);                                                                                   \
            expectNextCmdError(Status::StackOverflow);                                                                 \
        } else {                                                                                                       \
            newInterp(1, sizeof(type1) - 1);                                                                           \
            appendInstrHeader(Interpreter::instr);                                                                     \
            appendStackBytes(0xff, sizeof(type1) - 1);                                                                 \
            expectNextCmdError(Status::NotEnoughStackData);                                                            \
        }                                                                                                              \
    }

CONV_TEST(i8_to_i32, int8_t, int32_t, ConvertI8ToI32, -101, -101);
CONV_TEST(i8_to_u32, int8_t, uint32_t, ConvertI8ToU32, 122, 122);
CONV_TEST(u8_to_i32, uint8_t, int32_t, ConvertU8ToI32, 222, 222);
CONV_TEST(u8_to_u32, uint8_t, uint32_t, ConvertU8ToU32, 233, 233);
CONV_TEST(i16_to_i32, int16_t, int32_t, ConvertI16ToI32, -31186, -31186);
CONV_TEST(i16_to_u32, int16_t, uint32_t, ConvertI16ToU32, 21254, 21254);
CONV_TEST(u16_to_i32, uint16_t, int32_t, ConvertU16ToI32, 61867, 61867);
CONV_TEST(u16_to_u32, uint16_t, uint32_t, ConvertU16ToU32, 61877, 61877);
CONV_TEST(i32_to_i8, int32_t, int8_t, ConvertI32ToI8, -122, -122);
CONV_TEST(i32_to_i16, int32_t, int16_t, ConvertI32ToI16, -32011, -32011);
CONV_TEST(i32_to_i64, int32_t, int64_t, ConvertI32ToI64, -2107480648, -2107480648);
CONV_TEST(i32_to_u8, int32_t, uint8_t, ConvertI32ToU8, 212, 212);
CONV_TEST(i32_to_u16, int32_t, uint16_t, ConvertI32ToU16, 32011, 32011);
CONV_TEST(i32_to_u64, int32_t, uint64_t, ConvertI32ToU64, 2107480648, 2107480648);
CONV_TEST(i32_to_float, int32_t, float, ConvertI32ToFloat, -8088608, -8088608);
CONV_TEST(i32_to_double, int32_t, double, ConvertI32ToDouble, -2147000001, -2147000001);

#define OP_TEST(name, type, instr, op1, op2, res)                                                                      \
    TEST_F(InterpTest, name)                                                                                           \
    {                                                                                                                  \
        newInterp(1, sizeof(type) * 2);                                                                                \
        appendInstrHeader(Interpreter::instr);                                                                         \
        appendStack<type>(op1);                                                                                        \
        appendStack<type>(op2);                                                                                        \
        execNextCmd();                                                                                                 \
        expectStack<type>(res);                                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    TEST_F(InterpTest, name##_not_enough_stack_data)                                                                   \
    {                                                                                                                  \
        newInterp(1, sizeof(type) * 2 - 1);                                                                            \
        appendInstrHeader(Interpreter::instr);                                                                         \
        appendStackBytes(0xff, sizeof(type) * 2 - 1);                                                                  \
        expectNextCmdError(Status::NotEnoughStackData);                                                                \
    }

OP_TEST(add_i8, int8_t, AddI8, -53, -5, -58);
OP_TEST(add_i16, int16_t, AddI16, -10000, -2345, -12345);
OP_TEST(add_i32, int32_t, AddI32, -2100000000l, 100000000l, -2000000000l);
OP_TEST(add_i64, int64_t, AddI64, 9000000000000000000ll, 100000000000000000ll, 9100000000000000000ll);
OP_TEST(add_u8, uint8_t, AddU8, 12, 222, 234);
OP_TEST(add_u16, uint16_t, AddU16, 30000, 123, 30123);
OP_TEST(add_u32, uint32_t, AddU32, 4000000000ul, 100000001ul, 4100000001ul);
OP_TEST(add_u64, uint64_t, AddU64, 18000000000000000000ull, 111111111111111111ull, 18111111111111111111ull);
OP_TEST(add_float, float, AddFloat, -12.1234, 45.09875, 32.97535);
OP_TEST(add_double, double, AddDouble, -1209876.123445678, 45245.0987523467, -1164631.0246933314);
OP_TEST(sub_i8, int8_t, SubI8, 5, -95, -100);
OP_TEST(sub_i16, int16_t, SubI16, 123, -30000, -30123);
OP_TEST(sub_i32, int32_t, SubI32, 111111111l, -2000000000l, -2111111111l);
OP_TEST(sub_i64, int64_t, SubI64, 20000000000000000ll, -9200000000000000000ll, -9220000000000000000ll);
OP_TEST(sub_u8, uint8_t, SubU8, 33, 233, 200);
OP_TEST(sub_u16, uint16_t, SubU16, 5000, 65000, 60000);
OP_TEST(sub_u32, uint32_t, SubU32, 295ul, 4000000295ul, 4000000000ul);
OP_TEST(sub_u64, uint64_t, SubU64, 20000000000000000ull, 18000000000000000000ull, 17980000000000000000ull);
OP_TEST(sub_float, float, SubFloat, 3.234, 13.234, 10);
OP_TEST(sub_double, double, SubDouble, 98564.987654, 4643.098856645, -93921.888797355);
OP_TEST(mult_i8, int8_t, MultI8, 6, -20, -120);
OP_TEST(mult_i16, int16_t, MultI16, -42, 743, -31206);
OP_TEST(mult_i32, int32_t, MultI32, 79874l, -26885l, -2147412490l);
OP_TEST(mult_i64, int64_t, MultI64, -10575638543ll, 872133819ll, -9223372030870185717ll);
OP_TEST(mult_u8, uint8_t, MultU8, 4, 50, 200);
OP_TEST(mult_u16, uint16_t, MultU16, 543, 120, 65160);
OP_TEST(mult_u32, uint32_t, MultU32, 98712ul, 43510ul, 4294959120ul);
OP_TEST(mult_u64, uint64_t, MultU64, 49843287116ull, 370094854ull, 18446744068076101064ull);
OP_TEST(mult_float, float, MultFloat, 2.23, 4.876, 10.87348);
OP_TEST(mult_double, double, MultDouble, 123.0954, 245.9987564, 30281.31531856056);
OP_TEST(div_i8, int8_t, DivI8, -7, 120, -17);
OP_TEST(div_i16, int16_t, DivI16, 123, -32011, -260);
OP_TEST(div_i32, int32_t, DivI32, -198543l, 2147483601l, -10816l);
OP_TEST(div_i64, int64_t, DivI64, 92336190ll, -9220372030804000ll, -99856535ll);
OP_TEST(div_u8, uint8_t, DivU8, 53, 255, 4);
OP_TEST(div_u16, uint16_t, DivU16, 5833, 65000, 11);
OP_TEST(div_u32, uint32_t, DivU32, 687543ul, 4294067097ul, 6245ul);
OP_TEST(div_u64, uint64_t, DivU64, 19875643ull, 18446123373544750007ull, 928076811076ull);
OP_TEST(div_float, float, DivFloat, 8.88, 2.22, 0.25);
OP_TEST(div_double, double, DivDouble, 9875.64, 183142.1636976, 18.54484);
