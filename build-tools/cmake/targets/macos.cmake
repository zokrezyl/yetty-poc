# macOS desktop build target

include(${YETTY_ROOT}/build-tools/cmake/targets/shared.cmake)

# macOS-specific libraries
include(${YETTY_ROOT}/build-tools/cmake/libs/glfw.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/libjpeg-turbo.cmake)
include(${YETTY_ROOT}/build-tools/cmake/Libmagic.cmake)

# Add src/yetty (builds libraries)
add_subdirectory(${YETTY_ROOT}/src/yetty ${CMAKE_BINARY_DIR}/src/yetty)

# Desktop-specific subdirectories
add_subdirectory(${YETTY_ROOT}/src/yetty/gpu ${CMAKE_BINARY_DIR}/src/yetty/gpu)
add_subdirectory(${YETTY_ROOT}/src/yetty/client ${CMAKE_BINARY_DIR}/src/yetty/client)
add_subdirectory(${YETTY_ROOT}/src/yetty/ytop ${CMAKE_BINARY_DIR}/src/yetty/ytop)

# Platform manager sources (new architecture)
# Note: event-loop is included via yetty_base
set(YETTY_PLATFORM_SOURCES
    ${YETTY_ROOT}/src/yetty/platform/init-manager/glfw.cpp
    ${YETTY_ROOT}/src/yetty/platform/shared/glfw-window-singleton.cpp
    ${YETTY_ROOT}/src/yetty/platform/surface-manager/glfw.cpp
    ${YETTY_ROOT}/src/yetty/platform/pty-manager/unix.cpp
    ${YETTY_ROOT}/src/yetty/platform/pty-reader/unix.cpp
    ${YETTY_ROOT}/src/yetty/platform/fs-path-manager/unix.cpp
    ${YETTY_ROOT}/src/yetty/platform/clipboard-manager/glfw.cpp
    ${YETTY_ROOT}/src/yetty/platform/webgpu-manager/macos.cpp
)

# Create executable with core sources + platform
add_executable(yetty
    ${YETTY_CORE_SOURCES}
    ${YETTY_DESKTOP_SOURCES}
    ${YETTY_PLATFORM_SOURCES}
)

target_include_directories(yetty PRIVATE ${YETTY_INCLUDES} ${YETTY_RENDERER_INCLUDES} ${JPEG_INCLUDE_DIRS} ${BROTLI_INCLUDE_DIR})

# Embed all assets (logo, shaders, fonts, CDB files)
yetty_embed_assets(yetty)

# Dummy targets for dependency tracking (legacy)
add_custom_target(copy-shaders-for-incbin)
add_custom_target(copy-fonts-for-incbin)

target_compile_definitions(yetty PRIVATE
    ${YETTY_DEFINITIONS}
    YETTY_WEB=0
    YETTY_ANDROID=0
    YETTY_USE_PREBUILT_ATLAS=0
    YETTY_USE_CORETEXT=1
    YETTY_USE_FORKPTY=1
    YETTY_HAS_VNC=1
)

set_target_properties(yetty PROPERTIES ENABLE_EXPORTS TRUE)

# Core Text for font discovery
find_library(CORETEXT_LIBRARY CoreText REQUIRED)
find_library(COREFOUNDATION_LIBRARY CoreFoundation REQUIRED)

target_link_libraries(yetty PRIVATE
    ${YETTY_LIBS}
    glfw
    glfw3webgpu
    args
    ytrace::ytrace
    lz4_static
    uv_a
    yetty_gpu
    turbojpeg-static
    yetty_vnc
    ${CORETEXT_LIBRARY}
    ${COREFOUNDATION_LIBRARY}
    ${FREETYPE_ALL_LIBS}
    ${BROTLIDEC_LIBRARIES}
)

# CDB font generation
include(${YETTY_ROOT}/build-tools/cmake/cdb-gen.cmake)

# Copy runtime assets to build directory
add_subdirectory(${YETTY_ROOT}/assets ${CMAKE_BINARY_DIR}/assets-build)

# Ensure all runtime assets are in build output before yetty
add_dependencies(yetty generate-cdb copy-shaders copy-assets copy-shaders-for-incbin copy-fonts-for-incbin)

# Verify all required assets are present
add_custom_command(TARGET yetty POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DBUILD_DIR=${CMAKE_BINARY_DIR} -DTARGET_TYPE=desktop -P ${YETTY_ROOT}/build-tools/cmake/verify-assets.cmake
    COMMENT "Verifying build assets..."
)
