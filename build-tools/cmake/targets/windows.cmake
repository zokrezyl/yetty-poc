# Windows desktop build target

include(${YETTY_ROOT}/build-tools/cmake/targets/shared.cmake)

# Windows-specific libraries
include(${YETTY_ROOT}/build-tools/cmake/libs/glfw.cmake)

# Add src/yetty (builds libraries)
add_subdirectory(${YETTY_ROOT}/src/yetty ${CMAKE_BINARY_DIR}/src/yetty)

# Desktop-specific subdirectories
add_subdirectory(${YETTY_ROOT}/src/yetty/gpu ${CMAKE_BINARY_DIR}/src/yetty/gpu)

# Create executable with core sources + platform
add_executable(yetty
    ${YETTY_CORE_SOURCES}
    ${YETTY_DESKTOP_SOURCES}
    ${YETTY_ROOT}/src/yetty/platform/windows-platform.cpp
)

target_include_directories(yetty PRIVATE ${YETTY_INCLUDES} ${YETTY_RENDERER_INCLUDES})

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
    ytrace::ytrace
    lz4_static
    uv_a
    yetty_gpu
    dwrite
    ${FREETYPE_ALL_LIBS}
)

# CDB font generation
include(${YETTY_ROOT}/build-tools/cmake/cdb-gen.cmake)

# Copy assets
add_custom_command(TARGET yetty POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${YETTY_ROOT}/assets ${CMAKE_BINARY_DIR}/assets
)
configure_file(${YETTY_ROOT}/src/yetty/shaders/gpu-screen.wgsl ${CMAKE_BINARY_DIR}/gpu-screen.wgsl COPYONLY)
