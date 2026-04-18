cmake_minimum_required(VERSION 3.16)

add_executable(msdfgl-render-atlas
    main.cpp
)

target_link_libraries(msdfgl-render-atlas
    PRIVATE
        msdfgl-webgpu
        webgpu
        glfw
        glfw3webgpu
)

target_include_directories(msdfgl-render-atlas PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)
