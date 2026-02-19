add_library(yetty_card_qrcode STATIC qrcode.cpp qr-encoder.cpp)
target_include_directories(yetty_card_qrcode PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(yetty_card_qrcode PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace)
