# ygui card - retained mode UI widgets

add_library(yetty_card_ygui STATIC
    ygui.cpp
    ${YETTY_ROOT}/src/yetty/ygui/ygui-engine.cpp
    ${YETTY_ROOT}/src/yetty/ygui/data-types.cpp
)
target_include_directories(yetty_card_ygui PUBLIC
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
    ${YETTY_ROOT}/src/yetty/ygui
)
target_link_libraries(yetty_card_ygui PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace yetty_ydraw)
