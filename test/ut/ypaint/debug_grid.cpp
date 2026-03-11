// Standalone debug program to print grid state
#include "yetty/ypaint/ypaint-buffer.h"
#include "yetty/ypaint/ypaint-types.gen.h"
#include <yetty/ypaint/painter.h>
#include <yetty/card-manager.h>
#include <yetty/card-buffer-manager.h>
#include <yetty/gpu-allocator.h>

#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>

using namespace yetty;
using namespace yetty::ypaint;
using namespace yetty::ypaint::card;

// Minimal mocks - same as test file
class MockCardBufferManager : public CardBufferManager {
public:
    void reserve(uint32_t size) override { _pending += (size + 15) & ~15u; }
    Result<void> commitReservations() override {
        _storage.resize(std::max(_pending, 16u), 0);
        _offset = 0; _allocs.clear(); _pending = 0;
        return Ok();
    }
    Result<BufferHandle> allocateBuffer(uint32_t slot, const std::string& scope, uint32_t size) override {
        size = (size + 15) & ~15u;
        if (_offset + size > _storage.size()) return Err<BufferHandle>("OOM");
        BufferHandle h{_storage.data() + _offset, _offset, size};
        _allocs[{slot, scope}] = h;
        _offset += size;
        return Ok(h);
    }
    void markBufferDirty(BufferHandle) override {}
    BufferHandle getBufferHandle(uint32_t slot, const std::string& scope) const override {
        auto it = _allocs.find({slot, scope});
        return it != _allocs.end() ? it->second : BufferHandle::invalid();
    }
    Result<void> flush(WGPUQueue) override { return Ok(); }
    WGPUBuffer bufferGpu() const override { return nullptr; }
    uint32_t bufferHighWaterMark() const override { return _offset; }
    bool bindGroupDirty() const override { return false; }
    void clearBindGroupDirty() override {}
    SharedMemoryRegion::Ptr shm() const override { return nullptr; }
    bool usesSharedMemory() const override { return false; }
    Stats getStats() const override { return {_offset, (uint32_t)_storage.size(), 0}; }
    void dumpSubAllocations() const override {}
    std::string dumpSubAllocationsToString() const override { return ""; }
    std::vector<BufferInfo> getAllBuffers() const override { return {}; }
private:
    std::vector<uint8_t> _storage;
    uint32_t _offset = 0, _pending = 0;
    std::map<std::pair<uint32_t, std::string>, BufferHandle> _allocs;
};

class MockCardManager : public CardManager {
public:
    MockCardManager() : _bufMgr(std::make_shared<MockCardBufferManager>()), _meta(16*64, 0) {}
    Result<MetadataHandle> allocateMetadata(uint32_t) override { return Ok(MetadataHandle{0,64}); }
    Result<void> deallocateMetadata(MetadataHandle) override { return Ok(); }
    Result<void> writeMetadata(MetadataHandle h, const void* d, uint32_t s) override {
        if (h.offset + s > _meta.size()) return Err<void>("OOM");
        std::memcpy(_meta.data() + h.offset, d, s);
        return Ok();
    }
    Result<void> writeMetadataAt(MetadataHandle h, uint32_t off, const void* d, uint32_t s) override {
        return writeMetadata(MetadataHandle{h.offset+off, h.size-off}, d, s);
    }
    CardBufferManager::Ptr bufferManager() const override { return _bufMgr; }
    CardTextureManager::Ptr textureManager() const override { return nullptr; }
    WGPUBuffer metadataBuffer() const override { return nullptr; }
    WGPUBindGroupLayout sharedBindGroupLayout() const override { return nullptr; }
    WGPUBindGroup sharedBindGroup() const override { return nullptr; }
    Result<void> updateBindGroup() override { return Ok(); }
    void invalidateBindGroup() override {}
    Result<void> flush(WGPUQueue) override { return Ok(); }
private:
    std::shared_ptr<MockCardBufferManager> _bufMgr;
    std::vector<uint8_t> _meta;
};

static YPaintBuffer::Ptr createShapesDemoBuffer() {
    auto buf = *YPaintBuffer::create();
    buf->addCircle(0, 100, 120, 40, 0xFFe74c3c, 0xFFc0392b, 2, 0);
    buf->addBox(0, 250, 120, 50, 40, 0xFF3498db, 0xFF2980b9, 2, 8);
    buf->addTriangle(0, 400, 160, 350, 80, 450, 80, 0xFF2ecc71, 0xFF27ae60, 2, 0);
    buf->addEllipse(0, 550, 120, 60, 35, 0xFFf1c40f, 0xFFf39c12, 2, 0);
    buf->addSegment(0, 50, 220, 650, 220, 0, 0xFF9b59b6, 3, 0);
    return buf;
}

int main() {
    auto cardMgr = std::make_shared<MockCardManager>();
    auto gpuAlloc = std::make_shared<GpuAllocator>(nullptr);

    const float CELL_W = 10, CELL_H = 20;

    std::cout << "Testing shapes.sh scenario with different cursor positions\n\n";

    for (int cursorRow : {0, 3, 5, 10}) {
        auto builder = *YPaintBuilder::create(FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);
        builder->setSceneBounds(0, 0, 800, 600);
        builder->setGridCellSize(CELL_W, CELL_H);

        builder->setCursorPosition(0, cursorRow);
        builder->addYpaintBuffer(createShapesDemoBuffer());

        const auto& grid = builder->gridStaging();
        uint32_t gridW = builder->gridWidth();
        uint32_t gridH = builder->gridHeight();

        std::cout << "=== Cursor at row " << cursorRow << " ===" << std::endl;
        std::cout << "Grid: " << gridW << "x" << gridH << ", prims: " << builder->primitiveCount() << std::endl;

        // Check shader lookup positions for circle at (100, 120)
        // Shader looks up: cellX = floor(100/10) = 10, cellY = floor(120/20) = 6
        uint32_t shaderX = 10, shaderY = 6;
        uint32_t cellIdx = shaderY * gridW + shaderX;
        uint32_t count = 0;
        if (cellIdx < grid.size()) {
            uint32_t off = grid[cellIdx];
            if (off < grid.size()) count = grid[off];
        }
        std::cout << "Circle (100,120) -> shader cell (" << shaderX << "," << shaderY << "): "
                  << count << " prims" << std::endl;

        // Print rows with content
        std::cout << "Rows with content: ";
        for (uint32_t row = 0; row < gridH; row++) {
            for (uint32_t col = 0; col < gridW; col++) {
                uint32_t idx = row * gridW + col;
                if (idx < grid.size()) {
                    uint32_t off = grid[idx];
                    if (off < grid.size() && grid[off] > 0) {
                        std::cout << row << " ";
                        break;
                    }
                }
            }
        }
        std::cout << std::endl << std::endl;
    }

    return 0;
}
