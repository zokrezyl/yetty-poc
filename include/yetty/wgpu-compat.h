#pragma once

// Compatibility macros for WebGPU API differences between backends.
// With emdawnwebgpu, Emscripten now uses Dawn's API (same as desktop).

#include <webgpu/webgpu.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
// Emscripten: yield to JS event loop instead of device tick
#define WGPU_DEVICE_TICK(device) emscripten_sleep(1)
#else
// Desktop: use wgpuDeviceTick to process GPU work
#define WGPU_DEVICE_TICK(device) wgpuDeviceTick(device)
#endif

// All platforms now use Dawn's new API with WGPUStringView
#define WGPU_STR(s) (WGPUStringView{.data = (s), .length = WGPU_STRLEN})
#define WGPU_STR_NULL (WGPUStringView{.data = nullptr, .length = 0})
#define WGPU_USE_OLD_API 0

// Shader code: WGPUStringView
#define WGPU_SHADER_CODE(desc, src)                                            \
  (desc).code = {.data = (src).c_str(), .length = (src).size()}

// Render pass color attachment uses clearValue
#define WGPU_COLOR_ATTACHMENT_CLEAR(attachment, r, g, b, a)                    \
  (attachment).clearValue = {(r), (g), (b), (a)}

// Mipmap filter
#define WGPU_MIPMAP_FILTER_LINEAR WGPUMipmapFilterMode_Linear
#define WGPU_MIPMAP_FILTER_NEAREST WGPUMipmapFilterMode_Nearest
