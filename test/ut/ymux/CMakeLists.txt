#=============================================================================
# YMux Unit Tests
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

add_executable(ymux_tests
    main.cpp
    ymux_test.cpp
    ${CMAKE_SOURCE_DIR}/tools/ymux/pane.cpp
    ${CMAKE_SOURCE_DIR}/tools/ymux/ymux.cpp
)

target_include_directories(ymux_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/tools/ymux
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(ymux_tests PRIVATE
    ut
    vterm
)

# Register with CTest
add_test(NAME ymux_tests COMMAND ymux_tests)
