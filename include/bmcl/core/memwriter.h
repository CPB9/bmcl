#pragma once

#include "bmcl/core/writer.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t* start;
    uint8_t* current;
    const uint8_t* end;
} memwriter_t;

void memwriter_init(memwriter_t* self, void* dest, size_t max_size);

void memwriter_init_writer(memwriter_t* self, writer_t* writer);

#if BMCL_HAVE_MALLOC

memwriter_t* memwriter_create(size_t max_size);

memwriter_t* memwriter_create_with_dest(void* dest, size_t max_size);

void memwriter_destroy(memwriter_t* self);

writer_t* memwriter_create_writer(memwriter_t* self);

#endif

const void* memwriter_ptr(const memwriter_t* self);

const void* memwriter_current_ptr(const memwriter_t* self);

size_t memwriter_size(const memwriter_t* self); /* rename -> size_used */

bool memwriter_is_full(const memwriter_t* self);

size_t memwriter_max_size(const memwriter_t* self);

size_t memwriter_size_left(const memwriter_t* self);

void memwriter_write(memwriter_t* self, const void* data, size_t size);

void memwriter_fill(memwriter_t* self, uint8_t byte, size_t size);

void memwriter_fillup(memwriter_t* self, uint8_t byte);

void memwriter_write_uint8(memwriter_t* self, uint8_t value);

void memwriter_write_uint16le(memwriter_t* self, uint16_t value);

void memwriter_write_uint32le(memwriter_t* self, uint32_t value);

void memwriter_write_uint64le(memwriter_t* self, uint64_t value);

void memwriter_write_uint16be(memwriter_t* self, uint16_t value);

void memwriter_write_uint32be(memwriter_t* self, uint32_t value);

void memwriter_write_uint64be(memwriter_t* self, uint64_t value);

#ifdef __cplusplus
}
#endif
