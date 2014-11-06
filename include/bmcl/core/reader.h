/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "bmcl/core/status.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    bmcl_status_t (*read)(void* self, void* dest, size_t size);
    bmcl_status_t (*read_uint8)(void* self, uint8_t* value);
    bmcl_status_t (*read_uint16le)(void* self, uint16_t* value);
    bmcl_status_t (*read_uint32le)(void* self, uint32_t* value);
    bmcl_status_t (*read_uint64le)(void* self, uint64_t* value);
    bmcl_status_t (*read_uint16be)(void* self, uint16_t* value);
    bmcl_status_t (*read_uint32be)(void* self, uint32_t* value);
    bmcl_status_t (*read_uint64be)(void* self, uint64_t* value);
} bmcl_reader_impl_t;

typedef struct {
    void* data;
    const bmcl_reader_impl_t* impl;
} bmcl_reader_t;

#if BMCL_HAVE_MALLOC

static inline void bmcl_reader_destroy(bmcl_reader_t* self)
{
    free(self);
}

#endif

static inline void bmcl_reader_read(bmcl_reader_t* self, void* dest, size_t size)
{
    self->impl->read(self->data, dest, size);
}

static inline uint8_t bmcl_reader_read_uint8(bmcl_reader_t* self, uint8_t* value)
{
    return self->impl->read_uint8(self->data, value);
}

static inline uint16_t bmcl_reader_read_uint16le(bmcl_reader_t* self, uint16_t* value)
{
    return self->impl->read_uint16le(self->data, value);
}

static inline uint32_t bmcl_reader_read_uint32le(bmcl_reader_t* self, uint32_t* value)
{
    return self->impl->read_uint32le(self->data, value);
}

static inline uint64_t bmcl_reader_read_uint64le(bmcl_reader_t* self, uint64_t* value)
{
    return self->impl->read_uint64le(self->data, value);
}

static inline uint16_t bmcl_reader_read_uint16be(bmcl_reader_t* self, uint16_t* value)
{
    return self->impl->read_uint16be(self->data, value);
}

static inline uint32_t bmcl_reader_read_uint32be(bmcl_reader_t* self, uint32_t* value)
{
    return self->impl->read_uint32be(self->data, value);
}

static inline uint64_t bmcl_reader_read_uint64be(bmcl_reader_t* self, uint64_t* value)
{
    return self->impl->read_uint64be(self->data, value);
}
