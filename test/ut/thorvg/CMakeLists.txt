#=============================================================================
# ThorVG Unit Tests - ThorVG to YDraw rendering
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

add_executable(thorvg_tests
    main.cpp
    thorvg_test.cpp
    thorvg_rounded_rect_test.cpp
    thorvg_dash_test.cpp
    thorvg_opacity_test.cpp
    thorvg_gradient_test.cpp
)

target_include_directories(thorvg_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(thorvg_tests PRIVATE
    ut
    yetty_thorvg
    yetty_ydraw
    ytrace::ytrace
)

add_test(NAME thorvg_tests COMMAND thorvg_tests)
