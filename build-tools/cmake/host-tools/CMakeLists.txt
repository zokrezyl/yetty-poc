# Standalone build for host tools and VM tools
# Used when cross-compiling to build native tools, and for building
# static x86_64 binaries for JSLinux VM (yecho, ycat, ybrowser)
# No X11/GLFW/imgui dependencies

cmake_minimum_required(VERSION 3.20)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "Minimum CMake policy version")
cmake_policy(SET CMP0074 NEW)

project(yetty-host-tools LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)

# YETTY_ROOT is three levels up from build-tools/cmake/host-tools/
get_filename_component(YETTY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../.." ABSOLUTE)

include(${YETTY_ROOT}/build-tools/cmake/CPM.cmake)

# spdlog (needed by ytrace)
CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY gabime/spdlog
    GIT_TAG v1.16.0
)

# ytrace for logging
CPMAddPackage(
    NAME ytrace
    GITHUB_REPOSITORY zokrezyl/ytrace
    GIT_TAG da73e1fba172c61d204debd84f30f4703c6f3d19
    OPTIONS
        "YTRACE_BUILD_TOOLS OFF"
        "YTRACE_BUILD_EXAMPLES OFF"
)

# args for command line parsing
CPMAddPackage(
    NAME args
    GITHUB_REPOSITORY Taywee/args
    GIT_TAG 6.4.6
)

# glm for math
CPMAddPackage(
    NAME glm
    GITHUB_REPOSITORY g-truc/glm
    GIT_TAG 1.0.1
)

# zlib (required by libpng and FreeType)
include(${YETTY_ROOT}/build-tools/cmake/libs/zlib.cmake)

# libpng (required by FreeType)
include(${YETTY_ROOT}/build-tools/cmake/libs/libpng.cmake)

# FreeType (includes brotli, bzip2)
include(${YETTY_ROOT}/build-tools/cmake/FreeType.cmake)

# msdfgen
CPMAddPackage(
    NAME msdfgen
    GITHUB_REPOSITORY Chlumsky/msdfgen
    GIT_TAG v1.12
    DOWNLOAD_ONLY YES
)

set(tinyxml2_BUILD_TESTING OFF CACHE BOOL "" FORCE)
CPMAddPackage(
    NAME tinyxml2
    GITHUB_REPOSITORY leethomason/tinyxml2
    GIT_TAG 10.0.0
)

if(msdfgen_ADDED)
    file(GLOB MSDFGEN_CORE_SOURCES "${msdfgen_SOURCE_DIR}/core/*.cpp")
    add_library(msdfgen-core STATIC ${MSDFGEN_CORE_SOURCES})
    add_library(msdfgen::msdfgen-core ALIAS msdfgen-core)
    target_include_directories(msdfgen-core PUBLIC ${msdfgen_SOURCE_DIR})
    target_compile_features(msdfgen-core PUBLIC cxx_std_11)
    target_compile_definitions(msdfgen-core PUBLIC MSDFGEN_PUBLIC= MSDFGEN_USE_CPP11)

    set(MSDFGEN_EXT_SOURCES
        ${msdfgen_SOURCE_DIR}/ext/import-font.cpp
        ${msdfgen_SOURCE_DIR}/ext/import-svg.cpp
        ${msdfgen_SOURCE_DIR}/ext/resolve-shape-geometry.cpp
    )
    add_library(msdfgen-ext STATIC ${MSDFGEN_EXT_SOURCES})
    add_library(msdfgen::msdfgen-ext ALIAS msdfgen-ext)
    target_include_directories(msdfgen-ext PUBLIC ${msdfgen_SOURCE_DIR} PRIVATE ${FREETYPE_INCLUDE_DIR})
    target_compile_definitions(msdfgen-ext PUBLIC MSDFGEN_EXT_PUBLIC= MSDFGEN_EXTENSIONS MSDFGEN_USE_TINYXML2 MSDFGEN_DISABLE_PNG)
    target_link_libraries(msdfgen-ext PRIVATE msdfgen::msdfgen-core tinyxml2::tinyxml2)
endif()

# CDB (portable howerj version)
CPMAddPackage(
    NAME howerj_cdb
    URL https://github.com/howerj/cdb/archive/refs/tags/v4.3.2.tar.gz
    VERSION 4.3.2
    DOWNLOAD_ONLY YES
)
if(howerj_cdb_ADDED)
    add_library(cdb STATIC ${howerj_cdb_SOURCE_DIR}/cdb.c)
    target_include_directories(cdb PUBLIC ${howerj_cdb_SOURCE_DIR})
    target_compile_definitions(cdb PUBLIC YETTY_USE_HOWERJ_CDB=1)
    target_compile_options(cdb PRIVATE -w)
endif()

# cdb-wrapper
add_library(cdb-wrapper STATIC ${YETTY_ROOT}/src/yetty/cdb-wrapper.cpp)
target_include_directories(cdb-wrapper PUBLIC ${YETTY_ROOT}/include)
target_link_libraries(cdb-wrapper PUBLIC cdb ytrace::ytrace)
target_compile_definitions(cdb-wrapper PUBLIC YETTY_USE_HOWERJ_CDB=1)

# msdf-gen tool
add_executable(yetty-msdf-gen
    ${YETTY_ROOT}/src/yetty/msdf-gen/main.cpp
    ${YETTY_ROOT}/src/yetty/msdf-gen/generator.cpp
)
target_include_directories(yetty-msdf-gen PRIVATE
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
    ${FREETYPE_INCLUDE_DIR}
)
target_link_libraries(yetty-msdf-gen PRIVATE
    msdfgen::msdfgen-core
    msdfgen::msdfgen-ext
    glm::glm
    ytrace::ytrace
    args
    cdb-wrapper
    ${FREETYPE_ALL_LIBS}
)
target_compile_definitions(yetty-msdf-gen PRIVATE CMAKE_SOURCE_DIR="${YETTY_ROOT}")

# =============================================================================
# VM tools - statically linked for x86_64 Linux (JSLinux VM)
# =============================================================================

# Generate glyph registry header for yecho
set(GLYPH_REGISTRY_HEADER "${CMAKE_CURRENT_BINARY_DIR}/glyph-registry-generated.h")
set(SHADER_DIR "${YETTY_ROOT}/src/yetty/shaders")

find_package(Python3 COMPONENTS Interpreter REQUIRED)
execute_process(
    COMMAND ${Python3_EXECUTABLE}
        ${YETTY_ROOT}/src/yetty/yecho/gen-glyph-registry.py
        ${SHADER_DIR}
        ${GLYPH_REGISTRY_HEADER}
    WORKING_DIRECTORY ${YETTY_ROOT}/src/yetty/yecho
    RESULT_VARIABLE GEN_RESULT
)

# yecho - for VM (try static with musl, fallback to dynamic)
add_executable(yecho-static
    ${YETTY_ROOT}/src/yetty/yecho/yecho.cpp
    ${YETTY_ROOT}/src/yetty/yecho/yecho-parser.cpp
)
target_include_directories(yecho-static PRIVATE
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src/yetty/yecho
    ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(yecho-static PRIVATE args)

# Try to find musl-gcc for true static linking, otherwise build dynamic
find_program(MUSL_GCC musl-gcc)
if(MUSL_GCC)
    message(STATUS "Using musl-gcc for static VM tools")
    set(CMAKE_C_COMPILER ${MUSL_GCC})
    set(CMAKE_CXX_COMPILER ${MUSL_GCC})
    target_link_options(yecho-static PRIVATE -static)
else()
    message(STATUS "musl-gcc not found, building dynamic VM tools")
endif()

target_compile_features(yecho-static PRIVATE cxx_std_23)
set_target_properties(yecho-static PROPERTIES OUTPUT_NAME "yecho")

# =============================================================================
# ycat-static — syntax-highlighted cat for JSLinux VM
# =============================================================================

# cpr (HTTP client, builds libcurl from source)
# Prefer static OpenSSL for static linking
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

# Hide our custom zlib from curl (it tries ZLIB::ZLIB which doesn't match our target)
set(_SAVE_ZLIB_FOUND ${ZLIB_FOUND})
set(_SAVE_ZLIB_LIBRARY ${ZLIB_LIBRARY})
set(ZLIB_FOUND FALSE CACHE BOOL "" FORCE)
unset(ZLIB_LIBRARY CACHE)

set(CURL_ZLIB OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)
set(CURL_BROTLI OFF CACHE BOOL "" FORCE)
set(CURL_ZSTD OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBSSH2 OFF CACHE BOOL "" FORCE)

CPMAddPackage(
    NAME cpr
    GITHUB_REPOSITORY libcpr/cpr
    GIT_TAG 1.14.1
    OPTIONS
        "CPR_USE_SYSTEM_CURL OFF"
        "CPR_CURL_USE_LIBPSL OFF"
        "BUILD_SHARED_LIBS OFF"
        "CPR_ENABLE_SSL ON"
        "CPR_BUILD_TESTS OFF"
        "CPR_BUILD_TESTS_SSL OFF"
)

# Restore ZLIB state
set(ZLIB_FOUND ${_SAVE_ZLIB_FOUND} CACHE BOOL "" FORCE)
if(_SAVE_ZLIB_LIBRARY)
    set(ZLIB_LIBRARY ${_SAVE_ZLIB_LIBRARY} CACHE STRING "" FORCE)
endif()

# Tree-sitter + grammars
include(${YETTY_ROOT}/build-tools/cmake/TreeSitter.cmake)

# libmagic (file type detection)
include(${YETTY_ROOT}/build-tools/cmake/Libmagic.cmake)

# Generate tree-sitter query header (same logic as src/yetty/ycat/CMakeLists.txt)
set(TS_GRAMMAR_NAMES
    c cpp python javascript typescript rust go java bash json yaml toml html xml
)
set(TS_GENERATED_HEADER "${CMAKE_CURRENT_BINARY_DIR}/ts_queries_generated.h")

set(TS_QUERY_PARENT_cpp "c")
set(TS_QUERY_PARENT_typescript "javascript")

set(_TS_HEADER "// Auto-generated — do not edit\n#pragma once\n\n")
foreach(NAME ${TS_GRAMMAR_NAMES})
    set(_QUERY_FILE "${TS_QUERIES_DIR_${NAME}}/highlights.scm")
    if(EXISTS "${_QUERY_FILE}")
        set(_QUERY_CONTENT "")
        if(DEFINED TS_QUERY_PARENT_${NAME})
            set(_PARENT "${TS_QUERY_PARENT_${NAME}}")
            set(_PARENT_FILE "${TS_QUERIES_DIR_${_PARENT}}/highlights.scm")
            if(EXISTS "${_PARENT_FILE}")
                file(READ "${_PARENT_FILE}" _QUERY_CONTENT)
                string(APPEND _QUERY_CONTENT "\n")
            endif()
        endif()
        file(READ "${_QUERY_FILE}" _CHILD_CONTENT)
        string(APPEND _QUERY_CONTENT "${_CHILD_CONTENT}")
        string(REPLACE "\\" "\\\\" _QUERY_CONTENT "${_QUERY_CONTENT}")
        string(REPLACE "\"" "\\\"" _QUERY_CONTENT "${_QUERY_CONTENT}")
        string(REPLACE "\n" "\\n\"\n\"" _QUERY_CONTENT "${_QUERY_CONTENT}")
        string(APPEND _TS_HEADER "static const char TS_QUERY_${NAME}[] =\n\"${_QUERY_CONTENT}\";\n\n")
    else()
        string(APPEND _TS_HEADER "static const char TS_QUERY_${NAME}[] = \"\";\n\n")
    endif()
endforeach()
file(WRITE "${TS_GENERATED_HEADER}" "${_TS_HEADER}")

add_executable(ycat-static
    ${YETTY_ROOT}/src/yetty/ycat/main.cpp
    ${YETTY_ROOT}/src/yetty/ycat/osc.cpp
    ${YETTY_ROOT}/src/yetty/ycat/ts/grammars.cpp
    ${YETTY_ROOT}/src/yetty/ycat/ts/highlight.cpp
)
target_include_directories(ycat-static PRIVATE
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src/yetty/ycat
    ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(ycat-static PRIVATE
    args
    magic
    cpr::cpr
    tree-sitter-core
    ts-grammar-c
    ts-grammar-cpp
    ts-grammar-python
    ts-grammar-javascript
    ts-grammar-typescript
    ts-grammar-rust
    ts-grammar-go
    ts-grammar-java
    ts-grammar-bash
    ts-grammar-json
    ts-grammar-yaml
    ts-grammar-toml
    ts-grammar-html
    ts-grammar-xml
)
target_compile_definitions(ycat-static PRIVATE
    YCAT_HAS_LIBMAGIC=1
    YCAT_MAGIC_MGC_PATH="/usr/share/misc/magic.mgc"
)
target_compile_features(ycat-static PRIVATE cxx_std_23)
set_target_properties(ycat-static PROPERTIES OUTPUT_NAME "ycat")

# =============================================================================
# ybrowser-static — terminal HTML browser for JSLinux VM
# =============================================================================

# litehtml
CPMAddPackage(
    NAME litehtml
    GITHUB_REPOSITORY litehtml/litehtml
    GIT_TAG v0.9
    OPTIONS
        "LITEHTML_BUILD_TESTING OFF"
)

# stb (image loading)
include(${YETTY_ROOT}/build-tools/cmake/libs/stb.cmake)

# fontconfig (system package for font resolution, with static deps)
find_package(PkgConfig REQUIRED)
set(PKG_CONFIG_EXECUTABLE_SAVE ${PKG_CONFIG_EXECUTABLE})
pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
# Get static link flags (includes transitive deps like expat)
execute_process(
    COMMAND pkg-config --static --libs fontconfig
    OUTPUT_VARIABLE FONTCONFIG_STATIC_LDFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
# Parse into a list
separate_arguments(FONTCONFIG_STATIC_LIBS UNIX_COMMAND "${FONTCONFIG_STATIC_LDFLAGS}")

add_executable(ybrowser-static
    ${YETTY_ROOT}/tools/ybrowser/main.cpp
    ${YETTY_ROOT}/tools/ybrowser/stb_impl.cpp
    ${YETTY_ROOT}/src/yetty/yhtml/html-container.cpp
    ${YETTY_ROOT}/src/yetty/yhtml/html-renderer.cpp
    ${YETTY_ROOT}/src/yetty/yhtml/http-fetcher.cpp
    ${YETTY_ROOT}/src/yetty/ydraw/ydraw-buffer.cpp
    ${YETTY_ROOT}/src/yetty/font/raw-font.cpp
    ${YETTY_ROOT}/src/yetty/font/raw-font-manager.cpp
    ${YETTY_ROOT}/src/yetty/font/freetype.cpp
    ${YETTY_ROOT}/src/yetty/ycat/osc.cpp
)
target_include_directories(ybrowser-static PRIVATE
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
    ${YETTY_ROOT}/src/yetty/yhtml
    ${YETTY_ROOT}/src/yetty/ycat
    ${YETTY_ROOT}/src/yetty/ydraw
    ${FREETYPE_INCLUDE_DIR}
    ${FONTCONFIG_INCLUDE_DIRS}
)
target_link_libraries(ybrowser-static PRIVATE
    litehtml
    cpr::cpr
    ${FREETYPE_ALL_LIBS}
    args
    ytrace::ytrace
    stb
    ${FONTCONFIG_STATIC_LIBS}
)
target_compile_definitions(ybrowser-static PRIVATE YETTY_USE_FONTCONFIG=1)
target_compile_features(ybrowser-static PRIVATE cxx_std_20)
set_target_properties(ybrowser-static PROPERTIES OUTPUT_NAME "ybrowser")
