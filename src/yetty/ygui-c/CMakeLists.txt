# YGui-C - Pure C widget library with libuv event loop
# Outputs: libygui-c.a (static) and libygui.so (shared)
# Both are self-contained with ALL dependencies linked in (including libuv)

set(YDRAW_CAPI_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../ydraw/ydraw-capi.gen.c)

set(YGUI_SOURCES
    ygui_engine.c
    ygui_grid.c
    ygui_widgets.c
    ygui_render.c
    ygui_theme.c
    ygui_osc.c
    ${YDRAW_CAPI_SRC}
)

set(YGUI_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../ydraw
    ${YETTY_ROOT}/include
)

# =============================================================================
# Static library - links libuv statically
# =============================================================================
add_library(ygui-c STATIC ${YGUI_SOURCES})
target_include_directories(ygui-c PUBLIC ${YGUI_INCLUDES})

if(NOT EMSCRIPTEN)
    target_link_libraries(ygui-c PUBLIC uv_a)
endif()

set_target_properties(ygui-c PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
)

# =============================================================================
# Shared library - links libuv STATICALLY into the .so (no external deps)
# =============================================================================
add_library(ygui-c-shared SHARED ${YGUI_SOURCES})
target_include_directories(ygui-c-shared PUBLIC ${YGUI_INCLUDES})

if(NOT EMSCRIPTEN)
    # Link libuv STATIC library into the shared library
    # Use PRIVATE so libuv symbols are embedded, not exported as dependency
    target_link_libraries(ygui-c-shared PRIVATE uv_a)
endif()

set_target_properties(ygui-c-shared PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    OUTPUT_NAME ygui
    VERSION 0.2.0
    SOVERSION 0
)

# Install
install(FILES ygui.h DESTINATION include)
install(TARGETS ygui-c ARCHIVE DESTINATION lib)
install(TARGETS ygui-c-shared LIBRARY DESTINATION lib)
