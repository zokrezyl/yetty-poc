add_executable(pdf2ydraw
    main.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ypdf/pdf-content-parser.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ypdf/pdf-renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/ydraw/ydraw-writer-binary.cpp
)

target_include_directories(pdf2ydraw PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/yetty/ypdf
)

target_link_libraries(pdf2ydraw PRIVATE pdfio_lib args ytrace::ytrace)
target_compile_features(pdf2ydraw PRIVATE cxx_std_20)
