# libjpeg-turbo - JPEG compression library with SIMD optimizations
# BSD-3-Clause license, MIT-compatible
#
# libjpeg-turbo explicitly blocks add_subdirectory() in their CMakeLists.txt
# So we use CPM to download and ExternalProject to build it
if(TARGET turbojpeg-static)
    return()
endif()

include(ExternalProject)

# Use CPM to download only
CPMAddPackage(
    NAME libjpeg-turbo
    GITHUB_REPOSITORY libjpeg-turbo/libjpeg-turbo
    GIT_TAG 3.1.3
    DOWNLOAD_ONLY YES
)

# CPM sets CPM_PACKAGE_<name>_SOURCE_DIR - access via cache
set(_LIBJPEG_SRC_DIR "${CMAKE_BINARY_DIR}/_deps/libjpeg-turbo-src")

if(EXISTS "${_LIBJPEG_SRC_DIR}/src/turbojpeg.h")
    set(LIBJPEG_TURBO_INSTALL_DIR "${CMAKE_BINARY_DIR}/libjpeg-turbo-install")

    # Determine library directory (lib vs lib64)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND NOT APPLE AND NOT EMSCRIPTEN)
        set(_LIBJPEG_LIB_SUBDIR "lib64")
    else()
        set(_LIBJPEG_LIB_SUBDIR "lib")
    endif()

    # Platform-specific CMake arguments
    set(_LIBJPEG_CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${LIBJPEG_TURBO_INSTALL_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DCMAKE_INSTALL_LIBDIR=${_LIBJPEG_LIB_SUBDIR}
        -DENABLE_SHARED=OFF
        -DENABLE_STATIC=ON
        -DWITH_TURBOJPEG=ON
        -DWITH_JPEG8=ON
    )

    if(EMSCRIPTEN)
        # For Emscripten: use the toolchain and disable SIMD (use C fallback)
        list(APPEND _LIBJPEG_CMAKE_ARGS
            -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
            -DWITH_SIMD=OFF
            -DREQUIRE_SIMD=OFF
        )
    elseif(ANDROID)
        # For Android: pass cross-compilation settings
        if(CMAKE_TOOLCHAIN_FILE)
            list(APPEND _LIBJPEG_CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE})
        endif()
        if(ANDROID_ABI)
            list(APPEND _LIBJPEG_CMAKE_ARGS -DANDROID_ABI=${ANDROID_ABI})
        endif()
        if(ANDROID_NDK)
            list(APPEND _LIBJPEG_CMAKE_ARGS -DANDROID_NDK=${ANDROID_NDK})
        endif()
        if(ANDROID_PLATFORM)
            list(APPEND _LIBJPEG_CMAKE_ARGS -DANDROID_PLATFORM=${ANDROID_PLATFORM})
        endif()
        if(CMAKE_ANDROID_NDK)
            list(APPEND _LIBJPEG_CMAKE_ARGS -DCMAKE_ANDROID_NDK=${CMAKE_ANDROID_NDK})
        endif()
    endif()

    # Build libjpeg-turbo as external project
    ExternalProject_Add(libjpeg-turbo-ext
        SOURCE_DIR ${_LIBJPEG_SRC_DIR}
        INSTALL_DIR ${LIBJPEG_TURBO_INSTALL_DIR}
        CMAKE_ARGS ${_LIBJPEG_CMAKE_ARGS}
        BUILD_BYPRODUCTS
            ${LIBJPEG_TURBO_INSTALL_DIR}/${_LIBJPEG_LIB_SUBDIR}/libturbojpeg.a
    )

    # Create imported target
    add_library(turbojpeg-static STATIC IMPORTED GLOBAL)
    add_dependencies(turbojpeg-static libjpeg-turbo-ext)

    set_target_properties(turbojpeg-static PROPERTIES
        IMPORTED_LOCATION "${LIBJPEG_TURBO_INSTALL_DIR}/${_LIBJPEG_LIB_SUBDIR}/libturbojpeg.a"
    )

    # Use SOURCE directory headers (available immediately after CPM download)
    # turbojpeg.h is in src/ subdirectory
    target_include_directories(turbojpeg-static INTERFACE
        "${_LIBJPEG_SRC_DIR}"
        "${_LIBJPEG_SRC_DIR}/src"
    )

    # Export variables
    set(JPEG_FOUND TRUE CACHE BOOL "" FORCE)
    set(JPEG_INCLUDE_DIRS "${_LIBJPEG_SRC_DIR};${_LIBJPEG_SRC_DIR}/src" CACHE PATH "" FORCE)
    set(JPEG_LIBRARIES turbojpeg-static CACHE STRING "" FORCE)

    message(STATUS "libjpeg-turbo: Building from source v3.1.3 (ExternalProject)")
    message(STATUS "libjpeg-turbo: Include dirs = ${JPEG_INCLUDE_DIRS}")
else()
    message(WARNING "libjpeg-turbo: Source directory not found at ${_LIBJPEG_SRC_DIR}")
endif()
