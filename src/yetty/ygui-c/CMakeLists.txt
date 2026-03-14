# YGui-C - Pure C widget library with libuv event loop

# YDraw C API wrapper (C++ -> C)
set(YDRAW_CAPI_WRAPPER ${CMAKE_CURRENT_SOURCE_DIR}/../ydraw/ydraw-capi-wrapper.gen.cpp)
set(YDRAW_CAPI_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../ydraw/ydraw-capi.gen.c)

add_library(ygui-c STATIC
    ygui_engine.c
    ygui_grid.c
    ygui_widgets.c
    ygui_render.c
    ygui_theme.c
    ygui_osc.c
    ${YDRAW_CAPI_SRC}
)

target_include_directories(ygui-c PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../ydraw
    ${YETTY_ROOT}/include
)

# Link libuv and ydraw dependencies
if(NOT EMSCRIPTEN)
    target_link_libraries(ygui-c PUBLIC uv_a)
endif()

# C11 standard for _Thread_local
set_target_properties(ygui-c PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
)

# Install header
install(FILES ygui.h DESTINATION include)

install(TARGETS ygui-c
    ARCHIVE DESTINATION lib
)
