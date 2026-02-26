#=============================================================================
# OSC Scanner Unit Tests
#=============================================================================

if(POLICY CMP0175)
    cmake_policy(SET CMP0175 OLD)
endif()
CPMAddPackage(
    NAME ut
    GITHUB_REPOSITORY boost-ext/ut
    GIT_TAG v2.1.0
    OPTIONS "BUILD_BENCHMARKS OFF" "BUILD_EXAMPLES OFF"
)

add_executable(osc_tests
    main.cpp
    osc_scanner_test.cpp
    osc_integration_test.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/osc-scanner.cpp
)

target_include_directories(osc_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(osc_tests PRIVATE
    ut
)

# Register with CTest
add_test(NAME osc_tests COMMAND osc_tests)
