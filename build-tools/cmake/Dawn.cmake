#=============================================================================
# Dawn.cmake - Automatic download and setup of Dawn WebGPU
#=============================================================================
#
# This module downloads pre-built Dawn binaries from GitHub releases
# and sets up the webgpu target for linking.
#
# Supported platforms:
#   - Linux x86_64 (ubuntu-latest)
#   - macOS x86_64/aarch64 (macos-latest, macos-15-intel)
#   - Windows x86_64 (windows-latest)
#
# For Android, use the separate build-tools/android/build-dawn.sh script.
#
# Dawn release URL: https://github.com/google/dawn/releases
#
# Note: Dawn provides STATIC libraries (.a/.lib), not shared libraries.
#
#=============================================================================

include(FetchContent)

# Dawn release version (date-based versioning)
set(DAWN_VERSION "20260117.152313" CACHE STRING "Dawn version to use")
set(DAWN_COMMIT "d14ae3d97ad74100e9f382efef5e9c0872ddbeb2" CACHE STRING "Dawn commit hash")

# Determine platform and download URL
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64")
        set(DAWN_PLATFORM "ubuntu-latest")
        set(DAWN_LIB_DIR_NAME "lib64")
    else()
        message(FATAL_ERROR "Unsupported Linux architecture for Dawn: ${CMAKE_SYSTEM_PROCESSOR}\n"
            "Dawn pre-built binaries are only available for x86_64 on Linux.")
    endif()
    set(DAWN_LIB_NAME "libwebgpu_dawn.a")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64|ARM64")
        set(DAWN_PLATFORM "macos-latest")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64")
        set(DAWN_PLATFORM "macos-15-intel")
    else()
        message(FATAL_ERROR "Unsupported macOS architecture for Dawn: ${CMAKE_SYSTEM_PROCESSOR}")
    endif()
    set(DAWN_LIB_DIR_NAME "lib")
    set(DAWN_LIB_NAME "libwebgpu_dawn.a")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64")
        set(DAWN_PLATFORM "windows-latest")
    else()
        message(FATAL_ERROR "Unsupported Windows architecture for Dawn: ${CMAKE_SYSTEM_PROCESSOR}")
    endif()
    set(DAWN_LIB_DIR_NAME "lib")
    set(DAWN_LIB_NAME "webgpu_dawn.lib")
else()
    message(FATAL_ERROR "Unsupported platform for Dawn: ${CMAKE_SYSTEM_NAME}")
endif()

# Build type for Dawn (use Release for both Debug and Release builds - Dawn debug is huge)
set(DAWN_BUILD_TYPE "Release")

set(DAWN_URL "https://github.com/google/dawn/releases/download/v${DAWN_VERSION}/Dawn-${DAWN_COMMIT}-${DAWN_PLATFORM}-${DAWN_BUILD_TYPE}.tar.gz")
set(DAWN_DOWNLOAD_DIR "${CMAKE_BINARY_DIR}/_deps/dawn")

# Extracted directory name
set(DAWN_EXTRACTED_DIR "Dawn-${DAWN_COMMIT}-${DAWN_PLATFORM}-${DAWN_BUILD_TYPE}")

# Download and extract if not already done
if(NOT EXISTS "${DAWN_DOWNLOAD_DIR}/${DAWN_EXTRACTED_DIR}/${DAWN_LIB_DIR_NAME}/${DAWN_LIB_NAME}")
    message(STATUS "Downloading Dawn v${DAWN_VERSION} for ${DAWN_PLATFORM}...")
    message(STATUS "  URL: ${DAWN_URL}")

    # Download the library tarball
    set(DAWN_TAR "${CMAKE_BINARY_DIR}/dawn.tar.gz")
    file(DOWNLOAD "${DAWN_URL}" "${DAWN_TAR}"
        STATUS DOWNLOAD_STATUS
        SHOW_PROGRESS
    )
    list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
    list(GET DOWNLOAD_STATUS 1 STATUS_STRING)

    if(NOT STATUS_CODE EQUAL 0)
        message(FATAL_ERROR "Failed to download Dawn: ${STATUS_STRING}\n"
            "URL: ${DAWN_URL}\n"
            "You may need to download manually or check your internet connection.")
    endif()

    # Extract library
    message(STATUS "Extracting Dawn...")
    file(MAKE_DIRECTORY "${DAWN_DOWNLOAD_DIR}")
    file(ARCHIVE_EXTRACT INPUT "${DAWN_TAR}" DESTINATION "${DAWN_DOWNLOAD_DIR}")

    # Clean up tarball
    file(REMOVE "${DAWN_TAR}")

    message(STATUS "Dawn extracted to ${DAWN_DOWNLOAD_DIR}")
endif()

# Set paths - Dawn tarballs extract with subdirectory structure
# Structure: Dawn-{commit}-{platform}-{buildtype}/
#   - lib64/ (Linux) or lib/ (macOS/Windows): libwebgpu_dawn.a
#   - include/webgpu/webgpu.h
set(DAWN_INCLUDE_DIR "${DAWN_DOWNLOAD_DIR}/${DAWN_EXTRACTED_DIR}/include")
set(DAWN_LIB_DIR "${DAWN_DOWNLOAD_DIR}/${DAWN_EXTRACTED_DIR}/${DAWN_LIB_DIR_NAME}")
set(DAWN_LIB_PATH "${DAWN_LIB_DIR}/${DAWN_LIB_NAME}")

# Verify files exist
if(NOT EXISTS "${DAWN_INCLUDE_DIR}/webgpu/webgpu.h")
    message(FATAL_ERROR "Dawn headers not found at ${DAWN_INCLUDE_DIR}\n"
        "Expected: ${DAWN_INCLUDE_DIR}/webgpu/webgpu.h")
endif()

if(NOT EXISTS "${DAWN_LIB_PATH}")
    message(FATAL_ERROR "Dawn library not found at ${DAWN_LIB_PATH}")
endif()

# Create imported STATIC library target
add_library(webgpu STATIC IMPORTED GLOBAL)
set_target_properties(webgpu PROPERTIES
    IMPORTED_LOCATION "${DAWN_LIB_PATH}"
    INTERFACE_INCLUDE_DIRECTORIES "${DAWN_INCLUDE_DIR}"
)

# Dawn requires additional system libraries
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Dawn on Linux needs X11 and other system libs
    find_package(X11 REQUIRED)
    target_link_libraries(webgpu INTERFACE ${X11_LIBRARIES})

    # Also need dl and pthread
    target_link_libraries(webgpu INTERFACE ${CMAKE_DL_LIBS} pthread)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # Dawn on macOS needs Cocoa, Metal, QuartzCore
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(METAL_LIBRARY Metal REQUIRED)
    find_library(QUARTZCORE_LIBRARY QuartzCore REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(IOSURFACE_LIBRARY IOSurface REQUIRED)
    target_link_libraries(webgpu INTERFACE
        ${COCOA_LIBRARY}
        ${METAL_LIBRARY}
        ${QUARTZCORE_LIBRARY}
        ${IOKIT_LIBRARY}
        ${IOSURFACE_LIBRARY}
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Dawn on Windows needs various Windows libs
    target_link_libraries(webgpu INTERFACE
        d3d12 dxgi dxguid d3dcompiler
        user32 gdi32 ole32 shell32
    )
endif()

target_compile_definitions(webgpu INTERFACE WEBGPU_BACKEND_DAWN)

# Export variables for use elsewhere
set(DAWN_NATIVE_INCLUDE_DIR "${DAWN_INCLUDE_DIR}" CACHE INTERNAL "")
set(DAWN_NATIVE_LIB_DIR "${DAWN_LIB_DIR}" CACHE INTERNAL "")
set(DAWN_NATIVE_LIB_PATH "${DAWN_LIB_PATH}" CACHE INTERNAL "")

# Mark as available for CPM compatibility
set(CPM_PACKAGES "${CPM_PACKAGES};webgpu" CACHE INTERNAL "")
set(webgpu_SOURCE_DIR "${DAWN_INCLUDE_DIR}" CACHE INTERNAL "")
set(webgpu_VERSION "${DAWN_VERSION}" CACHE INTERNAL "")

message(STATUS "Dawn v${DAWN_VERSION} ready:")
message(STATUS "  Library: ${DAWN_LIB_PATH}")
message(STATUS "  Headers: ${DAWN_INCLUDE_DIR}")
