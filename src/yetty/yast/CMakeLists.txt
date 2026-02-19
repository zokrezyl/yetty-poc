# yast library
add_library(yetty_yast STATIC
    yast.cpp
)

target_include_directories(yetty_yast PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)
