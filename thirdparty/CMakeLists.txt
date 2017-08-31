set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "")
add_subdirectory(benchmark EXCLUDE_FROM_ALL)
add_subdirectory(gtest EXCLUDE_FROM_ALL)
set_target_properties(gtest PROPERTIES FOLDER "thirdparty")
set_target_properties(gtest_main PROPERTIES FOLDER "thirdparty")

