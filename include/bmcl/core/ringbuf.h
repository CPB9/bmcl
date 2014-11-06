/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/core/reader.h"
#include "bmcl/core/writer.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    size_t read_offset;
    size_t write_offset;
    size_t size;
    size_t free_space;
    uint8_t* data;
} bmcl_ringbuf_t;

void bmcl_ringbuf_init(bmcl_ringbuf_t* self, void* data, size_t size);

void bmcl_ringbuf_init_writer(bmcl_ringbuf_t* self, bmcl_writer_t* writer);

void bmcl_ringbuf_init_reader(bmcl_ringbuf_t* self, bmcl_reader_t* reader);

#if BMCL_HAVE_MALLOC

bmcl_ringbuf_t* bmcl_ringbuf_create(size_t size);

void bmcl_ringbuf_destroy(bmcl_ringbuf_t* self);

bmcl_writer_t* bmcl_ringbuf_create_writer(bmcl_ringbuf_t* self);

bmcl_reader_t* bmcl_ringbuf_create_reader(bmcl_ringbuf_t* self);

#endif

void bmcl_ringbuf_clear(bmcl_ringbuf_t* self);

size_t bmcl_ringbuf_get_free_space(const bmcl_ringbuf_t* self);

size_t bmcl_ringbuf_get_used_space(const bmcl_ringbuf_t* self);

bool bmcl_ringbuf_is_full(const bmcl_ringbuf_t* self);

bool bmcl_ringbuf_is_empty(const bmcl_ringbuf_t* self);

void bmcl_ringbuf_erase(bmcl_ringbuf_t* self, size_t size);

void bmcl_ringbuf_write(bmcl_ringbuf_t* self, const void* data, size_t size);

void bmcl_ringbuf_write_uint8(bmcl_ringbuf_t* self, uint8_t byte);

void bmcl_ringbuf_write_uint16le(bmcl_ringbuf_t* self, uint16_t value);

void bmcl_ringbuf_write_uint32le(bmcl_ringbuf_t* self, uint32_t value);

void bmcl_ringbuf_write_uint64le(bmcl_ringbuf_t* self, uint64_t value);

void bmcl_ringbuf_write_uint16be(bmcl_ringbuf_t* self, uint16_t value);

void bmcl_ringbuf_write_uint32be(bmcl_ringbuf_t* self, uint32_t value);

void bmcl_ringbuf_write_uint64be(bmcl_ringbuf_t* self, uint64_t value);

void bmcl_ringbuf_peek(const bmcl_ringbuf_t* self, void* dest, size_t size, size_t offset);

void bmcl_ringbuf_read(bmcl_ringbuf_t* self, void* dest, size_t size);

uint8_t bmcl_ringbuf_read_uint8(bmcl_ringbuf_t* self);

uint16_t bmcl_ringbuf_read_uint16le(bmcl_ringbuf_t* self);

uint32_t bmcl_ringbuf_read_uint32le(bmcl_ringbuf_t* self);

uint64_t bmcl_ringbuf_read_uint64le(bmcl_ringbuf_t* self);

uint16_t bmcl_ringbuf_read_uint16be(bmcl_ringbuf_t* self);

uint32_t bmcl_ringbuf_read_uint32be(bmcl_ringbuf_t* self);

uint64_t bmcl_ringbuf_read_uint64be(bmcl_ringbuf_t* self);

#ifdef __cplusplus
}
#endif
