# Shared PDF rendering logic (used by ypdf card)
add_library(yetty_ypdf STATIC
    pdf-content-parser.cpp
    pdf-renderer.cpp
)
target_include_directories(yetty_ypdf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${YETTY_ROOT}/include ${YETTY_ROOT}/src)
target_link_libraries(yetty_ypdf PUBLIC ytrace::ytrace yetty_ydraw yetty_font pdfio_lib)
