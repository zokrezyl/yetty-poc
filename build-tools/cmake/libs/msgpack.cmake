# msgpack - MessagePack serialization
if(TARGET msgpack-cxx)
    return()
endif()

CPMAddPackage(
    NAME msgpack-cxx
    GITHUB_REPOSITORY msgpack/msgpack-c
    GIT_TAG cpp-7.0.0
    OPTIONS
        "MSGPACK_BUILD_DOCS OFF"
        "MSGPACK_BUILD_TESTS OFF"
        "MSGPACK_BUILD_EXAMPLES OFF"
        "MSGPACK_USE_BOOST OFF"
)
