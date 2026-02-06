#=============================================================================
# YEcho Unit Tests
#=============================================================================

# Boost UT - Modern C++20 unit testing framework
if(POLICY CMP0175)
    cmake_policy(SET CMP0175 OLD)
endif()
CPMAddPackage(
    NAME ut
    GITHUB_REPOSITORY boost-ext/ut
    GIT_TAG v2.1.0
    OPTIONS "BUILD_BENCHMARKS OFF" "BUILD_EXAMPLES OFF"
)

add_executable(yecho_tests
    main.cpp
    yecho_test.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/yecho/yecho-parser.cpp
)

target_include_directories(yecho_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/yetty/yecho
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(yecho_tests PRIVATE ut)

# Register with CTest
add_test(NAME yecho_tests COMMAND yecho_tests)
