# Shared configuration for all platforms
# Include this before platform-specific target files

# Common libraries needed by all platforms
include(${YETTY_ROOT}/build-tools/cmake/libs/args.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/lz4.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/libuv.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/imgui.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/glm.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/stb.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/yaml-cpp.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/spdlog.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/ytrace.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/msgpack.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/webgpu.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/vterm.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/zlib.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/libpng.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/msdfgen.cmake)
include(${YETTY_ROOT}/build-tools/cmake/libs/cdb.cmake)

# Common include directories
set(YETTY_INCLUDES
    ${YETTY_ROOT}/src
    ${YETTY_ROOT}/include
)

# Common compile definitions
set(YETTY_DEFINITIONS
    CMAKE_SOURCE_DIR="${YETTY_ROOT}"
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
    yetty_yecho
    yetty_ydraw
    yetty_diagram
    ygrid
    yetty_cards
    yetty_yast
    msdf-wgsl
)

