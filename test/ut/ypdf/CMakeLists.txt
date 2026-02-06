#=============================================================================
# YPdf Unit Tests - PDF font extraction
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
)

target_include_directories(ypdf_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/yetty/cards/ypdf
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(ypdf_tests PRIVATE
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(ypdf_tests PRIVATE ut pdfio_lib)

add_test(NAME ypdf_tests COMMAND ypdf_tests)
