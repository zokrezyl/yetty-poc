add_library(yetty_card_plot STATIC
    plot.cpp
    plot-sampler-provider.cpp
    plot-transformer-provider.cpp
    plot-renderer-provider.cpp
)
target_include_directories(yetty_card_plot PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(yetty_card_plot PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace yetty_yecho yetty_yast)
