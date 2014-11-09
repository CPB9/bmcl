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
            delete [] _bytecode;
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

    void expectNextCmdError(bmcl_status_t err)
    {
        bmcl_status_t status = bmcl_sci_interp_exec_next(_interp);
        EXPECT_EQ(err, status);
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

private:
    bmcl_memwriter_t* _bcWriter;
    uint8_t* _bytecode;
    bmcl_sci_interp_t* _interp;
};

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

TEST_F(InterpTest, sum_i8)
{
    newInterp(1);
    appendInstrHeader(BMCL_INSTR_SUM_I8);
    appendStackUint8(-53);
    appendStackUint8(-5);
    execNextCmd();
    expectStackUint8(-58);
}

TEST_F(InterpTest, sum_i16)
{
    newInterp(1);
    appendInstrHeader(BMCL_INSTR_SUM_I16);
    appendStackUint16(-10000);
    appendStackUint16(-2345);
    execNextCmd();
    expectStackUint16(-12345);
}
