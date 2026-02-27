# libuv - async I/O
if(TARGET uv_a)
    return()
endif()

CPMAddPackage(
    NAME libuv
    GITHUB_REPOSITORY libuv/libuv
    GIT_TAG v1.48.0
    OPTIONS
        "LIBUV_BUILD_TESTS OFF"
        "LIBUV_BUILD_BENCH OFF"
        "LIBUV_BUILD_SHARED OFF"
)
