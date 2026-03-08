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

    # Android cross-compilation setup
    if(ANDROID)
        # Map Android ABI to meson cpu/cpu_family
        if(ANDROID_ABI STREQUAL "arm64-v8a")
            set(DAV1D_CPU "aarch64")
            set(DAV1D_CPU_FAMILY "aarch64")
        elseif(ANDROID_ABI STREQUAL "armeabi-v7a")
            set(DAV1D_CPU "armv7")
            set(DAV1D_CPU_FAMILY "arm")
        elseif(ANDROID_ABI STREQUAL "x86_64")
            set(DAV1D_CPU "x86_64")
            set(DAV1D_CPU_FAMILY "x86_64")
        elseif(ANDROID_ABI STREQUAL "x86")
            set(DAV1D_CPU "i686")
            set(DAV1D_CPU_FAMILY "x86")
        endif()

        # Generate meson cross file for Android
        set(DAV1D_CROSS_FILE "${CMAKE_BINARY_DIR}/_deps/dav1d-android-cross.txt")
        file(WRITE ${DAV1D_CROSS_FILE}
"[binaries]
c = '${CMAKE_C_COMPILER}'
cpp = '${CMAKE_CXX_COMPILER}'
ar = '${CMAKE_AR}'
strip = '${CMAKE_STRIP}'

[built-in options]
c_args = ['-DANDROID', '-fPIC', '--target=${ANDROID_LLVM_TRIPLE}', '--sysroot=${CMAKE_SYSROOT}']
c_link_args = ['--target=${ANDROID_LLVM_TRIPLE}', '--sysroot=${CMAKE_SYSROOT}']
cpp_args = ['-DANDROID', '-fPIC', '--target=${ANDROID_LLVM_TRIPLE}', '--sysroot=${CMAKE_SYSROOT}']
cpp_link_args = ['--target=${ANDROID_LLVM_TRIPLE}', '--sysroot=${CMAKE_SYSROOT}']

[host_machine]
system = 'android'
cpu_family = '${DAV1D_CPU_FAMILY}'
cpu = '${DAV1D_CPU}'
endian = 'little'
")
        set(DAV1D_CROSS_ARGS --cross-file ${DAV1D_CROSS_FILE})
    else()
        set(DAV1D_CROSS_ARGS "")
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
            ${DAV1D_CROSS_ARGS}

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
