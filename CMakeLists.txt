#=============================================================================
# yetty-poc - WebGPU Terminal Emulator
#=============================================================================
#
# BUILD TARGETS (use Makefile for convenience):
#
#   Desktop (Linux/macOS/Windows):
#     make release    # or: cmake -B build && cmake --build build
#
#   Web (Emscripten):
#     Prerequisites: Emscripten SDK, pre-built atlas (./yetty --generate-atlas)
#     make web        # or: emcmake cmake -B web-build && cmake --build web-build
#
#   Android:
#     Prerequisites: Android NDK, Toybox (auto-downloaded)
#     make android    # downloads Toybox + builds yetty shared library
#
#   Tests:
#     make test       # or: cmake --build build --target yetty_tests && ./build/test/ut/yetty_tests
#
# See Makefile for more targets and options.
#
#=============================================================================

cmake_minimum_required(VERSION 3.20)

# Allow dependencies with older cmake_minimum_required (yaml-cpp, etc.)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "Minimum CMake policy version for dependencies")

project(yetty-poc VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build target options
option(YETTY_ANDROID "Build for Android" OFF)

# WebGPU backend selection: "wgpu" (default) or "dawn"
# - wgpu: wgpu-native (Rust implementation, required for pygfx integration)
# - dawn: Dawn (Google's C++ WebGPU implementation)
set(WEBGPU_BACKEND "wgpu" CACHE STRING "WebGPU backend to use (wgpu or dawn)")
set_property(CACHE WEBGPU_BACKEND PROPERTY STRINGS "wgpu" "dawn")

# Toybox - BSD-licensed shell utilities (used on all platforms)
# Binaries available at https://landley.net/toybox/bin/
set(TOYBOX_PATH "" CACHE PATH "Path to Toybox binary")

# Include CPM.cmake
include(build-tools/cmake/CPM.cmake)

#-----------------------------------------------------------------------------
# Dependencies
#-----------------------------------------------------------------------------

# WebGPU setup - supports both wgpu-native and Dawn backends
message(STATUS "WebGPU backend: ${WEBGPU_BACKEND}")

if(YETTY_ANDROID)
    # Android: Use pre-built WebGPU library
    if(WEBGPU_BACKEND STREQUAL "dawn")
        # Dawn backend for Android
        set(WEBGPU_LIB "${CMAKE_CURRENT_SOURCE_DIR}/build-android/dawn-libs/arm64-v8a/libwebgpu_dawn.so")
        set(WEBGPU_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/build-android/dawn-include")
        set(WEBGPU_BACKEND_DEFINE "WEBGPU_BACKEND_DAWN")
        set(WEBGPU_VERSION "20260117")

        if(NOT EXISTS "${WEBGPU_LIB}")
            message(FATAL_ERROR
                "Dawn Android library not found at ${WEBGPU_LIB}\n"
                "Run 'make build-android-dawn-*' first.")
        endif()

        if(NOT EXISTS "${WEBGPU_INCLUDE}/webgpu/webgpu.h")
            message(FATAL_ERROR
                "Dawn headers not found at ${WEBGPU_INCLUDE}\n"
                "Run 'make build-android-dawn-*' first.")
        endif()

        message(STATUS "Using Dawn (Android):")
    else()
        # wgpu-native backend for Android (default)
        set(WEBGPU_LIB "${CMAKE_CURRENT_SOURCE_DIR}/build-android/wgpu-libs/arm64-v8a/libwgpu_native.so")
        set(WEBGPU_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/build-android/wgpu-include")
        set(WEBGPU_BACKEND_DEFINE "WEBGPU_BACKEND_WGPU")
        set(WEBGPU_VERSION "27.0.4.0")

        if(NOT EXISTS "${WEBGPU_LIB}")
            message(FATAL_ERROR
                "wgpu-native Android library not found at ${WEBGPU_LIB}\n"
                "Run 'make build-android-wgpu-*' first.")
        endif()

        if(NOT EXISTS "${WEBGPU_INCLUDE}/webgpu/webgpu.h")
            message(FATAL_ERROR
                "wgpu-native headers not found at ${WEBGPU_INCLUDE}\n"
                "Run 'make build-android-wgpu-*' first.")
        endif()

        message(STATUS "Using wgpu-native v${WEBGPU_VERSION} (Android):")
    endif()

    # Create imported library target for Android
    add_library(webgpu SHARED IMPORTED GLOBAL)
    set_target_properties(webgpu PROPERTIES
        IMPORTED_LOCATION "${WEBGPU_LIB}"
        IMPORTED_NO_SONAME TRUE
        INTERFACE_INCLUDE_DIRECTORIES "${WEBGPU_INCLUDE}"
    )
    target_compile_definitions(webgpu INTERFACE ${WEBGPU_BACKEND_DEFINE})

    # Mark webgpu as already added so CPM won't try to fetch it again
    set(CPM_PACKAGES "${CPM_PACKAGES};webgpu" CACHE INTERNAL "")
    set(webgpu_SOURCE_DIR "${WEBGPU_INCLUDE}" CACHE INTERNAL "")
    set(webgpu_VERSION "${WEBGPU_VERSION}" CACHE INTERNAL "")

    message(STATUS "  Library: ${WEBGPU_LIB}")
    message(STATUS "  Headers: ${WEBGPU_INCLUDE}")

elseif(NOT EMSCRIPTEN)
    # Desktop (Linux/macOS/Windows): Auto-download from GitHub releases
    if(WEBGPU_BACKEND STREQUAL "dawn")
        include(build-tools/cmake/Dawn.cmake)
    else()
        include(build-tools/cmake/WgpuNative.cmake)
    endif()
endif()

# GLM for math
CPMAddPackage(
    NAME glm
    GITHUB_REPOSITORY g-truc/glm
    GIT_TAG 1.0.1
)

# stb for image loading
CPMAddPackage(
    NAME stb
    GITHUB_REPOSITORY nothings/stb
    GIT_TAG master
    DOWNLOAD_ONLY YES
)

if(stb_ADDED)
    add_library(stb INTERFACE)
    target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})
endif()

# yaml-cpp for config files
CPMAddPackage(
    NAME yaml-cpp
    GITHUB_REPOSITORY jbeder/yaml-cpp
    GIT_TAG 0.8.0
    OPTIONS "YAML_CPP_BUILD_TESTS OFF" "YAML_CPP_BUILD_TOOLS OFF"
)

# Taywee/args for command line parsing (desktop only, not web)
if(NOT YETTY_ANDROID AND NOT EMSCRIPTEN)
    CPMAddPackage(
        NAME args
        GITHUB_REPOSITORY Taywee/args
        GIT_TAG 6.4.6
    )
endif()

# spdlog - add FIRST so ytrace detects it
CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY gabime/spdlog
    GIT_TAG v1.16.0
    OPTIONS "CMAKE_POSITION_INDEPENDENT_CODE ON"
)

# ytrace for logging with runtime control (will use existing spdlog)
CPMAddPackage(
    NAME ytrace
    GITHUB_REPOSITORY zokrezyl/ytrace
    GIT_TAG main
    OPTIONS
        "YTRACE_BUILD_TOOLS ON"
        "YTRACE_BUILD_EXAMPLES OFF"
        "YTRACE_ENABLE_YLOG ON"
        "YTRACE_ENABLE_YTRACE ON"
        "YTRACE_ENABLE_YDEBUG ON"
        "YTRACE_ENABLE_YINFO ON"
        "YTRACE_ENABLE_YWARN ON"
        "YTRACE_ENABLE_YFUNC ON"
)

# msgpack for RPC message serialization (header-only C++ library)
CPMAddPackage(
    NAME msgpack-cxx
    GITHUB_REPOSITORY msgpack/msgpack-c
    GIT_TAG cpp-7.0.0
    OPTIONS
        "MSGPACK_BUILD_DOCS OFF"
        "MSGPACK_BUILD_TESTS OFF"
        "MSGPACK_BUILD_EXAMPLES OFF"
        "MSGPACK_USE_BOOST OFF"
)

# LZ4 for fast font cache compression
CPMAddPackage(
    NAME lz4
    GITHUB_REPOSITORY lz4/lz4
    GIT_TAG v1.10.0
    SOURCE_SUBDIR build/cmake
    OPTIONS
        "LZ4_BUILD_CLI OFF"
        "LZ4_BUILD_LEGACY_LZ4C OFF"
        "BUILD_SHARED_LIBS OFF"
        "BUILD_STATIC_LIBS ON"
)
if(TARGET lz4_static)
    set_target_properties(lz4_static PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

#-----------------------------------------------------------------------------
# CDB (Constant Database) for fast key-value lookups
# Downloads from https://cdb.cr.yp.to (public domain)
#-----------------------------------------------------------------------------
if(NOT EMSCRIPTEN AND NOT YETTY_ANDROID)
    set(CDB_VERSION "20251021")
    set(CDB_URL "https://cdb.cr.yp.to/cdb-${CDB_VERSION}.tar.gz")
    set(CDB_SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/cdb-${CDB_VERSION}")

    if(NOT EXISTS "${CDB_SOURCE_DIR}/cdb.c")
        message(STATUS "Downloading CDB ${CDB_VERSION}...")
        file(DOWNLOAD ${CDB_URL} "${CMAKE_BINARY_DIR}/_deps/cdb-${CDB_VERSION}.tar.gz"
             STATUS CDB_DOWNLOAD_STATUS)
        list(GET CDB_DOWNLOAD_STATUS 0 CDB_DOWNLOAD_ERROR)
        if(CDB_DOWNLOAD_ERROR)
            message(FATAL_ERROR "Failed to download CDB: ${CDB_DOWNLOAD_STATUS}")
        endif()
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar xzf "${CMAKE_BINARY_DIR}/_deps/cdb-${CDB_VERSION}.tar.gz"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/_deps"
        )
    endif()

    # Build CDB as a static library
    add_library(cdb STATIC
        ${CDB_SOURCE_DIR}/cdb.c
        ${CDB_SOURCE_DIR}/cdb_hash.c
        ${CDB_SOURCE_DIR}/cdb_make.c
        ${CDB_SOURCE_DIR}/byte_copy.c
        ${CDB_SOURCE_DIR}/byte_diff.c
        ${CDB_SOURCE_DIR}/num_from4bytes.c
        ${CDB_SOURCE_DIR}/num_to4bytes.c
        ${CDB_SOURCE_DIR}/outbuf_init.c
        ${CDB_SOURCE_DIR}/outbuf_put.c
        ${CDB_SOURCE_DIR}/outbuf_write.c
        ${CDB_SOURCE_DIR}/inbuf_0.c
        ${CDB_SOURCE_DIR}/inbuf_init.c
        ${CDB_SOURCE_DIR}/inbuf_get.c
        ${CDB_SOURCE_DIR}/inbuf_read.c
        ${CDB_SOURCE_DIR}/seek_set.c
        ${CDB_SOURCE_DIR}/seek_cur.c
    )
    # Remove cdb's 'version' file that clashes with C++20 <version> header
    file(REMOVE ${CDB_SOURCE_DIR}/version)
    target_include_directories(cdb SYSTEM PUBLIC ${CDB_SOURCE_DIR})
    target_compile_options(cdb PRIVATE -w)  # Suppress warnings in third-party code
    set_target_properties(cdb PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

#-----------------------------------------------------------------------------
# Desktop-only dependencies (font atlas generation, GLFW windowing)
#-----------------------------------------------------------------------------
if(NOT EMSCRIPTEN AND NOT YETTY_ANDROID)
    # FreeType for font loading
    # NOTE: PNG support is required for color emoji fonts (CBDT/CBLC with PNG-compressed bitmaps)
    CPMAddPackage(
        NAME freetype
        GITHUB_REPOSITORY freetype/freetype
        GIT_TAG VER-2-13-2
        OPTIONS
            "FT_DISABLE_ZLIB OFF"
            "FT_DISABLE_BZIP2 ON"
            "FT_DISABLE_PNG OFF"
            "FT_DISABLE_HARFBUZZ ON"
            "FT_DISABLE_BROTLI ON"
    )
    # Create alias for msdfgen's find_package(Freetype)
    if(NOT TARGET Freetype::Freetype)
        add_library(Freetype::Freetype ALIAS freetype)
    endif()
    # Enable PIC for shared library plugins
    set_target_properties(freetype PROPERTIES POSITION_INDEPENDENT_CODE ON)

    # msdfgen with FreeType support
    CPMAddPackage(
        NAME msdfgen
        GITHUB_REPOSITORY Chlumsky/msdfgen
        GIT_TAG v1.12
        OPTIONS
            "MSDFGEN_BUILD_STANDALONE OFF"
            "MSDFGEN_CORE_ONLY OFF"
            "MSDFGEN_USE_SKIA OFF"
            "MSDFGEN_USE_VCPKG OFF"
            "MSDFGEN_DISABLE_SVG ON"
            "MSDFGEN_DISABLE_PNG ON"
            "MSDFGEN_INSTALL OFF"
    )
    # Enable PIC for shared library plugins
    if(TARGET msdfgen-core)
        set_target_properties(msdfgen-core PROPERTIES POSITION_INDEPENDENT_CODE ON)
    endif()
    if(TARGET msdfgen-ext)
        set_target_properties(msdfgen-ext PROPERTIES POSITION_INDEPENDENT_CODE ON)
    endif()

    # GLFW and glfw3webgpu adapter (desktop only, not Android)
    # Disable X11 on macOS to prevent glfw3webgpu from including X11 headers
    if(APPLE)
        set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE)
    endif()

    CPMAddPackage(
        NAME glfw
        GITHUB_REPOSITORY glfw/glfw
        GIT_TAG 3.4
        OPTIONS
            "GLFW_BUILD_DOCS OFF"
            "GLFW_BUILD_TESTS OFF"
            "GLFW_BUILD_EXAMPLES OFF"
            "GLFW_INSTALL OFF"
            "GLFW_BUILD_WAYLAND OFF"
    )

    CPMAddPackage(
        NAME glfw3webgpu
        GITHUB_REPOSITORY eliemichel/glfw3webgpu
        GIT_TAG main  # v27 API compatible
    )

    # glfw3webgpu needs _GLFW_X11 on Linux for X11 surface creation
    if(UNIX AND NOT APPLE)
        target_compile_definitions(glfw3webgpu PRIVATE _GLFW_X11)
    endif()

    #-------------------------------------------------------------------------
    # ImGui - Immediate mode GUI library with docking support
    #-------------------------------------------------------------------------
    CPMAddPackage(
        NAME imgui
        GITHUB_REPOSITORY ocornut/imgui
        GIT_TAG docking
        DOWNLOAD_ONLY YES
    )

    if(imgui_ADDED)
        # Core ImGui source files
        set(IMGUI_SOURCES
            ${imgui_SOURCE_DIR}/imgui.cpp
            ${imgui_SOURCE_DIR}/imgui_demo.cpp
            ${imgui_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_SOURCE_DIR}/imgui_tables.cpp
            ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        )

        # Platform backend: GLFW for desktop
        list(APPEND IMGUI_SOURCES ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp)

        # Renderer backend: WebGPU (wgpu-native)
        list(APPEND IMGUI_SOURCES ${imgui_SOURCE_DIR}/backends/imgui_impl_wgpu.cpp)

        add_library(imgui STATIC ${IMGUI_SOURCES})
        target_include_directories(imgui PUBLIC
            ${imgui_SOURCE_DIR}
            ${imgui_SOURCE_DIR}/backends
        )

        # Link dependencies and set compile definitions
        if(APPLE)
            target_link_libraries(imgui PUBLIC glfw webgpu)
            # imgui_impl_wgpu.cpp includes Cocoa.h and must be compiled as Objective-C++
            set_source_files_properties(
                ${imgui_SOURCE_DIR}/backends/imgui_impl_wgpu.cpp
                PROPERTIES COMPILE_FLAGS "-x objective-c++"
            )
        elseif(WIN32)
            target_link_libraries(imgui PUBLIC glfw webgpu)
        else()
            # Linux - requires X11
            find_package(X11 REQUIRED)
            target_link_libraries(imgui PUBLIC glfw webgpu X11::X11)
        endif()

        # Backend-specific defines: wgpu-native vs Dawn
        if(WEBGPU_BACKEND STREQUAL "wgpu")
            target_compile_definitions(imgui PUBLIC IMGUI_IMPL_WEBGPU_BACKEND_WGPU=1)
        else()
            # Dawn backend - uses standard WebGPU API
            target_compile_definitions(imgui PUBLIC IMGUI_IMPL_WEBGPU_BACKEND_DAWN=1)
        endif()

        set_target_properties(imgui PROPERTIES POSITION_INDEPENDENT_CODE ON)
    endif()

endif()

# libuv for async I/O (PTY reading, event loops) - needed for all platforms except Emscripten
if(NOT EMSCRIPTEN)
    CPMAddPackage(
        NAME libuv
        GITHUB_REPOSITORY libuv/libuv
        GIT_TAG v1.48.0
        OPTIONS
            "LIBUV_BUILD_TESTS OFF"
            "LIBUV_BUILD_BENCH OFF"
    )
endif()

#-----------------------------------------------------------------------------
# Android-specific dependencies
#-----------------------------------------------------------------------------
if(YETTY_ANDROID)
    # Add native_app_glue from Android NDK
    add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
    )
    target_include_directories(native_app_glue PUBLIC
        ${ANDROID_NDK}/sources/android/native_app_glue
    )

    # Toybox configuration (BSD-licensed shell utilities)
    if(TOYBOX_PATH)
        message(STATUS "Toybox path: ${TOYBOX_PATH}")
        add_compile_definitions(YETTY_TOYBOX_PATH="${TOYBOX_PATH}")
    else()
        message(STATUS "TOYBOX_PATH not set - will look for toybox at runtime")
    endif()
endif()

#-----------------------------------------------------------------------------
# libvterm (all platforms - terminal emulation library)
# Note: Web builds won't use PTY, but need vterm for types/parsing
#-----------------------------------------------------------------------------
set(LIBVTERM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/libvterm-0.3.3)
if(EXISTS ${LIBVTERM_DIR})
    add_library(vterm STATIC
        ${LIBVTERM_DIR}/src/encoding.c
        ${LIBVTERM_DIR}/src/keyboard.c
        ${LIBVTERM_DIR}/src/mouse.c
        ${LIBVTERM_DIR}/src/parser.c
        ${LIBVTERM_DIR}/src/pen.c
        ${LIBVTERM_DIR}/src/screen.c
        ${LIBVTERM_DIR}/src/state.c
        ${LIBVTERM_DIR}/src/unicode.c
        ${LIBVTERM_DIR}/src/vterm.c
    )
    target_include_directories(vterm PUBLIC
        ${LIBVTERM_DIR}/include
        ${LIBVTERM_DIR}/src
    )
    target_compile_options(vterm PRIVATE -w)  # Suppress warnings in third-party code
endif()

#-----------------------------------------------------------------------------
# Core shared library (for plugins to link against on desktop)
#-----------------------------------------------------------------------------
if(NOT EMSCRIPTEN AND NOT YETTY_ANDROID)
    # Python plugin option — actual build is triggered by src/yetty/python/CMakeLists.txt
    option(YETTY_PLUGIN_PYTHON "Build Python plugin (requires Python build)" ON)
endif()

#-----------------------------------------------------------------------------
# Main target (executable for desktop/web, shared library for Android)
#-----------------------------------------------------------------------------
# Common sources for all platforms
set(YETTY_SOURCES
)

# Add core sources for non-desktop platforms (desktop uses yetty_core library)
if(EMSCRIPTEN OR YETTY_ANDROID)
    list(APPEND YETTY_SOURCES
        # src/yetty/font.cpp  # Moved to obsolete
        src/yetty/font-manager.cpp
        # src/yetty/rich-text.cpp  # TODO: refactor for new Font interface
        src/yetty/bm-font.cpp
        src/yetty/shader-font.cpp
        src/yetty/shader-manager.cpp
        src/yetty/ydraw.cpp
        src/yetty/widget-frame-renderer.cpp
    )
endif()

# Platform-specific entry point and sources
if(YETTY_ANDROID)
    # Android uses unified main.cpp with android_main() entry point
    list(APPEND YETTY_SOURCES
        src/yetty/main.cpp
        src/yetty/yetty.cpp
        src/yetty/config.cpp
        src/yetty/terminal.cpp
        src/yetty/gpu-screen.cpp
        src/yetty/osc-command.cpp
        # src/yetty/widget-factory.cpp  # TODO: refactor later
    )
elseif(NOT EMSCRIPTEN)
    # Desktop uses main.cpp with GLFW
    # Desktop with plugin support
    list(APPEND YETTY_SOURCES
        src/yetty/main.cpp
        src/yetty/yetty.cpp
        src/yetty/config.cpp
        src/yetty/terminal.cpp
        src/yetty/terminal-view.cpp
        src/yetty/gpu-screen.cpp
        # src/yetty/widget-factory.cpp  # TODO: refactor later
        src/yetty/osc-command.cpp
        #src/yetty/custom-glyph-plugin.cpp
        src/yetty/card-buffer-manager.cpp
        src/yetty/card-factory.cpp
        src/yetty/card.cpp
        src/yetty/imgui-manager.cpp
        # Card sources added via add_subdirectory(src/yetty/cards)
        # Core sources (previously in yetty_core)
        # src/yetty/font.cpp  # Moved to obsolete
        src/yetty/ms-msdf-font.cpp
        src/yetty/font-manager.cpp
        src/yetty/msdf-cdb-provider.cpp
        src/yetty/msdf-gen/generator.cpp
        # src/yetty/rich-text.cpp  # TODO: refactor for new Font interface
        src/yetty/bm-font.cpp
        src/yetty/shader-font.cpp
        src/yetty/shader-manager.cpp
        #src/yetty/ydraw.cpp
        src/yetty/widget-frame-renderer.cpp
        src/yetty/tile.cpp
        src/yetty/view.cpp
        src/yetty/workspace.cpp
        src/yetty/base/event-loop.cpp
        src/yetty/rpc/rpc-server.cpp
        src/yetty/rpc/event-loop-handler.cpp
        src/yetty/rpc/socket-path.cpp
        # Ymery - YAML-driven ImGui widget system (ported from ymery-cpp)
        src/yetty/ymery/types.cpp
        src/yetty/ymery/data-bag.cpp
        src/yetty/ymery/data-tree.cpp
        src/yetty/ymery/dispatcher.cpp
        src/yetty/ymery/lang.cpp
        src/yetty/ymery/simple-data-tree.cpp
        src/yetty/ymery/renderer.cpp
    )
else()
    # Web uses main.cpp with Emscripten GLFW
    list(APPEND YETTY_SOURCES
        src/yetty/main.cpp
        src/yetty/yetty.cpp
        src/yetty/config.cpp
        src/yetty/input-handler.cpp
        src/yetty/web-terminal.cpp
        src/yetty/web-display.cpp
        src/yetty/gpu-screen.cpp
    )
endif()

# Android builds as shared library, others as executable
if(YETTY_ANDROID)
    add_library(yetty SHARED ${YETTY_SOURCES})
else()
    add_executable(yetty ${YETTY_SOURCES})
endif()

target_include_directories(yetty PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Card subdirectories (desktop only - cards use CardBufferManager, ImGui, etc.)
if(NOT EMSCRIPTEN AND NOT YETTY_ANDROID)
    add_subdirectory(src/yetty/cards)
endif()

target_compile_definitions(yetty PRIVATE
    CMAKE_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

if(EMSCRIPTEN)
    #-------------------------------------------------------------------------
    # Emscripten (Web) build - uses pre-built atlas, no msdfgen
    #-------------------------------------------------------------------------
    target_compile_definitions(yetty PRIVATE
        YETTY_WEB=1
        YETTY_ANDROID=0
        YETTY_USE_PREBUILT_ATLAS=1
        # Disable ytrace for Emscripten (POSIX socket code not supported)
        YTRACE_ENABLED=0
        YTRACE_ENABLE_YLOG=0
        YTRACE_ENABLE_YTRACE=0
        YTRACE_ENABLE_YDEBUG=0
        YTRACE_ENABLE_YINFO=0
        YTRACE_ENABLE_YWARN=0
        YTRACE_ENABLE_YERROR=0
        YTRACE_ENABLE_YFUNC=0
    )

    # Include ytrace headers (macros become no-ops with YTRACE_ENABLED=0)
    target_include_directories(yetty PRIVATE ${ytrace_SOURCE_DIR}/include)

    target_link_options(yetty PRIVATE
        -sUSE_GLFW=3
        -sUSE_WEBGPU
        -sASYNCIFY
        -sALLOW_MEMORY_GROWTH
        -sWASM_BIGINT
        --no-heap-copy
        "--preload-file=${CMAKE_CURRENT_SOURCE_DIR}/src/yetty/shaders/gpu-screen.wgsl@/gpu-screen.wgsl"
        "--preload-file=${CMAKE_CURRENT_SOURCE_DIR}/assets@/assets"
        "--preload-file=${CMAKE_CURRENT_SOURCE_DIR}/demo@/demo"
        # Export web terminal functions for JavaScript
        "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap','UTF8ToString','stringToUTF8','allocateUTF8','writeAsciiToMemory','FS']"
        "-sEXPORTED_FUNCTIONS=['_main','_web_terminal_init','_web_terminal_write','_web_terminal_print','_web_terminal_get_cell_char','_web_terminal_get_cursor_row','_web_terminal_get_cursor_col','_web_terminal_get_rows','_web_terminal_get_cols','_web_terminal_key_input','_web_terminal_read_output','_web_terminal_cleanup','_yetty_write','_yetty_key','_yetty_special_key','_yetty_read_input','_yetty_sync','_yetty_set_scale','_yetty_resize','_yetty_get_cols','_yetty_get_rows','_malloc','_free']"
    )

    # Use -O1 to avoid wasm-opt which has binaryen version mismatch issues
    target_compile_options(yetty PRIVATE -O1)
    target_link_options(yetty PRIVATE -O1)

    # Output as .js (not .html) to avoid html-minifier-terser dependency
    # A standalone index.html will be provided to load yetty.js
    set_target_properties(yetty PROPERTIES SUFFIX ".js")

    target_link_libraries(yetty PRIVATE
        webgpu
        glm::glm
        stb
        yaml-cpp
        vterm
        msgpack-cxx
        # ytrace disabled for Emscripten (POSIX sockets not supported)
    )

elseif(YETTY_ANDROID)
    #-------------------------------------------------------------------------
    # Android build - WebGPU via Vulkan, native_app_glue, Toybox shell
    #-------------------------------------------------------------------------
    target_compile_definitions(yetty PRIVATE
        YETTY_WEB=0
        YETTY_ANDROID=1
        YETTY_USE_PREBUILT_ATLAS=1
    )

    # Create shared library for Android (not executable)
    set_target_properties(yetty PROPERTIES
        LIBRARY_OUTPUT_NAME "yetty"
    )

    target_link_libraries(yetty PRIVATE
        webgpu
        -Wl,--whole-archive
        native_app_glue
        -Wl,--no-whole-archive
        glm::glm
        stb
        yaml-cpp
        vterm
        msgpack-cxx
        ytrace::ytrace
        lz4_static
        uv_a
        android
        log
    )

    # Copy assets to Android assets directory (where Gradle expects them)
    set(ANDROID_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build-android/assets")
    file(MAKE_DIRECTORY ${ANDROID_ASSETS_DIR})
    file(GLOB ASSET_FILES "${CMAKE_CURRENT_SOURCE_DIR}/assets/*")
    file(COPY ${ASSET_FILES} DESTINATION ${ANDROID_ASSETS_DIR})
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/yetty/shaders/gpu-screen.wgsl
        ${ANDROID_ASSETS_DIR}/gpu-screen.wgsl
        COPYONLY
    )

    message(STATUS "Building for Android with WebGPU (Vulkan backend)")

else()
    #-------------------------------------------------------------------------
    # Desktop (Native) build - can generate atlas at runtime
    #-------------------------------------------------------------------------
    target_compile_definitions(yetty PRIVATE
        YETTY_WEB=0
        YETTY_ANDROID=0
        YETTY_USE_PREBUILT_ATLAS=0
    )

    # Export symbols so plugins can find them (e.g., WidgetFactory::getShaderManager)
    set_target_properties(yetty PROPERTIES ENABLE_EXPORTS TRUE)

    # Platform-specific font discovery
    if(WIN32)
        target_compile_definitions(yetty PRIVATE YETTY_USE_DIRECTWRITE=1)
        target_link_libraries(yetty PRIVATE dwrite)
    else()
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
        target_include_directories(yetty PRIVATE ${FONTCONFIG_INCLUDE_DIRS})
        target_compile_definitions(yetty PRIVATE YETTY_USE_FONTCONFIG=1)
    endif()

    target_link_libraries(yetty PRIVATE
        webgpu
        glfw
        glfw3webgpu
        imgui
        glm::glm
        stb
        yaml-cpp
        args
        vterm
        msgpack-cxx
        msdfgen::msdfgen-core
        msdfgen::msdfgen-ext
        freetype
        msdf::wgsl
        ytrace::ytrace
        lz4_static
        uv_a
        cdb
        $<$<NOT:$<BOOL:${WIN32}>>:${FONTCONFIG_LIBRARIES}>
    )

    # Platform-specific PTY library
    if(WIN32)
        # Windows: Link against ConPTY (part of kernel32)
        target_compile_definitions(yetty PRIVATE YETTY_USE_CONPTY=1)
    else()
        # Unix: Link against util for forkpty (Linux only, macOS has it in libc)
        if(NOT APPLE)
            target_link_libraries(yetty PRIVATE util)
        endif()
        target_compile_definitions(yetty PRIVATE YETTY_USE_FORKPTY=1)

        # Link Python into main executable so symbols are globally visible to numpy extensions
        # Use --whole-archive (Linux) or -force_load (macOS) to export all Python symbols
        if(TARGET python_embedded)
            add_dependencies(yetty python_embedded)
            if(APPLE)
                # On macOS, also need libintl and iconv for Python's locale module
                if(EXISTS "/usr/local/lib/libintl.a")
                    set(INTL_LIBRARY "/usr/local/lib/libintl.a")
                elseif(EXISTS "/opt/homebrew/lib/libintl.a")
                    set(INTL_LIBRARY "/opt/homebrew/lib/libintl.a")
                elseif(EXISTS "/opt/local/lib/libintl.a")
                    set(INTL_LIBRARY "/opt/local/lib/libintl.a")
                endif()

                target_link_libraries(yetty PRIVATE
                    -Wl,-force_load,$<TARGET_FILE:python_embedded>
                    ${INTL_LIBRARY}
                    iconv
                    m pthread
                )
            else()
                target_link_libraries(yetty PRIVATE
                    -Wl,--whole-archive
                    python_embedded
                    -Wl,--no-whole-archive
                    m dl pthread
                )
            endif()

        endif()
    endif()

    # Copy assets at build time (not just configure time)
    add_custom_command(TARGET yetty POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            ${CMAKE_CURRENT_BINARY_DIR}/assets
        COMMENT "Copying assets to build directory"
    )
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/yetty/shaders/gpu-screen.wgsl
        ${CMAKE_CURRENT_BINARY_DIR}/gpu-screen.wgsl
        COPYONLY
    )

    #-------------------------------------------------------------------------
    # Unit tests (desktop only)
    #-------------------------------------------------------------------------
    option(YETTY_BUILD_TESTS "Build unit tests" OFF)
    if(YETTY_BUILD_TESTS)
        enable_testing()
        add_subdirectory(test/ut)
    endif()

    # Plugin tester disabled - was using yetty_core which is removed
    # TODO: Re-enable when needed

    # RPC client library (libyetty-rpc.a)
    add_subdirectory(src/yetty/rpc)

    # yettyc CLI client
    add_subdirectory(src/yetty/client)

    # Tools
    add_subdirectory(tools)

    # MSDF generator tool
    add_subdirectory(src/yetty/msdf-gen)

    # MSDFGL WebGPU port (sandbox) - abandoned, see README
    add_subdirectory(sandbox/msdfgl)

    # MSDF-WGSL: Compute shader-based MSDF generation
    add_subdirectory(sandbox/msdf-wgsl)
endif()


# Python subdirectory moved earlier in file (before yetty target)

