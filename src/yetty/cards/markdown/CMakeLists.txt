add_library(yetty_card_markdown STATIC markdown.cpp)
target_include_directories(yetty_card_markdown PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(yetty_card_markdown PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace)
