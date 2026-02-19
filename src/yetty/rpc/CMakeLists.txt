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
