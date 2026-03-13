# yecho - text with embedded glyphs
# Parses @glyph and {attrs: styled} syntax, outputs UTF-8

# Generate glyph registry header at configure time
set(GLYPH_REGISTRY_HEADER "${CMAKE_CURRENT_BINARY_DIR}/glyph-registry-generated.h")
set(SHADER_DIR "${YETTY_ROOT}/src/yetty/shaders")

find_package(Python3 COMPONENTS Interpreter REQUIRED)

execute_process(
    COMMAND ${Python3_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/gen-glyph-registry.py
        ${SHADER_DIR}
        ${GLYPH_REGISTRY_HEADER}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE GEN_RESULT
    OUTPUT_VARIABLE GEN_OUTPUT
    ERROR_VARIABLE GEN_ERROR
)

if(NOT GEN_RESULT EQUAL 0)
    message(WARNING "Failed to generate glyph registry: ${GEN_ERROR}")
else()
    message(STATUS "Glyph registry: ${GEN_OUTPUT}")
endif()

# Library for use by yetty
add_library(yetty_yecho STATIC
    yecho-parser.cpp
)

target_include_directories(yetty_yecho PUBLIC
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(yetty_yecho PUBLIC ytrace::ytrace)

# Standalone yecho executable (all platforms)
add_executable(yecho
    yecho.cpp
    yecho-parser.cpp
)

target_include_directories(yecho PRIVATE
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src/yetty/yecho
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(yecho PRIVATE args ytrace::ytrace)
target_compile_features(yecho PRIVATE cxx_std_23)

# Install yecho to Android jniLibs as libyecho.so
if(YETTY_ANDROID AND ANDROID_BUILD_DIR)
    add_custom_command(TARGET yecho POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:yecho>
                "${ANDROID_BUILD_DIR}/jniLibs/${ANDROID_ABI}/libyecho.so"
        COMMENT "Copying yecho to Android jniLibs"
    )
endif()
