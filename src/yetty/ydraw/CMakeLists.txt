# ydraw library
add_library(yetty_ydraw STATIC
    ydraw-builder.cpp
    ydraw-buffer.cpp
    animation.cpp
    yaml2ydraw.cpp
)

target_include_directories(yetty_ydraw PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(yetty_ydraw PUBLIC yaml-cpp glm::glm webgpu ytrace::ytrace)
