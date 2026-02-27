# JSLinux integration for WebAssembly builds
# Downloads and copies JSLinux files for iframe-based emulator

set(JSLINUX_DIR "${CMAKE_CURRENT_BINARY_DIR}/jslinux")
set(JSLINUX_URL "https://bellard.org/jslinux")

# List of files to download (excluding config - we create our own)
set(JSLINUX_FILES
    x86_64emu-wasm.js
    x86_64emu-wasm.wasm
    kernel-x86_64.bin
    jslinux.js
    term.js
)

# Download JSLinux files during configure
file(MAKE_DIRECTORY ${JSLINUX_DIR})

foreach(FILE ${JSLINUX_FILES})
    set(LOCAL_FILE "${JSLINUX_DIR}/${FILE}")
    if(NOT EXISTS "${LOCAL_FILE}")
        message(STATUS "Downloading ${FILE}...")
        file(DOWNLOAD "${JSLINUX_URL}/${FILE}" "${LOCAL_FILE}"
             STATUS DOWNLOAD_STATUS
             TIMEOUT 60)
        list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
        if(NOT STATUS_CODE EQUAL 0)
            message(WARNING "Failed to download ${FILE}")
        endif()
    endif()
endforeach()

# Create config file using local yetty-alpine filesystem (built by build-vfsync.sh)
set(CONFIG_FILE "${JSLINUX_DIR}/alpine-x86_64.cfg")
file(WRITE ${CONFIG_FILE} "/* VM configuration file - uses local vfsync filesystem */
{
    version: 1,
    machine: \"pc\",
    memory_size: 256,
    kernel: \"kernel-x86_64.bin\",
    cmdline: \"loglevel=3 console=hvc0 root=root rootfstype=9p rootflags=trans=virtio ro TZ=\${TZ}\",
    fs0: { file: \"../vfsync/u/os/yetty-alpine\" },
    eth0: { driver: \"user\" },
}
")

# Copy our bridge files
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/term-bridge.js
    ${JSLINUX_DIR}/term-bridge.js
    COPYONLY
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/vm-bridge.html
    ${JSLINUX_DIR}/vm-bridge.html
    COPYONLY
)

# Create custom target for jslinux assets
add_custom_target(jslinux-assets
    DEPENDS
        ${JSLINUX_DIR}/x86_64emu-wasm.js
        ${JSLINUX_DIR}/x86_64emu-wasm.wasm
        ${JSLINUX_DIR}/kernel-x86_64.bin
        ${JSLINUX_DIR}/alpine-x86_64.cfg
        ${JSLINUX_DIR}/term-bridge.js
        ${JSLINUX_DIR}/vm-bridge.html
    COMMENT "JSLinux assets ready"
)
