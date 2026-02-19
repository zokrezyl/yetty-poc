add_library(yetty_card_ytext STATIC ytext.cpp)
target_include_directories(yetty_card_ytext PUBLIC ${YETTY_ROOT}/include ${YETTY_ROOT}/src)
target_link_libraries(yetty_card_ytext PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace)
