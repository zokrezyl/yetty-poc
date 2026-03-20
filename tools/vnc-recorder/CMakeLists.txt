if(WIN32)
    add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
endif()

add_executable(vnc-recorder vnc-recorder.cpp)

target_link_libraries(vnc-recorder PRIVATE
    ytrace::ytrace
    args
    minimp4
    miniaudio
    fdk-aac
)

target_include_directories(vnc-recorder PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)
