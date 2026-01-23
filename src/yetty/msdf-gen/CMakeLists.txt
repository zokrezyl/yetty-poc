add_executable(yetty-msdf-gen
    main.cpp
    generator.cpp
)

target_include_directories(yetty-msdf-gen PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(yetty-msdf-gen PRIVATE
    msdfgen::msdfgen-core
    msdfgen::msdfgen-ext
    freetype
    glm::glm
    ytrace::ytrace
    args
    cdb
)

target_compile_definitions(yetty-msdf-gen PRIVATE
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)
