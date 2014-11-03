#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct {
    void (*append)(void* self, const void* element, size_t size);
    size_t (*count)(const void* self);
    size_t (*first_size)(const void* self);
    void (*copy_first)(const void* self, void* dest);
    void (*remove_first)(void* self);
    bool (*const_el_size)(const void* self, size_t* size);
} queue_impl_t;

typedef struct {
    void* data;
    const queue_impl_t* impl;
} queue_t;

#if BMCL_HAVE_MALLOC

static inline void queue_destroy(queue_t* self)
{
    free(self);
}

#endif

static inline void queue_append(queue_t* self, const void* element, size_t size)
{
    self->impl->append(self->data, element, size);
}

static inline size_t queue_count(const queue_t* self)
{
    return self->impl->count(self->data);
}

static inline size_t queue_first_size(const queue_t* self)
{
    return self->impl->first_size(self->data);
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

static inline bool queue_const_el_size(const queue_t* self, size_t* size)
{
    return self->impl->const_el_size(self->data, size);
}
