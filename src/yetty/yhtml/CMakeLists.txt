# Shared HTML rendering logic (used by yhtml card)
add_library(yetty_yhtml STATIC
    html-container.cpp
    html-renderer.cpp
    http-fetcher.cpp
)
target_include_directories(yetty_yhtml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${YETTY_ROOT}/include ${YETTY_ROOT}/src)
target_link_libraries(yetty_yhtml PUBLIC ytrace::ytrace yetty_ydraw yetty_font litehtml cpr::cpr stb)
