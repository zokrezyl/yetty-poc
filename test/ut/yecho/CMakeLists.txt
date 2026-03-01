#=============================================================================
# YEcho Unit Tests
#=============================================================================

add_executable(yecho_tests
    main.cpp
    yecho_test.cpp
)

target_include_directories(yecho_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(yecho_tests PRIVATE
    yetty_yecho
    ut
)

add_test(NAME yecho_tests COMMAND yecho_tests)
