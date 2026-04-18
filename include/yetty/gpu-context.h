#pragma once

#include <webgpu/webgpu.h>

namespace yetty {

// Low-level GPU context - pure WebGPU handles
// For components that only need GPU access without application-level dependencies
struct GPUContext {
    WGPUDevice device;
    WGPUQueue queue;
    WGPUTextureFormat surfaceFormat;
    WGPUBindGroupLayout sharedBindGroupLayout;
    WGPUBindGroup sharedBindGroup;
    WGPUBuffer sharedUniformBuffer;
    uint32_t sharedUniformSize;

    // Current render target dimensions (updated before each frame)
    // Used to clamp scissor rects during resize transitions
    uint32_t renderTargetWidth = 0;
    uint32_t renderTargetHeight = 0;
};

} // namespace yetty
