# ytrace - logging with runtime control
# Depends on spdlog
if(TARGET ytrace::ytrace)
    return()
endif()

include(${CMAKE_CURRENT_LIST_DIR}/spdlog.cmake)

if(EMSCRIPTEN OR YETTY_ANDROID)
    # No control socket on Emscripten (no filesystem) or Android (sandboxed fs)
    set(YTRACE_BUILD_TOOLS_OPT "YTRACE_BUILD_TOOLS OFF")
else()
    set(YTRACE_BUILD_TOOLS_OPT "YTRACE_BUILD_TOOLS ON")
endif()

CPMAddPackage(
    NAME ytrace
    GITHUB_REPOSITORY zokrezyl/ytrace
    GIT_TAG da73e1fba172c61d204debd84f30f4703c6f3d19
    OPTIONS
        ${YTRACE_BUILD_TOOLS_OPT}
        "YTRACE_BUILD_EXAMPLES OFF"
        "YTRACE_ENABLE_YLOG ON"
        "YTRACE_ENABLE_YTRACE ON"
        "YTRACE_ENABLE_YDEBUG ON"
        "YTRACE_ENABLE_YINFO ON"
        "YTRACE_ENABLE_YWARN ON"
        "YTRACE_ENABLE_YFUNC ON"
)

# Disable control socket on Android (can't create ~/.cache/ytrace)
if(YETTY_ANDROID)
    target_compile_definitions(ytrace INTERFACE YTRACE_NO_CONTROL_SOCKET)
# Disable control socket on Windows - avoids MSVC bind() vs std::bind() conflict
elseif(WIN32)
    target_compile_definitions(ytrace INTERFACE YTRACE_NO_CONTROL_SOCKET=1)
endif()

# Disable control socket on macOS and Windows (bind() conflicts with std::bind)
if(APPLE OR WIN32)
    target_compile_definitions(ytrace INTERFACE YTRACE_NO_CONTROL_SOCKET)
endif()
