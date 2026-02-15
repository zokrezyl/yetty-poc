# Shared HTML rendering logic (used by yhtml card and html2ydraw tool)
target_sources(yetty PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/html-container.h
    ${CMAKE_CURRENT_SOURCE_DIR}/html-container.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/html-renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/html-renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/http-fetcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/http-fetcher.cpp
)

target_include_directories(yetty PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
