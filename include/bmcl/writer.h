#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct {
    void* data;
    const struct writer_impl_s* impl;
} writer_t;

typedef struct writer_impl_s {
    void (*write)(void* self, const void* data, size_t size);
    void (*write_uint8)(void* self, uint8_t value);
    void (*write_uint16le)(void* self, uint16_t value);
    void (*write_uint32le)(void* self, uint32_t value);
    void (*write_uint64le)(void* self, uint64_t value);
    void (*write_uint16be)(void* self, uint16_t value);
    void (*write_uint32be)(void* self, uint32_t value);
    void (*write_uint64be)(void* self, uint64_t value);
} writer_impl_t;

static inline void writer_write(writer_t* self, const void* data, size_t size)
{
    self->impl->write(self->data, data, size);
}

static inline void writer_write_uint8(writer_t* self, uint8_t value)
{
    self->impl->write_uint8(self->data, value);
}

static inline void writer_write_uint16le(writer_t* self, uint16_t value)
{
    self->impl->write_uint16le(self->data, value);
}

static inline void writer_write_uint32le(writer_t* self, uint32_t value)
{
    self->impl->write_uint32le(self->data, value);
}

static inline void writer_write_uint64le(writer_t* self, uint64_t value)
{
    self->impl->write_uint64le(self->data, value);
}

static inline void writer_write_uint16be(writer_t* self, uint16_t value)
{
    self->impl->write_uint16be(self->data, value);
}

static inline void writer_write_uint32be(writer_t* self, uint32_t value)
{
    self->impl->write_uint32be(self->data, value);
}

static inline void writer_write_uint64be(writer_t* self, uint64_t value)
{
    self->impl->write_uint64be(self->data, value);
}
