# Standalone build for host tools (msdf-gen only)
# Used when cross-compiling to build native tools
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
