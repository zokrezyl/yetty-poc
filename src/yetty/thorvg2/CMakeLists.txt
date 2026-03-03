# ThorVG2 - RenderMethod-based ThorVG integration

add_library(yetty_thorvg2 STATIC
    ydraw-render-method.cpp
)

target_include_directories(yetty_thorvg2 PUBLIC
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
)

# ThorVG internal headers for RenderMethod interface (v1.0.1 from CPM)
target_include_directories(yetty_thorvg2 PRIVATE
    ${thorvg_SOURCE_DIR}/src/renderer
    ${thorvg_SOURCE_DIR}/src/common
)

# Windows: prevent min/max macros from windows.h conflicting with std::min/max
target_compile_definitions(yetty_thorvg2 PRIVATE
    $<$<PLATFORM_ID:Windows>:NOMINMAX>
    $<$<PLATFORM_ID:Windows>:WIN32_LEAN_AND_MEAN>
)

target_link_libraries(yetty_thorvg2 PUBLIC
    yetty_ydraw
    thorvg_lib
    ytrace::ytrace
)
