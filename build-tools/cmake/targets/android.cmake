# Android build target

include(${YETTY_ROOT}/build-tools/cmake/targets/shared.cmake)

# libjpeg-turbo for VNC compression
include(${YETTY_ROOT}/build-tools/cmake/libs/libjpeg-turbo.cmake)

# native_app_glue from Android NDK
add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)
target_include_directories(native_app_glue PUBLIC
    ${ANDROID_NDK}/sources/android/native_app_glue
)

# Toybox configuration
if(TOYBOX_PATH)
    add_compile_definitions(YETTY_TOYBOX_PATH="${TOYBOX_PATH}")
endif()

# Set ANDROID_ASSETS_DIR BEFORE adding yetty subdirectory so shaders/CMakeLists.txt can use it
set(ANDROID_ASSETS_DIR "${ANDROID_BUILD_DIR}/assets")
file(MAKE_DIRECTORY ${ANDROID_ASSETS_DIR})

# Add src/yetty (builds libraries)
add_subdirectory(${YETTY_ROOT}/src/yetty ${CMAKE_BINARY_DIR}/src/yetty)

# VNC server/client support
add_subdirectory(${YETTY_ROOT}/src/yetty/vnc ${CMAKE_BINARY_DIR}/src/yetty/vnc)

# Platform manager sources (new architecture)
# Note: event-loop is included via yetty_base
set(YETTY_PLATFORM_SOURCES
    ${YETTY_ROOT}/src/yetty/platform/init-manager/android.cpp
    ${YETTY_ROOT}/src/yetty/platform/shared/android-app-singleton.cpp
    ${YETTY_ROOT}/src/yetty/platform/surface-manager/android.cpp
    ${YETTY_ROOT}/src/yetty/platform/pty-manager/android.cpp
    ${YETTY_ROOT}/src/yetty/platform/fs-path-manager/android.cpp
    ${YETTY_ROOT}/src/yetty/platform/clipboard-manager/android.cpp
    ${YETTY_ROOT}/src/yetty/platform/webgpu-manager/android.cpp
)

# Create shared library with core sources + android platform
add_library(yetty SHARED
    ${YETTY_CORE_SOURCES}
    ${YETTY_ANDROID_SOURCES}
    ${YETTY_PLATFORM_SOURCES}
    ${YETTY_ROOT}/src/yetty/msdf-gen/generator.cpp
)

target_include_directories(yetty PRIVATE ${YETTY_INCLUDES} ${YETTY_RENDERER_INCLUDES} ${JPEG_INCLUDE_DIRS} ${BROTLI_INCLUDE_DIR})

# Embed all assets (logo, shaders, fonts, CDB files)
yetty_embed_assets(yetty)

# Dummy targets for dependency tracking (legacy)
add_custom_target(copy-shaders-for-incbin)
add_custom_target(copy-fonts-for-incbin)

target_compile_definitions(yetty PRIVATE
    ${YETTY_DEFINITIONS}
    YETTY_WEB=0
    YETTY_ANDROID=1
    YETTY_USE_PREBUILT_ATLAS=1
    YETTY_ASSETS_FROM_APK=1
    YETTY_HAS_VNC=1
)

set_target_properties(yetty PROPERTIES LIBRARY_OUTPUT_NAME "yetty")

target_link_libraries(yetty PRIVATE
    ${YETTY_LIBS}
    -Wl,--whole-archive
    native_app_glue
    -Wl,--no-whole-archive
    ytrace::ytrace
    lz4_static
    uv_a
    yetty_vnc
    turbojpeg-static
    ${FREETYPE_ALL_LIBS}
    ${BROTLIDEC_LIBRARIES}
    android
    log
)

# CDB font generation (builds host tools for cross-compilation)
include(${YETTY_ROOT}/build-tools/cmake/cdb-gen.cmake)

# Generate demo outputs (pre-run demo scripts to capture output for Android)
# This runs at configure time since Android can't run bash scripts
message(STATUS "Generating demo outputs for Android...")
execute_process(
    COMMAND ${CMAKE_COMMAND}
        -DYETTY_ROOT=${YETTY_ROOT}
        -DOUTPUT_DIR=${ANDROID_ASSETS_DIR}
        -P ${YETTY_ROOT}/build-tools/cmake/generate-demo-outputs.cmake
    WORKING_DIRECTORY ${YETTY_ROOT}
)

# Copy static assets to Android build directory (ANDROID_ASSETS_DIR already set above)
file(GLOB ASSET_FILES "${YETTY_ROOT}/assets/*")
file(COPY ${ASSET_FILES} DESTINATION ${ANDROID_ASSETS_DIR})

# Ensure CDB, shaders, and assets are built before yetty
add_dependencies(yetty generate-cdb copy-shaders copy-shaders-for-incbin copy-fonts-for-incbin)

# Copy generated CDB fonts to Android assets dir after build
# (shaders are already copied at configure time in shaders/CMakeLists.txt)
add_custom_command(TARGET yetty POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/assets/msdf-fonts ${ANDROID_ASSETS_DIR}/msdf-fonts
    COMMENT "Copying CDB fonts to Android assets"
)

# Verify all required assets are present
add_custom_command(TARGET yetty POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DBUILD_DIR=${ANDROID_ASSETS_DIR}/.. -DTARGET_TYPE=android -P ${YETTY_ROOT}/build-tools/cmake/verify-assets.cmake
    COMMENT "Verifying build assets..."
)
