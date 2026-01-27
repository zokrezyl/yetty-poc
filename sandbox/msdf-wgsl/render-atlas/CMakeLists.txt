add_executable(msdf-render-atlas main.cpp)

target_link_libraries(msdf-render-atlas PRIVATE
    msdf-wgsl
    glfw
    glfw3webgpu
    webgpu
)

target_include_directories(msdf-render-atlas PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)
