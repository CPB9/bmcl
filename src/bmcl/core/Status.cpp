/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/core/Status.h"

namespace bmcl {
namespace core {

const char* Status::toCString(Status::Msg status)
{
    switch (status) {
    case Status::Success:
        return "Success";
    case Status::BufferOverflow:
        return "Buffer overflow";
    case Status::BufferOverread:
        return "Buffer overread";
    case Status::StackOverflow:
        return "Stack overflow";
    case Status::NotEnoughStackData:
        return "Not enough stack data";
    case Status::InvalidInstruction:
        return "Invalid instruction";
    case Status::UnexpectedEndOfBytecode:
        return "Unexpected end of bytecode";
    }
    return "Undefined error";
}
}
}
