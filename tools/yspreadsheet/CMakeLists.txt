# yspreadsheet - Interactive spreadsheet editor using YRich
add_executable(yspreadsheet
    main.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ycat/osc.cpp
)

target_include_directories(yspreadsheet PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(yspreadsheet PRIVATE
    yetty_yrich
    yetty_ydraw
    uv_a
    ytrace::ytrace
    args
)

target_compile_features(yspreadsheet PRIVATE cxx_std_20)
