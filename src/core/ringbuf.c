#include "bmcl/core/endian.h"
#include "bmcl/core/ringbuf.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define min(a, b) (((a) > (b)) ? (b) : (a))

static const bmcl_reader_impl_t ringbuf_reader_impl = {
    (void (*)(void*, const void*, size_t))bmcl_ringbuf_read, (uint8_t (*)(void*))bmcl_ringbuf_read_uint8,
    (uint16_t (*)(void*))bmcl_ringbuf_read_uint16le,         (uint32_t (*)(void*))bmcl_ringbuf_read_uint32le,
    (uint64_t (*)(void*))bmcl_ringbuf_read_uint64le,         (uint16_t (*)(void*))bmcl_ringbuf_read_uint16be,
    (uint32_t (*)(void*))bmcl_ringbuf_read_uint32be,         (uint64_t (*)(void*))bmcl_ringbuf_read_uint64be,
};

static const bmcl_writer_impl_t ringbuf_writer_impl = {
    (void (*)(void*, const void*, size_t))bmcl_ringbuf_write, (void (*)(void*, uint8_t))bmcl_ringbuf_write_uint8,
    (void (*)(void*, uint16_t))bmcl_ringbuf_write_uint16le,   (void (*)(void*, uint32_t))bmcl_ringbuf_write_uint32le,
    (void (*)(void*, uint64_t))bmcl_ringbuf_write_uint64le,   (void (*)(void*, uint16_t))bmcl_ringbuf_write_uint16be,
    (void (*)(void*, uint32_t))bmcl_ringbuf_write_uint32be,   (void (*)(void*, uint64_t))bmcl_ringbuf_write_uint64be,
};

void bmcl_ringbuf_init(bmcl_ringbuf_t* self, void* data, size_t size)
{
    assert(size > 0);
    self->data = data;
    self->size = size;
    self->free_space = size;
    self->read_offset = 0;
    self->write_offset = 0;
}

void bmcl_ringbuf_init_reader(bmcl_ringbuf_t* self, bmcl_reader_t* reader)
{
    reader->data = self;
    reader->impl = &ringbuf_reader_impl;
}

void bmcl_ringbuf_init_writer(bmcl_ringbuf_t* self, bmcl_writer_t* writer)
{
    writer->data = self;
    writer->impl = &ringbuf_writer_impl;
}

#if BMCL_HAVE_MALLOC

bmcl_ringbuf_t* bmcl_ringbuf_create(size_t size)
{
    uint8_t* data = malloc(sizeof(bmcl_ringbuf_t) + size);
    bmcl_ringbuf_t* self = (bmcl_ringbuf_t*)data;
    bmcl_ringbuf_init(self, data + sizeof(bmcl_ringbuf_t), size);
    return self;
}

void bmcl_ringbuf_destroy(bmcl_ringbuf_t* self)
{
    free(self);
}

bmcl_writer_t* bmcl_ringbuf_create_writer(bmcl_ringbuf_t* self)
{
    bmcl_writer_t* writer = malloc(sizeof(bmcl_writer_t));
    bmcl_ringbuf_init_writer(self, writer);
    return writer;
}

bmcl_reader_t* bmcl_ringbuf_create_reader(bmcl_ringbuf_t* self)
{
    bmcl_reader_t* reader = malloc(sizeof(bmcl_reader_t));
    bmcl_ringbuf_init_reader(self, reader);
    return reader;
}

#endif

void bmcl_ringbuf_clear(bmcl_ringbuf_t* self)
{
    self->write_offset = 0;
    self->read_offset = 0;
    self->free_space = self->size;
}

size_t bmcl_ringbuf_get_free_space(const bmcl_ringbuf_t* self)
{
    return self->free_space;
}

size_t bmcl_ringbuf_get_used_space(const bmcl_ringbuf_t* self)
{
    return self->size - self->free_space;
}

bool bmcl_ringbuf_is_full(const bmcl_ringbuf_t* self)
{
    return self->free_space == 0;
}

bool bmcl_ringbuf_is_empty(const bmcl_ringbuf_t* self)
{
    return self->free_space == self->size;
}

void bmcl_ringbuf_erase(bmcl_ringbuf_t* self, size_t size)
{
    assert(self->size - self->free_space >= size);
    self->free_space += size;
    self->read_offset += size;
    if (self->read_offset >= self->size) {
        self->read_offset -= self->size;
    }
}

static void extend(bmcl_ringbuf_t* self, size_t size)
{
    self->write_offset += size;
    if (self->write_offset >= self->size) {
        self->write_offset -= self->size;
    }
    self->free_space -= size;
}

void bmcl_ringbuf_write(bmcl_ringbuf_t* self, const void* data, size_t size)
{
    assert(size > 0);
    assert(size <= self->size);
    if (self->free_space < size) {
        bmcl_ringbuf_erase(self, size - self->free_space);
    }

    if (self->read_offset > self->write_offset) { /* *********w---------------r************ */
        memcpy(self->data + self->write_offset, data, size);
    } else { /* ----------r**************w---------- */
        size_t right_data = self->size - self->write_offset;
        size_t first_chunk_size = min(size, right_data);
        memcpy(self->data + self->write_offset, data, first_chunk_size);
        if (size > first_chunk_size) {
            size_t second_chunk_size = size - first_chunk_size;
            memcpy(self->data, (const uint8_t*)data + first_chunk_size, second_chunk_size);
        }
    }
    extend(self, size);
}

void bmcl_ringbuf_write_uint8(bmcl_ringbuf_t* self, uint8_t byte)
{
    if (self->free_space < 1) {
        bmcl_ringbuf_erase(self, 1);
    }

    *(self->data + self->write_offset) = byte;
    extend(self, 1);
}

void bmcl_ringbuf_write_uint16le(bmcl_ringbuf_t* self, uint16_t value)
{
    uint16_t data = htole16(value);
    bmcl_ringbuf_write(self, &data, 2);
}

void bmcl_ringbuf_write_uint32le(bmcl_ringbuf_t* self, uint32_t value)
{
    uint32_t data = htole32(value);
    bmcl_ringbuf_write(self, &data, 4);
}

void bmcl_ringbuf_write_uint64le(bmcl_ringbuf_t* self, uint64_t value)
{
    uint64_t data = htole64(value);
    bmcl_ringbuf_write(self, &data, 8);
}

void bmcl_ringbuf_write_uint16be(bmcl_ringbuf_t* self, uint16_t value)
{
    uint16_t data = htobe16(value);
    bmcl_ringbuf_write(self, &data, 2);
}

void bmcl_ringbuf_write_uint32be(bmcl_ringbuf_t* self, uint32_t value)
{
    uint32_t data = htobe32(value);
    bmcl_ringbuf_write(self, &data, 4);
}

void bmcl_ringbuf_write_uint64be(bmcl_ringbuf_t* self, uint64_t value)
{
    uint64_t data = htobe64(value);
    bmcl_ringbuf_write(self, &data, 8);
}

void bmcl_ringbuf_peek(const bmcl_ringbuf_t* self, void* dest, size_t size, size_t offset)
{
    size_t read_offset = self->read_offset + offset;
    if (read_offset >= self->size) {
        read_offset -= self->size;
    }
    assert(size > 0);
    assert(size + offset <= self->size - self->free_space);
    if (read_offset < self->write_offset) { /* ----------r**************w---------- */
        memcpy(dest, self->data + read_offset, size);
    } else { /* *********w---------------r************ */
        size_t right_data = self->size - read_offset;
        size_t first_chunk_size = min(size, right_data);
        memcpy(dest, self->data + read_offset, first_chunk_size);
        if (size > first_chunk_size) {
            size_t second_chunk_size = size - first_chunk_size;
            memcpy((uint8_t*)dest + first_chunk_size, self->data, second_chunk_size);
        }
    }
}

void bmcl_ringbuf_read(bmcl_ringbuf_t* self, void* dest, size_t size)
{
    bmcl_ringbuf_peek(self, dest, size, 0);
    bmcl_ringbuf_erase(self, size);
}

uint8_t bmcl_ringbuf_read_uint8(bmcl_ringbuf_t* self)
{
    uint8_t byte = *(self->data + self->read_offset);
    bmcl_ringbuf_erase(self, 1);
    return byte;
}

uint16_t bmcl_ringbuf_read_uint16le(bmcl_ringbuf_t* self)
{
    uint16_t data;
    bmcl_ringbuf_peek(self, &data, 2, 0);
    bmcl_ringbuf_erase(self, 2);
    return le16toh(data);
}

uint32_t bmcl_ringbuf_read_uint32le(bmcl_ringbuf_t* self)
{
    uint32_t data;
    bmcl_ringbuf_peek(self, &data, 4, 0);
    bmcl_ringbuf_erase(self, 4);
    return le32toh(data);
}

uint64_t bmcl_ringbuf_read_uint64le(bmcl_ringbuf_t* self)
{
    uint64_t data;
    bmcl_ringbuf_peek(self, &data, 8, 0);
    bmcl_ringbuf_erase(self, 8);
    return le64toh(data);
}

uint16_t bmcl_ringbuf_read_uint16be(bmcl_ringbuf_t* self)
{
    uint16_t data;
    bmcl_ringbuf_peek(self, &data, 2, 0);
    bmcl_ringbuf_erase(self, 2);
    return be16toh(data);
}

uint32_t bmcl_ringbuf_read_uint32be(bmcl_ringbuf_t* self)
{
    uint32_t data;
    bmcl_ringbuf_peek(self, &data, 4, 0);
    bmcl_ringbuf_erase(self, 4);
    return be32toh(data);
}

uint64_t bmcl_ringbuf_read_uint64be(bmcl_ringbuf_t* self)
{
    uint64_t data;
    bmcl_ringbuf_peek(self, &data, 8, 0);
    bmcl_ringbuf_erase(self, 8);
    return be64toh(data);
}
