#include "bmcl/core/endian.h"
#include "bmcl/core/ringbuf.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define min(a, b) (((a) > (b)) ? (b) : (a))

#define MAKE_READ_FUNC(type, suffix, decode_func, encode_func)                                                         \
    type bmcl_ringbuf_read_##suffix(bmcl_ringbuf_t* self)                                                              \
    {                                                                                                                  \
        type data;                                                                                                     \
        bmcl_ringbuf_peek(self, &data, sizeof(type), 0);                                                               \
        bmcl_ringbuf_erase(self, sizeof(type));                                                                        \
        return decode_func(data);                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    static bmcl_status_t read_##suffix(void* ringbuf, type* dest)                                                             \
    {                                                                                                                  \
        bmcl_ringbuf_t* self = (bmcl_ringbuf_t*)ringbuf;                                                               \
        if (sizeof(type) > self->size - self->free_space) {                                                            \
            return BMCL_ERR_NOT_ENOUGH_SPACE;                                                                          \
        }                                                                                                              \
        type data;                                                                                                     \
        bmcl_ringbuf_peek(self, &data, sizeof(type), 0);                                                               \
        bmcl_ringbuf_erase(self, sizeof(type));                                                                        \
        *dest = decode_func(data);                                                                                     \
        return BMCL_SUCCESS;                                                                                           \
    }                                                                                                                  \
                                                                                                                       \
    void bmcl_ringbuf_write_##suffix(bmcl_ringbuf_t* self, type value)                                                 \
    {                                                                                                                  \
        type data = encode_func(value);                                                                                \
        bmcl_ringbuf_write(self, &data, sizeof(type));                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    static bmcl_status_t write_##suffix(void* ringbuf, type value)                                                            \
    {                                                                                                                  \
        bmcl_ringbuf_t* self = (bmcl_ringbuf_t*)ringbuf;                                                               \
        type data = encode_func(value);                                                                                \
        bmcl_ringbuf_write(self, &data, sizeof(type));                                                                 \
        return BMCL_SUCCESS;                                                                                           \
    }

#define id(value) value

MAKE_READ_FUNC(uint8_t, uint8, id, id);
MAKE_READ_FUNC(uint16_t, uint16le, le16toh, htole16);
MAKE_READ_FUNC(uint32_t, uint32le, le32toh, htole32);
MAKE_READ_FUNC(uint64_t, uint64le, le64toh, htole64);
MAKE_READ_FUNC(uint16_t, uint16be, be16toh, htobe16);
MAKE_READ_FUNC(uint32_t, uint32be, be32toh, htobe32);
MAKE_READ_FUNC(uint64_t, uint64be, be64toh, htobe64);

static bmcl_status_t read(void* ringbuf, void* dest, size_t size)
{
    bmcl_ringbuf_t* self = (bmcl_ringbuf_t*)ringbuf;
    bmcl_ringbuf_peek(self, dest, size, 0);
    bmcl_ringbuf_erase(self, size);
    return BMCL_SUCCESS;
}

static bmcl_status_t write(void* ringbuf, const void* src, size_t size)
{
    bmcl_ringbuf_t* self = (bmcl_ringbuf_t*)ringbuf;
    bmcl_ringbuf_write(self, src, size);
    return BMCL_SUCCESS;
}

static const bmcl_reader_impl_t ringbuf_reader_impl
    = {read, read_uint8, read_uint16le, read_uint32le, read_uint64le, read_uint16be, read_uint32be, read_uint64be};

static const bmcl_writer_impl_t ringbuf_writer_impl = {write,          write_uint8,    write_uint16le, write_uint32le,
                                                       write_uint64le, write_uint16be, write_uint32be, write_uint64be};

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
