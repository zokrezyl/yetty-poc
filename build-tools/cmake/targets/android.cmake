# Android build target

include(${YETTY_ROOT}/build-tools/cmake/targets/shared.cmake)

# Android-specific libraries
include(${YETTY_ROOT}/build-tools/cmake/libs/lz4.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/libuv.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/imgui.cmake)

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

# Add renderer sources to yetty (after target is created)
add_subdirectory(${YETTY_ROOT}/src/yetty/ydraw-zoo ${CMAKE_BINARY_DIR}/src/yetty/ydraw-zoo)
add_subdirectory(${YETTY_ROOT}/src/yetty/ydraw-maze ${CMAKE_BINARY_DIR}/src/yetty/ydraw-maze)
add_subdirectory(${YETTY_ROOT}/src/yetty/cards/ygrid ${CMAKE_BINARY_DIR}/src/yetty/cards/ygrid)

target_include_directories(yetty PRIVATE ${YETTY_INCLUDES})

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
    imgui
    ytrace::ytrace
    lz4_static
    uv_a
    ${FREETYPE_ALL_LIBS}
    android
    log
)

# Copy assets
set(ANDROID_ASSETS_DIR "${ANDROID_BUILD_DIR}/assets")
file(MAKE_DIRECTORY ${ANDROID_ASSETS_DIR})
file(GLOB ASSET_FILES "${YETTY_ROOT}/assets/*")
file(COPY ${ASSET_FILES} DESTINATION ${ANDROID_ASSETS_DIR})
configure_file(${YETTY_ROOT}/src/yetty/shaders/gpu-screen.wgsl ${ANDROID_ASSETS_DIR}/gpu-screen.wgsl COPYONLY)
