# Existing standalone tests
add_executable(ygui_c_scene_bounds_test scene_bounds_test.c)
target_include_directories(ygui_c_scene_bounds_test PRIVATE
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw
)
add_test(NAME ygui_c_scene_bounds_test COMMAND ygui_c_scene_bounds_test)

add_executable(ygui_c_coord_transform_test coord_transform_test.c)
target_link_libraries(ygui_c_coord_transform_test PRIVATE m)
add_test(NAME ygui_c_coord_transform_test COMMAND ygui_c_coord_transform_test)

# =============================================================================
# New comprehensive tests (require ygui-c library)
# =============================================================================

# Widget unit tests
add_executable(ygui_c_widget_test widget_test.c)
target_link_libraries(ygui_c_widget_test PRIVATE ygui-c m)
add_test(NAME ygui_c_widget_test COMMAND ygui_c_widget_test)

# Spatial grid tests
add_executable(ygui_c_grid_test grid_test.c)
target_link_libraries(ygui_c_grid_test PRIVATE ygui-c m)
add_test(NAME ygui_c_grid_test COMMAND ygui_c_grid_test)

# Integration tests with mock terminal
add_executable(ygui_c_integration_test integration_test.c)
target_link_libraries(ygui_c_integration_test PRIVATE ygui-c m)
add_test(NAME ygui_c_integration_test COMMAND ygui_c_integration_test)

# OSC parsing tests
add_executable(ygui_c_osc_parsing_test osc_parsing_test.c)
target_link_libraries(ygui_c_osc_parsing_test PRIVATE ygui-c m)
add_test(NAME ygui_c_osc_parsing_test COMMAND ygui_c_osc_parsing_test)

# =============================================================================
# PTY-based tests (require libuv, test full event loop)
# =============================================================================

# PTY integration tests - uses real PTY for libuv event loop testing
add_executable(ygui_c_pty_test pty_test.c)
target_link_libraries(ygui_c_pty_test PRIVATE ygui-c m util)
add_test(NAME ygui_c_pty_test COMMAND ygui_c_pty_test)

# Dual-process tests - full terminal emulator + client architecture
add_executable(ygui_c_dual_process_test dual_process_test.c)
target_link_libraries(ygui_c_dual_process_test PRIVATE ygui-c m util)
add_test(NAME ygui_c_dual_process_test COMMAND ygui_c_dual_process_test)
