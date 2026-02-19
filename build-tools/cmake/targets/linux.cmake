# Linux desktop build target

include(${CMAKE_SOURCE_DIR}/build-tools/cmake/targets/shared.cmake)

# Linux-specific libraries
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/args.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/lz4.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/libuv.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/glfw.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/imgui.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/TreeSitter.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/Libmagic.cmake)

# Add src/yetty (builds libraries)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/yetty ${CMAKE_BINARY_DIR}/src/yetty)

# Create executable with core sources + platform
add_executable(yetty
    ${YETTY_CORE_SOURCES}
    ${YETTY_DESKTOP_SOURCES}
    ${CMAKE_SOURCE_DIR}/src/yetty/platform/glfw-platform.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/msdf-gen/generator.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/rpc/rpc-server.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/rpc/event-loop-handler.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/rpc/stream-handler.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/rpc/socket-path.cpp
)

target_include_directories(yetty PRIVATE ${YETTY_INCLUDES})

target_compile_definitions(yetty PRIVATE
    ${YETTY_DEFINITIONS}
    YETTY_WEB=0
    YETTY_ANDROID=0
    YETTY_USE_PREBUILT_ATLAS=0
    YETTY_USE_FONTCONFIG=1
    YETTY_USE_FORKPTY=1
)

set_target_properties(yetty PROPERTIES ENABLE_EXPORTS TRUE)

# Fontconfig
find_package(PkgConfig REQUIRED)
pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
target_include_directories(yetty PRIVATE ${FONTCONFIG_INCLUDE_DIRS})
find_library(FONTCONFIG_STATIC_LIB libfontconfig.a PATHS /usr/lib/x86_64-linux-gnu /usr/lib REQUIRED)
find_library(EXPAT_STATIC_LIB libexpat.a PATHS /usr/lib/x86_64-linux-gnu /usr/lib REQUIRED)
find_library(UUID_STATIC_LIB libuuid.a PATHS /usr/lib/x86_64-linux-gnu /usr/lib REQUIRED)

target_link_libraries(yetty PRIVATE
    ${YETTY_LIBS}
    glfw
    glfw3webgpu
    imgui
    args
    ytrace::ytrace
    lz4_static
    uv_a
    yetty_gpu
    ${FONTCONFIG_STATIC_LIB}
    ${EXPAT_STATIC_LIB}
    ${UUID_STATIC_LIB}
    rt
    util
    ${FREETYPE_ALL_LIBS}
)

# CDB font generation
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/cdb-gen.cmake)

# Copy assets
add_custom_command(TARGET yetty POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)
configure_file(${CMAKE_SOURCE_DIR}/src/yetty/shaders/gpu-screen.wgsl ${CMAKE_BINARY_DIR}/gpu-screen.wgsl COPYONLY)

# Tests
option(YETTY_BUILD_YAST_TESTS "Build YAST parser tests" ON)
if(YETTY_BUILD_YAST_TESTS)
    enable_testing()
    add_subdirectory(${CMAKE_SOURCE_DIR}/test/ut/yast ${CMAKE_BINARY_DIR}/test/ut/yast)
endif()

option(YETTY_BUILD_YECHO_TESTS "Build YEcho parser tests" ON)
if(YETTY_BUILD_YECHO_TESTS)
    enable_testing()
    add_subdirectory(${CMAKE_SOURCE_DIR}/test/ut/yecho ${CMAKE_BINARY_DIR}/test/ut/yecho)
endif()
