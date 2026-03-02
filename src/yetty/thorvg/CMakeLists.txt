# ThorVG renderer - converts ThorVG scene graph to YDraw primitives

add_library(yetty_thorvg STATIC
    thorvg-renderer.cpp
)

target_include_directories(yetty_thorvg PUBLIC
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
)

# Windows: prevent min/max macros from windows.h conflicting with std::min/max
target_compile_definitions(yetty_thorvg PRIVATE
    $<$<PLATFORM_ID:Windows>:NOMINMAX>
    $<$<PLATFORM_ID:Windows>:WIN32_LEAN_AND_MEAN>
)

target_link_libraries(yetty_thorvg PUBLIC
    thorvg_lib
    ytrace::ytrace
)
