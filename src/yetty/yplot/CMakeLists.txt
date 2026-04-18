# yplot library - High-level plot system built on yfsvm and ydraw
add_library(yetty_yplot STATIC
    yplot.cpp
)

target_include_directories(yetty_yplot PUBLIC
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
)

target_link_libraries(yetty_yplot PUBLIC
    yetty_yfsvm
    yetty_yast
    yetty_ydraw
)
