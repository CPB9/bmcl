if(ARDUINO_BOARD)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/arduino-cmake/cmake/ArduinoToolchain.cmake)
endif()

project(bmcl)

cmake_minimum_required(VERSION 2.8.7)

enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(NOT ARDUINO_BOARD)
    if(SANITIZE_MEMORY)
        add_definitions(-fPIC -fno-omit-frame-pointer)
    elseif(SANITIZE_THREAD)
        add_definitions(-fPIC)
    endif()

    add_subdirectory(thirdparty)

    if(MSVC)
        foreach (flag_var
                 CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
                 CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
            string(REPLACE "/W3" "/W4" ${flag_var} "${${flag_var}}")
        endforeach()
    else()
        add_definitions(-Wall -Wextra -O0 -g -ggdb -pipe)
        add_definitions(-DBMCL_HAVE_MALLOC=1) # temp
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
endif()

set(BMCL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(src)

if(NOT ARDUINO_BOARD)
    set(GTEST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/gtest/include)
    add_subdirectory(tests)
endif()
