# libpng - PNG image library
# Depends on zlib
if(TARGET png_static)
    return()
endif()

include(${CMAKE_CURRENT_LIST_DIR}/zlib.cmake)

if(YETTY_ANDROID)
    CPMAddPackage(
        NAME libpng
        GITHUB_REPOSITORY glennrp/libpng
        GIT_TAG v1.6.43
        OPTIONS
            "PNG_SHARED OFF"
            "PNG_STATIC ON"
            "PNG_TESTS OFF"
            "PNG_TOOLS OFF"
            "SKIP_INSTALL_ALL ON"
    )
else()
    CPMAddPackage(
        NAME libpng
        GITHUB_REPOSITORY pnggroup/libpng
        GIT_TAG v1.6.43
        EXCLUDE_FROM_ALL YES
        OPTIONS
            "PNG_SHARED OFF"
            "PNG_STATIC ON"
            "PNG_TESTS OFF"
            "PNG_TOOLS OFF"
            "SKIP_INSTALL_ALL ON"
    )
endif()

if(libpng_ADDED)
    set_target_properties(png_static PROPERTIES POSITION_INDEPENDENT_CODE ON)
    set(PNG_FOUND TRUE CACHE BOOL "" FORCE)
    set(PNG_INCLUDE_DIRS "${libpng_SOURCE_DIR};${libpng_BINARY_DIR}" CACHE PATH "" FORCE)
    set(PNG_PNG_INCLUDE_DIR "${libpng_SOURCE_DIR}" CACHE PATH "" FORCE)
    set(PNG_LIBRARY png_static CACHE STRING "" FORCE)
    set(PNG_LIBRARIES png_static CACHE STRING "" FORCE)
    message(STATUS "libpng: Built from source v1.6.43")
endif()
