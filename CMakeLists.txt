project(bmcl)

cmake_minimum_required(VERSION 2.8.7)

enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(SANITIZE_MEMORY)
    add_definitions(-fPIC -fno-omit-frame-pointer)
elseif(SANITIZE_THREAD)
    add_definitions(-fPIC)
endif()

add_subdirectory(thirdparty)

add_definitions(-Wall -Wextra -O0 -ggdb)

macro(enable_sanitizer name)
    add_definitions(
        -fsanitize=${name}
        -fsanitize=integer
        -fsanitize=undefined
        #-fsanitize-blacklist=${CMAKE_CURRENT_SOURCE_DIR}/cmake/sanitizer-blacklist.txt
    )
    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=${name} -fsanitize=integer -fsanitize=undefined")
endmacro()

if(SANITIZE_ADDRESS)
    enable_sanitizer(address)
elseif(SANITIZE_MEMORY) # instrumented libc++ required
    enable_sanitizer(memory)
    add_definitions(-fsanitize-memory-track-origins=2)
elseif(SANITIZE_THREAD)
    enable_sanitizer(thread)
endif()

set(BMCL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(GTEST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/gtest/include)

add_subdirectory(src)
add_subdirectory(tests)

