# ygui card - retained mode UI widgets

add_library(yetty_card_ygui STATIC
    ygui.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ygui/ygui-engine.cpp
)
target_include_directories(yetty_card_ygui PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/yetty/ygui
)
target_link_libraries(yetty_card_ygui PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace yetty_ydraw)
