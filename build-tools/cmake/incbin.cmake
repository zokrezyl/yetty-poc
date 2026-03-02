# incbin.cmake - Include binary files in executables
#=============================================================================
# Cross-platform binary resource embedding using incbin.
# - GCC/Clang: Uses inline assembly via incbin.h
# - MSVC: Uses incbin tool to generate data.c
#
# Usage:
#   include(incbin)
#   incbin_add_resources(target
#       Logo "path/to/logo.jpeg"
#       Font "path/to/font.cdb"
#   )
#
# This creates symbols: gLogoData, gLogoSize, gFontData, gFontSize, etc.
#=============================================================================

include_guard(GLOBAL)

# Download incbin from GitHub
CPMAddPackage(
    NAME incbin
    GITHUB_REPOSITORY graphitemaster/incbin
    GIT_TAG main
    DOWNLOAD_ONLY YES
)

if(NOT incbin_ADDED)
    message(FATAL_ERROR "Failed to download incbin")
endif()

# Export incbin include directory
set(INCBIN_INCLUDE_DIR ${incbin_SOURCE_DIR} CACHE INTERNAL "incbin include directory")

# For MSVC, build the incbin tool
if(MSVC)
    add_executable(incbin_tool ${incbin_SOURCE_DIR}/incbin.c)
    set_target_properties(incbin_tool PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tools
    )
endif()

#-----------------------------------------------------------------------------
# incbin_add_resources(target NAME1 FILE1 [NAME2 FILE2 ...])
#
# Embeds binary resources into a target.
# Creates global symbols: g<NAME>Data, g<NAME>Size, g<NAME>End
#-----------------------------------------------------------------------------
function(incbin_add_resources TARGET)
    # Parse arguments as name-file pairs
    set(RESOURCES ${ARGN})
    list(LENGTH RESOURCES RESOURCES_LEN)
    math(EXPR PAIR_COUNT "${RESOURCES_LEN} / 2")

    if(${RESOURCES_LEN} EQUAL 0)
        message(FATAL_ERROR "incbin_add_resources: No resources specified")
    endif()

    math(EXPR REMAINDER "${RESOURCES_LEN} % 2")
    if(NOT ${REMAINDER} EQUAL 0)
        message(FATAL_ERROR "incbin_add_resources: Arguments must be NAME FILE pairs")
    endif()

    # Generate resource source file
    set(RESOURCE_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_resources.cpp")

    if(MSVC)
        # MSVC: Use incbin tool to generate resource file
        set(INCBIN_ARGS "")
        set(RESOURCE_FILES "")

        math(EXPR LAST_INDEX "${PAIR_COUNT} - 1")
        foreach(I RANGE 0 ${LAST_INDEX})
            math(EXPR NAME_INDEX "${I} * 2")
            math(EXPR FILE_INDEX "${I} * 2 + 1")
            list(GET RESOURCES ${NAME_INDEX} RES_NAME)
            list(GET RESOURCES ${FILE_INDEX} RES_FILE)

            # Make path absolute if relative
            if(NOT IS_ABSOLUTE ${RES_FILE})
                set(RES_FILE "${CMAKE_SOURCE_DIR}/${RES_FILE}")
            endif()

            list(APPEND RESOURCE_FILES ${RES_FILE})
        endforeach()

        # Create a source file that includes incbin.h with the resources
        # The incbin tool will process this and generate data.c
        set(INCBIN_INPUT "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_incbin_input.c")
        set(INCBIN_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_resources_data.c")

        # Write the input file for incbin tool
        file(WRITE ${INCBIN_INPUT} "#include \"incbin.h\"\n")

        math(EXPR LAST_INDEX "${PAIR_COUNT} - 1")
        foreach(I RANGE 0 ${LAST_INDEX})
            math(EXPR NAME_INDEX "${I} * 2")
            math(EXPR FILE_INDEX "${I} * 2 + 1")
            list(GET RESOURCES ${NAME_INDEX} RES_NAME)
            list(GET RESOURCES ${FILE_INDEX} RES_FILE)

            if(NOT IS_ABSOLUTE ${RES_FILE})
                set(RES_FILE "${CMAKE_SOURCE_DIR}/${RES_FILE}")
            endif()

            file(APPEND ${INCBIN_INPUT} "INCBIN(${RES_NAME}, \"${RES_FILE}\");\n")
        endforeach()

        # Run incbin tool at build time
        add_custom_command(
            OUTPUT ${INCBIN_OUTPUT}
            COMMAND incbin_tool -o ${INCBIN_OUTPUT} ${INCBIN_INPUT}
            DEPENDS incbin_tool ${INCBIN_INPUT} ${RESOURCE_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Generating embedded resources for ${TARGET}"
        )

        # Create wrapper source that declares extern symbols
        file(WRITE ${RESOURCE_SOURCE}
"// Generated resource declarations for ${TARGET}
#include <cstddef>

extern \"C\" {
")
        math(EXPR LAST_INDEX "${PAIR_COUNT} - 1")
        foreach(I RANGE 0 ${LAST_INDEX})
            math(EXPR NAME_INDEX "${I} * 2")
            list(GET RESOURCES ${NAME_INDEX} RES_NAME)
            file(APPEND ${RESOURCE_SOURCE}
"extern const unsigned char g${RES_NAME}Data[];
extern const unsigned char* const g${RES_NAME}End;
extern const unsigned int g${RES_NAME}Size;
")
        endforeach()
        file(APPEND ${RESOURCE_SOURCE} "}\n")

        target_sources(${TARGET} PRIVATE ${RESOURCE_SOURCE} ${INCBIN_OUTPUT})
        target_include_directories(${TARGET} PRIVATE ${INCBIN_INCLUDE_DIR})

    else()
        # GCC/Clang: Use incbin.h directly with inline assembly
        file(WRITE ${RESOURCE_SOURCE}
"// Generated embedded resources for ${TARGET}
// Using incbin for inline assembly resource embedding

#include \"incbin.h\"

")
        math(EXPR LAST_INDEX "${PAIR_COUNT} - 1")
        foreach(I RANGE 0 ${LAST_INDEX})
            math(EXPR NAME_INDEX "${I} * 2")
            math(EXPR FILE_INDEX "${I} * 2 + 1")
            list(GET RESOURCES ${NAME_INDEX} RES_NAME)
            list(GET RESOURCES ${FILE_INDEX} RES_FILE)

            # Make path absolute if relative
            if(NOT IS_ABSOLUTE ${RES_FILE})
                set(RES_FILE "${CMAKE_SOURCE_DIR}/${RES_FILE}")
            endif()

            file(APPEND ${RESOURCE_SOURCE} "INCBIN(${RES_NAME}, \"${RES_FILE}\");\n")
        endforeach()

        target_sources(${TARGET} PRIVATE ${RESOURCE_SOURCE})
        target_include_directories(${TARGET} PRIVATE ${INCBIN_INCLUDE_DIR})
    endif()
endfunction()

message(STATUS "incbin: Ready for resource embedding")
