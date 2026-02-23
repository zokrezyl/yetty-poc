# Verify build assets are present
# Usage: cmake -P verify-assets.cmake -DBUILD_DIR=<path> -DTARGET_TYPE=<webasm|desktop|android>

if(NOT BUILD_DIR)
    message(FATAL_ERROR "BUILD_DIR not set")
endif()

if(NOT TARGET_TYPE)
    message(FATAL_ERROR "TARGET_TYPE not set (webasm, desktop, or android)")
endif()

set(MISSING_FILES "")
set(CHECKED_COUNT 0)

function(check_file FILE_PATH DESCRIPTION)
    math(EXPR CHECKED_COUNT "${CHECKED_COUNT} + 1" OUTPUT_FORMAT DECIMAL)
    set(CHECKED_COUNT ${CHECKED_COUNT} PARENT_SCOPE)
    if(NOT EXISTS "${BUILD_DIR}/${FILE_PATH}")
        list(APPEND MISSING_FILES "${FILE_PATH} (${DESCRIPTION})")
        set(MISSING_FILES ${MISSING_FILES} PARENT_SCOPE)
    endif()
endfunction()

function(check_dir DIR_PATH DESCRIPTION)
    math(EXPR CHECKED_COUNT "${CHECKED_COUNT} + 1" OUTPUT_FORMAT DECIMAL)
    set(CHECKED_COUNT ${CHECKED_COUNT} PARENT_SCOPE)
    if(NOT IS_DIRECTORY "${BUILD_DIR}/${DIR_PATH}")
        list(APPEND MISSING_FILES "${DIR_PATH}/ (${DESCRIPTION})")
        set(MISSING_FILES ${MISSING_FILES} PARENT_SCOPE)
    endif()
endfunction()

message(STATUS "=== Verifying build assets for ${TARGET_TYPE} ===")
message(STATUS "Build directory: ${BUILD_DIR}")

#-----------------------------------------------------------------------------
# Common assets (all platforms)
#-----------------------------------------------------------------------------
message(STATUS "Checking common assets...")

# Font files
check_file("assets/DejaVuSansMNerdFontMono-Regular.ttf" "Regular font")
check_file("assets/DejaVuSansMNerdFontMono-Bold.ttf" "Bold font")
check_file("assets/DejaVuSansMNerdFontMono-Oblique.ttf" "Oblique font")
check_file("assets/DejaVuSansMNerdFontMono-BoldOblique.ttf" "Bold Oblique font")

# Font CDB files
check_dir("assets/fonts-cdb" "Font CDB directory")
check_file("assets/fonts-cdb/DejaVuSansMNerdFontMono-Regular.cdb" "Regular font CDB")
check_file("assets/fonts-cdb/DejaVuSansMNerdFontMono-Bold.cdb" "Bold font CDB")
check_file("assets/fonts-cdb/DejaVuSansMNerdFontMono-Oblique.cdb" "Oblique font CDB")
check_file("assets/fonts-cdb/DejaVuSansMNerdFontMono-BoldOblique.cdb" "Bold Oblique font CDB")

# Shader directory structure
check_dir("assets/shaders" "Shaders directory")
check_dir("assets/shaders/lib" "Shader lib directory")
check_dir("assets/shaders/cards" "Shader cards directory")
check_dir("assets/shaders/effects" "Shader effects directory")
check_dir("assets/shaders/pre-effects" "Shader pre-effects directory")
check_dir("assets/shaders/post-effects" "Shader post-effects directory")

# Core shaders
check_file("assets/shaders/gpu-screen.wgsl" "Main GPU screen shader")
check_file("assets/shaders/cursor.wgsl" "Cursor shader")
check_file("assets/shaders/msdf_gen.wgsl" "MSDF generation shader")
check_file("assets/shaders/scale-image.wgsl" "Image scaling shader")

# Card shaders
check_file("assets/shaders/cards/0x0000-texture.wgsl" "Texture card shader")
check_file("assets/shaders/cards/0x0001-plot.wgsl" "Plot card shader")
check_file("assets/shaders/cards/0x0002-hdraw.wgsl" "Hdraw card shader")
check_file("assets/shaders/cards/0x0003-ydraw.wgsl" "Ydraw card shader")
check_file("assets/shaders/cards/0x0004-kdraw.wgsl" "Kdraw card shader")
check_file("assets/shaders/cards/0x0005-jdraw.wgsl" "Jdraw card shader")
check_file("assets/shaders/cards/0x0006-ytext.wgsl" "Ytext card shader")

#-----------------------------------------------------------------------------
# WebAssembly specific
#-----------------------------------------------------------------------------
if(TARGET_TYPE STREQUAL "webasm")
    message(STATUS "Checking WebAssembly assets...")

    # WASM output files
    check_file("yetty.js" "Yetty JS loader")
    check_file("yetty.wasm" "Yetty WASM binary")
    check_file("yetty.data" "Yetty preloaded data")

    # Web files
    check_file("index.html" "Web index page")
    check_file("serve.py" "Development server")

    # Toybox (optional but expected)
    check_file("toybox.js" "Toybox JS loader")
    check_file("toybox.wasm" "Toybox WASM binary")

    # Note: demo directory is preloaded from source into yetty.data, not copied to build dir
endif()

#-----------------------------------------------------------------------------
# Desktop specific
#-----------------------------------------------------------------------------
if(TARGET_TYPE STREQUAL "desktop")
    message(STATUS "Checking Desktop assets...")

    # Executable
    check_file("yetty" "Yetty executable")

    # Demo directory
    check_dir("demo" "Demo scripts directory")
endif()

#-----------------------------------------------------------------------------
# Android specific
#-----------------------------------------------------------------------------
if(TARGET_TYPE STREQUAL "android")
    message(STATUS "Checking Android assets...")

    # Android outputs are in different locations
    # check_file("libyetty.so" "Yetty shared library")
endif()

#-----------------------------------------------------------------------------
# Report results
#-----------------------------------------------------------------------------
message(STATUS "")
message(STATUS "=== Asset Verification Results ===")
message(STATUS "Checked: ${CHECKED_COUNT} items")

list(LENGTH MISSING_FILES MISSING_COUNT)
if(MISSING_COUNT GREATER 0)
    message(STATUS "Missing: ${MISSING_COUNT} items")
    message(STATUS "")
    message(FATAL_ERROR "Missing files:\n  ${MISSING_FILES}")
else()
    message(STATUS "Missing: 0 items")
    message(STATUS "")
    message(STATUS "All assets verified successfully!")
endif()
