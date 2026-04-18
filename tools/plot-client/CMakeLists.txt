# plot-client demo - Streams data to a yetty plot card via shared memory

add_executable(plot-client main.cpp)

target_include_directories(plot-client PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Link shared-memory.cpp
target_sources(plot-client PRIVATE
    ${CMAKE_SOURCE_DIR}/src/yetty/shared-memory.cpp
)

# Link dependencies
target_link_libraries(plot-client PRIVATE
    msgpack-cxx
    ytrace::ytrace
    # POSIX shared memory (Linux only - macOS has shm_open in libc)
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<NOT:$<BOOL:${APPLE}>>>:rt>
)
