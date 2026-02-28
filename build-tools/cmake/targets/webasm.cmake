# WebAssembly (Emscripten) build target

include(${YETTY_ROOT}/build-tools/cmake/targets/shared.cmake)

# CDB font generation (builds host tools automatically)
include(${YETTY_ROOT}/build-tools/cmake/cdb-gen.cmake)

# Copy runtime assets (fonts, etc.) to build directory
add_subdirectory(${YETTY_ROOT}/assets ${CMAKE_BINARY_DIR}/assets-build)

# Global definitions for all webasm targets (applied before add_subdirectory)
add_compile_definitions(YETTY_WEB=1 YETTY_ANDROID=0)

# Set shader directory path for web (used by card libraries)
set(YETTY_SHADERS_DIR "/assets/shaders" CACHE STRING "Shader directory path")

# Add src/yetty (builds libraries)
add_subdirectory(${YETTY_ROOT}/src/yetty ${CMAKE_BINARY_DIR}/src/yetty)

# Create executable with core sources + web platform
add_executable(yetty
    ${YETTY_CORE_SOURCES}
    ${YETTY_ROOT}/src/yetty/platform/web-platform.cpp
)

# JSLinux integration (downloads and copies files)
add_subdirectory(${YETTY_ROOT}/build-tools/jslinux ${CMAKE_BINARY_DIR}/jslinux-build)

target_include_directories(yetty PRIVATE ${YETTY_INCLUDES} ${YETTY_RENDERER_INCLUDES})

add_dependencies(yetty generate-cdb copy-shaders copy-assets)

target_compile_definitions(yetty PRIVATE
    ${YETTY_DEFINITIONS}
    YETTY_WEB=1
    YETTY_ANDROID=0
    YETTY_USE_PREBUILT_ATLAS=1
    YTRACE_ENABLED=1
    YTRACE_NO_CONTROL_SOCKET=1
    YTRACE_USE_SPDLOG=1
    YETTY_ASSETS_DIR="/assets"
    YETTY_SHADERS_DIR="/assets/shaders"
)

target_include_directories(yetty PRIVATE ${ytrace_SOURCE_DIR}/include ${spdlog_SOURCE_DIR}/include)

target_link_options(yetty PRIVATE
    -sUSE_GLFW=3
    --use-port=emdawnwebgpu
    -sASYNCIFY
    -sASYNCIFY_STACK_SIZE=65536
    -sALLOW_MEMORY_GROWTH
    -sWASM_BIGINT
    -sFILESYSTEM=1
    "--preload-file=${CMAKE_BINARY_DIR}/assets@/assets"
    "--preload-file=${YETTY_ROOT}/demo@/demo"
    "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap','UTF8ToString','stringToUTF8','FS','ENV','HEAPU8']"
    "-sEXPORTED_FUNCTIONS=['_main','_malloc','_free','_yetty_write','_yetty_key','_yetty_special_key','_yetty_read_input','_yetty_sync','_yetty_set_scale','_yetty_resize','_yetty_get_cols','_yetty_get_rows','_webpty_on_data']"
)

target_compile_options(yetty PRIVATE --use-port=emdawnwebgpu)
set_target_properties(yetty PROPERTIES SUFFIX ".js")

target_link_libraries(yetty PRIVATE
    ${YETTY_LIBS}
    Freetype::Freetype
    zlibstatic
)

# Remove stamp file that can cause issues with Emscripten file packaging
add_custom_command(TARGET yetty PRE_LINK
    COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_BINARY_DIR}/assets/fonts-cdb/.cdb_generated
    COMMENT "Removing CDB stamp file before linking"
)

# Copy web files
add_custom_command(TARGET yetty POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${YETTY_ROOT}/build-tools/web/index.html ${CMAKE_BINARY_DIR}/index.html
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${YETTY_ROOT}/build-tools/web/serve.py ${CMAKE_BINARY_DIR}/serve.py
)

# Copy JSLinux files to build output
add_custom_command(TARGET yetty POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/jslinux
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/jslinux-build/jslinux ${CMAKE_BINARY_DIR}/jslinux
    COMMENT "Copying JSLinux files..."
)

# Toybox: run build-tools/web/build-toybox-minimal.sh manually if needed

# Generate pre-computed demo script outputs
add_custom_target(generate-demo-outputs
    COMMAND ${CMAKE_COMMAND}
        -DYETTY_ROOT=${YETTY_ROOT}
        -DOUTPUT_DIR=${CMAKE_BINARY_DIR}
        -P ${YETTY_ROOT}/build-tools/cmake/generate-demo-outputs.cmake
    COMMENT "Generating demo script outputs..."
)

add_dependencies(yetty generate-demo-outputs)

# Verify all required assets are present
add_custom_command(TARGET yetty POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DBUILD_DIR=${CMAKE_BINARY_DIR} -DTARGET_TYPE=webasm -P ${YETTY_ROOT}/build-tools/cmake/verify-assets.cmake
    COMMENT "Verifying build assets..."
)
