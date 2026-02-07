#=============================================================================
# FreeType.cmake - Build FreeType and remaining dependencies from source
#=============================================================================
# Builds: brotli, bzip2, freetype
# Expects: zlib and libpng to be already configured in parent CMakeLists.txt
# All as STATIC libraries with PIC enabled.
#=============================================================================

include_guard(GLOBAL)

# Verify zlib and libpng are available (built by parent CMakeLists.txt)
if(NOT TARGET ZLIB::ZLIB)
    message(FATAL_ERROR "FreeType.cmake requires ZLIB::ZLIB target. Add zlib before including this file.")
endif()
if(NOT TARGET png_static)
    message(FATAL_ERROR "FreeType.cmake requires png_static target. Add libpng before including this file.")
endif()

# We don't need install targets for any of these deps
set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)

# --- brotli ---
CPMAddPackage(
    NAME brotli
    GITHUB_REPOSITORY google/brotli
    GIT_TAG v1.1.0
    EXCLUDE_FROM_ALL YES
    OPTIONS
        "BROTLI_DISABLE_TESTS ON"
        "BROTLI_BUNDLED_MODE ON"
)
if(brotli_ADDED)
    # In bundled mode, brotli creates: brotlidec, brotlienc, brotlicommon (no -static suffix)
    foreach(_tgt brotlidec brotlicommon brotlienc)
        if(TARGET ${_tgt})
            set_target_properties(${_tgt} PROPERTIES POSITION_INDEPENDENT_CODE ON)
        endif()
    endforeach()
    set(BROTLIDEC_LIBRARIES brotlidec brotlicommon CACHE INTERNAL "")
    set(BROTLI_INCLUDE_DIR "${brotli_SOURCE_DIR}/c/include" CACHE INTERNAL "")
    message(STATUS "brotli: Built from source v1.1.0")
endif()

# --- bzip2 ---
CPMAddPackage(
    NAME bzip2
    GITHUB_REPOSITORY libarchive/bzip2
    GIT_TAG bzip2-1.0.8
    DOWNLOAD_ONLY YES
)
if(bzip2_ADDED)
    add_library(bz2_static STATIC
        ${bzip2_SOURCE_DIR}/blocksort.c
        ${bzip2_SOURCE_DIR}/huffman.c
        ${bzip2_SOURCE_DIR}/crctable.c
        ${bzip2_SOURCE_DIR}/randtable.c
        ${bzip2_SOURCE_DIR}/compress.c
        ${bzip2_SOURCE_DIR}/decompress.c
        ${bzip2_SOURCE_DIR}/bzlib.c
    )
    target_include_directories(bz2_static PUBLIC ${bzip2_SOURCE_DIR})
    set_target_properties(bz2_static PROPERTIES POSITION_INDEPENDENT_CODE ON)
    if(MSVC)
        target_compile_options(bz2_static PRIVATE /w)
    else()
        target_compile_options(bz2_static PRIVATE -w)
    endif()
    set(BZIP2_LIBRARIES bz2_static CACHE INTERNAL "")
    set(BZIP2_INCLUDE_DIR "${bzip2_SOURCE_DIR}" CACHE INTERNAL "")
    message(STATUS "bzip2: Built from source v1.0.8")
endif()

# --- FreeType ---
# Disable optional deps that FreeType tries to find via pkg-config/find_package
# We want it to use ONLY what we provide (zlib from parent)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG ON CACHE BOOL "" FORCE)
set(FT_DISABLE_ZLIB OFF CACHE BOOL "" FORCE)

CPMAddPackage(
    NAME freetype
    GITHUB_REPOSITORY freetype/freetype
    GIT_TAG VER-2-13-2
    EXCLUDE_FROM_ALL YES
    OPTIONS
        "FT_DISABLE_HARFBUZZ ON"
        "FT_DISABLE_BROTLI ON"
        "FT_DISABLE_BZIP2 ON"
        "FT_DISABLE_PNG ON"
        "FT_DISABLE_ZLIB OFF"
        "BUILD_SHARED_LIBS OFF"
)
if(freetype_ADDED)
    set_target_properties(freetype PROPERTIES POSITION_INDEPENDENT_CODE ON)

    set(FREETYPE_INCLUDE_DIR "${freetype_SOURCE_DIR}/include" CACHE INTERNAL "")
    set(FREETYPE_LIBRARY freetype CACHE INTERNAL "")

    # Create Freetype::Freetype alias for compatibility with find_package consumers
    if(NOT TARGET Freetype::Freetype)
        add_library(Freetype::Freetype ALIAS freetype)
    endif()

    # Bundle variable: all static libs in link order for final executables
    # Note: 'm' math library is only needed on Unix
    set(FREETYPE_ALL_LIBS
        freetype
        png_static
        brotlidec
        brotlicommon
        bz2_static
        zlibstatic
        $<$<NOT:$<BOOL:${WIN32}>>:m>
        CACHE INTERNAL "All FreeType static libs in link order"
    )
    message(STATUS "freetype: Built from source v2.13.2")
endif()
