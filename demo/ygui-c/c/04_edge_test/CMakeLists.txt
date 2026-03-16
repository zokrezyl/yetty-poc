# Demo 04: Edge Test
add_executable(ygui-demo-04-edge main.c)

target_link_libraries(ygui-demo-04-edge PRIVATE ygui-c)

target_compile_features(ygui-demo-04-edge PRIVATE c_std_11)
