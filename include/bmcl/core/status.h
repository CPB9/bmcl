/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

typedef enum { BMCL_SUCCESS = 0, BMCL_ERR_NOT_ENOUGH_SPACE = 1 } bmcl_status_t;

const char* bmcl_strerror(bmcl_status_t status);

#define BMCL_CHECK(expr)                                                                                      \
    {                                                                                                                  \
        bmcl_status_t status = expr;                                                                                   \
        if (status != BMCL_SUCCESS) {                                                                                  \
            return status;                                                                                             \
        }                                                                                                              \
    }
