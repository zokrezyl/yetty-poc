add_library(yetty_card_ydraw STATIC ydraw.cpp)
target_include_directories(yetty_card_ydraw PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(yetty_card_ydraw PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace lz4_static yetty_ydraw)
