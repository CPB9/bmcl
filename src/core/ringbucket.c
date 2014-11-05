#include "bmcl/core/ringbucket.h"

#include <assert.h>
#include <stdlib.h>

typedef size_t bmcl_ringbucket_size_t;

static bool constant_size(const bmcl_ringbucket_t* self, size_t* size)
{
    (void)self;
    (void)size;
    return false;
}

static const bmcl_queue_impl_t ringbucket_queue_impl
    = {(void (*)(void*, const void*, size_t))bmcl_ringbucket_append,
       (size_t (*)(const void*))bmcl_ringbucket_count,
       (size_t (*)(const void*))bmcl_ringbucket_first_size,
       (void (*)(const void* data, void* dest))bmcl_ringbucket_copy_first,
       (void (*)(void* data))bmcl_ringbucket_remove_first,
       (bool (*)(const void* self, size_t* size))constant_size};

void bmcl_ringbucket_init(bmcl_ringbucket_t* self, void* data, size_t size)
{
    assert(data != 0);
    assert(size != 0);
    bmcl_ringbuf_init(&self->ringbuf, data, size);
    self->count = 0;
}

void bmcl_ringbucket_init_queue(bmcl_ringbucket_t* self, bmcl_queue_t* queue)
{
    queue->data = self;
    queue->impl = &ringbucket_queue_impl;
}

#if BMCL_HAVE_MALLOC

bmcl_ringbucket_t* bmcl_ringbucket_create(size_t size)
{
    uint8_t* data = malloc(sizeof(bmcl_ringbucket_t) + size);
    bmcl_ringbucket_t* self = (bmcl_ringbucket_t*)data;
    bmcl_ringbucket_init(self, data + sizeof(bmcl_ringbucket_t), size);
    return self;
}

void bmcl_ringbucket_destroy(bmcl_ringbucket_t* self)
{
    free(self);
}

bmcl_queue_t* bmcl_ringbucket_create_queue(bmcl_ringbucket_t* self)
{
    bmcl_queue_t* queue = malloc(sizeof(bmcl_queue_t));
    bmcl_ringbucket_init_queue(self, queue);
    return queue;
}

#endif

static size_t erase_element(bmcl_ringbucket_t* self)
{
    bmcl_ringbucket_size_t element_size;
    bmcl_ringbuf_peek(&self->ringbuf, &element_size, sizeof(element_size), 0);
    element_size += sizeof(element_size);
    bmcl_ringbuf_erase(&self->ringbuf, element_size);
    self->count--;
    return element_size;
}

static void erase_elements_to_fit_size(bmcl_ringbucket_t* self, size_t size)
{
    size_t freeSpace = bmcl_ringbucket_get_free_space(self);
    while (freeSpace < size)
        freeSpace += erase_element(self);
}

size_t bmcl_ringbucket_get_free_space(const bmcl_ringbucket_t* self)
{
    return bmcl_ringbuf_get_free_space(&self->ringbuf);
}

size_t bmcl_ringbucket_header_size()
{
    return sizeof(bmcl_ringbucket_size_t);
}

bool bmcl_ringbucket_is_empty(const bmcl_ringbucket_t* self)
{
    return self->count == 0;
}

void bmcl_ringbucket_reset(bmcl_ringbucket_t* self)
{
    bmcl_ringbuf_clear(&self->ringbuf);
    self->count = 0;
}

size_t bmcl_ringbucket_count(const bmcl_ringbucket_t* self)
{
    return self->count;
}

void bmcl_ringbucket_remove_first(bmcl_ringbucket_t* self)
{
    assert(!bmcl_ringbucket_is_empty(self));
    erase_element(self);
}

static void prepare_for_append(bmcl_ringbucket_t* self, size_t data_size)
{
    bmcl_ringbucket_size_t element_size = data_size + sizeof(element_size);

    assert(element_size <= self->ringbuf.size);

    if (bmcl_ringbucket_get_free_space(self) < element_size)
        erase_elements_to_fit_size(self, element_size);

    bmcl_ringbuf_write(&self->ringbuf, &data_size, sizeof(data_size));
}

void bmcl_ringbucket_append(bmcl_ringbucket_t* self, const void* data, size_t data_size)
{
    prepare_for_append(self, data_size);
    bmcl_ringbuf_write(&self->ringbuf, data, data_size);

    self->count++;
}

size_t bmcl_ringbucket_first_size(const bmcl_ringbucket_t* self)
{
    assert(!bmcl_ringbucket_is_empty(self));
    bmcl_ringbucket_size_t size;
    bmcl_ringbuf_peek(&self->ringbuf, &size, sizeof(size), 0);
    return size;
}

void bmcl_ringbucket_copy_first(const bmcl_ringbucket_t* self, void* dest)
{
    assert(!bmcl_ringbucket_is_empty(self));
    bmcl_ringbucket_size_t first_size;
    bmcl_ringbuf_peek(&self->ringbuf, &first_size, sizeof(first_size), 0);
    bmcl_ringbuf_peek(&self->ringbuf, dest, first_size, sizeof(first_size));
}
