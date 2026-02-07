include(ExternalProject)
include(ProcessorCount)

ProcessorCount(NPROC)
if(NPROC EQUAL 0)
    set(NPROC 4)
endif()

# Use ccache if available
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(LIBMAGIC_CC "ccache ${CMAKE_C_COMPILER}")
else()
    set(LIBMAGIC_CC "${CMAKE_C_COMPILER}")
endif()

set(LIBMAGIC_VERSION "5.45")

# Use official distribution tarball — includes pre-generated configure script,
# no autotools (autoconf/automake/libtool) needed on the build machine.
#
# Note: Do NOT use --host flag - that triggers cross-compile mode which requires
# a pre-installed file binary of the exact same version. Native builds work fine.
#
# Note: Do NOT pass LDFLAGS/LIBS to configure - it breaks the "can run C programs"
# test because the linker paths affect the test binary execution.
# Configure detects system zlib, then we override paths during make.
ExternalProject_Add(libmagic_ext
    URL             https://astron.com/pub/file/file-${LIBMAGIC_VERSION}.tar.gz
    PREFIX          ${CMAKE_BINARY_DIR}/_deps/libmagic

    # After initial download+build, skip update to avoid rebuilds
    UPDATE_DISCONNECTED TRUE

    # Touch auto-generated files to prevent make from trying to regenerate them
    # Tarball timestamps can trigger aclocal.m4 regeneration otherwise
    PATCH_COMMAND
        find <SOURCE_DIR> -name "aclocal.m4" -o -name "configure" -o -name "Makefile.in" -o -name "config.h.in" | xargs touch

    # Configure without our custom paths - let it find system zlib for detection
    # --disable-maintainer-mode prevents make from trying to regenerate aclocal.m4
    CONFIGURE_COMMAND
        ${CMAKE_COMMAND} -E env "CC=${LIBMAGIC_CC}"
        <SOURCE_DIR>/configure
            --prefix=<INSTALL_DIR>
            --disable-shared
            --enable-static
            --disable-libseccomp
            --disable-bzlib
            --disable-xzlib
            --disable-zstdlib
            --disable-lzlib
            --disable-maintainer-mode
            "CFLAGS=-fPIC -O3 -DNDEBUG"

    # Override with our static zlib during build
    BUILD_COMMAND
        ${CMAKE_COMMAND} -E env
            "CPPFLAGS=-I${zlib_SOURCE_DIR} -I${zlib_BINARY_DIR}"
            "LDFLAGS=-L${zlib_BINARY_DIR}"
        make -j${NPROC} LIBS=-lz
    INSTALL_COMMAND   make install
    BUILD_IN_SOURCE   TRUE

    BUILD_BYPRODUCTS
        <INSTALL_DIR>/lib/libmagic.a
        <INSTALL_DIR>/include/magic.h
        <INSTALL_DIR>/share/misc/magic.mgc
)

# libmagic depends on zlib being built first
add_dependencies(libmagic_ext zlibstatic)

ExternalProject_Get_Property(libmagic_ext INSTALL_DIR)

# Pre-create include dir so CMake doesn't reject it at configure time
file(MAKE_DIRECTORY ${INSTALL_DIR}/include)

# Imported static library target
add_library(magic STATIC IMPORTED GLOBAL)
set_target_properties(magic PROPERTIES
    IMPORTED_LOCATION ${INSTALL_DIR}/lib/libmagic.a
    INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
    INTERFACE_LINK_LIBRARIES zlibstatic
)
add_dependencies(magic libmagic_ext)

# Export paths for runtime magic.mgc database
set(LIBMAGIC_MGC_PATH "${INSTALL_DIR}/share/misc/magic.mgc" CACHE FILEPATH
    "Path to compiled magic database" FORCE)
