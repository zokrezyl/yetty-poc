#include <yetty/card-texture-manager.h>
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cstring>
#include <vector>
#include <unordered_map>

// WebGPU string helper
#define WGPU_STR(s) WGPUStringView{.data = s, .length = WGPU_STRLEN}

namespace yetty {

// =============================================================================
// Texture handle tracking data
// =============================================================================

struct TextureHandleData {
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t atlasX = 0;
    uint32_t atlasY = 0;
    bool packed = false;
    bool dirty = false;           // pixels written since last upload
    const uint8_t* pixels = nullptr;  // last written pixel pointer (for upload)
};

// =============================================================================
// CardTextureManagerImpl
// =============================================================================

class CardTextureManagerImpl : public CardTextureManager {
public:
    CardTextureManagerImpl(GPUContext* gpuContext, Config config) noexcept;
    ~CardTextureManagerImpl() override;

    Result<void> init() noexcept;

    // Card API
    Result<TextureHandle> allocate(uint32_t width, uint32_t height) override;
    Result<void> deallocate(TextureHandle handle) override;
    Result<void> write(TextureHandle handle, const uint8_t* pixels) override;
    AtlasPosition getAtlasPosition(TextureHandle handle) const override;

    // gpu-screen / CardManager API
    Result<void> createAtlas() override;
    Result<void> uploadAtlas(WGPUQueue queue) override;

    // Accessors
    WGPUTexture atlasTexture() const override { return _atlasTexture; }
    WGPUTextureView atlasTextureView() const override { return _atlasTextureView; }
    WGPUSampler atlasSampler() const override { return _atlasSampler; }
    bool atlasInitialized() const override { return _atlasInitialized; }
    uint32_t atlasSize() const override { return _currentAtlasSize; }

    Stats getStats() const override;

private:
    Result<void> createAtlasTexture();
    Result<void> growAtlas();

    GPUContext* _gpuContext;
    WGPUDevice _device;
    Config _config;

    // Atlas GPU resources
    WGPUTexture _atlasTexture = nullptr;
    WGPUTextureView _atlasTextureView = nullptr;
    WGPUSampler _atlasSampler = nullptr;
    bool _atlasInitialized = false;
    uint32_t _currentAtlasSize;
    uint32_t _maxAtlasSize;

    // Texture handle tracking
    std::unordered_map<uint32_t, TextureHandleData> _textureHandles;
    uint32_t _nextTextureHandleId = 1;  // 0 = invalid
};

// =============================================================================
// Factory
// =============================================================================

Result<CardTextureManager::Ptr> CardTextureManager::create(GPUContext* gpuContext, Config config) noexcept {
    auto mgr = std::make_shared<CardTextureManagerImpl>(gpuContext, config);
    if (auto res = mgr->init(); !res) {
        return Err<Ptr>("Failed to initialize CardTextureManager", res);
    }
    return Ok(std::move(mgr));
}

// =============================================================================
// Implementation
// =============================================================================

CardTextureManagerImpl::CardTextureManagerImpl(GPUContext* gpuContext, Config config) noexcept
    : _gpuContext(gpuContext)
    , _device(gpuContext->device)
    , _config(config)
    , _currentAtlasSize(config.initialAtlasSize)
    , _maxAtlasSize(config.maxAtlasSize) {
}

Result<void> CardTextureManagerImpl::init() noexcept {
    // Atlas is lazily initialized on first createAtlas() call
    return Ok();
}

CardTextureManagerImpl::~CardTextureManagerImpl() {
    if (_atlasSampler) wgpuSamplerRelease(_atlasSampler);
    if (_atlasTextureView) wgpuTextureViewRelease(_atlasTextureView);
    if (_atlasTexture) wgpuTextureRelease(_atlasTexture);
}

// =============================================================================
// Card API
// =============================================================================

Result<TextureHandle> CardTextureManagerImpl::allocate(uint32_t width, uint32_t height) {
    if (width == 0 || height == 0) {
        return Err<TextureHandle>("allocate: width and height must be > 0");
    }

    uint32_t id = _nextTextureHandleId++;
    TextureHandleData data;
    data.width = width;
    data.height = height;
    _textureHandles[id] = data;

    ydebug("CardTextureManager: allocated handle id={} {}x{}", id, width, height);
    return Ok(TextureHandle{id});
}

Result<void> CardTextureManagerImpl::deallocate(TextureHandle handle) {
    auto it = _textureHandles.find(handle.id);
    if (it == _textureHandles.end()) {
        return Err("deallocate: invalid handle");
    }

    _textureHandles.erase(it);
    ydebug("CardTextureManager: deallocated handle id={}", handle.id);
    return Ok();
}

Result<void> CardTextureManagerImpl::write(TextureHandle handle, const uint8_t* pixels) {
    auto it = _textureHandles.find(handle.id);
    if (it == _textureHandles.end()) {
        return Err("write: invalid handle");
    }
    if (!pixels) {
        return Err("write: null pixels");
    }

    it->second.pixels = pixels;
    it->second.dirty = true;

    ydebug("CardTextureManager: write handle={} {}x{}", handle.id,
           it->second.width, it->second.height);
    return Ok();
}

AtlasPosition CardTextureManagerImpl::getAtlasPosition(TextureHandle handle) const {
    auto it = _textureHandles.find(handle.id);
    if (it == _textureHandles.end() || !it->second.packed) {
        return {0, 0};
    }
    return {it->second.atlasX, it->second.atlasY};
}

// =============================================================================
// gpu-screen / CardManager API
// =============================================================================

Result<void> CardTextureManagerImpl::createAtlas() {
    if (_textureHandles.empty()) {
        return Ok();
    }

    // Lazily initialize atlas GPU resources
    if (!_atlasInitialized) {
        if (auto res = createAtlasTexture(); !res) {
            return Err<void>("createAtlas: atlas texture init failed", res);
        }
        _atlasInitialized = true;
        yinfo("CardTextureManager: atlas initialized ({}x{})", _currentAtlasSize, _currentAtlasSize);
    }

    // Collect handles for packing
    struct PackEntry {
        uint32_t id;
        uint32_t width;
        uint32_t height;
    };
    std::vector<PackEntry> entries;
    entries.reserve(_textureHandles.size());

    for (const auto& [id, data] : _textureHandles) {
        if (data.width > 0 && data.height > 0) {
            entries.push_back({id, data.width, data.height});
        }
    }

    if (entries.empty()) {
        return Ok();
    }

    // Sort by height descending for better strip packing
    std::sort(entries.begin(), entries.end(), [](const PackEntry& a, const PackEntry& b) {
        return a.height > b.height;
    });

    // Row-based strip packing
    auto packWithSize = [&](uint32_t atlasSize) -> uint32_t {
        uint32_t currentX = 0;
        uint32_t currentY = 0;
        uint32_t rowHeight = 0;

        for (const auto& entry : entries) {
            if (currentX + entry.width > atlasSize) {
                currentY += rowHeight;
                currentX = 0;
                rowHeight = 0;
            }

            auto& data = _textureHandles[entry.id];
            data.atlasX = currentX;
            data.atlasY = currentY;
            data.packed = true;
            data.dirty = true;  // needs re-upload after repack

            currentX += entry.width;
            rowHeight = std::max(rowHeight, entry.height);
        }

        return currentY + rowHeight;
    };

    uint32_t totalHeight = packWithSize(_currentAtlasSize);

    // Grow atlas if needed
    while (totalHeight > _currentAtlasSize && _currentAtlasSize < _maxAtlasSize) {
        if (auto res = growAtlas(); !res) {
            return res;
        }
        totalHeight = packWithSize(_currentAtlasSize);
    }

    if (totalHeight > _currentAtlasSize) {
        yerror("CardTextureManager: atlas overflow! need {}px height, have {}px", totalHeight, _currentAtlasSize);
        return Err<void>("Atlas overflow - too many/large textures");
    }

    yinfo("CardTextureManager: packed {} textures into atlas (used {}px of {}px height)",
          entries.size(), totalHeight, _currentAtlasSize);

    return Ok();
}

Result<void> CardTextureManagerImpl::uploadAtlas(WGPUQueue queue) {
    if (!_atlasInitialized || !_atlasTexture) {
        return Ok();
    }

    for (auto& [id, data] : _textureHandles) {
        if (!data.dirty || !data.packed || !data.pixels) {
            continue;
        }

        WGPUTexelCopyTextureInfo destination = {};
        destination.texture = _atlasTexture;
        destination.mipLevel = 0;
        destination.origin = {data.atlasX, data.atlasY, 0};

        WGPUTexelCopyBufferLayout dataLayout = {};
        dataLayout.offset = 0;
        dataLayout.bytesPerRow = data.width * 4;  // RGBA8
        dataLayout.rowsPerImage = data.height;

        WGPUExtent3D writeSize = {data.width, data.height, 1};

        uint32_t totalBytes = data.width * data.height * 4;
        wgpuQueueWriteTexture(queue, &destination, data.pixels, totalBytes, &dataLayout, &writeSize);

        data.dirty = false;

        ydebug("CardTextureManager: uploaded {}x{} pixels to atlas at ({}, {})",
               data.width, data.height, data.atlasX, data.atlasY);
    }

    return Ok();
}

CardTextureManager::Stats CardTextureManagerImpl::getStats() const {
    uint32_t usedPixels = 0;
    for (const auto& [id, data] : _textureHandles) {
        if (data.packed) {
            usedPixels += data.width * data.height;
        }
    }

    return Stats{
        .textureCount = static_cast<uint32_t>(_textureHandles.size()),
        .atlasWidth = _currentAtlasSize,
        .atlasHeight = _currentAtlasSize,
        .usedPixels = usedPixels
    };
}

// =============================================================================
// Atlas texture creation
// =============================================================================

Result<void> CardTextureManagerImpl::createAtlasTexture() {
    // Release old resources if growing
    if (_atlasTextureView) {
        wgpuTextureViewRelease(_atlasTextureView);
        _atlasTextureView = nullptr;
    }
    if (_atlasTexture) {
        wgpuTextureDestroy(_atlasTexture);
        wgpuTextureRelease(_atlasTexture);
        _atlasTexture = nullptr;
    }
    if (_atlasSampler) {
        wgpuSamplerRelease(_atlasSampler);
        _atlasSampler = nullptr;
    }

    size_t atlasBytes = static_cast<size_t>(_currentAtlasSize) * _currentAtlasSize * 4;
    yinfo("GPU_ALLOC CardTextureManager: atlasTexture={}x{} RGBA8 = {} bytes ({:.2f} MB)",
          _currentAtlasSize, _currentAtlasSize, atlasBytes, atlasBytes / (1024.0 * 1024.0));

    WGPUTextureDescriptor texDesc = {};
    texDesc.label = WGPU_STR("ImageAtlas");
    texDesc.size.width = _currentAtlasSize;
    texDesc.size.height = _currentAtlasSize;
    texDesc.size.depthOrArrayLayers = 1;
    texDesc.mipLevelCount = 1;
    texDesc.sampleCount = 1;
    texDesc.dimension = WGPUTextureDimension_2D;
    texDesc.format = WGPUTextureFormat_RGBA8Unorm;
    texDesc.usage = WGPUTextureUsage_TextureBinding |
                    WGPUTextureUsage_CopyDst;

    _atlasTexture = wgpuDeviceCreateTexture(_device, &texDesc);
    if (!_atlasTexture) {
        return Err<void>("Failed to create atlas texture");
    }

    WGPUTextureViewDescriptor viewDesc = {};
    viewDesc.format = WGPUTextureFormat_RGBA8Unorm;
    viewDesc.dimension = WGPUTextureViewDimension_2D;
    viewDesc.mipLevelCount = 1;
    viewDesc.arrayLayerCount = 1;

    _atlasTextureView = wgpuTextureCreateView(_atlasTexture, &viewDesc);
    if (!_atlasTextureView) {
        return Err<void>("Failed to create atlas texture view");
    }

    WGPUSamplerDescriptor samplerDesc = {};
    samplerDesc.minFilter = WGPUFilterMode_Linear;
    samplerDesc.magFilter = WGPUFilterMode_Linear;
    samplerDesc.addressModeU = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeV = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeW = WGPUAddressMode_ClampToEdge;
    samplerDesc.maxAnisotropy = 1;

    _atlasSampler = wgpuDeviceCreateSampler(_device, &samplerDesc);
    if (!_atlasSampler) {
        return Err<void>("Failed to create atlas sampler");
    }

    return Ok();
}

Result<void> CardTextureManagerImpl::growAtlas() {
    uint32_t newSize = _currentAtlasSize * 2;
    if (newSize > _maxAtlasSize) {
        yerror("CardTextureManager: cannot grow atlas beyond {} (current: {})", _maxAtlasSize, _currentAtlasSize);
        return Err<void>("Atlas size limit reached");
    }

    yinfo("CardTextureManager: growing atlas from {}x{} to {}x{}",
          _currentAtlasSize, _currentAtlasSize, newSize, newSize);

    _currentAtlasSize = newSize;

    if (auto res = createAtlasTexture(); !res) {
        return res;
    }

    yinfo("CardTextureManager: atlas grown successfully to {}x{}", _currentAtlasSize, _currentAtlasSize);
    return Ok();
}

}  // namespace yetty
