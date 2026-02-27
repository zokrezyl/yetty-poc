# RPC server sources (used by yetty executable)
set(YETTY_RPC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/rpc-server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/event-loop-handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stream-handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/socket-path.cpp
    PARENT_SCOPE
)

# libyetty-rpc: standalone client library for connecting to the yetty RPC server
add_library(yetty-rpc STATIC
    rpc-client.cpp
)

target_link_libraries(yetty-rpc PUBLIC
    msgpack-cxx
    uv_a
)

target_include_directories(yetty-rpc PUBLIC
    ${YETTY_ROOT}/include
)

target_compile_features(yetty-rpc PUBLIC cxx_std_23)
