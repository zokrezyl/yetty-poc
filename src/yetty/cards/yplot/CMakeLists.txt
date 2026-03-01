# yplot card - Plot card composing YDrawBuffer and yfsvm
add_library(yetty_card_yplot STATIC
    yplot-card.cpp
)

target_include_directories(yetty_card_yplot PUBLIC
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
)

target_link_libraries(yetty_card_yplot PUBLIC
    glm::glm
    webgpu
    ytrace::ytrace
    yetty_yplot
    yetty_yfsvm
    yetty_ydraw
)
