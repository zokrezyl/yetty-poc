# CDB-based MSDF generator tool - works on all platforms via CdbWriter wrapper
add_executable(msdf-gen-cdb main.cpp)

target_link_libraries(msdf-gen-cdb PRIVATE
    msdf-wgsl
    glfw
    glfw3webgpu
    cdb-wrapper
    ${FREETYPE_ALL_LIBS}
)

target_include_directories(msdf-gen-cdb PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Copy shader next to executable
add_custom_command(TARGET msdf-gen-cdb POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/../shaders/msdf_gen.wgsl
        $<TARGET_FILE_DIR:msdf-gen-cdb>/msdf_gen.wgsl
)
