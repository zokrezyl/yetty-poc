# prepare-assets.cmake
# Auto-generates MSDF CDB fonts at configure time if they don't exist
# This runs BEFORE incbin so the GLOB finds the CDB files

set(PREBUILT_CDB_DIR "${YETTY_ROOT}/assets/fonts-cdb")
set(FONT_DIR "${YETTY_ROOT}/assets")

set(CDB_FILES
    "DejaVuSansMNerdFontMono-Regular.cdb"
    "DejaVuSansMNerdFontMono-Bold.cdb"
    "DejaVuSansMNerdFontMono-Oblique.cdb"
    "DejaVuSansMNerdFontMono-BoldOblique.cdb"
)

set(TTF_FILES
    "${FONT_DIR}/DejaVuSansMNerdFontMono-Regular.ttf"
    "${FONT_DIR}/DejaVuSansMNerdFontMono-Bold.ttf"
    "${FONT_DIR}/DejaVuSansMNerdFontMono-Oblique.ttf"
    "${FONT_DIR}/DejaVuSansMNerdFontMono-BoldOblique.ttf"
)

# Check if all CDB files exist
set(ALL_CDBS_EXIST TRUE)
foreach(CDB_FILE ${CDB_FILES})
    if(NOT EXISTS "${PREBUILT_CDB_DIR}/${CDB_FILE}")
        set(ALL_CDBS_EXIST FALSE)
        break()
    endif()
endforeach()

if(ALL_CDBS_EXIST)
    message(STATUS "MSDF CDB fonts found in ${PREBUILT_CDB_DIR}")
else()
    message(STATUS "MSDF CDB fonts not found - generating at configure time...")

    # Build directory for host tools
    set(HOST_TOOLS_BUILD_DIR "${CMAKE_BINARY_DIR}/_host-tools-bootstrap")
    set(HOST_MSDF_GEN "${HOST_TOOLS_BUILD_DIR}/yetty-msdf-gen")

    # Configure host tools
    message(STATUS "  Configuring host tools...")
    execute_process(
        COMMAND ${CMAKE_COMMAND}
            -S "${YETTY_ROOT}/build-tools/cmake/host-tools"
            -B "${HOST_TOOLS_BUILD_DIR}"
            -G Ninja
            -DCMAKE_BUILD_TYPE=Release
        RESULT_VARIABLE CONFIG_RESULT
        OUTPUT_VARIABLE CONFIG_OUTPUT
        ERROR_VARIABLE CONFIG_ERROR
    )
    if(NOT CONFIG_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to configure host tools:\n${CONFIG_ERROR}")
    endif()

    # Build yetty-msdf-gen
    message(STATUS "  Building yetty-msdf-gen...")
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build "${HOST_TOOLS_BUILD_DIR}" --target yetty-msdf-gen --parallel
        RESULT_VARIABLE BUILD_RESULT
        OUTPUT_VARIABLE BUILD_OUTPUT
        ERROR_VARIABLE BUILD_ERROR
    )
    if(NOT BUILD_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to build yetty-msdf-gen:\n${BUILD_ERROR}")
    endif()

    # Generate CDB files
    file(MAKE_DIRECTORY "${PREBUILT_CDB_DIR}")

    foreach(TTF_FILE ${TTF_FILES})
        get_filename_component(TTF_NAME "${TTF_FILE}" NAME)
        message(STATUS "  Generating CDB for ${TTF_NAME}...")
        execute_process(
            COMMAND "${HOST_MSDF_GEN}" --all "${TTF_FILE}" "${PREBUILT_CDB_DIR}"
            RESULT_VARIABLE GEN_RESULT
            OUTPUT_VARIABLE GEN_OUTPUT
            ERROR_VARIABLE GEN_ERROR
        )
        if(NOT GEN_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to generate CDB for ${TTF_NAME}:\n${GEN_ERROR}")
        endif()
    endforeach()

    message(STATUS "MSDF CDB fonts generated in ${PREBUILT_CDB_DIR}")
endif()
