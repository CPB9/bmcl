#include "bmcl/ringarray.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static inline void* get_write_ptr(const ringarray_t* self)
{
    return (uint8_t*)self->data + self->write_offset * self->element_size;
}

static inline void* get_read_ptr(const ringarray_t* self)
{
    return (uint8_t*)self->data + self->read_offset * self->element_size;
}

static void increment_read_ptr(ringarray_t* self)
{
    self->read_offset++;
    if (self->read_offset >= self->size)
        self->read_offset -= self->size;
}

static void increment_write_ptr(ringarray_t* self)
{
    self->write_offset++;
    if (self->write_offset >= self->size)
        self->write_offset -= self->size;
}

void ringarray_init(ringarray_t* self, void* ptr, size_t size, size_t element_size)
{
    assert(ptr != 0);
    assert(size != 0);
    assert(element_size != 0);
    assert(element_size <= size);
    ringarray_reset(self);
    self->data = ptr;
    self->element_size = element_size;
    self->size = size / element_size;
}

void ringarray_reset(ringarray_t* self)
{
    self->data = (void*)0;
    self->read_offset = 0;
    self->write_offset = 0;
    self->count = 0;
    self->size = 0;
    self->element_size = 0;
}

static void erase_first_element(ringarray_t* self)
{
    increment_read_ptr(self);
    --self->count;
}

void ringarray_append(ringarray_t* self, const void* element)
{
    if (ringarray_is_full(self))
        erase_first_element(self);
    memcpy(get_write_ptr(self), element, self->element_size);
    increment_write_ptr(self);
    self->count++;
}

size_t ringarray_count(const ringarray_t* self)
{
    return self->count;
}

bool ringarray_is_empty(const ringarray_t* self)
{
    return self->count == 0;
}

bool ringarray_is_full(const ringarray_t* self)
{
    return !ringarray_is_empty(self) && self->read_offset == self->write_offset;
}

size_t ringarray_element_size(const ringarray_t* self)
{
    assert(!ringarray_is_empty(self));
    return self->element_size;
}

void ringarray_copy_first(const ringarray_t* self, void* dest)
{
    assert(!ringarray_is_empty(self));
    memcpy(dest, get_read_ptr(self), self->element_size);
}

void ringarray_remove_first(ringarray_t* self)
{
    assert(!ringarray_is_empty(self));
    erase_first_element(self);
}
