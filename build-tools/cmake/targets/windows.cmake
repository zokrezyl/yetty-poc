# Windows desktop build target

include(${YETTY_ROOT}/build-tools/cmake/targets/shared.cmake)

# Windows-specific libraries
include(${YETTY_ROOT}/build-tools/cmake/libs/glfw.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/libjpeg-turbo.cmake)

# Add src/yetty (builds libraries)
add_subdirectory(${YETTY_ROOT}/src/yetty ${CMAKE_BINARY_DIR}/src/yetty)

# Desktop-specific subdirectories
add_subdirectory(${YETTY_ROOT}/src/yetty/gpu ${CMAKE_BINARY_DIR}/src/yetty/gpu)

# Create executable with core sources + platform
add_executable(yetty
    ${YETTY_CORE_SOURCES}
    ${YETTY_DESKTOP_SOURCES}
    ${YETTY_ROOT}/src/yetty/platform/windows-platform.cpp
)

target_include_directories(yetty PRIVATE ${YETTY_INCLUDES} ${YETTY_RENDERER_INCLUDES} ${JPEG_INCLUDE_DIRS})

# Embed resources (logo)
incbin_add_resources(yetty
    Logo "${YETTY_ROOT}/docs/logo.jpeg"
)

target_compile_definitions(yetty PRIVATE
    ${YETTY_DEFINITIONS}
    YETTY_WEB=0
    YETTY_ANDROID=0
    YETTY_USE_PREBUILT_ATLAS=0
    YETTY_USE_DIRECTWRITE=1
    YETTY_USE_CONPTY=1
    YETTY_HAS_VNC=1
    NOMINMAX
    WIN32_LEAN_AND_MEAN
)

set_target_properties(yetty PROPERTIES ENABLE_EXPORTS TRUE)

target_link_libraries(yetty PRIVATE
    ${YETTY_LIBS}
    glfw
    glfw3webgpu
    args
    ytrace::ytrace
    lz4_static
    uv_a
    yetty_gpu
    turbojpeg-static
    yetty_vnc
    dwrite
    ws2_32
    ${FREETYPE_ALL_LIBS}
)

# CDB font generation
include(${YETTY_ROOT}/build-tools/cmake/cdb-gen.cmake)

# Copy runtime assets to build directory
add_subdirectory(${YETTY_ROOT}/assets ${CMAKE_BINARY_DIR}/assets-build)

# Ensure all runtime assets are in build output before yetty
add_dependencies(yetty generate-cdb copy-shaders copy-assets)

# Copy DirectX runtime DLLs needed by Dawn (shader compiler + DXIL signing)
if(WIN32)
    # Find Windows SDK Redist directory for DirectX DLLs
    # Check both versioned paths (Redist/<version>/x64) and direct path (Redist/D3D/x64)
    set(_dx_redist_dir "")
    if(EXISTS "C:/Program Files (x86)/Windows Kits/10/Redist/D3D/x64/d3dcompiler_47.dll")
        set(_dx_redist_dir "C:/Program Files (x86)/Windows Kits/10/Redist/D3D/x64")
    else()
        file(GLOB _dx_redist_dirs "C:/Program Files (x86)/Windows Kits/10/Redist/*/x64")
        if(_dx_redist_dirs)
            list(SORT _dx_redist_dirs ORDER DESCENDING)
            list(GET _dx_redist_dirs 0 _dx_redist_dir)
        endif()
    endif()

    if(_dx_redist_dir)
        message(STATUS "DirectX Redist dir: ${_dx_redist_dir}")

        # Copy d3dcompiler_47.dll
        if(EXISTS "${_dx_redist_dir}/d3dcompiler_47.dll")
            add_custom_command(TARGET yetty POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${_dx_redist_dir}/d3dcompiler_47.dll"
                    "$<TARGET_FILE_DIR:yetty>/d3dcompiler_47.dll"
                COMMENT "Copying d3dcompiler_47.dll"
            )
        endif()

        # Copy dxil.dll
        if(EXISTS "${_dx_redist_dir}/dxil.dll")
            add_custom_command(TARGET yetty POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${_dx_redist_dir}/dxil.dll"
                    "$<TARGET_FILE_DIR:yetty>/dxil.dll"
                COMMENT "Copying dxil.dll"
            )
        endif()
    else()
        message(WARNING "Could not find Windows SDK Redist directory for DirectX DLLs")
    endif()
endif()

# Verify all required assets are present
add_custom_command(TARGET yetty POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DBUILD_DIR=${CMAKE_BINARY_DIR} -DTARGET_TYPE=desktop -P ${YETTY_ROOT}/build-tools/cmake/verify-assets.cmake
    COMMENT "Verifying build assets..."
)

# Tests
enable_testing()
add_subdirectory(${YETTY_ROOT}/test/ut/windows ${CMAKE_BINARY_DIR}/test/ut/windows)
