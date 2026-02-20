add_library(yetty_card_diagram STATIC diagram.cpp)
target_include_directories(yetty_card_diagram PUBLIC ${YETTY_ROOT}/include ${YETTY_ROOT}/src)
target_link_libraries(yetty_card_diagram PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace yetty_ydraw yetty_diagram)
