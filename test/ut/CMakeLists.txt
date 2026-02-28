#=============================================================================
# Yetty Unit Tests
#=============================================================================

# Coverage option
option(YETTY_COVERAGE "Enable code coverage" OFF)

# Boost UT - Modern C++20 unit testing framework
# Note: v2.3+ requires CMake 4.0, stick with v2.1.0
# Suppress CMP0175 warning from ut's add_custom_command usage
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
# Test library - shared test utilities
#-----------------------------------------------------------------------------
add_library(yetty_test_lib STATIC
    harness/terminal_harness.cpp
    # GPUScreen and dependencies for gpu_screen_test.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/gpu-screen.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/osc-command.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/shader-manager.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font-manager.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ms-msdf-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/bm-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/shader-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/card-manager.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/card-buffer-manager.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/card-texture-manager.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/gpu-allocator.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/shared-memory.cpp
    # ${CMAKE_SOURCE_DIR}/src/yetty/widget-factory.cpp  # TODO: refactor later
    ${CMAKE_SOURCE_DIR}/src/yetty/card-factory.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/base/event-loop.cpp
)

target_include_directories(yetty_test_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

# Fontconfig for bm-font.cpp
find_package(PkgConfig REQUIRED)
pkg_check_modules(FONTCONFIG REQUIRED fontconfig)

target_link_libraries(yetty_test_lib PUBLIC
    vterm
    webgpu
    glfw
    cdb
    ${FREETYPE_ALL_LIBS}
    msdfgen::msdfgen-core
    msdfgen::msdfgen-ext
    glm::glm
    yaml-cpp
    ytrace::ytrace
    uv_a
    ut
    ${FONTCONFIG_LIBRARIES}
)

#-----------------------------------------------------------------------------
# Unit test executable (disabled — needs link fixes for GPUScreenManager etc.)
#-----------------------------------------------------------------------------
# add_executable(yetty_tests
#     main.cpp
#     terminal_scroll_test.cpp
#     terminal_moverect_test.cpp
#     terminal_content_test.cpp
#     terminal_selection_test.cpp
#     terminal_pen_test.cpp
#     gpu_screen_test.cpp
# )
# target_compile_definitions(yetty_tests PRIVATE YETTY_SERVER_BUILD=1)
# target_link_libraries(yetty_tests PRIVATE yetty_test_lib ut ytrace::ytrace)
# add_test(NAME yetty_tests COMMAND yetty_tests)

# Custom target for running tests
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS yetty_tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Add subdirectory tests
add_subdirectory(cdb)
add_subdirectory(yast)
add_subdirectory(yecho)
add_subdirectory(yhtml)
add_subdirectory(ybrowser)
add_subdirectory(ymux)
add_subdirectory(yflame)
add_subdirectory(ygui)
