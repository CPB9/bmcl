set(BENCH_DIR ${CMAKE_BINARY_DIR}/bin/bench)
file(MAKE_DIRECTORY ${BENCH_DIR})

macro(add_bench target)
    add_executable(bmcl-bench-${target} ${ARGN})
    target_link_libraries(bmcl-bench-${target} benchmark bmcl)

    set_target_properties(bmcl-bench-${target}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${BENCH_DIR}
        FOLDER "tests"
    )
    bmcl_setup_flags(bmcl-bench-${target})

    target_include_directories(bmcl-bench-${target}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
        ${CMAKE_CURRENT_BINARY_DIR}/../src
    )
endmacro()

add_bench(sha3 Sha3.cpp)
