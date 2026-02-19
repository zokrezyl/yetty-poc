add_executable(ymux main.cpp ymux.cpp pane.cpp)

target_link_libraries(ymux PRIVATE vterm util)

target_compile_features(ymux PRIVATE cxx_std_17)
