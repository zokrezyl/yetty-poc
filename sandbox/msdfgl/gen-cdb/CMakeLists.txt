cmake_minimum_required(VERSION 3.16)

add_executable(msdfgl-gen-cdb
    main.cpp
)

target_include_directories(msdfgl-gen-cdb PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)

target_link_libraries(msdfgl-gen-cdb
    PRIVATE
        msdfgl::webgpu
        glfw
        glfw3webgpu
        webgpu
        cdb
)
