add_subdirectory(${CMAKE_SOURCE_DIR}/src/yetty/ydraw-maze ydraw-maze-lib)

target_sources(yetty PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ydraw-maze.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ydraw-maze.cpp
)
