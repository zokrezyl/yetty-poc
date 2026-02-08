#include <yetty/gpu-allocator.h>
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <iomanip>

namespace yetty {

GpuAllocator::GpuAllocator(WGPUDevice device)
    : _device(device) {}

std::string GpuAllocator::labelToString(WGPUStringView label) {
    if (!label.data) return "(unnamed)";
    if (label.length == WGPU_STRLEN) return std::string(label.data);
    return std::string(label.data, label.length);
}

WGPUBuffer GpuAllocator::createBuffer(const WGPUBufferDescriptor& desc) {
    std::string name = labelToString(desc.label);

    WGPUBuffer buffer = wgpuDeviceCreateBuffer(_device, &desc);
    if (!buffer) {
        yerror("GpuAllocator: failed to create buffer '{}'", name);
        return nullptr;
    }

    uint64_t size = desc.size;
    _allocations.push_back({name, size, AllocType::Buffer, buffer});
    _totalBytes += size;

    yinfo("GPU [+] buffer '{}': {} bytes ({:.2f} KB) — total: {} bytes ({:.2f} MB)",
          name, size, size / 1024.0,
          _totalBytes, _totalBytes / (1024.0 * 1024.0));

    return buffer;
}

void GpuAllocator::releaseBuffer(WGPUBuffer buffer) {
    if (!buffer) return;

    auto it = std::find_if(_allocations.begin(), _allocations.end(),
        [buffer](const Allocation& a) {
            return a.type == AllocType::Buffer && a.handle == buffer;
        });

    if (it != _allocations.end()) {
        _totalBytes -= it->size;
        yinfo("GPU [-] buffer '{}': {} bytes ({:.2f} KB) — total: {} bytes ({:.2f} MB)",
              it->name, it->size, it->size / 1024.0,
              _totalBytes, _totalBytes / (1024.0 * 1024.0));
        _allocations.erase(it);
    } else {
        ywarn("GpuAllocator: releaseBuffer called for untracked buffer");
    }

    wgpuBufferRelease(buffer);
}

WGPUTexture GpuAllocator::createTexture(const WGPUTextureDescriptor& desc) {
    std::string name = labelToString(desc.label);

    WGPUTexture texture = wgpuDeviceCreateTexture(_device, &desc);
    if (!texture) {
        yerror("GpuAllocator: failed to create texture '{}'", name);
        return nullptr;
    }

    uint64_t size = textureBytes(desc);
    _allocations.push_back({name, size, AllocType::Texture, texture});
    _totalBytes += size;

    yinfo("GPU [+] texture '{}': {}x{}x{} = {} bytes ({:.2f} MB) — total: {} bytes ({:.2f} MB)",
          name,
          desc.size.width, desc.size.height, desc.size.depthOrArrayLayers,
          size, size / (1024.0 * 1024.0),
          _totalBytes, _totalBytes / (1024.0 * 1024.0));

    return texture;
}

void GpuAllocator::releaseTexture(WGPUTexture texture) {
    if (!texture) return;

    auto it = std::find_if(_allocations.begin(), _allocations.end(),
        [texture](const Allocation& a) {
            return a.type == AllocType::Texture && a.handle == texture;
        });

    if (it != _allocations.end()) {
        _totalBytes -= it->size;
        yinfo("GPU [-] texture '{}': {} bytes ({:.2f} MB) — total: {} bytes ({:.2f} MB)",
              it->name, it->size, it->size / (1024.0 * 1024.0),
              _totalBytes, _totalBytes / (1024.0 * 1024.0));
        _allocations.erase(it);
    } else {
        ywarn("GpuAllocator: releaseTexture called for untracked texture");
    }

    wgpuTextureRelease(texture);
}

uint64_t GpuAllocator::totalBufferBytes() const {
    uint64_t total = 0;
    for (const auto& a : _allocations) {
        if (a.type == AllocType::Buffer) total += a.size;
    }
    return total;
}

uint64_t GpuAllocator::totalTextureBytes() const {
    uint64_t total = 0;
    for (const auto& a : _allocations) {
        if (a.type == AllocType::Texture) total += a.size;
    }
    return total;
}

void GpuAllocator::dumpAllocations() const {
    yinfo("=== GPU Allocations ({} resources, {} bytes / {:.2f} MB) ===",
          _allocations.size(), _totalBytes, _totalBytes / (1024.0 * 1024.0));

    for (const auto& a : _allocations) {
        const char* type = (a.type == AllocType::Buffer) ? "buffer" : "texture";
        yinfo("  {:>8} {:>10} bytes ({:>8.2f} KB)  {}",
              type, a.size, a.size / 1024.0, a.name);
    }

    yinfo("  Buffers:  {} bytes ({:.2f} MB)", totalBufferBytes(), totalBufferBytes() / (1024.0 * 1024.0));
    yinfo("  Textures: {} bytes ({:.2f} MB)", totalTextureBytes(), totalTextureBytes() / (1024.0 * 1024.0));
    yinfo("  Total:    {} bytes ({:.2f} MB)", _totalBytes, _totalBytes / (1024.0 * 1024.0));
}

std::string GpuAllocator::dumpAllocationsToString() const {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "  " << _allocations.size() << " resources, "
       << _totalBytes << " bytes (" << (_totalBytes / (1024.0 * 1024.0)) << " MB)\n";

    for (const auto& a : _allocations) {
        const char* type = (a.type == AllocType::Buffer) ? "buf" : "tex";
        ss << "    " << type << "  "
           << std::setw(10) << a.size << " B  ("
           << std::setw(8) << (a.size / 1024.0) << " KB)  "
           << a.name << "\n";
    }

    ss << "  Buffers:  " << totalBufferBytes() << " B ("
       << (totalBufferBytes() / (1024.0 * 1024.0)) << " MB)\n";
    ss << "  Textures: " << totalTextureBytes() << " B ("
       << (totalTextureBytes() / (1024.0 * 1024.0)) << " MB)\n";

    return ss.str();
}

uint64_t GpuAllocator::textureBytes(const WGPUTextureDescriptor& desc) {
    uint32_t bpp = bytesPerPixel(desc.format);
    return static_cast<uint64_t>(desc.size.width)
         * static_cast<uint64_t>(desc.size.height)
         * static_cast<uint64_t>(desc.size.depthOrArrayLayers)
         * bpp;
}

uint32_t GpuAllocator::bytesPerPixel(WGPUTextureFormat format) {
    switch (format) {
        case WGPUTextureFormat_RGBA8Unorm:
        case WGPUTextureFormat_RGBA8UnormSrgb:
        case WGPUTextureFormat_BGRA8Unorm:
        case WGPUTextureFormat_BGRA8UnormSrgb:
        case WGPUTextureFormat_RGBA8Snorm:
        case WGPUTextureFormat_RGBA8Uint:
        case WGPUTextureFormat_RGBA8Sint:
            return 4;
        case WGPUTextureFormat_R8Unorm:
        case WGPUTextureFormat_R8Snorm:
        case WGPUTextureFormat_R8Uint:
        case WGPUTextureFormat_R8Sint:
            return 1;
        case WGPUTextureFormat_RG8Unorm:
        case WGPUTextureFormat_RG8Snorm:
        case WGPUTextureFormat_RG8Uint:
        case WGPUTextureFormat_RG8Sint:
            return 2;
        case WGPUTextureFormat_RGBA16Float:
        case WGPUTextureFormat_RGBA16Uint:
        case WGPUTextureFormat_RGBA16Sint:
            return 8;
        case WGPUTextureFormat_RGBA32Float:
        case WGPUTextureFormat_RGBA32Uint:
        case WGPUTextureFormat_RGBA32Sint:
            return 16;
        case WGPUTextureFormat_R16Float:
        case WGPUTextureFormat_R16Uint:
        case WGPUTextureFormat_R16Sint:
            return 2;
        case WGPUTextureFormat_R32Float:
        case WGPUTextureFormat_R32Uint:
        case WGPUTextureFormat_R32Sint:
            return 4;
        case WGPUTextureFormat_RG16Float:
        case WGPUTextureFormat_RG16Uint:
        case WGPUTextureFormat_RG16Sint:
            return 4;
        case WGPUTextureFormat_RG32Float:
        case WGPUTextureFormat_RG32Uint:
        case WGPUTextureFormat_RG32Sint:
            return 8;
        case WGPUTextureFormat_Depth24Plus:
        case WGPUTextureFormat_Depth32Float:
            return 4;
        case WGPUTextureFormat_Depth24PlusStencil8:
        case WGPUTextureFormat_Depth32FloatStencil8:
            return 5; // approximate
        default:
            ywarn("GpuAllocator: unknown texture format {}, assuming 4 bpp",
                  static_cast<int>(format));
            return 4;
    }
}

} // namespace yetty
