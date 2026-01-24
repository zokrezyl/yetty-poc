#pragma once

#include <yetty/result.hpp>
#include <cstdint>
#include <vector>
#include <webgpu/webgpu.h>

namespace yetty {

// Constants for image atlas
constexpr uint32_t IMAGE_ATLAS_SIZE = 2048;       // 2048x2048 = 16MB RGBA8
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

    // High water mark: maximum offset+size ever allocated (for upload optimization)
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

// Free-list allocator for variable-size storage
class StorageAllocator {
public:
    explicit StorageAllocator(uint32_t capacity);

    Result<StorageHandle> allocate(uint32_t size);
    Result<void> deallocate(StorageHandle handle);

    uint32_t capacity() const { return _capacity; }
    uint32_t used() const { return _used; }
    uint32_t fragmentCount() const { return static_cast<uint32_t>(_freeBlocks.size()); }

    // High water mark: maximum offset+size ever allocated (for upload optimization)
    uint32_t highWaterMark() const { return _highWaterMark; }

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

// Configuration for CardBufferManager
struct CardBufferConfig {
    uint32_t metadataPool32Count = 0;    // No 32-byte slots needed currently
    uint32_t metadataPool64Count = 64;   // ImageCard uses 64-byte metadata (64 cards max)
    uint32_t metadataPool128Count = 16;
    uint32_t metadataPool256Count = 8;
    uint32_t storageCapacity = 1 * 1024 * 1024;       // 1MB for plot data (array<f32>)
    uint32_t imageDataCapacity = 8 * 1024 * 1024;     // 8MB for image pixels - ~2 1024x1024 images
};

// Main buffer manager
class CardBufferManager {
public:
    using Config = CardBufferConfig;
    using Ptr = std::unique_ptr<CardBufferManager>;

    ~CardBufferManager();

    CardBufferManager(const CardBufferManager&) = delete;
    CardBufferManager& operator=(const CardBufferManager&) = delete;

    static Result<Ptr> create(WGPUDevice device, Config config = {}) noexcept;

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

    // High water marks for upload optimization (only upload up to this extent)
    uint32_t metadataHighWaterMark() const { return _metadataAllocator.highWaterMark(); }
    uint32_t storageHighWaterMark() const { return _storageAllocator.highWaterMark(); }
    uint32_t imageDataHighWaterMark() const { return _imageDataAllocator.highWaterMark(); }

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
    CardBufferManager(WGPUDevice device, Config config) noexcept;
    Result<void> init() noexcept;

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
    WGPUBuffer _lastCellBuffer = nullptr;  // Track to avoid recreating bind group every frame

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
