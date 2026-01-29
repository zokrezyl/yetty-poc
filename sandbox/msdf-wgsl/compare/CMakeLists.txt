add_executable(msdf-compare-cdb main.cpp)

target_link_libraries(msdf-compare-cdb PRIVATE
    cdb
)

target_include_directories(msdf-compare-cdb PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)
