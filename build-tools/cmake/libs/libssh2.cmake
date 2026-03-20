# libssh2 - SSH2 protocol library
# https://github.com/libssh2/libssh2
if(TARGET libssh2_static)
    return()
endif()

# libssh2 requires OpenSSL/crypto
include(${YETTY_ROOT}/build-tools/cmake/openssl.cmake)

# Disable zlib to avoid export issues
set(ENABLE_ZLIB_COMPRESSION OFF CACHE BOOL "" FORCE)

CPMAddPackage(
    NAME libssh2
    GITHUB_REPOSITORY libssh2/libssh2
    GIT_TAG libssh2-1.11.1
    DOWNLOAD_ONLY YES
)

if(libssh2_ADDED)
    # Patch the CMakeLists.txt to disable exports
    file(READ "${libssh2_SOURCE_DIR}/src/CMakeLists.txt" _libssh2_cmake)
    string(REPLACE "install(EXPORT Libssh2Config" "# install(EXPORT Libssh2Config" _libssh2_cmake "${_libssh2_cmake}")
    string(REPLACE "export(TARGETS" "# export(TARGETS" _libssh2_cmake "${_libssh2_cmake}")
    file(WRITE "${libssh2_SOURCE_DIR}/src/CMakeLists.txt" "${_libssh2_cmake}")

    # Now add subdirectory
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
    set(CRYPTO_BACKEND "OpenSSL" CACHE STRING "" FORCE)
    add_subdirectory(${libssh2_SOURCE_DIR} ${libssh2_BINARY_DIR})

    message(STATUS "libssh2: Built from source v1.11.1")

    # Export variables for find_package compatibility
    set(LIBSSH2_FOUND TRUE CACHE BOOL "" FORCE)
    set(LIBSSH2_INCLUDE_DIR "${libssh2_SOURCE_DIR}/include;${libssh2_BINARY_DIR}/src" CACHE PATH "" FORCE)
    set(LIBSSH2_LIBRARY libssh2_static CACHE STRING "" FORCE)
    set(LIBSSH2_LIBRARIES libssh2_static CACHE STRING "" FORCE)
endif()
