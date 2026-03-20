# YRich library - document-centric abstraction for WYSIWYG content
# Supports collaborative editing via operation-based sync
add_library(yetty_yrich STATIC
    yrich-document.cpp
    yrich-operation.cpp
    yrich-sync.cpp
    yspreadsheet.cpp
    ydoc.cpp
)

target_include_directories(yetty_yrich PUBLIC
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
)

target_link_libraries(yetty_yrich PUBLIC
    yetty_ydraw
    msgpack-cxx
    uv_a
)
