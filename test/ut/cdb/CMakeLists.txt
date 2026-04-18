#=============================================================================
# CDB Unit Tests
#
# Tests for the CDB wrapper - tests both howerj/cdb and djb/cdb implementations
# depending on platform and configuration.
#=============================================================================

# Boost UT - Modern C++20 unit testing framework
if(POLICY CMP0175)
    cmake_policy(SET CMP0175 OLD)
endif()
CPMAddPackage(
    NAME ut
    GITHUB_REPOSITORY boost-ext/ut
    GIT_TAG v2.1.0
    OPTIONS "BUILD_BENCHMARKS OFF" "BUILD_EXAMPLES OFF"
)

#-----------------------------------------------------------------------------
# Test using platform-default CDB implementation
#-----------------------------------------------------------------------------
add_executable(cdb_tests
    main.cpp
    howerj_cdb_test.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/cdb-wrapper.cpp
)

target_include_directories(cdb_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(cdb_tests PRIVATE
    ut
    cdb
    ytrace::ytrace
)

# Propagate the CDB implementation define
if(YETTY_USE_HOWERJ_CDB)
    target_compile_definitions(cdb_tests PRIVATE YETTY_USE_HOWERJ_CDB=1)
endif()

# Register with CTest
add_test(NAME cdb_tests COMMAND cdb_tests)

#-----------------------------------------------------------------------------
# Test specifically the howerj/cdb implementation (portable)
# This is critical for Emscripten compatibility testing
#-----------------------------------------------------------------------------
# Create a separate howerj/cdb library for testing
CPMAddPackage(
    NAME howerj_cdb_test
    URL https://github.com/howerj/cdb/archive/refs/tags/v4.3.2.tar.gz
    VERSION 4.3.2
    DOWNLOAD_ONLY YES
)

if(howerj_cdb_test_ADDED)
    add_library(cdb_howerj_test STATIC ${howerj_cdb_test_SOURCE_DIR}/cdb.c)
    target_include_directories(cdb_howerj_test PUBLIC ${howerj_cdb_test_SOURCE_DIR})
    target_compile_definitions(cdb_howerj_test PUBLIC YETTY_USE_HOWERJ_CDB=1)
    if(MSVC)
        target_compile_options(cdb_howerj_test PRIVATE /w)
    else()
        target_compile_options(cdb_howerj_test PRIVATE -w)
    endif()

    add_executable(cdb_howerj_tests
        main.cpp
        howerj_cdb_test.cpp
        ${CMAKE_SOURCE_DIR}/src/yetty/cdb-wrapper.cpp
    )

    target_include_directories(cdb_howerj_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_compile_definitions(cdb_howerj_tests PRIVATE YETTY_USE_HOWERJ_CDB=1)

    target_link_libraries(cdb_howerj_tests PRIVATE
        ut
        cdb_howerj_test
        ytrace::ytrace
    )

    add_test(NAME cdb_howerj_tests COMMAND cdb_howerj_tests)
endif()
