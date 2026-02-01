# yetty_python — generic Python/C++ embedding with pybind11
# Provides: ref-counted interpreter, GIL helpers, yetty_card embedded module
# Self-contained: pulls in cpython build (python_embedded) + pybind11

# Build CPython from source (creates python_embedded imported target)
if(UNIX AND NOT TARGET python_embedded)
    add_subdirectory(${CMAKE_SOURCE_DIR}/build-tools/cmake/python ${CMAKE_BINARY_DIR}/python_build)
endif()

if(NOT TARGET python_embedded)
    message(WARNING "yetty_python: python_embedded not available (non-Unix or build failed)")
    return()
endif()

CPMAddPackage(
    NAME pybind11
    GITHUB_REPOSITORY pybind/pybind11
    VERSION 2.13.6
)

if(NOT pybind11_ADDED)
    message(WARNING "yetty_python: failed to fetch pybind11")
    return()
endif()

add_library(yetty_python STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/yetty_card_module.cpp
)

target_include_directories(yetty_python PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(yetty_python PUBLIC
    pybind11::headers
    ytrace::ytrace
    webgpu
)

# python_embedded provides Python include dirs and the static libpython
target_link_libraries(yetty_python PUBLIC python_embedded)
add_dependencies(yetty_python python_embedded)

target_compile_definitions(yetty_python PRIVATE
    CMAKE_BINARY_DIR="${CMAKE_BINARY_DIR}"
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

set_target_properties(yetty_python PROPERTIES POSITION_INDEPENDENT_CODE ON)

message(STATUS "yetty_python: enabled (pybind11 v2.13.6)")
