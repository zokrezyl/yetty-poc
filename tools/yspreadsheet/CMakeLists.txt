# ydoc - Interactive document editor using YRich
add_executable(ydoc
    main.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ycat/osc.cpp
)

target_include_directories(ydoc PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(ydoc PRIVATE
    yetty_yrich
    yetty_ydraw
    uv_a
    ytrace::ytrace
    args
)

target_compile_features(ydoc PRIVATE cxx_std_20)
