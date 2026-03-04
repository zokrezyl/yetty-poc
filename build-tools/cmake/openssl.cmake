# OpenSSL configuration for all platforms
# - Desktop (Linux, macOS, Windows): Use system OpenSSL or build from source
# - Android: Build from source
# - WebAssembly: Build from source (Emscripten compatible)

# Try to find system OpenSSL first (works on most desktop platforms)
if(NOT EMSCRIPTEN AND NOT YETTY_ANDROID)
    find_package(OpenSSL QUIET)
endif()

if(OPENSSL_FOUND)
    message(STATUS "Using system OpenSSL: ${OPENSSL_VERSION}")
else()
    message(STATUS "Building OpenSSL from source (janbar/openssl-cmake)")
    
    # Use janbar/openssl-cmake with CMake policy workaround
    # Pin to a specific commit known to work
    CPMAddPackage(
        NAME openssl
        GITHUB_REPOSITORY janbar/openssl-cmake
        GIT_TAG 1.1.1w-20250419
        OPTIONS
            "BUILD_SHARED_LIBS OFF"
            "WITH_APPS OFF"
    )

    # Create OpenSSL::Crypto target that cpr/curl expect
    if(NOT TARGET OpenSSL::Crypto)
        add_library(OpenSSL::Crypto INTERFACE IMPORTED GLOBAL)
        target_link_libraries(OpenSSL::Crypto INTERFACE crypto)
        target_include_directories(OpenSSL::Crypto INTERFACE "${openssl_SOURCE_DIR}/include")
    endif()

    # Create OpenSSL::SSL target that cpr/curl expect
    if(NOT TARGET OpenSSL::SSL)
        add_library(OpenSSL::SSL INTERFACE IMPORTED GLOBAL)
        target_link_libraries(OpenSSL::SSL INTERFACE ssl OpenSSL::Crypto)
        target_include_directories(OpenSSL::SSL INTERFACE "${openssl_SOURCE_DIR}/include")
    endif()

    # Set variables for find_package(OpenSSL) compatibility
    set(OPENSSL_FOUND TRUE CACHE BOOL "" FORCE)
    set(OPENSSL_INCLUDE_DIR "${openssl_SOURCE_DIR}/include" CACHE PATH "" FORCE)
    set(OPENSSL_CRYPTO_LIBRARY OpenSSL::Crypto CACHE STRING "" FORCE)
    set(OPENSSL_SSL_LIBRARY OpenSSL::SSL CACHE STRING "" FORCE)
    set(OPENSSL_LIBRARIES OpenSSL::SSL OpenSSL::Crypto CACHE STRING "" FORCE)
    set(OPENSSL_VERSION "1.1.1w" CACHE STRING "" FORCE)
endif()
