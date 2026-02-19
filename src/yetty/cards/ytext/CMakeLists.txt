add_library(yetty_card_ytext STATIC ytext.cpp)
target_include_directories(yetty_card_ytext PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(yetty_card_ytext PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace)
