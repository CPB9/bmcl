/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/core/status.h"

const char* bmcl_strerror(bmcl_status_t status)
{
    switch (status) {
    case BMCL_SUCCESS:
        return "Success";
    case BMCL_ERR_BUFFER_OVERFLOW:
        return "Buffer overflow";
    case BMCL_ERR_BUFFER_OVERREAD:
        return "Buffer overread";
    case BMCL_ERR_STACK_OVERFLOW:
        return "Stack overflow";
    case BMCL_ERR_NOT_ENOUGH_STACK_DATA:
        return "Not enough stack data";
    case BMCL_ERR_INVALID_INSTRUCTION:
        return "Invalid instruction";
    case BMCL_ERR_UNEXPECTED_END_OF_BYTECODE:
        return "Unexpected end of bytecode";
    }
    return "Undefined error";
}
