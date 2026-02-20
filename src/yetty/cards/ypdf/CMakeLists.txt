# YPdf Card - vector PDF rendering via pdfio (lightweight C library)

CPMAddPackage(
    NAME pdfio
    GITHUB_REPOSITORY michaelrsweet/pdfio
    GIT_TAG v1.4.0
    DOWNLOAD_ONLY YES
)

if(pdfio_ADDED)
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
    target_include_directories(pdfio_lib
        PUBLIC ${pdfio_SOURCE_DIR}
        PRIVATE ${ZLIB_INCLUDE_DIR}
    )
    target_link_libraries(pdfio_lib PRIVATE ZLIB::ZLIB)
    set_target_properties(pdfio_lib PROPERTIES
        C_STANDARD 99
        POSITION_INDEPENDENT_CODE ON
    )
    if(MSVC)
        target_compile_options(pdfio_lib PRIVATE /w)
    else()
        target_compile_options(pdfio_lib PRIVATE -w)
    endif()
endif()

add_subdirectory(${YETTY_ROOT}/src/yetty/ypdf ypdf)

add_library(yetty_card_ypdf STATIC ypdf.cpp)
target_include_directories(yetty_card_ypdf PUBLIC ${YETTY_ROOT}/include ${YETTY_ROOT}/src)
target_link_libraries(yetty_card_ypdf PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace yetty_ypdf pdfio_lib)
target_compile_definitions(yetty_card_ypdf PUBLIC YETTY_CARD_YPDF=1)
