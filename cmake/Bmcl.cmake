
macro(bmcl_setup_sanitizer_flags)
    if(NOT MSVC)
        if(NOT _BMCL_SANITIZER_FLAGS_SET)
            add_definitions(
                -fno-common
                -fno-omit-frame-pointer
                -fno-optimize-sibling-calls
                -fno-inline
            )
            set(_BMCL_SANITIZER_FLAGS_SET 1)
        endif()
    endif()
endmacro()

macro(bmcl_enable_sanitizer name)
    if(NOT MSVC)
        bmcl_setup_sanitizer_flags()
        add_definitions(-fsanitize=${name})
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=${name}")
    endif()
endmacro()

macro(bmcl_enable_asan)
    bmcl_enable_sanitizer(address)
    if(NOT MSVC)
        add_definitions(-fsanitize-address-use-after-scope)
    endif()
    set(_BMCL_ASAN_ENABLED 1)
endmacro()

macro(bmcl_enable_msan)
    bmcl_enable_sanitizer(memory)
    set(_BMCL_MSAN_ENABLED 1)
endmacro()

if(NOT _BMCL_ASAN_ENABLED)
    if(SANITIZE_ADDRESS)
        bmcl_enable_asan()
    endif()
endif()

if(NOT _BMCL_MSAN_ENABLED)
    if(SANITIZE_MEMORY)
        bmcl_enable_msan()
    endif()
endif()

function(bmcl_setup_compiler_flags target)
    if(NOT MSVC)
        foreach(src ${ARGN})
            get_source_file_property(_LANG ${src} LANGUAGE)
            if(_LANG STREQUAL "CXX")
                set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS -std=c++11)
            endif()
            if(_LANG STREQUAL "C")
                set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS -std=c99)
            endif()
        endforeach()
    endif()
endfunction()

function(bmcl_setup_warning_flags target)
    if(NOT MSVC)
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wno-unused-parameter)
    endif()
endfunction()

function(bmcl_setup_build_type_flags target)
    if(NOT MSVC)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            target_compile_options(${target} PRIVATE -O0 -ggdb3 -pipe)
        endif()
    endif()
endfunction()

function(bmcl_setup_flags target)
    bmcl_setup_warning_flags(${target})
    bmcl_setup_compiler_flags(${target} ${ARGN})
    bmcl_setup_build_type_flags(${target})
endfunction()

function(bmcl_disable_warnings target)
    if(NOT MSVC)
        target_compile_options(${target} PRIVATE -w)
    endif()
endfunction()

function(bmcl_create_tests_directory)
    set(TESTS_DIR ${CMAKE_BINARY_DIR}/bin/tests)
    file(MAKE_DIRECTORY ${TESTS_DIR})
endfunction()

function(bmcl_include_system_private_directories target)
    foreach(dir ${ARGN})
        target_include_directories(${target} SYSTEM PRIVATE ${dir})
    endforeach()
endfunction()

function(bmcl_add_unit_test test file)
    if(NOT GTEST_FOUND)
        message(FATAL_ERROR "Cannot create unit test without gtest found")
    endif()
    set(TESTS_DIR ${CMAKE_BINARY_DIR}/bin/tests)
    add_executable(${test} ${file})
    target_link_libraries(${test}
        gtest
        gtest_main
    )

    set_target_properties(${test}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TESTS_DIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${TESTS_DIR}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${TESTS_DIR}
        FOLDER "tests"
    )
    add_test(${test} ${TESTS_DIR}/${test})
    bmcl_setup_flags(${test} ${file})
endfunction()

function(bmcl_setup_output_directories)
    set_target_properties(${ARGN}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin
        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin
        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib
    )
endfunction()

function(bmcl_install_targets)
    install(TARGETS ${ARGN}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
endfunction()

function(bmcl_add_library target)
    add_library(${target} ${ARGN})
    bmcl_setup_flags(${target} ${ARGN})
    bmcl_install_targets(${target})
    bmcl_setup_output_directories(${target})
    set_target_properties(${target}
        PROPERTIES
        PREFIX "lib"
    )
endfunction()

function(bmcl_add_executable target)
    add_executable(${target} ${ARGN})
    bmcl_setup_flags(${target} ${ARGN})
    bmcl_install_targets(${target})
    bmcl_setup_output_directories(${target})
endfunction()

macro(bmcl_add_dep_bmcl dir)
    if(NOT BMCL_FOUND)
        add_subdirectory(${dir})
        target_include_directories(bmcl SYSTEM INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/src
            ${CMAKE_CURRENT_BINARY_DIR}/${dir}/src
        )
        bmcl_disable_warnings(bmcl)
        set(BMCL_FOUND 1)
    endif()
endmacro()

macro(bmcl_add_dep_tclap dir)
    if(NOT TCLAP_FOUND)
        add_library(tclap INTERFACE)
        target_include_directories(tclap SYSTEM INTERFACE ${dir}/include)
        set(TCLAP_FOUND 1)
    endif()
endmacro()

macro(bmcl_add_dep_gtest dir)
    if(NOT GTEST_FOUND)
        add_subdirectory(${dir} EXCLUDE_FROM_ALL)
        bmcl_disable_warnings(gtest)
        bmcl_disable_warnings(gtest_main)
        set(GTEST_INCLUDE_DIRS ${dir}/include)
        target_include_directories(gtest SYSTEM INTERFACE ${GTEST_INCLUDE_DIRS})
        target_include_directories(gtest_main SYSTEM INTERFACE ${GTEST_INCLUDE_DIRS})
        set(GTEST_FOUND 1)
    endif()
endmacro()

macro(bmcl_add_dep_asio dir)
    if(NOT ASIO_FOUND)
        add_subdirectory(${dir} EXCLUDE_FROM_ALL)
        bmcl_disable_warnings(asio)
        target_include_directories(asio SYSTEM INTERFACE ${dir}/asio/include)
        set(ASIO_FOUND 1)
    endif()
endmacro()

macro(bmcl_add_dep_caf dir)
    if(NOT CAF_FOUND)
        set(CAF_NO_EXAMPLES 1)
        set(CAF_NO_BENCHMARKS 1)
        set(CAF_NO_PYTHON 1)
        set(CAF_NO_OPENCL 1)
        set(CAF_NO_TOOLS 1)
        set(CAF_NO_UNIT_TESTS 1)
        set(CAF_LOG_LEVEL 0)
        add_library(caf-core INTERFACE)
        add_library(caf-io INTERFACE)
        if (NOT MSVC)
            set(_TMP_FLAGS ${CMAKE_CXX_FLAGS})
            set(CMAKE_CXX_FLAGS "")
        endif()
        add_subdirectory(${dir} EXCLUDE_FROM_ALL)
        if (UNIX)
            target_link_libraries(caf-core INTERFACE libcaf_core_shared)
            target_link_libraries(caf-io INTERFACE libcaf_io_shared)
            bmcl_disable_warnings(libcaf_core_shared)
            bmcl_disable_warnings(libcaf_io_shared)
        else()
            set(CAF_BUILD_STATIC_ONLY 1)
            target_link_libraries(caf-core INTERFACE libcaf_core_static)
            target_link_libraries(caf-io INTERFACE libcaf_io_static)
            bmcl_disable_warnings(libcaf_core_static)
            bmcl_disable_warnings(libcaf_io_static)
        endif()
        if (NOT MSVC)
            set(CMAKE_CXX_FLAGS ${_TMP_FLAGS})
        endif()
        target_include_directories(caf-core SYSTEM INTERFACE ${dir}/libcaf_core)
        target_include_directories(caf-io SYSTEM INTERFACE ${dir}/libcaf_io)
        set(CAF_FOUND 1)
    endif()
endmacro()


