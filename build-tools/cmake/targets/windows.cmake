# Windows desktop build target

include(${YETTY_ROOT}/build-tools/cmake/targets/shared.cmake)

# Windows-specific libraries
include(${YETTY_ROOT}/build-tools/cmake/libs/glfw.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/libjpeg-turbo.cmake)

# Add src/yetty (builds libraries)
add_subdirectory(${YETTY_ROOT}/src/yetty ${CMAKE_BINARY_DIR}/src/yetty)

# Desktop-specific subdirectories
add_subdirectory(${YETTY_ROOT}/src/yetty/gpu ${CMAKE_BINARY_DIR}/src/yetty/gpu)
add_subdirectory(${YETTY_ROOT}/src/yetty/client ${CMAKE_BINARY_DIR}/src/yetty/client)
add_subdirectory(${YETTY_ROOT}/src/yetty/ytop ${CMAKE_BINARY_DIR}/src/yetty/ytop)

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
    # Gather Windows SDK search dirs once, sorted descending (latest first)
    file(GLOB _sdk_bin_x64_dirs  "C:/Program Files (x86)/Windows Kits/10/bin/*/x64")
    file(GLOB _redist_x64_dirs   "C:/Program Files (x86)/Windows Kits/10/Redist/*/x64")
    if(_sdk_bin_x64_dirs)
        list(SORT _sdk_bin_x64_dirs ORDER DESCENDING)
    endif()
    if(_redist_x64_dirs)
        list(SORT _redist_x64_dirs ORDER DESCENDING)
    endif()

    # --- Find d3dcompiler_47.dll ---
    # Typically in Windows Kits Redist/D3D/x64 or versioned Redist/<version>/x64
    set(_d3dcompiler_dll "")
    if(EXISTS "C:/Program Files (x86)/Windows Kits/10/Redist/D3D/x64/d3dcompiler_47.dll")
        set(_d3dcompiler_dll "C:/Program Files (x86)/Windows Kits/10/Redist/D3D/x64/d3dcompiler_47.dll")
    else()
        foreach(_dir ${_redist_x64_dirs})
            if(EXISTS "${_dir}/d3dcompiler_47.dll")
                set(_d3dcompiler_dll "${_dir}/d3dcompiler_47.dll")
                break()
            endif()
        endforeach()
    endif()
    # Also search bin/<version>/x64 as fallback
    if(NOT _d3dcompiler_dll)
        foreach(_dir ${_sdk_bin_x64_dirs})
            if(EXISTS "${_dir}/d3dcompiler_47.dll")
                set(_d3dcompiler_dll "${_dir}/d3dcompiler_47.dll")
                break()
            endif()
        endforeach()
    endif()

    if(_d3dcompiler_dll)
        message(STATUS "Found d3dcompiler_47.dll: ${_d3dcompiler_dll}")
        add_custom_command(TARGET yetty POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${_d3dcompiler_dll}"
                "$<TARGET_FILE_DIR:yetty>/d3dcompiler_47.dll"
            COMMENT "Copying d3dcompiler_47.dll"
        )
    else()
        message(WARNING "Could not find d3dcompiler_47.dll in Windows SDK")
    endif()

    # --- Find dxil.dll ---
    # dxil.dll (DXC signing library) lives in Windows Kits bin/<version>/x64,
    # NOT in the Redist directory.
    set(_dxil_dll "")
    foreach(_dir ${_sdk_bin_x64_dirs})
        if(EXISTS "${_dir}/dxil.dll")
            set(_dxil_dll "${_dir}/dxil.dll")
            break()
        endif()
    endforeach()
    # Fallback: Redist/D3D/x64 or versioned Redist
    if(NOT _dxil_dll)
        if(EXISTS "C:/Program Files (x86)/Windows Kits/10/Redist/D3D/x64/dxil.dll")
            set(_dxil_dll "C:/Program Files (x86)/Windows Kits/10/Redist/D3D/x64/dxil.dll")
        else()
            foreach(_dir ${_redist_x64_dirs})
                if(EXISTS "${_dir}/dxil.dll")
                    set(_dxil_dll "${_dir}/dxil.dll")
                    break()
                endif()
            endforeach()
        endif()
    endif()

    if(_dxil_dll)
        message(STATUS "Found dxil.dll: ${_dxil_dll}")
        add_custom_command(TARGET yetty POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${_dxil_dll}"
                "$<TARGET_FILE_DIR:yetty>/dxil.dll"
            COMMENT "Copying dxil.dll"
        )
    else()
        message(WARNING "Could not find dxil.dll in Windows SDK")
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
