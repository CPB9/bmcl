/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/core/endian.h"
#include "bmcl/core/memwriter.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define MAKE_MEMWRITER(type, suffix, enc_func)                                                                         \
    void bmcl_memwriter_write_##suffix(bmcl_memwriter_t* self, type value)                                             \
    {                                                                                                                  \
        assert(bmcl_memwriter_size_left(self) >= sizeof(type));                                                        \
        enc_func(self->current, value);                                                                                \
        self->current += sizeof(type);                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    static bmcl_status_t write_##suffix(void* memwriter, type value)                                                   \
    {                                                                                                                  \
        bmcl_memwriter_t* self = (bmcl_memwriter_t*)memwriter;                                                         \
        if (bmcl_memwriter_size_left(self) < sizeof(type)) {                                                           \
            return BMCL_ERR_NOT_ENOUGH_SPACE;                                                                          \
        }                                                                                                              \
        enc_func(self->current, value);                                                                                \
        self->current += sizeof(type);                                                                                 \
        return BMCL_SUCCESS;                                                                                           \
    }

#define enc8(ptr, value) *ptr = value;
MAKE_MEMWRITER(uint8_t, uint8, enc8);
MAKE_MEMWRITER(uint16_t, uint16le, le16enc);
MAKE_MEMWRITER(uint32_t, uint32le, le32enc);
MAKE_MEMWRITER(uint64_t, uint64le, le64enc);
MAKE_MEMWRITER(uint16_t, uint16be, be16enc);
MAKE_MEMWRITER(uint32_t, uint32be, be32enc);
MAKE_MEMWRITER(uint64_t, uint64be, be64enc);

static bmcl_status_t write(void* memwriter, const void* data, size_t size)
{
    bmcl_memwriter_t* self = (bmcl_memwriter_t*)memwriter;
    if (bmcl_memwriter_size_left(self) < size) {
        return BMCL_ERR_NOT_ENOUGH_SPACE;
    }
    memcpy(self->current, data, size);
    self->current += size;
    return BMCL_SUCCESS;
}

static const bmcl_writer_impl_t memwriter_writer_impl = {
    write, write_uint8, write_uint16le, write_uint32le, write_uint64le, write_uint16be, write_uint32be, write_uint64be};

void bmcl_memwriter_init(bmcl_memwriter_t* self, void* dest, size_t max_size)
{
    self->start = (uint8_t*)dest;
    self->current = self->start;
    self->end = self->start + max_size;
}

void bmcl_memwriter_init_writer(bmcl_memwriter_t* self, bmcl_writer_t* writer)
{
    writer->data = self;
    writer->impl = &memwriter_writer_impl;
}

#if BMCL_HAVE_MALLOC

bmcl_memwriter_t* bmcl_memwriter_create(size_t max_size)
{
    uint8_t* data = malloc(sizeof(bmcl_memwriter_t) + max_size);
    bmcl_memwriter_t* self = (bmcl_memwriter_t*)data;
    bmcl_memwriter_init(self, data + sizeof(bmcl_memwriter_t), max_size);
    return self;
}

bmcl_memwriter_t* bmcl_memwriter_create_with_dest(void* dest, size_t max_size)
{
    bmcl_memwriter_t* self = malloc(sizeof(bmcl_memwriter_t));
    bmcl_memwriter_init(self, dest, max_size);
    return self;
}

void bmcl_memwriter_destroy(bmcl_memwriter_t* self)
{
    free(self);
}

bmcl_writer_t* bmcl_memwriter_create_writer(bmcl_memwriter_t* self)
{
    bmcl_writer_t* writer = malloc(sizeof(bmcl_writer_t));
    bmcl_memwriter_init_writer(self, writer);
    return writer;
}

#endif

const void* bmcl_memwriter_ptr(const bmcl_memwriter_t* self)
{
    return self->start;
}

const void* bmcl_memwriter_current_ptr(const bmcl_memwriter_t* self)
{
    return self->current;
}

size_t bmcl_memwriter_size(const bmcl_memwriter_t* self)
{
    return self->current - self->start;
}

bool bmcl_memwriter_is_full(const bmcl_memwriter_t* self)
{
    return self->current == self->end;
}

size_t bmcl_memwriter_max_size(const bmcl_memwriter_t* self)
{
    return self->end - self->start;
}

size_t bmcl_memwriter_size_left(const bmcl_memwriter_t* self)
{
    return self->end - self->current;
}

void bmcl_memwriter_write(bmcl_memwriter_t* self, const void* data, size_t size)
{
    assert(bmcl_memwriter_size_left(self) >= size);
    memcpy(self->current, data, size);
    self->current += size;
}

void bmcl_memwriter_pop(bmcl_memwriter_t* self, void* dest, size_t size)
{
    assert(bmcl_memwriter_size(self) >= size);
    self->current -= size;
    memcpy(dest, self->current, size);
}

void bmcl_memwriter_fill(bmcl_memwriter_t* self, uint8_t byte, size_t size)
{
    assert(bmcl_memwriter_size_left(self) >= size);
    memset(self->current, byte, size);
    self->current += size;
}

void bmcl_memwriter_fillup(bmcl_memwriter_t* self, uint8_t byte)
{
    size_t size = self->end - self->current;
    memset(self->current, byte, size);
    self->current += size;
}

// TODO: replace memcpy
#define MAKE_STACK_FUNC(type, suffix)                                                                                  \
    void bmcl_memwriter_write_##suffix(bmcl_memwriter_t* self, type value)                                             \
    {                                                                                                                  \
        assert(bmcl_memwriter_size_left(self) >= sizeof(type));                                                        \
        memcpy(self->current, &value, sizeof(type));                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    type bmcl_memwriter_pop_##suffix(bmcl_memwriter_t* self)                                                           \
    {                                                                                                                  \
        assert(bmcl_memwriter_size(self) >= sizeof(type));                                                             \
        type value;                                                                                                    \
        memcpy(&value, self->current, sizeof(type));                                                                   \
        self->current += sizeof(type);                                                                                 \
        return value;                                                                                                  \
    }

MAKE_STACK_FUNC(uint16_t, uint16);
MAKE_STACK_FUNC(uint32_t, uint32);
MAKE_STACK_FUNC(uint64_t, uint64);

uint8_t bmcl_memwriter_pop_uint8(bmcl_memwriter_t* self)
{
    assert(bmcl_memwriter_size(self) >= 1);
    uint8_t value = *self->current;
    self->current += 1;
    return value;
}
