set(BENCH_DIR ${CMAKE_BINARY_DIR}/bin/bench)
file(MAKE_DIRECTORY ${BENCH_DIR})

macro(add_bench target)
    add_executable(${target} ${ARGN})
    target_link_libraries(${target} benchmark bmcl)

    set_target_properties(${target}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${BENCH_DIR}
        FOLDER "tests"
    )
endmacro()

add_bench(sha3_bench Sha3.cpp)
