#pragma once

#include "bmcl/core/writer.h"

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void* ptr;
    size_t size;
} tm_var_t;

typedef struct {
    uint16_t* var_ids;
    size_t id_count;
    size_t total_size;
} tm_frame_t;

typedef struct {
    tm_var_t* vars;
    size_t var_count;
    tm_frame_t* frames;
    size_t frame_count;
} tm_db_t;

void tm_db_copy_frame(size_t frame_num, writer_t* dest);

bool tm_db_has_frame(size_t frame_num);

bool tm_db_can_copy(size_t frame_num, writer_t* dest);

size_t tm_db_recalc_frame_size(size_t frame_num);

size_t tm_db_frame_size(size_t frame_num);

size_t tm_db_frame_len(size_t frame_num);

size_t tm_db_frame_count();

size_t tm_db_var_count();

#ifdef __cplusplus
}
#endif
