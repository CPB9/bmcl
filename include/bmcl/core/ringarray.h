#pragma once

#include "bmcl/core/queue.h"

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void* data;
    size_t element_size;
    size_t size;
    size_t read_offset;
    size_t write_offset;
    size_t count;
} bmcl_ringarray_t;

void bmcl_ringarray_init(bmcl_ringarray_t* self, void* data, size_t buf_size, size_t element_size);

void bmcl_ringarray_init_queue(bmcl_ringarray_t* self, bmcl_queue_t* queue);

#if BMCL_HAVE_MALLOC

bmcl_ringarray_t* bmcl_ringarray_create(size_t element_num, size_t element_size);

void bmcl_ringarray_destroy(bmcl_ringarray_t* self);

bmcl_queue_t* bmcl_ringarray_create_queue(bmcl_ringarray_t* self);

#endif

void bmcl_ringarray_reset(bmcl_ringarray_t* self);

bool bmcl_ringarray_is_empty(const bmcl_ringarray_t* self);

bool bmcl_ringarray_is_full(const bmcl_ringarray_t* self);

void bmcl_ringarray_append(bmcl_ringarray_t* self, const void* element);

size_t bmcl_ringarray_count(const bmcl_ringarray_t* self);

size_t bmcl_ringarray_size(const bmcl_ringarray_t* self);

size_t bmcl_ringarray_element_size(const bmcl_ringarray_t* self);

void bmcl_ringarray_copy_first(const bmcl_ringarray_t* self, void* dest);

void bmcl_ringarray_remove_first(bmcl_ringarray_t* self);

#ifdef __cplusplus
}
#endif
