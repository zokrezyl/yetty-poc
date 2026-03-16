# YWasm card - WebAssembly runtime with ydraw rendering
add_library(yetty_card_ywasm STATIC ywasm.cpp)
target_include_directories(yetty_card_ywasm PUBLIC ${YETTY_ROOT}/include ${YETTY_ROOT}/src)
target_link_libraries(yetty_card_ywasm PUBLIC
    glm::glm
    webgpu
    yaml-cpp
    ytrace::ytrace
    wasm3_lib
    yetty_ydraw
)
target_compile_definitions(yetty_card_ywasm PRIVATE
    CMAKE_SOURCE_DIR="${YETTY_ROOT}"
)
