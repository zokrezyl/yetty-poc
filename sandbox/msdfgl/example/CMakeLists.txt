cmake_minimum_required(VERSION 3.16)

add_executable(msdfgl-example
    main.cpp
)

target_include_directories(msdfgl-example PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)

target_link_libraries(msdfgl-example
    PRIVATE
        msdfgl::webgpu
        glfw
        glfw3webgpu
        webgpu
        cdb
)

# Copy shader files to example build directory
add_custom_command(TARGET msdfgl-example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../shaders
        $<TARGET_FILE_DIR:msdfgl-example>/shaders
)
