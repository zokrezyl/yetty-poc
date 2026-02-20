# GLFW - Window and input handling
if(TARGET glfw)
    return()
endif()

# Disable X11 on macOS
if(APPLE)
    set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE)
endif()

CPMAddPackage(
    NAME glfw
    GITHUB_REPOSITORY glfw/glfw
    GIT_TAG 3.4
    OPTIONS
        "GLFW_BUILD_DOCS OFF"
        "GLFW_BUILD_TESTS OFF"
        "GLFW_BUILD_EXAMPLES OFF"
        "GLFW_INSTALL OFF"
        "GLFW_BUILD_WAYLAND OFF"
)

# glfw3webgpu adapter
if(NOT TARGET glfw3webgpu)
    CPMAddPackage(
        NAME glfw3webgpu
        GITHUB_REPOSITORY eliemichel/glfw3webgpu
        GIT_TAG main
    )
    # glfw3webgpu needs _GLFW_X11 on Linux for X11 surface creation
    if(UNIX AND NOT APPLE)
        target_compile_definitions(glfw3webgpu PRIVATE _GLFW_X11)
    endif()
endif()
