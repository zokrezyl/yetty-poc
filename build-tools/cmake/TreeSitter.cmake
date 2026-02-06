# Tree-sitter core library - download tarball instead of cloning
CPMAddPackage(
    NAME tree-sitter
    URL https://github.com/tree-sitter/tree-sitter/archive/refs/tags/v0.26.5.tar.gz
    DOWNLOAD_ONLY YES
)

if(tree-sitter_ADDED)
    add_library(tree-sitter-core STATIC
        ${tree-sitter_SOURCE_DIR}/lib/src/lib.c
    )
    target_include_directories(tree-sitter-core
        PUBLIC  ${tree-sitter_SOURCE_DIR}/lib/include
        PRIVATE ${tree-sitter_SOURCE_DIR}/lib/src
    )
    set_target_properties(tree-sitter-core PROPERTIES C_STANDARD 11)
endif()

# ─── Helper: add a tree-sitter grammar ──────────────────────────────────────
# Usage: add_ts_grammar(NAME c REPO tree-sitter/tree-sitter-c TAG v0.24.1)
#        add_ts_grammar(NAME cpp REPO tree-sitter/tree-sitter-cpp TAG v0.23.4 SCANNER)
#
# Downloads release tarballs instead of cloning git repos for faster builds.
function(add_ts_grammar)
    cmake_parse_arguments(G "SCANNER" "NAME;REPO;TAG;SUBDIR" "" ${ARGN})

    # Download tarball from GitHub releases instead of git clone
    CPMAddPackage(
        NAME tree-sitter-${G_NAME}
        URL https://github.com/${G_REPO}/archive/refs/tags/${G_TAG}.tar.gz
        DOWNLOAD_ONLY YES
    )

    if(G_SUBDIR)
        set(SRC_DIR ${tree-sitter-${G_NAME}_SOURCE_DIR}/${G_SUBDIR}/src)
    else()
        set(SRC_DIR ${tree-sitter-${G_NAME}_SOURCE_DIR}/src)
    endif()
    set(SRCS ${SRC_DIR}/parser.c)
    if(G_SCANNER)
        if(EXISTS ${SRC_DIR}/scanner.c)
            list(APPEND SRCS ${SRC_DIR}/scanner.c)
        elseif(EXISTS ${SRC_DIR}/scanner.cc)
            list(APPEND SRCS ${SRC_DIR}/scanner.cc)
        endif()
    endif()

    add_library(ts-grammar-${G_NAME} STATIC ${SRCS})
    target_include_directories(ts-grammar-${G_NAME} PRIVATE
        ${SRC_DIR}
        ${tree-sitter_SOURCE_DIR}/lib/include
    )

    # Export the queries directory path
    set(TS_QUERIES_DIR_${G_NAME} "${tree-sitter-${G_NAME}_SOURCE_DIR}/queries"
        CACHE PATH "Queries dir for tree-sitter-${G_NAME}" FORCE)
endfunction()

# ─── Grammars ───────────────────────────────────────────────────────────────
add_ts_grammar(NAME c          REPO tree-sitter/tree-sitter-c          TAG v0.24.1)
add_ts_grammar(NAME cpp        REPO tree-sitter/tree-sitter-cpp        TAG v0.23.4  SCANNER)
add_ts_grammar(NAME python     REPO tree-sitter/tree-sitter-python     TAG v0.25.0  SCANNER)
add_ts_grammar(NAME javascript REPO tree-sitter/tree-sitter-javascript TAG v0.25.0  SCANNER)
add_ts_grammar(NAME typescript REPO tree-sitter/tree-sitter-typescript TAG v0.23.2  SCANNER SUBDIR typescript)
add_ts_grammar(NAME rust       REPO tree-sitter/tree-sitter-rust       TAG v0.24.0  SCANNER)
add_ts_grammar(NAME go         REPO tree-sitter/tree-sitter-go         TAG v0.25.0)
add_ts_grammar(NAME java       REPO tree-sitter/tree-sitter-java       TAG v0.23.5)
add_ts_grammar(NAME bash       REPO tree-sitter/tree-sitter-bash       TAG v0.25.1  SCANNER)
add_ts_grammar(NAME json       REPO tree-sitter/tree-sitter-json       TAG v0.24.8)
add_ts_grammar(NAME yaml       REPO tree-sitter-grammars/tree-sitter-yaml TAG v0.7.2 SCANNER)
add_ts_grammar(NAME toml       REPO tree-sitter-grammars/tree-sitter-toml TAG v0.7.0 SCANNER)
