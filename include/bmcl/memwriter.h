#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    uint8_t* start;
    uint8_t* current;
    const uint8_t* end;
} memwriter_t;

void memwriter_init(memwriter_t* self, void* dest, size_t max_size);

const void* memwriter_ptr(const memwriter_t* self);

const void* memwriter_current_ptr(const memwriter_t* self);

size_t memwriter_size(const memwriter_t* self); /* rename -> size_used */

bool memwriter_is_full(const memwriter_t* self);

size_t memwriter_max_size(const memwriter_t* self);

size_t memwriter_size_left(const memwriter_t* self);

void memwriter_append(memwriter_t* self, const void* data, size_t size);

void memwriter_fill(memwriter_t* self, uint8_t byte, size_t size);

void memwriter_fillup(memwriter_t* self, uint8_t byte);

void memwriter_append_uint8(memwriter_t* self, uint8_t value);

void memwriter_append_uint16(memwriter_t* self, uint16_t value);

void memwriter_append_uint16be(memwriter_t* self, uint16_t value);

void memwriter_append_uint32be(memwriter_t* self, uint32_t value);

void memwriter_append_uint64be(memwriter_t* self, uint64_t value);
