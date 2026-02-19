# ymery - YAML-driven ImGui widget system
add_library(yetty_ymery STATIC
    types.cpp
    data-bag.cpp
    data-tree.cpp
    dispatcher.cpp
    lang.cpp
    simple-data-tree.cpp
    renderer.cpp
)

target_include_directories(yetty_ymery PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(yetty_ymery PUBLIC yaml-cpp imgui ytrace::ytrace)
