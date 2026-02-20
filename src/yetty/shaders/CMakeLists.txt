# Copy all shaders to build directory
set(SHADER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SHADER_DST_DIR ${CMAKE_BINARY_DIR}/assets/shaders)

# Collect all shader files recursively
file(GLOB_RECURSE SHADER_FILES
    ${SHADER_SRC_DIR}/*.wgsl
)

# Create custom target to copy shaders
add_custom_target(copy-shaders ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_DST_DIR}
    COMMENT "Copying shaders to build directory"
)

foreach(SHADER_FILE ${SHADER_FILES})
    # Get relative path from shader source dir
    file(RELATIVE_PATH REL_PATH ${SHADER_SRC_DIR} ${SHADER_FILE})
    get_filename_component(REL_DIR ${REL_PATH} DIRECTORY)

    add_custom_command(TARGET copy-shaders POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_DST_DIR}/${REL_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHADER_FILE} ${SHADER_DST_DIR}/${REL_PATH}
    )
endforeach()

# Also copy the main gpu-screen.wgsl to root for backward compat
add_custom_command(TARGET copy-shaders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHADER_SRC_DIR}/gpu-screen.wgsl ${CMAKE_BINARY_DIR}/assets/gpu-screen.wgsl
)
