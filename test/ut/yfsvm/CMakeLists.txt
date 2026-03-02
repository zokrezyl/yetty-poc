#=============================================================================
# YFSVM Unit Tests
#=============================================================================

add_executable(yfsvm_tests
    main.cpp
    yfsvm_test.cpp
)

target_include_directories(yfsvm_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(yfsvm_tests PRIVATE
    yetty_yfsvm
    ut
)

add_test(NAME yfsvm_tests COMMAND yfsvm_tests)
