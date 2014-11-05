#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    void (*write)(void* self, const void* data, size_t size);
    void (*write_uint8)(void* self, uint8_t value);
    void (*write_uint16le)(void* self, uint16_t value);
    void (*write_uint32le)(void* self, uint32_t value);
    void (*write_uint64le)(void* self, uint64_t value);
    void (*write_uint16be)(void* self, uint16_t value);
    void (*write_uint32be)(void* self, uint32_t value);
    void (*write_uint64be)(void* self, uint64_t value);
} bmcl_writer_impl_t;

typedef struct {
    void* data;
    const bmcl_writer_impl_t* impl;
} bmcl_writer_t;

#if BMCL_HAVE_MALLOC

static inline void bmcl_writer_destroy(bmcl_writer_t* self)
{
    free(self);
}

#endif

static inline void bmcl_writer_write(bmcl_writer_t* self, const void* data, size_t size)
{
    self->impl->write(self->data, data, size);
}

static inline void bmcl_writer_write_uint8(bmcl_writer_t* self, uint8_t value)
{
    self->impl->write_uint8(self->data, value);
}

static inline void bmcl_writer_write_uint16le(bmcl_writer_t* self, uint16_t value)
{
    self->impl->write_uint16le(self->data, value);
}

static inline void bmcl_writer_write_uint32le(bmcl_writer_t* self, uint32_t value)
{
    self->impl->write_uint32le(self->data, value);
}

static inline void bmcl_writer_write_uint64le(bmcl_writer_t* self, uint64_t value)
{
    self->impl->write_uint64le(self->data, value);
}

static inline void bmcl_writer_write_uint16be(bmcl_writer_t* self, uint16_t value)
{
    self->impl->write_uint16be(self->data, value);
}

static inline void bmcl_writer_write_uint32be(bmcl_writer_t* self, uint32_t value)
{
    self->impl->write_uint32be(self->data, value);
}

static inline void bmcl_writer_write_uint64be(bmcl_writer_t* self, uint64_t value)
{
    self->impl->write_uint64be(self->data, value);
}
