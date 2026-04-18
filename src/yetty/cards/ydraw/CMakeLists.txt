add_library(yetty_card_ydraw STATIC ydraw.cpp)
target_include_directories(yetty_card_ydraw PUBLIC ${YETTY_ROOT}/include ${YETTY_ROOT}/src)
target_link_libraries(yetty_card_ydraw PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace lz4_static yetty_ydraw)
