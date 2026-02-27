# Shared zoo rendering logic (used by ydraw-zoo card and standalone tools)
set(YETTY_ZOO_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/zoo-renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/zoo-renderer.cpp
    PARENT_SCOPE
)
set(YETTY_ZOO_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
    PARENT_SCOPE
)
