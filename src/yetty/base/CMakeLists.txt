# yetty base utilities
# Event loop implementation is platform-specific
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

if(NOT EMSCRIPTEN)
    target_link_libraries(yetty_base PUBLIC uv_a)
endif()
