# HTTP fetching via cpr with OpenSSL
# Disable optional curl dependencies
# Save ZLIB state - we need to hide it from curl but restore for other libs (libpng)
set(_SAVED_ZLIB_FOUND ${ZLIB_FOUND})
set(_SAVED_CMAKE_DISABLE_FIND_PACKAGE_ZLIB ${CMAKE_DISABLE_FIND_PACKAGE_ZLIB})

set(CURL_ZLIB OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)
set(CURL_BROTLI OFF CACHE BOOL "" FORCE)
set(CURL_ZSTD OFF CACHE BOOL "" FORCE)
set(ZLIB_FOUND FALSE CACHE BOOL "" FORCE)
set(CMAKE_DISABLE_FIND_PACKAGE_ZLIB TRUE CACHE BOOL "" FORCE)

# SSL backend: OpenSSL on Unix, WinSSL (Schannel) on Windows
if(WIN32)
    set(_CPR_SSL_OPTIONS
        "CPR_FORCE_WINSSL_BACKEND ON"
    )
else()
    include(${YETTY_ROOT}/build-tools/cmake/openssl.cmake)
    set(_CPR_SSL_OPTIONS
        "CPR_FORCE_OPENSSL_BACKEND ON"
    )
endif()

CPMAddPackage(
    NAME cpr
    GITHUB_REPOSITORY libcpr/cpr
    GIT_TAG 1.14.1
    OPTIONS
        "CPR_USE_SYSTEM_CURL OFF"
        "BUILD_SHARED_LIBS OFF"
        "CPR_ENABLE_SSL ON"
        ${_CPR_SSL_OPTIONS}
        "CPR_BUILD_TESTS OFF"
        "CPR_BUILD_TESTS_SSL OFF"
        "CPR_CURL_USE_LIBPSL OFF"
        "CURL_ZLIB OFF"
)

# Restore ZLIB state for other libraries (like libpng)
set(ZLIB_FOUND ${_SAVED_ZLIB_FOUND} CACHE BOOL "" FORCE)
if(_SAVED_CMAKE_DISABLE_FIND_PACKAGE_ZLIB)
    set(CMAKE_DISABLE_FIND_PACKAGE_ZLIB ${_SAVED_CMAKE_DISABLE_FIND_PACKAGE_ZLIB} CACHE BOOL "" FORCE)
else()
    unset(CMAKE_DISABLE_FIND_PACKAGE_ZLIB CACHE)
endif()

set(TS_GRAMMAR_NAMES
    c cpp python javascript typescript rust go java bash json yaml toml html xml
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

# libmagic is available on Unix (Linux/macOS), not Windows or Android
if(NOT WIN32 AND NOT ANDROID)
    set(YCAT_MAGIC_LIBS magic)
    set(YCAT_HAS_LIBMAGIC TRUE)
else()
    set(YCAT_MAGIC_LIBS "")
    set(YCAT_HAS_LIBMAGIC FALSE)
endif()

target_link_libraries(ycat PRIVATE
    args
    ${YCAT_MAGIC_LIBS}
    cpr::cpr
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
    ts-grammar-html
    ts-grammar-xml
)

target_include_directories(ycat PRIVATE
    ${YETTY_ROOT}/include
    ${CMAKE_CURRENT_BINARY_DIR}   # for ts_queries_generated.h
)

target_compile_features(ycat PRIVATE cxx_std_23)

# libmagic support - Unix only (Linux/macOS)
if(YCAT_HAS_LIBMAGIC)
    target_compile_definitions(ycat PRIVATE
        YCAT_HAS_LIBMAGIC=1
        YCAT_MAGIC_MGC_PATH="${LIBMAGIC_MGC_PATH}"
    )
endif()

# Install ycat to Android jniLibs as libycat.so
if(YETTY_ANDROID AND ANDROID_BUILD_DIR)
    add_custom_command(TARGET ycat POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ycat>
                "${ANDROID_BUILD_DIR}/jniLibs/${ANDROID_ABI}/libycat.so"
        COMMENT "Copying ycat to Android jniLibs"
    )
endif()
