# YHtml Card - HTML/CSS rendering via litehtml + YDrawBase SDF primitives

CPMAddPackage(
    NAME litehtml
    GITHUB_REPOSITORY litehtml/litehtml
    GIT_TAG v0.9
    OPTIONS
        "LITEHTML_BUILD_TESTING OFF"
)

# cpr builds libcurl from source. Force-disable optional curl features
# On Windows, use Schannel (native SSL). On other platforms, use OpenSSL.
if(WIN32)
    set(CURL_USE_SCHANNEL ON CACHE BOOL "" FORCE)
    set(CURL_USE_OPENSSL OFF CACHE BOOL "" FORCE)
else()
    find_package(OpenSSL REQUIRED)
    set(CURL_USE_SCHANNEL OFF CACHE BOOL "" FORCE)
    set(CURL_USE_OPENSSL ON CACHE BOOL "" FORCE)
endif()

# Save and restore ZLIB state to hide our zlib from curl
set(_SAVE_ZLIB_FOUND ${ZLIB_FOUND})
set(_SAVE_ZLIB_LIBRARY ${ZLIB_LIBRARY})
set(ZLIB_FOUND FALSE CACHE BOOL "" FORCE)
unset(ZLIB_LIBRARY CACHE)

set(CURL_ZLIB OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)
set(CURL_BROTLI OFF CACHE BOOL "" FORCE)
set(CURL_ZSTD OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBSSH2 OFF CACHE BOOL "" FORCE)

if(WIN32)
    set(_CPR_SSL_OPTIONS "CURL_USE_SCHANNEL ON" "CURL_USE_OPENSSL OFF")
else()
    set(_CPR_SSL_OPTIONS "CURL_USE_SCHANNEL OFF" "CURL_USE_OPENSSL ON")
endif()

CPMAddPackage(
    NAME cpr
    GITHUB_REPOSITORY libcpr/cpr
    GIT_TAG 1.14.1
    OPTIONS
        "CPR_USE_SYSTEM_CURL OFF"
        "CPR_CURL_USE_LIBPSL OFF"
        "BUILD_SHARED_LIBS OFF"
        "CPR_ENABLE_SSL ON"
        "CPR_BUILD_TESTS OFF"
        "CPR_BUILD_TESTS_SSL OFF"
        ${_CPR_SSL_OPTIONS}
)

# Restore ZLIB state after curl configuration
set(ZLIB_FOUND ${_SAVE_ZLIB_FOUND} CACHE BOOL "" FORCE)
if(_SAVE_ZLIB_LIBRARY)
    set(ZLIB_LIBRARY ${_SAVE_ZLIB_LIBRARY} CACHE STRING "" FORCE)
endif()

add_subdirectory(${YETTY_ROOT}/src/yetty/yhtml yhtml)

add_library(yetty_card_yhtml STATIC yhtml.cpp)
target_include_directories(yetty_card_yhtml PUBLIC ${YETTY_ROOT}/include ${YETTY_ROOT}/src)
target_link_libraries(yetty_card_yhtml PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace yetty_yhtml litehtml cpr::cpr)
target_compile_definitions(yetty_card_yhtml PUBLIC YETTY_CARD_YHTML=1)
