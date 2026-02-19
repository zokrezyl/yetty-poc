# CDB - Constant Database
# Two implementations: howerj/cdb (portable) or djb/cdb (Unix, faster)
if(TARGET cdb)
    return()
endif()

# Default to portable on Windows/Android/Emscripten, djb on Unix desktop
if(WIN32 OR YETTY_ANDROID OR EMSCRIPTEN)
    option(YETTY_USE_HOWERJ_CDB "Use portable howerj/cdb" ON)
else()
    option(YETTY_USE_HOWERJ_CDB "Use portable howerj/cdb" OFF)
endif()

if(YETTY_USE_HOWERJ_CDB)
    CPMAddPackage(
        NAME howerj_cdb
        URL https://github.com/howerj/cdb/archive/refs/tags/v4.3.2.tar.gz
        VERSION 4.3.2
        DOWNLOAD_ONLY YES
    )
    if(howerj_cdb_ADDED)
        add_library(cdb STATIC ${howerj_cdb_SOURCE_DIR}/cdb.c)
        target_include_directories(cdb PUBLIC ${howerj_cdb_SOURCE_DIR})
        target_compile_definitions(cdb PUBLIC YETTY_USE_HOWERJ_CDB=1)
        if(MSVC)
            target_compile_options(cdb PRIVATE /w)
        else()
            target_compile_options(cdb PRIVATE -w)
        endif()
        set_target_properties(cdb PROPERTIES POSITION_INDEPENDENT_CODE ON)
        message(STATUS "CDB: Using portable howerj/cdb v4.3.2")
    endif()
elseif(NOT WIN32)
    set(CDB_VERSION "20251021")
    set(CDB_URL "https://cdb.cr.yp.to/cdb-${CDB_VERSION}.tar.gz")
    set(CDB_SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/cdb-${CDB_VERSION}")

    if(NOT EXISTS "${CDB_SOURCE_DIR}/cdb.c")
        message(STATUS "Downloading CDB ${CDB_VERSION}...")
        file(DOWNLOAD ${CDB_URL} "${CMAKE_BINARY_DIR}/_deps/cdb-${CDB_VERSION}.tar.gz"
             STATUS CDB_DOWNLOAD_STATUS)
        list(GET CDB_DOWNLOAD_STATUS 0 CDB_DOWNLOAD_ERROR)
        if(CDB_DOWNLOAD_ERROR)
            message(FATAL_ERROR "Failed to download CDB: ${CDB_DOWNLOAD_STATUS}")
        endif()
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar xzf "${CMAKE_BINARY_DIR}/_deps/cdb-${CDB_VERSION}.tar.gz"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/_deps"
        )
    endif()

    add_library(cdb STATIC
        ${CDB_SOURCE_DIR}/cdb.c
        ${CDB_SOURCE_DIR}/cdb_hash.c
        ${CDB_SOURCE_DIR}/cdb_make.c
        ${CDB_SOURCE_DIR}/byte_copy.c
        ${CDB_SOURCE_DIR}/byte_diff.c
        ${CDB_SOURCE_DIR}/num_from4bytes.c
        ${CDB_SOURCE_DIR}/num_to4bytes.c
        ${CDB_SOURCE_DIR}/outbuf_init.c
        ${CDB_SOURCE_DIR}/outbuf_put.c
        ${CDB_SOURCE_DIR}/outbuf_write.c
        ${CDB_SOURCE_DIR}/inbuf_0.c
        ${CDB_SOURCE_DIR}/inbuf_init.c
        ${CDB_SOURCE_DIR}/inbuf_get.c
        ${CDB_SOURCE_DIR}/inbuf_read.c
        ${CDB_SOURCE_DIR}/seek_set.c
        ${CDB_SOURCE_DIR}/seek_cur.c
    )
    file(REMOVE ${CDB_SOURCE_DIR}/version)
    target_include_directories(cdb SYSTEM PUBLIC ${CDB_SOURCE_DIR})
    target_compile_options(cdb PRIVATE -w)
    set_target_properties(cdb PROPERTIES POSITION_INDEPENDENT_CODE ON)
    message(STATUS "CDB: Using original djb cdb (Unix only)")
else()
    message(FATAL_ERROR "Windows requires YETTY_USE_HOWERJ_CDB=ON")
endif()

# CDB wrapper library
if(NOT TARGET cdb-wrapper)
    add_library(cdb-wrapper STATIC ${CMAKE_SOURCE_DIR}/src/yetty/cdb-wrapper.cpp)
    target_include_directories(cdb-wrapper PUBLIC ${CMAKE_SOURCE_DIR}/include)
    if(EMSCRIPTEN)
        target_link_libraries(cdb-wrapper PUBLIC cdb)
        target_include_directories(cdb-wrapper PRIVATE ${ytrace_SOURCE_DIR}/include ${spdlog_SOURCE_DIR}/include)
        target_compile_definitions(cdb-wrapper PRIVATE YTRACE_ENABLED=1 YTRACE_USE_SPDLOG=1 YTRACE_NO_CONTROL_SOCKET=1)
    else()
        target_link_libraries(cdb-wrapper PUBLIC cdb ytrace::ytrace)
    endif()
    if(YETTY_USE_HOWERJ_CDB)
        target_compile_definitions(cdb-wrapper PUBLIC YETTY_USE_HOWERJ_CDB=1)
    endif()
    set_target_properties(cdb-wrapper PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
