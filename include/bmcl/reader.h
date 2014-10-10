#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct {
    void* data;
    const struct reader_impl_s* impl;
} reader_t;

typedef struct reader_impl_s {
    void (*read)(void* self, const void* dest, size_t size);
    uint8_t (*read_uint8)(void* self);
    uint16_t (*read_uint16le)(void* self);
    uint32_t (*read_uint32le)(void* self);
    uint64_t (*read_uint64le)(void* self);
    uint16_t (*read_uint16be)(void* self);
    uint32_t (*read_uint32be)(void* self);
    uint64_t (*read_uint64be)(void* self);
} reader_impl_t;

static inline void reader_read(reader_t* self, const void* dest, size_t size)
{
    self->impl->read(self->data, dest, size);
}

static inline uint8_t reader_read_uint8(reader_t* self)
{
    return self->impl->read_uint8(self->data);
}

static inline uint16_t reader_read_uint16le(reader_t* self)
{
    return self->impl->read_uint16le(self->data);
}

static inline uint32_t reader_read_uint32le(reader_t* self)
{
    return self->impl->read_uint32le(self->data);
}

static inline uint64_t reader_read_uint64le(reader_t* self)
{
    return self->impl->read_uint64le(self->data);
}

static inline uint16_t reader_read_uint16be(reader_t* self)
{
    return self->impl->read_uint16be(self->data);
}

static inline uint32_t reader_read_uint32be(reader_t* self)
{
    return self->impl->read_uint32be(self->data);
}

static inline uint64_t reader_read_uint64be(reader_t* self)
{
    return self->impl->read_uint64be(self->data);
}
