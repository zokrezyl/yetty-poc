#include <yetty/card-buffer-manager.h>
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cstring>
#include <array>
#include <vector>
#include <unordered_map>

#ifndef CMAKE_SOURCE_DIR
#define CMAKE_SOURCE_DIR "."
#endif

// WebGPU string helper
#define WGPU_STR(s) WGPUStringView{.data = s, .length = WGPU_STRLEN}

namespace yetty {

// =============================================================================
// Helper classes (internal to this .cpp file)
// =============================================================================

// Pool allocator for fixed-size metadata slots
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

// Metadata allocator with multiple pools
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

// Free-list allocator for variable-size buffer data
class BufferAllocator {
public:
    explicit BufferAllocator(uint32_t capacity);

    Result<BufferHandle> allocate(uint32_t size);
    Result<void> deallocate(BufferHandle handle);

    uint32_t capacity() const { return _capacity; }
    uint32_t used() const { return _used; }
    uint32_t fragmentCount() const { return static_cast<uint32_t>(_freeBlocks.size()); }
    uint32_t highWaterMark() const { return _highWaterMark; }

    // Grow capacity - adds new free space at the end
    void grow(uint32_t newCapacity) {
        if (newCapacity <= _capacity) return;
        uint32_t extraSpace = newCapacity - _capacity;
        _freeBlocks.push_back({_capacity, extraSpace});
        _capacity = newCapacity;
        mergeFreeBlocks();
    }

private:
    struct FreeBlock {
        uint32_t offset;
        uint32_t size;
    };

    void mergeFreeBlocks();

    uint32_t _capacity;
    uint32_t _used;
    uint32_t _highWaterMark = 0;
    std::vector<FreeBlock> _freeBlocks;
};

// Dirty region tracking
class DirtyTracker {
public:
    void markDirty(uint32_t offset, uint32_t size);
    void clear();

    std::vector<std::pair<uint32_t, uint32_t>> getCoalescedRanges(uint32_t maxGap = 64);
    bool hasDirty() const { return !_dirtyRanges.empty(); }

private:
    std::vector<std::pair<uint32_t, uint32_t>> _dirtyRanges;
};

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
// CardBufferManagerImpl - implementation class
// =============================================================================

class CardBufferManagerImpl : public CardBufferManager {
public:
    CardBufferManagerImpl(GPUContext* gpuContext, WGPUBuffer uniformBuffer, uint32_t uniformSize, Config config) noexcept;
    ~CardBufferManagerImpl() override;

    Result<void> init() noexcept;

    // Metadata
    Result<MetadataHandle> allocateMetadata(uint32_t size) override;
    Result<void> deallocateMetadata(MetadataHandle handle) override;
    Result<void> writeMetadata(MetadataHandle handle, const void* data, uint32_t size) override;
    Result<void> writeMetadataAt(MetadataHandle handle, uint32_t offset, const void* data, uint32_t size) override;

    // Buffer (linear data for plot/draw cards)
    Result<BufferHandle> allocateBuffer(uint32_t size) override;
    Result<void> deallocateBuffer(BufferHandle handle) override;
    Result<void> writeBuffer(BufferHandle handle, const void* data, uint32_t size) override;
    Result<void> writeBufferAt(BufferHandle handle, uint32_t offset, const void* data, uint32_t size) override;
    Result<BufferHandle> allocateBufferAndLink(MetadataHandle metaHandle,
                                                uint32_t metaFieldOffset,
                                                uint32_t bufferSize) override;
    void markBufferDirty(BufferHandle handle) override;

    // Texture atlas
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

    // GPU upload
    Result<void> flush(WGPUQueue queue) override;
    Result<void> initAtlas() override;

    // Accessors
    WGPUBuffer metadataBuffer() const override { return _metadataGpuBuffer; }
    WGPUBuffer bufferGpu() const override { return _bufferGpuBuffer; }

    uint32_t metadataHighWaterMark() const override { return _metadataAllocator.highWaterMark(); }
    uint32_t bufferHighWaterMark() const override { return _bufferAllocator.highWaterMark(); }

    WGPUTexture atlasTexture() const override { return _atlasTexture; }
    WGPUTextureView atlasTextureView() const override { return _atlasTextureView; }
    WGPUSampler atlasSampler() const override { return _atlasSampler; }
    bool atlasInitialized() const override { return _atlasInitialized; }
    WGPUBindGroupLayout sharedBindGroupLayout() const override { return _sharedBindGroupLayout; }
    WGPUBindGroup sharedBindGroup() const override { return _sharedBindGroup; }

    Stats getStats() const override;

private:
    Result<void> createGpuBuffers();
    Result<void> createAtlasTexture();
    Result<void> createSharedBindGroup();
    Result<void> growBufferGpu(uint32_t requiredSize);
    Result<void> growAtlas();

    GPUContext* _gpuContext;
    WGPUDevice _device;
    Config _config;
    WGPUBuffer _uniformBuffer;
    uint32_t _uniformSize;

    // CPU buffers
    std::vector<uint32_t> _metadataCpuBuffer;
    std::vector<uint32_t> _bufferCpuBuffer;

    WGPUBuffer _metadataGpuBuffer = nullptr;
    WGPUBuffer _bufferGpuBuffer = nullptr;

    MetadataAllocator _metadataAllocator;
    BufferAllocator _bufferAllocator;

    DirtyTracker _metadataDirty;
    DirtyTracker _bufferDirty;

    // Atlas Resources
    WGPUTexture _atlasTexture = nullptr;
    WGPUTextureView _atlasTextureView = nullptr;
    WGPUSampler _atlasSampler = nullptr;
    bool _atlasInitialized = false;
    uint32_t _currentAtlasSize = IMAGE_ATLAS_SIZE;
    static constexpr uint32_t MAX_ATLAS_SIZE = 8192;

    // Texture handle tracking
    std::unordered_map<uint32_t, TextureHandleData> _textureHandles;
    uint32_t _nextTextureHandleId = 1;  // 0 = invalid

    // Current buffer capacity (can grow)
    uint32_t _currentBufferCapacity;
    static constexpr uint32_t MAX_BUFFER_SIZE = 32 * 1024 * 1024;  // 32 MB max

    // Shared bind group for rendering
    WGPUBindGroupLayout _sharedBindGroupLayout = nullptr;
    WGPUBindGroup _sharedBindGroup = nullptr;

    // Dummy resources for bind group creation before atlas is initialized
    WGPUTexture _dummyAtlasTexture = nullptr;
    WGPUTextureView _dummyAtlasTextureView = nullptr;
    WGPUSampler _dummyAtlasSampler = nullptr;
    WGPUBuffer _dummyBuffer = nullptr;  // Dummy for binding 5 (was textureData)
    Result<void> createDummyResources();
};

// =============================================================================
// Helper class implementations
// =============================================================================

// --- MetadataPool ---

MetadataPool::MetadataPool(uint32_t slotSize, uint32_t baseOffset, uint32_t slotCount)
    : _slotSize(slotSize)
    , _baseOffset(baseOffset)
    , _slotCount(slotCount) {
    _freeSlots.reserve(slotCount);
    for (uint32_t i = 0; i < slotCount; ++i) {
        _freeSlots.push_back(baseOffset + (slotCount - 1 - i) * slotSize);
    }
}

Result<uint32_t> MetadataPool::allocate() {
    if (_freeSlots.empty()) {
        return Err("MetadataPool: no free slots");
    }
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

// --- MetadataAllocator ---

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
    if (!pool) {
        return Err("MetadataAllocator: requested size too large");
    }

    auto result = pool->allocate();
    if (!result) {
        return Err("MetadataAllocator: pool exhausted");
    }

    uint32_t offset = result.value();
    uint32_t slotSize = pool->slotSize();

    uint32_t endOffset = offset + slotSize;
    if (endOffset > _highWaterMark) {
        _highWaterMark = endOffset;
    }

    return Ok(MetadataHandle{offset, slotSize});
}

Result<void> MetadataAllocator::deallocate(MetadataHandle handle) {
    MetadataPool* pool = findPoolBySlotSize(handle.size);
    if (!pool) {
        return Err("MetadataAllocator: invalid handle size");
    }
    return pool->deallocate(handle.offset);
}

// --- BufferAllocator ---

BufferAllocator::BufferAllocator(uint32_t capacity)
    : _capacity(capacity)
    , _used(0) {
    _freeBlocks.push_back({0, capacity});
}

Result<BufferHandle> BufferAllocator::allocate(uint32_t size) {
    if (size == 0) {
        return Err("BufferAllocator: cannot allocate zero bytes");
    }

    // Align to 16 bytes
    size = (size + 15) & ~15u;

    // First-fit allocation
    for (auto it = _freeBlocks.begin(); it != _freeBlocks.end(); ++it) {
        if (it->size >= size) {
            uint32_t offset = it->offset;

            if (it->size == size) {
                _freeBlocks.erase(it);
            } else {
                it->offset += size;
                it->size -= size;
            }

            _used += size;

            uint32_t endOffset = offset + size;
            if (endOffset > _highWaterMark) {
                _highWaterMark = endOffset;
            }

            return Ok(BufferHandle{nullptr, offset, size});
        }
    }

    return Err("BufferAllocator: out of memory");
}

Result<void> BufferAllocator::deallocate(BufferHandle handle) {
    if (!handle.isValid()) {
        return Err("BufferAllocator: invalid handle");
    }

    auto it = std::lower_bound(_freeBlocks.begin(), _freeBlocks.end(), handle.offset,
        [](const FreeBlock& block, uint32_t offset) {
            return block.offset < offset;
        });

    _freeBlocks.insert(it, {handle.offset, handle.size});
    _used -= handle.size;

    mergeFreeBlocks();
    return Ok();
}

void BufferAllocator::mergeFreeBlocks() {
    if (_freeBlocks.size() < 2) return;

    std::vector<FreeBlock> merged;
    merged.reserve(_freeBlocks.size());
    merged.push_back(_freeBlocks[0]);

    for (size_t i = 1; i < _freeBlocks.size(); ++i) {
        FreeBlock& last = merged.back();
        const FreeBlock& curr = _freeBlocks[i];

        if (last.offset + last.size == curr.offset) {
            last.size += curr.size;
        } else {
            merged.push_back(curr);
        }
    }

    _freeBlocks = std::move(merged);
}

// --- DirtyTracker ---

void DirtyTracker::markDirty(uint32_t offset, uint32_t size) {
    _dirtyRanges.emplace_back(offset, size);
}

void DirtyTracker::clear() {
    _dirtyRanges.clear();
}

std::vector<std::pair<uint32_t, uint32_t>> DirtyTracker::getCoalescedRanges(uint32_t maxGap) {
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

// =============================================================================
// CardBufferManager factory
// =============================================================================

Result<CardBufferManager::Ptr> CardBufferManager::create(GPUContext* gpuContext, WGPUBuffer uniformBuffer, uint32_t uniformSize, Config config) noexcept {
    auto mgr = std::make_shared<CardBufferManagerImpl>(gpuContext, uniformBuffer, uniformSize, config);
    if (auto res = mgr->init(); !res) {
        return Err<Ptr>("Failed to initialize CardBufferManager", res);
    }
    return Ok(std::move(mgr));
}

// =============================================================================
// CardBufferManagerImpl implementation
// =============================================================================

CardBufferManagerImpl::CardBufferManagerImpl(GPUContext* gpuContext, WGPUBuffer uniformBuffer, uint32_t uniformSize, Config config) noexcept
    : _gpuContext(gpuContext)
    , _device(gpuContext->device)
    , _config(config)
    , _uniformBuffer(uniformBuffer)
    , _uniformSize(uniformSize)
    , _metadataAllocator(config.metadataPool32Count, config.metadataPool64Count,
                         config.metadataPool128Count, config.metadataPool256Count)
    , _bufferAllocator(4)          // Start tiny, grow on demand
    , _currentBufferCapacity(4) {
}

Result<void> CardBufferManagerImpl::init() noexcept {
    _metadataCpuBuffer.resize((_metadataAllocator.totalSize() + 3) / 4, 0);
    _bufferCpuBuffer.resize(1, 0);  // 4 bytes placeholder

    if (auto res = createGpuBuffers(); !res) {
        return Err<void>("Failed to create GPU buffers", res);
    }

    if (auto res = createDummyResources(); !res) {
        return Err<void>("Failed to create dummy resources", res);
    }

    if (auto res = createSharedBindGroup(); !res) {
        return Err<void>("Failed to create shared bind group", res);
    }

    return Ok();
}

CardBufferManagerImpl::~CardBufferManagerImpl() {
    if (_metadataGpuBuffer) wgpuBufferRelease(_metadataGpuBuffer);
    if (_bufferGpuBuffer) wgpuBufferRelease(_bufferGpuBuffer);

    if (_sharedBindGroup) wgpuBindGroupRelease(_sharedBindGroup);

    if (_atlasSampler) wgpuSamplerRelease(_atlasSampler);
    if (_atlasTextureView) wgpuTextureViewRelease(_atlasTextureView);
    if (_atlasTexture) wgpuTextureRelease(_atlasTexture);

    if (_dummyAtlasSampler) wgpuSamplerRelease(_dummyAtlasSampler);
    if (_dummyAtlasTextureView) wgpuTextureViewRelease(_dummyAtlasTextureView);
    if (_dummyAtlasTexture) wgpuTextureRelease(_dummyAtlasTexture);
    if (_dummyBuffer) wgpuBufferRelease(_dummyBuffer);
}

Result<void> CardBufferManagerImpl::createGpuBuffers() {
    size_t metadataBytes = _metadataCpuBuffer.size() * sizeof(uint32_t);
    size_t bufferBytes = _bufferCpuBuffer.size() * sizeof(uint32_t);

    yinfo("GPU_ALLOC CardBufferManager: metadataBuffer={} bytes ({:.2f} MB)",
          metadataBytes, metadataBytes / (1024.0 * 1024.0));
    yinfo("GPU_ALLOC CardBufferManager: bufferGpu={} bytes ({:.2f} MB)",
          bufferBytes, bufferBytes / (1024.0 * 1024.0));

    WGPUBufferDescriptor metaDesc = {};
    metaDesc.label = WGPU_STR("CardMetadataBuffer");
    metaDesc.size = metadataBytes;
    metaDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    metaDesc.mappedAtCreation = false;

    _metadataGpuBuffer = wgpuDeviceCreateBuffer(_device, &metaDesc);
    if (!_metadataGpuBuffer) {
        return Err("Failed to create metadata GPU buffer");
    }

    WGPUBufferDescriptor bufDesc = {};
    bufDesc.label = WGPU_STR("CardBuffer");
    bufDesc.size = bufferBytes;
    bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    bufDesc.mappedAtCreation = false;

    _bufferGpuBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);
    if (!_bufferGpuBuffer) {
        return Err("Failed to create buffer GPU buffer");
    }

    return Ok();
}

Result<void> CardBufferManagerImpl::createDummyResources() {
    // 1x1 RGBA8 texture as placeholder for atlas texture binding
    WGPUTextureDescriptor texDesc = {};
    texDesc.label = WGPU_STR("dummy atlas");
    texDesc.size = {1, 1, 1};
    texDesc.format = WGPUTextureFormat_RGBA8Unorm;
    texDesc.usage = WGPUTextureUsage_TextureBinding;
    texDesc.dimension = WGPUTextureDimension_2D;
    texDesc.mipLevelCount = 1;
    texDesc.sampleCount = 1;
    _dummyAtlasTexture = wgpuDeviceCreateTexture(_device, &texDesc);
    if (!_dummyAtlasTexture) {
        return Err<void>("Failed to create dummy atlas texture");
    }

    WGPUTextureViewDescriptor viewDesc = {};
    viewDesc.format = WGPUTextureFormat_RGBA8Unorm;
    viewDesc.dimension = WGPUTextureViewDimension_2D;
    viewDesc.mipLevelCount = 1;
    viewDesc.arrayLayerCount = 1;
    _dummyAtlasTextureView = wgpuTextureCreateView(_dummyAtlasTexture, &viewDesc);
    if (!_dummyAtlasTextureView) {
        return Err<void>("Failed to create dummy atlas texture view");
    }

    WGPUSamplerDescriptor samplerDesc = {};
    samplerDesc.magFilter = WGPUFilterMode_Linear;
    samplerDesc.minFilter = WGPUFilterMode_Linear;
    samplerDesc.maxAnisotropy = 1;
    _dummyAtlasSampler = wgpuDeviceCreateSampler(_device, &samplerDesc);
    if (!_dummyAtlasSampler) {
        return Err<void>("Failed to create dummy atlas sampler");
    }

    // Dummy buffer for binding 5 (was textureData, no longer used by shaders)
    WGPUBufferDescriptor dummyBufDesc = {};
    dummyBufDesc.label = WGPU_STR("DummyBinding5");
    dummyBufDesc.size = 4;
    dummyBufDesc.usage = WGPUBufferUsage_Storage;
    dummyBufDesc.mappedAtCreation = false;
    _dummyBuffer = wgpuDeviceCreateBuffer(_device, &dummyBufDesc);
    if (!_dummyBuffer) {
        return Err<void>("Failed to create dummy buffer");
    }

    return Ok();
}

Result<void> CardBufferManagerImpl::createSharedBindGroup() {
    if (_sharedBindGroup) {
        wgpuBindGroupRelease(_sharedBindGroup);
        _sharedBindGroup = nullptr;
    }

    if (!_sharedBindGroupLayout) {
        _sharedBindGroupLayout = _gpuContext->sharedBindGroupLayout;
        if (!_sharedBindGroupLayout) {
            return Err<void>("No shared bind group layout in GPUContext");
        }
    }

    WGPUTextureView texView = _atlasTextureView ? _atlasTextureView : _dummyAtlasTextureView;
    WGPUSampler sampler = _atlasSampler ? _atlasSampler : _dummyAtlasSampler;

    if (!texView || !sampler) {
        return Ok();
    }

    std::array<WGPUBindGroupEntry, 6> bindEntries = {};

    bindEntries[0].binding = 0;
    bindEntries[0].buffer = _uniformBuffer;
    bindEntries[0].size = _uniformSize;

    bindEntries[1].binding = 1;
    bindEntries[1].buffer = _metadataGpuBuffer;
    bindEntries[1].size = wgpuBufferGetSize(_metadataGpuBuffer);

    bindEntries[2].binding = 2;
    bindEntries[2].buffer = _bufferGpuBuffer;
    bindEntries[2].size = wgpuBufferGetSize(_bufferGpuBuffer);

    bindEntries[3].binding = 3;
    bindEntries[3].textureView = texView;

    bindEntries[4].binding = 4;
    bindEntries[4].sampler = sampler;

    // Binding 5: dummy buffer (was textureData, no longer needed)
    bindEntries[5].binding = 5;
    bindEntries[5].buffer = _dummyBuffer;
    bindEntries[5].size = 4;

    WGPUBindGroupDescriptor bindDesc = {};
    bindDesc.label = WGPU_STR("Shared Bind Group");
    bindDesc.layout = _sharedBindGroupLayout;
    bindDesc.entryCount = bindEntries.size();
    bindDesc.entries = bindEntries.data();

    _sharedBindGroup = wgpuDeviceCreateBindGroup(_device, &bindDesc);
    if (!_sharedBindGroup) {
        return Err<void>("Failed to create shared bind group");
    }

    _gpuContext->sharedBindGroup = _sharedBindGroup;

    yinfo("CardBufferManager: shared bind group created/updated");
    return Ok();
}

// =============================================================================
// Metadata operations
// =============================================================================

Result<MetadataHandle> CardBufferManagerImpl::allocateMetadata(uint32_t size) {
    return _metadataAllocator.allocate(size);
}

Result<void> CardBufferManagerImpl::deallocateMetadata(MetadataHandle handle) {
    return _metadataAllocator.deallocate(handle);
}

Result<void> CardBufferManagerImpl::writeMetadata(MetadataHandle handle, const void* data, uint32_t size) {
    return writeMetadataAt(handle, 0, data, size);
}

Result<void> CardBufferManagerImpl::writeMetadataAt(MetadataHandle handle, uint32_t offset,
                                                     const void* data, uint32_t size) {
    if (!handle.isValid()) {
        return Err("writeMetadataAt: invalid handle");
    }
    if (offset + size > handle.size) {
        return Err("writeMetadataAt: write exceeds slot size");
    }

    uint32_t bufferOffset = handle.offset + offset;
    uint8_t* bufferBytes = reinterpret_cast<uint8_t*>(_metadataCpuBuffer.data());
    std::memcpy(bufferBytes + bufferOffset, data, size);
    _metadataDirty.markDirty(bufferOffset, size);

    return Ok();
}

// =============================================================================
// Buffer operations (linear data for plot/draw cards)
// =============================================================================

Result<BufferHandle> CardBufferManagerImpl::allocateBuffer(uint32_t size) {
    auto result = _bufferAllocator.allocate(size);
    if (!result) {
        // Grow buffer to fit
        uint32_t requiredCapacity = _bufferAllocator.used() + size;
        uint32_t newCapacity = std::max(_config.bufferCapacity, _currentBufferCapacity * 2);
        while (newCapacity < requiredCapacity && newCapacity < MAX_BUFFER_SIZE) {
            newCapacity *= 2;
        }
        newCapacity = std::min(newCapacity, MAX_BUFFER_SIZE);
        if (newCapacity < requiredCapacity) {
            return Err<BufferHandle>("Buffer would exceed maximum size");
        }

        if (auto res = growBufferGpu(newCapacity); !res) {
            return Err<BufferHandle>("Failed to grow buffer", res);
        }

        result = _bufferAllocator.allocate(size);
        if (!result) {
            return result;
        }
    }
    BufferHandle handle = *result;
    uint8_t* bufferBytes = reinterpret_cast<uint8_t*>(_bufferCpuBuffer.data());
    handle.data = bufferBytes + handle.offset;
    return Ok(handle);
}

Result<void> CardBufferManagerImpl::deallocateBuffer(BufferHandle handle) {
    return _bufferAllocator.deallocate(handle);
}

Result<void> CardBufferManagerImpl::writeBuffer(BufferHandle handle, const void* data, uint32_t size) {
    return writeBufferAt(handle, 0, data, size);
}

Result<void> CardBufferManagerImpl::writeBufferAt(BufferHandle handle, uint32_t offset,
                                                    const void* data, uint32_t size) {
    if (!handle.isValid()) {
        return Err("writeBufferAt: invalid handle");
    }
    if (offset + size > handle.size) {
        return Err("writeBufferAt: write exceeds allocated size");
    }

    uint32_t bufferOffset = handle.offset + offset;
    uint8_t* bufferBytes = reinterpret_cast<uint8_t*>(_bufferCpuBuffer.data());
    std::memcpy(bufferBytes + bufferOffset, data, size);
    _bufferDirty.markDirty(bufferOffset, size);

    return Ok();
}

Result<BufferHandle> CardBufferManagerImpl::allocateBufferAndLink(MetadataHandle metaHandle,
                                                                    uint32_t metaFieldOffset,
                                                                    uint32_t bufferSize) {
    auto bufResult = allocateBuffer(bufferSize);
    if (!bufResult) {
        return Err("allocateBufferAndLink: buffer allocation failed");
    }

    BufferHandle buf = bufResult.value();

    auto writeResult = writeMetadataAt(metaHandle, metaFieldOffset, &buf.offset, sizeof(uint32_t));
    if (!writeResult) {
        deallocateBuffer(buf);
        return Err("allocateBufferAndLink: failed to write link");
    }

    return Ok(buf);
}

void CardBufferManagerImpl::markBufferDirty(BufferHandle handle) {
    if (handle.isValid()) {
        _bufferDirty.markDirty(handle.offset, handle.size);
    }
}

// =============================================================================
// Texture atlas operations
// =============================================================================

Result<TextureHandle> CardBufferManagerImpl::allocateTextureHandle() {
    uint32_t id = _nextTextureHandleId++;
    _textureHandles[id] = TextureHandleData{};
    ydebug("CardBufferManager: allocated texture handle id={}", id);
    return Ok(TextureHandle{id});
}

Result<void> CardBufferManagerImpl::linkTextureData(TextureHandle handle,
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

    ydebug("CardBufferManager: linked texture data handle={} {}x{}", handle.id, width, height);
    return Ok();
}

Result<void> CardBufferManagerImpl::deallocateTextureHandle(TextureHandle handle) {
    auto it = _textureHandles.find(handle.id);
    if (it == _textureHandles.end()) {
        return Err("deallocateTextureHandle: invalid handle");
    }

    _textureHandles.erase(it);
    ydebug("CardBufferManager: deallocated texture handle id={}", handle.id);
    return Ok();
}

void CardBufferManagerImpl::invalidateAllTextureHandles() {
    for (auto& [id, data] : _textureHandles) {
        data.packed = false;
    }
    yinfo("CardBufferManager: invalidated all {} texture handles", _textureHandles.size());
}

Result<void> CardBufferManagerImpl::packAtlas(WGPUQueue queue) {
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
    while (totalHeight > _currentAtlasSize && _currentAtlasSize < MAX_ATLAS_SIZE) {
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
        yerror("CardBufferManager: atlas overflow! need {}px height, have {}px", totalHeight, _currentAtlasSize);
        return Err<void>("Atlas overflow - too many/large textures");
    }

    yinfo("CardBufferManager: packed {} textures into atlas (used {}px of {}px height)",
          entries.size(), totalHeight, _currentAtlasSize);

    return Ok();
}

Result<void> CardBufferManagerImpl::writeToAtlas(TextureHandle handle, WGPUQueue queue) {
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

    ydebug("CardBufferManager: wrote {}x{} pixels to atlas at ({}, {})",
           data.width, data.height, data.atlasX, data.atlasY);

    return Ok();
}

Result<void> CardBufferManagerImpl::writeAllToAtlas(WGPUQueue queue) {
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

AtlasPosition CardBufferManagerImpl::getAtlasPosition(TextureHandle handle) const {
    auto it = _textureHandles.find(handle.id);
    if (it == _textureHandles.end() || !it->second.packed) {
        return {0, 0};
    }
    return {it->second.atlasX, it->second.atlasY};
}

// =============================================================================
// GPU upload
// =============================================================================

Result<void> CardBufferManagerImpl::flush(WGPUQueue queue) {
    const uint8_t* metadataBytes = reinterpret_cast<const uint8_t*>(_metadataCpuBuffer.data());
    const uint8_t* bufferBytes = reinterpret_cast<const uint8_t*>(_bufferCpuBuffer.data());

    // Upload dirty metadata regions
    if (_metadataDirty.hasDirty()) {
        auto ranges = _metadataDirty.getCoalescedRanges();
        for (const auto& [offset, size] : ranges) {
            ydebug("CardBufMgr::flush: metadata upload offset={} size={}", offset, size);
            wgpuQueueWriteBuffer(queue, _metadataGpuBuffer, offset,
                                 metadataBytes + offset, size);
        }
        _metadataDirty.clear();
    }

    // Upload dirty buffer regions
    if (_bufferDirty.hasDirty()) {
        auto ranges = _bufferDirty.getCoalescedRanges();
        for (const auto& [offset, size] : ranges) {
            ydebug("CardBufMgr::flush: buffer upload offset={} size={}", offset, size);
            wgpuQueueWriteBuffer(queue, _bufferGpuBuffer, offset,
                                 bufferBytes + offset, size);
        }
        _bufferDirty.clear();
    }

    // Atlas packing and writing is now driven by gpu-screen (two-pass rendering),
    // not by flush(). flush() only uploads metadata and buffer data.

    return Ok();
}

CardBufferManager::Stats CardBufferManagerImpl::getStats() const {
    return Stats{
        .metadataUsed = _metadataAllocator.highWaterMark(),
        .metadataCapacity = _metadataAllocator.totalSize(),
        .bufferUsed = _bufferAllocator.used(),
        .bufferCapacity = _bufferAllocator.capacity(),
        .atlasTextureCount = static_cast<uint32_t>(_textureHandles.size()),
        .atlasWidth = _currentAtlasSize,
        .atlasHeight = _currentAtlasSize,
        .pendingMetadataUploads = _metadataDirty.hasDirty() ? 1u : 0u,
        .pendingBufferUploads = _bufferDirty.hasDirty() ? 1u : 0u
    };
}

// =============================================================================
// Atlas texture creation
// =============================================================================

Result<void> CardBufferManagerImpl::initAtlas() {
    if (auto res = createAtlasTexture(); !res) {
        return res;
    }

    _atlasInitialized = true;
    yinfo("CardBufferManager: atlas initialized ({}x{})", _currentAtlasSize, _currentAtlasSize);

    if (auto res = createSharedBindGroup(); !res) {
        return res;
    }

    return Ok();
}

Result<void> CardBufferManagerImpl::createAtlasTexture() {
    size_t atlasBytes = static_cast<size_t>(_currentAtlasSize) * _currentAtlasSize * 4;
    yinfo("GPU_ALLOC CardBufferManager: atlasTexture={}x{} RGBA8 = {} bytes ({:.2f} MB)",
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

Result<void> CardBufferManagerImpl::growAtlas() {
    uint32_t newSize = _currentAtlasSize * 2;
    if (newSize > MAX_ATLAS_SIZE) {
        yerror("CardBufferManager: cannot grow atlas beyond {} (current: {})", MAX_ATLAS_SIZE, _currentAtlasSize);
        return Err<void>("Atlas size limit reached");
    }

    yinfo("CardBufferManager: growing atlas from {}x{} to {}x{}",
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

    if (auto res = createSharedBindGroup(); !res) {
        return res;
    }

    yinfo("CardBufferManager: atlas grown successfully to {}x{}", _currentAtlasSize, _currentAtlasSize);
    return Ok();
}

Result<void> CardBufferManagerImpl::growBufferGpu(uint32_t newCapacity) {
    if (newCapacity <= _currentBufferCapacity) {
        return Ok();
    }

    yinfo("CardBufferManager: growing buffer from {} to {} bytes ({:.2f} MB to {:.2f} MB)",
          _currentBufferCapacity, newCapacity,
          _currentBufferCapacity / (1024.0 * 1024.0),
          newCapacity / (1024.0 * 1024.0));

    uint32_t newU32Count = (newCapacity + 3) / 4;
    _bufferCpuBuffer.resize(newU32Count, 0);

    _bufferAllocator.grow(newCapacity);

    if (_bufferGpuBuffer) {
        wgpuBufferRelease(_bufferGpuBuffer);
        _bufferGpuBuffer = nullptr;
    }

    WGPUBufferDescriptor bufDesc = {};
    bufDesc.label = WGPU_STR("CardBuffer");
    bufDesc.size = newU32Count * sizeof(uint32_t);
    bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    bufDesc.mappedAtCreation = false;

    _bufferGpuBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);
    if (!_bufferGpuBuffer) {
        return Err<void>("Failed to create larger buffer GPU buffer");
    }

    uint32_t usedBytes = _bufferAllocator.highWaterMark();
    if (usedBytes > 0) {
        _bufferDirty.markDirty(0, usedBytes);
    }

    _currentBufferCapacity = newCapacity;

    if (auto res = createSharedBindGroup(); !res) {
        return Err<void>("Failed to recreate shared bind group after buffer growth", res);
    }

    yinfo("GPU_ALLOC CardBufferManager: buffer grown to {} bytes ({:.2f} MB)",
          newCapacity, newCapacity / (1024.0 * 1024.0));
    return Ok();
}

}  // namespace yetty
