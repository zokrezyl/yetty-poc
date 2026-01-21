#pragma once

#include <yetty/result.hpp>
#include <cstdint>
#include <vector>
#include <webgpu/webgpu.h>

namespace yetty {

// Constants for image atlas
constexpr uint32_t IMAGE_ATLAS_SIZE = 4096;       // 4096x4096 = 64MB RGBA8
constexpr uint32_t MAX_CARD_SLOTS = 16384;        // Max unique card slots to track

// Handle types for type safety
struct MetadataHandle {
    uint32_t offset;
    uint32_t size;

    bool isValid() const { return size > 0; }
    static MetadataHandle invalid() { return {0, 0}; }
};

struct StorageHandle {
    uint32_t offset;
    uint32_t size;

    bool isValid() const { return size > 0; }
    static StorageHandle invalid() { return {0, 0}; }
};

// Alias for image data (same structure as StorageHandle)
using ImageDataHandle = StorageHandle;

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

private:
    MetadataPool* findPool(uint32_t size);
    MetadataPool* findPoolBySlotSize(uint32_t slotSize);

    MetadataPool _pool32;
    MetadataPool _pool64;
    MetadataPool _pool128;
    MetadataPool _pool256;
    uint32_t _totalSize;
};

// Free-list allocator for variable-size storage
class StorageAllocator {
public:
    explicit StorageAllocator(uint32_t capacity);

    Result<StorageHandle> allocate(uint32_t size);
    Result<void> deallocate(StorageHandle handle);

    uint32_t capacity() const { return _capacity; }
    uint32_t used() const { return _used; }
    uint32_t fragmentCount() const { return static_cast<uint32_t>(_freeBlocks.size()); }

private:
    struct FreeBlock {
        uint32_t offset;
        uint32_t size;
    };

    void mergeFreeBlocks();

    uint32_t _capacity;
    uint32_t _used;
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

// Configuration for CardBufferManager
struct CardBufferConfig {
    uint32_t metadataPool32Count = 0;    // No 32-byte slots needed currently
    uint32_t metadataPool64Count = 256;  // ImageCard uses 64-byte metadata
    uint32_t metadataPool128Count = 64;
    uint32_t metadataPool256Count = 32;
    uint32_t storageCapacity = 16 * 1024 * 1024;      // For plot data (array<f32>)
    uint32_t imageDataCapacity = 256 * 1024 * 1024;   // For image pixels (array<u32> RGBA8) - scrollback storage
};

// Main buffer manager
class CardBufferManager {
public:
    using Config = CardBufferConfig;

    CardBufferManager(WGPUDevice device, Config config = {});
    ~CardBufferManager();

    CardBufferManager(const CardBufferManager&) = delete;
    CardBufferManager& operator=(const CardBufferManager&) = delete;

    // Metadata operations
    Result<MetadataHandle> allocateMetadata(uint32_t size);
    Result<void> deallocateMetadata(MetadataHandle handle);
    Result<void> writeMetadata(MetadataHandle handle, const void* data, uint32_t size);
    Result<void> writeMetadataAt(MetadataHandle handle, uint32_t offset, const void* data, uint32_t size);

    // Storage operations (for float data like plot points)
    Result<StorageHandle> allocateStorage(uint32_t size);
    Result<void> deallocateStorage(StorageHandle handle);
    Result<void> writeStorage(StorageHandle handle, const void* data, uint32_t size);
    Result<void> writeStorageAt(StorageHandle handle, uint32_t offset, const void* data, uint32_t size);

    // Allocate storage and write offset to metadata field
    Result<StorageHandle> allocateStorageAndLink(MetadataHandle metaHandle,
                                                  uint32_t metaFieldOffset,
                                                  uint32_t storageSize);

    // Image data operations (for RGBA8 pixel data)
    Result<ImageDataHandle> allocateImageData(uint32_t size);
    Result<void> deallocateImageData(ImageDataHandle handle);
    Result<void> writeImageData(ImageDataHandle handle, const void* data, uint32_t size);
    Result<void> writeImageDataAt(ImageDataHandle handle, uint32_t offset, const void* data, uint32_t size);

    // GPU upload
    Result<void> flush(WGPUQueue queue);

    // =========================================================================
    // Image Atlas (GPU-driven)
    // =========================================================================

    // Initialize atlas resources (call after constructor)
    Result<void> initAtlas();

    // Dispatch compute shader to populate atlas before render pass
    // cellBuffer: terminal cell buffer with glyph/fg/bg data
    // gridCols, gridRows: grid dimensions
    Result<void> prepareAtlas(WGPUCommandEncoder encoder, WGPUQueue queue,
                              WGPUBuffer cellBuffer,
                              uint32_t gridCols, uint32_t gridRows);

    // Accessors
    WGPUBuffer metadataBuffer() const { return _metadataGpuBuffer; }
    WGPUBuffer storageBuffer() const { return _storageGpuBuffer; }
    WGPUBuffer imageDataBuffer() const { return _imageDataGpuBuffer; }

    // Atlas accessors (for fragment shader binding)
    WGPUTexture atlasTexture() const { return _atlasTexture; }
    WGPUTextureView atlasTextureView() const { return _atlasTextureView; }
    WGPUSampler atlasSampler() const { return _atlasSampler; }
    bool atlasInitialized() const { return _atlasInitialized; }

    struct Stats {
        uint32_t metadataUsed;
        uint32_t metadataCapacity;
        uint32_t storageUsed;
        uint32_t storageCapacity;
        uint32_t imageDataUsed;
        uint32_t imageDataCapacity;
        uint32_t pendingMetadataUploads;
        uint32_t pendingStorageUploads;
        uint32_t pendingImageDataUploads;
    };
    Stats getStats() const;

private:
    Result<void> createGpuBuffers();
    Result<void> createAtlasTexture();
    Result<void> createAtlasComputePipeline();

    WGPUDevice _device;
    Config _config;

    // CPU buffers - use uint32_t for guaranteed 32-bit alignment
    std::vector<uint32_t> _metadataCpuBuffer;
    std::vector<uint32_t> _storageCpuBuffer;
    std::vector<uint32_t> _imageDataCpuBuffer;

    WGPUBuffer _metadataGpuBuffer;
    WGPUBuffer _storageGpuBuffer;
    WGPUBuffer _imageDataGpuBuffer;

    MetadataAllocator _metadataAllocator;
    StorageAllocator _storageAllocator;
    StorageAllocator _imageDataAllocator;

    DirtyTracker _metadataDirty;
    DirtyTracker _storageDirty;
    DirtyTracker _imageDataDirty;

    // =========================================================================
    // Image Atlas Resources
    // =========================================================================
    WGPUTexture _atlasTexture = nullptr;
    WGPUTextureView _atlasTextureView = nullptr;
    WGPUSampler _atlasSampler = nullptr;

    // Compute pipeline for atlas population
    WGPUShaderModule _atlasShaderModule = nullptr;
    WGPUComputePipeline _scanPipeline = nullptr;     // Phase 1: scan cells, allocate positions
    WGPUComputePipeline _copyPipeline = nullptr;     // Phase 2: copy pixels

    // Bind group for compute shader
    WGPUBindGroupLayout _atlasBindGroupLayout = nullptr;
    WGPUBindGroup _atlasBindGroup = nullptr;

    // Atlas state buffer (for atomic allocation)
    struct AtlasState {
        uint32_t nextX;
        uint32_t nextY;
        uint32_t rowHeight;
        uint32_t atlasWidth;
        uint32_t atlasHeight;
        uint32_t gridCols;
        uint32_t gridRows;
        uint32_t _padding;
    };
    WGPUBuffer _atlasStateBuffer = nullptr;

    // Processed cards tracking (to avoid duplicate processing)
    WGPUBuffer _processedCardsBuffer = nullptr;

    bool _atlasInitialized = false;
};

}  // namespace yetty
