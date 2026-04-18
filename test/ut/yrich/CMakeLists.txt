#=============================================================================
# YRich Unit Tests
#=============================================================================

add_executable(yrich_tests
    main.cpp
    persist_test.cpp
)

target_include_directories(yrich_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(yrich_tests PRIVATE
    yetty_yrich
    ut
)

add_test(NAME yrich_tests COMMAND yrich_tests)
