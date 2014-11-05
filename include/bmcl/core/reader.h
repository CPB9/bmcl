#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    void (*read)(void* self, const void* dest, size_t size);
    uint8_t (*read_uint8)(void* self);
    uint16_t (*read_uint16le)(void* self);
    uint32_t (*read_uint32le)(void* self);
    uint64_t (*read_uint64le)(void* self);
    uint16_t (*read_uint16be)(void* self);
    uint32_t (*read_uint32be)(void* self);
    uint64_t (*read_uint64be)(void* self);
} bmcl_reader_impl_t;

typedef struct {
    void* data;
    const bmcl_reader_impl_t* impl;
} bmcl_reader_t;

#if BMCL_HAVE_MALLOC

static inline void bmcl_reader_destroy(bmcl_reader_t* self)
{
    free(self);
}

#endif

static inline void bmcl_reader_read(bmcl_reader_t* self, const void* dest, size_t size)
{
    self->impl->read(self->data, dest, size);
}

static inline uint8_t bmcl_reader_read_uint8(bmcl_reader_t* self)
{
    return self->impl->read_uint8(self->data);
}

static inline uint16_t bmcl_reader_read_uint16le(bmcl_reader_t* self)
{
    return self->impl->read_uint16le(self->data);
}

static inline uint32_t bmcl_reader_read_uint32le(bmcl_reader_t* self)
{
    return self->impl->read_uint32le(self->data);
}

static inline uint64_t bmcl_reader_read_uint64le(bmcl_reader_t* self)
{
    return self->impl->read_uint64le(self->data);
}

static inline uint16_t bmcl_reader_read_uint16be(bmcl_reader_t* self)
{
    return self->impl->read_uint16be(self->data);
}

static inline uint32_t bmcl_reader_read_uint32be(bmcl_reader_t* self)
{
    return self->impl->read_uint32be(self->data);
}

static inline uint64_t bmcl_reader_read_uint64be(bmcl_reader_t* self)
{
    return self->impl->read_uint64be(self->data);
}
