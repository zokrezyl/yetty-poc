# YThorVG card - uses RenderMethod-based ThorVG integration

add_library(yetty_card_ythorvg STATIC
    ythorvg.cpp
)

target_include_directories(yetty_card_ythorvg PUBLIC
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
)

# ThorVG internal headers for Paint interface
target_include_directories(yetty_card_ythorvg PRIVATE
    ${thorvg_SOURCE_DIR}/src/renderer
    ${thorvg_SOURCE_DIR}/src/common
)

# Windows: prevent min/max macros
target_compile_definitions(yetty_card_ythorvg PRIVATE
    $<$<PLATFORM_ID:Windows>:NOMINMAX>
    $<$<PLATFORM_ID:Windows>:WIN32_LEAN_AND_MEAN>
)

target_link_libraries(yetty_card_ythorvg PUBLIC
    yetty_ythorvg
    yetty_ydraw
    yetty_base
    thorvg_lib
    glm::glm
    webgpu
    yaml-cpp
    ytrace::ytrace
)
