# Demo 03: All Widgets
add_executable(ygui-demo-03-widgets main.c)

target_link_libraries(ygui-demo-03-widgets PRIVATE ygui-c)

target_compile_features(ygui-demo-03-widgets PRIVATE c_std_11)
