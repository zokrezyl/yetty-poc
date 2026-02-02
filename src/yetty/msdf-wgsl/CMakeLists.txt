cmake_minimum_required(VERSION 3.16)
project(msdf-wgsl VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main library
add_library(msdf-wgsl
    src/msdf-wgsl.cpp
)

add_library(msdf::wgsl ALIAS msdf-wgsl)

target_include_directories(msdf-wgsl
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
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

# CDB generator
add_subdirectory(gen-cdb)

# Atlas viewer
add_subdirectory(render-atlas)

# CDB comparison tool
add_subdirectory(compare)
