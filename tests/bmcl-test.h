#pragma once

#include "gtest/gtest.h"

static inline void print_mem(const void* mem, std::size_t size)
{
    for (std::size_t i = 0; i < size; i++) {
        std::cout << std::hex << (std::size_t) * ((uint8_t*)mem + i);
    }
}

static inline void check_eq_mem(const uint8_t* expected, const uint8_t* actual, size_t size, const char* fname,
                                std::size_t line)
{
    if (memcmp(expected, actual, size) != 0) {
        ADD_FAILURE_AT(fname, line);
        std::cout << "Expected memory: ";
        print_mem(expected, size);
        std::cout << std::endl << "Actual memory:   ";
        print_mem(actual, size);
        std::cout << std::endl;
    }
}

#define EXPECT_EQ_MEM(expected, actual, size)                                                                          \
    check_eq_mem((const uint8_t*)expected, (const uint8_t*)actual, size, __FILE__, __LINE__)
