# yetty base utilities
# Event loop implementation is platform-specific
# - Desktop: libuv with uv_run(UV_RUN_DEFAULT)
# - Android: ALooper + timerfd (libuv only for EventQueue cross-thread wakeup)
# - WebASM: browser requestAnimationFrame
if(EMSCRIPTEN)
    set(EVENT_LOOP_SRC ${YETTY_ROOT}/src/yetty/platform/event-loop/webasm.cpp)
elseif(YETTY_ANDROID)
    set(EVENT_LOOP_SRC ${YETTY_ROOT}/src/yetty/platform/event-loop/android.cpp)
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
