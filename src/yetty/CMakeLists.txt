# yetty sources and subdirectories

# Core sources
set(YETTY_CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/yetty.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/terminal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/terminal-view.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpu-screen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/osc-command.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpu-allocator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/card-manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/card-buffer-manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/card-texture-manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpu-screen-manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/card-factory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/card.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/name-generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui-manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/workspace.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/view.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/frame-renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader-manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/font-manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/msdf-atlas.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ms-msdf-font.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vector-sdf-font.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vector-coverage-font.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raster-font.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bm-font.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader-font.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared-memory.cpp
    PARENT_SCOPE
)

# Desktop-only sources
set(YETTY_DESKTOP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/msdf-cdb-provider.cpp
    PARENT_SCOPE
)

# Android-specific sources
set(YETTY_ANDROID_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/hdraw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/msdf-cdb-provider.cpp
    PARENT_SCOPE
)

# Subdirectories that build libraries
add_subdirectory(base)
add_subdirectory(font)
add_subdirectory(ymery)
add_subdirectory(yecho)
add_subdirectory(ydraw)
add_subdirectory(diagram)
add_subdirectory(cards)
add_subdirectory(yast)
add_subdirectory(msdf-wgsl)

# Desktop-only subdirectories
if(NOT EMSCRIPTEN AND NOT YETTY_ANDROID)
    add_subdirectory(gpu)
    add_subdirectory(msdf-gen)
    if(NOT WIN32)
        add_subdirectory(rpc)
        add_subdirectory(client)
        add_subdirectory(ytop)
    endif()
endif()
