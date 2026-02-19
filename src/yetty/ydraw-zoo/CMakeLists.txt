# Shared zoo rendering logic (used by ydraw-zoo card and standalone tools)
target_sources(yetty PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/zoo-renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/zoo-renderer.cpp
)
target_include_directories(yetty PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
