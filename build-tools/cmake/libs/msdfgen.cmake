# msdfgen - Multi-channel signed distance field generator
# Depends on freetype, tinyxml2
if(TARGET msdfgen::msdfgen-core)
    return()
endif()

include(${CMAKE_CURRENT_LIST_DIR}/tinyxml2.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/FreeType.cmake)

CPMAddPackage(
    NAME msdfgen
    GITHUB_REPOSITORY Chlumsky/msdfgen
    GIT_TAG v1.12
    DOWNLOAD_ONLY YES
)

if(msdfgen_ADDED)
    # Core library - pure math, no external deps
    file(GLOB MSDFGEN_CORE_SOURCES "${msdfgen_SOURCE_DIR}/core/*.cpp")
    file(GLOB MSDFGEN_CORE_HEADERS "${msdfgen_SOURCE_DIR}/core/*.h")
    add_library(msdfgen-core STATIC ${MSDFGEN_CORE_SOURCES} ${MSDFGEN_CORE_HEADERS})
    add_library(msdfgen::msdfgen-core ALIAS msdfgen-core)
    target_include_directories(msdfgen-core PUBLIC ${msdfgen_SOURCE_DIR})
    target_compile_features(msdfgen-core PUBLIC cxx_std_11)
    target_compile_definitions(msdfgen-core PUBLIC MSDFGEN_PUBLIC= MSDFGEN_USE_CPP11)
    set_target_properties(msdfgen-core PROPERTIES POSITION_INDEPENDENT_CODE ON)

    # Extensions library - freetype for font import, tinyxml2 for SVG
    set(MSDFGEN_EXT_SOURCES
        ${msdfgen_SOURCE_DIR}/ext/import-font.cpp
        ${msdfgen_SOURCE_DIR}/ext/import-svg.cpp
        ${msdfgen_SOURCE_DIR}/ext/resolve-shape-geometry.cpp
    )
    file(GLOB MSDFGEN_EXT_HEADERS "${msdfgen_SOURCE_DIR}/ext/*.h")
    add_library(msdfgen-ext STATIC ${MSDFGEN_EXT_SOURCES} ${MSDFGEN_EXT_HEADERS})
    add_library(msdfgen::msdfgen-ext ALIAS msdfgen-ext)
    target_include_directories(msdfgen-ext PUBLIC ${msdfgen_SOURCE_DIR} PRIVATE ${FREETYPE_INCLUDE_DIR})
    target_compile_definitions(msdfgen-ext PUBLIC
        MSDFGEN_EXT_PUBLIC=
        MSDFGEN_EXTENSIONS
        MSDFGEN_USE_TINYXML2
        MSDFGEN_DISABLE_PNG
    )
    target_link_libraries(msdfgen-ext PRIVATE msdfgen::msdfgen-core tinyxml2::tinyxml2)
    set_target_properties(msdfgen-ext PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
