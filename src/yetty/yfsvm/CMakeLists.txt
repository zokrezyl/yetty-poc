# yfsvm library - Yetty Fragment Shader Virtual Machine
add_library(yetty_yfsvm STATIC
    yfsvm.cpp
    yfsvm-compiler.cpp
    yfsvm-wgsl-gen.cpp
)

target_include_directories(yetty_yfsvm PUBLIC
    ${YETTY_ROOT}/include
    ${YETTY_ROOT}/src
)

target_link_libraries(yetty_yfsvm PUBLIC
    yetty_yast
)

# WGSL generator tool - generates yfsvm.wgsl from opcode definitions
if(NOT CMAKE_CROSSCOMPILING)
    add_executable(yfsvm_wgsl_gen
        yfsvm-wgsl-gen-tool.cpp
        yfsvm-wgsl-gen.cpp
    )
    target_include_directories(yfsvm_wgsl_gen PRIVATE
        ${YETTY_ROOT}/include
        ${YETTY_ROOT}/src
    )

    # Generate yfsvm.wgsl at build time
    set(YFSVM_WGSL_OUTPUT "${YETTY_ROOT}/src/yetty/shaders/lib/yfsvm.wgsl")
    add_custom_command(
        OUTPUT ${YFSVM_WGSL_OUTPUT}
        COMMAND yfsvm_wgsl_gen ${YFSVM_WGSL_OUTPUT}
        DEPENDS yfsvm_wgsl_gen
        COMMENT "Generating yfsvm.wgsl from opcode definitions"
        VERBATIM
    )
    add_custom_target(generate_yfsvm_wgsl ALL DEPENDS ${YFSVM_WGSL_OUTPUT})

    # Make sure shaders are copied after WGSL is generated
    if(TARGET copy-shaders)
        add_dependencies(copy-shaders generate_yfsvm_wgsl)
    endif()
endif()
