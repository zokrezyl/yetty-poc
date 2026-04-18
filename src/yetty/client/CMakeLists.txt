add_executable(yettyc
    main.cpp
    buffers.cpp
    cards.cpp
    event.cpp
    ui.cpp
)

target_link_libraries(yettyc PRIVATE
    yetty-rpc
    args
)

target_include_directories(yettyc PRIVATE
    ${YETTY_ROOT}/include
)

target_compile_features(yettyc PRIVATE cxx_std_23)
