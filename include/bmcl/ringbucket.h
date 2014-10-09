#pragma once

#include "bmcl/ringbuf.h"

#include <stddef.h>
#include <stdbool.h>

typedef size_t ringbucket_size_t;

typedef struct
{
    ringbuf_t ringbuf;
    size_t count;
} ringbucket_t;

size_t ringbucket_get_free_space(const ringbucket_t* self);

bool ringbucket_is_empty(const ringbucket_t* self);

void ringbucket_init(ringbucket_t* self, void* data, size_t size);

void ringbucket_reset(ringbucket_t* self);

void ringbucket_append(ringbucket_t* self, const void* element, ringbucket_size_t size);

size_t ringbucket_count(const ringbucket_t* self);

ringbucket_size_t ringbucket_first_size(const ringbucket_t* self);

void ringbucket_copy_first(const ringbucket_t* self, void* dest);

void ringbucket_remove_first(ringbucket_t* self);
