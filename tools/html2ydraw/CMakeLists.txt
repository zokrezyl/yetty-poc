add_executable(html2ydraw
    main.cpp
    stb_impl.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/yhtml/html-container.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/yhtml/html-renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/yhtml/http-fetcher.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw/ydraw-buffer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font/raw-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font/raw-font-manager.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font/freetype.cpp
)

target_include_directories(html2ydraw PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/yetty/yhtml
    ${FREETYPE_INCLUDE_DIR}
)

# Fontconfig for resolving CSS font-family names to system TTF files
if(NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
    target_include_directories(html2ydraw PRIVATE ${FONTCONFIG_INCLUDE_DIRS})
    target_compile_definitions(html2ydraw PRIVATE YETTY_USE_FONTCONFIG=1)
    target_link_libraries(html2ydraw PRIVATE ${FONTCONFIG_LIBRARIES})
endif()

target_link_libraries(html2ydraw PRIVATE litehtml cpr::cpr freetype args ytrace::ytrace stb)
target_compile_features(html2ydraw PRIVATE cxx_std_20)
