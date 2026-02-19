add_library(yetty_card_ydraw_maze STATIC ydraw-maze.cpp)
target_include_directories(yetty_card_ydraw_maze PUBLIC ${YETTY_ROOT}/include ${YETTY_ROOT}/src)
target_link_libraries(yetty_card_ydraw_maze PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace yetty_ydraw)
