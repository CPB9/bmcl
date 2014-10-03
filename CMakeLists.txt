project(bort)

cmake_minimum_required(VERSION 2.8.12)

enable_testing()

add_subdirectory(thirdparty)

add_definitions(-Wall -Wextra -O0 -ggdb)

add_subdirectory(src)

set(BORT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(GTEST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/gtest/include)
add_subdirectory(tests)

