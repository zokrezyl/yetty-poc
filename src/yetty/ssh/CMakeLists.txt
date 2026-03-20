# SSH client for secure shell connections via libssh2
# Provides SSH PTY reader for remote shell access

add_library(yetty_ssh STATIC
    ssh-client.cpp
    ssh-pty-reader.cpp
)

target_include_directories(yetty_ssh PUBLIC
    ${YETTY_ROOT}/src
    ${YETTY_ROOT}/include
    ${LIBSSH2_INCLUDE_DIR}
)

target_link_libraries(yetty_ssh PUBLIC
    ytrace::ytrace
    yetty_base
    libssh2_static
    uv_a
)
