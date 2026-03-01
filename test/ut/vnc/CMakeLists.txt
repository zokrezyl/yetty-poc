#=============================================================================
# VNC Unit Tests
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

add_executable(vnc_tests
    main.cpp
    protocol_test.cpp
    input_injection_test.cpp
    server_input_test.cpp
    socket_integration_test.cpp
)

target_include_directories(vnc_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(vnc_tests PRIVATE
    ut
    ytrace::ytrace
)

# Register with CTest
add_test(NAME vnc_tests COMMAND vnc_tests)
