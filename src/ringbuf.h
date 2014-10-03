#pragma once

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

void ringbuf_clear(ringbuf_t* self);

size_t ringbuf_get_free_space(const ringbuf_t* self);

size_t ringbuf_get_used_space(const ringbuf_t* self);

bool ringbuf_is_full(const ringbuf_t* self);

bool ringbuf_is_empty(const ringbuf_t* self);

void ringbuf_erase(ringbuf_t* self, size_t size);

void ringbuf_append(ringbuf_t* self, const void* data, size_t size);

void ringbuf_append_uint8(ringbuf_t* self, uint8_t byte);

void ringbuf_append_uint16(ringbuf_t* self, uint16_t data);

void ringbuf_append_uint32(ringbuf_t* self, uint32_t data);

void ringbuf_append_uint64(ringbuf_t* self, uint64_t data);

void ringbuf_peek(const ringbuf_t* self, void* dest, size_t size, size_t offset);

uint8_t ringbuf_peek_uint8(const ringbuf_t* self);

uint16_t ringbuf_peek_uint16(const ringbuf_t* self);

uint32_t ringbuf_peek_uint32(const ringbuf_t* self);

uint64_t ringbuf_peek_uint64(const ringbuf_t* self);

void ringbuf_read(ringbuf_t* self, void* dest, size_t size);

uint8_t ringbuf_read_uint8(ringbuf_t* self);

uint16_t ringbuf_read_uint16(ringbuf_t* self);

uint32_t ringbuf_read_uint32(ringbuf_t* self);

uint64_t ringbuf_read_uint64(ringbuf_t* self);

void ringbuf_rewind(ringbuf_t* self, size_t size);

#ifdef __cplusplus
}
#endif
