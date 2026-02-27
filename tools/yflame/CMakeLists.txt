add_executable(yflame
    main.cpp
    flame-renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw/ydraw-buffer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ycat/osc.cpp
)

target_include_directories(yflame PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/yetty/ycat
)

target_link_libraries(yflame PRIVATE ytrace::ytrace)
target_compile_features(yflame PRIVATE cxx_std_20)
