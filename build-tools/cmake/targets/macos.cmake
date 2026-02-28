# macOS desktop build target

include(${YETTY_ROOT}/build-tools/cmake/targets/shared.cmake)

# macOS-specific libraries
include(${YETTY_ROOT}/build-tools/cmake/libs/glfw.cmake)
include(${YETTY_ROOT}/build-tools/cmake/TreeSitter.cmake)
include(${YETTY_ROOT}/build-tools/cmake/Libmagic.cmake)

# Add src/yetty (builds libraries)
add_subdirectory(${YETTY_ROOT}/src/yetty ${CMAKE_BINARY_DIR}/src/yetty)

# Desktop-specific subdirectories
add_subdirectory(${YETTY_ROOT}/src/yetty/gpu ${CMAKE_BINARY_DIR}/src/yetty/gpu)
add_subdirectory(${YETTY_ROOT}/src/yetty/client ${CMAKE_BINARY_DIR}/src/yetty/client)
add_subdirectory(${YETTY_ROOT}/src/yetty/ytop ${CMAKE_BINARY_DIR}/src/yetty/ytop)

# Create executable with core sources + platform
add_executable(yetty
    ${YETTY_CORE_SOURCES}
    ${YETTY_DESKTOP_SOURCES}
    ${YETTY_ROOT}/src/yetty/platform/glfw-platform.cpp
)

target_include_directories(yetty PRIVATE ${YETTY_INCLUDES} ${YETTY_RENDERER_INCLUDES})

target_compile_definitions(yetty PRIVATE
    ${YETTY_DEFINITIONS}
    YETTY_WEB=0
    YETTY_ANDROID=0
    YETTY_USE_PREBUILT_ATLAS=0
    YETTY_USE_CORETEXT=1
    YETTY_USE_FORKPTY=1
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
    ${CORETEXT_LIBRARY}
    ${COREFOUNDATION_LIBRARY}
    ${FREETYPE_ALL_LIBS}
)

# CDB font generation
include(${YETTY_ROOT}/build-tools/cmake/cdb-gen.cmake)

# Copy runtime assets to build directory
add_subdirectory(${YETTY_ROOT}/assets ${CMAKE_BINARY_DIR}/assets-build)

# Ensure all runtime assets are in build output before yetty
add_dependencies(yetty generate-cdb copy-shaders copy-assets)

# Verify all required assets are present
add_custom_command(TARGET yetty POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DBUILD_DIR=${CMAKE_BINARY_DIR} -DTARGET_TYPE=desktop -P ${YETTY_ROOT}/build-tools/cmake/verify-assets.cmake
    COMMENT "Verifying build assets..."
)
