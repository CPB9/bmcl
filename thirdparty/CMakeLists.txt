set(BENCHMARK_ENABLE_TESTING 0)
add_subdirectory(benchmark)
add_subdirectory(gtest)
set_target_properties(gtest PROPERTIES FOLDER "thirdparty")
set_target_properties(gtest_main PROPERTIES FOLDER "thirdparty")

