cmake_minimum_required(VERSION 3.20)
project(libweb-to-ydraw LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use ccache + distcc for faster builds
find_program(CCACHE_PROGRAM ccache)
find_program(DISTCC_PROGRAM distcc)
if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    if(DISTCC_PROGRAM)
        set(ENV{CCACHE_PREFIX} "distcc")
        message(STATUS "Using ccache + distcc for distributed builds")
    else()
        message(STATUS "Using ccache")
    endif()
endif()

# Compiler flags
add_compile_options(-Wall -Wextra -fPIC)

# Include paths
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Find dependencies
find_package(CURL QUIET)

# === YDraw Buffer Standalone (no Ladybird dependencies) ===
add_library(ydraw-buffer STATIC
    ydraw-buffer-standalone.cpp
)
target_include_directories(ydraw-buffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# === Test executable (standalone display list) ===
add_executable(ladybird-ydraw
    main.cpp
)
target_link_libraries(ladybird-ydraw PRIVATE ydraw-buffer)


# === LibWeb integration (requires Ladybird source) ===
option(BUILD_LIBWEB "Build LibWeb integration" OFF)
set(LADYBIRD_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../tmp/ladybird" CACHE PATH "Path to Ladybird source")

if(BUILD_LIBWEB)
    # Verify Ladybird source exists
    if(NOT EXISTS "${LADYBIRD_SOURCE_DIR}/Libraries/LibWeb")
        message(FATAL_ERROR "Ladybird source not found at ${LADYBIRD_SOURCE_DIR}. Clone it to tmp/ladybird/")
    endif()
    message(STATUS "Using Ladybird source: ${LADYBIRD_SOURCE_DIR}")

    # Find required dependencies
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SIMDUTF REQUIRED simdutf)
    pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)
    find_package(fmt REQUIRED)
    find_package(ICU REQUIRED COMPONENTS uc i18n data)

    # Find fast_float (header-only library)
    # In nix, standard include paths are already set
    find_path(FAST_FLOAT_INCLUDE_DIR fast_float/fast_float.h)
    if(NOT FAST_FLOAT_INCLUDE_DIR)
        message(FATAL_ERROR "fast_float not found. Add fast-float to nix dependencies.")
    endif()
    message(STATUS "Found fast_float: ${FAST_FLOAT_INCLUDE_DIR}")

    # Generated files directory
    set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
    file(MAKE_DIRECTORY ${GENERATED_DIR})
    file(MAKE_DIRECTORY ${GENERATED_DIR}/LibURL)
    file(MAKE_DIRECTORY ${GENERATED_DIR}/LibUnicode)

    # === Generate PublicSuffixData ===
    set(PUBLIC_SUFFIX_URL "https://raw.githubusercontent.com/publicsuffix/list/master/public_suffix_list.dat")
    set(PUBLIC_SUFFIX_DAT "${CMAKE_CURRENT_BINARY_DIR}/public_suffix_list.dat")
    set(PUBLIC_SUFFIX_HEADER "${GENERATED_DIR}/LibURL/PublicSuffixData.h")
    set(PUBLIC_SUFFIX_SOURCE "${GENERATED_DIR}/LibURL/PublicSuffixData.cpp")

    # Check if pre-fetched in source dir (for nix sandboxed builds)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/public_suffix_list.dat")
        message(STATUS "Using pre-fetched public_suffix_list.dat from source dir")
        file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/public_suffix_list.dat" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
    elseif(NOT EXISTS ${PUBLIC_SUFFIX_DAT})
        # Download public suffix list at configure time
        message(STATUS "Downloading public_suffix_list.dat...")
        file(DOWNLOAD ${PUBLIC_SUFFIX_URL} ${PUBLIC_SUFFIX_DAT}
             STATUS DOWNLOAD_STATUS
             TIMEOUT 60)
        list(GET DOWNLOAD_STATUS 0 DOWNLOAD_CODE)
        if(NOT DOWNLOAD_CODE EQUAL 0)
            message(FATAL_ERROR "Failed to download public_suffix_list.dat")
        endif()
    endif()

    # Generate PublicSuffixData at configure time
    if(NOT EXISTS ${PUBLIC_SUFFIX_HEADER})
        message(STATUS "Generating PublicSuffixData...")
        execute_process(
            COMMAND python3 "${LADYBIRD_SOURCE_DIR}/Meta/generate_public_suffix_data.py"
                    -p "${PUBLIC_SUFFIX_DAT}"
                    -h "${PUBLIC_SUFFIX_HEADER}"
                    -c "${PUBLIC_SUFFIX_SOURCE}"
            RESULT_VARIABLE GEN_RESULT
        )
        if(NOT GEN_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to generate PublicSuffixData")
        endif()
    endif()

    # === Common include directories ===
    # Stubs directory first to override generated Export.h files
    set(LADYBIRD_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/stubs
        ${LADYBIRD_SOURCE_DIR}
        ${LADYBIRD_SOURCE_DIR}/Libraries
        ${GENERATED_DIR}
        ${SIMDUTF_INCLUDE_DIRS}
        ${FAST_FLOAT_INCLUDE_DIR}
        ${LIBXML2_INCLUDE_DIRS}
    )

    # === Common compile definitions ===
    set(LADYBIRD_COMPILE_DEFS
        AK_DONT_REPLACE_STD=1
    )

    # === Generate AK headers ===
    file(MAKE_DIRECTORY ${GENERATED_DIR}/AK)

    # Set all debug flags to 0
    set(AK_STRINGBASE_VERIFY_LAUNDER_DEBUG 0)
    set(AUDIO_DEBUG 0)
    set(BMP_DEBUG 0)
    set(BINDINGS_GENERATOR_DEBUG 0)
    set(CACHE_DEBUG 0)
    set(CALLBACK_MACHINE_DEBUG 0)
    set(CANVAS_RENDERING_CONTEXT_2D_DEBUG 0)
    set(CRYPTO_DEBUG 0)
    set(CSS_LOADER_DEBUG 0)
    set(CSS_PARSER_DEBUG 0)
    set(CSS_TOKENIZER_DEBUG 0)
    set(CSS_TRANSITIONS_DEBUG 0)
    set(CURL_DEBUG 0)
    set(DEVTOOLS_DEBUG 0)
    set(DNS_DEBUG 0)
    set(EDITOR_DEBUG 0)
    set(EMOJI_DEBUG 0)
    set(FILE_WATCHER_DEBUG 0)
    set(FLAC_ENCODER_DEBUG 0)
    set(FORMATTING_CONTEXT_TRACE_DEBUG 0)
    set(GENERATE_DEBUG 0)
    set(GHASH_PROCESS_DEBUG 0)
    set(GIF_DEBUG 0)
    set(HEAP_DEBUG 0)
    set(HIGHLIGHT_FOCUSED_FRAME_DEBUG 0)
    set(HTML_SCRIPT_DEBUG 0)
    set(HTTP_DISK_CACHE_DEBUG 0)
    set(HTTP_MEMORY_CACHE_DEBUG 0)
    set(HTTPJOB_DEBUG 0)
    set(HUNKS_DEBUG 0)
    set(ICO_DEBUG 0)
    set(IDB_DEBUG 0)
    set(IDL_DEBUG 0)
    set(IMAGE_DECODER_DEBUG 0)
    set(IMAGE_LOADER_DEBUG 0)
    set(JOB_DEBUG 0)
    set(JS_BYTECODE_DEBUG 0)
    set(JS_MODULE_DEBUG 0)
    set(LEXER_DEBUG 0)
    set(LIBWEB_CSS_ANIMATION_DEBUG 0)
    set(LIBWEB_CSS_DEBUG 0)
    set(LIBWEB_WASM_DEBUG 0)
    set(LINE_EDITOR_DEBUG 0)
    set(LZW_DEBUG 0)
    set(MACH_PORT_DEBUG 0)
    set(MATROSKA_DEBUG 0)
    set(MATROSKA_TRACE_DEBUG 0)
    set(NETWORKJOB_DEBUG 0)
    set(NT_DEBUG 0)
    set(OPENTYPE_GPOS_DEBUG 0)
    set(HTML_PARSER_DEBUG 0)
    set(PATH_DEBUG 0)
    set(PLAYBACK_MANAGER_DEBUG 0)
    set(PNG_DEBUG 0)
    set(PROMISE_DEBUG 0)
    set(REGEX_DEBUG 0)
    set(REQUESTSERVER_DEBUG 0)
    set(RESOURCE_DEBUG 0)
    set(RSA_PARSE_DEBUG 0)
    set(SHARED_QUEUE_DEBUG 0)
    set(SPAM_DEBUG 0)
    set(SYNTAX_HIGHLIGHTING_DEBUG 0)
    set(STYLE_INVALIDATION_DEBUG 0)
    set(TEXTEDITOR_DEBUG 0)
    set(TIFF_DEBUG 0)
    set(TIME_ZONE_DEBUG 0)
    set(TLS_DEBUG 0)
    set(TOKENIZER_TRACE_DEBUG 0)
    set(UPDATE_LAYOUT_DEBUG 0)
    set(URL_PARSER_DEBUG 0)
    set(URL_PATTERN_DEBUG 0)
    set(UTF8_DEBUG 0)
    set(VPX_DEBUG 0)
    set(VULKAN_VALIDATION_LAYERS_DEBUG 0)
    set(WASI_DEBUG 0)
    set(WASI_FINE_GRAINED_DEBUG 0)
    set(WASM_BINPARSER_DEBUG 0)
    set(WASM_TRACE_DEBUG 0)
    set(WASM_VALIDATOR_DEBUG 0)
    set(WEBDRIVER_DEBUG 0)
    set(WEBDRIVER_ROUTE_DEBUG 0)
    set(WEBGL_CONTEXT_DEBUG 0)
    set(WEBVIEW_PROCESS_DEBUG 0)
    set(WEB_FETCH_DEBUG 0)
    set(WEB_WORKER_DEBUG 0)
    set(WEBP_DEBUG 0)
    set(XML_PARSER_DEBUG 0)

    configure_file("${LADYBIRD_SOURCE_DIR}/AK/Debug.h.in" "${GENERATED_DIR}/AK/Debug.h" @ONLY)

    # Backtrace settings
    set(BACKTRACE_HEADER "<execinfo.h>")
    set(HAVE_BACKTRACE 1)
    configure_file("${LADYBIRD_SOURCE_DIR}/AK/Backtrace.h.in" "${GENERATED_DIR}/AK/Backtrace.h" @ONLY)

    # === AK Library ===
    file(GLOB AK_SOURCES "${LADYBIRD_SOURCE_DIR}/AK/*.cpp")
    list(FILTER AK_SOURCES EXCLUDE REGEX ".*Windows.*")
    add_library(ak STATIC ${AK_SOURCES})
    target_include_directories(ak PUBLIC ${LADYBIRD_INCLUDE_DIRS})
    target_link_libraries(ak PUBLIC ${SIMDUTF_LIBRARIES} fmt::fmt)
    target_compile_definitions(ak PUBLIC ${LADYBIRD_COMPILE_DEFS})

    # === LibMain ===
    add_library(main STATIC "${LADYBIRD_SOURCE_DIR}/Libraries/LibMain/Main.cpp")
    target_include_directories(main PUBLIC ${LADYBIRD_INCLUDE_DIRS})
    target_link_libraries(main PUBLIC ak)

    # === LibGC (Garbage Collector) ===
    file(GLOB LIBGC_SOURCES "${LADYBIRD_SOURCE_DIR}/Libraries/LibGC/*.cpp")
    add_library(gc STATIC ${LIBGC_SOURCES})
    target_include_directories(gc PUBLIC ${LADYBIRD_INCLUDE_DIRS})
    target_link_libraries(gc PUBLIC ak)

    # === LibCore ===
    file(GLOB LIBCORE_SOURCES "${LADYBIRD_SOURCE_DIR}/Libraries/LibCore/*.cpp")
    list(FILTER LIBCORE_SOURCES EXCLUDE REGEX ".*Windows.*")
    list(FILTER LIBCORE_SOURCES EXCLUDE REGEX ".*Darwin.*")
    list(FILTER LIBCORE_SOURCES EXCLUDE REGEX ".*Mach.*")
    list(FILTER LIBCORE_SOURCES EXCLUDE REGEX ".*/IOSurface\\.cpp$")
    add_library(core STATIC ${LIBCORE_SOURCES})
    target_include_directories(core PUBLIC ${LADYBIRD_INCLUDE_DIRS})
    target_link_libraries(core PUBLIC ak main)

    # === LibURL ===
    file(GLOB LIBURL_SOURCES "${LADYBIRD_SOURCE_DIR}/Libraries/LibURL/*.cpp")
    add_library(url STATIC ${LIBURL_SOURCES} ${PUBLIC_SUFFIX_SOURCE})
    target_include_directories(url PUBLIC ${LADYBIRD_INCLUDE_DIRS})
    target_link_libraries(url PUBLIC ak)

    # === LibTextCodec ===
    # Generate LookupTables.h/cpp
    set(TEXTCODEC_LOOKUP_HEADER "${GENERATED_DIR}/LibTextCodec/LookupTables.h")
    set(TEXTCODEC_LOOKUP_SOURCE "${GENERATED_DIR}/LibTextCodec/LookupTables.cpp")
    file(MAKE_DIRECTORY ${GENERATED_DIR}/LibTextCodec)
    if(NOT EXISTS ${TEXTCODEC_LOOKUP_HEADER})
        message(STATUS "Generating LibTextCodec LookupTables...")
        execute_process(
            COMMAND python3 "${LADYBIRD_SOURCE_DIR}/Meta/generate_encoding_indexes.py"
                    -j "${LADYBIRD_SOURCE_DIR}/Libraries/LibTextCodec/indexes.json"
                    -h "${TEXTCODEC_LOOKUP_HEADER}"
                    -c "${TEXTCODEC_LOOKUP_SOURCE}"
            RESULT_VARIABLE GEN_RESULT
        )
        if(NOT GEN_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to generate LibTextCodec LookupTables")
        endif()
    endif()
    file(GLOB LIBTEXTCODEC_SOURCES "${LADYBIRD_SOURCE_DIR}/Libraries/LibTextCodec/*.cpp")
    add_library(textcodec STATIC ${LIBTEXTCODEC_SOURCES} ${TEXTCODEC_LOOKUP_SOURCE})
    target_include_directories(textcodec PUBLIC ${LADYBIRD_INCLUDE_DIRS})
    target_link_libraries(textcodec PUBLIC ak)

    # === LibUnicode ===
    file(GLOB LIBUNICODE_SOURCES "${LADYBIRD_SOURCE_DIR}/Libraries/LibUnicode/*.cpp")
    add_library(unicode STATIC ${LIBUNICODE_SOURCES})
    target_include_directories(unicode PUBLIC ${LADYBIRD_INCLUDE_DIRS})
    target_link_libraries(unicode PUBLIC ak ICU::uc ICU::i18n ICU::data)

    # === LibThreading ===
    file(GLOB LIBTHREADING_SOURCES "${LADYBIRD_SOURCE_DIR}/Libraries/LibThreading/*.cpp")
    add_library(threading STATIC ${LIBTHREADING_SOURCES})
    target_include_directories(threading PUBLIC ${LADYBIRD_INCLUDE_DIRS})
    target_link_libraries(threading PUBLIC ak core)

    # === LibIPC ===
    file(GLOB LIBIPC_SOURCES "${LADYBIRD_SOURCE_DIR}/Libraries/LibIPC/*.cpp")
    list(FILTER LIBIPC_SOURCES EXCLUDE REGEX ".*Windows.*")
    add_library(ipc STATIC ${LIBIPC_SOURCES})
    target_include_directories(ipc PUBLIC ${LADYBIRD_INCLUDE_DIRS})
    target_link_libraries(ipc PUBLIC ak core threading)

    # === LibGfx (minimal - no Skia/Vulkan) ===
    # Generate TIFF metadata
    file(MAKE_DIRECTORY ${GENERATED_DIR}/LibGfx/ImageFormats)
    set(TIFF_METADATA_H "${GENERATED_DIR}/LibGfx/ImageFormats/TIFFMetadata.h")
    set(TIFF_TAGHANDLER_CPP "${GENERATED_DIR}/LibGfx/ImageFormats/TIFFTagHandler.cpp")
    if(NOT EXISTS ${TIFF_METADATA_H})
        message(STATUS "Generating TIFF metadata...")
        execute_process(
            COMMAND python3 "${LADYBIRD_SOURCE_DIR}/Libraries/LibGfx/TIFFGenerator.py"
                    -o "${GENERATED_DIR}/LibGfx/ImageFormats"
            RESULT_VARIABLE GEN_RESULT
        )
        if(NOT GEN_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to generate TIFF metadata")
        endif()
    endif()

    file(GLOB LIBGFX_SOURCES "${LADYBIRD_SOURCE_DIR}/Libraries/LibGfx/*.cpp")
    file(GLOB LIBGFX_FONT_SOURCES "${LADYBIRD_SOURCE_DIR}/Libraries/LibGfx/Font/*.cpp")
    file(GLOB LIBGFX_IMAGE_SOURCES "${LADYBIRD_SOURCE_DIR}/Libraries/LibGfx/ImageFormats/*.cpp")
    # Exclude all Skia/Vulkan-dependent files
    list(FILTER LIBGFX_SOURCES EXCLUDE REGEX ".*Skia.*")
    list(FILTER LIBGFX_SOURCES EXCLUDE REGEX ".*Vulkan.*")
    list(FILTER LIBGFX_SOURCES EXCLUDE REGEX ".*/Bitmap\\.cpp$")
    list(FILTER LIBGFX_SOURCES EXCLUDE REGEX ".*/ColorSpace\\.cpp$")
    list(FILTER LIBGFX_SOURCES EXCLUDE REGEX ".*/Filter\\.cpp$")
    list(FILTER LIBGFX_SOURCES EXCLUDE REGEX ".*/ImmutableBitmap\\.cpp$")
    list(FILTER LIBGFX_SOURCES EXCLUDE REGEX ".*/Painter\\.cpp$")
    list(FILTER LIBGFX_SOURCES EXCLUDE REGEX ".*/PaintingSurface\\.cpp$")
    list(FILTER LIBGFX_SOURCES EXCLUDE REGEX ".*/Path\\.cpp$")
    list(FILTER LIBGFX_SOURCES EXCLUDE REGEX ".*/TextLayout\\.cpp$")
    list(FILTER LIBGFX_SOURCES EXCLUDE REGEX ".*/VectorGraphic\\.cpp$")
    list(FILTER LIBGFX_SOURCES EXCLUDE REGEX ".*/YUVData\\.cpp$")
    list(FILTER LIBGFX_FONT_SOURCES EXCLUDE REGEX ".*Skia.*")
    list(FILTER LIBGFX_FONT_SOURCES EXCLUDE REGEX ".*/Font\\.cpp$")
    list(FILTER LIBGFX_FONT_SOURCES EXCLUDE REGEX ".*/FontDatabase\\.cpp$")
    list(FILTER LIBGFX_FONT_SOURCES EXCLUDE REGEX ".*/Typeface\\.cpp$")
    list(FILTER LIBGFX_FONT_SOURCES EXCLUDE REGEX ".*/FontSupport\\.cpp$")
    list(FILTER LIBGFX_IMAGE_SOURCES EXCLUDE REGEX ".*/AVIFLoader\\.cpp$")
    add_library(gfx STATIC ${LIBGFX_SOURCES} ${LIBGFX_FONT_SOURCES} ${LIBGFX_IMAGE_SOURCES} ${TIFF_TAGHANDLER_CPP}
        "${CMAKE_CURRENT_SOURCE_DIR}/LibGfx-stubs.cpp")
    target_include_directories(gfx PUBLIC ${LADYBIRD_INCLUDE_DIRS})
    target_link_libraries(gfx PUBLIC ak unicode ipc)
    target_compile_definitions(gfx PRIVATE LIBGFX_NO_SKIA=1)

    # === LibFileSystem ===
    add_library(filesystem STATIC "${LADYBIRD_SOURCE_DIR}/Libraries/LibFileSystem/FileSystem.cpp")
    target_include_directories(filesystem PUBLIC ${LADYBIRD_INCLUDE_DIRS})
    target_link_libraries(filesystem PUBLIC ak core)

    # === LibIDL (IDL parser for code generators) ===
    file(GLOB LIBIDL_SOURCES "${LADYBIRD_SOURCE_DIR}/Libraries/LibIDL/*.cpp")
    add_library(idl STATIC ${LIBIDL_SOURCES})
    target_include_directories(idl PUBLIC ${LADYBIRD_INCLUDE_DIRS})
    target_link_libraries(idl PUBLIC ak filesystem)

    # === Code Generators ===
    set(CODEGEN_DIR "${LADYBIRD_SOURCE_DIR}/Meta/Lagom/Tools/CodeGenerators/LibWeb")

    # Simple generators (need AK, Main, Core)
    foreach(GEN_NAME
        GenerateCSSDescriptors
        GenerateCSSEnums
        GenerateCSSEnvironmentVariable
        GenerateCSSKeyword
        GenerateCSSMathFunctions
        GenerateCSSMediaFeatureID
        GenerateCSSPropertyID
        GenerateCSSPseudoClass
        GenerateCSSPseudoElement
        GenerateCSSTransformFunctions
        GenerateCSSUnits
        GenerateCSSStyleProperties
        GenerateCSSNumericFactoryMethods
        GenerateAriaRoles
        GenerateNamedCharacterReferences
    )
        add_executable(${GEN_NAME} "${CODEGEN_DIR}/${GEN_NAME}.cpp")
        target_include_directories(${GEN_NAME} PRIVATE ${LADYBIRD_INCLUDE_DIRS})
        target_link_libraries(${GEN_NAME} PRIVATE ak main core)
    endforeach()

    # WindowOrWorkerInterfaces generator needs LibIDL
    add_executable(GenerateWindowOrWorkerInterfaces "${CODEGEN_DIR}/GenerateWindowOrWorkerInterfaces.cpp")
    target_include_directories(GenerateWindowOrWorkerInterfaces PRIVATE ${LADYBIRD_INCLUDE_DIRS})
    target_link_libraries(GenerateWindowOrWorkerInterfaces PRIVATE ak main core idl)

    # IDL Bindings Generator
    add_executable(BindingsGenerator
        "${CODEGEN_DIR}/BindingsGenerator/main.cpp"
        "${CODEGEN_DIR}/BindingsGenerator/IDLGenerators.cpp"
    )
    target_include_directories(BindingsGenerator PRIVATE ${LADYBIRD_INCLUDE_DIRS} "${CODEGEN_DIR}/BindingsGenerator")
    target_link_libraries(BindingsGenerator PRIVATE ak main core idl)

    # === Generate CSS files ===
    set(LIBWEB_INPUT "${LADYBIRD_SOURCE_DIR}/Libraries/LibWeb")
    file(MAKE_DIRECTORY ${GENERATED_DIR}/LibWeb/CSS)
    file(MAKE_DIRECTORY ${GENERATED_DIR}/LibWeb/HTML/Parser)
    file(MAKE_DIRECTORY ${GENERATED_DIR}/LibWeb/ARIA)
    file(MAKE_DIRECTORY ${GENERATED_DIR}/LibWeb/Bindings)

    # CSS Generators - run at configure time
    macro(run_css_generator GEN_NAME JSON_FILE OUT_H OUT_CPP)
        if(NOT EXISTS "${GENERATED_DIR}/LibWeb/${OUT_H}")
            message(STATUS "Generating ${OUT_H}...")
            execute_process(
                COMMAND $<TARGET_FILE:${GEN_NAME}> -j "${LIBWEB_INPUT}/${JSON_FILE}"
                        -h "${GENERATED_DIR}/LibWeb/${OUT_H}"
                        -c "${GENERATED_DIR}/LibWeb/${OUT_CPP}"
                RESULT_VARIABLE GEN_RESULT
            )
        endif()
    endmacro()

    # We need to build generators first, then run them
    # Use custom commands for proper dependency tracking
    add_custom_command(
        OUTPUT "${GENERATED_DIR}/LibWeb/CSS/Enums.h" "${GENERATED_DIR}/LibWeb/CSS/Enums.cpp"
        COMMAND GenerateCSSEnums -j "${LIBWEB_INPUT}/CSS/Enums.json"
                -h "${GENERATED_DIR}/LibWeb/CSS/Enums.h"
                -c "${GENERATED_DIR}/LibWeb/CSS/Enums.cpp"
        DEPENDS GenerateCSSEnums "${LIBWEB_INPUT}/CSS/Enums.json"
    )

    add_custom_command(
        OUTPUT "${GENERATED_DIR}/LibWeb/CSS/Keyword.h" "${GENERATED_DIR}/LibWeb/CSS/Keyword.cpp"
        COMMAND GenerateCSSKeyword -j "${LIBWEB_INPUT}/CSS/Keywords.json"
                -h "${GENERATED_DIR}/LibWeb/CSS/Keyword.h"
                -c "${GENERATED_DIR}/LibWeb/CSS/Keyword.cpp"
        DEPENDS GenerateCSSKeyword "${LIBWEB_INPUT}/CSS/Keywords.json"
    )

    add_custom_command(
        OUTPUT "${GENERATED_DIR}/LibWeb/CSS/PropertyID.h" "${GENERATED_DIR}/LibWeb/CSS/PropertyID.cpp"
        COMMAND GenerateCSSPropertyID -j "${LIBWEB_INPUT}/CSS/Properties.json"
                -e "${LIBWEB_INPUT}/CSS/Enums.json"
                -g "${LIBWEB_INPUT}/CSS/LogicalPropertyGroups.json"
                -h "${GENERATED_DIR}/LibWeb/CSS/PropertyID.h"
                -c "${GENERATED_DIR}/LibWeb/CSS/PropertyID.cpp"
        DEPENDS GenerateCSSPropertyID "${LIBWEB_INPUT}/CSS/Properties.json"
    )

    add_custom_command(
        OUTPUT "${GENERATED_DIR}/LibWeb/CSS/MediaFeatureID.h" "${GENERATED_DIR}/LibWeb/CSS/MediaFeatureID.cpp"
        COMMAND GenerateCSSMediaFeatureID -j "${LIBWEB_INPUT}/CSS/MediaFeatures.json"
                -h "${GENERATED_DIR}/LibWeb/CSS/MediaFeatureID.h"
                -c "${GENERATED_DIR}/LibWeb/CSS/MediaFeatureID.cpp"
        DEPENDS GenerateCSSMediaFeatureID "${LIBWEB_INPUT}/CSS/MediaFeatures.json"
    )

    add_custom_command(
        OUTPUT "${GENERATED_DIR}/LibWeb/CSS/PseudoClass.h" "${GENERATED_DIR}/LibWeb/CSS/PseudoClass.cpp"
        COMMAND GenerateCSSPseudoClass -j "${LIBWEB_INPUT}/CSS/PseudoClasses.json"
                -h "${GENERATED_DIR}/LibWeb/CSS/PseudoClass.h"
                -c "${GENERATED_DIR}/LibWeb/CSS/PseudoClass.cpp"
        DEPENDS GenerateCSSPseudoClass "${LIBWEB_INPUT}/CSS/PseudoClasses.json"
    )

    add_custom_command(
        OUTPUT "${GENERATED_DIR}/LibWeb/CSS/PseudoElement.h" "${GENERATED_DIR}/LibWeb/CSS/PseudoElement.cpp"
        COMMAND GenerateCSSPseudoElement -j "${LIBWEB_INPUT}/CSS/PseudoElements.json"
                -h "${GENERATED_DIR}/LibWeb/CSS/PseudoElement.h"
                -c "${GENERATED_DIR}/LibWeb/CSS/PseudoElement.cpp"
        DEPENDS GenerateCSSPseudoElement "${LIBWEB_INPUT}/CSS/PseudoElements.json"
    )

    add_custom_command(
        OUTPUT "${GENERATED_DIR}/LibWeb/CSS/MathFunctions.h" "${GENERATED_DIR}/LibWeb/CSS/MathFunctions.cpp"
        COMMAND GenerateCSSMathFunctions -j "${LIBWEB_INPUT}/CSS/MathFunctions.json"
                -h "${GENERATED_DIR}/LibWeb/CSS/MathFunctions.h"
                -c "${GENERATED_DIR}/LibWeb/CSS/MathFunctions.cpp"
        DEPENDS GenerateCSSMathFunctions "${LIBWEB_INPUT}/CSS/MathFunctions.json"
    )

    add_custom_command(
        OUTPUT "${GENERATED_DIR}/LibWeb/CSS/TransformFunctions.h" "${GENERATED_DIR}/LibWeb/CSS/TransformFunctions.cpp"
        COMMAND GenerateCSSTransformFunctions -j "${LIBWEB_INPUT}/CSS/TransformFunctions.json"
                -h "${GENERATED_DIR}/LibWeb/CSS/TransformFunctions.h"
                -c "${GENERATED_DIR}/LibWeb/CSS/TransformFunctions.cpp"
        DEPENDS GenerateCSSTransformFunctions "${LIBWEB_INPUT}/CSS/TransformFunctions.json"
    )

    add_custom_command(
        OUTPUT "${GENERATED_DIR}/LibWeb/CSS/Units.h" "${GENERATED_DIR}/LibWeb/CSS/Units.cpp"
        COMMAND GenerateCSSUnits -j "${LIBWEB_INPUT}/CSS/Units.json"
                -h "${GENERATED_DIR}/LibWeb/CSS/Units.h"
                -c "${GENERATED_DIR}/LibWeb/CSS/Units.cpp"
        DEPENDS GenerateCSSUnits "${LIBWEB_INPUT}/CSS/Units.json"
    )

    add_custom_command(
        OUTPUT "${GENERATED_DIR}/LibWeb/CSS/DescriptorID.h" "${GENERATED_DIR}/LibWeb/CSS/DescriptorID.cpp"
        COMMAND GenerateCSSDescriptors -j "${LIBWEB_INPUT}/CSS/Descriptors.json"
                -h "${GENERATED_DIR}/LibWeb/CSS/DescriptorID.h"
                -c "${GENERATED_DIR}/LibWeb/CSS/DescriptorID.cpp"
        DEPENDS GenerateCSSDescriptors "${LIBWEB_INPUT}/CSS/Descriptors.json"
    )

    add_custom_command(
        OUTPUT "${GENERATED_DIR}/LibWeb/CSS/EnvironmentVariable.h" "${GENERATED_DIR}/LibWeb/CSS/EnvironmentVariable.cpp"
        COMMAND GenerateCSSEnvironmentVariable -j "${LIBWEB_INPUT}/CSS/EnvironmentVariables.json"
                -h "${GENERATED_DIR}/LibWeb/CSS/EnvironmentVariable.h"
                -c "${GENERATED_DIR}/LibWeb/CSS/EnvironmentVariable.cpp"
        DEPENDS GenerateCSSEnvironmentVariable "${LIBWEB_INPUT}/CSS/EnvironmentVariables.json"
    )

    add_custom_command(
        OUTPUT "${GENERATED_DIR}/LibWeb/CSS/GeneratedCSSStyleProperties.h"
               "${GENERATED_DIR}/LibWeb/CSS/GeneratedCSSStyleProperties.cpp"
               "${GENERATED_DIR}/LibWeb/CSS/GeneratedCSSStyleProperties.idl"
        COMMAND GenerateCSSStyleProperties -j "${LIBWEB_INPUT}/CSS/Properties.json"
                -h "${GENERATED_DIR}/LibWeb/CSS/GeneratedCSSStyleProperties.h"
                -c "${GENERATED_DIR}/LibWeb/CSS/GeneratedCSSStyleProperties.cpp"
                -i "${GENERATED_DIR}/LibWeb/CSS/GeneratedCSSStyleProperties.idl"
        DEPENDS GenerateCSSStyleProperties "${LIBWEB_INPUT}/CSS/Properties.json"
    )

    add_custom_command(
        OUTPUT "${GENERATED_DIR}/LibWeb/CSS/GeneratedCSSNumericFactoryMethods.h"
               "${GENERATED_DIR}/LibWeb/CSS/GeneratedCSSNumericFactoryMethods.cpp"
               "${GENERATED_DIR}/LibWeb/CSS/GeneratedCSSNumericFactoryMethods.idl"
        COMMAND GenerateCSSNumericFactoryMethods -j "${LIBWEB_INPUT}/CSS/Units.json"
                -h "${GENERATED_DIR}/LibWeb/CSS/GeneratedCSSNumericFactoryMethods.h"
                -c "${GENERATED_DIR}/LibWeb/CSS/GeneratedCSSNumericFactoryMethods.cpp"
                -i "${GENERATED_DIR}/LibWeb/CSS/GeneratedCSSNumericFactoryMethods.idl"
        DEPENDS GenerateCSSNumericFactoryMethods "${LIBWEB_INPUT}/CSS/Units.json"
    )

    add_custom_command(
        OUTPUT "${GENERATED_DIR}/LibWeb/ARIA/AriaRoles.h" "${GENERATED_DIR}/LibWeb/ARIA/AriaRoles.cpp"
        COMMAND GenerateAriaRoles -j "${LIBWEB_INPUT}/ARIA/AriaRoles.json"
                -h "${GENERATED_DIR}/LibWeb/ARIA/AriaRoles.h"
                -c "${GENERATED_DIR}/LibWeb/ARIA/AriaRoles.cpp"
        DEPENDS GenerateAriaRoles "${LIBWEB_INPUT}/ARIA/AriaRoles.json"
    )

    add_custom_command(
        OUTPUT "${GENERATED_DIR}/LibWeb/HTML/Parser/NamedCharacterReferences.h"
               "${GENERATED_DIR}/LibWeb/HTML/Parser/NamedCharacterReferences.cpp"
        COMMAND GenerateNamedCharacterReferences -j "${LIBWEB_INPUT}/HTML/Parser/Entities.json"
                -h "${GENERATED_DIR}/LibWeb/HTML/Parser/NamedCharacterReferences.h"
                -c "${GENERATED_DIR}/LibWeb/HTML/Parser/NamedCharacterReferences.cpp"
        DEPENDS GenerateNamedCharacterReferences "${LIBWEB_INPUT}/HTML/Parser/Entities.json"
    )

    # Generate MediaControlsDOM from HTML template
    file(MAKE_DIRECTORY "${GENERATED_DIR}/LibWeb/HTML")
    add_custom_command(
        OUTPUT "${GENERATED_DIR}/LibWeb/HTML/MediaControlsDOM.h"
               "${GENERATED_DIR}/LibWeb/HTML/MediaControlsDOM.cpp"
        COMMAND python3 "${LADYBIRD_SOURCE_DIR}/Meta/generate-dom-tree.py"
                -h "${GENERATED_DIR}/LibWeb/HTML/MediaControlsDOM.h"
                -c "${GENERATED_DIR}/LibWeb/HTML/MediaControlsDOM.cpp"
                -i "${LIBWEB_INPUT}/HTML/MediaControls.html"
                -s MediaControlsDOM
                -n "Web::HTML"
                --html-tags "${LIBWEB_INPUT}/HTML/TagNames.h"
                --html-attributes "${LIBWEB_INPUT}/HTML/AttributeNames.h"
                --svg-tags "${LIBWEB_INPUT}/SVG/TagNames.h"
                --svg-attributes "${LIBWEB_INPUT}/SVG/AttributeNames.h"
        DEPENDS "${LIBWEB_INPUT}/HTML/MediaControls.html"
                "${LIBWEB_INPUT}/HTML/MediaControls.css"
        COMMENT "Generating MediaControlsDOM"
    )

    # Collect all generated CSS sources
    set(GENERATED_CSS_SOURCES
        "${GENERATED_DIR}/LibWeb/CSS/Enums.cpp"
        "${GENERATED_DIR}/LibWeb/CSS/Keyword.cpp"
        "${GENERATED_DIR}/LibWeb/CSS/PropertyID.cpp"
        "${GENERATED_DIR}/LibWeb/CSS/MediaFeatureID.cpp"
        "${GENERATED_DIR}/LibWeb/CSS/PseudoClass.cpp"
        "${GENERATED_DIR}/LibWeb/CSS/PseudoElement.cpp"
        "${GENERATED_DIR}/LibWeb/CSS/MathFunctions.cpp"
        "${GENERATED_DIR}/LibWeb/CSS/TransformFunctions.cpp"
        "${GENERATED_DIR}/LibWeb/CSS/Units.cpp"
        "${GENERATED_DIR}/LibWeb/CSS/DescriptorID.cpp"
        "${GENERATED_DIR}/LibWeb/CSS/EnvironmentVariable.cpp"
        "${GENERATED_DIR}/LibWeb/CSS/GeneratedCSSStyleProperties.cpp"
        "${GENERATED_DIR}/LibWeb/CSS/GeneratedCSSNumericFactoryMethods.cpp"
        "${GENERATED_DIR}/LibWeb/ARIA/AriaRoles.cpp"
        "${GENERATED_DIR}/LibWeb/HTML/Parser/NamedCharacterReferences.cpp"
        "${GENERATED_DIR}/LibWeb/HTML/MediaControlsDOM.cpp"
    )

    # === Generate IDL Bindings ===
    # Read the list of IDL files from Ladybird's idl_files.cmake
    # (This filters out mixin files which should not be processed directly)
    set(LIBWEB_IDL_FILES "")
    set(LIBWEB_IDL_OPTIONS "") # Store options for each IDL file
    file(STRINGS "${LIBWEB_INPUT}/idl_files.cmake" IDL_FILE_LINES)
    foreach(LINE ${IDL_FILE_LINES})
        if(LINE MATCHES "libweb_js_bindings\\(([^ )]+)(.*)\\)")
            set(IDL_REL_PATH "${CMAKE_MATCH_1}")
            set(IDL_OPTIONS "${CMAKE_MATCH_2}")
            # Skip NAMESPACE entries (they don't generate Prototype files)
            if(NOT IDL_OPTIONS MATCHES "NAMESPACE")
                list(APPEND LIBWEB_IDL_FILES "${LIBWEB_INPUT}/${IDL_REL_PATH}.idl")
                list(APPEND LIBWEB_IDL_OPTIONS "${IDL_OPTIONS}")
            endif()
        endif()
    endforeach()

    # Note: Generated CSS IDL files are mixins, not interfaces - don't include them here

    # Prepare IDL files list for GenerateWindowOrWorkerInterfaces
    set(IDL_FILES_ARG ${LIBWEB_IDL_FILES})

    # Generate IntrinsicDefinitions and exposed interface files
    set(EXPOSED_INTERFACE_OUTPUTS
        "${GENERATED_DIR}/LibWeb/Bindings/IntrinsicDefinitions.h"
        "${GENERATED_DIR}/LibWeb/Bindings/IntrinsicDefinitions.cpp"
        "${GENERATED_DIR}/LibWeb/Bindings/DedicatedWorkerExposedInterfaces.h"
        "${GENERATED_DIR}/LibWeb/Bindings/DedicatedWorkerExposedInterfaces.cpp"
        "${GENERATED_DIR}/LibWeb/Bindings/SharedWorkerExposedInterfaces.h"
        "${GENERATED_DIR}/LibWeb/Bindings/SharedWorkerExposedInterfaces.cpp"
        "${GENERATED_DIR}/LibWeb/Bindings/ShadowRealmExposedInterfaces.h"
        "${GENERATED_DIR}/LibWeb/Bindings/ShadowRealmExposedInterfaces.cpp"
        "${GENERATED_DIR}/LibWeb/Bindings/WindowExposedInterfaces.h"
        "${GENERATED_DIR}/LibWeb/Bindings/WindowExposedInterfaces.cpp"
    )

    add_custom_command(
        OUTPUT ${EXPOSED_INTERFACE_OUTPUTS}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${GENERATED_DIR}/LibWeb/Bindings/tmp"
        COMMAND GenerateWindowOrWorkerInterfaces
                -o "${GENERATED_DIR}/LibWeb/Bindings/tmp"
                -b "${LIBWEB_INPUT}"
                -b "${GENERATED_DIR}/LibWeb"
                ${IDL_FILES_ARG}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${GENERATED_DIR}/LibWeb/Bindings/tmp/IntrinsicDefinitions.h"
                "${GENERATED_DIR}/LibWeb/Bindings/IntrinsicDefinitions.h"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${GENERATED_DIR}/LibWeb/Bindings/tmp/IntrinsicDefinitions.cpp"
                "${GENERATED_DIR}/LibWeb/Bindings/IntrinsicDefinitions.cpp"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${GENERATED_DIR}/LibWeb/Bindings/tmp/DedicatedWorkerExposedInterfaces.h"
                "${GENERATED_DIR}/LibWeb/Bindings/DedicatedWorkerExposedInterfaces.h"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${GENERATED_DIR}/LibWeb/Bindings/tmp/DedicatedWorkerExposedInterfaces.cpp"
                "${GENERATED_DIR}/LibWeb/Bindings/DedicatedWorkerExposedInterfaces.cpp"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${GENERATED_DIR}/LibWeb/Bindings/tmp/SharedWorkerExposedInterfaces.h"
                "${GENERATED_DIR}/LibWeb/Bindings/SharedWorkerExposedInterfaces.h"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${GENERATED_DIR}/LibWeb/Bindings/tmp/SharedWorkerExposedInterfaces.cpp"
                "${GENERATED_DIR}/LibWeb/Bindings/SharedWorkerExposedInterfaces.cpp"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${GENERATED_DIR}/LibWeb/Bindings/tmp/ShadowRealmExposedInterfaces.h"
                "${GENERATED_DIR}/LibWeb/Bindings/ShadowRealmExposedInterfaces.h"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${GENERATED_DIR}/LibWeb/Bindings/tmp/ShadowRealmExposedInterfaces.cpp"
                "${GENERATED_DIR}/LibWeb/Bindings/ShadowRealmExposedInterfaces.cpp"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${GENERATED_DIR}/LibWeb/Bindings/tmp/WindowExposedInterfaces.h"
                "${GENERATED_DIR}/LibWeb/Bindings/WindowExposedInterfaces.h"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${GENERATED_DIR}/LibWeb/Bindings/tmp/WindowExposedInterfaces.cpp"
                "${GENERATED_DIR}/LibWeb/Bindings/WindowExposedInterfaces.cpp"
        DEPENDS GenerateWindowOrWorkerInterfaces ${LIBWEB_IDL_FILES}
            "${GENERATED_DIR}/LibWeb/CSS/GeneratedCSSStyleProperties.idl"
            "${GENERATED_DIR}/LibWeb/CSS/GeneratedCSSNumericFactoryMethods.idl"
        COMMENT "Generating Exposed Interface files"
    )
    add_custom_target(generate_exposed_interfaces DEPENDS ${EXPOSED_INTERFACE_OUTPUTS})

    # Generate IDL bindings for each IDL file
    set(ALL_IDL_GENERATED_SOURCES "")
    foreach(IDL_FILE ${LIBWEB_IDL_FILES})
        # Get relative path and basename
        file(RELATIVE_PATH REL_IDL_PATH "${LIBWEB_INPUT}" "${IDL_FILE}")
        get_filename_component(IDL_DIR "${REL_IDL_PATH}" DIRECTORY)
        get_filename_component(IDL_NAME "${IDL_FILE}" NAME_WE)

        # Output files
        set(BINDING_OUTPUT_DIR "${GENERATED_DIR}/LibWeb/Bindings")
        set(BINDING_H "${BINDING_OUTPUT_DIR}/${IDL_NAME}Prototype.h")
        set(BINDING_CPP "${BINDING_OUTPUT_DIR}/${IDL_NAME}Prototype.cpp")
        set(CONSTRUCTOR_H "${BINDING_OUTPUT_DIR}/${IDL_NAME}Constructor.h")
        set(CONSTRUCTOR_CPP "${BINDING_OUTPUT_DIR}/${IDL_NAME}Constructor.cpp")

        add_custom_command(
            OUTPUT "${BINDING_H}" "${BINDING_CPP}" "${CONSTRUCTOR_H}" "${CONSTRUCTOR_CPP}"
            COMMAND BindingsGenerator
                    -o "${BINDING_OUTPUT_DIR}"
                    "${IDL_FILE}"
                    "${LIBWEB_INPUT}"
                    "${GENERATED_DIR}/LibWeb"
            DEPENDS BindingsGenerator "${IDL_FILE}" generate_exposed_interfaces
            COMMENT "Generating bindings for ${IDL_NAME}"
        )
        list(APPEND ALL_IDL_GENERATED_SOURCES "${BINDING_CPP}" "${CONSTRUCTOR_CPP}")
    endforeach()

    # Add exposed interfaces sources
    list(APPEND ALL_IDL_GENERATED_SOURCES
        "${GENERATED_DIR}/LibWeb/Bindings/IntrinsicDefinitions.cpp"
        "${GENERATED_DIR}/LibWeb/Bindings/DedicatedWorkerExposedInterfaces.cpp"
        "${GENERATED_DIR}/LibWeb/Bindings/SharedWorkerExposedInterfaces.cpp"
        "${GENERATED_DIR}/LibWeb/Bindings/ShadowRealmExposedInterfaces.cpp"
        "${GENERATED_DIR}/LibWeb/Bindings/WindowExposedInterfaces.cpp"
    )

    add_custom_target(generate_all_idl_bindings DEPENDS ${ALL_IDL_GENERATED_SOURCES})

    # === LibJS ===
    # Generate bytecode files
    file(MAKE_DIRECTORY ${GENERATED_DIR}/LibJS/Bytecode)
    set(LIBJS_BYTECODE_OP_H "${GENERATED_DIR}/LibJS/Bytecode/Op.h")
    set(LIBJS_BYTECODE_OP_CPP "${GENERATED_DIR}/LibJS/Bytecode/Op.cpp")
    set(LIBJS_BYTECODE_OPCODES_H "${GENERATED_DIR}/LibJS/Bytecode/OpCodes.h")
    if(NOT EXISTS ${LIBJS_BYTECODE_OP_H})
        message(STATUS "Generating LibJS Bytecode...")
        execute_process(
            COMMAND python3 "${LADYBIRD_SOURCE_DIR}/Meta/generate-libjs-bytecode-def-derived.py"
                    -h "${LIBJS_BYTECODE_OP_H}"
                    -c "${LIBJS_BYTECODE_OP_CPP}"
                    -x "${LIBJS_BYTECODE_OPCODES_H}"
                    -i "${LADYBIRD_SOURCE_DIR}/Libraries/LibJS/Bytecode/Bytecode.def"
            RESULT_VARIABLE GEN_RESULT
        )
        if(NOT GEN_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to generate LibJS Bytecode")
        endif()
    endif()
    file(GLOB_RECURSE LIBJS_SOURCES "${LADYBIRD_SOURCE_DIR}/Libraries/LibJS/*.cpp")
    list(FILTER LIBJS_SOURCES EXCLUDE REGEX ".*/gen_asm_offsets\\.cpp$")
    add_library(js STATIC ${LIBJS_SOURCES} ${LIBJS_BYTECODE_OP_CPP})
    target_include_directories(js PUBLIC ${LADYBIRD_INCLUDE_DIRS})
    target_link_libraries(js PUBLIC ak gc core unicode textcodec)

    # === LibWeb ===
    file(GLOB_RECURSE LIBWEB_SOURCES "${LADYBIRD_SOURCE_DIR}/Libraries/LibWeb/*.cpp")
    # Exclude Skia (we use YDraw backend)
    list(FILTER LIBWEB_SOURCES EXCLUDE REGEX ".*Skia.*")
    # Exclude Worker/ServiceWorker (requires IPC)
    list(FILTER LIBWEB_SOURCES EXCLUDE REGEX ".*/Worker/.*")
    list(FILTER LIBWEB_SOURCES EXCLUDE REGEX ".*/ServiceWorker/.*")
    list(FILTER LIBWEB_SOURCES EXCLUDE REGEX ".*WorkerAgent.*")
    # Exclude SDL-dependent files (not needed for static rendering)
    list(FILTER LIBWEB_SOURCES EXCLUDE REGEX ".*/Gamepad/.*")
    list(FILTER LIBWEB_SOURCES EXCLUDE REGEX ".*InternalGamepad.*")
    list(FILTER LIBWEB_SOURCES EXCLUDE REGEX ".*EventHandler.*")
    # Exclude IPC-dependent files (requires RequestServer/WebContent endpoints)
    list(FILTER LIBWEB_SOURCES EXCLUDE REGEX ".*/Loader/.*")
    list(FILTER LIBWEB_SOURCES EXCLUDE REGEX ".*BackingStoreManager.*")
    # Exclude WebGL (requires GLES2)
    list(FILTER LIBWEB_SOURCES EXCLUDE REGEX ".*/WebGL/.*")
    # Exclude WebSockets (requires RequestServer IPC)
    list(FILTER LIBWEB_SOURCES EXCLUDE REGEX ".*/WebSockets/.*")
    add_library(web STATIC ${LIBWEB_SOURCES} ${GENERATED_CSS_SOURCES} ${ALL_IDL_GENERATED_SOURCES})
    target_include_directories(web PUBLIC ${LADYBIRD_INCLUDE_DIRS})
    target_link_libraries(web PUBLIC ak gc js gfx core url unicode textcodec idl)
    target_compile_definitions(web PRIVATE LIBWEB_NO_SKIA=1)
    add_dependencies(web
        GenerateCSSEnums GenerateCSSKeyword GenerateCSSPropertyID
        GenerateCSSMediaFeatureID GenerateCSSPseudoClass GenerateCSSPseudoElement
        GenerateCSSMathFunctions GenerateCSSTransformFunctions GenerateCSSUnits
        GenerateCSSDescriptors GenerateCSSEnvironmentVariable
        GenerateCSSStyleProperties GenerateCSSNumericFactoryMethods
        GenerateAriaRoles GenerateNamedCharacterReferences
        generate_all_idl_bindings
    )

    # === DisplayListPlayerYDraw ===
    add_library(display-list-player-ydraw STATIC
        DisplayListPlayerYDraw.cpp
    )
    target_include_directories(display-list-player-ydraw PUBLIC ${LADYBIRD_INCLUDE_DIRS})
    target_link_libraries(display-list-player-ydraw PUBLIC web ydraw-buffer)

    # === libweb-to-ydraw executable ===
    add_executable(libweb-to-ydraw-full
        libweb-to-ydraw.cpp
        HeadlessPageClient.cpp
    )
    target_include_directories(libweb-to-ydraw-full PRIVATE ${LADYBIRD_INCLUDE_DIRS})
    target_link_libraries(libweb-to-ydraw-full PRIVATE
        display-list-player-ydraw
        web
        ydraw-buffer
        CURL::libcurl
    )
    if(CURL_FOUND)
        target_link_libraries(libweb-to-ydraw-full PRIVATE CURL::libcurl)
        target_compile_definitions(libweb-to-ydraw-full PRIVATE HAS_CURL=1)
    endif()
endif()
