# minimp4 - Minimalistic MP4 demuxer (header-only)
# MIT license, lieff
# Simple MP4/MOV container parsing

CPMAddPackage(
    NAME minimp4
    GITHUB_REPOSITORY lieff/minimp4
    GIT_TAG master
    DOWNLOAD_ONLY YES
)

if(minimp4_ADDED)
    # Header-only library
    add_library(minimp4 INTERFACE)
    target_include_directories(minimp4 INTERFACE ${minimp4_SOURCE_DIR})

    message(STATUS "minimp4: Header-only library added")
endif()
