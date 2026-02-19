#=============================================================================
# yetty - WebGPU Terminal Emulator
#=============================================================================
cmake_minimum_required(VERSION 3.20)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "Minimum CMake policy version")
cmake_policy(SET CMP0074 NEW)

project(yetty VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

# Build options
option(YETTY_ANDROID "Build for Android" OFF)
set(WEBGPU_BACKEND "dawn" CACHE STRING "WebGPU backend (wgpu or dawn)")
set(TOYBOX_PATH "" CACHE PATH "Path to Toybox binary")

# Include CPM
include(build-tools/cmake/CPM.cmake)

# Detect platform and include appropriate target
if(YETTY_ANDROID)
    include(build-tools/cmake/targets/android.cmake)
elseif(EMSCRIPTEN)
    include(build-tools/cmake/targets/webasm.cmake)
elseif(WIN32)
    include(build-tools/cmake/targets/windows.cmake)
elseif(APPLE)
    include(build-tools/cmake/targets/macos.cmake)
else()
    # Linux and other Unix
    include(build-tools/cmake/targets/linux.cmake)
endif()
