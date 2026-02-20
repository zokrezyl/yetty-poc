# WebAssembly (Emscripten) build target

include(${YETTY_ROOT}/build-tools/cmake/targets/shared.cmake)

# WebAssembly-specific libraries
include(${YETTY_ROOT}/build-tools/cmake/libs/imgui.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/lz4.cmake)

# CDB font generation (builds host tools automatically)
include(${YETTY_ROOT}/build-tools/cmake/cdb-gen.cmake)

# Copy runtime assets (fonts, etc.) to build directory
add_subdirectory(${YETTY_ROOT}/assets ${CMAKE_BINARY_DIR}/assets-build)

# Global definitions for all webasm targets (applied before add_subdirectory)
add_compile_definitions(YETTY_WEB=1 YETTY_ANDROID=0)

# Add src/yetty (builds libraries)
add_subdirectory(${YETTY_ROOT}/src/yetty ${CMAKE_BINARY_DIR}/src/yetty)

# Create executable with core sources + web platform
add_executable(yetty
    ${YETTY_CORE_SOURCES}
    ${YETTY_ROOT}/src/yetty/platform/web-platform.cpp
)

target_include_directories(yetty PRIVATE ${YETTY_INCLUDES})

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
    "-sEXPORTED_FUNCTIONS=['_main','_malloc','_free','_yetty_write','_yetty_key','_yetty_special_key','_yetty_read_input','_yetty_sync','_yetty_set_scale','_yetty_resize','_yetty_get_cols','_yetty_get_rows']"
)

target_compile_options(yetty PRIVATE --use-port=emdawnwebgpu)
set_target_properties(yetty PROPERTIES SUFFIX ".js")

target_link_libraries(yetty PRIVATE
    ${YETTY_LIBS}
    imgui
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

# Build toybox if source exists
if(EXISTS "${YETTY_ROOT}/tmp/toybox")
    add_custom_command(TARGET yetty POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E env BUILD_DIR=${CMAKE_BINARY_DIR} bash ${YETTY_ROOT}/build-tools/web/build-toybox-minimal.sh
        WORKING_DIRECTORY ${YETTY_ROOT}
    )
endif()

# Verify all required assets are present
add_custom_command(TARGET yetty POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DBUILD_DIR=${CMAKE_BINARY_DIR} -DTARGET_TYPE=webasm -P ${YETTY_ROOT}/build-tools/cmake/verify-assets.cmake
    COMMENT "Verifying build assets..."
)
