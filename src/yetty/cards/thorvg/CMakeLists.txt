# ThorVG card - thin wrapper around ThorVgRenderer

add_library(yetty_card_thorvg STATIC
    thorvg.cpp
)

target_include_directories(yetty_card_thorvg PUBLIC
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
)

target_link_libraries(yetty_card_thorvg PUBLIC
    yetty_thorvg
    yetty_ydraw
    glm::glm
    webgpu
    yaml-cpp
    ytrace::ytrace
)
