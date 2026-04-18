add_executable(ybrowser
    main.cpp
    stb_impl.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/yhtml/html-container.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/yhtml/html-renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/yhtml/http-fetcher.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw/ydraw-buffer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font/raw-font.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font/raw-font-manager.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/font/freetype.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ycat/osc.cpp
)

target_include_directories(ybrowser PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/yetty/yhtml
    ${CMAKE_SOURCE_DIR}/src/yetty/ycat
    ${FREETYPE_INCLUDE_DIR}
)

# Fontconfig for resolving CSS font-family names to system TTF files
if(NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
    target_include_directories(ybrowser PRIVATE ${FONTCONFIG_INCLUDE_DIRS})
    target_compile_definitions(ybrowser PRIVATE YETTY_USE_FONTCONFIG=1)
    target_link_libraries(ybrowser PRIVATE ${FONTCONFIG_LIBRARIES})
endif()

target_link_libraries(ybrowser PRIVATE litehtml cpr::cpr freetype args ytrace::ytrace stb)
target_compile_features(ybrowser PRIVATE cxx_std_20)
