#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const uint8_t* start;
    const uint8_t* current;
    const uint8_t* end;
} memreader_t;

void memreader_init(memreader_t* self, const void* ptr, size_t size);

const uint8_t* memreader_current_ptr(const memreader_t* self);

size_t memreader_size(const memreader_t* self);

size_t memreader_size_left(const memreader_t* self);

size_t memreader_size_read(const memreader_t* self);

bool memreader_is_empty(const memreader_t* self);

void memreader_skip(memreader_t* self, size_t size);

void memreader_peek(const memreader_t* self, void* dest, size_t size, size_t offset);

void memreader_read(memreader_t* self, void* dest, size_t size);

uint8_t memreader_read_uint8(memreader_t* self);

uint16_t memreader_read_uint16le(memreader_t* self);

uint32_t memreader_read_uint32le(memreader_t* self);

uint64_t memreader_read_uint64le(memreader_t* self);

uint16_t memreader_read_uint16be(memreader_t* self);

uint32_t memreader_read_uint32be(memreader_t* self);

uint64_t memreader_read_uint64be(memreader_t* self);

#ifdef __cplusplus
}
#endif
