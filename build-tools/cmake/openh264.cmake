# openh264 - H.264 video decoder
# BSD 2-Clause license, Cisco
# Used in Firefox, WebRTC - battle-tested

CPMAddPackage(
    NAME openh264
    GITHUB_REPOSITORY cisco/openh264
    VERSION 2.4.1
    DOWNLOAD_ONLY YES
)

if(openh264_ADDED)
    include(ExternalProject)
    include(ProcessorCount)

    ProcessorCount(NPROC)
    if(NPROC EQUAL 0)
        set(NPROC 4)
    endif()

    set(OPENH264_INSTALL_DIR "${CMAKE_BINARY_DIR}/_deps/openh264-install")

    # Platform-specific library name
    if(WIN32)
        set(OPENH264_LIB_NAME "openh264.lib")
    else()
        set(OPENH264_LIB_NAME "libopenh264.a")
    endif()

    # Platform-specific build setup
    if(ANDROID)
        # Map Android ABI to openh264 ARCH
        if(ANDROID_ABI STREQUAL "arm64-v8a")
            set(OPENH264_ARCH "arm64")
        elseif(ANDROID_ABI STREQUAL "armeabi-v7a")
            set(OPENH264_ARCH "arm")
        elseif(ANDROID_ABI STREQUAL "x86_64")
            set(OPENH264_ARCH "x86_64")
        elseif(ANDROID_ABI STREQUAL "x86")
            set(OPENH264_ARCH "x86")
        endif()

        set(OPENH264_BUILD_ARGS "OS=android ARCH=${OPENH264_ARCH} NDKROOT=${ANDROID_NDK} TARGET=android-${ANDROID_NATIVE_API_LEVEL} NDKLEVEL=${ANDROID_NATIVE_API_LEVEL}")

        # Build openh264 using make (Android)
        ExternalProject_Add(openh264_ext
            SOURCE_DIR ${openh264_SOURCE_DIR}
            BUILD_IN_SOURCE TRUE
            INSTALL_DIR ${OPENH264_INSTALL_DIR}

            UPDATE_DISCONNECTED TRUE

            CONFIGURE_COMMAND ""

            BUILD_COMMAND sh -c "make MAKEFLAGS= -j${NPROC} libraries BUILDTYPE=Release ENABLE_SHARED=No PREFIX=${OPENH264_INSTALL_DIR} ${OPENH264_BUILD_ARGS}"

            INSTALL_COMMAND sh -c "make MAKEFLAGS= install-static BUILDTYPE=Release ENABLE_SHARED=No PREFIX=${OPENH264_INSTALL_DIR} ${OPENH264_BUILD_ARGS}"

            BUILD_BYPRODUCTS
                ${OPENH264_INSTALL_DIR}/lib/${OPENH264_LIB_NAME}
        )
    elseif(WIN32)
        # Windows: Use make with OS=msvc (requires make installed via chocolatey)
        # Determine architecture
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(OPENH264_ARCH "x86_64")
        else()
            set(OPENH264_ARCH "i386")
        endif()

        ExternalProject_Add(openh264_ext
            SOURCE_DIR ${openh264_SOURCE_DIR}
            BUILD_IN_SOURCE TRUE
            INSTALL_DIR ${OPENH264_INSTALL_DIR}

            UPDATE_DISCONNECTED TRUE

            CONFIGURE_COMMAND ""

            BUILD_COMMAND make MAKEFLAGS= -j${NPROC} OS=msvc ARCH=${OPENH264_ARCH} libraries BUILDTYPE=Release ENABLE_SHARED=No PREFIX=${OPENH264_INSTALL_DIR}

            INSTALL_COMMAND make MAKEFLAGS= OS=msvc ARCH=${OPENH264_ARCH} install-static BUILDTYPE=Release ENABLE_SHARED=No PREFIX=${OPENH264_INSTALL_DIR}

            BUILD_BYPRODUCTS
                ${OPENH264_INSTALL_DIR}/lib/${OPENH264_LIB_NAME}
        )
    elseif(EMSCRIPTEN)
        # Emscripten/WebAssembly: Use emmake with ARCH= to prevent -m64 flags
        # Add -pthread for threading support
        # Define __APPLE__ to use pthread_cond_t instead of sem_t (sem_timedwait not available in emscripten)
        ExternalProject_Add(openh264_ext
            SOURCE_DIR ${openh264_SOURCE_DIR}
            BUILD_IN_SOURCE TRUE
            INSTALL_DIR ${OPENH264_INSTALL_DIR}

            UPDATE_DISCONNECTED TRUE

            CONFIGURE_COMMAND ""

            BUILD_COMMAND sh -c "emmake make MAKEFLAGS= -j${NPROC} libraries BUILDTYPE=Release ENABLE_SHARED=No USE_ASM=No ARCH= 'CFLAGS=-pthread -D__APPLE__' 'CXXFLAGS=-pthread -D__APPLE__' 'LDFLAGS=-pthread' PREFIX=${OPENH264_INSTALL_DIR}"

            INSTALL_COMMAND sh -c "emmake make MAKEFLAGS= install-static BUILDTYPE=Release ENABLE_SHARED=No USE_ASM=No ARCH= PREFIX=${OPENH264_INSTALL_DIR}"

            BUILD_BYPRODUCTS
                ${OPENH264_INSTALL_DIR}/lib/${OPENH264_LIB_NAME}
        )
    else()
        # Unix/Linux/macOS: Use make
        ExternalProject_Add(openh264_ext
            SOURCE_DIR ${openh264_SOURCE_DIR}
            BUILD_IN_SOURCE TRUE
            INSTALL_DIR ${OPENH264_INSTALL_DIR}

            UPDATE_DISCONNECTED TRUE

            CONFIGURE_COMMAND ""

            BUILD_COMMAND sh -c "make MAKEFLAGS= -j${NPROC} libraries BUILDTYPE=Release ENABLE_SHARED=No PREFIX=${OPENH264_INSTALL_DIR}"

            INSTALL_COMMAND sh -c "make MAKEFLAGS= install-static BUILDTYPE=Release ENABLE_SHARED=No PREFIX=${OPENH264_INSTALL_DIR}"

            BUILD_BYPRODUCTS
                ${OPENH264_INSTALL_DIR}/lib/${OPENH264_LIB_NAME}
        )
    endif()

    # Pre-create include dir for CMake validation
    file(MAKE_DIRECTORY ${OPENH264_INSTALL_DIR}/include)

    # Create imported static library target
    add_library(openh264 STATIC IMPORTED GLOBAL)
    set_target_properties(openh264 PROPERTIES
        IMPORTED_LOCATION ${OPENH264_INSTALL_DIR}/lib/${OPENH264_LIB_NAME}
        INTERFACE_INCLUDE_DIRECTORIES ${OPENH264_INSTALL_DIR}/include
    )
    add_dependencies(openh264 openh264_ext)

    # On Unix, openh264 needs pthread and stdc++
    if(UNIX)
        find_package(Threads REQUIRED)
        set_target_properties(openh264 PROPERTIES
            INTERFACE_LINK_LIBRARIES "Threads::Threads;stdc++"
        )
    endif()

    message(STATUS "openh264: Built from source v2.4.1")
endif()
