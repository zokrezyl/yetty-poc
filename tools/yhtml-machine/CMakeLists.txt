add_executable(yhtml-machine
    main.cpp
    ${CMAKE_SOURCE_DIR}/src/yetty/yhtml/http-fetcher.cpp
)

target_include_directories(yhtml-machine PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/yetty/yhtml
)

target_link_libraries(yhtml-machine PRIVATE cpr::cpr args ytrace::ytrace)
target_compile_features(yhtml-machine PRIVATE cxx_std_20)
