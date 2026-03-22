# yetty sources and subdirectories

# YGui-C library (pure C, for FFI bindings)
add_subdirectory(ygui-c)

# Sources for yetty executable (collected from subdirectories)
add_subdirectory(ydraw-zoo)
add_subdirectory(ydraw-maze)
add_subdirectory(cards/ygrid)
add_subdirectory(msdf-gen)
add_subdirectory(rpc)

# Core sources - ALL sources needed for yetty executable (platform-agnostic)
set(YETTY_CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/yetty.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/incbin-assets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/osc-scanner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/terminal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/terminal-view.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpu-screen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/osc-command.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpu-allocator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpu-memory-manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpu-buffer-manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpu-texture-manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpu-screen-manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/card-factory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/card.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/name-generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ygui/ygui-overlay.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/screen-draw-layer.cpp
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
    # Renderer sources (from subdirectories)
    ${YETTY_ZOO_SOURCES}
    ${YETTY_MAZE_SOURCES}
    ${YETTY_YGRID_SOURCES}
    PARENT_SCOPE
)

# Desktop-only sources (Linux, macOS - NOT Windows)
set(YETTY_DESKTOP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/msdf-cdb-provider.cpp
    ${YETTY_MSDF_GEN_SOURCES}
    ${YETTY_RPC_SOURCES}
    PARENT_SCOPE
)

# Windows-specific sources
set(YETTY_WINDOWS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/msdf-cdb-provider.cpp
    ${YETTY_MSDF_GEN_SOURCES}
    ${YETTY_RPC_SOURCES}
    PARENT_SCOPE
)

# WebAssembly-specific sources
set(YETTY_WEB_SOURCES
    PARENT_SCOPE
)

# Android-specific sources
set(YETTY_ANDROID_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/msdf-cdb-provider.cpp
    PARENT_SCOPE
)

# iPhone/iOS-specific sources
set(YETTY_IOS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/msdf-cdb-provider.cpp
    PARENT_SCOPE
)

# Include directories for renderer sources
set(YETTY_RENDERER_INCLUDES
    ${YETTY_ZOO_INCLUDES}
    ${YETTY_MAZE_INCLUDES}
    PARENT_SCOPE
)

# Subdirectories that build libraries (common to all platforms)
add_subdirectory(base)
add_subdirectory(font)
add_subdirectory(yecho)
# ycat requires OpenSSL via cpr and libmagic, skip for WebAssembly/iOS
if(NOT EMSCRIPTEN AND NOT YETTY_IOS)
    add_subdirectory(ycat)
endif()
add_subdirectory(ydraw)
add_subdirectory(yrich)
add_subdirectory(ypaint)
add_subdirectory(diagram)
add_subdirectory(ygrid)
# ythorvg requires thorvg which overrides global new/delete - disabled by default
if(YETTY_ENABLE_THORVG)
    add_subdirectory(ythorvg)
endif()
# yvideo - H.264/AV1 codec support for VNC
add_subdirectory(yvideo)
add_subdirectory(cards)
add_subdirectory(yast)
add_subdirectory(yfsvm)
add_subdirectory(yplot)
add_subdirectory(msdf-wgsl)
add_subdirectory(shaders)

# VNC is available on all platforms except Android
# - Desktop (Linux, macOS, Windows): uses POSIX/Winsock sockets
# - iPhone/iOS: uses POSIX sockets
# - WebAssembly: uses Emscripten WebSocket API
if(NOT YETTY_ANDROID)
    add_subdirectory(vnc)
endif()

# Telnet is available on Unix platforms (primarily for Android local connections)
# Windows: telnet/CMakeLists.txt creates an INTERFACE stub (uses ConPTY instead)
add_subdirectory(telnet)

# SSH is available on all platforms
add_subdirectory(ssh)
