# Demo 02: Coordinate Debug
add_executable(ygui-demo-02-coord main.c)

target_link_libraries(ygui-demo-02-coord PRIVATE ygui-c)

target_compile_features(ygui-demo-02-coord PRIVATE c_std_11)
