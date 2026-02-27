# ygrid business logic library

add_library(ygrid STATIC
    ygrid.cpp
)

target_include_directories(ygrid PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(ygrid PUBLIC
    yetty_base
)

set_target_properties(ygrid PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)
