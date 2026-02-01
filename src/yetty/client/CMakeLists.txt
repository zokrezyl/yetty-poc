add_executable(yettyc
    main.cpp
    event.cpp
    ui.cpp
)

target_link_libraries(yettyc PRIVATE
    yetty-rpc
    args
)

target_include_directories(yettyc PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_features(yettyc PRIVATE cxx_std_23)
