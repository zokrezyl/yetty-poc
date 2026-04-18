add_library(yetty_card_texture STATIC
    texture-provider.cpp
    providers/image.cpp
)
target_include_directories(yetty_card_texture PUBLIC ${YETTY_ROOT}/include ${YETTY_ROOT}/src)
target_link_libraries(yetty_card_texture PUBLIC glm::glm stb webgpu yaml-cpp ytrace::ytrace)
