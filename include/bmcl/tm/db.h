#pragma once

#include "bmcl/core/writer.h"

#include <stddef.h>

typedef struct {
    void* ptr;
    size_t size;
} tm_var_t;

typedef struct {
    uint16_t* var_ids;
    size_t id_num;
} tm_frame_t;

typedef struct {
    tm_var_t* vars;
    size_t var_num;
    tm_frame_t* frames;
    size_t frame_num;
} tm_db_t;

extern tm_db_t tm_global_db;

void tm_db_copy_frame(tm_db_t* self, size_t frame_num, writer_t* dest);
