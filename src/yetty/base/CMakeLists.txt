# yetty base utilities
# Event loop runs on render thread:
# - Desktop + Android: libuv (uv_run)
# - WebASM: browser requestAnimationFrame (single-threaded)
if(EMSCRIPTEN)
    set(EVENT_LOOP_SRC ${YETTY_ROOT}/src/yetty/platform/event-loop/webasm.cpp)
else()
    set(EVENT_LOOP_SRC ${YETTY_ROOT}/src/yetty/platform/event-loop/libuv.cpp)
endif()

add_library(yetty_base STATIC
    event-queue.cpp
    ${EVENT_LOOP_SRC}
)

target_include_directories(yetty_base PUBLIC
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
)

target_link_libraries(yetty_base PUBLIC ytrace::ytrace yaml-cpp)

# libuv for desktop and Android (not WebASM)
if(NOT EMSCRIPTEN)
    target_link_libraries(yetty_base PUBLIC uv_a)
endif()
