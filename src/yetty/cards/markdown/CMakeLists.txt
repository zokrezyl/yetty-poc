add_library(yetty_card_markdown STATIC markdown.cpp)
target_include_directories(yetty_card_markdown PUBLIC ${YETTY_ROOT}/include ${YETTY_ROOT}/src)
target_link_libraries(yetty_card_markdown PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace)
