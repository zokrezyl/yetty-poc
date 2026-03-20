add_executable(yslides main.cpp)

target_link_libraries(yslides PRIVATE
    yetty_yrich
    yetty_ydraw
    args
    uv_a
)

target_include_directories(yslides PRIVATE
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
)
