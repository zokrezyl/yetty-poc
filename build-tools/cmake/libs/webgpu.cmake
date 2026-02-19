# WebGPU - Graphics API abstraction
# Supports: Dawn, wgpu-native, Emscripten emdawnwebgpu
if(TARGET webgpu)
    return()
endif()

message(STATUS "WebGPU backend: ${WEBGPU_BACKEND}")

if(YETTY_ANDROID)
    # Android: Use pre-built WebGPU library from ANDROID_BUILD_DIR
    if(NOT DEFINED ANDROID_BUILD_DIR)
        if(DEFINED ENV{ANDROID_BUILD_DIR})
            set(ANDROID_BUILD_DIR $ENV{ANDROID_BUILD_DIR})
        else()
            message(FATAL_ERROR "ANDROID_BUILD_DIR not set")
        endif()
    endif()

    if(WEBGPU_BACKEND STREQUAL "dawn")
        set(WEBGPU_LIB "${ANDROID_BUILD_DIR}/dawn-libs/arm64-v8a/libwebgpu_dawn.a")
        set(WEBGPU_INCLUDE "${ANDROID_BUILD_DIR}/dawn-include")
        set(WEBGPU_BACKEND_DEFINE "WEBGPU_BACKEND_DAWN")
        set(WEBGPU_IS_STATIC TRUE)
    else()
        set(WEBGPU_LIB "${ANDROID_BUILD_DIR}/wgpu-libs/arm64-v8a/libwgpu_native.so")
        set(WEBGPU_INCLUDE "${ANDROID_BUILD_DIR}/wgpu-include")
        set(WEBGPU_BACKEND_DEFINE "WEBGPU_BACKEND_WGPU")
        set(WEBGPU_IS_STATIC FALSE)
    endif()

    if(NOT EXISTS "${WEBGPU_LIB}")
        message(FATAL_ERROR "WebGPU library not found at ${WEBGPU_LIB}")
    endif()

    if(WEBGPU_IS_STATIC)
        add_library(webgpu STATIC IMPORTED GLOBAL)
        set_target_properties(webgpu PROPERTIES
            IMPORTED_LOCATION "${WEBGPU_LIB}"
            INTERFACE_INCLUDE_DIRECTORIES "${WEBGPU_INCLUDE}"
        )
    else()
        add_library(webgpu SHARED IMPORTED GLOBAL)
        set_target_properties(webgpu PROPERTIES
            IMPORTED_LOCATION "${WEBGPU_LIB}"
            IMPORTED_NO_SONAME TRUE
            INTERFACE_INCLUDE_DIRECTORIES "${WEBGPU_INCLUDE}"
        )
    endif()
    target_compile_definitions(webgpu INTERFACE ${WEBGPU_BACKEND_DEFINE})

elseif(EMSCRIPTEN)
    # Emscripten: WebGPU via emdawnwebgpu port
    add_library(webgpu INTERFACE)
    target_compile_options(webgpu INTERFACE "SHELL:--use-port=emdawnwebgpu")
    message(STATUS "WebGPU: Using emdawnwebgpu port for Emscripten")

else()
    # Desktop: Auto-download from GitHub releases
    if(WEBGPU_BACKEND STREQUAL "dawn")
        include(${CMAKE_SOURCE_DIR}/build-tools/cmake/Dawn.cmake)
    else()
        include(${CMAKE_SOURCE_DIR}/build-tools/cmake/WgpuNative.cmake)
    endif()
endif()
