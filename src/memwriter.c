#include "bmcl/endian.h"
#include "bmcl/memwriter.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

void memwriter_init(memwriter_t* self, void* dest, size_t max_size)
{
    self->start = (uint8_t*)dest;
    self->current = self->start;
    self->end = self->start + max_size;
}

const void* memwriter_ptr(const memwriter_t* self)
{
    return self->start;
}

const void* memwriter_current_ptr(const memwriter_t* self)
{
    return self->current;
}

size_t memwriter_size(const memwriter_t* self)
{
    return self->current - self->start;
}

bool memwriter_is_full(const memwriter_t* self)
{
    return self->current == self->end;
}

size_t memwriter_max_size(const memwriter_t* self)
{
    return self->end - self->start;
}

size_t memwriter_size_left(const memwriter_t* self)
{
    return self->end - self->current;
}

void memwriter_write(memwriter_t* self, const void* data, size_t size)
{
    assert(memwriter_size_left(self) >= size);
    memcpy(self->current, data, size);
    self->current += size;
}

void memwriter_fill(memwriter_t* self, uint8_t byte, size_t size)
{
    assert(memwriter_size_left(self) >= size);
    memset(self->current, byte, size);
    self->current += size;
}

void memwriter_fillup(memwriter_t* self, uint8_t byte)
{
    size_t size = self->end - self->current;
    memset(self->current, byte, size);
    self->current += size;
}

void memwriter_write_uint8(memwriter_t* self, uint8_t value)
{
    assert(memwriter_size_left(self) >= 1);
    *self->current = value;
    self->current++;
}

void memwriter_write_uint16le(memwriter_t* self, uint16_t value)
{
    assert(memwriter_size_left(self) >= 2);
    le16enc(self->current, value);
    self->current += 2;
}

void memwriter_write_uint32le(memwriter_t* self, uint32_t value)
{
    assert(memwriter_size_left(self) >= 4);
    le32enc(self->current, value);
    self->current += 4;
}

void memwriter_write_uint64le(memwriter_t* self, uint64_t value)
{
    assert(memwriter_size_left(self) >= 8);
    le64enc(self->current, value);
    self->current += 8;
}

void memwriter_write_uint16be(memwriter_t* self, uint16_t value)
{
    assert(memwriter_size_left(self) >= 2);
    be16enc(self->current, value);
    self->current += 2;
}

void memwriter_write_uint32be(memwriter_t* self, uint32_t value)
{
    assert(memwriter_size_left(self) >= 4);
    be32enc(self->current, value);
    self->current += 4;
}

void memwriter_write_uint64be(memwriter_t* self, uint64_t value)
{
    assert(memwriter_size_left(self) >= 8);
    be64enc(self->current, value);
    self->current += 8;
}
