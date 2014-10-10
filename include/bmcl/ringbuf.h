#pragma once

#include "bmcl/writer.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    size_t read_offset;
    size_t write_offset;
    size_t size;
    size_t free_space;
    uint8_t* data;
} ringbuf_t;

void ringbuf_init(ringbuf_t* self, void* data, size_t size);

void ringbuf_init_writer(ringbuf_t* self, writer_t* writer);

void ringbuf_clear(ringbuf_t* self);

size_t ringbuf_get_free_space(const ringbuf_t* self);

size_t ringbuf_get_used_space(const ringbuf_t* self);

bool ringbuf_is_full(const ringbuf_t* self);

bool ringbuf_is_empty(const ringbuf_t* self);

void ringbuf_erase(ringbuf_t* self, size_t size);

void ringbuf_write(ringbuf_t* self, const void* data, size_t size);

void ringbuf_write_uint8(ringbuf_t* self, uint8_t byte);

void ringbuf_write_uint16le(ringbuf_t* self, uint16_t value);

void ringbuf_write_uint32le(ringbuf_t* self, uint32_t value);

void ringbuf_write_uint64le(ringbuf_t* self, uint64_t value);

void ringbuf_write_uint16be(ringbuf_t* self, uint16_t value);

void ringbuf_write_uint32be(ringbuf_t* self, uint32_t value);

void ringbuf_write_uint64be(ringbuf_t* self, uint64_t value);

void ringbuf_peek(const ringbuf_t* self, void* dest, size_t size, size_t offset);

void ringbuf_read(ringbuf_t* self, void* dest, size_t size);

uint8_t ringbuf_read_uint8(ringbuf_t* self);

uint16_t ringbuf_read_uint16le(ringbuf_t* self);

uint32_t ringbuf_read_uint32le(ringbuf_t* self);

uint64_t ringbuf_read_uint64le(ringbuf_t* self);

uint16_t ringbuf_read_uint16be(ringbuf_t* self);

uint32_t ringbuf_read_uint32be(ringbuf_t* self);

uint64_t ringbuf_read_uint64be(ringbuf_t* self);

void ringbuf_rewind(ringbuf_t* self, size_t size);

#ifdef __cplusplus
}
#endif
