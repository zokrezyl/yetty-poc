include(ExternalProject)

set(LIBMAGIC_VERSION "5.45")
set(LIBMAGIC_TAG "FILE5_45")

# Locate the zlib static library built by CPM
# zlibstatic is the CMake target name from zlib's own CMakeLists.txt
set(ZLIB_STATIC_LIB "$<TARGET_FILE:zlibstatic>")

ExternalProject_Add(libmagic_ext
    GIT_REPOSITORY  https://github.com/file/file.git
    GIT_TAG         ${LIBMAGIC_TAG}
    GIT_SHALLOW     TRUE
    PREFIX          ${CMAKE_BINARY_DIR}/_deps/libmagic

    # After initial clone+build, skip update/patch to avoid rebuilds
    UPDATE_DISCONNECTED TRUE

    # autoreconf only runs once (before first configure)
    PATCH_COMMAND   autoreconf -fi

    CONFIGURE_COMMAND
        <SOURCE_DIR>/configure
            --prefix=<INSTALL_DIR>
            --disable-shared
            --enable-static
            --disable-libseccomp
            --disable-bzlib
            --disable-xzlib
            --disable-zstdlib
            "CFLAGS=-fPIC -I${zlib_SOURCE_DIR} -I${zlib_BINARY_DIR}"
            "LDFLAGS=-L${zlib_BINARY_DIR}"

    BUILD_COMMAND     make -j${CMAKE_BUILD_PARALLEL_LEVEL}
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
