/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/config.h"
#include "bmcl/core/MemWriter.h"
#include "bmcl/core/MemReader.h"
#include "bmcl/core/Status.h"

#include <limits>

namespace bmcl {
namespace interpreter {

class Interpreter {
public:
    enum Instruction {
        Push8 = 0,
        Push16,
        Push32,
        Push64,
        ConvertI8ToI32,
        ConvertI8ToU32,
        ConvertU8ToI32,
        ConvertU8ToU32,
        ConvertI16ToI32,
        ConvertI16ToU32,
        ConvertU16ToI32,
        ConvertU16ToU32,
        ConvertI32ToI8,
        ConvertI32ToU8,
        ConvertI32ToI16,
        ConvertI32ToU16,
        ConvertI32ToI64,
        ConvertI32ToU64,
        ConvertI32ToFloat,
        ConvertI32ToDouble,
        ConvertU32ToI8,
        ConvertU32ToU8,
        ConvertU32ToI16,
        ConvertU32ToU16,
        ConvertU32ToI64,
        ConvertU32ToU64,
        ConvertU32ToFloat,
        ConvertU32ToDouble,
        ConvertI64ToI32,
        ConvertI64ToU32,
        ConvertI64ToFloat,
        ConvertI64ToDouble,
        ConvertU64ToI32,
        ConvertU64ToU32,
        ConvertU64ToFloat,
        ConvertU64ToDouble,
        ConvertFloatToI32,
        ConvertFloatToU32,
        ConvertFloatToI64,
        ConvertFloatToU64,
        ConvertFloatToDouble,
        ConvertDoubleToI32,
        ConvertDoubleToU32,
        ConvertDoubleToI64,
        ConvertDoubleToU64,
        ConvertDoubleToFloat,
        AddI8,
        AddI16,
        AddI32,
        AddI64,
        AddU8,
        AddU16,
        AddU32,
        AddU64,
        AddFloat,
        AddDouble,
        SubI8,
        SubI16,
        SubI32,
        SubI64,
        SubU8,
        SubU16,
        SubU32,
        SubU64,
        SubFloat,
        SubDouble,
        MultI8,
        MultI16,
        MultI32,
        MultI64,
        MultU8,
        MultU16,
        MultU32,
        MultU64,
        MultFloat,
        MultDouble,
        DivI8,
        DivI16,
        DivI32,
        DivI64,
        DivU8,
        DivU16,
        DivU32,
        DivU64,
        DivFloat,
        DivDouble,
    };

    Interpreter(const void* bytecode, std::size_t bytecodeSize, void* stack, std::size_t stackSize);

#if BMCL_HAVE_MALLOC

    Interpreter(const void* bytecode, std::size_t bytecodeSize, std::size_t stackSize);

#endif

    bmcl::core::Status::Msg execNext();
    bmcl::core::MemWriter* stack() { return &_stack; }

private:
    template <typename T>
    inline bmcl::core::Status::Msg stackPush();

    template <typename T1, typename T2>
    inline bmcl::core::Status::Msg stackConvert();

    template <typename T, typename F>
    inline bmcl::core::Status::Msg stackOp(F func);

    bmcl::core::MemReader _bytecode;
    bmcl::core::MemWriter _stack;
};
}
}
