# Demo 01: Button Click Test
add_executable(ygui-demo-01-button main.c)

target_link_libraries(ygui-demo-01-button PRIVATE ygui-c)

target_compile_features(ygui-demo-01-button PRIVATE c_std_11)
