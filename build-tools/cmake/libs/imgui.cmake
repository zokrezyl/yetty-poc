# ImGui - Immediate mode GUI
# Platform-specific: GLFW on desktop, WebGPU backend on all
if(TARGET imgui)
    return()
endif()

CPMAddPackage(
    NAME imgui
    URL https://github.com/ocornut/imgui/archive/refs/tags/v1.92.5.tar.gz
    VERSION 1.92.5
    DOWNLOAD_ONLY YES
)

if(imgui_ADDED)
    # Core ImGui sources
    set(IMGUI_SOURCES
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_wgpu.cpp
    )

    # Platform backend
    if(EMSCRIPTEN)
        list(APPEND IMGUI_SOURCES ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp)
    elseif(NOT YETTY_ANDROID)
        list(APPEND IMGUI_SOURCES ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp)
    endif()

    add_library(imgui STATIC ${IMGUI_SOURCES})
    target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)

    # Link and compile options based on platform
    if(EMSCRIPTEN)
        target_link_libraries(imgui PUBLIC webgpu)
        target_compile_options(imgui PUBLIC --use-port=emdawnwebgpu)
        target_link_options(imgui PUBLIC -sUSE_GLFW=3)
        target_compile_definitions(imgui PUBLIC IMGUI_IMPL_WEBGPU_BACKEND_DAWN=1)
    elseif(YETTY_ANDROID)
        target_link_libraries(imgui PUBLIC webgpu)
        if(WEBGPU_BACKEND STREQUAL "wgpu")
            target_compile_definitions(imgui PUBLIC IMGUI_IMPL_WEBGPU_BACKEND_WGPU=1)
        else()
            target_compile_definitions(imgui PUBLIC IMGUI_IMPL_WEBGPU_BACKEND_DAWN=1)
        endif()
    else()
        # Desktop
        include(${CMAKE_CURRENT_LIST_DIR}/glfw.cmake)
        if(APPLE)
            target_link_libraries(imgui PUBLIC glfw webgpu)
            set_source_files_properties(
                ${imgui_SOURCE_DIR}/backends/imgui_impl_wgpu.cpp
                PROPERTIES COMPILE_FLAGS "-x objective-c++"
            )
        elseif(WIN32)
            target_link_libraries(imgui PUBLIC glfw webgpu)
        else()
            # Linux - requires X11
            find_package(X11 REQUIRED)
            target_link_libraries(imgui PUBLIC glfw webgpu X11::X11)
        endif()
        if(WEBGPU_BACKEND STREQUAL "wgpu")
            target_compile_definitions(imgui PUBLIC IMGUI_IMPL_WEBGPU_BACKEND_WGPU=1)
        else()
            target_compile_definitions(imgui PUBLIC IMGUI_IMPL_WEBGPU_BACKEND_DAWN=1)
        endif()
    endif()

    set_target_properties(imgui PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
