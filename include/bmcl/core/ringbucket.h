#pragma once

#include "bmcl/core/queue.h"
#include "bmcl/core/ringbuf.h"

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    ringbuf_t ringbuf;
    size_t count;
} ringbucket_t;

void ringbucket_init(ringbucket_t* self, void* data, size_t size);

void ringbucket_init_queue(ringbucket_t* self, queue_t* queue);

#if BMCL_HAVE_MALLOC

ringbucket_t* ringbucket_create(size_t size);

void ringbucket_destroy(ringbucket_t* self);

queue_t* ringbucket_create_queue(ringbucket_t* self);

#endif

size_t ringbucket_get_free_space(const ringbucket_t* self);

size_t ringbucket_header_size();

bool ringbucket_is_empty(const ringbucket_t* self);

void ringbucket_reset(ringbucket_t* self);

void ringbucket_append(ringbucket_t* self, const void* element, size_t size);

size_t ringbucket_count(const ringbucket_t* self);

size_t ringbucket_first_size(const ringbucket_t* self);

void ringbucket_copy_first(const ringbucket_t* self, void* dest);

void ringbucket_remove_first(ringbucket_t* self);

#ifdef __cplusplus
}
#endif
