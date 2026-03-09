//=============================================================================
// Test for shapes.sh demo scenario
// Bug: shapes don't render when cursor is at position like (0, 3)
//=============================================================================

#include <boost/ut.hpp>

#include "yetty/ydraw/ydraw-buffer.h"
#include "yetty/ydraw/ydraw-types.gen.h"
#include <yetty/ydraw-builder.h>
#include <yetty/card-manager.h>
#include <yetty/card-buffer-manager.h>
#include <yetty/gpu-allocator.h>

#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::card;

//=============================================================================
// Mock CardBufferManager — copied from ydraw_scrolling_test.cpp
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
            return Err<BufferHandle>("MockCardBufferManager: out of space");
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
// Mock CardManager — copied from ydraw_scrolling_test.cpp
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
            return Err<void>("MockCardManager: metadata overflow");
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

private:
    std::shared_ptr<MockCardBufferManager> _bufMgr;
    std::vector<uint8_t> _metadata;
};

static GpuAllocator::Ptr testAllocator() {
    static auto alloc = std::make_shared<GpuAllocator>(nullptr);
    return alloc;
}

//=============================================================================
// Create buffer mimicking shapes.sh demo payload
// Shapes span Y from ~30 to ~420 pixels
//=============================================================================
static YDrawBuffer::Ptr createShapesDemoBuffer() {
    auto buf = *YDrawBuffer::create();

    // Circle at (100, 120), radius 40
    buf->addCircle(0, 100, 120, 40, 0xFFe74c3c, 0xFFc0392b, 2, 0);

    // Box at (250, 120), size 100x80 -> half size 50x40
    buf->addBox(0, 250, 120, 50, 40, 0xFF3498db, 0xFF2980b9, 2, 8);

    // Triangle vertices at (400,160), (350,80), (450,80)
    buf->addTriangle(0, 400, 160, 350, 80, 450, 80, 0xFF2ecc71, 0xFF27ae60, 2, 0);

    // Ellipse at (550, 120), radii 60x35
    buf->addEllipse(0, 550, 120, 60, 35, 0xFFf1c40f, 0xFFf39c12, 2, 0);

    // Segment from (50, 220) to (650, 220)
    buf->addSegment(0, 50, 220, 650, 220, 0, 0xFF9b59b6, 3, 0);

    // Bezier 1: (50,320) -> (200,270) -> (350,370)
    buf->addBezier2(0, 50, 320, 200, 270, 350, 370, 0, 0xFFe67e22, 3, 0);

    // Bezier 2: (350,370) -> (500,270) -> (650,320)
    buf->addBezier2(0, 350, 370, 500, 270, 650, 320, 0, 0xFFe67e22, 3, 0);

    // Arc at (550, 350)
    buf->addArc(0, 550, 350, 270 * 3.14159f / 180.0f, 0, 50, 12, 0xFF1abc9c, 0, 0, 0);

    return buf;
}

//=============================================================================
// Visual grid dump
//=============================================================================
static void dumpGrid(YDrawBuilder* builder, const char* label) {
    const auto& grid = builder->gridStaging();
    uint32_t gridW = builder->gridWidth();
    uint32_t gridH = builder->gridHeight();
    uint32_t primCount = builder->primitiveCount();

    std::cout << "\n=== " << label << " ===" << std::endl;
    std::cout << "Grid: " << gridW << "x" << gridH << ", prims: " << primCount << std::endl;

    for (uint32_t row = 0; row < gridH; row++) {
        bool hasContent = false;
        for (uint32_t col = 0; col < gridW; col++) {
            uint32_t cellIdx = row * gridW + col;
            if (cellIdx < grid.size()) {
                uint32_t offset = grid[cellIdx];
                if (offset < grid.size() && grid[offset] > 0) {
                    hasContent = true;
                    break;
                }
            }
        }

        if (hasContent || row % 5 == 0) {
            std::cout << "Row " << std::setw(3) << row << ": ";
            for (uint32_t col = 0; col < gridW && col < 80; col++) {
                uint32_t cellIdx = row * gridW + col;
                if (cellIdx < grid.size()) {
                    uint32_t offset = grid[cellIdx];
                    if (offset < grid.size()) {
                        uint32_t count = grid[offset];
                        if (count > 0) {
                            if (count < 10) std::cout << count;
                            else std::cout << "+";
                        } else {
                            std::cout << ".";
                        }
                    }
                }
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::flush;
}

//=============================================================================
// Tests
//=============================================================================
suite shapes_demo_tests = [] {

    "shapes_demo_cursor_0_0"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();
        auto builder = *YDrawBuilder::create(FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Terminal-like scene: 800x600 pixels, cell 10x20
        builder->setSceneBounds(0, 0, 800, 600);
        builder->setGridCellSize(10, 20);

        std::cout << "\n########## SHAPES DEMO: CURSOR (0,0) ##########" << std::endl;

        builder->setCursorPosition(0, 0);
        builder->addYdrawBuffer(createShapesDemoBuffer());

        dumpGrid(builder.get(), "Cursor (0,0)");

        // Shapes span Y from ~80 to ~420, so rows 4 to 21 should have content
        const auto& grid = builder->gridStaging();
        uint32_t gridW = builder->gridWidth();

        bool hasContentInExpectedRows = false;
        for (uint32_t row = 4; row <= 21; row++) {
            for (uint32_t col = 0; col < gridW; col++) {
                uint32_t cellIdx = row * gridW + col;
                if (cellIdx < grid.size()) {
                    uint32_t offset = grid[cellIdx];
                    if (offset < grid.size() && grid[offset] > 0) {
                        hasContentInExpectedRows = true;
                        break;
                    }
                }
            }
            if (hasContentInExpectedRows) break;
        }
        expect(hasContentInExpectedRows) << "Should have content in rows 4-21";
    };

    "shapes_demo_cursor_0_3"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();
        auto builder = *YDrawBuilder::create(FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        builder->setSceneBounds(0, 0, 800, 600);
        builder->setGridCellSize(10, 20);

        std::cout << "\n########## SHAPES DEMO: CURSOR (0,3) ##########" << std::endl;

        // THIS IS THE FAILING CASE - cursor at row 3
        builder->setCursorPosition(0, 3);
        builder->addYdrawBuffer(createShapesDemoBuffer());

        dumpGrid(builder.get(), "Cursor (0,3)");

        // With cursor at row 3, shapes should be offset by 3 rows
        // Original rows 4-21 become rows 7-24
        const auto& grid = builder->gridStaging();
        uint32_t gridW = builder->gridWidth();

        bool hasContentInExpectedRows = false;
        for (uint32_t row = 7; row <= 24; row++) {
            for (uint32_t col = 0; col < gridW; col++) {
                uint32_t cellIdx = row * gridW + col;
                if (cellIdx < grid.size()) {
                    uint32_t offset = grid[cellIdx];
                    if (offset < grid.size() && grid[offset] > 0) {
                        hasContentInExpectedRows = true;
                        break;
                    }
                }
            }
            if (hasContentInExpectedRows) break;
        }
        expect(hasContentInExpectedRows) << "Should have content in rows 7-24 (offset by cursor row 3)";
    };

    "shapes_demo_cursor_0_10"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();
        auto builder = *YDrawBuilder::create(FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        builder->setSceneBounds(0, 0, 800, 800);
        builder->setGridCellSize(10, 20);

        std::cout << "\n########## SHAPES DEMO: CURSOR (0,10) ##########" << std::endl;

        builder->setCursorPosition(0, 10);
        builder->addYdrawBuffer(createShapesDemoBuffer());

        dumpGrid(builder.get(), "Cursor (0,10)");

        const auto& grid = builder->gridStaging();
        uint32_t gridW = builder->gridWidth();

        // With cursor at row 10, shapes should be in rows 14-31
        bool hasContentInExpectedRows = false;
        for (uint32_t row = 14; row <= 31; row++) {
            for (uint32_t col = 0; col < gridW; col++) {
                uint32_t cellIdx = row * gridW + col;
                if (cellIdx < grid.size()) {
                    uint32_t offset = grid[cellIdx];
                    if (offset < grid.size() && grid[offset] > 0) {
                        hasContentInExpectedRows = true;
                        break;
                    }
                }
            }
            if (hasContentInExpectedRows) break;
        }
        expect(hasContentInExpectedRows) << "Should have content in rows 14-31";
    };

    "shapes_demo_multiple_adds_with_scroll"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();
        auto builder = *YDrawBuilder::create(FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        builder->setSceneBounds(0, 0, 800, 600);
        builder->setGridCellSize(10, 20);

        std::cout << "\n########## SHAPES DEMO: MULTIPLE ADDS WITH SCROLL ##########" << std::endl;

        // First add at cursor 0,0
        builder->setCursorPosition(0, 0);
        builder->addYdrawBuffer(createShapesDemoBuffer());
        dumpGrid(builder.get(), "After 1st add at cursor(0,0)");

        // Second add - simulating echo output advancing cursor
        builder->setCursorPosition(0, 22);
        builder->addYdrawBuffer(createShapesDemoBuffer());
        dumpGrid(builder.get(), "After 2nd add at cursor(0,22)");

        // Scroll happens
        std::cout << "\n>>> SCROLL 10 LINES <<<" << std::endl;
        builder->scrollLines(10);
        dumpGrid(builder.get(), "After scroll(10)");

        // Third add
        builder->setCursorPosition(0, 25);
        builder->addYdrawBuffer(createShapesDemoBuffer());
        dumpGrid(builder.get(), "After 3rd add at cursor(0,25)");

        expect(builder->primitiveCount() > 0_u) << "Should have primitives";
    };

    "shapes_demo_verify_grid_vs_shader_lookup"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();
        auto builder = *YDrawBuilder::create(FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const float CELL_H = 20.0f;
        const float CELL_W = 10.0f;
        builder->setSceneBounds(0, 0, 800, 600);
        builder->setGridCellSize(CELL_W, CELL_H);

        std::cout << "\n########## VERIFY GRID VS SHADER LOOKUP ##########" << std::endl;

        builder->setCursorPosition(0, 3);
        builder->addYdrawBuffer(createShapesDemoBuffer());

        const auto& grid = builder->gridStaging();
        uint32_t gridW = builder->gridWidth();
        uint32_t gridH = builder->gridHeight();

        // The shader looks up primitives by pixel position:
        // cellX = floor((pixelX - sceneMinX) / cellSizeX)
        // cellY = floor((pixelY - sceneMinY) / cellSizeY)

        // Circle is at (100, 120). In shader:
        // cellX = floor(100 / 10) = 10
        // cellY = floor(120 / 20) = 6

        // So shader expects primitive in grid cell (10, 6)
        // But with cursor offset, where did we actually store it?

        std::cout << "Circle at pixel (100, 120):" << std::endl;
        std::cout << "  Shader lookup: cell (" << int(100/CELL_W) << ", " << int(120/CELL_H) << ")" << std::endl;

        uint32_t shaderCellX = 10;
        uint32_t shaderCellY = 6;
        uint32_t cellIdx = shaderCellY * gridW + shaderCellX;

        if (cellIdx < grid.size()) {
            uint32_t offset = grid[cellIdx];
            if (offset < grid.size()) {
                uint32_t count = grid[offset];
                std::cout << "  Grid cell (" << shaderCellX << "," << shaderCellY << ") has " << count << " prims" << std::endl;
                expect(count > 0_u) << "Shader lookup cell should have primitive!";
            }
        }

        // Print which rows actually have content
        std::cout << "\nRows with content:" << std::endl;
        for (uint32_t row = 0; row < gridH; row++) {
            uint32_t rowPrims = 0;
            for (uint32_t col = 0; col < gridW; col++) {
                uint32_t idx = row * gridW + col;
                if (idx < grid.size()) {
                    uint32_t off = grid[idx];
                    if (off < grid.size()) rowPrims += grid[off];
                }
            }
            if (rowPrims > 0) {
                std::cout << "  Row " << row << ": " << rowPrims << " refs" << std::endl;
            }
        }
    };
};
