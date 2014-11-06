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
