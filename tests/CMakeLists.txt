include_directories(
    ${BMCL_INCLUDE_DIRS}
    SYSTEM
    ${GTEST_INCLUDE_DIRS}
)

add_definitions(-DDATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
set(TESTS_DIR ${CMAKE_BINARY_DIR}/bin/tests)
file(MAKE_DIRECTORY ${TESTS_DIR})

set(ALL_TESTS "")

macro(add_unit_test test file)
    add_executable(${test} ${file})
    target_link_libraries(${test}
        ${ARGN}
        bmcl
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

add_unit_test(environment_tests Environment.cpp)
add_unit_test(buffer_tests Buffer.cpp)
add_unit_test(memreader_tests MemReader.cpp)
add_unit_test(memwriter_tests MemWriter.cpp)
#add_unit_test(reader_tests Reader.cpp)
add_unit_test(ringarray_tests RingArray.cpp)
add_unit_test(ringbucket_tests RingBucket.cpp)
add_unit_test(ringbuf_tests RingBuffer.cpp)
#add_unit_test(writer_tests Writer.cpp)
add_unit_test(option_tests Option.cpp)
add_unit_test(either_tests Either.cpp)
add_unit_test(result_tests Result.cpp)
add_unit_test(panic_tests Panic.cpp)
add_unit_test(logging_tests Logging.cpp)
add_unit_test(utils_tests Utils.cpp)

add_executable(colorstream_test ColorStream.cpp)
target_link_libraries(colorstream_test bmcl)
set_target_properties(colorstream_test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TESTS_DIR}
    FOLDER "tests"
)

add_executable(all_tests EXCLUDE_FROM_ALL ${ALL_TESTS})
target_link_libraries(all_tests
    bmcl
    gtest
    gtest_main
)

set_target_properties(all_tests
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TESTS_DIR}
)
