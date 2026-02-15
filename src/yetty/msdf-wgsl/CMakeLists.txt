cmake_minimum_required(VERSION 3.16)
project(msdf-wgsl VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main library
add_library(msdf-wgsl
    src/msdf-wgsl.cpp
)

# Windows: prevent min/max macros from conflicting with std::numeric_limits
if(WIN32)
    target_compile_definitions(msdf-wgsl PRIVATE NOMINMAX)
endif()

add_library(msdf::wgsl ALIAS msdf-wgsl)

# Get the yetty root directory (3 levels up from src/yetty/msdf-wgsl)
get_filename_component(YETTY_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../.." ABSOLUTE)

target_include_directories(msdf-wgsl
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${YETTY_ROOT_DIR}/include
)

# Link dependencies
if(TARGET webgpu)
    target_link_libraries(msdf-wgsl PUBLIC webgpu)
endif()

if(TARGET cdb)
    target_link_libraries(msdf-wgsl PUBLIC cdb)
endif()

# FreeType headers only — actual static lib linked by final executables
target_include_directories(msdf-wgsl PUBLIC ${FREETYPE_INCLUDE_DIR})

# Shader files
set(MSDF_WGSL_SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders PARENT_SCOPE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaders/msdf_gen.wgsl
               ${CMAKE_CURRENT_BINARY_DIR}/shaders/msdf_gen.wgsl COPYONLY)

# Desktop-only tools (not needed on Android which uses pre-built atlases)
if(NOT YETTY_ANDROID)
    # CDB generator
    add_subdirectory(gen-cdb)

    # Atlas viewer
    add_subdirectory(render-atlas)

    # CDB comparison tool
    add_subdirectory(compare)
endif()
