set(TS_GRAMMAR_NAMES
    c cpp python javascript typescript rust go java bash json yaml toml
)

set(TS_GENERATED_HEADER "${CMAKE_CURRENT_BINARY_DIR}/ts_queries_generated.h")

# Query inheritance: child grammar → parent grammar
# These grammars only define additional patterns on top of their parent's query
set(TS_QUERY_PARENT_cpp "c")
set(TS_QUERY_PARENT_typescript "javascript")

# Generate embedded query header at configure time
set(_TS_HEADER "// Auto-generated — do not edit\n#pragma once\n\n")
foreach(NAME ${TS_GRAMMAR_NAMES})
    set(_QUERY_FILE "${TS_QUERIES_DIR_${NAME}}/highlights.scm")
    if(EXISTS "${_QUERY_FILE}")
        # If this grammar inherits from a parent, prepend parent's query
        set(_QUERY_CONTENT "")
        if(DEFINED TS_QUERY_PARENT_${NAME})
            set(_PARENT "${TS_QUERY_PARENT_${NAME}}")
            set(_PARENT_FILE "${TS_QUERIES_DIR_${_PARENT}}/highlights.scm")
            if(EXISTS "${_PARENT_FILE}")
                file(READ "${_PARENT_FILE}" _QUERY_CONTENT)
                string(APPEND _QUERY_CONTENT "\n")
            endif()
        endif()
        file(READ "${_QUERY_FILE}" _CHILD_CONTENT)
        string(APPEND _QUERY_CONTENT "${_CHILD_CONTENT}")
        # Escape backslashes, then double-quotes, then newlines
        string(REPLACE "\\" "\\\\" _QUERY_CONTENT "${_QUERY_CONTENT}")
        string(REPLACE "\"" "\\\"" _QUERY_CONTENT "${_QUERY_CONTENT}")
        string(REPLACE "\n" "\\n\"\n\"" _QUERY_CONTENT "${_QUERY_CONTENT}")
        string(APPEND _TS_HEADER "static const char TS_QUERY_${NAME}[] =\n\"${_QUERY_CONTENT}\";\n\n")
    else()
        string(APPEND _TS_HEADER "static const char TS_QUERY_${NAME}[] = \"\";\n\n")
    endif()
endforeach()
file(WRITE "${TS_GENERATED_HEADER}" "${_TS_HEADER}")

add_executable(ycat
    main.cpp
    osc.cpp
    ts/grammars.cpp
    ts/highlight.cpp
)

target_link_libraries(ycat PRIVATE
    args
    magic
    tree-sitter-core
    ts-grammar-c
    ts-grammar-cpp
    ts-grammar-python
    ts-grammar-javascript
    ts-grammar-typescript
    ts-grammar-rust
    ts-grammar-go
    ts-grammar-java
    ts-grammar-bash
    ts-grammar-json
    ts-grammar-yaml
    ts-grammar-toml
)

target_include_directories(ycat PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}   # for ts_queries_generated.h
)

target_compile_features(ycat PRIVATE cxx_std_23)

# Compiled magic database path (built by Libmagic.cmake)
target_compile_definitions(ycat PRIVATE
    YCAT_MAGIC_MGC_PATH="${LIBMAGIC_MGC_PATH}"
)
