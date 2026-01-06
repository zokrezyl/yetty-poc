# Python embedded build from source
# Builds a minimal Python interpreter for embedding
include(ExternalProject)

set(PYTHON_VERSION "3.13.2" CACHE STRING "Python version to build")
set(PYTHON_URL "https://github.com/python/cpython/archive/refs/tags/v${PYTHON_VERSION}.tar.gz")

set(PYTHON_PREFIX "${CMAKE_BINARY_DIR}/python")
set(PYTHON_INSTALL_DIR "${PYTHON_PREFIX}/install")
set(PYTHON_INCLUDE_DIR "${PYTHON_INSTALL_DIR}/include/python3.13")
set(PYTHON_LIB_DIR "${PYTHON_INSTALL_DIR}/lib")

include(ProcessorCount)
ProcessorCount(NPROC)
if(NPROC EQUAL 0)
    set(NPROC 4)
endif()

# Configure options for embedded Python
# --enable-shared: Build libpython as shared library
# --with-ensurepip: Include pip for package installation
# --disable-test-modules: Skip test modules to reduce size
set(PYTHON_CONFIGURE_OPTS
    --prefix=${PYTHON_INSTALL_DIR}
    --enable-shared
    --enable-optimizations
    --with-lto
    --with-ensurepip
    --disable-test-modules
)

if(APPLE)
    # macOS specific options
    # Don't use --enable-universalsdk as it requires specifying architectures
    # and we only need to build for the native architecture
    list(APPEND PYTHON_CONFIGURE_OPTS
        --without-dtrace
    )
endif()

# Determine library name based on platform
if(APPLE)
    set(PYTHON_LIB_NAME "libpython3.13.dylib")
elseif(WIN32)
    set(PYTHON_LIB_NAME "python313.dll")
else()
    set(PYTHON_LIB_NAME "libpython3.13.so")
endif()

set(PYTHON_LIB_PATH "${PYTHON_LIB_DIR}/${PYTHON_LIB_NAME}")

ExternalProject_Add(python_build
    URL ${PYTHON_URL}
    PREFIX ${PYTHON_PREFIX}
    CONFIGURE_COMMAND <SOURCE_DIR>/configure ${PYTHON_CONFIGURE_OPTS}
    BUILD_COMMAND make -j${NPROC}
    INSTALL_COMMAND make install
    BUILD_IN_SOURCE 0
    BUILD_BYPRODUCTS
        ${PYTHON_LIB_PATH}
)

# Create the include directory early so CMake doesn't complain
# The actual headers will be installed by python_build
file(MAKE_DIRECTORY ${PYTHON_INCLUDE_DIR})

# Create imported target for Python
add_library(python_embedded SHARED IMPORTED GLOBAL)
set_target_properties(python_embedded PROPERTIES
    IMPORTED_LOCATION "${PYTHON_LIB_PATH}"
    IMPORTED_NO_SONAME TRUE
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

message(STATUS "Python ${PYTHON_VERSION} will be built from source")
message(STATUS "  Install dir: ${PYTHON_INSTALL_DIR}")
