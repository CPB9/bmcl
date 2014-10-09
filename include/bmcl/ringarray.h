#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    void* data;
    size_t element_size;
    size_t size;
    size_t read_offset;
    size_t write_offset;
    size_t count;
} ringarray_t;

void ringarray_init(ringarray_t* self, void* data, size_t size, size_t element_size);

void ringarray_reset(ringarray_t* self);

bool ringarray_is_empty(const ringarray_t* self);

bool ringarray_is_full(const ringarray_t* self);

void ringarray_append(ringarray_t* self, const void* element);

size_t ringarray_count(const ringarray_t* self);

size_t ringarray_element_size(const ringarray_t* self);

void ringarray_copy_first(const ringarray_t* self, void* dest);

void ringarray_remove_first(ringarray_t* self);
