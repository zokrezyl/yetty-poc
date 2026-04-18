cmake_minimum_required(VERSION 3.20)
project(webgpu-test)

set(CMAKE_CXX_STANDARD 20)

add_executable(cell-test cell-test.cpp)

if(EMSCRIPTEN)
    target_compile_options(cell-test PRIVATE "SHELL:--use-port=emdawnwebgpu")
    target_link_options(cell-test PRIVATE
        "--use-port=emdawnwebgpu"
        "-sASYNCIFY=1"
        "-sALLOW_MEMORY_GROWTH=1"
    )
    set_target_properties(cell-test PROPERTIES SUFFIX ".js")
endif()
