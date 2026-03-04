# yvideo - video decoder library using dav1d (AV1) and openh264 (H.264)
add_library(yetty_yvideo STATIC
    yvideo-decoder.cpp
)

target_include_directories(yetty_yvideo PUBLIC
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
)

target_link_libraries(yetty_yvideo PUBLIC
    dav1d
    openh264
    minimp4
    ytrace::ytrace
)
