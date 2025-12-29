#pragma once

// Compatibility macros for wgpu-native v27+ API
// Provides helpers for the new WebGPU standard API

#include <webgpu/webgpu.h>

// Helper macro to create WGPUStringView from const char*
// In v27+, label fields are WGPUStringView instead of const char*
#define WGPU_STR(s) (WGPUStringView{ .data = (s), .length = WGPU_STRLEN })

// Null string view
#define WGPU_STR_NULL (WGPUStringView{ .data = nullptr, .length = 0 })
