# Card Runner - standalone interactive card renderer
# Usage: card-runner <card-name> "<args>"
# Example: card-runner yplot "{f=sin(x) @range=-pi..pi,-1..1 @grid=on}"

add_executable(card-runner main.cpp)

target_include_directories(card-runner PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_compile_definitions(card-runner PRIVATE
    YETTY_SHADERS_DIR="${CMAKE_BINARY_DIR}/assets/shaders"
    YETTY_ASSETS_DIR="${CMAKE_BINARY_DIR}/assets"
)

target_link_libraries(card-runner PRIVATE
    # WebGPU + GLFW
    webgpu
    glfw
    glfw3webgpu

    # Utils
    ytrace::ytrace
    args
)

target_compile_features(card-runner PRIVATE cxx_std_23)
