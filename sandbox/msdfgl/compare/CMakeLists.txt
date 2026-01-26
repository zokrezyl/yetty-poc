cmake_minimum_required(VERSION 3.16)

add_executable(msdfgl-compare
    main.cpp
)

target_include_directories(msdfgl-compare PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)

target_link_libraries(msdfgl-compare
    PRIVATE
        cdb
)
