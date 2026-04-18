#=============================================================================
# YPlot Unit Tests
#=============================================================================

add_executable(yplot_tests
    main.cpp
    yplot_test.cpp
)

target_include_directories(yplot_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(yplot_tests PRIVATE
    yetty_yplot
    yetty_yfsvm
    yetty_yast
    yetty_ydraw
    ut
)

add_test(NAME yplot_tests COMMAND yplot_tests)
