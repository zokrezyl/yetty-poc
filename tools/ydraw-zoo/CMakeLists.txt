add_executable(ydraw-zoo
    main.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw-zoo/zoo-renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw/ydraw-buffer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ycat/osc.cpp
)

target_include_directories(ydraw-zoo PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw-zoo
    ${CMAKE_SOURCE_DIR}/src/yetty/ycat
)

target_link_libraries(ydraw-zoo PRIVATE ytrace::ytrace)
target_compile_features(ydraw-zoo PRIVATE cxx_std_20)
