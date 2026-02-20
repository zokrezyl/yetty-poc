# LZ4 - fast compression
if(TARGET lz4_static)
    return()
endif()

CPMAddPackage(
    NAME lz4
    GITHUB_REPOSITORY lz4/lz4
    GIT_TAG v1.10.0
    SOURCE_SUBDIR build/cmake
    OPTIONS
        "LZ4_BUILD_CLI OFF"
        "LZ4_BUILD_LEGACY_LZ4C OFF"
        "BUILD_SHARED_LIBS OFF"
        "BUILD_STATIC_LIBS ON"
)

if(TARGET lz4_static)
    set_target_properties(lz4_static PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
