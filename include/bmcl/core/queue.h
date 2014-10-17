#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    void* data;
    const struct queue_impl_s* impl;
} queue_t;

typedef struct queue_impl_s {
    void (*append)(void* self, const void* element, size_t size);
    size_t (*count)(const void* self);
    size_t (*first_size)(const void* data);
    void (*copy_first)(const void* data, void* dest);
    void (*remove_first)(void* data);
} queue_impl_t;

static inline void queue_append(queue_t* self, const void* element, size_t size)
{
    self->impl->append(self->data, element, size);
}

static inline size_t queue_count(const queue_t* self)
{
    return self->impl->count(self);
}

static inline size_t queue_first_size(const queue_t* self)
{
    return self->impl->first_size(self);
}

static inline void queue_copy_first(const queue_t* self, void* dest)
{
    self->impl->copy_first(self->data, dest);
}

static inline void queue_remove_first(queue_t* self)
{
    self->impl->remove_first(self->data);
}

static inline bool queue_is_empty(const queue_t* self)
{
    return queue_count(self) == 0;
}
