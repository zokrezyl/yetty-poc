# Python card — runs Python scripts that render to texture via CardBufferManager
# Gated on YETTY_PLUGIN_PYTHON; python_embedded is built by src/yetty/python/

if(NOT UNIX OR NOT YETTY_PLUGIN_PYTHON)
    message(STATUS "Python card: disabled (requires Unix and -DYETTY_PLUGIN_PYTHON=ON)")
    return()
endif()

# Build the generic Python embedding library (engine + yetty_card module)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/yetty/python ${CMAKE_BINARY_DIR}/yetty_python_build)

# Add card sources to yetty target
target_sources(yetty PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/python.h
    ${CMAKE_CURRENT_SOURCE_DIR}/python.cpp
)

# Link the Python embedding library
target_link_libraries(yetty PRIVATE yetty_python)

# Set compile definition so card-factory.cpp can conditionally register the card
target_compile_definitions(yetty PRIVATE YETTY_CARD_PYTHON=1)

message(STATUS "Python card: enabled")
