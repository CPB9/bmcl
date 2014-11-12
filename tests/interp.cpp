#include "bmcl/interp/interp.h"

#include "bmcl-test.h"

class InterpTest : public ::testing::Test {
protected:
    void SetUp()
    {
        _interp = 0;
    }

    void TearDown()
    {
        if (_interp) {
            bmcl_memwriter_destroy(_bcWriter);
            bmcl_sci_interp_destroy(_interp);
            delete[] _bytecode;
        }
    }

    void newInterp(std::size_t bytecodeSize, std::size_t stackSize = 1000)
    {
        assert(_interp == 0);
        _bytecode = new uint8_t[bytecodeSize];
        _bcWriter = bmcl_memwriter_create_with_dest(_bytecode, bytecodeSize);
        _interp = bmcl_sci_interp_create(_bytecode, bytecodeSize, stackSize);
    }

    void execNextCmd()
    {
        bmcl_status_t status = bmcl_sci_interp_exec_next(_interp);
        EXPECT_EQ(BMCL_SUCCESS, status) << bmcl_strerror(status);
    }

    void appendInstrHeader(bmcl_sci_instr_type_t type)
    {
        bmcl_memwriter_write_uint8(_bcWriter, (uint8_t)type);
    }

    void appendBcUint8(uint8_t value)
    {
        bmcl_memwriter_write_uint8(_bcWriter, value);
    }

    void appendBcUint16(uint16_t value)
    {
        bmcl_memwriter_write_uint16be(_bcWriter, value);
    }

    void appendBcUint32(uint32_t value)
    {
        bmcl_memwriter_write_uint32be(_bcWriter, value);
    }

    void appendBcUint64(uint64_t value)
    {
        bmcl_memwriter_write_uint64be(_bcWriter, value);
    }

    template <typename T>
    void appendStack(T value)
    {
        bmcl_memwriter_write(&_interp->stack, &value, sizeof(value));
    }

    void appendStackUint8(uint8_t value)
    {
        bmcl_memwriter_write_uint8(&_interp->stack, value);
    }

    void appendStackUint16(uint16_t value)
    {
        bmcl_memwriter_write_uint16(&_interp->stack, value);
    }

    void appendStackUint32(uint32_t value)
    {
        bmcl_memwriter_write_uint32(&_interp->stack, value);
    }

    void appendStackUint64(uint64_t value)
    {
        bmcl_memwriter_write_uint64(&_interp->stack, value);
    }

    void appendStackFloat(float value)
    {
        bmcl_memwriter_write(&_interp->stack, &value, sizeof(value));
    }

    void appendStackDouble(double value)
    {
        bmcl_memwriter_write(&_interp->stack, &value, sizeof(value));
    }

    void expectNextCmdError(bmcl_status_t err)
    {
        bmcl_status_t status = bmcl_sci_interp_exec_next(_interp);
        EXPECT_EQ(err, status);
    }

    template <typename T>
    void expectStack(T expected)
    {
        T value;
        bmcl_memwriter_pop(&_interp->stack, &value, sizeof(value));
        EXPECT_EQ(expected, value);
    }

    void expectStackUint8(uint8_t value)
    {
        EXPECT_EQ(value, bmcl_memwriter_pop_uint8(&_interp->stack));
    }

    void expectStackUint16(uint16_t value)
    {
        EXPECT_EQ(value, bmcl_memwriter_pop_uint16(&_interp->stack));
    }

    void expectStackUint32(uint32_t value)
    {
        EXPECT_EQ(value, bmcl_memwriter_pop_uint32(&_interp->stack));
    }

    void expectStackUint64(uint64_t value)
    {
        EXPECT_EQ(value, bmcl_memwriter_pop_uint64(&_interp->stack));
    }

    void expectStackFloat(float expected)
    {
        float value;
        bmcl_memwriter_pop(&_interp->stack, &value, sizeof(value));
        EXPECT_FLOAT_EQ(expected, value);
    }

    void expectStackDouble(double expected)
    {
        double value;
        bmcl_memwriter_pop(&_interp->stack, &value, sizeof(value));
        EXPECT_DOUBLE_EQ(expected, value);
    }

private:
    bmcl_memwriter_t* _bcWriter;
    uint8_t* _bytecode;
    bmcl_sci_interp_t* _interp;
};

template <>
void InterpTest::expectStack<double>(double expected)
{
    double value;
    bmcl_memwriter_pop(&_interp->stack, &value, sizeof(value));
    EXPECT_DOUBLE_EQ(expected, value);
}

template <>
void InterpTest::expectStack<float>(float expected)
{
    float value;
    bmcl_memwriter_pop(&_interp->stack, &value, sizeof(value));
    EXPECT_FLOAT_EQ(expected, value);
}

TEST_F(InterpTest, push8)
{
    newInterp(2);
    appendInstrHeader(BMCL_INSTR_PUSH8);
    appendBcUint8(0xba);
    execNextCmd();
    expectStackUint8(0xba);
}

TEST_F(InterpTest, push8_ueobc)
{
    newInterp(1);
    appendInstrHeader(BMCL_INSTR_PUSH8);
    expectNextCmdError(BMCL_ERR_UNEXPECTED_END_OF_BYTECODE);
}

TEST_F(InterpTest, push8_stack_overflow)
{
    newInterp(2, 0);
    appendInstrHeader(BMCL_INSTR_PUSH8);
    appendBcUint8(13);
    expectNextCmdError(BMCL_ERR_STACK_OVERFLOW);
}

TEST_F(InterpTest, push16)
{
    newInterp(3);
    appendInstrHeader(BMCL_INSTR_PUSH16);
    appendBcUint16(0x1234);
    execNextCmd();
    expectStackUint16(0x1234);
}

TEST_F(InterpTest, push16_ueobc)
{
    newInterp(2);
    appendInstrHeader(BMCL_INSTR_PUSH16);
    appendBcUint8(122);
    expectNextCmdError(BMCL_ERR_UNEXPECTED_END_OF_BYTECODE);
}

TEST_F(InterpTest, push16_stack_overflow)
{
    newInterp(3, 1);
    appendInstrHeader(BMCL_INSTR_PUSH16);
    appendBcUint16(12334);
    expectNextCmdError(BMCL_ERR_STACK_OVERFLOW);
}

TEST_F(InterpTest, push32)
{
    newInterp(5);
    appendInstrHeader(BMCL_INSTR_PUSH32);
    appendBcUint32(0xaabbccdd);
    execNextCmd();
    expectStackUint32(0xaabbccdd);
}

TEST_F(InterpTest, push32_ueobc)
{
    newInterp(4);
    appendInstrHeader(BMCL_INSTR_PUSH32);
    appendBcUint8(123);
    appendBcUint16(123);
    expectNextCmdError(BMCL_ERR_UNEXPECTED_END_OF_BYTECODE);
}

TEST_F(InterpTest, push32_stack_overflow)
{
    newInterp(5, 3);
    appendInstrHeader(BMCL_INSTR_PUSH32);
    appendBcUint32(0xaabbccdd);
    expectNextCmdError(BMCL_ERR_STACK_OVERFLOW);
}

TEST_F(InterpTest, push64)
{
    newInterp(9);
    appendInstrHeader(BMCL_INSTR_PUSH64);
    appendBcUint64(0xaabbccdd22445566);
    execNextCmd();
    expectStackUint64(0xaabbccdd22445566);
}

TEST_F(InterpTest, push64_ueobc)
{
    newInterp(8);
    appendInstrHeader(BMCL_INSTR_PUSH64);
    appendBcUint32(123);
    appendBcUint16(123);
    appendBcUint8(123);
    expectNextCmdError(BMCL_ERR_UNEXPECTED_END_OF_BYTECODE);
}

TEST_F(InterpTest, push64_stack_overflow)
{
    newInterp(9, 7);
    appendInstrHeader(BMCL_INSTR_PUSH64);
    appendBcUint64(0x1234567809876543);
    expectNextCmdError(BMCL_ERR_STACK_OVERFLOW);
}

TEST_F(InterpTest, convert_i8_to_i32)
{
    newInterp(1);
    appendInstrHeader(BMCL_INSTR_CONVERT_I8_TO_I32);
    appendStackUint8(-101);
    execNextCmd();
    expectStackUint32(-101);
}

TEST_F(InterpTest, convert_i8_to_u32)
{
    newInterp(1);
    appendInstrHeader(BMCL_INSTR_CONVERT_I8_TO_U32);
    appendStackUint8(122);
    execNextCmd();
    expectStackUint32(122);
}

TEST_F(InterpTest, convert_u8_to_i32)
{
    newInterp(1);
    appendInstrHeader(BMCL_INSTR_CONVERT_U8_TO_I32);
    appendStackUint8(242);
    execNextCmd();
    expectStackUint32(242);
}

TEST_F(InterpTest, convert_u8_to_u32)
{
    newInterp(1);
    appendInstrHeader(BMCL_INSTR_CONVERT_U8_TO_U32);
    appendStackUint8(55);
    execNextCmd();
    expectStackUint32(55);
}

TEST_F(InterpTest, convert_i16_to_i32)
{
    newInterp(1);
    appendInstrHeader(BMCL_INSTR_CONVERT_I16_TO_I32);
    appendStackUint16(-31186);
    execNextCmd();
    expectStackUint32(-31186);
}

TEST_F(InterpTest, convert_i16_to_u32)
{
    newInterp(1);
    appendInstrHeader(BMCL_INSTR_CONVERT_I16_TO_U32);
    appendStackUint16(21254);
    execNextCmd();
    expectStackUint32(21254);
}

TEST_F(InterpTest, convert_u16_to_i32)
{
    newInterp(1);
    appendInstrHeader(BMCL_INSTR_CONVERT_U16_TO_I32);
    appendStackUint16(61876);
    execNextCmd();
    expectStackUint32(61876);
}

TEST_F(InterpTest, convert_u16_to_u32)
{
    newInterp(1);
    appendInstrHeader(BMCL_INSTR_CONVERT_U16_TO_U32);
    appendStackUint16(61873);
    execNextCmd();
    expectStackUint32(61873);
}

#define OP_TEST(name, type, instr, op1, op2, res)                                                                      \
    TEST_F(InterpTest, name)                                                                                           \
    {                                                                                                                  \
        newInterp(1);                                                                                                  \
        appendInstrHeader(instr);                                                                                      \
        appendStack<type>(op1);                                                                                        \
        appendStack<type>(op2);                                                                                        \
        execNextCmd();                                                                                                 \
        expectStack<type>(res);                                                                                        \
    }

OP_TEST(add_i8, int8_t, BMCL_INSTR_ADD_I8, -53, -5, -58);
OP_TEST(add_i16, int16_t, BMCL_INSTR_ADD_I16, -10000, -2345, -12345);
OP_TEST(add_i32, int32_t, BMCL_INSTR_ADD_I32, -2100000000l, 100000000l, -2000000000l);
OP_TEST(add_i64, int64_t, BMCL_INSTR_ADD_I64, 9000000000000000000ll, 100000000000000000ll, 9100000000000000000ll);
OP_TEST(add_u8, uint8_t, BMCL_INSTR_ADD_U8, 12, 222, 234);
OP_TEST(add_u16, uint16_t, BMCL_INSTR_ADD_U16, 30000, 123, 30123);
OP_TEST(add_u32, uint32_t, BMCL_INSTR_ADD_U32, 4000000000ul, 100000001ul, 4100000001ul);
OP_TEST(add_u64, uint64_t, BMCL_INSTR_ADD_U64, 18000000000000000000ull, 111111111111111111ull, 18111111111111111111ull);
OP_TEST(add_float, float, BMCL_INSTR_ADD_FLOAT, -12.1234, 45.09875, 32.97535);
OP_TEST(add_double, double, BMCL_INSTR_ADD_DOUBLE, -1209876.123445678, 45245.0987523467, -1164631.0246933314);
OP_TEST(sub_i8, int8_t, BMCL_INSTR_SUB_I8, 5, -95, -100);
OP_TEST(sub_i16, int16_t, BMCL_INSTR_SUB_I16, 123, -30000, -30123);
OP_TEST(sub_i32, int32_t, BMCL_INSTR_SUB_I32, 111111111l, -2000000000l, -2111111111l);
OP_TEST(sub_i64, int64_t, BMCL_INSTR_SUB_I64, 20000000000000000ll, -9200000000000000000ll, -9220000000000000000ll);
OP_TEST(sub_u8, uint8_t, BMCL_INSTR_SUB_U8, 33, 233, 200);
OP_TEST(sub_u16, uint16_t, BMCL_INSTR_SUB_U16, 5000, 65000, 60000);
OP_TEST(sub_u32, uint32_t, BMCL_INSTR_SUB_U32, 295ul, 4000000295ul, 4000000000ul);
OP_TEST(sub_u64, uint64_t, BMCL_INSTR_SUB_U64, 20000000000000000ull, 18000000000000000000ull, 17980000000000000000ull);
OP_TEST(sub_float, float, BMCL_INSTR_SUB_FLOAT, 3.234, 13.234, 10);
OP_TEST(sub_double, double, BMCL_INSTR_SUB_DOUBLE, 98564.987654, 4643.098856645, -93921.888797355);
OP_TEST(mult_i8, int8_t, BMCL_INSTR_MULT_I8, 6, -20, -120);
OP_TEST(mult_i16, int16_t, BMCL_INSTR_MULT_I16, -42, 743, -31206);
OP_TEST(mult_i32, int32_t, BMCL_INSTR_MULT_I32, 79874l, -26885l, -2147412490l);
OP_TEST(mult_i64, int64_t, BMCL_INSTR_MULT_I64, -10575638543ll, 872133819ll, -9223372030870185717ll);
OP_TEST(mult_u8, uint8_t, BMCL_INSTR_MULT_U8, 4, 50, 200);
OP_TEST(mult_u16, uint16_t, BMCL_INSTR_MULT_U16, 543, 120, 65160);
OP_TEST(mult_u32, uint32_t, BMCL_INSTR_MULT_U32, 98712ul, 43510ul, 4294959120ul);
OP_TEST(mult_u64, uint64_t, BMCL_INSTR_MULT_U64, 49843287116ull, 370094854ull, 18446744068076101064ull);
OP_TEST(mult_float, float, BMCL_INSTR_MULT_FLOAT, 2.23, 4.876, 10.87348);
OP_TEST(mult_double, double, BMCL_INSTR_MULT_DOUBLE, 123.0954, 245.9987564, 30281.31531856056);
OP_TEST(div_i8, int8_t, BMCL_INSTR_DIV_I8, -7, 120, -17);
OP_TEST(div_i16, int16_t, BMCL_INSTR_DIV_I16, 123, -32011, -260);
OP_TEST(div_i32, int32_t, BMCL_INSTR_DIV_I32, -198543l, 2147483601l, -10816l);
OP_TEST(div_i64, int64_t, BMCL_INSTR_DIV_I64, 92336190ll, -9220372030804000ll, -99856535ll);
OP_TEST(div_u8, uint8_t, BMCL_INSTR_DIV_U8, 53, 255, 4);
OP_TEST(div_u16, uint16_t, BMCL_INSTR_DIV_U16, 5833, 65000, 11);
OP_TEST(div_u32, uint32_t, BMCL_INSTR_DIV_U32, 687543ul, 4294067097ul, 6245ul);
OP_TEST(div_u64, uint64_t, BMCL_INSTR_DIV_U64, 19875643ull, 18446123373544750007ull, 928076811076ull);
OP_TEST(div_float, float, BMCL_INSTR_DIV_FLOAT, 8.88, 2.22, 0.25);
OP_TEST(div_double, double, BMCL_INSTR_DIV_DOUBLE, 9875.64, 183142.1636976, 18.54484);

