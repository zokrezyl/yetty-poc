add_executable(msdf-gen-cdb main.cpp)

target_link_libraries(msdf-gen-cdb PRIVATE
    msdf-wgsl
    glfw
    glfw3webgpu
    cdb
)

target_include_directories(msdf-gen-cdb PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)
