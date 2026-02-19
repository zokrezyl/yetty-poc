# PDF Card - requires PDFium prebuilt binaries
# Gated behind YETTY_CARD_PDF option (default OFF, set in parent CMakeLists.txt)

# Determine platform-specific PDFium package URL
if(APPLE)
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        set(PDFIUM_PLATFORM "mac-arm64")
    else()
        set(PDFIUM_PLATFORM "mac-x64")
    endif()
elseif(WIN32)
    set(PDFIUM_PLATFORM "win-x64")
elseif(UNIX)
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
        set(PDFIUM_PLATFORM "linux-arm64")
    else()
        set(PDFIUM_PLATFORM "linux-x64")
    endif()
else()
    message(FATAL_ERROR "Unsupported platform for PDFium")
endif()

set(PDFIUM_VERSION "7651")
set(PDFIUM_URL "https://github.com/bblanchon/pdfium-binaries/releases/download/chromium/${PDFIUM_VERSION}/pdfium-${PDFIUM_PLATFORM}.tgz")

CPMAddPackage(
    NAME pdfium
    URL ${PDFIUM_URL}
    DOWNLOAD_ONLY YES
)

if(pdfium_ADDED)
    include(${YETTY_ROOT}/build-tools/cmake/pdfium/CMakeLists.txt)
endif()

target_sources(yetty PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/pdf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pdf.cpp
)

target_link_libraries(yetty PRIVATE pdfium_lib)
target_compile_definitions(yetty PRIVATE YETTY_CARD_PDF=1)
