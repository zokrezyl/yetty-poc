add_subdirectory(gpu-info)
add_subdirectory(ydraw-maze)
add_subdirectory(ydraw-zoo)
add_subdirectory(ymux)

# PDF and HTML tools (desktop only - require pdfio, litehtml)
if(NOT YETTY_ANDROID AND NOT EMSCRIPTEN)
    add_subdirectory(pdf2ydraw)
    add_subdirectory(html2ydraw)
    add_subdirectory(yhtml-machine)
    add_subdirectory(ybrowser)
endif()

# Ymery test tool - standalone ImGui/OpenGL3 app that loads YAML layouts (optional)
find_package(OpenGL QUIET)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ymery/test.cpp" AND OpenGL_FOUND)
add_executable(ymery-test
    ymery/test.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ymery/types.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ymery/data-bag.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ymery/data-tree.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ymery/dispatcher.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ymery/lang.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ymery/simple-data-tree.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ymery/renderer.cpp
)

target_include_directories(ymery-test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# ImGui with OpenGL3 backend (not WebGPU)
target_sources(ymery-test PRIVATE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(ymery-test PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(ymery-test PRIVATE
    glfw
    OpenGL::GL
    yaml-cpp
    ytrace::ytrace
    yetty_card_ygui
)

target_compile_features(ymery-test PRIVATE cxx_std_23)
endif()
