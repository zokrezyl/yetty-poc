# diagram library - Mermaid/Graphviz diagram support
add_library(yetty_diagram STATIC
    mermaid-parser.cpp
    sugiyama-layout.cpp
    diagram-renderer.cpp
)

target_include_directories(yetty_diagram PUBLIC
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
)

target_link_libraries(yetty_diagram PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace)
