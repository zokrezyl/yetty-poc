#pragma once

#include <yetty/result.hpp>
#include <cstdint>
#include <vector>
#include <webgpu/webgpu.h>

namespace yetty {

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
    uint32_t metadataPool32Count = 256;
    uint32_t metadataPool64Count = 128;
    uint32_t metadataPool128Count = 64;
    uint32_t metadataPool256Count = 32;
    uint32_t storageCapacity = 16 * 1024 * 1024;
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

    // Storage operations
    Result<StorageHandle> allocateStorage(uint32_t size);
    Result<void> deallocateStorage(StorageHandle handle);
    Result<void> writeStorage(StorageHandle handle, const void* data, uint32_t size);
    Result<void> writeStorageAt(StorageHandle handle, uint32_t offset, const void* data, uint32_t size);

    // Allocate storage and write offset to metadata field
    Result<StorageHandle> allocateStorageAndLink(MetadataHandle metaHandle,
                                                  uint32_t metaFieldOffset,
                                                  uint32_t storageSize);

    // GPU upload
    Result<void> flush(WGPUQueue queue);

    // Accessors
    WGPUBuffer metadataBuffer() const { return _metadataGpuBuffer; }
    WGPUBuffer storageBuffer() const { return _storageGpuBuffer; }

    struct Stats {
        uint32_t metadataUsed;
        uint32_t metadataCapacity;
        uint32_t storageUsed;
        uint32_t storageCapacity;
        uint32_t pendingMetadataUploads;
        uint32_t pendingStorageUploads;
    };
    Stats getStats() const;

private:
    Result<void> createGpuBuffers();

    WGPUDevice _device;
    Config _config;

    std::vector<uint8_t> _metadataCpuBuffer;
    std::vector<uint8_t> _storageCpuBuffer;

    WGPUBuffer _metadataGpuBuffer;
    WGPUBuffer _storageGpuBuffer;

    MetadataAllocator _metadataAllocator;
    StorageAllocator _storageAllocator;

    DirtyTracker _metadataDirty;
    DirtyTracker _storageDirty;
};

}  // namespace yetty
