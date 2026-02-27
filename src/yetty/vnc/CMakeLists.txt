# VNC-like remote display client/server

add_library(yetty_vnc STATIC
    vnc-client.cpp
    vnc-server.cpp
)

target_include_directories(yetty_vnc PUBLIC
    ${YETTY_ROOT}/src
    ${YETTY_ROOT}/include
    ${JPEG_INCLUDE_DIRS}
)

target_link_libraries(yetty_vnc PUBLIC
    webgpu
    turbojpeg-static
    ytrace::ytrace
)
