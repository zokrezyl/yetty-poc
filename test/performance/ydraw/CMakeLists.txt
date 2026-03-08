#=============================================================================
# YDraw Performance Tests
#=============================================================================

find_package(PkgConfig REQUIRED)
pkg_check_modules(FONTCONFIG REQUIRED fontconfig)

add_executable(scrolling_perf_test
    scrolling_perf_test.cpp
    # YDraw core
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw/ydraw-buffer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw/ydraw-builder.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw/triangulate.cpp
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
    # Font system
    ${CMAKE_SOURCE_DIR}/src/yetty/vector-sdf-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/vector-coverage-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/raster-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/msdf-gen/generator.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font/freetype.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font/raw-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font/raw-font-manager.cpp
)

target_include_directories(scrolling_perf_test PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(scrolling_perf_test PRIVATE
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
