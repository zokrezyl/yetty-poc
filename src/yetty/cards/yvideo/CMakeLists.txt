# YVideo card - AV1 video playback
add_library(yetty_card_yvideo STATIC yvideo.cpp)
target_include_directories(yetty_card_yvideo PUBLIC ${YETTY_ROOT}/include ${YETTY_ROOT}/src)
target_link_libraries(yetty_card_yvideo PUBLIC
    glm::glm
    webgpu
    yaml-cpp
    ytrace::ytrace
    yetty_yvideo
)
