#include "bmcl/core/endian.h"
#include "bmcl/core/memreader.h"
#include "bmcl/core/status.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const bmcl_reader_impl_t memreader_reader_impl = {
    (void (*)(void*, const void*, size_t))bmcl_memreader_read, (uint8_t (*)(void*))bmcl_memreader_read_uint8,
    (uint16_t (*)(void*))bmcl_memreader_read_uint16le,         (uint32_t (*)(void*))bmcl_memreader_read_uint32le,
    (uint64_t (*)(void*))bmcl_memreader_read_uint64le,         (uint16_t (*)(void*))bmcl_memreader_read_uint16be,
    (uint32_t (*)(void*))bmcl_memreader_read_uint32be,         (uint64_t (*)(void*))bmcl_memreader_read_uint64be,
};

void bmcl_memreader_init(bmcl_memreader_t* self, const void* ptr, size_t size)
{
    self->start = (uint8_t*)ptr;
    self->current = self->start;
    self->end = self->start + size;
}

void bmcl_memreader_init_reader(bmcl_memreader_t* self, bmcl_reader_t* reader)
{
    reader->data = self;
    reader->impl = &memreader_reader_impl;
}

#if BMCL_HAVE_MALLOC

bmcl_memreader_t* bmcl_memreader_create(const void* ptr, size_t size)
{
    bmcl_memreader_t* self = malloc(sizeof(bmcl_memreader_t));
    bmcl_memreader_init(self, ptr, size);
    return self;
}

void bmcl_memreader_destroy(bmcl_memreader_t* self)
{
    free(self);
}

bmcl_reader_t* bmcl_memreader_create_reader(bmcl_memreader_t* self)
{
    bmcl_reader_t* reader = malloc(sizeof(bmcl_reader_t));
    bmcl_memreader_init_reader(self, reader);
    return reader;
}

#endif

const uint8_t* bmcl_memreader_current_ptr(const bmcl_memreader_t* self)
{
    return self->current;
}

size_t bmcl_memreader_size(const bmcl_memreader_t* self)
{
    return self->end - self->start;
}

size_t bmcl_memreader_size_left(const bmcl_memreader_t* self)
{
    return self->end - self->current;
}

size_t bmcl_memreader_size_read(const bmcl_memreader_t* self)
{
    return self->current - self->start;
}

bool bmcl_memreader_is_empty(const bmcl_memreader_t* self)
{
    return self->current == self->end;
}

void bmcl_memreader_skip(bmcl_memreader_t* self, size_t size)
{
    assert(bmcl_memreader_size_left(self) >= size);
    self->current += size;
}

void bmcl_memreader_peek(const bmcl_memreader_t* self, void* dest, size_t size, size_t offset)
{
    assert(bmcl_memreader_size_left(self) >= size + offset);
    memcpy(dest, self->current + offset, size);
}

void bmcl_memreader_read(bmcl_memreader_t* self, void* dest, size_t size)
{
    assert(bmcl_memreader_size_left(self) >= size);
    memcpy(dest, self->current, size);
    self->current += size;
}

#define MAKE_MEMREADER(type, suffix, dec_func)                                                                         \
    type bmcl_memreader_read_##suffix(bmcl_memreader_t* self)                                                          \
    {                                                                                                                  \
        assert(bmcl_memreader_size_left(self) >= sizeof(type));                                                        \
        type value = dec_func(self->current);                                                                          \
        self->current += sizeof(type);                                                                                 \
        return value;                                                                                                  \
    }

MAKE_MEMREADER(uint8_t, uint8, *);
MAKE_MEMREADER(uint16_t, uint16le, le16dec);
MAKE_MEMREADER(uint32_t, uint32le, le32dec);
MAKE_MEMREADER(uint64_t, uint64le, le64dec);
MAKE_MEMREADER(uint16_t, uint16be, be16dec);
MAKE_MEMREADER(uint32_t, uint32be, be32dec);
MAKE_MEMREADER(uint64_t, uint64be, be64dec);

static bmcl_status_t read_uint8(void* memreader, uint8_t* dest)
{
    bmcl_memreader_t* self = (bmcl_memreader_t*)memreader;
    if (bmcl_memreader_size_left(self) >= 1) {
    }
    *dest = *self->current;
    return BMCL_SUCCESS;
}
