# Copy runtime assets to build directory
set(ASSETS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(ASSETS_DST_DIR ${CMAKE_BINARY_DIR}/assets)

# Files to copy (fonts, atlas, etc.)
set(ASSET_FILES
    ${ASSETS_SRC_DIR}/DejaVuSansMNerdFontMono-Regular.ttf
    ${ASSETS_SRC_DIR}/DejaVuSansMNerdFontMono-Bold.ttf
    ${ASSETS_SRC_DIR}/DejaVuSansMNerdFontMono-Oblique.ttf
    ${ASSETS_SRC_DIR}/DejaVuSansMNerdFontMono-BoldOblique.ttf
)

# Create custom target to copy assets
add_custom_target(copy-assets ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${ASSETS_DST_DIR}
    COMMENT "Copying assets to build directory"
)

foreach(ASSET_FILE ${ASSET_FILES})
    get_filename_component(ASSET_NAME ${ASSET_FILE} NAME)
    add_custom_command(TARGET copy-assets POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ASSET_FILE} ${ASSETS_DST_DIR}/${ASSET_NAME}
    )
endforeach()
