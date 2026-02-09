#include <yetty/card-texture-manager.h>
#include <yetty/gpu-allocator.h>
#include <yetty/wgpu-compat.h>
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cstring>
#include <vector>
#include <unordered_map>

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
    CardTextureManagerImpl(GPUContext* gpuContext, GpuAllocator::Ptr allocator, Config config) noexcept;
    ~CardTextureManagerImpl() override;

    Result<void> init() noexcept;

    // Card API
    Result<TextureHandle> allocate(uint32_t width, uint32_t height) override;
    Result<void> write(TextureHandle handle, const uint8_t* pixels) override;
    AtlasPosition getAtlasPosition(TextureHandle handle) const override;

    // gpu-screen / CardManager API
    void clearHandles() override;
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

    GPUContext* _gpuContext;
    WGPUDevice _device;
    GpuAllocator::Ptr _allocator;
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

Result<CardTextureManager::Ptr> CardTextureManager::create(GPUContext* gpuContext,
                                                              GpuAllocator::Ptr allocator,
                                                              Config config) noexcept {
    auto mgr = std::make_shared<CardTextureManagerImpl>(gpuContext, std::move(allocator), config);
    if (auto res = mgr->init(); !res) {
        return Err<Ptr>("Failed to initialize CardTextureManager", res);
    }
    return Ok(std::move(mgr));
}

// =============================================================================
// Implementation
// =============================================================================

CardTextureManagerImpl::CardTextureManagerImpl(GPUContext* gpuContext, GpuAllocator::Ptr allocator,
                                                 Config config) noexcept
    : _gpuContext(gpuContext)
    , _device(gpuContext->device)
    , _allocator(std::move(allocator))
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
    if (_atlasTexture) _allocator->releaseTexture(_atlasTexture);
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

Result<void> CardTextureManagerImpl::write(TextureHandle handle, const uint8_t* pixels) {
    if (!_atlasInitialized) {
        return Err("write: atlas not created yet (call createAtlas first)");
    }
    auto it = _textureHandles.find(handle.id);
    if (it == _textureHandles.end()) {
        return Err("write: invalid handle");
    }
    if (!it->second.packed) {
        return Err("write: handle not packed (call createAtlas first)");
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

void CardTextureManagerImpl::clearHandles() {
    _textureHandles.clear();
    _nextTextureHandleId = 1;
}

Result<void> CardTextureManagerImpl::createAtlas() {
    // No textures — release atlas GPU resources and reset size
    if (_textureHandles.empty()) {
        if (_atlasInitialized) {
            yinfo("CardTextureManager: no textures, releasing atlas (was {}x{})",
                  _currentAtlasSize, _currentAtlasSize);
            if (_atlasSampler) { wgpuSamplerRelease(_atlasSampler); _atlasSampler = nullptr; }
            if (_atlasTextureView) { wgpuTextureViewRelease(_atlasTextureView); _atlasTextureView = nullptr; }
            if (_atlasTexture) { _allocator->releaseTexture(_atlasTexture); _atlasTexture = nullptr; }
            _atlasInitialized = false;
            _currentAtlasSize = _config.initialAtlasSize;
        }
        return Ok();
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

    // Row-based strip packing — assigns atlas positions to each handle
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
            data.dirty = true;

            currentX += entry.width;
            rowHeight = std::max(rowHeight, entry.height);
        }

        return currentY + rowHeight;
    };

    // Find smallest power-of-2 atlas size that fits
    uint32_t maxWidth = 0;
    for (const auto& entry : entries) {
        maxWidth = std::max(maxWidth, entry.width);
    }
    uint32_t neededSize = std::max(_config.initialAtlasSize, maxWidth);
    while (neededSize <= _maxAtlasSize) {
        uint32_t h = packWithSize(neededSize);
        if (h <= neededSize) break;
        neededSize *= 2;
    }

    if (neededSize > _maxAtlasSize) {
        yerror("CardTextureManager: atlas overflow, cannot fit textures in {}x{}", _maxAtlasSize, _maxAtlasSize);
        return Err<void>("Atlas overflow - too many/large textures");
    }

    // Recreate GPU texture if size changed
    if (neededSize != _currentAtlasSize || !_atlasInitialized) {
        yinfo("CardTextureManager: atlas {}x{} -> {}x{}", _currentAtlasSize, _currentAtlasSize, neededSize, neededSize);
        _currentAtlasSize = neededSize;
        if (auto res = createAtlasTexture(); !res) {
            return res;
        }
        _atlasInitialized = true;
        // Re-pack with final size (positions may differ after resize)
        packWithSize(_currentAtlasSize);
    }

    yinfo("CardTextureManager: packed {} textures into {}x{} atlas",
          entries.size(), _currentAtlasSize, _currentAtlasSize);

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
        _allocator->releaseTexture(_atlasTexture);
        _atlasTexture = nullptr;
    }
    if (_atlasSampler) {
        wgpuSamplerRelease(_atlasSampler);
        _atlasSampler = nullptr;
    }

    WGPUTextureDescriptor texDesc = {};
    texDesc.label = WGPU_STR("CardTextureAtlas");
    texDesc.size.width = _currentAtlasSize;
    texDesc.size.height = _currentAtlasSize;
    texDesc.size.depthOrArrayLayers = 1;
    texDesc.mipLevelCount = 1;
    texDesc.sampleCount = 1;
    texDesc.dimension = WGPUTextureDimension_2D;
    texDesc.format = WGPUTextureFormat_RGBA8Unorm;
    texDesc.usage = WGPUTextureUsage_TextureBinding |
                    WGPUTextureUsage_CopyDst;

    _atlasTexture = _allocator->createTexture(texDesc);
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

}  // namespace yetty
