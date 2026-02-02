#include <yetty/card-buffer-manager.h>
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cstring>
#include <vector>

#define WGPU_STR(s) WGPUStringView{.data = s, .length = WGPU_STRLEN}

namespace yetty {

// =============================================================================
// Helper classes
// =============================================================================

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
// CardBufferManagerImpl
// =============================================================================

class CardBufferManagerImpl : public CardBufferManager {
public:
    CardBufferManagerImpl(GPUContext* gpuContext, Config config) noexcept;
    ~CardBufferManagerImpl() override;

    Result<void> init() noexcept;

    // Reservation API
    void reserve(uint32_t size) override;
    Result<void> commitReservations() override;

    // Buffer operations
    Result<BufferHandle> allocateBuffer(uint32_t size) override;
    Result<void> deallocateBuffer(BufferHandle handle) override;
    Result<void> writeBuffer(BufferHandle handle, const void* data, uint32_t size) override;
    Result<void> writeBufferAt(BufferHandle handle, uint32_t offset, const void* data, uint32_t size) override;
    void markBufferDirty(BufferHandle handle) override;

    Result<void> flush(WGPUQueue queue) override;

    WGPUBuffer bufferGpu() const override { return _bufferGpuBuffer; }
    uint32_t bufferHighWaterMark() const override { return _bufferAllocator.highWaterMark(); }

    bool bindGroupDirty() const override { return _bindGroupDirty; }
    void clearBindGroupDirty() override { _bindGroupDirty = false; }

    Stats getStats() const override;

private:
    Result<void> createGpuBuffer();
    Result<void> growBufferGpu(uint32_t requiredSize);

    GPUContext* _gpuContext;
    WGPUDevice _device;
    Config _config;

    std::vector<uint32_t> _bufferCpuBuffer;
    WGPUBuffer _bufferGpuBuffer = nullptr;

    BufferAllocator _bufferAllocator;
    DirtyTracker _bufferDirty;

    uint32_t _currentBufferCapacity;
    uint32_t _pendingReservation = 0;
    static constexpr uint32_t MAX_BUFFER_SIZE = 32 * 1024 * 1024;

    bool _bindGroupDirty = true;
};

// =============================================================================
// Helper implementations
// =============================================================================

BufferAllocator::BufferAllocator(uint32_t capacity)
    : _capacity(capacity)
    , _used(0) {
    _freeBlocks.push_back({0, capacity});
}

Result<BufferHandle> BufferAllocator::allocate(uint32_t size) {
    if (size == 0) {
        return Err("BufferAllocator: cannot allocate zero bytes");
    }

    size = (size + 15) & ~15u;  // Align to 16 bytes

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
            if (endOffset > _highWaterMark) _highWaterMark = endOffset;
            return Ok(BufferHandle{nullptr, offset, size});
        }
    }
    return Err("BufferAllocator: out of memory");
}

Result<void> BufferAllocator::deallocate(BufferHandle handle) {
    if (!handle.isValid()) return Err("BufferAllocator: invalid handle");

    auto it = std::lower_bound(_freeBlocks.begin(), _freeBlocks.end(), handle.offset,
        [](const FreeBlock& block, uint32_t offset) { return block.offset < offset; });

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
// Factory
// =============================================================================

Result<CardBufferManager::Ptr> CardBufferManager::create(GPUContext* gpuContext, Config config) noexcept {
    auto mgr = std::make_shared<CardBufferManagerImpl>(gpuContext, config);
    if (auto res = mgr->init(); !res) {
        return Err<Ptr>("Failed to initialize CardBufferManager", res);
    }
    return Ok(std::move(mgr));
}

// =============================================================================
// CardBufferManagerImpl
// =============================================================================

CardBufferManagerImpl::CardBufferManagerImpl(GPUContext* gpuContext, Config config) noexcept
    : _gpuContext(gpuContext)
    , _device(gpuContext->device)
    , _config(config)
    , _bufferAllocator(4)
    , _currentBufferCapacity(4) {
}

Result<void> CardBufferManagerImpl::init() noexcept {
    _bufferCpuBuffer.resize(1, 0);  // 4 bytes placeholder
    if (auto res = createGpuBuffer(); !res) {
        return Err<void>("Failed to create GPU buffer", res);
    }
    return Ok();
}

CardBufferManagerImpl::~CardBufferManagerImpl() {
    if (_bufferGpuBuffer) wgpuBufferRelease(_bufferGpuBuffer);
}

Result<void> CardBufferManagerImpl::createGpuBuffer() {
    size_t bufferBytes = _bufferCpuBuffer.size() * sizeof(uint32_t);

    yinfo("GPU_ALLOC CardBufferManager: bufferGpu={} bytes ({:.2f} MB)",
          bufferBytes, bufferBytes / (1024.0 * 1024.0));

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

// =============================================================================
// Reservation API
// =============================================================================

void CardBufferManagerImpl::reserve(uint32_t size) {
    _pendingReservation += (size + 15) & ~15u;  // align to 16 bytes like allocator
}

Result<void> CardBufferManagerImpl::commitReservations() {
    if (_pendingReservation == 0) return Ok();

    uint32_t totalNeeded = _bufferAllocator.used() + _pendingReservation;
    _pendingReservation = 0;

    if (totalNeeded > _currentBufferCapacity) {
        uint32_t newCapacity = std::max(_config.bufferCapacity, _currentBufferCapacity * 2);
        while (newCapacity < totalNeeded && newCapacity < MAX_BUFFER_SIZE) {
            newCapacity *= 2;
        }
        newCapacity = std::min(newCapacity, MAX_BUFFER_SIZE);
        if (newCapacity < totalNeeded) {
            return Err("Buffer reservations would exceed maximum size");
        }
        if (auto res = growBufferGpu(newCapacity); !res) {
            return Err<void>("Failed to grow buffer for reservations", res);
        }
        yinfo("CardBufferManager::commitReservations: grew to {} bytes ({:.2f} MB) for {} bytes needed",
              newCapacity, newCapacity / (1024.0 * 1024.0), totalNeeded);
    }

    return Ok();
}

// =============================================================================
// Buffer operations
// =============================================================================

Result<BufferHandle> CardBufferManagerImpl::allocateBuffer(uint32_t size) {
    auto result = _bufferAllocator.allocate(size);
    if (!result) {
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
        if (!result) return result;
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
    if (!handle.isValid()) return Err("writeBufferAt: invalid handle");
    if (offset + size > handle.size) return Err("writeBufferAt: write exceeds allocated size");

    uint32_t bufferOffset = handle.offset + offset;
    uint8_t* bufferBytes = reinterpret_cast<uint8_t*>(_bufferCpuBuffer.data());
    std::memcpy(bufferBytes + bufferOffset, data, size);
    _bufferDirty.markDirty(bufferOffset, size);
    return Ok();
}

void CardBufferManagerImpl::markBufferDirty(BufferHandle handle) {
    if (handle.isValid()) {
        _bufferDirty.markDirty(handle.offset, handle.size);
    }
}


// =============================================================================
// GPU upload
// =============================================================================

Result<void> CardBufferManagerImpl::flush(WGPUQueue queue) {
    if (_bufferDirty.hasDirty()) {
        const uint8_t* bufferBytes = reinterpret_cast<const uint8_t*>(_bufferCpuBuffer.data());
        auto ranges = _bufferDirty.getCoalescedRanges();
        for (const auto& [offset, size] : ranges) {
            ydebug("CardBufMgr::flush: buffer upload offset={} size={}", offset, size);
            wgpuQueueWriteBuffer(queue, _bufferGpuBuffer, offset,
                                 bufferBytes + offset, size);
        }
        _bufferDirty.clear();
    }
    return Ok();
}

CardBufferManager::Stats CardBufferManagerImpl::getStats() const {
    return Stats{
        .bufferUsed = _bufferAllocator.used(),
        .bufferCapacity = _bufferAllocator.capacity(),
        .pendingBufferUploads = _bufferDirty.hasDirty() ? 1u : 0u
    };
}

// =============================================================================
// Buffer growth
// =============================================================================

Result<void> CardBufferManagerImpl::growBufferGpu(uint32_t newCapacity) {
    if (newCapacity <= _currentBufferCapacity) return Ok();

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
    _bindGroupDirty = true;

    yinfo("GPU_ALLOC CardBufferManager: buffer grown to {} bytes ({:.2f} MB)",
          newCapacity, newCapacity / (1024.0 * 1024.0));
    return Ok();
}

}  // namespace yetty
