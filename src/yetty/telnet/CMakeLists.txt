# Telnet client for Android terminal connections
# Connects to local telnetd (e.g., Termux's busybox telnetd)
# Uses POSIX socket APIs — not available on Windows (uses ConPTY instead)
if(WIN32)
    # Stub library so dependent targets still resolve
    add_library(yetty_telnet INTERFACE)
    target_include_directories(yetty_telnet INTERFACE
        ${YETTY_ROOT}/src
        ${YETTY_ROOT}/include
    )
    return()
endif()

add_library(yetty_telnet STATIC
    telnet-client.cpp
    telnet-pty-reader.cpp
)

target_include_directories(yetty_telnet PUBLIC
    ${YETTY_ROOT}/src
    ${YETTY_ROOT}/include
)

target_link_libraries(yetty_telnet PUBLIC
    ytrace::ytrace
    yetty_base
)
