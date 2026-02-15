# Core ygui library - retained mode UI widgets
# Used by both ygui card and external tools

target_sources(yetty PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ygui-widgets.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ygui-parser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ygui-parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ygui-renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ygui-renderer.cpp
)
