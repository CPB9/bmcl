project(bmcl)

cmake_minimum_required(VERSION 2.8.7)

enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(BMCL_HAVE_QT5 0)
find_package(Qt5Core)
if (Qt5Core_FOUND)
    set(BMCL_HAVE_QT 1)
endif()

set(BMCL_HAVE_MALLOC 1)
if(BMCL_NO_MALLOC)
    set(BMCL_HAVE_MALLOC 0)
endif()

include(TestBigEndian)

test_big_endian(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
    set(BMCL_BIG_ENDIAN 1)
else()
    set(BMCL_LITTLE_ENDIAN 1)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/bmcl/Config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)

get_directory_property(HAS_PARENT_SCOPE PARENT_DIRECTORY)
if(HAS_PARENT_SCOPE)
    set(BMCL_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_BINARY_DIR}/src
        PARENT_SCOPE
    )
endif()

if(SANITIZE_MEMORY)
    add_definitions(-fPIC -fno-omit-frame-pointer)
elseif(SANITIZE_THREAD)
    add_definitions(-fPIC)
endif()

if(NOT HAS_PARENT_SCOPE)
    add_subdirectory(thirdparty)
endif()

if(MSVC)
    foreach (flag_var
             CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
             CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        string(REPLACE "/W3" "/W4" ${flag_var} "${${flag_var}}")
    endforeach()
else()
    add_definitions(-std=c++11)
    add_definitions(-Wall -Wextra)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_definitions(-O0 -ggdb3 -pipe)
    endif()
endif()

macro(enable_sanitizer name)
    add_definitions(
        -fsanitize=${name}
        -fsanitize=integer
        -fsanitize=undefined
        #-fsanitize-blacklist=${CMAKE_CURRENT_SOURCE_DIR}/cmake/sanitizer-blacklist.txt
    )
    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=${name} -fsanitize=integer -fsanitize=undefined")
endmacro()

if(TEST_COVERAGE)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/test-coverage.sh.in ${CMAKE_BINARY_DIR}/test-coverage.sh)
    execute_process(COMMAND chmod +x ${CMAKE_BINARY_DIR}/test-coverage.sh)
    add_definitions(-fprofile-arcs -ftest-coverage)
    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

if(SANITIZE_ADDRESS)
    enable_sanitizer(address)
elseif(SANITIZE_MEMORY) # instrumented libc++ required
    enable_sanitizer(memory)
    add_definitions(-fsanitize-memory-track-origins=2)
elseif(SANITIZE_THREAD)
    enable_sanitizer(thread)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(src/bmcl)

if(NOT HAS_PARENT_SCOPE)
    if(BMCL_HAVE_MALLOC)
        set(GTEST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/gtest/include)
        add_subdirectory(tests)
    endif()
endif()
