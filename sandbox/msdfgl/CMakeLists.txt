cmake_minimum_required(VERSION 3.16)
project(msdfgl-webgpu VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(MSDFGL_BUILD_EXAMPLE "Build example application" OFF)

# Main library
add_library(msdfgl-webgpu
    src/msdfgl.cpp
)

add_library(msdfgl::webgpu ALIAS msdfgl-webgpu)

target_include_directories(msdfgl-webgpu
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        # Include yetty headers for wgpu-compat.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

# Link against dependencies from parent yetty project
# These targets should already be available when included from yetty's CMakeLists.txt
if(TARGET webgpu)
    target_link_libraries(msdfgl-webgpu PUBLIC webgpu)
else()
    message(WARNING "webgpu target not found - msdfgl-webgpu may not link correctly")
endif()

if(TARGET freetype OR TARGET Freetype::Freetype)
    if(TARGET Freetype::Freetype)
        target_link_libraries(msdfgl-webgpu PUBLIC Freetype::Freetype)
    else()
        target_link_libraries(msdfgl-webgpu PUBLIC freetype)
    endif()
else()
    find_package(Freetype REQUIRED)
    target_link_libraries(msdfgl-webgpu PUBLIC Freetype::Freetype)
endif()

target_compile_features(msdfgl-webgpu PUBLIC cxx_std_17)

# Shader files - make them available for downstream projects
set(MSDFGL_SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders PARENT_SCOPE)

# Copy shader files to build directory
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaders/msdf_gen.wgsl
               ${CMAKE_CURRENT_BINARY_DIR}/shaders/msdf_gen.wgsl COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaders/font_render.wgsl
               ${CMAKE_CURRENT_BINARY_DIR}/shaders/font_render.wgsl COPYONLY)

# Example application - always build when building msdfgl
add_subdirectory(example)

# CDB generator application
add_subdirectory(gen-cdb)

# CDB comparison tool
add_subdirectory(compare)

# Atlas viewer (debug tool)
add_subdirectory(render-atlas)
