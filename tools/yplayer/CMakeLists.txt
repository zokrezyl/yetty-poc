add_executable(yplayer main.cpp)

target_link_libraries(yplayer PRIVATE
    yetty_yvideo
    ytrace
    spdlog::spdlog
    args
)

target_include_directories(yplayer PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

# Link stb for image encoding
target_compile_definitions(yplayer PRIVATE STB_IMAGE_WRITE_IMPLEMENTATION)
