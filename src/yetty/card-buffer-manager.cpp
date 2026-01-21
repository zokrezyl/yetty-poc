#include "card-buffer-manager.h"
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <sstream>
#include <array>

#ifndef CMAKE_SOURCE_DIR
#define CMAKE_SOURCE_DIR "."
#endif

// WebGPU string helper
#define WGPU_STR(s) WGPUStringView{.data = s, .length = WGPU_STRLEN}

namespace yetty {

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

    return Ok(MetadataHandle{result.value(), pool->slotSize()});
}

Result<void> MetadataAllocator::deallocate(MetadataHandle handle) {
    MetadataPool* pool = findPoolBySlotSize(handle.size);
    if (!pool) {
        return Err("MetadataAllocator: invalid handle size");
    }
    return pool->deallocate(handle.offset);
}

// --- StorageAllocator ---

StorageAllocator::StorageAllocator(uint32_t capacity)
    : _capacity(capacity)
    , _used(0) {
    _freeBlocks.push_back({0, capacity});
}

Result<StorageHandle> StorageAllocator::allocate(uint32_t size) {
    if (size == 0) {
        return Err("StorageAllocator: cannot allocate zero bytes");
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
            return Ok(StorageHandle{offset, size});
        }
    }

    return Err("StorageAllocator: out of memory");
}

Result<void> StorageAllocator::deallocate(StorageHandle handle) {
    if (!handle.isValid()) {
        return Err("StorageAllocator: invalid handle");
    }

    // Insert into sorted position
    auto it = std::lower_bound(_freeBlocks.begin(), _freeBlocks.end(), handle.offset,
        [](const FreeBlock& block, uint32_t offset) {
            return block.offset < offset;
        });

    _freeBlocks.insert(it, {handle.offset, handle.size});
    _used -= handle.size;

    mergeFreeBlocks();
    return Ok();
}

void StorageAllocator::mergeFreeBlocks() {
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

    // Sort by offset
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

// --- CardBufferManager ---

CardBufferManager::CardBufferManager(WGPUDevice device, Config config)
    : _device(device)
    , _config(config)
    , _metadataGpuBuffer(nullptr)
    , _storageGpuBuffer(nullptr)
    , _imageDataGpuBuffer(nullptr)
    , _metadataAllocator(config.metadataPool32Count, config.metadataPool64Count,
                         config.metadataPool128Count, config.metadataPool256Count)
    , _storageAllocator(config.storageCapacity)
    , _imageDataAllocator(config.imageDataCapacity) {

    // Resize to byte count / 4 (buffers are uint32_t for alignment)
    _metadataCpuBuffer.resize((_metadataAllocator.totalSize() + 3) / 4, 0);
    _storageCpuBuffer.resize((config.storageCapacity + 3) / 4, 0);
    _imageDataCpuBuffer.resize((config.imageDataCapacity + 3) / 4, 0);

    auto result = createGpuBuffers();
    if (!result) {
        // Log error but don't throw - buffers will be null
    }
}

CardBufferManager::~CardBufferManager() {
    if (_metadataGpuBuffer) {
        wgpuBufferRelease(_metadataGpuBuffer);
    }
    if (_storageGpuBuffer) {
        wgpuBufferRelease(_storageGpuBuffer);
    }
    if (_imageDataGpuBuffer) {
        wgpuBufferRelease(_imageDataGpuBuffer);
    }

    // Release atlas resources
    if (_atlasBindGroup) wgpuBindGroupRelease(_atlasBindGroup);
    if (_atlasBindGroupLayout) wgpuBindGroupLayoutRelease(_atlasBindGroupLayout);
    if (_scanPipeline) wgpuComputePipelineRelease(_scanPipeline);
    if (_copyPipeline) wgpuComputePipelineRelease(_copyPipeline);
    if (_atlasShaderModule) wgpuShaderModuleRelease(_atlasShaderModule);
    if (_processedCardsBuffer) wgpuBufferRelease(_processedCardsBuffer);
    if (_atlasStateBuffer) wgpuBufferRelease(_atlasStateBuffer);
    if (_atlasSampler) wgpuSamplerRelease(_atlasSampler);
    if (_atlasTextureView) wgpuTextureViewRelease(_atlasTextureView);
    if (_atlasTexture) wgpuTextureRelease(_atlasTexture);
}

Result<void> CardBufferManager::createGpuBuffers() {
    // GPU buffer sizes in bytes (CPU buffers are uint32_t vectors)
    size_t metadataBytes = _metadataCpuBuffer.size() * sizeof(uint32_t);
    size_t storageBytes = _storageCpuBuffer.size() * sizeof(uint32_t);
    size_t imageDataBytes = _imageDataCpuBuffer.size() * sizeof(uint32_t);

    WGPUBufferDescriptor metaDesc = {};
    metaDesc.label = {.data = "CardMetadataBuffer", .length = WGPU_STRLEN};
    metaDesc.size = metadataBytes;
    metaDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    metaDesc.mappedAtCreation = false;

    _metadataGpuBuffer = wgpuDeviceCreateBuffer(_device, &metaDesc);
    if (!_metadataGpuBuffer) {
        return Err("Failed to create metadata GPU buffer");
    }

    WGPUBufferDescriptor storageDesc = {};
    storageDesc.label = {.data = "CardStorageBuffer", .length = WGPU_STRLEN};
    storageDesc.size = storageBytes;
    storageDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    storageDesc.mappedAtCreation = false;

    _storageGpuBuffer = wgpuDeviceCreateBuffer(_device, &storageDesc);
    if (!_storageGpuBuffer) {
        return Err("Failed to create storage GPU buffer");
    }

    WGPUBufferDescriptor imageDataDesc = {};
    imageDataDesc.label = {.data = "CardImageDataBuffer", .length = WGPU_STRLEN};
    imageDataDesc.size = imageDataBytes;
    imageDataDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    imageDataDesc.mappedAtCreation = false;

    _imageDataGpuBuffer = wgpuDeviceCreateBuffer(_device, &imageDataDesc);
    if (!_imageDataGpuBuffer) {
        return Err("Failed to create image data GPU buffer");
    }

    return Ok();
}

Result<MetadataHandle> CardBufferManager::allocateMetadata(uint32_t size) {
    return _metadataAllocator.allocate(size);
}

Result<void> CardBufferManager::deallocateMetadata(MetadataHandle handle) {
    return _metadataAllocator.deallocate(handle);
}

Result<void> CardBufferManager::writeMetadata(MetadataHandle handle, const void* data, uint32_t size) {
    return writeMetadataAt(handle, 0, data, size);
}

Result<void> CardBufferManager::writeMetadataAt(MetadataHandle handle, uint32_t offset,
                                                 const void* data, uint32_t size) {
    if (!handle.isValid()) {
        return Err("writeMetadataAt: invalid handle");
    }
    if (offset + size > handle.size) {
        return Err("writeMetadataAt: write exceeds slot size");
    }

    uint32_t bufferOffset = handle.offset + offset;
    // Cast to uint8_t* for byte-based offset arithmetic
    uint8_t* bufferBytes = reinterpret_cast<uint8_t*>(_metadataCpuBuffer.data());
    std::memcpy(bufferBytes + bufferOffset, data, size);
    _metadataDirty.markDirty(bufferOffset, size);

    return Ok();
}

Result<StorageHandle> CardBufferManager::allocateStorage(uint32_t size) {
    return _storageAllocator.allocate(size);
}

Result<void> CardBufferManager::deallocateStorage(StorageHandle handle) {
    return _storageAllocator.deallocate(handle);
}

Result<void> CardBufferManager::writeStorage(StorageHandle handle, const void* data, uint32_t size) {
    return writeStorageAt(handle, 0, data, size);
}

Result<void> CardBufferManager::writeStorageAt(StorageHandle handle, uint32_t offset,
                                                const void* data, uint32_t size) {
    if (!handle.isValid()) {
        return Err("writeStorageAt: invalid handle");
    }
    if (offset + size > handle.size) {
        return Err("writeStorageAt: write exceeds allocated size");
    }

    uint32_t bufferOffset = handle.offset + offset;
    // Cast to uint8_t* for byte-based offset arithmetic
    uint8_t* bufferBytes = reinterpret_cast<uint8_t*>(_storageCpuBuffer.data());
    std::memcpy(bufferBytes + bufferOffset, data, size);
    _storageDirty.markDirty(bufferOffset, size);

    return Ok();
}

Result<StorageHandle> CardBufferManager::allocateStorageAndLink(MetadataHandle metaHandle,
                                                                 uint32_t metaFieldOffset,
                                                                 uint32_t storageSize) {
    auto storageResult = allocateStorage(storageSize);
    if (!storageResult) {
        return Err("allocateStorageAndLink: storage allocation failed");
    }

    StorageHandle storage = storageResult.value();

    // Write storage offset to metadata field
    auto writeResult = writeMetadataAt(metaHandle, metaFieldOffset, &storage.offset, sizeof(uint32_t));
    if (!writeResult) {
        deallocateStorage(storage);
        return Err("allocateStorageAndLink: failed to write link");
    }

    return Ok(storage);
}

Result<ImageDataHandle> CardBufferManager::allocateImageData(uint32_t size) {
    return _imageDataAllocator.allocate(size);
}

Result<void> CardBufferManager::deallocateImageData(ImageDataHandle handle) {
    return _imageDataAllocator.deallocate(handle);
}

Result<void> CardBufferManager::writeImageData(ImageDataHandle handle, const void* data, uint32_t size) {
    return writeImageDataAt(handle, 0, data, size);
}

Result<void> CardBufferManager::writeImageDataAt(ImageDataHandle handle, uint32_t offset,
                                                  const void* data, uint32_t size) {
    if (!handle.isValid()) {
        return Err("writeImageDataAt: invalid handle");
    }
    if (offset + size > handle.size) {
        return Err("writeImageDataAt: write exceeds allocated size");
    }

    uint32_t bufferOffset = handle.offset + offset;
    // Cast to uint8_t* for byte-based offset arithmetic
    uint8_t* bufferBytes = reinterpret_cast<uint8_t*>(_imageDataCpuBuffer.data());
    std::memcpy(bufferBytes + bufferOffset, data, size);
    _imageDataDirty.markDirty(bufferOffset, size);

    return Ok();
}

Result<void> CardBufferManager::flush(WGPUQueue queue) {
    // Cast to uint8_t* for byte-based offset arithmetic
    const uint8_t* metadataBytes = reinterpret_cast<const uint8_t*>(_metadataCpuBuffer.data());
    const uint8_t* storageBytes = reinterpret_cast<const uint8_t*>(_storageCpuBuffer.data());
    const uint8_t* imageDataBytes = reinterpret_cast<const uint8_t*>(_imageDataCpuBuffer.data());

    // Upload dirty metadata regions
    if (_metadataDirty.hasDirty()) {
        auto ranges = _metadataDirty.getCoalescedRanges();
        for (const auto& [offset, size] : ranges) {
            yinfo("DEBUG flush: uploading metadata offset={} size={}", offset, size);
            wgpuQueueWriteBuffer(queue, _metadataGpuBuffer, offset,
                                 metadataBytes + offset, size);
        }
        _metadataDirty.clear();
    }

    // Upload dirty storage regions
    if (_storageDirty.hasDirty()) {
        auto ranges = _storageDirty.getCoalescedRanges();
        for (const auto& [offset, size] : ranges) {
            wgpuQueueWriteBuffer(queue, _storageGpuBuffer, offset,
                                 storageBytes + offset, size);
        }
        _storageDirty.clear();
    }

    // Upload dirty image data regions
    if (_imageDataDirty.hasDirty()) {
        auto ranges = _imageDataDirty.getCoalescedRanges();
        yinfo("DEBUG flush: uploading {} imageData ranges", ranges.size());
        for (const auto& [offset, size] : ranges) {
            yinfo("DEBUG flush: uploading imageData offset={} size={}", offset, size);
            wgpuQueueWriteBuffer(queue, _imageDataGpuBuffer, offset,
                                 imageDataBytes + offset, size);
        }
        _imageDataDirty.clear();
    } else {
        static bool loggedOnce = false;
        if (!loggedOnce) {
            yinfo("DEBUG flush: no dirty imageData");
            loggedOnce = false; // Allow repeated logging for debugging
        }
    }

    return Ok();
}

CardBufferManager::Stats CardBufferManager::getStats() const {
    return Stats{
        .metadataUsed = _metadataAllocator.totalSize() - 0,  // TODO: track properly
        .metadataCapacity = _metadataAllocator.totalSize(),
        .storageUsed = _storageAllocator.used(),
        .storageCapacity = _storageAllocator.capacity(),
        .imageDataUsed = _imageDataAllocator.used(),
        .imageDataCapacity = _imageDataAllocator.capacity(),
        .pendingMetadataUploads = _metadataDirty.hasDirty() ? 1u : 0u,
        .pendingStorageUploads = _storageDirty.hasDirty() ? 1u : 0u,
        .pendingImageDataUploads = _imageDataDirty.hasDirty() ? 1u : 0u
    };
}

// =============================================================================
// Image Atlas Implementation
// =============================================================================

Result<void> CardBufferManager::initAtlas() {
    if (auto res = createAtlasTexture(); !res) {
        return res;
    }
    if (auto res = createAtlasComputePipeline(); !res) {
        return res;
    }

    _atlasInitialized = true;
    yinfo("CardBufferManager: atlas initialized ({}x{})", IMAGE_ATLAS_SIZE, IMAGE_ATLAS_SIZE);
    return Ok();
}

Result<void> CardBufferManager::createAtlasTexture() {
    yinfo("Creating atlas texture {}x{}", IMAGE_ATLAS_SIZE, IMAGE_ATLAS_SIZE);

    WGPUTextureDescriptor texDesc = {};
    texDesc.label = WGPU_STR("ImageAtlas");
    texDesc.size.width = IMAGE_ATLAS_SIZE;
    texDesc.size.height = IMAGE_ATLAS_SIZE;
    texDesc.size.depthOrArrayLayers = 1;
    texDesc.mipLevelCount = 1;
    texDesc.sampleCount = 1;
    texDesc.dimension = WGPUTextureDimension_2D;
    texDesc.format = WGPUTextureFormat_RGBA8Unorm;
    texDesc.usage = WGPUTextureUsage_StorageBinding |  // Compute shader writes
                    WGPUTextureUsage_TextureBinding |  // Fragment shader reads
                    WGPUTextureUsage_CopyDst;          // For clearing

    _atlasTexture = wgpuDeviceCreateTexture(_device, &texDesc);
    if (!_atlasTexture) {
        yerror("Failed to create atlas texture");
        return Err<void>("Failed to create atlas texture");
    }
    yinfo("Atlas texture created: {}", (void*)_atlasTexture);

    WGPUTextureViewDescriptor viewDesc = {};
    viewDesc.format = WGPUTextureFormat_RGBA8Unorm;
    viewDesc.dimension = WGPUTextureViewDimension_2D;
    viewDesc.mipLevelCount = 1;
    viewDesc.arrayLayerCount = 1;

    _atlasTextureView = wgpuTextureCreateView(_atlasTexture, &viewDesc);
    if (!_atlasTextureView) {
        yerror("Failed to create atlas texture view");
        return Err<void>("Failed to create atlas texture view");
    }
    yinfo("Atlas texture view created: {}", (void*)_atlasTextureView);

    WGPUSamplerDescriptor samplerDesc = {};
    samplerDesc.minFilter = WGPUFilterMode_Linear;
    samplerDesc.magFilter = WGPUFilterMode_Linear;
    samplerDesc.addressModeU = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeV = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeW = WGPUAddressMode_ClampToEdge;
    samplerDesc.maxAnisotropy = 1;

    _atlasSampler = wgpuDeviceCreateSampler(_device, &samplerDesc);
    if (!_atlasSampler) {
        yerror("Failed to create atlas sampler");
        return Err<void>("Failed to create atlas sampler");
    }
    yinfo("Atlas sampler created: {}", (void*)_atlasSampler);

    // Create atlas state buffer
    WGPUBufferDescriptor stateDesc = {};
    stateDesc.label = WGPU_STR("AtlasState");
    stateDesc.size = sizeof(AtlasState);
    stateDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    stateDesc.mappedAtCreation = false;

    _atlasStateBuffer = wgpuDeviceCreateBuffer(_device, &stateDesc);
    if (!_atlasStateBuffer) {
        return Err<void>("Failed to create atlas state buffer");
    }

    // Create processed cards buffer
    WGPUBufferDescriptor processedDesc = {};
    processedDesc.label = WGPU_STR("ProcessedCards");
    processedDesc.size = MAX_CARD_SLOTS * sizeof(uint32_t);
    processedDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    processedDesc.mappedAtCreation = false;

    _processedCardsBuffer = wgpuDeviceCreateBuffer(_device, &processedDesc);
    if (!_processedCardsBuffer) {
        return Err<void>("Failed to create processed cards buffer");
    }

    return Ok();
}

Result<void> CardBufferManager::createAtlasComputePipeline() {
    // Load shader source
    const char* shaderPath = CMAKE_SOURCE_DIR "/src/yetty/shaders/image-atlas-copy.wgsl";
    yinfo("Loading compute shader from: {}", shaderPath);
    std::ifstream file(shaderPath);
    if (!file.is_open()) {
        yerror("Failed to open compute shader: {}", shaderPath);
        return Err<void>(std::string("Failed to open compute shader: ") + shaderPath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string shaderSource = buffer.str();

    // Create shader module
    WGPUShaderSourceWGSL wgslDesc = {};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslDesc.code = WGPU_STR(shaderSource.c_str());

    WGPUShaderModuleDescriptor moduleDesc = {};
    moduleDesc.nextInChain = &wgslDesc.chain;
    moduleDesc.label = WGPU_STR("ImageAtlasCopyShader");

    _atlasShaderModule = wgpuDeviceCreateShaderModule(_device, &moduleDesc);
    if (!_atlasShaderModule) {
        yerror("Failed to create compute shader module");
        return Err<void>("Failed to create compute shader module");
    }
    yinfo("Compute shader module created");

    // Create bind group layout
    // Binding 0: cellBuffer (read-only storage)
    // Binding 1: cardMetadata (read-write storage)
    // Binding 2: cardImageData (read-only storage)
    // Binding 3: atlasTexture (storage texture, write)
    // Binding 4: atlasState (read-write storage)
    // Binding 5: processedCards (read-write storage)
    std::array<WGPUBindGroupLayoutEntry, 6> layoutEntries = {};

    layoutEntries[0].binding = 0;
    layoutEntries[0].visibility = WGPUShaderStage_Compute;
    layoutEntries[0].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    layoutEntries[1].binding = 1;
    layoutEntries[1].visibility = WGPUShaderStage_Compute;
    layoutEntries[1].buffer.type = WGPUBufferBindingType_Storage;  // read-write

    layoutEntries[2].binding = 2;
    layoutEntries[2].visibility = WGPUShaderStage_Compute;
    layoutEntries[2].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    layoutEntries[3].binding = 3;
    layoutEntries[3].visibility = WGPUShaderStage_Compute;
    layoutEntries[3].storageTexture.access = WGPUStorageTextureAccess_WriteOnly;
    layoutEntries[3].storageTexture.format = WGPUTextureFormat_RGBA8Unorm;
    layoutEntries[3].storageTexture.viewDimension = WGPUTextureViewDimension_2D;

    layoutEntries[4].binding = 4;
    layoutEntries[4].visibility = WGPUShaderStage_Compute;
    layoutEntries[4].buffer.type = WGPUBufferBindingType_Storage;

    layoutEntries[5].binding = 5;
    layoutEntries[5].visibility = WGPUShaderStage_Compute;
    layoutEntries[5].buffer.type = WGPUBufferBindingType_Storage;

    WGPUBindGroupLayoutDescriptor layoutDesc = {};
    layoutDesc.label = WGPU_STR("ImageAtlasComputeLayout");
    layoutDesc.entryCount = layoutEntries.size();
    layoutDesc.entries = layoutEntries.data();

    _atlasBindGroupLayout = wgpuDeviceCreateBindGroupLayout(_device, &layoutDesc);
    if (!_atlasBindGroupLayout) {
        yerror("Failed to create compute bind group layout");
        return Err<void>("Failed to create compute bind group layout");
    }
    yinfo("Compute bind group layout created");

    // Create pipeline layout
    WGPUPipelineLayoutDescriptor pipelineLayoutDesc = {};
    pipelineLayoutDesc.bindGroupLayoutCount = 1;
    pipelineLayoutDesc.bindGroupLayouts = &_atlasBindGroupLayout;

    WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(_device, &pipelineLayoutDesc);
    if (!pipelineLayout) {
        return Err<void>("Failed to create compute pipeline layout");
    }

    // Create scan pipeline (Phase 1)
    WGPUComputePipelineDescriptor scanPipelineDesc = {};
    scanPipelineDesc.label = WGPU_STR("ImageAtlasScanPipeline");
    scanPipelineDesc.layout = pipelineLayout;
    scanPipelineDesc.compute.module = _atlasShaderModule;
    scanPipelineDesc.compute.entryPoint = WGPU_STR("scanAndAllocate");

    _scanPipeline = wgpuDeviceCreateComputePipeline(_device, &scanPipelineDesc);
    if (!_scanPipeline) {
        wgpuPipelineLayoutRelease(pipelineLayout);
        return Err<void>("Failed to create scan compute pipeline");
    }

    // Create copy pipeline (Phase 2)
    WGPUComputePipelineDescriptor copyPipelineDesc = {};
    copyPipelineDesc.label = WGPU_STR("ImageAtlasCopyPipeline");
    copyPipelineDesc.layout = pipelineLayout;
    copyPipelineDesc.compute.module = _atlasShaderModule;
    copyPipelineDesc.compute.entryPoint = WGPU_STR("copyPixels");

    _copyPipeline = wgpuDeviceCreateComputePipeline(_device, &copyPipelineDesc);
    if (!_copyPipeline) {
        wgpuPipelineLayoutRelease(pipelineLayout);
        return Err<void>("Failed to create copy compute pipeline");
    }

    wgpuPipelineLayoutRelease(pipelineLayout);

    yinfo("CardBufferManager: compute pipelines created");
    return Ok();
}

Result<void> CardBufferManager::prepareAtlas(WGPUCommandEncoder encoder, WGPUQueue queue,
                                              WGPUBuffer cellBuffer,
                                              uint32_t gridCols, uint32_t gridRows) {
    if (!_atlasInitialized) {
        return Err<void>("Atlas not initialized");
    }

    // DEBUG: Dump metadata for slot 256 (where image card is allocated)
    const uint32_t* metaU32 = _metadataCpuBuffer.data();
    uint32_t slot = 256;
    uint32_t offset = slot * 8;  // 32 bytes = 8 u32s per slot index
    if (offset + 5 < _metadataCpuBuffer.size()) {
        uint32_t imageDataOffset = metaU32[offset + 0];
        uint32_t imageWidth = metaU32[offset + 1];
        uint32_t imageHeight = metaU32[offset + 2];
        uint32_t atlasX = metaU32[offset + 3];
        uint32_t atlasY = metaU32[offset + 4];
        if (imageWidth > 0 && imageHeight > 0) {
            yinfo("DEBUG slot[256]: dataOffset={} size={}x{} atlasPos=({},{}) bufSize={}",
                  imageDataOffset, imageWidth, imageHeight, atlasX, atlasY,
                  _metadataCpuBuffer.size() * sizeof(uint32_t));
        }
    }

    // Reset atlas state for this frame
    AtlasState initialState = {};
    initialState.nextX = 0;
    initialState.nextY = 0;
    initialState.rowHeight = 0;
    initialState.atlasWidth = IMAGE_ATLAS_SIZE;
    initialState.atlasHeight = IMAGE_ATLAS_SIZE;
    initialState.gridCols = gridCols;
    initialState.gridRows = gridRows;
    initialState._padding = 0;

    wgpuQueueWriteBuffer(queue, _atlasStateBuffer, 0, &initialState, sizeof(initialState));

    // Clear processed cards buffer
    std::vector<uint32_t> zeros(MAX_CARD_SLOTS, 0);
    wgpuQueueWriteBuffer(queue, _processedCardsBuffer, 0, zeros.data(), zeros.size() * sizeof(uint32_t));

    // Create bind group with current buffers
    std::array<WGPUBindGroupEntry, 6> bindEntries = {};

    // Check cellBuffer size
    uint64_t cellBufferSize = wgpuBufferGetSize(cellBuffer);
    uint64_t expectedCellBufferSize = static_cast<uint64_t>(gridCols) * gridRows * 12;
    yinfo("prepareAtlas: cellBuffer size={} expected={}", cellBufferSize, expectedCellBufferSize);
    if (cellBufferSize < expectedCellBufferSize) {
        yerror("prepareAtlas: cellBuffer too small! size={} expected={}", cellBufferSize, expectedCellBufferSize);
    }

    bindEntries[0].binding = 0;
    bindEntries[0].buffer = cellBuffer;
    bindEntries[0].offset = 0;
    bindEntries[0].size = gridCols * gridRows * 12;  // 12 bytes per cell (3 u32s)

    bindEntries[1].binding = 1;
    bindEntries[1].buffer = _metadataGpuBuffer;
    bindEntries[1].offset = 0;
    bindEntries[1].size = _metadataCpuBuffer.size() * sizeof(uint32_t);  // Convert to bytes
    yinfo("prepareAtlas: compute bind group using metadataBuffer={}", (void*)_metadataGpuBuffer);

    bindEntries[2].binding = 2;
    bindEntries[2].buffer = _imageDataGpuBuffer;
    bindEntries[2].offset = 0;
    bindEntries[2].size = _imageDataCpuBuffer.size() * sizeof(uint32_t);  // Convert to bytes

    bindEntries[3].binding = 3;
    bindEntries[3].textureView = _atlasTextureView;

    bindEntries[4].binding = 4;
    bindEntries[4].buffer = _atlasStateBuffer;
    bindEntries[4].offset = 0;
    bindEntries[4].size = sizeof(AtlasState);

    bindEntries[5].binding = 5;
    bindEntries[5].buffer = _processedCardsBuffer;
    bindEntries[5].offset = 0;
    bindEntries[5].size = MAX_CARD_SLOTS * sizeof(uint32_t);

    WGPUBindGroupDescriptor bindGroupDesc = {};
    bindGroupDesc.label = WGPU_STR("ImageAtlasComputeBindGroup");
    bindGroupDesc.layout = _atlasBindGroupLayout;
    bindGroupDesc.entryCount = bindEntries.size();
    bindGroupDesc.entries = bindEntries.data();

    // Release previous bind group if exists
    if (_atlasBindGroup) {
        wgpuBindGroupRelease(_atlasBindGroup);
    }
    _atlasBindGroup = wgpuDeviceCreateBindGroup(_device, &bindGroupDesc);
    if (!_atlasBindGroup) {
        return Err<void>("Failed to create compute bind group");
    }

    // Begin compute pass
    WGPUComputePassDescriptor computePassDesc = {};
    computePassDesc.label = WGPU_STR("ImageAtlasComputePass");

    WGPUComputePassEncoder computePass = wgpuCommandEncoderBeginComputePass(encoder, &computePassDesc);
    if (!computePass) {
        return Err<void>("Failed to begin compute pass");
    }

    // Phase 1: Scan cells and allocate atlas positions
    if (!_scanPipeline) {
        yerror("prepareAtlas: _scanPipeline is NULL!");
        wgpuComputePassEncoderEnd(computePass);
        wgpuComputePassEncoderRelease(computePass);
        return Err<void>("_scanPipeline is NULL");
    }

    wgpuComputePassEncoderSetPipeline(computePass, _scanPipeline);
    wgpuComputePassEncoderSetBindGroup(computePass, 0, _atlasBindGroup, 0, nullptr);

    uint32_t totalCells = gridCols * gridRows;
    uint32_t workgroupsX = (totalCells + 255) / 256;
    yinfo("prepareAtlas: dispatching scanAndAllocate with {} workgroups ({} cells, {}x{})",
          workgroupsX, totalCells, gridCols, gridRows);
    wgpuComputePassEncoderDispatchWorkgroups(computePass, workgroupsX, 1, 1);

    // Phase 2: Copy pixels to atlas
    wgpuComputePassEncoderSetPipeline(computePass, _copyPipeline);
    // Note: bind group is already set

    // Dispatch workgroups: (imageWidth/16, imageHeight/16, numSlots)
    // Slots 256+ are for 64-byte metadata (pool64 starts at byte 8192 = slot 256)
    // So we need to dispatch for slots 0-511 to cover pool32 and pool64
    constexpr uint32_t MAX_VISIBLE_CARDS = 512;
    uint32_t maxImageDim = 512;  // Max image dimension we handle per dispatch
    uint32_t workgroupsPerImage = (maxImageDim + 15) / 16;  // 32 workgroups
    wgpuComputePassEncoderDispatchWorkgroups(computePass, workgroupsPerImage, workgroupsPerImage, MAX_VISIBLE_CARDS);

    wgpuComputePassEncoderEnd(computePass);
    wgpuComputePassEncoderRelease(computePass);

    return Ok();
}

}  // namespace yetty
