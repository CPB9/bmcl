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

private:
    bmcl_memwriter_t* _bcWriter;
    uint8_t* _bytecode;
    bmcl_sci_interp_t* _interp;
};

TEST_F(InterpTest, push8)
{
    newInterp(2);
    appendInstrHeader(BMCL_INSTR_PUSH8);
    appendBcUint8(79);
    execNextCmd();
    expectStackUint8(79);
}

TEST_F(InterpTest, push8_ueob)
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
    appendBcUint16(45676);
    execNextCmd();
    expectStackUint16(45676);
}

TEST_F(InterpTest, push16_ueob)
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
