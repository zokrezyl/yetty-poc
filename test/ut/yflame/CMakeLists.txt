#=============================================================================
# yflame Unit Tests - FlameRenderer parsing, rendering, serialization, OSC
#=============================================================================

if(POLICY CMP0175)
    cmake_policy(SET CMP0175 OLD)
endif()
CPMAddPackage(
    NAME ut
    GITHUB_REPOSITORY boost-ext/ut
    GIT_TAG v2.1.0
    OPTIONS "BUILD_BENCHMARKS OFF" "BUILD_EXAMPLES OFF"
)

add_executable(yflame_tests
    main.cpp
    yflame_test.cpp
    # FlameRenderer under test
    ${CMAKE_SOURCE_DIR}/tools/yflame/flame-renderer.cpp
    # Dependencies
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw/ydraw-buffer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ycat/osc.cpp
)

target_include_directories(yflame_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/tools/yflame
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/yetty/ycat
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(yflame_tests PRIVATE
    ut
    ytrace::ytrace
)

target_compile_features(yflame_tests PRIVATE cxx_std_20)

add_test(NAME yflame_tests COMMAND yflame_tests)
