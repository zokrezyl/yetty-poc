# ThorVG renderer - converts ThorVG scene graph to YDraw primitives

add_library(yetty_thorvg STATIC
    thorvg-renderer.cpp
)

target_include_directories(yetty_thorvg PUBLIC
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
)

target_link_libraries(yetty_thorvg PUBLIC
    thorvg_lib
    ytrace::ytrace
)
