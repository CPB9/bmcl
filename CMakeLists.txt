project(bmcl)

cmake_minimum_required(VERSION 2.8.7)

if(BMCL_FOUND)
    return()
endif()

set(BMCL_FOUND 1)

enable_testing()

include(cmake/Bmcl.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(BMCL_HAVE_QT5 0)
if(NOT BMCL_NO_QT)
    find_package(Qt5Core)
    if (Qt5Core_FOUND)
        set(BMCL_HAVE_QT 1)
    endif()
endif()

include(TestBigEndian)

test_big_endian(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
    set(BMCL_BIG_ENDIAN 1)
else()
    set(BMCL_LITTLE_ENDIAN 1)
endif()

if(MSVC OR MINGW)
    set(BMCL_DLL 1)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/bmcl/Config.h)

if(TEST_COVERAGE)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/test-coverage.sh.in ${CMAKE_BINARY_DIR}/test-coverage.sh)
    execute_process(COMMAND chmod +x ${CMAKE_BINARY_DIR}/test-coverage.sh)
    add_definitions(-fprofile-arcs -ftest-coverage)
    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

add_subdirectory(src/bmcl)


if(NOT HAS_PARENT_SCOPE)
    bmcl_add_dep_gtest(thirdparty/gtest)
    add_subdirectory(tests)
endif()

