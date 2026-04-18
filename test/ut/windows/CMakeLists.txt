#=============================================================================
# Windows Platform Unit Tests (ConPTY, etc.)
#=============================================================================

if(NOT WIN32)
    return()
endif()

if(POLICY CMP0175)
    cmake_policy(SET CMP0175 OLD)
endif()
CPMAddPackage(
    NAME ut
    GITHUB_REPOSITORY boost-ext/ut
    GIT_TAG v2.1.0
    OPTIONS "BUILD_BENCHMARKS OFF" "BUILD_EXAMPLES OFF"
)

add_executable(windows_conpty_tests
    main.cpp
    conpty_test.cpp
)

target_include_directories(windows_conpty_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Disable boost.ut C++20 module export (conflicts with MSVC /std:c++latest)
target_compile_definitions(windows_conpty_tests PRIVATE
    BOOST_UT_DISABLE_MODULE
)

target_link_libraries(windows_conpty_tests PRIVATE
    ut
)

# Register with CTest
add_test(NAME windows_conpty_tests COMMAND windows_conpty_tests)
