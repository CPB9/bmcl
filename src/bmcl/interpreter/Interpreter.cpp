/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/config.h"
#include "bmcl/interpreter/Interpreter.h"

#include <cassert>
#include <cstdlib>

namespace bmcl {
namespace interpreter {

using namespace bmcl::core;

// TODO: static assert

Interpreter::Interpreter(const void* bytecode, std::size_t bytecodeSize, void* stack, std::size_t stackSize)
    : _bytecode(bytecode, bytecodeSize)
    , _stack(stack, stackSize)
{
    assert(std::numeric_limits<double>::is_iec559);
    assert(std::numeric_limits<float>::is_iec559);
}

Interpreter::Interpreter(const void* bytecode, std::size_t bytecodeSize, std::size_t stackSize)
    : _bytecode(bytecode, bytecodeSize)
    , _stack(stackSize)
{
    assert(std::numeric_limits<double>::is_iec559);
    assert(std::numeric_limits<float>::is_iec559);
}

template <typename T>
Status::Msg Interpreter::stackPush()
{
    if (_bytecode.sizeLeft() < sizeof(T)) {
        return Status::UnexpectedEndOfBytecode;
    }

    if (_stack.availableSize() < sizeof(T)) {
        return Status::StackOverflow;
    }

    T value = _bytecode.readType<T>();
    _stack.writeType<T>(value);
    return Status::Success;
}

template <typename T1, typename T2>
Status::Msg Interpreter::stackConvert()
{
    if (_stack.sizeUsed() < sizeof(T1)) {
        return Status::NotEnoughStackData;
    }

    T1 var1 = _stack.popType<T1>();
    if (_stack.availableSize() < sizeof(T2)) {
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
    inline T operator()(T val1, T val2) const { return val1 + val2; }
};

template <typename T>
struct OpSub {
    inline T operator()(T val1, T val2) const { return val1 - val2; }
};

template <typename T>
struct OpMult {
    inline T operator()(T val1, T val2) const { return val1 * val2; }
};

template <typename T>
struct OpDiv {
    inline T operator()(T val1, T val2) const { return val1 / val2; }
};

Status::Msg Interpreter::execNext()
{
    if (_bytecode.isEmpty()) {
        return Status::UnexpectedEndOfBytecode; // TODO: eof
    }

    uint8_t instrId = _bytecode.readUint8();

    switch (instrId) {
    case Interpreter::Push8:
        return stackPush<uint8_t>();
    case Interpreter::Push16:
        return stackPush<uint16_t>();
    case Interpreter::Push32:
        return stackPush<uint32_t>();
    case Interpreter::Push64:
        return stackPush<uint64_t>();
    case Interpreter::ConvertI8ToI32:
        return stackConvert<int8_t, int32_t>();
    case Interpreter::ConvertI8ToU32:
        return stackConvert<int8_t, uint32_t>();
    case Interpreter::ConvertU8ToI32:
        return stackConvert<uint8_t, int32_t>();
    case Interpreter::ConvertU8ToU32:
        return stackConvert<uint8_t, uint32_t>();
    case Interpreter::ConvertI16ToI32:
        return stackConvert<int16_t, int32_t>();
    case Interpreter::ConvertI16ToU32:
        return stackConvert<int16_t, uint32_t>();
    case Interpreter::ConvertU16ToI32:
        return stackConvert<uint16_t, int32_t>();
    case Interpreter::ConvertU16ToU32:
        return stackConvert<uint16_t, uint32_t>();
    case Interpreter::ConvertI32ToI8:
        return stackConvert<int32_t, int8_t>();
    case Interpreter::ConvertI32ToU8:
        return stackConvert<int32_t, uint8_t>();
    case Interpreter::ConvertI32ToI16:
        return stackConvert<int32_t, int16_t>();
    case Interpreter::ConvertI32ToU16:
        return stackConvert<int32_t, uint16_t>();
    case Interpreter::ConvertI32ToI64:
        return stackConvert<int32_t, int64_t>();
    case Interpreter::ConvertI32ToU64:
        return stackConvert<int32_t, uint64_t>();
    case Interpreter::ConvertI32ToFloat:
        return stackConvert<int32_t, float>();
    case Interpreter::ConvertI32ToDouble:
        return stackConvert<int32_t, double>();
    case Interpreter::ConvertU32ToI8:
        return stackConvert<uint32_t, int8_t>();
    case Interpreter::ConvertU32ToU8:
        return stackConvert<uint32_t, uint8_t>();
    case Interpreter::ConvertU32ToI16:
        return stackConvert<uint32_t, int16_t>();
    case Interpreter::ConvertU32ToU16:
        return stackConvert<uint32_t, uint16_t>();
    case Interpreter::ConvertU32ToI64:
        return stackConvert<uint32_t, int64_t>();
    case Interpreter::ConvertU32ToU64:
        return stackConvert<uint32_t, uint64_t>();
    case Interpreter::ConvertU32ToFloat:
        return stackConvert<uint32_t, float>();
    case Interpreter::ConvertU32ToDouble:
        return stackConvert<uint32_t, double>();
    case Interpreter::ConvertI64ToI32:
        return stackConvert<int64_t, int32_t>();
    case Interpreter::ConvertI64ToU32:
        return stackConvert<int64_t, uint32_t>();
    case Interpreter::ConvertI64ToFloat:
        return stackConvert<int64_t, float>();
    case Interpreter::ConvertI64ToDouble:
        return stackConvert<int64_t, double>();
    case Interpreter::ConvertU64ToI32:
        return stackConvert<uint64_t, int32_t>();
    case Interpreter::ConvertU64ToU32:
        return stackConvert<uint64_t, uint32_t>();
    case Interpreter::ConvertU64ToFloat:
        return stackConvert<uint64_t, float>();
    case Interpreter::ConvertU64ToDouble:
        return stackConvert<uint64_t, double>();
    case Interpreter::ConvertFloatToI32:
        return stackConvert<float, int32_t>();
    case Interpreter::ConvertFloatToU32:
        return stackConvert<float, uint32_t>();
    case Interpreter::ConvertFloatToI64:
        return stackConvert<float, int64_t>();
    case Interpreter::ConvertFloatToU64:
        return stackConvert<float, uint64_t>();
    case Interpreter::ConvertFloatToDouble:
        return stackConvert<float, double>();
    case Interpreter::ConvertDoubleToI32:
        return stackConvert<double, int32_t>();
    case Interpreter::ConvertDoubleToU32:
        return stackConvert<double, uint32_t>();
    case Interpreter::ConvertDoubleToI64:
        return stackConvert<double, int64_t>();
    case Interpreter::ConvertDoubleToU64:
        return stackConvert<double, uint64_t>();
    case Interpreter::ConvertDoubleToFloat:
        return stackConvert<double, float>();
    case Interpreter::AddI8:
        return stackOp<int8_t>(OpAdd<int8_t>());
    case Interpreter::AddI16:
        return stackOp<int16_t>(OpAdd<int16_t>());
    case Interpreter::AddI32:
        return stackOp<int32_t>(OpAdd<int32_t>());
    case Interpreter::AddI64:
        return stackOp<int64_t>(OpAdd<int64_t>());
    case Interpreter::AddU8:
        return stackOp<uint8_t>(OpAdd<uint8_t>());
    case Interpreter::AddU16:
        return stackOp<uint16_t>(OpAdd<uint16_t>());
    case Interpreter::AddU32:
        return stackOp<uint32_t>(OpAdd<uint32_t>());
    case Interpreter::AddU64:
        return stackOp<uint64_t>(OpAdd<uint64_t>());
    case Interpreter::AddFloat:
        return stackOp<float>(OpAdd<float>());
    case Interpreter::AddDouble:
        return stackOp<double>(OpAdd<double>());
    case Interpreter::SubI8:
        return stackOp<int8_t>(OpSub<int8_t>());
    case Interpreter::SubI16:
        return stackOp<int16_t>(OpSub<int16_t>());
    case Interpreter::SubI32:
        return stackOp<int32_t>(OpSub<int32_t>());
    case Interpreter::SubI64:
        return stackOp<int64_t>(OpSub<int64_t>());
    case Interpreter::SubU8:
        return stackOp<uint8_t>(OpSub<uint8_t>());
    case Interpreter::SubU16:
        return stackOp<uint16_t>(OpSub<uint16_t>());
    case Interpreter::SubU32:
        return stackOp<uint32_t>(OpSub<uint32_t>());
    case Interpreter::SubU64:
        return stackOp<uint64_t>(OpSub<uint64_t>());
    case Interpreter::SubFloat:
        return stackOp<float>(OpSub<float>());
    case Interpreter::SubDouble:
        return stackOp<double>(OpSub<double>());
    case Interpreter::MultI8:
        return stackOp<int8_t>(OpMult<int8_t>());
    case Interpreter::MultI16:
        return stackOp<int16_t>(OpMult<int16_t>());
    case Interpreter::MultI32:
        return stackOp<int32_t>(OpMult<int32_t>());
    case Interpreter::MultI64:
        return stackOp<int64_t>(OpMult<int64_t>());
    case Interpreter::MultU8:
        return stackOp<uint8_t>(OpMult<uint8_t>());
    case Interpreter::MultU16:
        return stackOp<uint16_t>(OpMult<uint16_t>());
    case Interpreter::MultU32:
        return stackOp<uint32_t>(OpMult<uint32_t>());
    case Interpreter::MultU64:
        return stackOp<uint64_t>(OpMult<uint64_t>());
    case Interpreter::MultFloat:
        return stackOp<float>(OpMult<float>());
    case Interpreter::MultDouble:
        return stackOp<double>(OpMult<double>());
    case Interpreter::DivI8:
        return stackOp<int8_t>(OpDiv<int8_t>());
    case Interpreter::DivI16:
        return stackOp<int16_t>(OpDiv<int16_t>());
    case Interpreter::DivI32:
        return stackOp<int32_t>(OpDiv<int32_t>());
    case Interpreter::DivI64:
        return stackOp<int64_t>(OpDiv<int64_t>());
    case Interpreter::DivU8:
        return stackOp<uint8_t>(OpDiv<uint8_t>());
    case Interpreter::DivU16:
        return stackOp<uint16_t>(OpDiv<uint16_t>());
    case Interpreter::DivU32:
        return stackOp<uint32_t>(OpDiv<uint32_t>());
    case Interpreter::DivU64:
        return stackOp<uint64_t>(OpDiv<uint64_t>());
    case Interpreter::DivFloat:
        return stackOp<float>(OpDiv<float>());
    case Interpreter::DivDouble:
        return stackOp<double>(OpDiv<double>());
    }

    return Status::InvalidInstruction;
}
}
}
