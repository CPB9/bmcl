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
} bmcl_queue_impl_t;

typedef struct {
    void* data;
    const bmcl_queue_impl_t* impl;
} bmcl_queue_t;

#if BMCL_HAVE_MALLOC

static inline void bmcl_queue_destroy(bmcl_queue_t* self)
{
    free(self);
}

#endif

static inline void bmcl_queue_append(bmcl_queue_t* self, const void* element, size_t size)
{
    self->impl->append(self->data, element, size);
}

static inline size_t bmcl_queue_count(const bmcl_queue_t* self)
{
    return self->impl->count(self->data);
}

static inline size_t bmcl_queue_first_size(const bmcl_queue_t* self)
{
    return self->impl->first_size(self->data);
}

static inline void bmcl_queue_copy_first(const bmcl_queue_t* self, void* dest)
{
    self->impl->copy_first(self->data, dest);
}

static inline void bmcl_queue_remove_first(bmcl_queue_t* self)
{
    self->impl->remove_first(self->data);
}

static inline bool bmcl_queue_is_empty(const bmcl_queue_t* self)
{
    return bmcl_queue_count(self) == 0;
}

static inline bool bmcl_queue_const_el_size(const bmcl_queue_t* self, size_t* size)
{
    return self->impl->const_el_size(self->data, size);
}
