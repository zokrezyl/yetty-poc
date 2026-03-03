#=============================================================================
# ThorVG2 Unit Tests - RenderMethod-based ThorVG integration
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

add_executable(thorvg2_tests
    main.cpp
    thorvg2_test.cpp
)

target_include_directories(thorvg2_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${thorvg_SOURCE_DIR}/src/renderer
    ${thorvg_SOURCE_DIR}/src/common
)

target_link_libraries(thorvg2_tests PRIVATE
    ut
    yetty_thorvg2
    yetty_ydraw
    thorvg_lib
    ytrace::ytrace
)

add_test(NAME thorvg2_tests COMMAND thorvg2_tests)
