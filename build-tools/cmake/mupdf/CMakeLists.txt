# CMake wrapper for MuPDF
# This builds a minimal static library with fitz core + PDF support
# Downloads MuPDF release tarball which includes all thirdparty code

cmake_minimum_required(VERSION 3.16)

# MuPDF source directory (set by CPMAddPackage)
if(NOT DEFINED mupdf_SOURCE_DIR)
    message(FATAL_ERROR "mupdf_SOURCE_DIR must be set by CPMAddPackage")
endif()

set(MUPDF_SOURCE_DIR ${mupdf_SOURCE_DIR})

message(STATUS "Building MuPDF from: ${MUPDF_SOURCE_DIR}")

# Collect fitz source files (core library)
file(GLOB FITZ_SOURCES "${MUPDF_SOURCE_DIR}/source/fitz/*.c")

# Collect pdf source files
file(GLOB PDF_SOURCES "${MUPDF_SOURCE_DIR}/source/pdf/*.c")

# Collect generated sources (cmap, fonts, etc.)
file(GLOB GENERATED_SOURCES "${MUPDF_SOURCE_DIR}/generated/*.c")

# Bundled FreeType sources (specific modules from MuPDF Makelists)
set(FREETYPE_SOURCES
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/base/ftbase.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/base/ftbbox.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/base/ftbitmap.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/base/ftdebug.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/base/ftfstype.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/base/ftgasp.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/base/ftglyph.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/base/ftinit.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/base/ftstroke.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/base/ftsynth.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/base/ftsystem.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/base/fttype1.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/cff/cff.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/cid/type1cid.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/psaux/psaux.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/pshinter/pshinter.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/psnames/psnames.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/raster/raster.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/sfnt/sfnt.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/smooth/smooth.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/truetype/truetype.c
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/src/type1/type1.c
)

# Bundled zlib sources (skip on macOS - use system zlib to avoid header conflicts)
if(NOT APPLE)
    file(GLOB ZLIB_SOURCES "${MUPDF_SOURCE_DIR}/thirdparty/zlib/*.c")
endif()

# Bundled libjpeg sources (from MuPDF Makelists)
set(LIBJPEG_SOURCES
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jaricom.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jcapimin.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jcapistd.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jcarith.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jccoefct.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jccolor.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jcdctmgr.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jchuff.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jcinit.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jcmainct.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jcmarker.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jcmaster.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jcomapi.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jcparam.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jcprepct.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jcsample.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jdapimin.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jdapistd.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jdarith.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jdatadst.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jdatasrc.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jdcoefct.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jdcolor.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jddctmgr.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jdhuff.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jdinput.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jdmainct.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jdmarker.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jdmaster.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jdmerge.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jdpostct.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jdsample.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jdtrans.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jerror.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jfdctflt.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jfdctfst.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jfdctint.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jidctflt.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jidctfst.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jidctint.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jmemmgr.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jquant1.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jquant2.c
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg/jutils.c
)

# JBIG2 decoder (from MuPDF Makelists)
set(JBIG2_SOURCES
    ${MUPDF_SOURCE_DIR}/thirdparty/jbig2dec/jbig2.c
    ${MUPDF_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_arith.c
    ${MUPDF_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_arith_iaid.c
    ${MUPDF_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_arith_int.c
    ${MUPDF_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_generic.c
    ${MUPDF_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_halftone.c
    ${MUPDF_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_huffman.c
    ${MUPDF_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_hufftab.c
    ${MUPDF_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_image.c
    ${MUPDF_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_mmr.c
    ${MUPDF_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_page.c
    ${MUPDF_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_refinement.c
    ${MUPDF_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_segment.c
    ${MUPDF_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_symbol_dict.c
    ${MUPDF_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_text.c
)

# OpenJPEG (JPEG2000) from MuPDF Makelists
set(OPENJPEG_SOURCES
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/bio.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/cio.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/dwt.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/event.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/function_list.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/ht_dec.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/image.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/invert.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/j2k.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/jp2.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/mct.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/mqc.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/openjpeg.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/pi.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/sparse_array.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/t1.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/t2.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/tcd.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/tgt.c
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2/thread.c
)

# LCMS2 (color management)
file(GLOB LCMS2_SOURCES "${MUPDF_SOURCE_DIR}/thirdparty/lcms2/src/*.c")

# MuJS (JavaScript support for PDF forms)
set(MUJS_SOURCES
    ${MUPDF_SOURCE_DIR}/thirdparty/mujs/one.c
)

# Gumbo HTML5 parser
file(GLOB GUMBO_SOURCES "${MUPDF_SOURCE_DIR}/thirdparty/gumbo-parser/src/*.c")

# URW fonts (Base14 PDF fonts)
file(GLOB URW_FONT_SOURCES "${MUPDF_SOURCE_DIR}/generated/resources/fonts/urw/*.c")

# HTML module (needed for story API used by pdf-appearance.c)
file(GLOB HTML_SOURCES "${MUPDF_SOURCE_DIR}/source/html/*.c")

# HarfBuzz (text shaping) - C++ files (from MuPDF Makelists)
set(HARFBUZZ_SOURCES
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/graph/gsubgpos-context.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-aat-layout.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-aat-map.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-blob.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-buffer.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-buffer-verify.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-buffer-serialize.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-common.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-face.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-fallback-shape.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-font.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ft.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-map.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-number.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-cff1-table.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-cff2-table.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-color.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-face.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-font.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-layout.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-map.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-math.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-meta.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-metrics.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-name.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-shape.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-shape-fallback.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-shape-normalize.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-shaper-arabic.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-shaper-default.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-shaper-hangul.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-shaper-hebrew.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-shaper-indic.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-shaper-indic-table.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-shaper-khmer.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-shaper-myanmar.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-shaper-syllabic.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-shaper-thai.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-shaper-use.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-shaper-vowel-constraints.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-tag.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ot-var.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-set.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-shape.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-shape-plan.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-shaper.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-static.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-subset.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-subset-cff1.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-subset-cff2.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-subset-cff-common.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-subset-input.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-subset-plan.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-ucd.cc
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src/hb-unicode.cc
)

# Create static library
add_library(mupdf STATIC
    ${FITZ_SOURCES}
    ${PDF_SOURCES}
    ${HTML_SOURCES}
    ${GENERATED_SOURCES}
    ${URW_FONT_SOURCES}
    ${FREETYPE_SOURCES}
    ${ZLIB_SOURCES}
    ${LIBJPEG_SOURCES}
    ${JBIG2_SOURCES}
    ${OPENJPEG_SOURCES}
    ${LCMS2_SOURCES}
    ${MUJS_SOURCES}
    ${GUMBO_SOURCES}
    ${HARFBUZZ_SOURCES}
)

# Public include directories (for users of mupdf)
target_include_directories(mupdf PUBLIC
    ${MUPDF_SOURCE_DIR}/include
)

# Private include directories (for building mupdf)
target_include_directories(mupdf PRIVATE
    ${MUPDF_SOURCE_DIR}/include/mupdf
    ${MUPDF_SOURCE_DIR}/thirdparty/freetype/include
    ${MUPDF_SOURCE_DIR}/thirdparty/harfbuzz/src
    ${MUPDF_SOURCE_DIR}/thirdparty/jbig2dec
    ${MUPDF_SOURCE_DIR}/thirdparty/openjpeg/src/lib/openjp2
    ${MUPDF_SOURCE_DIR}/thirdparty/lcms2/include
    ${MUPDF_SOURCE_DIR}/thirdparty/libjpeg
    $<$<NOT:$<PLATFORM_ID:Darwin>>:${MUPDF_SOURCE_DIR}/thirdparty/zlib>
    ${MUPDF_SOURCE_DIR}/thirdparty/gumbo-parser/src
    ${MUPDF_SOURCE_DIR}/thirdparty/mujs
    ${MUPDF_SOURCE_DIR}/scripts/freetype
    ${MUPDF_SOURCE_DIR}/scripts/libjpeg
    ${MUPDF_SOURCE_DIR}/scripts/openjpeg
)

# Compile definitions
target_compile_definitions(mupdf PRIVATE
    # zlib needs proper POSIX defines on Linux (macOS uses system zlib)
    $<$<PLATFORM_ID:Linux>:Z_HAVE_UNISTD_H>
    $<$<PLATFORM_ID:Linux>:HAVE_FSEEKO>

    # Disable features we don't need (reduces font binary size)
    TOFU
    TOFU_CJK
    TOFU_CJK_EXT
    TOFU_CJK_LANG
    TOFU_EMOJI
    TOFU_HISTORIC
    TOFU_SYMBOL
    TOFU_SIL

    # FreeType config
    FT2_BUILD_LIBRARY
    FT_CONFIG_MODULES_H="slimftmodules.h"
    FT_CONFIG_OPTIONS_H="slimftoptions.h"

    # HarfBuzz config
    HAVE_FALLBACK=1
    HAVE_FREETYPE
    HAVE_OT
    HAVE_ROUND
    HAVE_UCDN
    HB_NO_MT
    HB_NO_PRAGMA_GCC_DIAGNOSTIC

    # OpenJPEG
    OPJ_STATIC
    OPJ_HAVE_STDINT_H
    OPJ_HAVE_INTTYPES_H
    MUTEX_pthread=0

    # JBIG2
    HAVE_STDINT_H
    JBIG_EXTERNAL_MEMENTO_H="mupdf/memento.h"

    # LCMS2
    HAVE_LCMS2MT=0

    # Disable optional dependencies
    FZ_ENABLE_XPS=0
    FZ_ENABLE_SVG=0
    FZ_ENABLE_CBZ=0
    FZ_ENABLE_IMG=0
    FZ_ENABLE_HTML=1
    FZ_ENABLE_EPUB=0
    FZ_ENABLE_OCR_OUTPUT=0
    FZ_ENABLE_DOCX_OUTPUT=0
    FZ_ENABLE_ODT_OUTPUT=0
)

# Suppress all warnings for third-party code
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(mupdf PRIVATE -w)
endif()

# Enable position independent code for shared library plugins
set_target_properties(mupdf PROPERTIES POSITION_INDEPENDENT_CODE ON)

# C++ flags for HarfBuzz sources
set_source_files_properties(${HARFBUZZ_SOURCES}
    PROPERTIES
    COMPILE_FLAGS "-fno-exceptions -fno-rtti -fno-threadsafe-statics -fvisibility-inlines-hidden -std=gnu++11 -w"
    COMPILE_DEFINITIONS "hb_malloc_impl=fz_hb_malloc;hb_calloc_impl=fz_hb_calloc;hb_free_impl=fz_hb_free;hb_realloc_impl=fz_hb_realloc"
)

# Link math library on Unix (not macOS)
if(UNIX AND NOT APPLE)
    target_link_libraries(mupdf PUBLIC m)
endif()

# On macOS, use system zlib instead of bundled (avoids header conflicts)
if(APPLE)
    find_package(ZLIB REQUIRED)
    target_link_libraries(mupdf PUBLIC ZLIB::ZLIB)
endif()
