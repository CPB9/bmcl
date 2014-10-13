#include "bmcl/endian.h"
#include "bmcl/ringbuf.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define Sys_MIN(a, b) (((a) > (b)) ? (b) : (a))

static const reader_impl_t ringbuf_reader_impl = {
    (void (*)(void*, const void*, size_t))ringbuf_read,
    (uint8_t (*)(void*))ringbuf_read_uint8,
    (uint16_t (*)(void*))ringbuf_read_uint16le,
    (uint32_t (*)(void*))ringbuf_read_uint32le,
    (uint64_t (*)(void*))ringbuf_read_uint64le,
    (uint16_t (*)(void*))ringbuf_read_uint16le,
    (uint32_t (*)(void*))ringbuf_read_uint32le,
    (uint64_t (*)(void*))ringbuf_read_uint64le,
};

static const writer_impl_t ringbuf_writer_impl = {
    (void (*)(void*, const void*, size_t))ringbuf_write,
    (void (*)(void*, uint8_t))ringbuf_write_uint8,
    (void (*)(void*, uint16_t))ringbuf_write_uint16le,
    (void (*)(void*, uint32_t))ringbuf_write_uint32le,
    (void (*)(void*, uint64_t))ringbuf_write_uint64le,
    (void (*)(void*, uint16_t))ringbuf_write_uint16be,
    (void (*)(void*, uint32_t))ringbuf_write_uint32be,
    (void (*)(void*, uint64_t))ringbuf_write_uint64be,
};

void ringbuf_init(ringbuf_t* self, void* data, size_t size)
{
    assert(size > 0);
    self->data = data;
    self->size = size;
    self->free_space = size;
    self->read_offset = 0;
    self->write_offset = 0;
}

void ringbuf_init_reader(ringbuf_t* self, reader_t* reader)
{
    reader->data = self;
    reader->impl = &ringbuf_reader_impl;
}

void ringbuf_init_writer(ringbuf_t* self, writer_t* writer)
{
    writer->data = self;
    writer->impl = &ringbuf_writer_impl;
}

void ringbuf_clear(ringbuf_t* self)
{
    self->write_offset = 0;
    self->read_offset = 0;
    self->free_space = self->size;
}

size_t ringbuf_get_free_space(const ringbuf_t* self)
{
    return self->free_space;
}

size_t ringbuf_get_used_space(const ringbuf_t* self)
{
    return self->size - self->free_space;
}

bool ringbuf_is_full(const ringbuf_t* self)
{
    return self->free_space == 0;
}

bool ringbuf_is_empty(const ringbuf_t* self)
{
    return self->free_space == self->size;
}

void ringbuf_erase(ringbuf_t* self, size_t size)
{
    assert(self->size - self->free_space >= size);
    self->free_space += size;
    self->read_offset += size;
    if (self->read_offset >= self->size) {
        self->read_offset -= self->size;
    }
}

static void extend(ringbuf_t* self, size_t size)
{
    self->write_offset += size;
    if (self->write_offset >= self->size) {
        self->write_offset -= self->size;
    }
    self->free_space -= size;
}

void ringbuf_write(ringbuf_t* self, const void* data, size_t size)
{
    assert(size > 0);
    assert(size <= self->size);
    if (self->free_space < size) {
        ringbuf_erase(self, size - self->free_space);
    }

    if (self->read_offset > self->write_offset) { /* *********w---------------r************ */
        memcpy(self->data + self->write_offset, data, size);
    } else { /* ----------r**************w---------- */
        size_t right_data = self->size - self->write_offset;
        size_t first_chunk_size = Sys_MIN(size, right_data);
        memcpy(self->data + self->write_offset, data, first_chunk_size);
        if (size > first_chunk_size) {
            size_t second_chunk_size = size - first_chunk_size;
            memcpy(self->data, (const uint8_t*)data + first_chunk_size, second_chunk_size);
        }
    }
    extend(self, size);
}

void ringbuf_write_uint8(ringbuf_t* self, uint8_t byte)
{
    if (self->free_space < 1) {
        ringbuf_erase(self, 1);
    }

    *(self->data + self->write_offset) = byte;
    extend(self, 1);
}

void ringbuf_write_uint16le(ringbuf_t* self, uint16_t value)
{
    uint16_t data = htole16(value);
    ringbuf_write(self, &data, 2);
}

void ringbuf_write_uint32le(ringbuf_t* self, uint32_t value)
{
    uint32_t data = htole32(value);
    ringbuf_write(self, &data, 4);
}

void ringbuf_write_uint64le(ringbuf_t* self, uint64_t value)
{
    uint64_t data = htole64(value);
    ringbuf_write(self, &data, 8);
}

void ringbuf_write_uint16be(ringbuf_t* self, uint16_t value)
{
    uint16_t data = htobe16(value);
    ringbuf_write(self, &data, 2);
}

void ringbuf_write_uint32be(ringbuf_t* self, uint32_t value)
{
    uint32_t data = htobe32(value);
    ringbuf_write(self, &data, 4);
}

void ringbuf_write_uint64be(ringbuf_t* self, uint64_t value)
{
    uint64_t data = htobe64(value);
    ringbuf_write(self, &data, 8);
}

void ringbuf_peek(const ringbuf_t* self, void* dest, size_t size, size_t offset)
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
        size_t first_chunk_size = Sys_MIN(size, right_data);
        memcpy(dest, self->data + read_offset, first_chunk_size);
        if (size > first_chunk_size) {
            size_t second_chunk_size = size - first_chunk_size;
            memcpy((uint8_t*)dest + first_chunk_size, self->data, second_chunk_size);
        }
    }
}

void ringbuf_read(ringbuf_t* self, void* dest, size_t size)
{
    ringbuf_peek(self, dest, size, 0);
    ringbuf_erase(self, size);
}

uint8_t ringbuf_read_uint8(ringbuf_t* self)
{
    uint8_t byte = *(self->data + self->read_offset);
    ringbuf_erase(self, 1);
    return byte;
}

uint16_t ringbuf_read_uint16le(ringbuf_t* self)
{
    uint16_t data;
    ringbuf_peek(self, &data, 2, 0);
    ringbuf_erase(self, 2);
    return le16toh(data);
}

uint32_t ringbuf_read_uint32le(ringbuf_t* self)
{
    uint32_t data;
    ringbuf_peek(self, &data, 4, 0);
    ringbuf_erase(self, 4);
    return le32toh(data);
}

uint64_t ringbuf_read_uint64le(ringbuf_t* self)
{
    uint64_t data;
    ringbuf_peek(self, &data, 8, 0);
    ringbuf_erase(self, 8);
    return le64toh(data);
}

uint16_t ringbuf_read_uint16be(ringbuf_t* self)
{
    uint16_t data;
    ringbuf_peek(self, &data, 2, 0);
    ringbuf_erase(self, 2);
    return be16toh(data);
}

uint32_t ringbuf_read_uint32be(ringbuf_t* self)
{
    uint32_t data;
    ringbuf_peek(self, &data, 4, 0);
    ringbuf_erase(self, 4);
    return be32toh(data);
}

uint64_t ringbuf_read_uint64be(ringbuf_t* self)
{
    uint64_t data;
    ringbuf_peek(self, &data, 8, 0);
    ringbuf_erase(self, 8);
    return be64toh(data);
}

void ringbuf_rewind(ringbuf_t* self, size_t size)
{
    assert(ringbuf_get_used_space(self) >= size);
    self->free_space += size;

    if (self->write_offset < size) {
        self->write_offset = self->size - (size - self->write_offset);
    } else {
        self->write_offset -= size;
    }
}
