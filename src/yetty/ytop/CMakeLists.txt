# ytop - Top-like CPU monitor using yetty plots

add_executable(ytop
    main.cpp
)

target_link_libraries(ytop PRIVATE
    args
)

target_include_directories(ytop PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_features(ytop PRIVATE cxx_std_23)
