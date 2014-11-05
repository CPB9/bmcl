#pragma once

typedef enum {
    BMCL_SUCCESS = 0
} bmcl_status_t;

const char* bmcl_strerror(bmcl_status_t status);
