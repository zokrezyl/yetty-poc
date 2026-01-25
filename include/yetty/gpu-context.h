#pragma once

#include <webgpu/webgpu.h>

namespace yetty {

// Immutable GPU context - all handles stable for app lifetime
// Created once by NewYetty, passed to all renderers
struct GPUContext {
    WGPUDevice device;
    WGPUQueue queue;
    WGPUTextureFormat surfaceFormat;
    WGPUBindGroupLayout sharedBindGroupLayout;
    WGPUBindGroup sharedBindGroup;
};

} // namespace yetty
