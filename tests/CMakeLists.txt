if(NOT MSVC)
    add_definitions(-std=c++11)
endif()

include_directories(
    ${BMCL_INCLUDE_DIRS}
    SYSTEM
    ${GTEST_INCLUDE_DIRS}
)

set(TESTS_DIR ${CMAKE_BINARY_DIR}/bin/tests)
file(MAKE_DIRECTORY ${TESTS_DIR})

set(ALL_TESTS "")

macro(add_unit_test test)
    add_executable(${test} ${ARGN})
    target_link_libraries(${test}
        bmclcore
        gtest
        gtest_main
    )

    set_target_properties(${test}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TESTS_DIR}
        FOLDER "tests"
    )
    add_test(${test} ${TESTS_DIR}/${test})
    list(APPEND ALL_TESTS ${test})
endmacro()

add_unit_test(environment environment.cpp)
add_unit_test(ringbuf ringbuf.cpp)
add_unit_test(writer writer.cpp)
add_unit_test(memwriter memwriter.cpp)
add_unit_test(memreader memreader.cpp)
add_unit_test(sched sched.cpp)


add_executable(all_tests EXCLUDE_FROM_ALL ${ALL_TESTS})
target_link_libraries(all_tests
    bmclcore
    gtest
    gtest_main
)

set_target_properties(all_tests
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TESTS_DIR}
)
