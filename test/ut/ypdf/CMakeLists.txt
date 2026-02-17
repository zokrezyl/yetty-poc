#=============================================================================
# YPdf Unit Tests - Font extraction + full rendering pipeline
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

# pdfio sources are already downloaded by the ypdf card.
# Reuse pdfio_lib if it exists, otherwise build our own.
if(NOT TARGET pdfio_lib)
    CPMAddPackage(
        NAME pdfio
        GITHUB_REPOSITORY michaelrsweet/pdfio
        GIT_TAG v1.4.0
        DOWNLOAD_ONLY YES
    )
    add_library(pdfio_lib STATIC
        ${pdfio_SOURCE_DIR}/pdfio-aes.c
        ${pdfio_SOURCE_DIR}/pdfio-array.c
        ${pdfio_SOURCE_DIR}/pdfio-common.c
        ${pdfio_SOURCE_DIR}/pdfio-content.c
        ${pdfio_SOURCE_DIR}/pdfio-crypto.c
        ${pdfio_SOURCE_DIR}/pdfio-dict.c
        ${pdfio_SOURCE_DIR}/pdfio-file.c
        ${pdfio_SOURCE_DIR}/pdfio-md5.c
        ${pdfio_SOURCE_DIR}/pdfio-object.c
        ${pdfio_SOURCE_DIR}/pdfio-page.c
        ${pdfio_SOURCE_DIR}/pdfio-rc4.c
        ${pdfio_SOURCE_DIR}/pdfio-sha256.c
        ${pdfio_SOURCE_DIR}/pdfio-stream.c
        ${pdfio_SOURCE_DIR}/pdfio-string.c
        ${pdfio_SOURCE_DIR}/pdfio-token.c
        ${pdfio_SOURCE_DIR}/pdfio-value.c
    )
    target_include_directories(pdfio_lib PUBLIC ${pdfio_SOURCE_DIR})
    target_link_libraries(pdfio_lib PRIVATE z)
    set_target_properties(pdfio_lib PROPERTIES C_STANDARD 99 POSITION_INDEPENDENT_CODE ON)
    target_compile_options(pdfio_lib PRIVATE -w)
endif()

add_executable(ypdf_tests
    main.cpp
    ypdf_font_test.cpp
    ypdf_render_test.cpp
    # PDF renderer
    ${CMAKE_SOURCE_DIR}/src/yetty/ypdf/pdf-renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ypdf/pdf-content-parser.cpp
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

target_include_directories(ypdf_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/yetty/cards/ypdf
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(ypdf_tests PRIVATE
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(ypdf_tests PRIVATE
    ut
    pdfio_lib
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

add_test(NAME ypdf_tests COMMAND ypdf_tests)
