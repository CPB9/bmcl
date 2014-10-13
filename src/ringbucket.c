#include "bmcl/ringbucket.h"

#include <assert.h>

static const queue_impl_t ringbucket_queue_impl = {
    (void (*)(void*, const void*, size_t))ringbucket_append,
    (size_t (*)(const void*))ringbucket_count,
    (size_t (*)(const void*))ringbucket_first_size,
    (void (*)(const void* data, void* dest))ringbucket_copy_first,
    (void (*)(void* data))ringbucket_remove_first
};

void ringbucket_init(ringbucket_t* self, void* data, size_t size)
{
    assert(data != 0);
    assert(size != 0);
    ringbuf_init(&self->ringbuf, data, size);
    self->count = 0;
}

void ringbucket_init_queue(ringbucket_t* self, queue_t* queue)
{
    queue->data = self;
    queue->impl = &ringbucket_queue_impl;
}

static size_t erase_element(ringbucket_t* self)
{
    ringbucket_size_t element_size;
    ringbuf_peek(&self->ringbuf, &element_size, sizeof(element_size), 0);
    element_size += sizeof(element_size);
    ringbuf_erase(&self->ringbuf, element_size);
    self->count--;
    return element_size;
}

static void erase_elements_to_fit_size(ringbucket_t* self, size_t size)
{
    size_t freeSpace = ringbucket_get_free_space(self);
    while (freeSpace < size)
        freeSpace += erase_element(self);
}

size_t ringbucket_get_free_space(const ringbucket_t* self)
{
    return ringbuf_get_free_space(&self->ringbuf);
}

bool ringbucket_is_empty(const ringbucket_t* self)
{
    return self->count == 0;
}

void ringbucket_reset(ringbucket_t* self)
{
    ringbuf_clear(&self->ringbuf);
    self->count = 0;
}

size_t ringbucket_count(const ringbucket_t* self)
{
    return self->count;
}

void ringbucket_remove_first(ringbucket_t* self)
{
    assert(!ringbucket_is_empty(self));
    erase_element(self);
}

static void prepare_for_append(ringbucket_t* self, size_t data_size)
{
    ringbucket_size_t element_size = data_size + sizeof(element_size);

    assert(element_size <= self->ringbuf.size);

    if (ringbucket_get_free_space(self) < element_size)
        erase_elements_to_fit_size(self, element_size);

    ringbuf_write(&self->ringbuf, &data_size, sizeof(data_size));
}

void ringbucket_append(ringbucket_t* self, const void* data, ringbucket_size_t data_size)
{
    prepare_for_append(self, data_size);
    ringbuf_write(&self->ringbuf, data, data_size);

    self->count++;
}

ringbucket_size_t ringbucket_first_size(const ringbucket_t* self)
{
    assert(!ringbucket_is_empty(self));
    ringbucket_size_t size;
    ringbuf_peek(&self->ringbuf, &size, sizeof(size), 0);
    return size;
}

void ringbucket_copy_first(const ringbucket_t* self, void* dest)
{
    assert(!ringbucket_is_empty(self));
    ringbucket_size_t first_size;
    ringbuf_peek(&self->ringbuf, &first_size, sizeof(first_size), 0);
    ringbuf_peek(&self->ringbuf, dest, first_size, sizeof(first_size));
}
