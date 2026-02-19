add_executable(ydraw-maze
    main.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw-maze/maze-renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw/ydraw-buffer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ycat/osc.cpp
)

target_include_directories(ydraw-maze PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw-maze
    ${CMAKE_SOURCE_DIR}/src/yetty/ycat
)

target_link_libraries(ydraw-maze PRIVATE ytrace::ytrace)
target_compile_features(ydraw-maze PRIVATE cxx_std_20)
