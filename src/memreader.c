#include "bmcl/endian.h"
#include "bmcl/memreader.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void memreader_init(memreader_t* self, const void* ptr, size_t size)
{
    self->start = (uint8_t*)ptr;
    self->current = self->start;
    self->end = self->start + size;
}

const uint8_t* memreader_current_ptr(const memreader_t* self)
{
    return self->current;
}

size_t memreader_size(const memreader_t* self)
{
    return self->end - self->start;
}

size_t memreader_size_left(const memreader_t* self)
{
    return self->end - self->current;
}

size_t memreader_size_read(const memreader_t* self)
{
    return self->current - self->start;
}

bool memreader_is_empty(const memreader_t* self)
{
    return self->current == self->end;
}

void memreader_skip(memreader_t* self, size_t size)
{
    assert(memreader_size_left(self) >= size);
    self->current += size;
}

void memreader_peek(const memreader_t* self, void* dest, size_t size, size_t offset)
{
    assert(memreader_size_left(self) >= size + offset);
    memcpy(dest, self->current + offset, size);
}

void memreader_read(memreader_t* self, void* dest, size_t size)
{
    assert(memreader_size_left(self) >= size);
    memcpy(dest, self->current, size);
    self->current += size;
}

uint8_t memreader_read_uint8(memreader_t* self)
{
    assert(memreader_size_left(self) >= 1);
    uint8_t value = *self->current;
    self->current += 1;
    return value;
}

uint16_t memreader_read_uint16le(memreader_t* self)
{
    assert(memreader_size_left(self) >= 2);
    uint16_t value = le16dec(self->current);
    self->current += 2;
    return value;
}

uint32_t memreader_read_uint32le(memreader_t* self)
{
    assert(memreader_size_left(self) >= 4);
    uint32_t value = le32dec(self->current);
    self->current += 4;
    return value;
}

uint64_t memreader_read_uint64le(memreader_t* self)
{
    assert(memreader_size_left(self) >= 8);
    uint64_t value = le64dec(self->current);
    self->current += 8;
    return value;
}

uint16_t memreader_read_uint16be(memreader_t* self)
{
    assert(memreader_size_left(self) >= 2);
    uint16_t value = be16dec(self->current);
    self->current += 2;
    return value;
}

uint32_t memreader_read_uint32be(memreader_t* self)
{
    assert(memreader_size_left(self) >= 4);
    uint32_t value = be32dec(self->current);
    self->current += 4;
    return value;
}

uint64_t memreader_read_uint64be(memreader_t* self)
{
    assert(memreader_size_left(self) >= 8);
    uint64_t value = be64dec(self->current);
    self->current += 8;
    return value;
}
