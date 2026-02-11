# Diagram module - Mermaid/Graphviz diagram support

target_sources(yetty PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/graph-ir.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mermaid-parser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mermaid-parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/layout-engine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sugiyama-layout.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sugiyama-layout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/diagram-renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/diagram-renderer.cpp
)
