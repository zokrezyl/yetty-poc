//=============================================================================
// YDrawBuilder Scrolling Performance Tests
//
// Measures performance of scrolling mode with configurable:
// - Primitives per buffer
// - Number of buffers (simulating terminal lines)
//
// Usage: ./scrolling_perf_test [prims_per_buffer] [num_buffers]
// Defaults: 10 primitives per buffer, 1000 buffers
//=============================================================================

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <iomanip>

#include "yetty/ydraw/ydraw-buffer.h"
#include <yetty/ydraw-builder.h>
#include <yetty/card-manager.h>
#include <yetty/card-buffer-manager.h>
#include <yetty/gpu-allocator.h>

#include <cstring>
#include <map>
#include <vector>

using namespace yetty;
using namespace yetty::card;

//=============================================================================
// Mock CardBufferManager
//=============================================================================
class MockCardBufferManager : public CardBufferManager {
public:
    void reserve(uint32_t size) override {
        _pendingReservation += (size + 15) & ~15u;
    }

    Result<void> commitReservations() override {
        uint32_t total = std::max(_pendingReservation, 16u);
        _storage.resize(total, 0);
        _nextOffset = 0;
        _allocs.clear();
        _pendingReservation = 0;
        return Ok();
    }

    Result<BufferHandle> allocateBuffer(uint32_t slotIndex,
                                         const std::string& scope,
                                         uint32_t size) override {
        size = (size + 15) & ~15u;
        if (_nextOffset + size > _storage.size()) {
            _storage.resize(_nextOffset + size);
        }
        BufferHandle h{_storage.data() + _nextOffset, _nextOffset, size};
        _allocs[{slotIndex, scope}] = h;
        _nextOffset += size;
        return Ok(h);
    }

    void markBufferDirty(BufferHandle) override {}

    BufferHandle getBufferHandle(uint32_t slotIndex,
                                  const std::string& scope) const override {
        auto it = _allocs.find({slotIndex, scope});
        if (it != _allocs.end()) return it->second;
        return BufferHandle::invalid();
    }

    Result<void> flush(WGPUQueue) override { return Ok(); }
    WGPUBuffer bufferGpu() const override { return nullptr; }
    uint32_t bufferHighWaterMark() const override { return _nextOffset; }
    bool bindGroupDirty() const override { return false; }
    void clearBindGroupDirty() override {}
    SharedMemoryRegion::Ptr shm() const override { return nullptr; }
    bool usesSharedMemory() const override { return false; }
    Stats getStats() const override { return {_nextOffset, (uint32_t)_storage.size(), 0}; }
    void dumpSubAllocations() const override {}
    std::string dumpSubAllocationsToString() const override { return ""; }
    std::vector<BufferInfo> getAllBuffers() const override { return {}; }

private:
    std::vector<uint8_t> _storage;
    uint32_t _nextOffset = 0;
    uint32_t _pendingReservation = 0;
    std::map<std::pair<uint32_t, std::string>, BufferHandle> _allocs;
};

//=============================================================================
// Mock CardManager
//=============================================================================
class MockCardManager : public CardManager {
public:
    MockCardManager()
        : _bufMgr(std::make_shared<MockCardBufferManager>())
        , _metadata(16 * 64, 0) {}

    Result<MetadataHandle> allocateMetadata(uint32_t size) override {
        return Ok(MetadataHandle{0, 64});
    }
    Result<void> deallocateMetadata(MetadataHandle) override { return Ok(); }

    Result<void> writeMetadata(MetadataHandle handle,
                                const void* data, uint32_t size) override {
        if (handle.offset + size > _metadata.size()) {
            _metadata.resize(handle.offset + size);
        }
        std::memcpy(_metadata.data() + handle.offset, data, size);
        return Ok();
    }

    Result<void> writeMetadataAt(MetadataHandle handle, uint32_t offset,
                                  const void* data, uint32_t size) override {
        return writeMetadata(
            MetadataHandle{handle.offset + offset, handle.size - offset},
            data, size);
    }

    CardBufferManager::Ptr bufferManager() const override { return _bufMgr; }
    CardTextureManager::Ptr textureManager() const override { return nullptr; }

    WGPUBuffer metadataBuffer() const override { return nullptr; }
    WGPUBindGroupLayout sharedBindGroupLayout() const override { return nullptr; }
    WGPUBindGroup sharedBindGroup() const override { return nullptr; }
    Result<void> updateBindGroup() override { return Ok(); }
    void invalidateBindGroup() override {}
    Result<void> flush(WGPUQueue) override { return Ok(); }

    std::shared_ptr<MockCardBufferManager> mockBufMgr() const { return _bufMgr; }

private:
    std::shared_ptr<MockCardBufferManager> _bufMgr;
    std::vector<uint8_t> _metadata;
};

//=============================================================================
// Performance test
//=============================================================================

int main(int argc, char** argv) {
    // Parse command line arguments
    int primsPerBuffer = 10;
    int numBuffers = 1000;

    if (argc > 1) primsPerBuffer = std::atoi(argv[1]);
    if (argc > 2) numBuffers = std::atoi(argv[2]);

    if (primsPerBuffer < 1) primsPerBuffer = 1;
    if (numBuffers < 1) numBuffers = 1;

    std::cout << "=== YDrawBuilder Scrolling Performance Test ===" << std::endl;
    std::cout << "Primitives per buffer: " << primsPerBuffer << std::endl;
    std::cout << "Number of buffers: " << numBuffers << std::endl;
    std::cout << "Total primitives: " << (primsPerBuffer * numBuffers) << std::endl;
    std::cout << std::endl;

    auto cardMgr = std::make_shared<MockCardManager>();
    auto gpuAlloc = std::make_shared<GpuAllocator>(nullptr);

    // Create builder with scrolling mode
    auto builder = *YDrawBuilder::create(
        FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

    // Set scene: 80 columns x 24 lines (typical terminal)
    float cellW = 10.0f;
    float cellH = 20.0f;
    builder->setSceneBounds(0, 0, 80 * cellW, 24 * cellH);
    builder->setGridCellSize(cellW, cellH);

    std::cout << "Scene: " << (80 * cellW) << "x" << (24 * cellH)
              << " (" << builder->sceneHeightInLines() << " lines)" << std::endl;
    std::cout << std::endl;

    // Measure addYdrawBuffer performance
    auto startAdd = std::chrono::high_resolution_clock::now();

    for (int row = 0; row < numBuffers; ++row) {
        builder->setCursorPosition(0, static_cast<uint16_t>(row));

        auto buf = *YDrawBuffer::create();
        for (int p = 0; p < primsPerBuffer; ++p) {
            float x = static_cast<float>(p % 80) * cellW + cellW / 2;
            float y = cellH / 2;
            buf->addCircle(0, x, y, 3.0f, 0xFFFFFFFF, 0, 0, 0);
        }
        builder->addYdrawBuffer(buf);
    }

    auto endAdd = std::chrono::high_resolution_clock::now();
    auto addDuration = std::chrono::duration_cast<std::chrono::microseconds>(endAdd - startAdd);

    std::cout << "addYdrawBuffer phase:" << std::endl;
    std::cout << "  Total time: " << addDuration.count() << " us" << std::endl;
    std::cout << "  Per buffer: " << (addDuration.count() / numBuffers) << " us" << std::endl;
    std::cout << "  Per primitive: " << (addDuration.count() / (numBuffers * primsPerBuffer)) << " us" << std::endl;
    std::cout << std::endl;

    // Measure GPU buffer allocation phase
    auto startAlloc = std::chrono::high_resolution_clock::now();

    builder->declareBufferNeeds();
    cardMgr->mockBufMgr()->commitReservations();
    builder->allocateBuffers();

    auto endAlloc = std::chrono::high_resolution_clock::now();
    auto allocDuration = std::chrono::duration_cast<std::chrono::microseconds>(endAlloc - startAlloc);

    std::cout << "Buffer allocation phase:" << std::endl;
    std::cout << "  Total time: " << allocDuration.count() << " us" << std::endl;
    std::cout << std::endl;

    // Measure writeBuffers phase
    auto startWrite = std::chrono::high_resolution_clock::now();

    builder->writeBuffers();

    auto endWrite = std::chrono::high_resolution_clock::now();
    auto writeDuration = std::chrono::duration_cast<std::chrono::microseconds>(endWrite - startWrite);

    std::cout << "writeBuffers phase:" << std::endl;
    std::cout << "  Total time: " << writeDuration.count() << " us" << std::endl;
    std::cout << std::endl;

    // Summary
    auto totalDuration = addDuration + allocDuration + writeDuration;
    std::cout << "=== Summary ===" << std::endl;
    std::cout << "Total primitives: " << builder->primitiveCount() << std::endl;
    std::cout << "Total time: " << totalDuration.count() << " us ("
              << (totalDuration.count() / 1000.0) << " ms)" << std::endl;
    std::cout << "Throughput: " << std::fixed << std::setprecision(0)
              << (builder->primitiveCount() * 1000000.0 / totalDuration.count())
              << " prims/sec" << std::endl;

    return 0;
}
