# dav1d - AV1 video decoder
# BSD 2-Clause license, VideoLAN project
# Uses Meson build system, requires meson and ninja

include(ExternalProject)
include(ProcessorCount)

ProcessorCount(NPROC)
if(NPROC EQUAL 0)
    set(NPROC 4)
endif()

set(DAV1D_VERSION "1.5.0")

# Download dav1d source
CPMAddPackage(
    NAME dav1d
    URL https://code.videolan.org/videolan/dav1d/-/archive/${DAV1D_VERSION}/dav1d-${DAV1D_VERSION}.tar.gz
    DOWNLOAD_ONLY YES
)

if(dav1d_ADDED)
    set(DAV1D_INSTALL_DIR "${CMAKE_BINARY_DIR}/_deps/dav1d-install")
    set(DAV1D_BUILD_DIR "${CMAKE_BINARY_DIR}/_deps/dav1d-build")

    # Platform-specific library name
    if(WIN32)
        set(DAV1D_LIB_NAME "dav1d.lib")
    else()
        set(DAV1D_LIB_NAME "libdav1d.a")
    endif()

    # Build dav1d using meson + ninja
    ExternalProject_Add(dav1d_ext
        SOURCE_DIR ${dav1d_SOURCE_DIR}
        BINARY_DIR ${DAV1D_BUILD_DIR}
        INSTALL_DIR ${DAV1D_INSTALL_DIR}

        UPDATE_DISCONNECTED TRUE

        CONFIGURE_COMMAND
            meson setup ${dav1d_SOURCE_DIR} ${DAV1D_BUILD_DIR}
            --prefix=${DAV1D_INSTALL_DIR}
            --libdir=lib
            --default-library=static
            --buildtype=release
            -Denable_tools=false
            -Denable_tests=false
            -Denable_examples=false

        BUILD_COMMAND ninja -C ${DAV1D_BUILD_DIR} -j${NPROC}
        INSTALL_COMMAND ninja -C ${DAV1D_BUILD_DIR} install

        BUILD_BYPRODUCTS
            ${DAV1D_INSTALL_DIR}/lib/${DAV1D_LIB_NAME}
            ${DAV1D_INSTALL_DIR}/include/dav1d/dav1d.h
    )

    # Pre-create include dir for CMake validation
    file(MAKE_DIRECTORY ${DAV1D_INSTALL_DIR}/include)

    # Create imported static library target
    add_library(dav1d STATIC IMPORTED GLOBAL)
    set_target_properties(dav1d PROPERTIES
        IMPORTED_LOCATION ${DAV1D_INSTALL_DIR}/lib/${DAV1D_LIB_NAME}
        INTERFACE_INCLUDE_DIRECTORIES ${DAV1D_INSTALL_DIR}/include
    )
    add_dependencies(dav1d dav1d_ext)

    # On Unix, dav1d may need pthreads and libdl
    if(UNIX AND NOT APPLE)
        find_package(Threads REQUIRED)
        set_target_properties(dav1d PROPERTIES
            INTERFACE_LINK_LIBRARIES "Threads::Threads;${CMAKE_DL_LIBS}"
        )
    endif()

    message(STATUS "dav1d: Built from source v${DAV1D_VERSION}")
endif()
