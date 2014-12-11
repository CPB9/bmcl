#pragma once

#include <gtest/gtest.h>

#include <cstddef>
#include <stdint.h>

static inline void print_mem(const void* mem, std::size_t size)
{
    std::ios::fmtflags f(std::cout.flags());
    std::cout << std::hex;
    for (std::size_t i = 0; i < size; i++) {
        std::cout << (std::size_t) * ((uint8_t*)mem + i);
    }
    std::cout.flags(f);
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

template <std::size_t nt, typename T, std::size_t nr, typename R>
static void expect_eq_arrays(const T (&array1)[nt], const R (&array2)[nr], const char* fname, std::size_t line)
{
    std::size_t size1 = sizeof(T) * nt;
    std::size_t size2 = sizeof(R) * nr;
    if (size1 != size2) {
        ADD_FAILURE_AT(fname, line);
        std::cout << "Different array sizes";
        return;
    }
    check_eq_mem((const uint8_t*)array1, (const uint8_t*)array2, size1, fname, line);
}

#define EXPECT_EQ_MEM(expected, actual, size)                                                                          \
    check_eq_mem((const uint8_t*)expected, (const uint8_t*)actual, size, __FILE__, __LINE__)

#define EXPECT_EQ_ARRAYS(expected, actual) expect_eq_arrays(expected, actual, __FILE__, __LINE__)
