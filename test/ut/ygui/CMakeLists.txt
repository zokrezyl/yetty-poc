#=============================================================================
# YGui Unit Tests - Widget rendering pipeline (prims + text → GPU)
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

find_package(PkgConfig REQUIRED)
pkg_check_modules(FONTCONFIG REQUIRED fontconfig)

add_executable(ygui_tests
    main.cpp
    ygui_render_test.cpp
    ygui_widget_test.cpp
    # YGui engine
    ${CMAKE_SOURCE_DIR}/src/yetty/ygui/ygui-engine.cpp
    # YDraw core
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw/ydraw-buffer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw/ydraw-builder.cpp
    # Builder transitive deps
    ${CMAKE_SOURCE_DIR}/src/yetty/msdf-atlas.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/msdf-cdb-provider.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ms-msdf-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font-manager.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/bm-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/shader-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/card-manager.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/card-buffer-manager.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/card-texture-manager.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/gpu-allocator.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/shared-memory.cpp
    # Font system (font-manager transitive deps)
    ${CMAKE_SOURCE_DIR}/src/yetty/vector-sdf-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/vector-coverage-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/raster-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/msdf-gen/generator.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font/freetype.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font/util.cpp
)

target_include_directories(ygui_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(ygui_tests PRIVATE
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(ygui_tests PRIVATE
    ut
    webgpu
    cdb-wrapper
    msdf::wgsl
    ${FREETYPE_ALL_LIBS}
    msdfgen::msdfgen-core
    msdfgen::msdfgen-ext
    glm::glm
    ytrace::ytrace
    ${FONTCONFIG_LIBRARIES}
)

add_test(NAME ygui_tests COMMAND ygui_tests)
