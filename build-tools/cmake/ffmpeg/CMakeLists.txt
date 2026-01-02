# FFmpeg minimal build from source - decode only, no encoding/filters
include(ExternalProject)

set(FFMPEG_VERSION "n8.0.1")
set(FFMPEG_URL "https://github.com/FFmpeg/FFmpeg/archive/refs/tags/${FFMPEG_VERSION}.tar.gz")

set(FFMPEG_PREFIX "${CMAKE_BINARY_DIR}/ffmpeg")
set(FFMPEG_INSTALL_DIR "${FFMPEG_PREFIX}/install")

# Minimal config: decode only, no encoding, no filters, no devices
# PIC required for shared library plugins - disable all asm for maximum compatibility
set(FFMPEG_CONFIGURE_OPTS
    --prefix=${FFMPEG_INSTALL_DIR}
    --disable-programs
    --disable-doc
    --disable-network
    --disable-autodetect
    --disable-x86asm
    --disable-asm
    --disable-inline-asm
    --enable-static
    --disable-shared
    --enable-pic
    --disable-avdevice
    --disable-avfilter
    --disable-encoders
    --disable-muxers
    --disable-hwaccels
    --enable-swscale
)

include(ProcessorCount)
ProcessorCount(NPROC)
if(NPROC EQUAL 0)
    set(NPROC 4)
endif()

ExternalProject_Add(ffmpeg_build
    URL ${FFMPEG_URL}
    PREFIX ${FFMPEG_PREFIX}
    CONFIGURE_COMMAND <SOURCE_DIR>/configure ${FFMPEG_CONFIGURE_OPTS}
    BUILD_COMMAND make -j${NPROC}
    INSTALL_COMMAND make install
    BUILD_IN_SOURCE 0
    BUILD_BYPRODUCTS
        ${FFMPEG_INSTALL_DIR}/lib/libavformat.a
        ${FFMPEG_INSTALL_DIR}/lib/libavcodec.a
        ${FFMPEG_INSTALL_DIR}/lib/libavutil.a
        ${FFMPEG_INSTALL_DIR}/lib/libswscale.a
        ${FFMPEG_INSTALL_DIR}/lib/libswresample.a
)

# Imported targets
add_library(ffmpeg::avutil STATIC IMPORTED GLOBAL)
set_target_properties(ffmpeg::avutil PROPERTIES
    IMPORTED_LOCATION ${FFMPEG_INSTALL_DIR}/lib/libavutil.a
    INTERFACE_INCLUDE_DIRECTORIES ${FFMPEG_INSTALL_DIR}/include
)
add_dependencies(ffmpeg::avutil ffmpeg_build)

add_library(ffmpeg::swresample STATIC IMPORTED GLOBAL)
set_target_properties(ffmpeg::swresample PROPERTIES
    IMPORTED_LOCATION ${FFMPEG_INSTALL_DIR}/lib/libswresample.a
    INTERFACE_INCLUDE_DIRECTORIES ${FFMPEG_INSTALL_DIR}/include
)
add_dependencies(ffmpeg::swresample ffmpeg_build)

add_library(ffmpeg::swscale STATIC IMPORTED GLOBAL)
set_target_properties(ffmpeg::swscale PROPERTIES
    IMPORTED_LOCATION ${FFMPEG_INSTALL_DIR}/lib/libswscale.a
    INTERFACE_INCLUDE_DIRECTORIES ${FFMPEG_INSTALL_DIR}/include
    INTERFACE_LINK_LIBRARIES ffmpeg::avutil
)
add_dependencies(ffmpeg::swscale ffmpeg_build)

add_library(ffmpeg::avcodec STATIC IMPORTED GLOBAL)
set_target_properties(ffmpeg::avcodec PROPERTIES
    IMPORTED_LOCATION ${FFMPEG_INSTALL_DIR}/lib/libavcodec.a
    INTERFACE_INCLUDE_DIRECTORIES ${FFMPEG_INSTALL_DIR}/include
    INTERFACE_LINK_LIBRARIES "ffmpeg::swresample;ffmpeg::avutil"
)
add_dependencies(ffmpeg::avcodec ffmpeg_build)

add_library(ffmpeg::avformat STATIC IMPORTED GLOBAL)
set_target_properties(ffmpeg::avformat PROPERTIES
    IMPORTED_LOCATION ${FFMPEG_INSTALL_DIR}/lib/libavformat.a
    INTERFACE_INCLUDE_DIRECTORIES ${FFMPEG_INSTALL_DIR}/include
    INTERFACE_LINK_LIBRARIES "ffmpeg::avcodec;ffmpeg::avutil"
)
add_dependencies(ffmpeg::avformat ffmpeg_build)

# Combined target
add_library(ffmpeg INTERFACE)
target_link_libraries(ffmpeg INTERFACE
    ffmpeg::avformat
    ffmpeg::avcodec
    ffmpeg::swscale
    ffmpeg::swresample
    ffmpeg::avutil
    m z pthread
)
add_dependencies(ffmpeg ffmpeg_build)

set(FFMPEG_INCLUDE_DIR ${FFMPEG_INSTALL_DIR}/include PARENT_SCOPE)
set(FFMPEG_LIBRARIES ffmpeg PARENT_SCOPE)
