# yetty base utilities
add_library(yetty_base STATIC
    event-loop.cpp
)

target_include_directories(yetty_base PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(yetty_base PUBLIC ytrace::ytrace yaml-cpp)

if(NOT EMSCRIPTEN)
    target_link_libraries(yetty_base PUBLIC uv_a)
endif()
