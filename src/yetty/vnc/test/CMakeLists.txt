# VNC Test Server - Terminal rendering with vterm + FreeType
add_executable(vnc-test-server vnc-test-server.cpp)

target_link_libraries(vnc-test-server PRIVATE
    yetty_vnc
    yetty_base
    ytrace::ytrace
    args
    webgpu
    vterm
    freetype
)

target_include_directories(vnc-test-server PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${FREETYPE_INCLUDE_DIR}
)
