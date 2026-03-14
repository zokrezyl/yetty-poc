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

# Brotli compression quality (0-11, higher = slower but smaller)
# Default to 6 for good speed/size tradeoff
if(DEFINED ENV{BROTLI_QUALITY})
    set(BROTLI_QUALITY $ENV{BROTLI_QUALITY})
else()
    set(BROTLI_QUALITY 6)
endif()
message(STATUS "incbin: Brotli quality level: ${BROTLI_QUALITY}")

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
    # Debug: Show platform detection
    message(STATUS "incbin: TARGET=${TARGET} MSVC=${MSVC} EMSCRIPTEN=${EMSCRIPTEN} CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")

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

    elseif(EMSCRIPTEN)
        # Emscripten: incbin's inline assembly doesn't work with WASM
        # Provide empty stubs - icons not supported on web
        # Compile as C (not C++) to get external linkage for const by default
        message(STATUS "incbin: Using Emscripten stubs for ${TARGET}")

        # Use .c extension so it compiles as C (const has external linkage in C)
        set(RESOURCE_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_resources.c")

        file(WRITE ${RESOURCE_SOURCE}
"/* Generated stub resources for ${TARGET} (Emscripten) */
/* incbin inline assembly not supported on WASM - providing empty stubs */
/* Compiled as C for external linkage of const variables */

")
        math(EXPR LAST_INDEX "${PAIR_COUNT} - 1")
        foreach(I RANGE 0 ${LAST_INDEX})
            math(EXPR NAME_INDEX "${I} * 2")
            list(GET RESOURCES ${NAME_INDEX} RES_NAME)

            file(APPEND ${RESOURCE_SOURCE}
"const unsigned char g${RES_NAME}Data[] = {0};
const unsigned char* const g${RES_NAME}End = g${RES_NAME}Data;
const unsigned int g${RES_NAME}Size = 0;
")
        endforeach()

        target_sources(${TARGET} PRIVATE ${RESOURCE_SOURCE})

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

#-----------------------------------------------------------------------------
# incbin_add_directory(target PREFIX DIR [PATTERN])
#
# Embeds all files matching PATTERN from DIR with asset names prefixed by PREFIX.
# Default PATTERN is "*" (all files).
#
# Example:
#   incbin_add_directory(yetty "shaders" "${CMAKE_SOURCE_DIR}/src/yetty/shaders" "*.wgsl")
#   # Creates assets: shaders/gpu-screen.wgsl, shaders/effects/wave.wgsl, etc.
#
# Also generates a manifest source file for IncbinAssets registration.
#-----------------------------------------------------------------------------
function(incbin_add_directory TARGET PREFIX DIR)
    set(PATTERN "*")
    set(COMPRESS FALSE)
    if(ARGC GREATER 3)
        set(PATTERN ${ARGV3})
    endif()
    if(ARGC GREATER 4)
        set(COMPRESS ${ARGV4})
    endif()

    # Sanitize PREFIX for C++ identifiers (replace - with _)
    string(REPLACE "-" "_" PREFIX_SAFE "${PREFIX}")

    # Find brotli for compression
    if(COMPRESS)
        find_program(BROTLI_EXECUTABLE brotli)
        if(NOT BROTLI_EXECUTABLE)
            message(WARNING "incbin: brotli not found, embedding uncompressed files")
            set(COMPRESS FALSE)
        endif()
    endif()

    # Find all matching files recursively
    file(GLOB_RECURSE FILES "${DIR}/${PATTERN}")

    if(NOT FILES)
        message(WARNING "incbin_add_directory: No files found matching ${DIR}/${PATTERN}")
        return()
    endif()

    list(LENGTH FILES FILE_COUNT)
    message(STATUS "incbin: Packing ${FILE_COUNT} files from ${DIR} with prefix '${PREFIX}'")

    # Build resource list
    set(MANIFEST_ENTRIES "")

    # Generate source file for this directory (separate from main resources)
    set(RESOURCE_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_${PREFIX}_resources.cpp")

    # Compression output directory
    set(COMPRESSED_DIR "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_${PREFIX_SAFE}_compressed")
    if(COMPRESS)
        file(MAKE_DIRECTORY "${COMPRESSED_DIR}")
    endif()

    if(EMSCRIPTEN)
        # Emscripten: provide stubs
        file(WRITE ${RESOURCE_SOURCE}
"/* Generated stub resources for ${TARGET}/${PREFIX} (Emscripten) */
")
        foreach(FILE ${FILES})
            file(RELATIVE_PATH REL_PATH "${DIR}" "${FILE}")
            string(REPLACE "/" "_" SAFE_NAME "${REL_PATH}")
            string(REPLACE "." "_" SAFE_NAME "${SAFE_NAME}")
            string(REPLACE "-" "_" SAFE_NAME "${SAFE_NAME}")
            set(SYMBOL_NAME "${PREFIX_SAFE}_${SAFE_NAME}")
            set(ASSET_NAME "${PREFIX}/${REL_PATH}")
            list(APPEND MANIFEST_ENTRIES "${SYMBOL_NAME}|${ASSET_NAME}|0")

            file(APPEND ${RESOURCE_SOURCE}
"const unsigned char g${SYMBOL_NAME}Data[] = {0};
const unsigned char* const g${SYMBOL_NAME}End = g${SYMBOL_NAME}Data;
const unsigned int g${SYMBOL_NAME}Size = 0;
")
        endforeach()
    elseif(MSVC)
        # MSVC: Use incbin tool to generate data file
        set(INCBIN_INPUT "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_${PREFIX_SAFE}_incbin_input.c")
        set(INCBIN_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_${PREFIX_SAFE}_data.c")

        file(WRITE ${INCBIN_INPUT} "#include \"incbin.h\"\n")

        foreach(FILE ${FILES})
            file(RELATIVE_PATH REL_PATH "${DIR}" "${FILE}")
            string(REPLACE "/" "_" SAFE_NAME "${REL_PATH}")
            string(REPLACE "." "_" SAFE_NAME "${SAFE_NAME}")
            string(REPLACE "-" "_" SAFE_NAME "${SAFE_NAME}")
            set(SYMBOL_NAME "${PREFIX_SAFE}_${SAFE_NAME}")
            set(ASSET_NAME "${PREFIX}/${REL_PATH}")

            if(COMPRESS)
                get_filename_component(FILE_NAME "${FILE}" NAME)
                set(COMPRESSED_FILE "${COMPRESSED_DIR}/${FILE_NAME}.br")
                execute_process(
                    COMMAND ${BROTLI_EXECUTABLE} -q ${BROTLI_QUALITY} -f -o "${COMPRESSED_FILE}" "${FILE}"
                    RESULT_VARIABLE BROTLI_RESULT
                )
                if(BROTLI_RESULT EQUAL 0)
                    file(APPEND ${INCBIN_INPUT} "INCBIN(${SYMBOL_NAME}, \"${COMPRESSED_FILE}\");\n")
                    list(APPEND MANIFEST_ENTRIES "${SYMBOL_NAME}|${ASSET_NAME}|1")
                else()
                    file(APPEND ${INCBIN_INPUT} "INCBIN(${SYMBOL_NAME}, \"${FILE}\");\n")
                    list(APPEND MANIFEST_ENTRIES "${SYMBOL_NAME}|${ASSET_NAME}|0")
                endif()
            else()
                file(APPEND ${INCBIN_INPUT} "INCBIN(${SYMBOL_NAME}, \"${FILE}\");\n")
                list(APPEND MANIFEST_ENTRIES "${SYMBOL_NAME}|${ASSET_NAME}|0")
            endif()
        endforeach()

        # Run incbin tool at build time
        add_custom_command(
            OUTPUT ${INCBIN_OUTPUT}
            COMMAND incbin_tool -o ${INCBIN_OUTPUT} ${INCBIN_INPUT}
            DEPENDS incbin_tool ${INCBIN_INPUT} ${FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Generating embedded resources for ${TARGET}/${PREFIX}"
        )

        # Create wrapper with extern declarations
        file(WRITE ${RESOURCE_SOURCE}
"// Generated resource declarations for ${TARGET}/${PREFIX} (MSVC)
#include <cstddef>

extern \"C\" {
")
        foreach(FILE ${FILES})
            file(RELATIVE_PATH REL_PATH "${DIR}" "${FILE}")
            string(REPLACE "/" "_" SAFE_NAME "${REL_PATH}")
            string(REPLACE "." "_" SAFE_NAME "${SAFE_NAME}")
            string(REPLACE "-" "_" SAFE_NAME "${SAFE_NAME}")
            set(SYMBOL_NAME "${PREFIX_SAFE}_${SAFE_NAME}")
            file(APPEND ${RESOURCE_SOURCE}
"extern const unsigned char g${SYMBOL_NAME}Data[];
extern const unsigned char* const g${SYMBOL_NAME}End;
extern const unsigned int g${SYMBOL_NAME}Size;
")
        endforeach()
        file(APPEND ${RESOURCE_SOURCE} "}\n")

        target_sources(${TARGET} PRIVATE ${RESOURCE_SOURCE} ${INCBIN_OUTPUT})
        target_include_directories(${TARGET} PRIVATE ${INCBIN_INCLUDE_DIR})

        # Skip the common target_sources below for MSVC
        set(MSVC_HANDLED TRUE)
    else()
        # GCC/Clang: Use incbin with inline assembly
        file(WRITE ${RESOURCE_SOURCE}
"// Generated embedded resources for ${TARGET}/${PREFIX}
#include \"incbin.h\"

")
        foreach(FILE ${FILES})
            file(RELATIVE_PATH REL_PATH "${DIR}" "${FILE}")
            string(REPLACE "/" "_" SAFE_NAME "${REL_PATH}")
            string(REPLACE "." "_" SAFE_NAME "${SAFE_NAME}")
            string(REPLACE "-" "_" SAFE_NAME "${SAFE_NAME}")
            set(SYMBOL_NAME "${PREFIX_SAFE}_${SAFE_NAME}")
            set(ASSET_NAME "${PREFIX}/${REL_PATH}")

            # Compress file if requested
            if(COMPRESS)
                get_filename_component(FILE_NAME "${FILE}" NAME)
                set(COMPRESSED_FILE "${COMPRESSED_DIR}/${FILE_NAME}.br")
                execute_process(
                    COMMAND ${BROTLI_EXECUTABLE} -q ${BROTLI_QUALITY} -f -o "${COMPRESSED_FILE}" "${FILE}"
                    RESULT_VARIABLE BROTLI_RESULT
                )
                if(BROTLI_RESULT EQUAL 0)
                    file(APPEND ${RESOURCE_SOURCE} "INCBIN(${SYMBOL_NAME}, \"${COMPRESSED_FILE}\");\n")
                    list(APPEND MANIFEST_ENTRIES "${SYMBOL_NAME}|${ASSET_NAME}|1")
                    message(STATUS "incbin: Compressed ${FILE_NAME}")
                else()
                    message(WARNING "incbin: Failed to compress ${FILE_NAME}, using uncompressed")
                    file(APPEND ${RESOURCE_SOURCE} "INCBIN(${SYMBOL_NAME}, \"${FILE}\");\n")
                    list(APPEND MANIFEST_ENTRIES "${SYMBOL_NAME}|${ASSET_NAME}|0")
                endif()
            else()
                file(APPEND ${RESOURCE_SOURCE} "INCBIN(${SYMBOL_NAME}, \"${FILE}\");\n")
                list(APPEND MANIFEST_ENTRIES "${SYMBOL_NAME}|${ASSET_NAME}|0")
            endif()
        endforeach()
    endif()

    # Add sources (skip for MSVC as it's handled above with the incbin tool output)
    if(NOT MSVC_HANDLED)
        target_sources(${TARGET} PRIVATE ${RESOURCE_SOURCE})
        target_include_directories(${TARGET} PRIVATE ${INCBIN_INCLUDE_DIR})
    endif()

    # Generate manifest header for asset registration
    set(MANIFEST_HEADER "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_${PREFIX}_manifest.h")

    file(WRITE ${MANIFEST_HEADER}
"// Generated asset manifest for ${PREFIX}
// Auto-generated by incbin.cmake - DO NOT EDIT

#pragma once
#include <cstdint>
#include <string_view>
#include <functional>

")

    # Add extern declarations
    file(APPEND ${MANIFEST_HEADER} "extern \"C\" {\n")
    foreach(ENTRY ${MANIFEST_ENTRIES})
        string(REPLACE "|" ";" PARTS "${ENTRY}")
        list(GET PARTS 0 SYMBOL_NAME)
        file(APPEND ${MANIFEST_HEADER}
"    extern const unsigned char g${SYMBOL_NAME}Data[];
    extern const unsigned int g${SYMBOL_NAME}Size;
")
    endforeach()
    file(APPEND ${MANIFEST_HEADER} "}\n\n")

    # Add registration function (use PREFIX_SAFE for valid C++ identifier)
    # Callback signature: void(const char* name, const uint8_t* data, size_t size, bool compressed)
    file(APPEND ${MANIFEST_HEADER}
"// Register all ${PREFIX} assets with a callback
// Callback signature: void(const char* name, const uint8_t* data, size_t size, bool compressed)
template<typename Callback>
inline void register_${PREFIX_SAFE}_assets(Callback&& cb) {
")

    foreach(ENTRY ${MANIFEST_ENTRIES})
        string(REPLACE "|" ";" PARTS "${ENTRY}")
        list(GET PARTS 0 SYMBOL_NAME)
        list(GET PARTS 1 ASSET_NAME)
        list(GET PARTS 2 IS_COMPRESSED)
        if(IS_COMPRESSED)
            set(COMPRESSED_BOOL "true")
        else()
            set(COMPRESSED_BOOL "false")
        endif()
        file(APPEND ${MANIFEST_HEADER}
"    cb(\"${ASSET_NAME}\", g${SYMBOL_NAME}Data, static_cast<size_t>(g${SYMBOL_NAME}Size), ${COMPRESSED_BOOL});
")
    endforeach()

    file(APPEND ${MANIFEST_HEADER} "}\n")

    # Export manifest path for the target
    set(INCBIN_${PREFIX}_MANIFEST ${MANIFEST_HEADER} PARENT_SCOPE)

    # Add compile definition so the code knows this manifest exists
    string(TOUPPER "${PREFIX_SAFE}" PREFIX_UPPER)
    target_compile_definitions(${TARGET} PRIVATE HAS_${PREFIX_UPPER}_MANIFEST=1)

    message(STATUS "incbin: Generated manifest ${MANIFEST_HEADER}")
endfunction()

message(STATUS "incbin: Ready for resource embedding")
