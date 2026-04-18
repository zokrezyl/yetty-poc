add_executable(ydraw-generator
    main.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw/ydraw-buffer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ycat/osc.cpp
)

target_include_directories(ydraw-generator PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw
    ${CMAKE_SOURCE_DIR}/src/yetty/ycat
)

target_link_libraries(ydraw-generator PRIVATE ytrace::ytrace args)
target_compile_features(ydraw-generator PRIVATE cxx_std_20)
