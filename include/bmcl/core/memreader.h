#pragma once

#include "bmcl/core/reader.h"

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
} bmcl_memreader_t;

void bmcl_memreader_init(bmcl_memreader_t* self, const void* ptr, size_t size);

void bmcl_memreader_init_reader(bmcl_memreader_t* self, bmcl_reader_t* reader);

#if BMCL_HAVE_MALLOC

bmcl_memreader_t* bmcl_memreader_create(const void* ptr, size_t size);

void bmcl_memreader_destroy(bmcl_memreader_t* self);

bmcl_reader_t* bmcl_memreader_create_reader(bmcl_memreader_t* self);

#endif

const uint8_t* bmcl_memreader_current_ptr(const bmcl_memreader_t* self);

size_t bmcl_memreader_size(const bmcl_memreader_t* self);

size_t bmcl_memreader_size_left(const bmcl_memreader_t* self);

size_t bmcl_memreader_size_read(const bmcl_memreader_t* self);

bool bmcl_memreader_is_empty(const bmcl_memreader_t* self);

void bmcl_memreader_skip(bmcl_memreader_t* self, size_t size);

void bmcl_memreader_peek(const bmcl_memreader_t* self, void* dest, size_t size, size_t offset);

void bmcl_memreader_read(bmcl_memreader_t* self, void* dest, size_t size);

uint8_t bmcl_memreader_read_uint8(bmcl_memreader_t* self);

uint16_t bmcl_memreader_read_uint16le(bmcl_memreader_t* self);

uint32_t bmcl_memreader_read_uint32le(bmcl_memreader_t* self);

uint64_t bmcl_memreader_read_uint64le(bmcl_memreader_t* self);

uint16_t bmcl_memreader_read_uint16be(bmcl_memreader_t* self);

uint32_t bmcl_memreader_read_uint32be(bmcl_memreader_t* self);

uint64_t bmcl_memreader_read_uint64be(bmcl_memreader_t* self);

#ifdef __cplusplus
}
#endif
