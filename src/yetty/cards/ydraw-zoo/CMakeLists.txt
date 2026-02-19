add_subdirectory(${CMAKE_SOURCE_DIR}/src/yetty/ydraw-zoo ydraw-zoo-lib)

target_sources(yetty PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ydraw-zoo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ydraw-zoo.cpp
)
