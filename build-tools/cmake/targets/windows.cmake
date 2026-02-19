# Windows desktop build target

include(${CMAKE_SOURCE_DIR}/build-tools/cmake/targets/shared.cmake)

# Windows-specific libraries
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/args.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/lz4.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/libuv.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/glfw.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/imgui.cmake)

# Add src/yetty (builds libraries)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/yetty ${CMAKE_BINARY_DIR}/src/yetty)

# Create executable with core sources + platform
add_executable(yetty
    ${YETTY_CORE_SOURCES}
    ${YETTY_DESKTOP_SOURCES}
    ${CMAKE_SOURCE_DIR}/src/yetty/platform/glfw-platform.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/msdf-gen/generator.cpp
)

target_include_directories(yetty PRIVATE ${YETTY_INCLUDES})

target_compile_definitions(yetty PRIVATE
    ${YETTY_DEFINITIONS}
    YETTY_WEB=0
    YETTY_ANDROID=0
    YETTY_USE_PREBUILT_ATLAS=0
    YETTY_USE_DIRECTWRITE=1
    YETTY_USE_CONPTY=1
    NOMINMAX
    WIN32_LEAN_AND_MEAN
)

set_target_properties(yetty PROPERTIES ENABLE_EXPORTS TRUE)

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
    dwrite
    ${FREETYPE_ALL_LIBS}
)

# CDB font generation
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/cdb-gen.cmake)

# Copy assets
add_custom_command(TARGET yetty POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)
configure_file(${CMAKE_SOURCE_DIR}/src/yetty/shaders/gpu-screen.wgsl ${CMAKE_BINARY_DIR}/gpu-screen.wgsl COPYONLY)
