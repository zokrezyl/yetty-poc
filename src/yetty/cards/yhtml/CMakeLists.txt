# YHtml Card - HTML/CSS rendering via litehtml + YDrawBase SDF primitives
# Gated behind YETTY_CARD_YHTML option (set in parent CMakeLists.txt)

CPMAddPackage(
    NAME litehtml
    GITHUB_REPOSITORY litehtml/litehtml
    GIT_TAG v0.9
    OPTIONS
        "LITEHTML_BUILD_TESTING OFF"
)

# cpr builds libcurl from source. Force-disable optional curl features
# that pull in system libraries curl can't cleanly export.
# These must be set before CPMAddPackage(cpr) because cpr adds curl as
# a subdirectory — CPM OPTIONS only apply to the direct package.
find_package(OpenSSL REQUIRED)

# Save and restore ZLIB state to hide our zlib from curl.
set(_SAVE_ZLIB_FOUND ${ZLIB_FOUND})
set(_SAVE_ZLIB_LIBRARY ${ZLIB_LIBRARY})
set(ZLIB_FOUND FALSE CACHE BOOL "" FORCE)
unset(ZLIB_LIBRARY CACHE)

set(CURL_ZLIB OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)
set(CURL_BROTLI OFF CACHE BOOL "" FORCE)
set(CURL_ZSTD OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBSSH2 OFF CACHE BOOL "" FORCE)

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
)

# Restore ZLIB state after curl configuration
set(ZLIB_FOUND ${_SAVE_ZLIB_FOUND} CACHE BOOL "" FORCE)
if(_SAVE_ZLIB_LIBRARY)
    set(ZLIB_LIBRARY ${_SAVE_ZLIB_LIBRARY} CACHE STRING "" FORCE)
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/src/yetty/yhtml yhtml)

target_sources(yetty PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/yhtml.h
    ${CMAKE_CURRENT_SOURCE_DIR}/yhtml.cpp
)

target_link_libraries(yetty PRIVATE litehtml cpr::cpr)
target_compile_definitions(yetty PRIVATE YETTY_CARD_YHTML=1)
