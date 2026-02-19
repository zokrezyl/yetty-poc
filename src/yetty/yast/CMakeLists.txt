# yast library
add_library(yetty_yast STATIC
    yast.cpp
)

target_include_directories(yetty_yast PUBLIC
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
)
