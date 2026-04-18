# gpu-info - Dawn-based GPU information tool
add_executable(gpu-info gpu-info.cpp)

target_link_libraries(gpu-info PRIVATE webgpu)

target_compile_features(gpu-info PRIVATE cxx_std_17)
