#=============================================================================
# YAST Unit Tests
#=============================================================================

add_executable(yast_tests
    main.cpp
    yast_test.cpp
)

target_include_directories(yast_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(yast_tests PRIVATE
    yetty_yast
    ut
)

add_test(NAME yast_tests COMMAND yast_tests)
