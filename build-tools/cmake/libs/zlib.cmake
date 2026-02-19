# zlib - compression library
# Uses zlib-ng on desktop/webasm, standard zlib on Android
if(TARGET zlib OR TARGET zlibstatic)
    return()
endif()

if(YETTY_ANDROID)
    CPMAddPackage(
        NAME zlib
        GITHUB_REPOSITORY madler/zlib
        GIT_TAG v1.3.1
        OPTIONS
            "ZLIB_BUILD_EXAMPLES OFF"
            "SKIP_INSTALL_ALL ON"
    )
    if(zlib_ADDED)
        set_target_properties(zlibstatic PROPERTIES POSITION_INDEPENDENT_CODE ON)
        if(NOT TARGET ZLIB::ZLIB)
            add_library(ZLIB::ZLIB ALIAS zlibstatic)
        endif()
        set(ZLIB_FOUND TRUE CACHE BOOL "" FORCE)
        set(ZLIB_LIBRARIES zlibstatic CACHE STRING "" FORCE)
        set(ZLIB_INCLUDE_DIRS "${zlib_SOURCE_DIR};${zlib_BINARY_DIR}" CACHE STRING "" FORCE)
        set(ZLIB_INCLUDE_DIR "${zlib_SOURCE_DIR}" CACHE PATH "" FORCE)
    endif()
else()
    # zlib-ng for desktop and webasm
    CPMAddPackage(
        NAME zlib
        GITHUB_REPOSITORY zlib-ng/zlib-ng
        GIT_TAG 2.2.4
        OPTIONS
            "ZLIB_COMPAT ON"
            "ZLIB_ENABLE_TESTS OFF"
            "ZLIBNG_ENABLE_TESTS OFF"
            "WITH_GTEST OFF"
    )
    if(zlib_ADDED)
        if(TARGET zlib)
            set_target_properties(zlib PROPERTIES POSITION_INDEPENDENT_CODE ON)
        endif()
        if(NOT TARGET ZLIB::ZLIB)
            add_library(ZLIB::ZLIB ALIAS zlib)
        endif()
        set(ZLIB_FOUND TRUE CACHE BOOL "" FORCE)
        set(ZLIB_INCLUDE_DIR "${zlib_SOURCE_DIR};${zlib_BINARY_DIR}" CACHE PATH "" FORCE)
        set(ZLIB_INCLUDE_DIRS "${zlib_SOURCE_DIR};${zlib_BINARY_DIR}" CACHE PATH "" FORCE)
        set(ZLIB_LIBRARY zlib CACHE STRING "" FORCE)
        set(ZLIB_LIBRARIES zlib CACHE STRING "" FORCE)
        set(ZLIB_ROOT "${zlib_SOURCE_DIR}" CACHE PATH "" FORCE)
        set(CMAKE_INCLUDE_PATH "${zlib_SOURCE_DIR};${zlib_BINARY_DIR}" CACHE PATH "" FORCE)
        message(STATUS "zlib-ng: Built from source v2.2.4 (compat mode)")
    endif()
endif()
