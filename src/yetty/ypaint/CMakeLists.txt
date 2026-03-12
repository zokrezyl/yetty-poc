# ypaint library (scrolling SDF renderer)
add_library(yetty_ypaint STATIC
    canvas.cpp
    painter.cpp
    ypaint-buffer.cpp
    animation.cpp
    yaml2ypaint.cpp
    triangulate.cpp
)

target_include_directories(yetty_ypaint PUBLIC
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
)

target_link_libraries(yetty_ypaint PUBLIC yaml-cpp glm::glm webgpu ytrace::ytrace)
