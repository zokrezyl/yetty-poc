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
    
    # Use janbar/openssl-cmake - download only first
    CPMAddPackage(
        NAME openssl
        GITHUB_REPOSITORY janbar/openssl-cmake
        GIT_TAG 1.1.1w-20250419
        DOWNLOAD_ONLY YES
    )
    
    # Fix line 146: if( HAVE_LONG_INT AND (${LONG_INT} EQUAL 8) )
    # When LONG_INT is empty (e.g., Emscripten), CMake fails. Quote the variable.
    file(READ "${openssl_SOURCE_DIR}/CMakeLists.txt" _openssl_cmake)
    string(REPLACE 
        "if( HAVE_LONG_INT AND (\${LONG_INT} EQUAL 8) )"
        "if( HAVE_LONG_INT AND (\"\${LONG_INT}\" EQUAL 8) )"
        _openssl_cmake "${_openssl_cmake}")
    file(WRITE "${openssl_SOURCE_DIR}/CMakeLists.txt" "${_openssl_cmake}")
    
    # Now add the subdirectory with options
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(WITH_APPS OFF CACHE BOOL "" FORCE)
    add_subdirectory(${openssl_SOURCE_DIR} ${openssl_BINARY_DIR})

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
