# Shared PDF rendering logic (used by ypdf card and pdf2ydraw tool)
target_sources(yetty PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/pdf-content-parser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pdf-content-parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pdf-renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pdf-renderer.cpp
)

target_include_directories(yetty PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
