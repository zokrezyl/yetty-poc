# GPU monitoring subsystem
# Provides unified interface for querying GPU utilization across vendors
#
# NVML: We define minimal types inline rather than downloading headers.
#       The library is loaded dynamically at runtime via dlopen.
# AMD/Intel: Use sysfs, no external dependencies.

set(GPU_SOURCES gpu-monitor.cpp)

if(NOT WIN32)
    list(APPEND GPU_SOURCES
        nvidia-monitor.cpp
        amd-monitor.cpp
        intel-monitor.cpp
    )
endif()

add_library(yetty_gpu STATIC ${GPU_SOURCES})

target_include_directories(yetty_gpu PUBLIC
    ${YETTY_ROOT}/include           # For include/yetty/gpu-monitor.h
    ${CMAKE_CURRENT_SOURCE_DIR}           # For internal headers
)

target_link_libraries(yetty_gpu PRIVATE
    ytrace::ytrace
    ${CMAKE_DL_LIBS}  # For dlopen on Linux (NVML dynamic loading)
)

set_target_properties(yetty_gpu PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)
