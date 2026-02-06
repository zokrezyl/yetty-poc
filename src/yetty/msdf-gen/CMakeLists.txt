add_executable(yetty-msdf-gen
    main.cpp
    generator.cpp
)

target_include_directories(yetty-msdf-gen PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${FREETYPE_INCLUDE_DIR}
)

target_link_libraries(yetty-msdf-gen PRIVATE
    msdfgen::msdfgen-core
    msdfgen::msdfgen-ext
    glm::glm
    ytrace::ytrace
    args
    cdb
    # FreeType static libs LAST — msdfgen references FT symbols
    ${FREETYPE_ALL_STATIC_LIBS}
)

target_compile_definitions(yetty-msdf-gen PRIVATE
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)
