#=============================================================================
# YPaint Unit Tests - Buffer, AABB, GPU layout, builder grid + lifecycle
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

add_executable(ypaint_tests
    main.cpp
    ypaint_test.cpp
    ypaint_bezier_test.cpp
    ypaint_gradient_test.cpp
    ypaint_opacity_test.cpp
    ypaint_scrolling_test.cpp
    ypaint_shapes_demo_test.cpp
    triangulate_test.cpp
    gpu_buffer_verification_test.cpp
    polygon_test.cpp
    # YPaint core
    ${CMAKE_SOURCE_DIR}/src/yetty/ypaint/ypaint-buffer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ypaint/ypaint-builder.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ypaint/triangulate.cpp
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
    ${CMAKE_SOURCE_DIR}/src/yetty/font/raw-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font/raw-font-manager.cpp
)

target_include_directories(ypaint_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(ypaint_tests PRIVATE
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(ypaint_tests PRIVATE
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

add_test(NAME ypaint_tests COMMAND ypaint_tests)

# Debug grid program
add_executable(ypaint_debug_grid
    debug_grid.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ypaint/ypaint-buffer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ypaint/ypaint-builder.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ypaint/triangulate.cpp
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
    ${CMAKE_SOURCE_DIR}/src/yetty/vector-sdf-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/vector-coverage-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/raster-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/msdf-gen/generator.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font/freetype.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font/raw-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font/raw-font-manager.cpp
)

target_include_directories(ypaint_debug_grid PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(ypaint_debug_grid PRIVATE
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
