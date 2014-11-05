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
} bmcl_memwriter_t;

void bmcl_memwriter_init(bmcl_memwriter_t* self, void* dest, size_t max_size);

void bmcl_memwriter_init_writer(bmcl_memwriter_t* self, bmcl_writer_t* writer);

#if BMCL_HAVE_MALLOC

bmcl_memwriter_t* bmcl_memwriter_create(size_t max_size);

bmcl_memwriter_t* bmcl_memwriter_create_with_dest(void* dest, size_t max_size);

void bmcl_memwriter_destroy(bmcl_memwriter_t* self);

bmcl_writer_t* bmcl_memwriter_create_writer(bmcl_memwriter_t* self);

#endif

const void* bmcl_memwriter_ptr(const bmcl_memwriter_t* self);

const void* bmcl_memwriter_current_ptr(const bmcl_memwriter_t* self);

size_t bmcl_memwriter_size(const bmcl_memwriter_t* self); /* rename -> size_used */

bool bmcl_memwriter_is_full(const bmcl_memwriter_t* self);

size_t bmcl_memwriter_max_size(const bmcl_memwriter_t* self);

size_t bmcl_memwriter_size_left(const bmcl_memwriter_t* self);

void bmcl_memwriter_write(bmcl_memwriter_t* self, const void* data, size_t size);

void bmcl_memwriter_fill(bmcl_memwriter_t* self, uint8_t byte, size_t size);

void bmcl_memwriter_fillup(bmcl_memwriter_t* self, uint8_t byte);

void bmcl_memwriter_write_uint8(bmcl_memwriter_t* self, uint8_t value);

void bmcl_memwriter_write_uint16le(bmcl_memwriter_t* self, uint16_t value);

void bmcl_memwriter_write_uint32le(bmcl_memwriter_t* self, uint32_t value);

void bmcl_memwriter_write_uint64le(bmcl_memwriter_t* self, uint64_t value);

void bmcl_memwriter_write_uint16be(bmcl_memwriter_t* self, uint16_t value);

void bmcl_memwriter_write_uint32be(bmcl_memwriter_t* self, uint32_t value);

void bmcl_memwriter_write_uint64be(bmcl_memwriter_t* self, uint64_t value);

#ifdef __cplusplus
}
#endif
