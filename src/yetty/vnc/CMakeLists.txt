# VNC-like remote display client/server

add_library(yetty_vnc STATIC
    vnc-client.cpp
    vnc-server.cpp
    vnc-view.cpp
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
    yetty_base
)

# yvideo provides H.264/AV1 codec support (not available on EMSCRIPTEN)
if(NOT EMSCRIPTEN)
    target_link_libraries(yetty_vnc PUBLIC yetty_yvideo)
    target_compile_definitions(yetty_vnc PRIVATE YETTY_HAS_YVIDEO=1)
endif()

if(WIN32)
    target_compile_definitions(yetty_vnc PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
    target_link_libraries(yetty_vnc PUBLIC ws2_32)
endif()

# Test utilities (desktop only - require GLFW)
if(NOT YETTY_ANDROID AND NOT EMSCRIPTEN)
    add_subdirectory(test)
endif()
