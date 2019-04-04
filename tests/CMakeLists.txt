include_directories(
    ${BMCL_INCLUDE_DIRS}
    SYSTEM
    ${GTEST_INCLUDE_DIRS}
)

bmcl_create_tests_directory()

add_definitions(-DDATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

function(add_unit_test target file)
    bmcl_add_unit_test(bmcl-test-${target} ${file})
    target_link_libraries(bmcl-test-${target} bmcl)
    target_include_directories(bmcl-test-${target}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
        ${CMAKE_CURRENT_BINARY_DIR}/../src
    )
endfunction()

add_unit_test(alignedunion AlignedUnion.cpp)
add_unit_test(arrayview ArrayView.cpp)
add_unit_test(buffer Buffer.cpp)
add_unit_test(bitarray BitArray.cpp)
add_unit_test(cstring CString.cpp)
add_unit_test(either Either.cpp)
add_unit_test(environment Environment.cpp)
add_unit_test(logging Logging.cpp)
add_unit_test(memreader MemReader.cpp)
add_unit_test(memwriter MemWriter.cpp)
add_unit_test(mmapopener MmapOpener.cpp)
add_unit_test(option Option.cpp)
add_unit_test(result Result.cpp)
add_unit_test(ringbuf RingBuffer.cpp)
add_unit_test(sha3 Sha3.cpp)
add_unit_test(sharedbytes SharedBytes.cpp)
add_unit_test(string String.cpp)
add_unit_test(stringview StringView.cpp)
add_unit_test(utils Utils.cpp)
add_unit_test(uuid Uuid.cpp)
add_unit_test(variant Variant.cpp)
add_unit_test(misc Misc.cpp)

add_unit_test(colorstream ColorStream.cpp)

#add_unit_test(reader_tests Reader.cpp)
#add_unit_test(writer_tests Writer.cpp)
#add_unit_test(panic_tests Panic.cpp)
