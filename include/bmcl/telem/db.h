/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/core/writer.h"

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef bool (*bmcl_tm_frame_serializer_t)(size_t frame, bmcl_writer_t* dest, void* user_data);

typedef struct {
    void* ptr;
    size_t size;
} bmcl_tm_var_t;

typedef struct {
    uint16_t* var_ids;
    size_t id_count;
    size_t total_size;
} bmcl_tm_frame_t;

typedef struct {
    bmcl_tm_var_t* vars;
    size_t var_count;
    bmcl_tm_frame_t* frames;
    size_t frame_count;
    bmcl_tm_frame_serializer_t serializer;
} bmcl_tm_db_t;

void bmcl_tm_db_init(bmcl_tm_var_t* vars, size_t var_count, bmcl_tm_frame_t* frames, size_t frame_count,
                     bmcl_tm_frame_serializer_t serializer);

bool bmcl_tm_db_serialize_frame(size_t frame_num, bmcl_writer_t* dest, void* user_data);

void bmcl_tm_db_copy_frame(size_t frame_num, bmcl_writer_t* dest);

bool bmcl_tm_db_has_frame(size_t frame_num);

bool bmcl_tm_db_can_copy(size_t frame_num, bmcl_writer_t* dest);

size_t bmcl_tm_db_recalc_frame_size(size_t frame_num);

size_t bmcl_tm_db_frame_size(size_t frame_num);

size_t bmcl_tm_db_frame_len(size_t frame_num);

size_t bmcl_tm_db_frame_count();

size_t bmcl_tm_db_var_count();

#ifdef __cplusplus
}
#endif
