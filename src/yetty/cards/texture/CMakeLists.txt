add_library(yetty_card_texture STATIC
    texture-provider.cpp
    providers/image.cpp
)
target_include_directories(yetty_card_texture PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(yetty_card_texture PUBLIC glm::glm stb webgpu yaml-cpp ytrace::ytrace)
