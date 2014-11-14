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

macro(add_unit_test test file)
    add_executable(${test} ${file})
    target_link_libraries(${test}
        ${ARGN}
        bmcl-core
        gtest
        gtest_main
    )

    set_target_properties(${test}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TESTS_DIR}
        FOLDER "tests"
    )
    add_test(${test} ${TESTS_DIR}/${test})
    list(APPEND ALL_TESTS ${file})
endmacro()

add_unit_test(environment_tests environment.cpp)
add_unit_test(memreader_tests memreader.cpp)
add_unit_test(memwriter_tests memwriter.cpp)
# add_unit_test(queue queue.cpp)
add_unit_test(reader_tests reader.cpp)
add_unit_test(ringarray_tests ringarray.cpp)
add_unit_test(ringbucket_tests ringbucket.cpp)
add_unit_test(ringbuf_tests ringbuf.cpp)
add_unit_test(writer_tests writer.cpp)
#
add_unit_test(interp_tests interp.cpp bmcl-interp)

add_executable(all_tests EXCLUDE_FROM_ALL ${ALL_TESTS})
target_link_libraries(all_tests
    bmcl-core
    bmcl-interp
#    bmcl-telem
    gtest
    gtest_main
)

set_target_properties(all_tests
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TESTS_DIR}
)
