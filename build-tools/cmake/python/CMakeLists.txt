# Python embedded build from source
# Builds a minimal Python interpreter for embedding
include(ExternalProject)

# Use CPYTHON_BUILD_VERSION to avoid conflict with pybind11's PYTHON_VERSION variable
set(CPYTHON_BUILD_VERSION "3.13.2" CACHE STRING "Python version to build from source")
set(PYTHON_URL "https://github.com/python/cpython/archive/refs/tags/v${CPYTHON_BUILD_VERSION}.tar.gz")

# Derive major.minor from version (e.g. "3.13" from "3.13.2")
string(REGEX MATCH "^[0-9]+\\.[0-9]+" PYTHON_MAJOR_MINOR "${CPYTHON_BUILD_VERSION}")

set(PYTHON_PREFIX "${CMAKE_BINARY_DIR}/python")
set(PYTHON_INSTALL_DIR "${PYTHON_PREFIX}/install")
set(PYTHON_INCLUDE_DIR "${PYTHON_INSTALL_DIR}/include/python${PYTHON_MAJOR_MINOR}")
set(PYTHON_LIB_DIR "${PYTHON_INSTALL_DIR}/lib")

include(ProcessorCount)
ProcessorCount(NPROC)
if(NPROC EQUAL 0)
    set(NPROC 4)
endif()

# Configure options for embedded Python
# --disable-shared: Build libpython as static library (avoid runtime symbol conflicts with system Python)
# --with-ensurepip: Include pip for package installation
# --disable-test-modules: Skip test modules to reduce size
# CFLAGS=-fPIC: Position-independent code required for shared plugin
set(PYTHON_CONFIGURE_OPTS
    --prefix=${PYTHON_INSTALL_DIR}
    --disable-shared
    --with-ensurepip
    --disable-test-modules
)

# Set environment variables for configure
# Use -O3 without -g to avoid 25MB of debug info in final binary
# Use ccache if available for faster rebuilds
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(PYTHON_CC "ccache ${CMAKE_C_COMPILER}")
    set(PYTHON_CXX "ccache ${CMAKE_CXX_COMPILER}")
else()
    set(PYTHON_CC "${CMAKE_C_COMPILER}")
    set(PYTHON_CXX "${CMAKE_CXX_COMPILER}")
endif()

set(PYTHON_CONFIGURE_ENV
    "CC=${PYTHON_CC}"
    "CXX=${PYTHON_CXX}"
    "CFLAGS=-fPIC -O3 -DNDEBUG"
    "CXXFLAGS=-fPIC -O3 -DNDEBUG"
)

if(APPLE)
    # macOS specific options
    # Don't use --enable-universalsdk as it requires specifying architectures
    # and we only need to build for the native architecture
    list(APPEND PYTHON_CONFIGURE_OPTS
        --without-dtrace
    )
    # Prevent Python from finding libintl by unsetting the library paths
    # This avoids libintl dependency in the _locale module
    list(APPEND PYTHON_CONFIGURE_ENV
        "LIBRARY_PATH="
        "C_INCLUDE_PATH="
        "CPLUS_INCLUDE_PATH="
    )
endif()

# Determine library name based on platform
if(APPLE)
    set(PYTHON_LIB_NAME "libpython${PYTHON_MAJOR_MINOR}.a")
elseif(WIN32)
    set(PYTHON_LIB_NAME "libpython${PYTHON_MAJOR_MINOR}.a")
else()
    set(PYTHON_LIB_NAME "libpython${PYTHON_MAJOR_MINOR}.a")
endif()

set(PYTHON_LIB_PATH "${PYTHON_LIB_DIR}/${PYTHON_LIB_NAME}")

ExternalProject_Add(python_build
    URL ${PYTHON_URL}
    PREFIX ${PYTHON_PREFIX}
    CONFIGURE_COMMAND env ${PYTHON_CONFIGURE_ENV} <SOURCE_DIR>/configure ${PYTHON_CONFIGURE_OPTS}
    BUILD_COMMAND make -j${NPROC}
    INSTALL_COMMAND make install
    BUILD_IN_SOURCE 1
    BUILD_BYPRODUCTS
        ${PYTHON_LIB_PATH}
)

# Create the include directory early so CMake doesn't complain
# The actual headers will be installed by python_build
file(MAKE_DIRECTORY ${PYTHON_INCLUDE_DIR})

# Create imported target for Python (static library)
add_library(python_embedded STATIC IMPORTED GLOBAL)
set_target_properties(python_embedded PROPERTIES
    IMPORTED_LOCATION "${PYTHON_LIB_PATH}"
)

# Use target_include_directories with SYSTEM to avoid warnings
# and wrap in BUILD_INTERFACE to avoid export issues
target_include_directories(python_embedded SYSTEM INTERFACE
    $<BUILD_INTERFACE:${PYTHON_INCLUDE_DIR}>
)

add_dependencies(python_embedded python_build)

# Export paths for use elsewhere
set(PYTHON_EMBEDDED_INCLUDE_DIR ${PYTHON_INCLUDE_DIR} CACHE INTERNAL "")
set(PYTHON_EMBEDDED_LIB_DIR ${PYTHON_LIB_DIR} CACHE INTERNAL "")
set(PYTHON_EMBEDDED_BIN_DIR ${PYTHON_INSTALL_DIR}/bin CACHE INTERNAL "")
set(PYTHON_EMBEDDED_EXECUTABLE ${PYTHON_INSTALL_DIR}/bin/python3 CACHE INTERNAL "")

message(STATUS "Python ${CPYTHON_BUILD_VERSION} will be built from source")
message(STATUS "  Install dir: ${PYTHON_INSTALL_DIR}")
