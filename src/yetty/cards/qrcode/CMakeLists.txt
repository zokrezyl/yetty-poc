add_library(yetty_card_qrcode STATIC qrcode.cpp qr-encoder.cpp)
target_include_directories(yetty_card_qrcode PUBLIC ${YETTY_ROOT}/include ${YETTY_ROOT}/src)
target_link_libraries(yetty_card_qrcode PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace)
