# Android build target

include(${YETTY_ROOT}/build-tools/cmake/targets/shared.cmake)

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

# Add src/yetty (builds libraries)
add_subdirectory(${YETTY_ROOT}/src/yetty ${CMAKE_BINARY_DIR}/src/yetty)

# Create shared library with core sources + android platform
add_library(yetty SHARED
    ${YETTY_CORE_SOURCES}
    ${YETTY_ANDROID_SOURCES}
    ${YETTY_ROOT}/src/yetty/platform/android-platform.cpp
    ${YETTY_ROOT}/src/yetty/msdf-gen/generator.cpp
)

target_include_directories(yetty PRIVATE ${YETTY_INCLUDES} ${YETTY_RENDERER_INCLUDES})

target_compile_definitions(yetty PRIVATE
    ${YETTY_DEFINITIONS}
    YETTY_WEB=0
    YETTY_ANDROID=1
    YETTY_USE_PREBUILT_ATLAS=1
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
    ${FREETYPE_ALL_LIBS}
    android
    log
)

# CDB font generation (builds host tools for cross-compilation)
include(${YETTY_ROOT}/build-tools/cmake/cdb-gen.cmake)

# Copy assets to Android build directory
set(ANDROID_ASSETS_DIR "${ANDROID_BUILD_DIR}/assets")
file(MAKE_DIRECTORY ${ANDROID_ASSETS_DIR})
file(GLOB ASSET_FILES "${YETTY_ROOT}/assets/*")
file(COPY ${ASSET_FILES} DESTINATION ${ANDROID_ASSETS_DIR})

# Ensure CDB, shaders, and assets are built before yetty
add_dependencies(yetty generate-cdb copy-shaders)

# Copy generated assets (CDB fonts, shaders) to Android assets dir after build
add_custom_command(TARGET yetty POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/assets/fonts-cdb ${ANDROID_ASSETS_DIR}/fonts-cdb
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/assets/shaders ${ANDROID_ASSETS_DIR}/shaders
    COMMENT "Copying CDB fonts and shaders to Android assets"
)

# Verify all required assets are present
add_custom_command(TARGET yetty POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DBUILD_DIR=${ANDROID_ASSETS_DIR}/.. -DTARGET_TYPE=android -P ${YETTY_ROOT}/build-tools/cmake/verify-assets.cmake
    COMMENT "Verifying build assets..."
)
