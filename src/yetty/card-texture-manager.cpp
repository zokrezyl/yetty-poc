#include <yetty/card-texture-manager.h>
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cstring>
#include <vector>
#include <unordered_map>

#ifndef CMAKE_SOURCE_DIR
#define CMAKE_SOURCE_DIR "."
#endif

// WebGPU string helper
#define WGPU_STR(s) WGPUStringView{.data = s, .length = WGPU_STRLEN}

namespace yetty {

// =============================================================================
// Texture handle tracking data
// =============================================================================

struct TextureHandleData {
    const uint8_t* pixels = nullptr;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t atlasX = 0;
    uint32_t atlasY = 0;
    bool packed = false;
};

// =============================================================================
// CardTextureManagerImpl - implementation class
// =============================================================================

class CardTextureManagerImpl : public CardTextureManager {
public:
    CardTextureManagerImpl(GPUContext* gpuContext, Config config) noexcept;
    ~CardTextureManagerImpl() override;

    Result<void> init() noexcept;

    // Texture atlas operations
    Result<TextureHandle> allocateTextureHandle() override;
    Result<void> linkTextureData(TextureHandle handle,
                                  const uint8_t* pixels,
                                  uint32_t width, uint32_t height) override;
    Result<void> deallocateTextureHandle(TextureHandle handle) override;
    void invalidateAllTextureHandles() override;
    Result<void> packAtlas(WGPUQueue queue) override;
    Result<void> writeToAtlas(TextureHandle handle, WGPUQueue queue) override;
    Result<void> writeAllToAtlas(WGPUQueue queue) override;
    AtlasPosition getAtlasPosition(TextureHandle handle) const override;

    // Atlas initialization
    Result<void> initAtlas() override;

    // Accessors
    WGPUTexture atlasTexture() const override { return _atlasTexture; }
    WGPUTextureView atlasTextureView() const override { return _atlasTextureView; }
    WGPUSampler atlasSampler() const override { return _atlasSampler; }
    WGPUBuffer textureBuffer() const override { return _textureBuffer; }
    bool atlasInitialized() const override { return _atlasInitialized; }
    uint32_t atlasSize() const override { return _currentAtlasSize; }

    Stats getStats() const override;

private:
    Result<void> createAtlasTexture();
    Result<void> growAtlas();

    GPUContext* _gpuContext;
    WGPUDevice _device;
    Config _config;

    // Atlas Resources
    WGPUTexture _atlasTexture = nullptr;
    WGPUTextureView _atlasTextureView = nullptr;
    WGPUSampler _atlasSampler = nullptr;
    bool _atlasInitialized = false;
    uint32_t _currentAtlasSize;
    uint32_t _maxAtlasSize;

    // Texture buffer (binding 5: raw pixel data)
    WGPUBuffer _textureBuffer = nullptr;
    std::vector<uint32_t> _textureCpuBuffer;
    uint32_t _textureBufferCapacity = 4;  // Start tiny
    static constexpr uint32_t MAX_TEXTURE_BUFFER_SIZE = 128 * 1024 * 1024;  // 128 MB max

    // Texture handle tracking
    std::unordered_map<uint32_t, TextureHandleData> _textureHandles;
    uint32_t _nextTextureHandleId = 1;  // 0 = invalid
};

// =============================================================================
// CardTextureManager factory
// =============================================================================

Result<CardTextureManager::Ptr> CardTextureManager::create(GPUContext* gpuContext, Config config) noexcept {
    auto mgr = std::make_shared<CardTextureManagerImpl>(gpuContext, config);
    if (auto res = mgr->init(); !res) {
        return Err<Ptr>("Failed to initialize CardTextureManager", res);
    }
    return Ok(std::move(mgr));
}

// =============================================================================
// CardTextureManagerImpl implementation
// =============================================================================

CardTextureManagerImpl::CardTextureManagerImpl(GPUContext* gpuContext, Config config) noexcept
    : _gpuContext(gpuContext)
    , _device(gpuContext->device)
    , _config(config)
    , _currentAtlasSize(config.initialAtlasSize)
    , _maxAtlasSize(config.maxAtlasSize) {
}

Result<void> CardTextureManagerImpl::init() noexcept {
    // Create texture buffer (binding 5) - starts tiny, grows on demand
    _textureCpuBuffer.resize(1, 0);  // 4 bytes placeholder

    WGPUBufferDescriptor bufDesc = {};
    bufDesc.label = WGPU_STR("CardTextureBuffer");
    bufDesc.size = _textureCpuBuffer.size() * sizeof(uint32_t);
    bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    bufDesc.mappedAtCreation = false;

    _textureBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);
    if (!_textureBuffer) {
        return Err<void>("Failed to create texture buffer");
    }

    yinfo("GPU_ALLOC CardTextureManager: textureBuffer={} bytes", bufDesc.size);

    // Atlas is lazily initialized on first packAtlas() call
    return Ok();
}

CardTextureManagerImpl::~CardTextureManagerImpl() {
    if (_atlasSampler) wgpuSamplerRelease(_atlasSampler);
    if (_atlasTextureView) wgpuTextureViewRelease(_atlasTextureView);
    if (_atlasTexture) wgpuTextureRelease(_atlasTexture);
    if (_textureBuffer) wgpuBufferRelease(_textureBuffer);
}

// =============================================================================
// Texture atlas operations
// =============================================================================

Result<TextureHandle> CardTextureManagerImpl::allocateTextureHandle() {
    uint32_t id = _nextTextureHandleId++;
    _textureHandles[id] = TextureHandleData{};
    ydebug("CardTextureManager: allocated texture handle id={}", id);
    return Ok(TextureHandle{id});
}

Result<void> CardTextureManagerImpl::linkTextureData(TextureHandle handle,
                                                      const uint8_t* pixels,
                                                      uint32_t width, uint32_t height) {
    auto it = _textureHandles.find(handle.id);
    if (it == _textureHandles.end()) {
        return Err("linkTextureData: invalid handle");
    }

    it->second.pixels = pixels;
    it->second.width = width;
    it->second.height = height;
    it->second.packed = false;  // Needs repacking

    ydebug("CardTextureManager: linked texture data handle={} {}x{}", handle.id, width, height);
    return Ok();
}

Result<void> CardTextureManagerImpl::deallocateTextureHandle(TextureHandle handle) {
    auto it = _textureHandles.find(handle.id);
    if (it == _textureHandles.end()) {
        return Err("deallocateTextureHandle: invalid handle");
    }

    _textureHandles.erase(it);
    ydebug("CardTextureManager: deallocated texture handle id={}", handle.id);
    return Ok();
}

void CardTextureManagerImpl::invalidateAllTextureHandles() {
    for (auto& [id, data] : _textureHandles) {
        data.packed = false;
    }
    yinfo("CardTextureManager: invalidated all {} texture handles", _textureHandles.size());
}

Result<void> CardTextureManagerImpl::packAtlas(WGPUQueue queue) {
    (void)queue;

    // Lazily initialize atlas on first pack
    if (!_atlasInitialized && !_textureHandles.empty()) {
        if (auto res = initAtlas(); !res) {
            return Err<void>("packAtlas: lazy atlas init failed", res);
        }
    }

    if (_textureHandles.empty()) {
        return Ok();
    }

    // Collect handles that have linked pixel data
    struct PackEntry {
        uint32_t id;
        uint32_t width;
        uint32_t height;
    };
    std::vector<PackEntry> entries;
    entries.reserve(_textureHandles.size());

    for (const auto& [id, data] : _textureHandles) {
        if (data.pixels && data.width > 0 && data.height > 0) {
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
    uint32_t currentX = 0;
    uint32_t currentY = 0;
    uint32_t rowHeight = 0;

    for (const auto& entry : entries) {
        if (currentX + entry.width > _currentAtlasSize) {
            // Move to next row
            currentY += rowHeight;
            currentX = 0;
            rowHeight = 0;
        }

        auto& data = _textureHandles[entry.id];
        data.atlasX = currentX;
        data.atlasY = currentY;
        data.packed = true;

        currentX += entry.width;
        rowHeight = std::max(rowHeight, entry.height);
    }

    uint32_t totalHeight = currentY + rowHeight;

    // Grow atlas if needed
    while (totalHeight > _currentAtlasSize && _currentAtlasSize < _maxAtlasSize) {
        if (auto res = growAtlas(); !res) {
            return res;
        }

        // Repack with new atlas size
        currentX = 0;
        currentY = 0;
        rowHeight = 0;

        for (const auto& entry : entries) {
            if (currentX + entry.width > _currentAtlasSize) {
                currentY += rowHeight;
                currentX = 0;
                rowHeight = 0;
            }

            auto& data = _textureHandles[entry.id];
            data.atlasX = currentX;
            data.atlasY = currentY;
            data.packed = true;

            currentX += entry.width;
            rowHeight = std::max(rowHeight, entry.height);
        }

        totalHeight = currentY + rowHeight;
    }

    if (totalHeight > _currentAtlasSize) {
        yerror("CardTextureManager: atlas overflow! need {}px height, have {}px", totalHeight, _currentAtlasSize);
        return Err<void>("Atlas overflow - too many/large textures");
    }

    yinfo("CardTextureManager: packed {} textures into atlas (used {}px of {}px height)",
          entries.size(), totalHeight, _currentAtlasSize);

    return Ok();
}

Result<void> CardTextureManagerImpl::writeToAtlas(TextureHandle handle, WGPUQueue queue) {
    auto it = _textureHandles.find(handle.id);
    if (it == _textureHandles.end()) {
        return Err("writeToAtlas: invalid handle");
    }

    const auto& data = it->second;
    if (!data.packed) {
        return Err("writeToAtlas: handle not packed (call packAtlas first)");
    }
    if (!data.pixels || data.width == 0 || data.height == 0) {
        return Err("writeToAtlas: no pixel data linked");
    }

    // Write pixels to the assigned atlas region via wgpuQueueWriteTexture
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

    ydebug("CardTextureManager: wrote {}x{} pixels to atlas at ({}, {})",
           data.width, data.height, data.atlasX, data.atlasY);

    return Ok();
}

Result<void> CardTextureManagerImpl::writeAllToAtlas(WGPUQueue queue) {
    for (const auto& [id, data] : _textureHandles) {
        if (data.packed && data.pixels && data.width > 0 && data.height > 0) {
            TextureHandle h;
            h.id = id;
            if (auto res = writeToAtlas(h, queue); !res) {
                return res;
            }
        }
    }
    return Ok();
}

AtlasPosition CardTextureManagerImpl::getAtlasPosition(TextureHandle handle) const {
    auto it = _textureHandles.find(handle.id);
    if (it == _textureHandles.end() || !it->second.packed) {
        return {0, 0};
    }
    return {it->second.atlasX, it->second.atlasY};
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

Result<void> CardTextureManagerImpl::initAtlas() {
    if (auto res = createAtlasTexture(); !res) {
        return res;
    }

    _atlasInitialized = true;
    yinfo("CardTextureManager: atlas initialized ({}x{})", _currentAtlasSize, _currentAtlasSize);

    return Ok();
}

Result<void> CardTextureManagerImpl::createAtlasTexture() {
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
    // No StorageBinding needed — CPU writes via wgpuQueueWriteTexture
    texDesc.usage = WGPUTextureUsage_TextureBinding |  // Fragment shader reads
                    WGPUTextureUsage_CopyDst;          // CPU writes

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

    _currentAtlasSize = newSize;

    if (auto res = createAtlasTexture(); !res) {
        return res;
    }

    yinfo("CardTextureManager: atlas grown successfully to {}x{}", _currentAtlasSize, _currentAtlasSize);
    return Ok();
}

}  // namespace yetty
