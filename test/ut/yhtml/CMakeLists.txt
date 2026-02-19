#=============================================================================
# YHtml Unit Tests - HTML rendering to YDrawBuffer
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

find_package(PkgConfig REQUIRED)
pkg_check_modules(FONTCONFIG REQUIRED fontconfig)

add_executable(yhtml_tests
    main.cpp
    stb_impl.cpp
    # YHtml core
    ${CMAKE_SOURCE_DIR}/src/yetty/yhtml/html-container.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/yhtml/html-renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/yhtml/http-fetcher.cpp
    # YDraw buffer
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw/ydraw-buffer.cpp
    # Font system for text measurement
    ${CMAKE_SOURCE_DIR}/src/yetty/font/raw-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font/raw-font-manager.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font/freetype.cpp
)

target_include_directories(yhtml_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/yetty/yhtml
)

target_compile_definitions(yhtml_tests PRIVATE
    YETTY_USE_FONTCONFIG
)

target_link_libraries(yhtml_tests PRIVATE
    ut
    litehtml
    cpr::cpr
    stb
    ${FREETYPE_ALL_LIBS}
    ytrace::ytrace
    ${FONTCONFIG_LIBRARIES}
)

add_test(NAME yhtml_tests COMMAND yhtml_tests)
