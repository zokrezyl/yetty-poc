# Shared configuration for all platforms
# Include this before platform-specific target files

# Common libraries needed by all platforms
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/glm.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/stb.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/yaml-cpp.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/spdlog.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/ytrace.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/msgpack.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/webgpu.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/vterm.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/zlib.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/libpng.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/msdfgen.cmake)
include(${CMAKE_SOURCE_DIR}/build-tools/cmake/libs/cdb.cmake)

# Common include directories
set(YETTY_INCLUDES
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

# Common compile definitions
set(YETTY_DEFINITIONS
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

# Common libraries to link
set(YETTY_LIBS
    webgpu
    glm::glm
    stb
    yaml-cpp
    vterm
    msgpack-cxx
    msdfgen::msdfgen-core
    msdfgen::msdfgen-ext
    cdb-wrapper
    yetty_base
    yetty_font
    yetty_ymery
    yetty_yecho
    yetty_ydraw
    yetty_diagram
    yetty_cards
    yetty_yast
    msdf-wgsl
)

