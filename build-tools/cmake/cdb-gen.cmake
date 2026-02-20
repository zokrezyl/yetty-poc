# CDB Font Generation
# Generates MSDF CDB files from TTF fonts using yetty-msdf-gen tool
# Outputs to ${CMAKE_BINARY_DIR}/fonts-cdb/
# If pre-built CDB files exist in assets/fonts-cdb/, copies them instead

set(CDB_OUTPUT_DIR "${CMAKE_BINARY_DIR}/assets/fonts-cdb")
set(FONT_DIR "${YETTY_ROOT}/assets")
set(PREBUILT_CDB_DIR "${YETTY_ROOT}/assets/fonts-cdb")

set(FONT_FILES
    "${FONT_DIR}/DejaVuSansMNerdFontMono-Regular.ttf"
    "${FONT_DIR}/DejaVuSansMNerdFontMono-Bold.ttf"
    "${FONT_DIR}/DejaVuSansMNerdFontMono-Oblique.ttf"
    "${FONT_DIR}/DejaVuSansMNerdFontMono-BoldOblique.ttf"
)

set(CDB_FILES
    "DejaVuSansMNerdFontMono-Regular.cdb"
    "DejaVuSansMNerdFontMono-Bold.cdb"
    "DejaVuSansMNerdFontMono-Oblique.cdb"
    "DejaVuSansMNerdFontMono-BoldOblique.cdb"
)

set(CDB_STAMP_FILE "${CDB_OUTPUT_DIR}/.cdb_generated")

# Check if prebuilt CDB files exist
set(PREBUILT_EXISTS TRUE)
foreach(CDB_FILE ${CDB_FILES})
    if(NOT EXISTS "${PREBUILT_CDB_DIR}/${CDB_FILE}")
        set(PREBUILT_EXISTS FALSE)
        break()
    endif()
endforeach()

if(PREBUILT_EXISTS)
    # Use prebuilt CDB files - just copy them
    message(STATUS "CDB: Using prebuilt fonts from ${PREBUILT_CDB_DIR}")
    add_custom_command(
        OUTPUT ${CDB_STAMP_FILE}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CDB_OUTPUT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${PREBUILT_CDB_DIR}" "${CDB_OUTPUT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E touch "${CDB_STAMP_FILE}"
        COMMENT "Copying prebuilt MSDF CDB files"
        VERBATIM
    )
elseif(CMAKE_CROSSCOMPILING)
    # Cross-compiling (Emscripten): build host tools using standalone CMakeLists.txt
    set(HOST_TOOLS_DIR "${CMAKE_BINARY_DIR}/host-tools")
    set(HOST_MSDF_GEN "${HOST_TOOLS_DIR}/yetty-msdf-gen")

    # Configure host tools build using standalone CMakeLists.txt (no X11/GLFW deps)
    add_custom_command(
        OUTPUT "${HOST_TOOLS_DIR}/build.ninja"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${HOST_TOOLS_DIR}"
        COMMAND ${CMAKE_COMMAND}
            -S "${YETTY_ROOT}/build-tools/cmake/host-tools"
            -B "${HOST_TOOLS_DIR}"
            -G Ninja
            -DCMAKE_BUILD_TYPE=Release
        COMMENT "Configuring host tools for CDB generation"
        VERBATIM
    )

    # Build host msdf-gen tool
    add_custom_command(
        OUTPUT "${HOST_MSDF_GEN}"
        COMMAND ${CMAKE_COMMAND} --build "${HOST_TOOLS_DIR}" --target yetty-msdf-gen --parallel
        DEPENDS "${HOST_TOOLS_DIR}/build.ninja"
        COMMENT "Building host yetty-msdf-gen tool"
        VERBATIM
    )

    # Generate CDB files
    add_custom_command(
        OUTPUT ${CDB_STAMP_FILE}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CDB_OUTPUT_DIR}"
        COMMAND "${HOST_MSDF_GEN}" --all "${FONT_DIR}/DejaVuSansMNerdFontMono-Regular.ttf" "${CDB_OUTPUT_DIR}"
        COMMAND "${HOST_MSDF_GEN}" --all "${FONT_DIR}/DejaVuSansMNerdFontMono-Bold.ttf" "${CDB_OUTPUT_DIR}"
        COMMAND "${HOST_MSDF_GEN}" --all "${FONT_DIR}/DejaVuSansMNerdFontMono-Oblique.ttf" "${CDB_OUTPUT_DIR}"
        COMMAND "${HOST_MSDF_GEN}" --all "${FONT_DIR}/DejaVuSansMNerdFontMono-BoldOblique.ttf" "${CDB_OUTPUT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E touch "${CDB_STAMP_FILE}"
        DEPENDS "${HOST_MSDF_GEN}" ${FONT_FILES}
        COMMENT "Generating MSDF CDB files from TTF fonts"
        VERBATIM
    )
else()
    # Native build: use msdf-gen from this build
    add_custom_command(
        OUTPUT ${CDB_STAMP_FILE}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CDB_OUTPUT_DIR}"
        COMMAND $<TARGET_FILE:yetty-msdf-gen> --all "${FONT_DIR}/DejaVuSansMNerdFontMono-Regular.ttf" "${CDB_OUTPUT_DIR}"
        COMMAND $<TARGET_FILE:yetty-msdf-gen> --all "${FONT_DIR}/DejaVuSansMNerdFontMono-Bold.ttf" "${CDB_OUTPUT_DIR}"
        COMMAND $<TARGET_FILE:yetty-msdf-gen> --all "${FONT_DIR}/DejaVuSansMNerdFontMono-Oblique.ttf" "${CDB_OUTPUT_DIR}"
        COMMAND $<TARGET_FILE:yetty-msdf-gen> --all "${FONT_DIR}/DejaVuSansMNerdFontMono-BoldOblique.ttf" "${CDB_OUTPUT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E touch "${CDB_STAMP_FILE}"
        DEPENDS yetty-msdf-gen ${FONT_FILES}
        COMMENT "Generating MSDF CDB files from TTF fonts"
        VERBATIM
    )
endif()

add_custom_target(generate-cdb DEPENDS ${CDB_STAMP_FILE})
