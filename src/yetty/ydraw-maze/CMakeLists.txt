# Shared maze rendering logic (used by ydraw-maze card and standalone tools)
target_sources(yetty PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/maze-renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/maze-renderer.cpp
)
target_include_directories(yetty PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
