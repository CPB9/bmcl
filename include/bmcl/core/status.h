/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace bmcl {
namespace core {

class Status {
public:
    enum Msg {
        Success = 0,
        BufferOverflow,
        BufferOverread,
        StackOverflow,
        NotEnoughStackData,
        InvalidInstruction,
        UnexpectedEndOfBytecode,
    };

    static const char* toCString(Status::Msg msg);

};
}
}
