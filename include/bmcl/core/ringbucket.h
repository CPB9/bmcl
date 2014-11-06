/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/core/queue.h"
#include "bmcl/core/ringbuf.h"

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bmcl_ringbuf_t ringbuf;
    size_t count;
} bmcl_ringbucket_t;

void bmcl_ringbucket_init(bmcl_ringbucket_t* self, void* data, size_t size);

void bmcl_ringbucket_init_queue(bmcl_ringbucket_t* self, bmcl_queue_t* queue);

#if BMCL_HAVE_MALLOC

bmcl_ringbucket_t* bmcl_ringbucket_create(size_t size);

void bmcl_ringbucket_destroy(bmcl_ringbucket_t* self);

bmcl_queue_t* bmcl_ringbucket_create_queue(bmcl_ringbucket_t* self);

#endif

size_t bmcl_ringbucket_get_free_space(const bmcl_ringbucket_t* self);

size_t bmcl_ringbucket_header_size();

bool bmcl_ringbucket_is_empty(const bmcl_ringbucket_t* self);

void bmcl_ringbucket_reset(bmcl_ringbucket_t* self);

void bmcl_ringbucket_append(bmcl_ringbucket_t* self, const void* element, size_t size);

size_t bmcl_ringbucket_count(const bmcl_ringbucket_t* self);

size_t bmcl_ringbucket_first_size(const bmcl_ringbucket_t* self);

void bmcl_ringbucket_copy_first(const bmcl_ringbucket_t* self, void* dest);

void bmcl_ringbucket_remove_first(bmcl_ringbucket_t* self);

#ifdef __cplusplus
}
#endif
