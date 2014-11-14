#include "bmcl/interp/interp.h"

#include <cassert>
#include <cstdlib>

namespace bmcl {
namespace interp {

using namespace bmcl::core;

template <typename T>
Status::Msg Interpreter::stackPush()
{
    if (_bytecode.sizeLeft() < sizeof(T)) {
        return Status::UnexpectedEndOfBytecode;
    }

    if (_stack.sizeLeft() < sizeof(T)) {
        return Status::StackOverflow;
    }

    T value = _bytecode.readType<T>();
    _stack.writeType<T>(value);
    return Status::Success;
}

template <typename T1, typename T2>
Status::Msg Interpreter::stackConvert()
{
    if (_stack.sizeLeft() < sizeof(T1)) {
        return Status::NotEnoughStackData;
    }

    T1 var1 = _stack.popType<T1>();
    if (_stack.sizeLeft() < sizeof(T2)) {
        return Status::StackOverflow;
    }

    T2 var2 = (T2)var1;
    _stack.writeType<T2>(var2);
    return Status::Success;
}

template <typename T, typename F>
Status::Msg Interpreter::stackOp(F func)
{
    if (_stack.sizeUsed() < 2 * sizeof(T)) {
        return Status::NotEnoughStackData;
    }
    T op1 = _stack.popType<T>();
    T op2 = _stack.popType<T>();
    T result = func(op1, op2);
    _stack.pushType<T>(result);
    return Status::Success;
}

// avoiding STL

template <typename T>
struct OpAdd {
    inline T operator()(T val1, T val2)
    {
        return val1 + val2;
    }
};

template <typename T>
struct OpSub {
    inline T operator()(T val1, T val2)
    {
        return val1 - val2;
    }
};

template <typename T>
struct OpMult {
    inline T operator()(T val1, T val2)
    {
        return val1 * val2;
    }
};

template <typename T>
struct OpDiv {
    inline T operator()(T val1, T val2)
    {
        return val1 / val2;
    }
};

Status::Msg Interpreter::execNext()
{
    if (_bytecode.isEmpty()) {
        return Status::UnexpectedEndOfBytecode; // TODO: eof
    }

    uint8_t instrId = _bytecode.readUint8();

    switch (instrId) {
    case Instruction::Push8:
        return stackPush<uint8_t>();
    case Instruction::Push16:
        return stackPush<uint16_t>();
    case Instruction::Push32:
        return stackPush<uint32_t>();
    case Instruction::Push64:
        return stackPush<uint64_t>();
    case Instruction::ConvertI8ToI32:
        return stackConvert<int8_t, int32_t>();
    case Instruction::ConvertI8ToU32:
        return stackConvert<int8_t, uint32_t>();
    case Instruction::ConvertU8ToI32:
        return stackConvert<uint8_t, int32_t>();
    case Instruction::ConvertU8ToU32:
        return stackConvert<uint8_t, uint32_t>();
    case Instruction::ConvertI16ToI32:
        return stackConvert<int16_t, int32_t>();
    case Instruction::ConvertI16ToU32:
        return stackConvert<int16_t, uint32_t>();
    case Instruction::ConvertU16ToI32:
        return stackConvert<uint16_t, int32_t>();
    case Instruction::ConvertU16ToU32:
        return stackConvert<uint16_t, uint32_t>();
    case Instruction::ConvertI32ToI8:
        return stackConvert<int32_t, int8_t>();
    case Instruction::ConvertI32ToU8:
        return stackConvert<int32_t, uint8_t>();
    case Instruction::ConvertI32ToI16:
        return stackConvert<int32_t, int16_t>();
    case Instruction::ConvertI32ToU16:
        return stackConvert<int32_t, uint16_t>();
    case Instruction::ConvertI32ToI64:
        return stackConvert<int32_t, int64_t>();
    case Instruction::ConvertI32ToU64:
        return stackConvert<int32_t, uint64_t>();
    case Instruction::ConvertI32ToFloat:
        return stackConvert<int32_t, float>();
    case Instruction::ConvertI32ToDouble:
        return stackConvert<int32_t, double>();
    case Instruction::ConvertU32ToI8:
        return stackConvert<uint32_t, int8_t>();
    case Instruction::ConvertU32ToU8:
        return stackConvert<uint32_t, uint8_t>();
    case Instruction::ConvertU32ToI16:
        return stackConvert<uint32_t, int16_t>();
    case Instruction::ConvertU32ToU16:
        return stackConvert<uint32_t, uint16_t>();
    case Instruction::ConvertU32ToI64:
        return stackConvert<uint32_t, int64_t>();
    case Instruction::ConvertU32ToU64:
        return stackConvert<uint32_t, uint64_t>();
    case Instruction::ConvertU32ToFloat:
        return stackConvert<uint32_t, float>();
    case Instruction::ConvertU32ToDouble:
        return stackConvert<uint32_t, double>();
    case Instruction::ConvertI64ToI32:
        return stackConvert<int64_t, int32_t>();
    case Instruction::ConvertI64ToU32:
        return stackConvert<int64_t, uint32_t>();
    case Instruction::ConvertI64ToFloat:
        return stackConvert<int64_t, float>();
    case Instruction::ConvertI64ToDouble:
        return stackConvert<int64_t, double>();
    case Instruction::ConvertU64ToI32:
        return stackConvert<uint64_t, int32_t>();
    case Instruction::ConvertU64ToU32:
        return stackConvert<uint64_t, uint32_t>();
    case Instruction::ConvertU64ToFloat:
        return stackConvert<uint64_t, float>();
    case Instruction::ConvertU64ToDouble:
        return stackConvert<uint64_t, double>();
    case Instruction::ConvertFloatToI32:
        return stackConvert<float, int32_t>();
    case Instruction::ConvertFloatToU32:
        return stackConvert<float, uint32_t>();
    case Instruction::ConvertFloatToI64:
        return stackConvert<float, int64_t>();
    case Instruction::ConvertFloatToU64:
        return stackConvert<float, uint64_t>();
    case Instruction::ConvertFloatToDouble:
        return stackConvert<float, double>();
    case Instruction::ConvertDoubleToI32:
        return stackConvert<double, int32_t>();
    case Instruction::ConvertDoubleToU32:
        return stackConvert<double, uint32_t>();
    case Instruction::ConvertDoubleToI64:
        return stackConvert<double, int64_t>();
    case Instruction::ConvertDoubleToU64:
        return stackConvert<double, uint64_t>();
    case Instruction::ConvertDoubleToFloat:
        return stackConvert<double, float>();
    case Instruction::AddI8:
        return stackOp<int8_t>(OpAdd<int8_t>());
    case Instruction::AddI16:
        return stackOp<int16_t>(OpAdd<int16_t>());
    case Instruction::AddI32:
        return stackOp<int32_t>(OpAdd<int32_t>());
    case Instruction::AddI64:
        return stackOp<int64_t>(OpAdd<int64_t>());
    case Instruction::AddU8:
        return stackOp<uint8_t>(OpAdd<uint8_t>());
    case Instruction::AddU16:
        return stackOp<uint16_t>(OpAdd<uint16_t>());
    case Instruction::AddU32:
        return stackOp<uint32_t>(OpAdd<uint32_t>());
    case Instruction::AddU64:
        return stackOp<uint64_t>(OpAdd<uint64_t>());
    case Instruction::AddFloat:
        return stackOp<float>(OpAdd<float>());
    case Instruction::AddDouble:
        return stackOp<double>(OpAdd<double>());
    case Instruction::SubI8:
        return stackOp<int8_t>(OpSub<int8_t>());
    case Instruction::SubI16:
        return stackOp<int16_t>(OpSub<int16_t>());
    case Instruction::SubI32:
        return stackOp<int32_t>(OpSub<int32_t>());
    case Instruction::SubI64:
        return stackOp<int64_t>(OpSub<int64_t>());
    case Instruction::SubU8:
        return stackOp<uint8_t>(OpSub<uint8_t>());
    case Instruction::SubU16:
        return stackOp<uint16_t>(OpSub<uint16_t>());
    case Instruction::SubU32:
        return stackOp<uint32_t>(OpSub<uint32_t>());
    case Instruction::SubU64:
        return stackOp<uint64_t>(OpSub<uint64_t>());
    case Instruction::SubFloat:
        return stackOp<float>(OpSub<float>());
    case Instruction::SubDouble:
        return stackOp<double>(OpSub<double>());
    case Instruction::MultI8:
        return stackOp<int8_t>(OpMult<int8_t>());
    case Instruction::MultI16:
        return stackOp<int16_t>(OpMult<int16_t>());
    case Instruction::MultI32:
        return stackOp<int32_t>(OpMult<int32_t>());
    case Instruction::MultI64:
        return stackOp<int64_t>(OpMult<int64_t>());
    case Instruction::MultU8:
        return stackOp<uint8_t>(OpMult<uint8_t>());
    case Instruction::MultU16:
        return stackOp<uint16_t>(OpMult<uint16_t>());
    case Instruction::MultU32:
        return stackOp<uint32_t>(OpMult<uint32_t>());
    case Instruction::MultU64:
        return stackOp<uint64_t>(OpMult<uint64_t>());
    case Instruction::MultFloat:
        return stackOp<float>(OpMult<float>());
    case Instruction::MultDouble:
        return stackOp<double>(OpMult<double>());
    case Instruction::DivI8:
        return stackOp<int8_t>(OpDiv<int8_t>());
    case Instruction::DivI16:
        return stackOp<int16_t>(OpDiv<int16_t>());
    case Instruction::DivI32:
        return stackOp<int32_t>(OpDiv<int32_t>());
    case Instruction::DivI64:
        return stackOp<int64_t>(OpDiv<int64_t>());
    case Instruction::DivU8:
        return stackOp<uint8_t>(OpDiv<uint8_t>());
    case Instruction::DivU16:
        return stackOp<uint16_t>(OpDiv<uint16_t>());
    case Instruction::DivU32:
        return stackOp<uint32_t>(OpDiv<uint32_t>());
    case Instruction::DivU64:
        return stackOp<uint64_t>(OpDiv<uint64_t>());
    case Instruction::DivFloat:
        return stackOp<float>(OpDiv<float>());
    case Instruction::DivDouble:
        return stackOp<double>(OpDiv<double>());
    }

    return Status::InvalidInstruction;
}
}
}
