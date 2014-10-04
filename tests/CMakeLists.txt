add_definitions(-std=c++11)

include_directories(
    ${BMCL_INCLUDE_DIRS}
    SYSTEM
    ${GTEST_INCLUDE_DIRS}
)

set(TESTS_DIR ${CMAKE_BINARY_DIR}/bin/tests)
file(MAKE_DIRECTORY ${TESTS_DIR})

macro(add_unit_test test)
    add_executable(${test} ${ARGN})
    target_link_libraries(${test}
        bmcl
        gtest
        gtest_main
    )

    set_target_properties(${test}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TESTS_DIR}
    )
    add_test(${test} ${TESTS_DIR}/${test})
endmacro()

add_unit_test(ringbuf ringbuf.cpp)