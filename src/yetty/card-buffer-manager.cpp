#include <yetty/card-buffer-manager.h>
#include <yetty/gpu-allocator.h>
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <map>
#include <sstream>
#include <vector>

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

// Sub-allocation key: (slotIndex, scope)
using SubAllocKey = std::pair<uint32_t, std::string>;

// =============================================================================
// CardBufferManagerImpl
// =============================================================================

class CardBufferManagerImpl : public CardBufferManager {
public:
    CardBufferManagerImpl(GPUContext* gpuContext, GpuAllocator::Ptr allocator) noexcept;
    ~CardBufferManagerImpl() override;

    Result<void> init() noexcept;

    // Reservation API
    void reserve(uint32_t size) override;
    Result<void> commitReservations() override;

    // Buffer operations
    Result<BufferHandle> allocateBuffer(uint32_t slotIndex,
                                        const std::string& scope,
                                        uint32_t size) override;
    Result<void> writeBuffer(BufferHandle handle, const void* data, uint32_t size) override;
    Result<void> writeBufferAt(BufferHandle handle, uint32_t offset, const void* data, uint32_t size) override;
    void markBufferDirty(BufferHandle handle) override;

    Result<void> flush(WGPUQueue queue) override;

    WGPUBuffer bufferGpu() const override { return _bufferGpuBuffer; }
    uint32_t bufferHighWaterMark() const override { return _bufferAllocator.highWaterMark(); }

    bool bindGroupDirty() const override { return _bindGroupDirty; }
    void clearBindGroupDirty() override { _bindGroupDirty = false; }

    Stats getStats() const override;
    void dumpSubAllocations() const override;
    std::string dumpSubAllocationsToString() const override;

private:
    Result<void> createGpuBuffer();

    GPUContext* _gpuContext;
    WGPUDevice _device;
    GpuAllocator::Ptr _allocator;

    std::vector<uint32_t> _bufferCpuBuffer;
    WGPUBuffer _bufferGpuBuffer = nullptr;

    BufferAllocator _bufferAllocator;
    DirtyTracker _bufferDirty;

    uint32_t _currentBufferCapacity;
    uint32_t _pendingReservation = 0;

    bool _bindGroupDirty = true;

    // Keyed sub-allocation tracking: (slotIndex, scope) -> BufferHandle
    std::map<SubAllocKey, BufferHandle> _subAllocations;
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

Result<CardBufferManager::Ptr> CardBufferManager::create(GPUContext* gpuContext,
                                                          GpuAllocator::Ptr allocator) noexcept {
    auto mgr = std::make_shared<CardBufferManagerImpl>(gpuContext, std::move(allocator));
    if (auto res = mgr->init(); !res) {
        return Err<Ptr>("Failed to initialize CardBufferManager", res);
    }
    return Ok(std::move(mgr));
}

// =============================================================================
// CardBufferManagerImpl
// =============================================================================

CardBufferManagerImpl::CardBufferManagerImpl(GPUContext* gpuContext,
                                               GpuAllocator::Ptr allocator) noexcept
    : _gpuContext(gpuContext)
    , _device(gpuContext->device)
    , _allocator(std::move(allocator))
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
    if (_bufferGpuBuffer) _allocator->releaseBuffer(_bufferGpuBuffer);
}

Result<void> CardBufferManagerImpl::createGpuBuffer() {
    size_t bufferBytes = _bufferCpuBuffer.size() * sizeof(uint32_t);

    WGPUBufferDescriptor bufDesc = {};
    bufDesc.label = WGPU_STR("CardBuffer");
    bufDesc.size = bufferBytes;
    bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    bufDesc.mappedAtCreation = false;

    _bufferGpuBuffer = _allocator->createBuffer(bufDesc);
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
    uint32_t totalNeeded = std::max(_pendingReservation, 4u);  // minimum 4 bytes
    _pendingReservation = 0;

    // Reset allocator — all old allocations are invalid
    _bufferAllocator = BufferAllocator(totalNeeded);
    _subAllocations.clear();

    if (totalNeeded != _currentBufferCapacity) {
        // Recreate GPU buffer at exactly the needed size
        if (_bufferGpuBuffer) {
            _allocator->releaseBuffer(_bufferGpuBuffer);
            _bufferGpuBuffer = nullptr;
        }

        uint32_t newU32Count = (totalNeeded + 3) / 4;
        _bufferCpuBuffer.resize(newU32Count, 0);
        _bufferCpuBuffer.shrink_to_fit();

        WGPUBufferDescriptor bufDesc = {};
        bufDesc.label = WGPU_STR("CardBuffer");
        bufDesc.size = newU32Count * sizeof(uint32_t);
        bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
        bufDesc.mappedAtCreation = false;

        _bufferGpuBuffer = _allocator->createBuffer(bufDesc);
        if (!_bufferGpuBuffer) {
            _currentBufferCapacity = 0;
            return Err<void>("Failed to create GPU buffer");
        }

        _currentBufferCapacity = totalNeeded;
        _bindGroupDirty = true;
    }

    return Ok();
}

// =============================================================================
// Buffer operations
// =============================================================================

Result<BufferHandle> CardBufferManagerImpl::allocateBuffer(uint32_t slotIndex,
                                                            const std::string& scope,
                                                            uint32_t size) {
    SubAllocKey key{slotIndex, scope};

    // Auto-replace: if key already exists, free old allocation first
    auto it = _subAllocations.find(key);
    if (it != _subAllocations.end()) {
        _bufferAllocator.deallocate(it->second);
        _subAllocations.erase(it);
    }

    auto result = _bufferAllocator.allocate(size);
    if (!result) {
        return Err<BufferHandle>("allocateBuffer: out of reserved space");
    }

    BufferHandle handle = *result;
    uint8_t* bufferBytes = reinterpret_cast<uint8_t*>(_bufferCpuBuffer.data());
    handle.data = bufferBytes + handle.offset;

    _subAllocations[key] = handle;

    ydebug("CardBufferManager: allocated slot={} scope='{}' size={} offset={}",
           slotIndex, scope, handle.size, handle.offset);

    return Ok(handle);
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

void CardBufferManagerImpl::dumpSubAllocations() const {
    yinfo("=== CardBufferManager Sub-Allocations ({} entries, {}/{} bytes used) ===",
          _subAllocations.size(),
          _bufferAllocator.used(), _bufferAllocator.capacity());

    for (const auto& [key, handle] : _subAllocations) {
        yinfo("  slot={:>4} scope={:<12} offset={:>8} size={:>8} ({:.2f} KB)",
              key.first, key.second,
              handle.offset, handle.size, handle.size / 1024.0);
    }
}

std::string CardBufferManagerImpl::dumpSubAllocationsToString() const {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "  " << _subAllocations.size() << " sub-allocs, "
       << _bufferAllocator.used() << "/" << _bufferAllocator.capacity() << " bytes used\n";

    for (const auto& [key, handle] : _subAllocations) {
        ss << "    slot=" << std::setw(4) << key.first
           << " scope=" << std::left << std::setw(12) << key.second << std::right
           << "  " << std::setw(8) << handle.size << " B ("
           << std::setw(8) << (handle.size / 1024.0) << " KB)\n";
    }

    return ss.str();
}

}  // namespace yetty
