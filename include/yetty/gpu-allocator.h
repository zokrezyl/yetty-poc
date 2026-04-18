#pragma once

#include <yetty/wgpu-compat.h>
#include <webgpu/webgpu.h>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace yetty {

class GpuAllocator {
public:
    using Ptr = std::shared_ptr<GpuAllocator>;

    explicit GpuAllocator(WGPUDevice device);
    ~GpuAllocator() = default;

    // Buffer allocation — name extracted from desc.label
    WGPUBuffer createBuffer(const WGPUBufferDescriptor& desc);
    void releaseBuffer(WGPUBuffer buffer);

    // Texture allocation — name extracted from desc.label
    WGPUTexture createTexture(const WGPUTextureDescriptor& desc);
    void releaseTexture(WGPUTexture texture);

    // Query
    uint64_t totalAllocatedBytes() const { return _totalBytes; }
    uint64_t totalBufferBytes() const;
    uint64_t totalTextureBytes() const;
    uint32_t allocationCount() const { return static_cast<uint32_t>(_allocations.size()); }

    // Log all live allocations with names and sizes
    void dumpAllocations() const;

    // Return all live allocations as formatted text
    std::string dumpAllocationsToString() const;

private:
    enum class AllocType { Buffer, Texture };

    struct Allocation {
        std::string name;
        uint64_t size;
        AllocType type;
        void* handle;  // WGPUBuffer or WGPUTexture (opaque pointer for lookup)
    };

    static std::string labelToString(WGPUStringView label);
    static uint64_t textureBytes(const WGPUTextureDescriptor& desc);
    static uint32_t bytesPerPixel(WGPUTextureFormat format);

    WGPUDevice _device;
    std::vector<Allocation> _allocations;
    uint64_t _totalBytes = 0;
};

} // namespace yetty
