/*
 * Copyright (c) 2014 CPB9 team. See the COPYRIGHT file at the top-level directory.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bmcl/core/ringarray.h"
#include "bmcl/core/queue.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void bmcl_ringarray_append_with_size(bmcl_ringarray_t* self, const void* element, size_t size)
{
    assert(bmcl_ringarray_element_size(self) == size);
    bmcl_ringarray_append(self, element);
}

static bool constant_size(const bmcl_ringarray_t* self, size_t* size)
{
    if (size) {
        *size = bmcl_ringarray_element_size(self);
    }
    return true;
}

static const bmcl_queue_impl_t ringarray_queue_impl
    = {(void (*)(void*, const void*, size_t))bmcl_ringarray_append_with_size,
       (size_t (*)(const void*))bmcl_ringarray_count,
       (size_t (*)(const void*))bmcl_ringarray_element_size,
       (void (*)(const void* data, void* dest))bmcl_ringarray_copy_first,
       (void (*)(void* data))bmcl_ringarray_remove_first,
       (bool (*)(const void* self, size_t* size))constant_size};

static inline void* get_write_ptr(const bmcl_ringarray_t* self)
{
    return (uint8_t*)self->data + self->write_offset * self->element_size;
}

static inline void* get_read_ptr(const bmcl_ringarray_t* self)
{
    return (uint8_t*)self->data + self->read_offset * self->element_size;
}

static void increment_read_ptr(bmcl_ringarray_t* self)
{
    self->read_offset++;
    if (self->read_offset >= self->size)
        self->read_offset -= self->size;
}

static void increment_write_ptr(bmcl_ringarray_t* self)
{
    self->write_offset++;
    if (self->write_offset >= self->size)
        self->write_offset -= self->size;
}

void bmcl_ringarray_init(bmcl_ringarray_t* self, void* ptr, size_t buf_size, size_t element_size)
{
    assert(ptr != 0);
    assert(buf_size != 0);
    assert(element_size != 0);
    assert(element_size <= buf_size);
    bmcl_ringarray_reset(self);
    self->data = ptr;
    self->element_size = element_size;
    self->size = buf_size / element_size;
}

void bmcl_ringarray_init_queue(bmcl_ringarray_t* self, bmcl_queue_t* queue)
{
    queue->data = self;
    queue->impl = &ringarray_queue_impl;
}

#if BMCL_HAVE_MALLOC

bmcl_ringarray_t* bmcl_ringarray_create(size_t element_num, size_t element_size)
{
    // FIXME: check overflow
    size_t buf_size = element_size * element_num;
    uint8_t* data = malloc(sizeof(bmcl_ringarray_t) + buf_size);
    bmcl_ringarray_t* self = (bmcl_ringarray_t*)data;
    bmcl_ringarray_init(self, data + sizeof(bmcl_ringarray_t), buf_size, element_size);
    return self;
}

void bmcl_ringarray_destroy(bmcl_ringarray_t* self)
{
    free(self);
}

bmcl_queue_t* bmcl_ringarray_create_queue(bmcl_ringarray_t* self)
{
    bmcl_queue_t* queue = malloc(sizeof(bmcl_queue_t));
    bmcl_ringarray_init_queue(self, queue);
    return queue;
}

#endif

void bmcl_ringarray_reset(bmcl_ringarray_t* self)
{
    self->data = (void*)0;
    self->read_offset = 0;
    self->write_offset = 0;
    self->count = 0;
    self->size = 0;
    self->element_size = 0;
}

static void erase_first_element(bmcl_ringarray_t* self)
{
    increment_read_ptr(self);
    --self->count;
}

void bmcl_ringarray_append(bmcl_ringarray_t* self, const void* element)
{
    if (bmcl_ringarray_is_full(self))
        erase_first_element(self);
    memcpy(get_write_ptr(self), element, self->element_size);
    increment_write_ptr(self);
    self->count++;
}

size_t bmcl_ringarray_count(const bmcl_ringarray_t* self)
{
    return self->count;
}

size_t bmcl_ringarray_size(const bmcl_ringarray_t* self)
{
    return self->size;
}

bool bmcl_ringarray_is_empty(const bmcl_ringarray_t* self)
{
    return self->count == 0;
}

bool bmcl_ringarray_is_full(const bmcl_ringarray_t* self)
{
    return !bmcl_ringarray_is_empty(self) && self->read_offset == self->write_offset;
}

size_t bmcl_ringarray_element_size(const bmcl_ringarray_t* self)
{
    return self->element_size;
}

void bmcl_ringarray_copy_first(const bmcl_ringarray_t* self, void* dest)
{
    assert(!bmcl_ringarray_is_empty(self));
    memcpy(dest, get_read_ptr(self), self->element_size);
}

void bmcl_ringarray_remove_first(bmcl_ringarray_t* self)
{
    assert(!bmcl_ringarray_is_empty(self));
    erase_first_element(self);
}
