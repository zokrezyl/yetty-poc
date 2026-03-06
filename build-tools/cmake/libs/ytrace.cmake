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

# Allow command-line override of logging levels (default: all ON)
# yinfo builds pass -DYTRACE_ENABLE_YTRACE=0 -DYTRACE_ENABLE_YDEBUG=0
if(NOT DEFINED YTRACE_ENABLE_YTRACE)
    set(YTRACE_ENABLE_YTRACE ON)
endif()
if(NOT DEFINED YTRACE_ENABLE_YDEBUG)
    set(YTRACE_ENABLE_YDEBUG ON)
endif()
if(NOT DEFINED YTRACE_ENABLE_YINFO)
    set(YTRACE_ENABLE_YINFO ON)
endif()
if(NOT DEFINED YTRACE_ENABLE_YWARN)
    set(YTRACE_ENABLE_YWARN ON)
endif()
if(NOT DEFINED YTRACE_ENABLE_YLOG)
    set(YTRACE_ENABLE_YLOG ON)
endif()
if(NOT DEFINED YTRACE_ENABLE_YFUNC)
    set(YTRACE_ENABLE_YFUNC ON)
endif()

CPMAddPackage(
    NAME ytrace
    GITHUB_REPOSITORY zokrezyl/ytrace
    GIT_TAG da73e1fba172c61d204debd84f30f4703c6f3d19
    OPTIONS
        ${YTRACE_BUILD_TOOLS_OPT}
        "YTRACE_BUILD_EXAMPLES OFF"
        "YTRACE_ENABLE_YLOG ${YTRACE_ENABLE_YLOG}"
        "YTRACE_ENABLE_YTRACE ${YTRACE_ENABLE_YTRACE}"
        "YTRACE_ENABLE_YDEBUG ${YTRACE_ENABLE_YDEBUG}"
        "YTRACE_ENABLE_YINFO ${YTRACE_ENABLE_YINFO}"
        "YTRACE_ENABLE_YWARN ${YTRACE_ENABLE_YWARN}"
        "YTRACE_ENABLE_YFUNC ${YTRACE_ENABLE_YFUNC}"
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
