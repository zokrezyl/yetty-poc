# MSDF generator tool - works on all platforms via CdbWriter wrapper
add_executable(yetty-msdf-gen
    main.cpp
    generator.cpp
)

target_include_directories(yetty-msdf-gen PRIVATE
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
    ${FREETYPE_INCLUDE_DIR}
)

target_link_libraries(yetty-msdf-gen PRIVATE
    msdfgen::msdfgen-core
    msdfgen::msdfgen-ext
    glm::glm
    ytrace::ytrace
    args
    cdb-wrapper
    # FreeType static libs LAST — msdfgen references FT symbols
    ${FREETYPE_ALL_LIBS}
)

target_compile_definitions(yetty-msdf-gen PRIVATE
    CMAKE_SOURCE_DIR="${YETTY_ROOT}"
    $<$<BOOL:${WIN32}>:NOMINMAX>
)
