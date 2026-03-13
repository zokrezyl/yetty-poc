# YGui-C - Pure C widget library

add_library(ygui-c STATIC
    ygui_engine.c
    ygui_grid.c
    ygui_widgets.c
    ygui_render.c
    ygui_theme.c
)

target_include_directories(ygui-c PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# C11 standard for _Thread_local
set_target_properties(ygui-c PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
)

# TODO: Link to ydraw-c when available
# target_link_libraries(ygui-c PRIVATE ydraw-c)

# Install header
install(FILES ygui.h DESTINATION include)

# Shared library for FFI bindings
add_library(ygui-c-shared SHARED
    ygui_engine.c
    ygui_grid.c
    ygui_widgets.c
    ygui_render.c
    ygui_theme.c
)

target_include_directories(ygui-c-shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(ygui-c-shared PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    OUTPUT_NAME ygui
    VERSION 0.1.0
    SOVERSION 0
)

install(TARGETS ygui-c-shared
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
