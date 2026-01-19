#include "card-buffer-manager.h"
#include <algorithm>
#include <cstring>

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
    , _metadataAllocator(config.metadataPool32Count, config.metadataPool64Count,
                         config.metadataPool128Count, config.metadataPool256Count)
    , _storageAllocator(config.storageCapacity) {

    _metadataCpuBuffer.resize(_metadataAllocator.totalSize(), 0);
    _storageCpuBuffer.resize(config.storageCapacity, 0);

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
}

Result<void> CardBufferManager::createGpuBuffers() {
    WGPUBufferDescriptor metaDesc = {};
    metaDesc.label = {.data = "CardMetadataBuffer", .length = WGPU_STRLEN};
    metaDesc.size = _metadataCpuBuffer.size();
    metaDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    metaDesc.mappedAtCreation = false;

    _metadataGpuBuffer = wgpuDeviceCreateBuffer(_device, &metaDesc);
    if (!_metadataGpuBuffer) {
        return Err("Failed to create metadata GPU buffer");
    }

    WGPUBufferDescriptor storageDesc = {};
    storageDesc.label = {.data = "CardStorageBuffer", .length = WGPU_STRLEN};
    storageDesc.size = _storageCpuBuffer.size();
    storageDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    storageDesc.mappedAtCreation = false;

    _storageGpuBuffer = wgpuDeviceCreateBuffer(_device, &storageDesc);
    if (!_storageGpuBuffer) {
        return Err("Failed to create storage GPU buffer");
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
    std::memcpy(_metadataCpuBuffer.data() + bufferOffset, data, size);
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
    std::memcpy(_storageCpuBuffer.data() + bufferOffset, data, size);
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

Result<void> CardBufferManager::flush(WGPUQueue queue) {
    // Upload dirty metadata regions
    if (_metadataDirty.hasDirty()) {
        auto ranges = _metadataDirty.getCoalescedRanges();
        for (const auto& [offset, size] : ranges) {
            wgpuQueueWriteBuffer(queue, _metadataGpuBuffer, offset,
                                 _metadataCpuBuffer.data() + offset, size);
        }
        _metadataDirty.clear();
    }

    // Upload dirty storage regions
    if (_storageDirty.hasDirty()) {
        auto ranges = _storageDirty.getCoalescedRanges();
        for (const auto& [offset, size] : ranges) {
            wgpuQueueWriteBuffer(queue, _storageGpuBuffer, offset,
                                 _storageCpuBuffer.data() + offset, size);
        }
        _storageDirty.clear();
    }

    return Ok();
}

CardBufferManager::Stats CardBufferManager::getStats() const {
    return Stats{
        .metadataUsed = _metadataAllocator.totalSize() - 0,  // TODO: track properly
        .metadataCapacity = _metadataAllocator.totalSize(),
        .storageUsed = _storageAllocator.used(),
        .storageCapacity = _storageAllocator.capacity(),
        .pendingMetadataUploads = _metadataDirty.hasDirty() ? 1u : 0u,
        .pendingStorageUploads = _storageDirty.hasDirty() ? 1u : 0u
    };
}

}  // namespace yetty
