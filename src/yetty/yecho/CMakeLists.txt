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

# Standalone yecho executable (desktop only)
if(NOT EMSCRIPTEN AND NOT YETTY_ANDROID)
    add_executable(yecho
        yecho.cpp
        yecho-parser.cpp
    )

    target_include_directories(yecho PRIVATE
        ${YETTY_ROOT}/include
        ${YETTY_ROOT}/src/yetty/yecho
        ${CMAKE_CURRENT_BINARY_DIR}
    )

    target_link_libraries(yecho PRIVATE args)
    target_compile_features(yecho PRIVATE cxx_std_23)
endif()
