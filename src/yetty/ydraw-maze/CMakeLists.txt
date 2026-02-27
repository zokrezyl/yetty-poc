# Shared maze rendering logic (used by ydraw-maze card and standalone tools)
set(YETTY_MAZE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/maze-renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/maze-renderer.cpp
    PARENT_SCOPE
)
set(YETTY_MAZE_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
    PARENT_SCOPE
)
