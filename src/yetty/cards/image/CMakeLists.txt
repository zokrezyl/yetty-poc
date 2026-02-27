# Image card
add_library(yetty_card_image STATIC image.cpp)
target_include_directories(yetty_card_image PUBLIC ${YETTY_ROOT}/include ${YETTY_ROOT}/src)
target_link_libraries(yetty_card_image PUBLIC glm::glm webgpu yaml-cpp ytrace::ytrace stb)
target_compile_definitions(yetty_card_image PRIVATE
    CMAKE_SOURCE_DIR="${YETTY_ROOT}"
    YETTY_SHADERS_DIR="${CMAKE_BINARY_DIR}/assets/shaders"
)
