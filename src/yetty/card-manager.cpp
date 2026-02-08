#include <yetty/card-manager.h>
#include <yetty/gpu-allocator.h>
#include <yetty/wgpu-compat.h>
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cstring>
#include <array>
#include <vector>

namespace yetty {

// =============================================================================
// Helper classes for metadata (moved from card-buffer-manager.cpp)
// =============================================================================

class MetadataPool {
public:
    MetadataPool(uint32_t slotSize, uint32_t baseOffset, uint32_t slotCount);
    Result<uint32_t> allocate();
    Result<void> deallocate(uint32_t offset);
    uint32_t slotSize() const { return _slotSize; }
    uint32_t capacity() const { return _slotCount; }
    uint32_t used() const { return _slotCount - static_cast<uint32_t>(_freeSlots.size()); }

private:
    uint32_t _slotSize;
    uint32_t _baseOffset;
    uint32_t _slotCount;
    std::vector<uint32_t> _freeSlots;
};

class MetadataAllocator {
public:
    static constexpr uint32_t SLOT_32 = 32;
    static constexpr uint32_t SLOT_64 = 64;
    static constexpr uint32_t SLOT_128 = 128;
    static constexpr uint32_t SLOT_256 = 256;

    MetadataAllocator(uint32_t pool32Count, uint32_t pool64Count,
                      uint32_t pool128Count, uint32_t pool256Count);
    Result<MetadataHandle> allocate(uint32_t size);
    Result<void> deallocate(MetadataHandle handle);
    uint32_t totalSize() const { return _totalSize; }
    uint32_t highWaterMark() const { return _highWaterMark; }

private:
    MetadataPool* findPool(uint32_t size);
    MetadataPool* findPoolBySlotSize(uint32_t slotSize);

    MetadataPool _pool32;
    MetadataPool _pool64;
    MetadataPool _pool128;
    MetadataPool _pool256;
    uint32_t _totalSize;
    uint32_t _highWaterMark = 0;
};

class MetadataDirtyTracker {
public:
    void markDirty(uint32_t offset, uint32_t size) { _dirtyRanges.emplace_back(offset, size); }
    void clear() { _dirtyRanges.clear(); }
    bool hasDirty() const { return !_dirtyRanges.empty(); }

    std::vector<std::pair<uint32_t, uint32_t>> getCoalescedRanges(uint32_t maxGap = 64) {
        if (_dirtyRanges.empty()) return {};
        std::sort(_dirtyRanges.begin(), _dirtyRanges.end());
        std::vector<std::pair<uint32_t, uint32_t>> coalesced;
        uint32_t start = _dirtyRanges[0].first;
        uint32_t end = start + _dirtyRanges[0].second;
        for (size_t i = 1; i < _dirtyRanges.size(); ++i) {
            uint32_t rStart = _dirtyRanges[i].first;
            uint32_t rEnd = rStart + _dirtyRanges[i].second;
            if (rStart <= end + maxGap) {
                end = std::max(end, rEnd);
            } else {
                coalesced.emplace_back(start, end - start);
                start = rStart;
                end = rEnd;
            }
        }
        coalesced.emplace_back(start, end - start);
        return coalesced;
    }

private:
    std::vector<std::pair<uint32_t, uint32_t>> _dirtyRanges;
};

// =============================================================================
// MetadataPool / MetadataAllocator implementations
// =============================================================================

MetadataPool::MetadataPool(uint32_t slotSize, uint32_t baseOffset, uint32_t slotCount)
    : _slotSize(slotSize), _baseOffset(baseOffset), _slotCount(slotCount) {
    _freeSlots.reserve(slotCount);
    for (uint32_t i = 0; i < slotCount; ++i) {
        _freeSlots.push_back(baseOffset + (slotCount - 1 - i) * slotSize);
    }
}

Result<uint32_t> MetadataPool::allocate() {
    if (_freeSlots.empty()) return Err("MetadataPool: no free slots");
    uint32_t offset = _freeSlots.back();
    _freeSlots.pop_back();
    return Ok(offset);
}

Result<void> MetadataPool::deallocate(uint32_t offset) {
    if (offset < _baseOffset || offset >= _baseOffset + _slotCount * _slotSize) {
        return Err("MetadataPool: offset out of range");
    }
    if ((offset - _baseOffset) % _slotSize != 0) {
        return Err("MetadataPool: misaligned offset");
    }
    _freeSlots.push_back(offset);
    return Ok();
}

MetadataAllocator::MetadataAllocator(uint32_t pool32Count, uint32_t pool64Count,
                                     uint32_t pool128Count, uint32_t pool256Count)
    : _pool32(SLOT_32, 0, pool32Count)
    , _pool64(SLOT_64, pool32Count * SLOT_32, pool64Count)
    , _pool128(SLOT_128, pool32Count * SLOT_32 + pool64Count * SLOT_64, pool128Count)
    , _pool256(SLOT_256, pool32Count * SLOT_32 + pool64Count * SLOT_64 + pool128Count * SLOT_128, pool256Count)
    , _totalSize(pool32Count * SLOT_32 + pool64Count * SLOT_64 +
                 pool128Count * SLOT_128 + pool256Count * SLOT_256) {
}

MetadataPool* MetadataAllocator::findPool(uint32_t size) {
    if (size <= SLOT_32) return &_pool32;
    if (size <= SLOT_64) return &_pool64;
    if (size <= SLOT_128) return &_pool128;
    if (size <= SLOT_256) return &_pool256;
    return nullptr;
}

MetadataPool* MetadataAllocator::findPoolBySlotSize(uint32_t slotSize) {
    if (slotSize == SLOT_32) return &_pool32;
    if (slotSize == SLOT_64) return &_pool64;
    if (slotSize == SLOT_128) return &_pool128;
    if (slotSize == SLOT_256) return &_pool256;
    return nullptr;
}

Result<MetadataHandle> MetadataAllocator::allocate(uint32_t size) {
    MetadataPool* pool = findPool(size);
    if (!pool) return Err("MetadataAllocator: requested size too large");
    auto result = pool->allocate();
    if (!result) return Err("MetadataAllocator: pool exhausted");
    uint32_t offset = result.value();
    uint32_t slotSize = pool->slotSize();
    uint32_t endOffset = offset + slotSize;
    if (endOffset > _highWaterMark) _highWaterMark = endOffset;
    return Ok(MetadataHandle{offset, slotSize});
}

Result<void> MetadataAllocator::deallocate(MetadataHandle handle) {
    MetadataPool* pool = findPoolBySlotSize(handle.size);
    if (!pool) return Err("MetadataAllocator: invalid handle size");
    return pool->deallocate(handle.offset);
}

// =============================================================================
// CardManagerImpl
// =============================================================================

class CardManagerImpl : public CardManager {
public:
    CardManagerImpl(GPUContext* gpuContext, GpuAllocator::Ptr allocator,
                    WGPUBuffer uniformBuffer, uint32_t uniformSize, Config config)
        : _gpuContext(gpuContext)
        , _device(gpuContext->device)
        , _allocator(std::move(allocator))
        , _uniformBuffer(uniformBuffer)
        , _uniformSize(uniformSize)
        , _config(config)
        , _metadataAllocator(config.metadata.pool32Count, config.metadata.pool64Count,
                             config.metadata.pool128Count, config.metadata.pool256Count) {}

    ~CardManagerImpl() override;
    Result<void> init() noexcept;

    // Metadata
    Result<MetadataHandle> allocateMetadata(uint32_t size) override;
    Result<void> deallocateMetadata(MetadataHandle handle) override;
    Result<void> writeMetadata(MetadataHandle handle, const void* data, uint32_t size) override;
    Result<void> writeMetadataAt(MetadataHandle handle, uint32_t offset, const void* data, uint32_t size) override;

    // Manager accessors
    CardBufferManager::Ptr bufferManager() const override { return _bufferMgr; }
    CardTextureManager::Ptr textureManager() const override { return _textureMgr; }

    // GPU resources
    WGPUBuffer metadataBuffer() const override { return _metadataGpuBuffer; }
    WGPUBindGroupLayout sharedBindGroupLayout() const override { return _sharedBindGroupLayout; }
    WGPUBindGroup sharedBindGroup() const override { return _sharedBindGroup; }
    Result<void> updateBindGroup() override;
    void invalidateBindGroup() override { _bindGroupHasRealAtlas = false; }

    // GPU upload
    Result<void> flush(WGPUQueue queue) override;

private:
    Result<void> createMetadataGpuBuffer();
    Result<void> createDummyResources();
    Result<void> createSharedBindGroup();

    GPUContext* _gpuContext;
    WGPUDevice _device;
    GpuAllocator::Ptr _allocator;
    WGPUBuffer _uniformBuffer;
    uint32_t _uniformSize;
    Config _config;

    CardBufferManager::Ptr _bufferMgr;
    CardTextureManager::Ptr _textureMgr;

    // Metadata (owned by CardManager)
    MetadataAllocator _metadataAllocator;
    std::vector<uint32_t> _metadataCpuBuffer;
    WGPUBuffer _metadataGpuBuffer = nullptr;
    MetadataDirtyTracker _metadataDirty;

    // Bind group
    WGPUBindGroupLayout _sharedBindGroupLayout = nullptr;
    WGPUBindGroup _sharedBindGroup = nullptr;
    bool _bindGroupHasRealAtlas = false;

    // Dummy resources for bind group before atlas init
    WGPUTexture _dummyAtlasTexture = nullptr;
    WGPUTextureView _dummyAtlasTextureView = nullptr;
    WGPUSampler _dummyAtlasSampler = nullptr;
};

// =============================================================================
// Factory
// =============================================================================

Result<CardManager::Ptr> CardManager::create(GPUContext* gpuContext,
                                              GpuAllocator::Ptr allocator,
                                              WGPUBuffer uniformBuffer,
                                              uint32_t uniformSize,
                                              Config config) noexcept {
    auto mgr = std::make_shared<CardManagerImpl>(gpuContext, std::move(allocator), uniformBuffer, uniformSize, config);
    if (auto res = mgr->init(); !res) {
        return Err<Ptr>("CardManager init failed", res);
    }
    return Ok(std::static_pointer_cast<CardManager>(mgr));
}

// =============================================================================
// Init / Destroy
// =============================================================================

Result<void> CardManagerImpl::init() noexcept {
    // Create texture manager
    auto texRes = CardTextureManager::create(_gpuContext, _allocator, _config.texture);
    if (!texRes) return Err<void>("Failed to create CardTextureManager", texRes);
    _textureMgr = *texRes;

    // Create buffer manager
    auto bufRes = CardBufferManager::create(_gpuContext, _allocator);
    if (!bufRes) return Err<void>("Failed to create CardBufferManager", bufRes);
    _bufferMgr = *bufRes;

    // Create metadata GPU buffer
    _metadataCpuBuffer.resize((_metadataAllocator.totalSize() + 3) / 4, 0);
    if (auto res = createMetadataGpuBuffer(); !res) {
        return Err<void>("Failed to create metadata GPU buffer", res);
    }

    // Create dummy resources for bind group
    if (auto res = createDummyResources(); !res) {
        return Err<void>("Failed to create dummy resources", res);
    }

    // Create initial bind group
    if (auto res = createSharedBindGroup(); !res) {
        return Err<void>("Failed to create shared bind group", res);
    }

    yinfo("CardManager: initialized with metadata, buffer, and texture managers");
    return Ok();
}

CardManagerImpl::~CardManagerImpl() {
    if (_sharedBindGroup) wgpuBindGroupRelease(_sharedBindGroup);
    if (_metadataGpuBuffer) _allocator->releaseBuffer(_metadataGpuBuffer);
    if (_dummyAtlasSampler) wgpuSamplerRelease(_dummyAtlasSampler);
    if (_dummyAtlasTextureView) wgpuTextureViewRelease(_dummyAtlasTextureView);
    if (_dummyAtlasTexture) _allocator->releaseTexture(_dummyAtlasTexture);
}

// =============================================================================
// Metadata GPU buffer
// =============================================================================

Result<void> CardManagerImpl::createMetadataGpuBuffer() {
    size_t metadataBytes = _metadataCpuBuffer.size() * sizeof(uint32_t);

    WGPUBufferDescriptor metaDesc = {};
    metaDesc.label = WGPU_STR("CardMetadataBuffer");
    metaDesc.size = metadataBytes;
    metaDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    metaDesc.mappedAtCreation = false;

    _metadataGpuBuffer = _allocator->createBuffer(metaDesc);
    if (!_metadataGpuBuffer) {
        return Err("Failed to create metadata GPU buffer");
    }
    return Ok();
}

// =============================================================================
// Metadata operations
// =============================================================================

Result<MetadataHandle> CardManagerImpl::allocateMetadata(uint32_t size) {
    return _metadataAllocator.allocate(size);
}

Result<void> CardManagerImpl::deallocateMetadata(MetadataHandle handle) {
    return _metadataAllocator.deallocate(handle);
}

Result<void> CardManagerImpl::writeMetadata(MetadataHandle handle, const void* data, uint32_t size) {
    return writeMetadataAt(handle, 0, data, size);
}

Result<void> CardManagerImpl::writeMetadataAt(MetadataHandle handle, uint32_t offset,
                                               const void* data, uint32_t size) {
    if (!handle.isValid()) return Err("writeMetadataAt: invalid handle");
    if (offset + size > handle.size) return Err("writeMetadataAt: write exceeds slot size");

    uint32_t bufferOffset = handle.offset + offset;
    uint8_t* bufferBytes = reinterpret_cast<uint8_t*>(_metadataCpuBuffer.data());
    std::memcpy(bufferBytes + bufferOffset, data, size);
    _metadataDirty.markDirty(bufferOffset, size);
    return Ok();
}

// =============================================================================
// Dummy resources & bind group
// =============================================================================

Result<void> CardManagerImpl::createDummyResources() {
    WGPUTextureDescriptor texDesc = {};
    texDesc.label = WGPU_STR("dummy atlas");
    texDesc.size = {1, 1, 1};
    texDesc.format = WGPUTextureFormat_RGBA8Unorm;
    texDesc.usage = WGPUTextureUsage_TextureBinding;
    texDesc.dimension = WGPUTextureDimension_2D;
    texDesc.mipLevelCount = 1;
    texDesc.sampleCount = 1;
    _dummyAtlasTexture = _allocator->createTexture(texDesc);
    if (!_dummyAtlasTexture) return Err<void>("Failed to create dummy atlas texture");

    WGPUTextureViewDescriptor viewDesc = {};
    viewDesc.format = WGPUTextureFormat_RGBA8Unorm;
    viewDesc.dimension = WGPUTextureViewDimension_2D;
    viewDesc.mipLevelCount = 1;
    viewDesc.arrayLayerCount = 1;
    _dummyAtlasTextureView = wgpuTextureCreateView(_dummyAtlasTexture, &viewDesc);
    if (!_dummyAtlasTextureView) return Err<void>("Failed to create dummy atlas texture view");

    WGPUSamplerDescriptor samplerDesc = {};
    samplerDesc.minFilter = WGPUFilterMode_Linear;
    samplerDesc.magFilter = WGPUFilterMode_Linear;
    samplerDesc.addressModeU = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeV = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeW = WGPUAddressMode_ClampToEdge;
    samplerDesc.maxAnisotropy = 1;
    _dummyAtlasSampler = wgpuDeviceCreateSampler(_device, &samplerDesc);
    if (!_dummyAtlasSampler) return Err<void>("Failed to create dummy atlas sampler");

    return Ok();
}

Result<void> CardManagerImpl::createSharedBindGroup() {
    if (_sharedBindGroup) {
        wgpuBindGroupRelease(_sharedBindGroup);
        _sharedBindGroup = nullptr;
    }

    if (!_sharedBindGroupLayout) {
        _sharedBindGroupLayout = _gpuContext->sharedBindGroupLayout;
        if (!_sharedBindGroupLayout) return Err<void>("No shared bind group layout in GPUContext");
    }

    bool atlasReady = _textureMgr && _textureMgr->atlasInitialized();
    WGPUTextureView texView = atlasReady ? _textureMgr->atlasTextureView() : _dummyAtlasTextureView;
    WGPUSampler sampler = atlasReady ? _textureMgr->atlasSampler() : _dummyAtlasSampler;

    if (!texView || !sampler) return Err<void>("Missing texture view or sampler for bind group");

    std::array<WGPUBindGroupEntry, 5> bindEntries = {};

    // Binding 0: uniform buffer
    bindEntries[0].binding = 0;
    bindEntries[0].buffer = _uniformBuffer;
    bindEntries[0].size = _uniformSize;

    // Binding 1: metadata buffer (owned by CardManager)
    bindEntries[1].binding = 1;
    bindEntries[1].buffer = _metadataGpuBuffer;
    bindEntries[1].size = wgpuBufferGetSize(_metadataGpuBuffer);

    // Binding 2: linear data buffer (owned by CardBufferManager)
    bindEntries[2].binding = 2;
    bindEntries[2].buffer = _bufferMgr->bufferGpu();
    bindEntries[2].size = wgpuBufferGetSize(_bufferMgr->bufferGpu());

    // Binding 3: atlas texture view
    bindEntries[3].binding = 3;
    bindEntries[3].textureView = texView;

    // Binding 4: atlas sampler
    bindEntries[4].binding = 4;
    bindEntries[4].sampler = sampler;

    WGPUBindGroupDescriptor bindDesc = {};
    bindDesc.label = WGPU_STR("Shared Bind Group");
    bindDesc.layout = _sharedBindGroupLayout;
    bindDesc.entryCount = bindEntries.size();
    bindDesc.entries = bindEntries.data();

    _sharedBindGroup = wgpuDeviceCreateBindGroup(_device, &bindDesc);
    if (!_sharedBindGroup) return Err<void>("Failed to create shared bind group");

    _bindGroupHasRealAtlas = atlasReady;
    _gpuContext->sharedBindGroup = _sharedBindGroup;

    yinfo("CardManager: bind group created (atlas={})", atlasReady ? "real" : "dummy");
    return Ok();
}

Result<void> CardManagerImpl::updateBindGroup() {
    bool atlasReady = _textureMgr && _textureMgr->atlasInitialized();
    bool bufferDirty = _bufferMgr && _bufferMgr->bindGroupDirty();

    if (atlasReady != _bindGroupHasRealAtlas || bufferDirty) {
        if (_bufferMgr) _bufferMgr->clearBindGroupDirty();
        return createSharedBindGroup();
    }
    return Ok();
}

// =============================================================================
// Flush: metadata + buffer + atlas
// =============================================================================

Result<void> CardManagerImpl::flush(WGPUQueue queue) {
    // Flush metadata
    if (_metadataDirty.hasDirty()) {
        const uint8_t* metadataBytes = reinterpret_cast<const uint8_t*>(_metadataCpuBuffer.data());
        auto ranges = _metadataDirty.getCoalescedRanges();
        for (const auto& [offset, size] : ranges) {
            ydebug("CardManager::flush: metadata GPU upload offset={} size={}", offset, size);
            wgpuQueueWriteBuffer(queue, _metadataGpuBuffer, offset,
                                 metadataBytes + offset, size);
        }
        _metadataDirty.clear();
    } else {
        ydebug("CardManager::flush: no dirty metadata");
    }

    // Upload dirty atlas regions to GPU
    if (_textureMgr) {
        ydebug("CardManager::flush: uploadAtlas");
        if (auto res = _textureMgr->uploadAtlas(queue); !res) {
            return Err<void>("CardManager: atlas upload failed", res);
        }
    }

    // Update bind group if needed
    bool atlasReady = _textureMgr && _textureMgr->atlasInitialized();
    ydebug("CardManager::flush: updateBindGroup atlasReady={} hasRealAtlas={}", atlasReady, _bindGroupHasRealAtlas);
    if (auto res = updateBindGroup(); !res) {
        return Err<void>("CardManager: bind group update failed", res);
    }

    // Flush buffer manager
    if (_bufferMgr) {
        if (auto res = _bufferMgr->flush(queue); !res) {
            return Err<void>("CardManager: buffer flush failed", res);
        }
    }

    return Ok();
}

}  // namespace yetty
