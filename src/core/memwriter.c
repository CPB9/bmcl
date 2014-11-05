#include "bmcl/core/endian.h"
#include "bmcl/core/memwriter.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

static const bmcl_writer_impl_t memwriter_writer_impl = {
    (void (*)(void*, const void*, size_t))bmcl_memwriter_write,
    (void (*)(void*, uint8_t))bmcl_memwriter_write_uint8,
    (void (*)(void*, uint16_t))bmcl_memwriter_write_uint16le,
    (void (*)(void*, uint32_t))bmcl_memwriter_write_uint32le,
    (void (*)(void*, uint64_t))bmcl_memwriter_write_uint64le,
    (void (*)(void*, uint16_t))bmcl_memwriter_write_uint16be,
    (void (*)(void*, uint32_t))bmcl_memwriter_write_uint32be,
    (void (*)(void*, uint64_t))bmcl_memwriter_write_uint64be,
};

void bmcl_memwriter_init(bmcl_memwriter_t* self, void* dest, size_t max_size)
{
    self->start = (uint8_t*)dest;
    self->current = self->start;
    self->end = self->start + max_size;
}

void bmcl_memwriter_init_writer(bmcl_memwriter_t* self, bmcl_writer_t* writer)
{
    writer->data = self;
    writer->impl = &memwriter_writer_impl;
}

#if BMCL_HAVE_MALLOC

bmcl_memwriter_t* bmcl_memwriter_create(size_t max_size)
{
    uint8_t* data = malloc(sizeof(bmcl_memwriter_t) + max_size);
    bmcl_memwriter_t* self = (bmcl_memwriter_t*)data;
    bmcl_memwriter_init(self, data + sizeof(bmcl_memwriter_t), max_size);
    return self;
}

bmcl_memwriter_t* bmcl_memwriter_create_with_dest(void* dest, size_t max_size)
{
    bmcl_memwriter_t* self = malloc(sizeof(bmcl_memwriter_t));
    bmcl_memwriter_init(self, dest, max_size);
    return self;
}

void bmcl_memwriter_destroy(bmcl_memwriter_t* self)
{
    free(self);
}

bmcl_writer_t* bmcl_memwriter_create_writer(bmcl_memwriter_t* self)
{
    bmcl_writer_t* writer = malloc(sizeof(bmcl_writer_t));
    bmcl_memwriter_init_writer(self, writer);
    return writer;
}

#endif

const void* bmcl_memwriter_ptr(const bmcl_memwriter_t* self)
{
    return self->start;
}

const void* bmcl_memwriter_current_ptr(const bmcl_memwriter_t* self)
{
    return self->current;
}

size_t bmcl_memwriter_size(const bmcl_memwriter_t* self)
{
    return self->current - self->start;
}

bool bmcl_memwriter_is_full(const bmcl_memwriter_t* self)
{
    return self->current == self->end;
}

size_t bmcl_memwriter_max_size(const bmcl_memwriter_t* self)
{
    return self->end - self->start;
}

size_t bmcl_memwriter_size_left(const bmcl_memwriter_t* self)
{
    return self->end - self->current;
}

void bmcl_memwriter_write(bmcl_memwriter_t* self, const void* data, size_t size)
{
    assert(bmcl_memwriter_size_left(self) >= size);
    memcpy(self->current, data, size);
    self->current += size;
}

void bmcl_memwriter_fill(bmcl_memwriter_t* self, uint8_t byte, size_t size)
{
    assert(bmcl_memwriter_size_left(self) >= size);
    memset(self->current, byte, size);
    self->current += size;
}

void bmcl_memwriter_fillup(bmcl_memwriter_t* self, uint8_t byte)
{
    size_t size = self->end - self->current;
    memset(self->current, byte, size);
    self->current += size;
}

void bmcl_memwriter_write_uint8(bmcl_memwriter_t* self, uint8_t value)
{
    assert(bmcl_memwriter_size_left(self) >= 1);
    *self->current = value;
    self->current++;
}

void bmcl_memwriter_write_uint16le(bmcl_memwriter_t* self, uint16_t value)
{
    assert(bmcl_memwriter_size_left(self) >= 2);
    le16enc(self->current, value);
    self->current += 2;
}

void bmcl_memwriter_write_uint32le(bmcl_memwriter_t* self, uint32_t value)
{
    assert(bmcl_memwriter_size_left(self) >= 4);
    le32enc(self->current, value);
    self->current += 4;
}

void bmcl_memwriter_write_uint64le(bmcl_memwriter_t* self, uint64_t value)
{
    assert(bmcl_memwriter_size_left(self) >= 8);
    le64enc(self->current, value);
    self->current += 8;
}

void bmcl_memwriter_write_uint16be(bmcl_memwriter_t* self, uint16_t value)
{
    assert(bmcl_memwriter_size_left(self) >= 2);
    be16enc(self->current, value);
    self->current += 2;
}

void bmcl_memwriter_write_uint32be(bmcl_memwriter_t* self, uint32_t value)
{
    assert(bmcl_memwriter_size_left(self) >= 4);
    be32enc(self->current, value);
    self->current += 4;
}

void bmcl_memwriter_write_uint64be(bmcl_memwriter_t* self, uint64_t value)
{
    assert(bmcl_memwriter_size_left(self) >= 8);
    be64enc(self->current, value);
    self->current += 8;
}
