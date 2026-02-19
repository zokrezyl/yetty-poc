add_library(yetty_card_ydraw_zoo STATIC ydraw-zoo.cpp)
target_include_directories(yetty_card_ydraw_zoo PUBLIC ${YETTY_ROOT}/include ${YETTY_ROOT}/src)
target_link_libraries(yetty_card_ydraw_zoo PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace yetty_ydraw)
