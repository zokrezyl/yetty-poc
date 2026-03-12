//=============================================================================
// Painter Scrolling Mode Unit Tests
//
// Tests terminal-style scrolling behavior:
// - Cursor positioning
// - Content overflow and scroll
// - Multi-page scrolling with progressive buffer additions
// - Hundreds of buffers with various sizes (1-5 lines)
// - Cursor positioning at arbitrary locations
//=============================================================================

#include <algorithm>
#include <random>

#include <boost/ut.hpp>

#include "yetty/ypaint/ypaint-buffer.h"
#include "yetty/ypaint/ypaint-types.gen.h"
#include <yetty/ypaint/painter.h>
#include <yetty/card-manager.h>
#include <yetty/card-buffer-manager.h>
#include <yetty/gpu-allocator.h>

#include <cstring>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::ypaint;
using namespace yetty::card;

//=============================================================================
// Helper: read u32 from float buffer at word index
//=============================================================================
static uint32_t readU32(const float* buf, uint32_t wordIdx) {
    uint32_t val;
    std::memcpy(&val, &buf[wordIdx], sizeof(uint32_t));
    return val;
}

//=============================================================================
// Mock CardBufferManager — CPU byte array with bump allocator
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

    // Stubs
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

    // Test access
    const float* storageAsFloat() const {
        return reinterpret_cast<const float*>(_storage.data());
    }

private:
    std::vector<uint8_t> _storage;
    uint32_t _nextOffset = 0;
    uint32_t _pendingReservation = 0;
    std::map<std::pair<uint32_t, std::string>, BufferHandle> _allocs;
};

//=============================================================================
// Mock CardManager — owns mock buffer manager + CPU metadata array
//=============================================================================
class MockCardManager : public CardManager {
public:
    MockCardManager()
        : _bufMgr(std::make_shared<MockCardBufferManager>())
        , _metadata(16 * 64, 0) {}

    // Metadata
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

    // Manager accessors
    CardBufferManager::Ptr bufferManager() const override { return _bufMgr; }
    CardTextureManager::Ptr textureManager() const override { return nullptr; }

    // GPU stubs
    WGPUBuffer metadataBuffer() const override { return nullptr; }
    WGPUBindGroupLayout sharedBindGroupLayout() const override { return nullptr; }
    WGPUBindGroup sharedBindGroup() const override { return nullptr; }
    Result<void> updateBindGroup() override { return Ok(); }
    void invalidateBindGroup() override {}
    Result<void> flush(WGPUQueue) override { return Ok(); }

    // Test access
    std::shared_ptr<MockCardBufferManager> mockBufMgr() const { return _bufMgr; }

    const uint32_t* metadataAsU32() const {
        return reinterpret_cast<const uint32_t*>(_metadata.data());
    }

private:
    std::shared_ptr<MockCardBufferManager> _bufMgr;
    std::vector<uint8_t> _metadata;
};

//=============================================================================
// Shared GpuAllocator with nullptr device
//=============================================================================
static GpuAllocator::Ptr testAllocator() {
    static auto alloc = std::make_shared<GpuAllocator>(nullptr);
    return alloc;
}

//=============================================================================
// Helper: Create a shape that spans N lines (height = N * cellHeight - 1)
//=============================================================================
static YPaintBuffer::Ptr createShapeSpanningLines(uint32_t numLines, float cellHeight) {
    auto buf = *YPaintBuffer::create();
    // Box spanning Y from 0 to (numLines * cellHeight - 1)
    // addBox uses (layer, cx, cy, hw, hh) where hw/hh are HALF sizes
    float height = numLines * cellHeight - 1.0f;
    float halfHeight = height / 2.0f;
    float centerY = halfHeight;  // AABB spans [0, height]
    buf->addBox(0, 50, centerY, 25, halfHeight, 0xFFFFFFFF, 0, 0, 0, 0);
    return buf;
}

//=============================================================================
// Tests
//=============================================================================

suite scrolling_tests = [] {
    "scrolling_mode_construction"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        // Create builder WITHOUT scrolling mode
        auto builderNoScroll = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, false);
        expect(builderNoScroll->scrollingMode() == false);

        // Create builder WITH scrolling mode
        auto builderScroll = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 1, true);
        expect(builderScroll->scrollingMode() == true);
    };

    "cursor_position_default"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Default cursor position should be (0, 0)
        expect(builder->cursorCol() == 0_u);
        expect(builder->cursorRow() == 0_u);
    };

    "cursor_position_set"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        builder->setCursorPosition(5, 10);
        expect(builder->cursorCol() == 5_u);
        expect(builder->cursorRow() == 10_u);

        builder->setCursorPosition(100, 200);
        expect(builder->cursorCol() == 100_u);
        expect(builder->cursorRow() == 200_u);
    };

    "scene_height_in_lines"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Set scene bounds: 0,0 to 100,200 with cell size 10x20
        builder->setSceneBounds(0, 0, 100, 200);
        builder->setGridCellSize(10.0f, 20.0f);

        // Scene height = 200, cell height = 20 → 10 lines
        expect(builder->sceneHeightInLines() == 10_u);

        // Change cell size
        builder->setGridCellSize(10.0f, 50.0f);
        // Scene height = 200, cell height = 50 → 4 lines
        expect(builder->sceneHeightInLines() == 4_u);
    };

    //=========================================================================
    // COMPREHENSIVE SCROLLING TESTS
    //=========================================================================

    "scroll_100_single_line_shapes"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Terminal: 24 lines visible
        const uint32_t VISIBLE_LINES = 24;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 800, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        expect(builder->sceneHeightInLines() == VISIBLE_LINES);

        uint32_t cursorRow = 0;

        // Add 100 single-line shapes, simulating terminal behavior
        for (uint32_t i = 0; i < 100; ++i) {
            // Scroll BEFORE adding if cursor would overflow
            // This matches how terminal works: cursor at bottom, newline causes scroll
            if (cursorRow >= VISIBLE_LINES) {
                uint32_t overflow = cursorRow - VISIBLE_LINES + 1;
                builder->scrollLines(static_cast<uint16_t>(overflow));
                cursorRow -= overflow;
            }

            builder->setCursorPosition(0, static_cast<uint16_t>(cursorRow));
            auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
            auto result = builder->addYpaintBuffer(buf);
            expect(result.has_value()) << "addYpaintBuffer failed at i=" << i;

            // Advance cursor by 1 line (shape height)
            cursorRow++;
        }

        // After adding 100 shapes to 24-line display with proper scrolling,
        // we should have exactly VISIBLE_LINES primitives
        uint32_t primCount = builder->primitiveCount();
        expect(primCount > 0_u) << "Should have at least some primitives, got " << primCount;
        expect(primCount == VISIBLE_LINES) << "Should have exactly " << VISIBLE_LINES
                                           << " prims, got " << primCount;

        // Verify no crashes when building
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();
    };

    "scroll_200_shapes_1_to_5_lines"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Terminal: 30 lines visible
        const uint32_t VISIBLE_LINES = 30;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 800, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        std::mt19937 rng(42);  // Fixed seed for reproducibility
        std::uniform_int_distribution<uint32_t> lineDist(1, 5);

        uint32_t currentRow = 0;

        // Add 200 shapes with varying heights (1-5 lines each)
        for (uint32_t i = 0; i < 200; ++i) {
            uint32_t shapeLines = lineDist(rng);

            builder->setCursorPosition(0, static_cast<uint16_t>(currentRow));
            auto buf = createShapeSpanningLines(shapeLines, CELL_HEIGHT);
            auto result = builder->addYpaintBuffer(buf);
            expect(result.has_value()) << "addYpaintBuffer failed at i=" << i;

            // Advance cursor by shape height
            currentRow += shapeLines;

            // If we overflow, scrollLines will be called by onScrollRect in real use
            // Here we simulate by calling scrollLines directly when cursor exceeds visible
            while (currentRow >= VISIBLE_LINES) {
                builder->scrollLines(1);
                currentRow--;
            }
        }

        // Should have primitives
        uint32_t primCount = builder->primitiveCount();
        expect(primCount > 0_u) << "Should have primitives after 200 additions";

        // Verify no crashes when building
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();
    };

    "scroll_multiple_pages_sequential"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Terminal: 20 lines
        const uint32_t VISIBLE_LINES = 20;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 800, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Add 5 pages worth of content (100 shapes, 1 line each = 5 pages)
        uint32_t totalShapes = VISIBLE_LINES * 5;
        uint32_t cursorRow = 0;

        for (uint32_t i = 0; i < totalShapes; ++i) {
            // If cursor is at last line and we're about to add, scroll first
            // This simulates the terminal scrolling when cursor is at bottom
            if (cursorRow >= VISIBLE_LINES) {
                uint32_t overflow = cursorRow - VISIBLE_LINES + 1;
                builder->scrollLines(static_cast<uint16_t>(overflow));
                cursorRow -= overflow;
            }

            builder->setCursorPosition(0, static_cast<uint16_t>(cursorRow));
            auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
            builder->addYpaintBuffer(buf);

            cursorRow++;
        }

        // After 5 pages, we should have exactly VISIBLE_LINES primitives
        uint32_t primCount = builder->primitiveCount();
        expect(primCount == VISIBLE_LINES) << "Should have " << VISIBLE_LINES
                                           << " prims after 5 pages, got " << primCount;

        // Build and verify
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();
    };

    "cursor_jump_to_arbitrary_position"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Terminal: 24 lines
        const uint32_t VISIBLE_LINES = 24;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 800, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Fill first 10 lines
        for (uint32_t i = 0; i < 10; ++i) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i));
            auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
            builder->addYpaintBuffer(buf);
        }
        expect(builder->primitiveCount() == 10_u) << "Should have 10 prims";

        // Jump cursor to line 20 and add more shapes
        builder->setCursorPosition(0, 20);
        for (uint32_t i = 0; i < 5; ++i) {
            builder->setCursorPosition(0, static_cast<uint16_t>(20 + i));
            auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
            builder->addYpaintBuffer(buf);

            // Scroll if needed (simulating vterm)
            if (20 + i >= VISIBLE_LINES) {
                uint32_t overflow = (20 + i + 1) - VISIBLE_LINES;
                builder->scrollLines(static_cast<uint16_t>(overflow));
            }
        }

        // Should still have primitives
        uint32_t primCount = builder->primitiveCount();
        expect(primCount > 0_u) << "Should have prims after cursor jump";

        // Build and verify
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();
    };

    "cursor_random_positions_500_shapes"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Terminal: 40 lines
        const uint32_t VISIBLE_LINES = 40;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 800, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        std::mt19937 rng(123);
        std::uniform_int_distribution<uint32_t> rowDist(0, VISIBLE_LINES - 1);
        std::uniform_int_distribution<uint32_t> lineDist(1, 3);

        // Add 500 shapes at random cursor positions
        for (uint32_t i = 0; i < 500; ++i) {
            uint32_t row = rowDist(rng);
            uint32_t lines = lineDist(rng);

            builder->setCursorPosition(0, static_cast<uint16_t>(row));
            auto buf = createShapeSpanningLines(lines, CELL_HEIGHT);
            auto result = builder->addYpaintBuffer(buf);
            expect(result.has_value()) << "addYpaintBuffer failed at i=" << i;

            // Simulate scroll if content overflows
            if (row + lines > VISIBLE_LINES) {
                uint32_t overflow = row + lines - VISIBLE_LINES;
                builder->scrollLines(static_cast<uint16_t>(overflow));
            }
        }

        // Should have primitives
        uint32_t primCount = builder->primitiveCount();
        expect(primCount > 0_u) << "Should have prims after 500 random adds";

        // Build and verify no crashes
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();
    };

    "scroll_then_add_more"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Terminal: 10 lines
        const uint32_t VISIBLE_LINES = 10;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 800, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Fill all 10 lines
        for (uint32_t i = 0; i < 10; ++i) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i));
            auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
            builder->addYpaintBuffer(buf);
        }
        expect(builder->primitiveCount() == 10_u);

        // Now scroll 5 lines
        builder->scrollLines(5);
        expect(builder->primitiveCount() == 5_u) << "Should have 5 prims after scrolling 5 lines";

        // Add 5 more shapes at positions 5-9
        for (uint32_t i = 5; i < 10; ++i) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i));
            auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
            builder->addYpaintBuffer(buf);
        }
        expect(builder->primitiveCount() == 10_u) << "Should have 10 prims after refilling";

        // Scroll 3 more
        builder->scrollLines(3);
        expect(builder->primitiveCount() == 7_u) << "Should have 7 prims after scrolling 3 more";

        // Build
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();
    };

    "multi_line_shapes_scroll_correctly"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Terminal: 10 lines
        const uint32_t VISIBLE_LINES = 10;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 800, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Add shape spanning lines 0-2 (3 lines)
        builder->setCursorPosition(0, 0);
        auto buf1 = createShapeSpanningLines(3, CELL_HEIGHT);
        builder->addYpaintBuffer(buf1);
        expect(builder->primitiveCount() == 1_u);

        // Add shape spanning lines 3-5 (3 lines)
        builder->setCursorPosition(0, 3);
        auto buf2 = createShapeSpanningLines(3, CELL_HEIGHT);
        builder->addYpaintBuffer(buf2);
        expect(builder->primitiveCount() == 2_u);

        // Add shape spanning lines 6-9 (4 lines)
        builder->setCursorPosition(0, 6);
        auto buf3 = createShapeSpanningLines(4, CELL_HEIGHT);
        builder->addYpaintBuffer(buf3);
        expect(builder->primitiveCount() == 3_u);

        // Scroll 1 line - shape at 0-2 should still be visible (now at -1 to 1, partial)
        builder->scrollLines(1);
        // Shape 1 base was line 2, now line 1 - still exists
        expect(builder->primitiveCount() == 3_u);

        // Scroll 2 more lines - shape at 0-2 should be gone (base was line 2, now -1)
        builder->scrollLines(2);
        // Shape 1 base was line 2, after scroll 3 total, base is now line -1 (gone)
        expect(builder->primitiveCount() == 2_u) << "First shape should be scrolled out";

        // Scroll 3 more - shape at 3-5 should be gone
        builder->scrollLines(3);
        expect(builder->primitiveCount() == 1_u) << "Second shape should be scrolled out";

        // Build
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();
    };

    "stress_1000_shapes_mixed_sizes"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Terminal: 50 lines
        const uint32_t VISIBLE_LINES = 50;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 800, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        std::mt19937 rng(999);
        std::uniform_int_distribution<uint32_t> lineDist(1, 5);

        uint32_t cursorRow = 0;

        // Add 1000 shapes
        for (uint32_t i = 0; i < 1000; ++i) {
            uint32_t lines = lineDist(rng);

            builder->setCursorPosition(0, static_cast<uint16_t>(cursorRow));
            auto buf = createShapeSpanningLines(lines, CELL_HEIGHT);
            auto result = builder->addYpaintBuffer(buf);
            expect(result.has_value()) << "addYpaintBuffer failed at i=" << i;

            cursorRow += lines;

            // Scroll when overflow
            while (cursorRow >= VISIBLE_LINES) {
                builder->scrollLines(1);
                cursorRow--;
            }
        }

        // Should have primitives fitting in visible area
        uint32_t primCount = builder->primitiveCount();
        expect(primCount > 0_u) << "Should have prims after 1000 stress adds";
        expect(primCount <= (VISIBLE_LINES * 5)) << "Should not have excessive prims";

        // Build without crashes
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();
    };

    "verify_refs_cleaned_after_scroll"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Terminal: 5 lines
        const uint32_t VISIBLE_LINES = 5;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 100, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Add shape at line 0
        builder->setCursorPosition(0, 0);
        auto buf0 = createShapeSpanningLines(1, CELL_HEIGHT);
        builder->addYpaintBuffer(buf0);

        // Add shape at line 4
        builder->setCursorPosition(0, 4);
        auto buf4 = createShapeSpanningLines(1, CELL_HEIGHT);
        builder->addYpaintBuffer(buf4);

        expect(builder->primitiveCount() == 2_u);

        // Scroll 1 line - shape at line 0 should be gone
        builder->scrollLines(1);
        expect(builder->primitiveCount() == 1_u) << "Should have 1 prim after scroll";

        // Build and verify grid is valid
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        // Verify hasContent still works
        expect(builder->hasContent() == true) << "Should still have content";
    };

    "scroll_entire_screen_multiple_times"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Terminal: 10 lines
        const uint32_t VISIBLE_LINES = 10;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 800, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Fill screen, scroll entire screen, repeat 10 times
        for (uint32_t page = 0; page < 10; ++page) {
            // Fill all lines
            for (uint32_t i = 0; i < VISIBLE_LINES; ++i) {
                builder->setCursorPosition(0, static_cast<uint16_t>(i));
                auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
                builder->addYpaintBuffer(buf);
            }

            expect(builder->primitiveCount() == VISIBLE_LINES)
                << "Page " << page << ": should have " << VISIBLE_LINES << " prims";

            // Scroll entire screen
            builder->scrollLines(static_cast<uint16_t>(VISIBLE_LINES));
            expect(builder->primitiveCount() == 0_u)
                << "Page " << page << ": should have 0 prims after full scroll";
        }

        // Final state: empty
        expect(builder->hasContent() == false);
    };

    "cursor_at_each_row_then_scroll"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Terminal: 20 lines
        const uint32_t VISIBLE_LINES = 20;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 800, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Test setting cursor at every possible row
        for (uint32_t row = 0; row < VISIBLE_LINES; ++row) {
            builder->setCursorPosition(5, static_cast<uint16_t>(row));
            expect(builder->cursorRow() == row);
            expect(builder->cursorCol() == 5_u);

            auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
            builder->addYpaintBuffer(buf);
        }

        expect(builder->primitiveCount() == VISIBLE_LINES);

        // Scroll each row one by one and verify count decreases
        for (uint32_t i = 0; i < VISIBLE_LINES; ++i) {
            builder->scrollLines(1);
            expect(builder->primitiveCount() == VISIBLE_LINES - i - 1)
                << "After scrolling " << (i + 1) << " lines";
        }

        expect(builder->primitiveCount() == 0_u);
        expect(builder->hasContent() == false);
    };

    "lightweight_factory_scrolling"_test = [] {
        auto gpuAlloc = testAllocator();

        // Test lightweight factory with scrolling mode
        auto builder = *Painter::create(FontManager::Ptr{}, gpuAlloc, true);
        expect(builder->scrollingMode() == true);

        auto builder2 = *Painter::create(FontManager::Ptr{}, gpuAlloc, false);
        expect(builder2->scrollingMode() == false);
    };

    //=========================================================================
    // GRID HEALTH VERIFICATION TESTS
    // Verify grid integrity after scrolling operations
    //=========================================================================

    "grid_health_after_100_scrolls"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 20;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 400, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        uint32_t cursorRow = 0;

        // Add 100 shapes and scroll, verify grid can be built each time
        for (uint32_t i = 0; i < 100; ++i) {
            if (cursorRow >= VISIBLE_LINES) {
                builder->scrollLines(1);
                cursorRow--;
            }

            builder->setCursorPosition(0, static_cast<uint16_t>(cursorRow));
            auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
            auto result = builder->addYpaintBuffer(buf);
            expect(result.has_value()) << "addYpaintBuffer failed at i=" << i;
            cursorRow++;

            // Verify grid can be rebuilt after each operation
            builder->declareBufferNeeds();
            auto commitRes = cardMgr->mockBufMgr()->commitReservations();
            expect(commitRes.has_value()) << "commitReservations failed at i=" << i;

            auto allocRes = builder->allocateBuffers();
            expect(allocRes.has_value()) << "allocateBuffers failed at i=" << i;

            auto writeRes = builder->writeBuffers();
            expect(writeRes.has_value()) << "writeBuffers failed at i=" << i;
        }

        // Final check
        expect(builder->primitiveCount() == VISIBLE_LINES)
            << "Final primitive count should be " << VISIBLE_LINES;
    };

    "grid_health_multiline_shapes_scroll"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 15;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 400, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Add shapes spanning 1-5 lines, scroll progressively
        std::mt19937 rng(777);
        std::uniform_int_distribution<uint32_t> lineDist(1, 5);
        uint32_t cursorRow = 0;

        for (uint32_t i = 0; i < 50; ++i) {
            uint32_t shapeLines = lineDist(rng);

            // Scroll if needed
            while (cursorRow + shapeLines > VISIBLE_LINES) {
                builder->scrollLines(1);
                if (cursorRow > 0) cursorRow--;
            }

            builder->setCursorPosition(0, static_cast<uint16_t>(cursorRow));
            auto buf = createShapeSpanningLines(shapeLines, CELL_HEIGHT);
            auto result = builder->addYpaintBuffer(buf);
            expect(result.has_value()) << "addYpaintBuffer failed at i=" << i;

            cursorRow += shapeLines;

            // Verify grid integrity every 10 iterations
            if (i % 10 == 0) {
                builder->declareBufferNeeds();
                cardMgr->mockBufMgr()->commitReservations();
                auto allocRes = builder->allocateBuffers();
                expect(allocRes.has_value()) << "allocateBuffers failed at i=" << i;
                auto writeRes = builder->writeBuffers();
                expect(writeRes.has_value()) << "writeBuffers failed at i=" << i;
            }
        }

        // Final grid build
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        expect(builder->primitiveCount() > 0_u) << "Should have primitives";
        expect(builder->hasContent() == true) << "Should have content";
    };

    "grid_health_stress_500_shapes_random_positions"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 30;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 600, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        std::mt19937 rng(555);
        std::uniform_int_distribution<uint32_t> rowDist(0, VISIBLE_LINES - 1);
        std::uniform_int_distribution<uint32_t> lineDist(1, 4);
        std::uniform_int_distribution<uint32_t> scrollDist(0, 3);

        for (uint32_t i = 0; i < 500; ++i) {
            // Sometimes scroll randomly
            if (scrollDist(rng) == 0 && builder->primitiveCount() > 0) {
                uint32_t scrollAmount = std::min(3u, builder->primitiveCount());
                builder->scrollLines(static_cast<uint16_t>(scrollAmount));
            }

            uint32_t row = rowDist(rng);
            uint32_t lines = lineDist(rng);

            builder->setCursorPosition(0, static_cast<uint16_t>(row));
            auto buf = createShapeSpanningLines(lines, CELL_HEIGHT);
            auto result = builder->addYpaintBuffer(buf);
            expect(result.has_value()) << "addYpaintBuffer failed at i=" << i;

            // Scroll if content overflows
            if (row + lines > VISIBLE_LINES) {
                uint32_t overflow = row + lines - VISIBLE_LINES;
                builder->scrollLines(static_cast<uint16_t>(overflow));
            }
        }

        // Final build to verify grid integrity
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        auto allocRes = builder->allocateBuffers();
        expect(allocRes.has_value()) << "Final allocateBuffers failed";
        auto writeRes = builder->writeBuffers();
        expect(writeRes.has_value()) << "Final writeBuffers failed";
    };

    "grid_health_verify_buffer_contents"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 10;
        const float CELL_HEIGHT = 20.0f;
        const float CELL_WIDTH = 10.0f;
        builder->setSceneBounds(0, 0, 100, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(CELL_WIDTH, CELL_HEIGHT);

        // Add 5 shapes
        for (uint32_t i = 0; i < 5; ++i) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i * 2));
            auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
            builder->addYpaintBuffer(buf);
        }

        // Build
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        // Verify primitive count
        expect(builder->primitiveCount() == 5_u);

        // Read metadata and verify structure
        const uint32_t* meta = cardMgr->metadataAsU32();
        uint32_t primOffset = meta[0];
        uint32_t primCount = meta[1];
        uint32_t gridOffset = meta[2];
        uint32_t gridWidth = meta[3];
        uint32_t gridHeight = meta[4];

        expect(primCount == 5_u) << "Metadata primCount should be 5";
        expect(gridWidth > 0_u) << "Grid width should be > 0";
        expect(gridHeight > 0_u) << "Grid height should be > 0";
        expect(gridHeight == VISIBLE_LINES) << "Grid height should match visible lines";

        // Scroll 2 lines and rebuild
        builder->scrollLines(2);
        expect(builder->primitiveCount() == 4_u) << "After scroll, should have 4 prims";

        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        // Verify metadata updated
        primCount = meta[1];
        expect(primCount == 4_u) << "Metadata primCount should be 4 after scroll";
    };

    "grid_health_extreme_scroll_1000_lines"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 25;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 500, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        uint32_t cursorRow = 0;

        // Add 1000 single-line shapes with scrolling
        for (uint32_t i = 0; i < 1000; ++i) {
            if (cursorRow >= VISIBLE_LINES) {
                builder->scrollLines(1);
                cursorRow--;
            }

            builder->setCursorPosition(0, static_cast<uint16_t>(cursorRow));
            auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
            builder->addYpaintBuffer(buf);
            cursorRow++;
        }

        // After 1000 adds with scrolling, should have VISIBLE_LINES prims
        expect(builder->primitiveCount() == VISIBLE_LINES)
            << "After 1000 shapes, should have " << VISIBLE_LINES << " prims";

        // Build and verify
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        auto allocRes = builder->allocateBuffers();
        expect(allocRes.has_value()) << "allocateBuffers failed after 1000 shapes";
        auto writeRes = builder->writeBuffers();
        expect(writeRes.has_value()) << "writeBuffers failed after 1000 shapes";
    };

    "grid_health_clear_and_refill"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 20;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 400, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Fill, scroll all, refill - repeat 5 times
        for (uint32_t cycle = 0; cycle < 5; ++cycle) {
            // Fill
            for (uint32_t i = 0; i < VISIBLE_LINES; ++i) {
                builder->setCursorPosition(0, static_cast<uint16_t>(i));
                auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
                builder->addYpaintBuffer(buf);
            }
            expect(builder->primitiveCount() == VISIBLE_LINES)
                << "Cycle " << cycle << ": should have " << VISIBLE_LINES << " prims after fill";

            // Build to verify
            builder->declareBufferNeeds();
            cardMgr->mockBufMgr()->commitReservations();
            builder->allocateBuffers();
            builder->writeBuffers();

            // Clear by scrolling all
            builder->scrollLines(static_cast<uint16_t>(VISIBLE_LINES));
            expect(builder->primitiveCount() == 0_u)
                << "Cycle " << cycle << ": should have 0 prims after clear";
            expect(builder->hasContent() == false)
                << "Cycle " << cycle << ": hasContent should be false";
        }
    };

    "grid_refs_validity_after_partial_scroll"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 10;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 200, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Add shapes at specific positions
        // Shape A at lines 0-2 (3 lines)
        builder->setCursorPosition(0, 0);
        auto bufA = createShapeSpanningLines(3, CELL_HEIGHT);
        builder->addYpaintBuffer(bufA);

        // Shape B at lines 4-6 (3 lines)
        builder->setCursorPosition(0, 4);
        auto bufB = createShapeSpanningLines(3, CELL_HEIGHT);
        builder->addYpaintBuffer(bufB);

        // Shape C at lines 8-9 (2 lines)
        builder->setCursorPosition(0, 8);
        auto bufC = createShapeSpanningLines(2, CELL_HEIGHT);
        builder->addYpaintBuffer(bufC);

        expect(builder->primitiveCount() == 3_u);

        // Build and verify
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        // Scroll 1 line - Shape A should still exist (base at line 2, now line 1)
        builder->scrollLines(1);
        expect(builder->primitiveCount() == 3_u) << "After scroll 1: all shapes should exist";

        // Rebuild grid
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        // Scroll 2 more lines - Shape A base was at 2, after scroll 3, base at -1 (gone)
        builder->scrollLines(2);
        expect(builder->primitiveCount() == 2_u) << "After scroll 3: Shape A should be gone";

        // Rebuild
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        auto res = builder->allocateBuffers();
        expect(res.has_value()) << "allocateBuffers should succeed after partial scroll";
        res = builder->writeBuffers();
        expect(res.has_value()) << "writeBuffers should succeed after partial scroll";
    };

    //=========================================================================
    // GPU BUFFER LAYOUT VERIFICATION TESTS
    // These tests verify the EXACT format of gridStaging() and primitive data
    // that the shader will read. Critical for catching rendering bugs.
    //=========================================================================

    "gpu_layout_grid_staging_format"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 5;
        const float CELL_HEIGHT = 20.0f;
        const float CELL_WIDTH = 10.0f;
        builder->setSceneBounds(0, 0, 100, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(CELL_WIDTH, CELL_HEIGHT);

        // Add one shape at line 0
        builder->setCursorPosition(0, 0);
        auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
        builder->addYpaintBuffer(buf);

        // Get grid staging
        const auto& grid = builder->gridStaging();
        uint32_t gridW = builder->gridWidth();
        uint32_t gridH = builder->gridHeight();
        uint32_t numCells = gridW * gridH;

        expect(grid.size() > numCells) << "Grid staging should have offsets + entries";
        expect(gridW > 0_u) << "Grid width should be > 0";
        expect(gridH > 0_u) << "Grid height should be > 0";

        // First numCells entries are offsets into packed data
        // Each offset should point to a valid position in the array
        for (uint32_t i = 0; i < numCells; i++) {
            uint32_t offset = grid[i];
            expect(offset >= numCells) << "Cell " << i << " offset " << offset
                                       << " should be >= numCells " << numCells;
            expect(offset < grid.size()) << "Cell " << i << " offset " << offset
                                         << " should be < grid.size " << grid.size();
        }

        // At each offset, first entry is count, followed by that many prim indices
        for (uint32_t i = 0; i < numCells; i++) {
            uint32_t offset = grid[i];
            uint32_t count = grid[offset];
            // Verify we have enough space for count + indices
            expect(offset + 1 + count <= grid.size())
                << "Cell " << i << " at offset " << offset << " with count " << count
                << " overflows grid.size " << grid.size();
        }
    };

    "gpu_layout_verify_prim_indices_valid"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 10;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 200, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(20.0f, CELL_HEIGHT);

        // Add 5 shapes
        for (uint32_t i = 0; i < 5; i++) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i * 2));
            auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
            builder->addYpaintBuffer(buf);
        }

        const auto& grid = builder->gridStaging();
        uint32_t gridW = builder->gridWidth();
        uint32_t gridH = builder->gridHeight();
        uint32_t numCells = gridW * gridH;
        uint32_t primCount = builder->primitiveCount();

        expect(primCount == 5_u) << "Should have 5 primitives";

        // Verify all primitive indices in grid cells are < primCount
        for (uint32_t cellIdx = 0; cellIdx < numCells; cellIdx++) {
            uint32_t offset = grid[cellIdx];
            uint32_t count = grid[offset];
            for (uint32_t j = 0; j < count; j++) {
                uint32_t primIdx = grid[offset + 1 + j];
                // Check if it's a glyph (high bit set) or primitive
                if ((primIdx & 0x80000000u) == 0) {
                    expect(primIdx < primCount)
                        << "Cell " << cellIdx << " entry " << j
                        << " has invalid primIdx " << primIdx
                        << " >= primCount " << primCount;
                }
            }
        }
    };

    "gpu_layout_after_scroll_grid_valid"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 10;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 200, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(20.0f, CELL_HEIGHT);

        // Add 10 shapes, one per line
        for (uint32_t i = 0; i < 10; i++) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i));
            auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
            builder->addYpaintBuffer(buf);
        }

        expect(builder->primitiveCount() == 10_u);

        // Scroll 3 lines
        builder->scrollLines(3);
        expect(builder->primitiveCount() == 7_u);

        // Verify grid is still valid after scroll
        const auto& grid = builder->gridStaging();
        uint32_t gridW = builder->gridWidth();
        uint32_t gridH = builder->gridHeight();
        uint32_t numCells = gridW * gridH;
        uint32_t primCount = builder->primitiveCount();

        expect(grid.size() > 0_u) << "Grid should not be empty after scroll";

        // Verify all offsets are valid
        for (uint32_t i = 0; i < numCells && i < grid.size(); i++) {
            uint32_t offset = grid[i];
            expect(offset < grid.size())
                << "After scroll: cell " << i << " offset " << offset
                << " >= grid.size " << grid.size();

            if (offset < grid.size()) {
                uint32_t count = grid[offset];
                expect(offset + 1 + count <= grid.size())
                    << "After scroll: cell " << i << " overflow";

                // Verify prim indices
                for (uint32_t j = 0; j < count; j++) {
                    uint32_t primIdx = grid[offset + 1 + j];
                    if ((primIdx & 0x80000000u) == 0) {
                        expect(primIdx < primCount)
                            << "After scroll: cell " << i << " has invalid primIdx "
                            << primIdx << " >= " << primCount;
                    }
                }
            }
        }
    };

    "gpu_layout_prim_row_offset_after_scroll"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 10;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 200, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(20.0f, CELL_HEIGHT);

        // Add shape at row 5
        builder->setCursorPosition(0, 5);
        auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
        builder->addYpaintBuffer(buf);

        // Build to get primitive staging
        std::vector<uint32_t> primStaging;
        builder->buildPrimStaging(primStaging);

        // primStaging format for scrolling mode:
        // [0..primCount-1] = offset for each primitive
        // [primCount + offset + 0] = packed(col | row<<16) = gridOffset
        // [primCount + offset + 1] = type
        // [primCount + offset + 2] = layer
        // [primCount + offset + 3+] = geometry
        uint32_t primCount = builder->primitiveCount();
        expect(primCount == 1_u) << "Should have 1 primitive";
        expect(primStaging.size() > primCount + 2) << "Prim staging should have primitive data";

        uint32_t offset = primStaging[0];  // offset for prim 0
        uint32_t packed = primStaging[primCount + offset + 0];  // word[0] = gridOffset (packed row/col)
        int16_t initialRow = static_cast<int16_t>((packed >> 16) & 0xFFFF);
        expect(initialRow == 5_i) << "Initial row should be 5, got " << initialRow;

        // Now scroll 2 lines
        builder->scrollLines(2);

        // Rebuild prim staging
        primStaging.clear();
        builder->buildPrimStaging(primStaging);

        primCount = builder->primitiveCount();
        expect(primCount == 1_u) << "Should still have 1 primitive after scroll 2";

        offset = primStaging[0];
        packed = primStaging[primCount + offset + 0];  // word[0] = gridOffset
        int16_t newRow = static_cast<int16_t>((packed >> 16) & 0xFFFF);
        expect(newRow == 3_i) << "After scroll 2, row should be 3, got " << newRow;

        // Scroll 4 more lines (row becomes -1)
        builder->scrollLines(4);

        primStaging.clear();
        builder->buildPrimStaging(primStaging);

        primCount = builder->primitiveCount();
        // After scrolling 6 total, primitive at baseLine 5 should be scrolled out
        // (baseLine 5 - scroll 6 = -1, line is gone)
        expect(primCount == 0_u) << "Primitive should be gone after scroll 6, got " << primCount;
    };

    "gpu_layout_incremental_add_verify_each_step"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 10;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 200, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(20.0f, CELL_HEIGHT);

        // Add shapes one by one and verify grid after each
        for (uint32_t i = 0; i < 5; i++) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i * 2));
            auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
            builder->addYpaintBuffer(buf);

            // Verify grid after this add
            const auto& grid = builder->gridStaging();
            uint32_t primCount = builder->primitiveCount();

            expect(primCount == i + 1)
                << "After add " << i << ", primCount should be " << (i + 1)
                << ", got " << primCount;

            expect(grid.size() > 0_u)
                << "After add " << i << ", grid should not be empty";

            // Verify structure
            uint32_t gridW = builder->gridWidth();
            uint32_t gridH = builder->gridHeight();
            uint32_t numCells = gridW * gridH;

            for (uint32_t c = 0; c < numCells && c < grid.size(); c++) {
                uint32_t offset = grid[c];
                expect(offset < grid.size())
                    << "Add " << i << " cell " << c << " bad offset";
            }
        }
    };

    "gpu_layout_scroll_step_by_step_verify"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 5;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 100, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Add 5 shapes, one per line
        for (uint32_t i = 0; i < 5; i++) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i));
            auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
            builder->addYpaintBuffer(buf);
        }

        expect(builder->primitiveCount() == 5_u);

        // Scroll one line at a time and verify grid each time
        for (uint32_t scrolled = 1; scrolled <= 5; scrolled++) {
            builder->scrollLines(1);

            uint32_t expectedPrims = 5 - scrolled;
            expect(builder->primitiveCount() == expectedPrims)
                << "After scrolling " << scrolled << " lines, expected "
                << expectedPrims << " prims, got " << builder->primitiveCount();

            const auto& grid = builder->gridStaging();

            if (expectedPrims > 0) {
                expect(grid.size() > 0_u)
                    << "After scroll " << scrolled << ", grid should not be empty";

                // Verify grid structure
                uint32_t gridW = builder->gridWidth();
                uint32_t gridH = builder->gridHeight();
                uint32_t numCells = gridW * gridH;

                for (uint32_t c = 0; c < numCells && c < grid.size(); c++) {
                    uint32_t offset = grid[c];
                    expect(offset < grid.size())
                        << "Scroll " << scrolled << " cell " << c
                        << " offset " << offset << " >= " << grid.size();

                    if (offset < grid.size()) {
                        uint32_t count = grid[offset];
                        for (uint32_t j = 0; j < count; j++) {
                            uint32_t primIdx = grid[offset + 1 + j];
                            if ((primIdx & 0x80000000u) == 0) {
                                expect(primIdx < expectedPrims)
                                    << "Scroll " << scrolled << " cell " << c
                                    << " primIdx " << primIdx << " >= " << expectedPrims;
                            }
                        }
                    }
                }
            }
        }
    };

    // Visual grid dump - only uses cout for visual verification
    auto dumpGridState = [](Painter* builder, const char* label) {
        const auto& grid = builder->gridStaging();
        uint32_t gridW = builder->gridWidth();
        uint32_t gridH = builder->gridHeight();
        uint32_t primCount = builder->primitiveCount();

        std::cout << "\n=== " << label << " ===" << std::endl;
        std::cout << "Grid: " << gridW << "x" << gridH << ", prims: " << primCount << std::endl;

        for (uint32_t row = 0; row < gridH; row++) {
            // Check if row has any content
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

            // Only print rows with content or every 10th row for context
            if (hasContent || row % 10 == 0) {
                std::cout << "Row " << std::setw(3) << row << ": ";
                for (uint32_t col = 0; col < gridW && col < 40; col++) {
                    uint32_t cellIdx = row * gridW + col;
                    if (cellIdx < grid.size()) {
                        uint32_t offset = grid[cellIdx];
                        if (offset < grid.size()) {
                            uint32_t count = grid[offset];
                            if (count > 0) {
                                std::cout << count;
                            } else {
                                std::cout << ".";
                            }
                        } else {
                            std::cout << "?";
                        }
                    }
                }
                if (gridW > 40) std::cout << "...";
                std::cout << std::endl;
            }
        }
        std::cout << std::flush;
    };

    "gpu_layout_multiline_shape_grid_coverage"_test = [&dumpGridState] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 10;
        const float CELL_HEIGHT = 20.0f;
        const float CELL_WIDTH = 50.0f;
        builder->setSceneBounds(0, 0, 100, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(CELL_WIDTH, CELL_HEIGHT);

        // Add shape spanning lines 2-5 (4 lines)
        // Shape AABB: Y=[0, 79], with cursor at row 2 -> primLines=[2,5]
        builder->setCursorPosition(0, 2);
        auto buf = createShapeSpanningLines(4, CELL_HEIGHT);
        builder->addYpaintBuffer(buf);

        dumpGridState(builder.get(), "After addYpaintBuffer at cursor (0,2)");

        const auto& grid = builder->gridStaging();
        uint32_t gridW = builder->gridWidth();
        uint32_t gridH = builder->gridHeight();

        expect(gridH >= 6_u) << "Grid should have at least 6 rows for shape at lines 2-5";

        // The shape should appear in cells for rows 2, 3, 4, 5
        for (uint32_t row = 2; row < 6 && row < gridH; row++) {
            bool foundInRow = false;
            for (uint32_t col = 0; col < gridW; col++) {
                uint32_t cellIdx = row * gridW + col;
                if (cellIdx < grid.size()) {
                    uint32_t offset = grid[cellIdx];
                    if (offset < grid.size()) {
                        uint32_t count = grid[offset];
                        if (count > 0) {
                            foundInRow = true;
                            break;
                        }
                    }
                }
            }
            expect(foundInRow) << "Shape should be referenced in row " << row;
        }
    };

    "gpu_layout_cursor_at_10_10"_test = [&dumpGridState] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 20;
        const float CELL_HEIGHT = 20.0f;
        const float CELL_WIDTH = 50.0f;
        builder->setSceneBounds(0, 0, 500, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(CELL_WIDTH, CELL_HEIGHT);

        // Cursor at (10, 10), add shape spanning 3 lines
        builder->setCursorPosition(10, 10);
        auto buf = createShapeSpanningLines(3, CELL_HEIGHT);
        builder->addYpaintBuffer(buf);

        dumpGridState(builder.get(), "After addYpaintBuffer at cursor (10,10)");

        const auto& grid = builder->gridStaging();
        uint32_t gridW = builder->gridWidth();
        uint32_t gridH = builder->gridHeight();

        // Shape AABB: Y=[0, 59], with cursor at row 10 -> primLines=[10,12]
        for (uint32_t row = 10; row <= 12 && row < gridH; row++) {
            bool foundInRow = false;
            for (uint32_t col = 0; col < gridW; col++) {
                uint32_t cellIdx = row * gridW + col;
                if (cellIdx < grid.size()) {
                    uint32_t offset = grid[cellIdx];
                    if (offset < grid.size()) {
                        uint32_t count = grid[offset];
                        if (count > 0) {
                            foundInRow = true;
                            break;
                        }
                    }
                }
            }
            expect(foundInRow) << "Shape should be referenced in row " << row;
        }
    };

    "gpu_layout_large_grid_progressive"_test = [&dumpGridState] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Large grid: 80 columns x 60 rows
        const uint32_t VISIBLE_LINES = 60;
        const float CELL_HEIGHT = 20.0f;
        const float CELL_WIDTH = 10.0f;
        builder->setSceneBounds(0, 0, 800, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(CELL_WIDTH, CELL_HEIGHT);

        std::cout << "\n========== LARGE GRID PROGRESSIVE TEST ==========" << std::endl;

        // Shape 1: cursor at (0, 0), 2 lines
        builder->setCursorPosition(0, 0);
        builder->addYpaintBuffer(createShapeSpanningLines(2, CELL_HEIGHT));
        dumpGridState(builder.get(), "Shape 1 @ cursor(0,0) 2 lines");

        // Shape 2: cursor at (10, 5), 3 lines
        builder->setCursorPosition(10, 5);
        builder->addYpaintBuffer(createShapeSpanningLines(3, CELL_HEIGHT));
        dumpGridState(builder.get(), "Shape 2 @ cursor(10,5) 3 lines");

        // Shape 3: cursor at (20, 15), 4 lines
        builder->setCursorPosition(20, 15);
        builder->addYpaintBuffer(createShapeSpanningLines(4, CELL_HEIGHT));
        dumpGridState(builder.get(), "Shape 3 @ cursor(20,15) 4 lines");

        // Shape 4: cursor at (5, 25), 2 lines
        builder->setCursorPosition(5, 25);
        builder->addYpaintBuffer(createShapeSpanningLines(2, CELL_HEIGHT));
        dumpGridState(builder.get(), "Shape 4 @ cursor(5,25) 2 lines");

        // Shape 5: cursor at (30, 35), 5 lines
        builder->setCursorPosition(30, 35);
        builder->addYpaintBuffer(createShapeSpanningLines(5, CELL_HEIGHT));
        dumpGridState(builder.get(), "Shape 5 @ cursor(30,35) 5 lines");

        // Shape 6: cursor at (0, 45), 3 lines
        builder->setCursorPosition(0, 45);
        builder->addYpaintBuffer(createShapeSpanningLines(3, CELL_HEIGHT));
        dumpGridState(builder.get(), "Shape 6 @ cursor(0,45) 3 lines");

        expect(builder->primitiveCount() == 6_u) << "Should have 6 primitives";

        // Verify each shape is in expected rows
        auto checkRowsHavePrim = [&](uint32_t startRow, uint32_t endRow, const char* label) {
            const auto& grid = builder->gridStaging();
            uint32_t gridW = builder->gridWidth();
            uint32_t gridH = builder->gridHeight();
            for (uint32_t row = startRow; row <= endRow && row < gridH; row++) {
                bool found = false;
                for (uint32_t col = 0; col < gridW; col++) {
                    uint32_t cellIdx = row * gridW + col;
                    if (cellIdx < grid.size()) {
                        uint32_t offset = grid[cellIdx];
                        if (offset < grid.size() && grid[offset] > 0) {
                            found = true;
                            break;
                        }
                    }
                }
                expect(found) << label << " should be in row " << row;
            }
        };

        checkRowsHavePrim(0, 1, "Shape 1");
        checkRowsHavePrim(5, 7, "Shape 2");
        checkRowsHavePrim(15, 18, "Shape 3");
        checkRowsHavePrim(25, 26, "Shape 4");
        checkRowsHavePrim(35, 39, "Shape 5");
        checkRowsHavePrim(45, 47, "Shape 6");
    };

    "gpu_layout_large_grid_with_scrolling"_test = [&dumpGridState] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Large grid: 80 columns x 40 rows
        const uint32_t VISIBLE_LINES = 40;
        const float CELL_HEIGHT = 20.0f;
        const float CELL_WIDTH = 10.0f;
        builder->setSceneBounds(0, 0, 800, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(CELL_WIDTH, CELL_HEIGHT);

        std::cout << "\n========== LARGE GRID WITH SCROLLING ==========\n";

        builder->setCursorPosition(0, 0);
        builder->addYpaintBuffer(createShapeSpanningLines(3, CELL_HEIGHT));
        dumpGridState(builder.get(), "Step1: cursor(0,0) 3 lines");

        builder->setCursorPosition(5, 5);
        builder->addYpaintBuffer(createShapeSpanningLines(4, CELL_HEIGHT));
        dumpGridState(builder.get(), "Step2: cursor(5,5) 4 lines");

        builder->setCursorPosition(10, 12);
        builder->addYpaintBuffer(createShapeSpanningLines(3, CELL_HEIGHT));
        dumpGridState(builder.get(), "Step3: cursor(10,12) 3 lines");

        std::cout << "\n>>> SCROLL 5 LINES <<<\n";
        builder->scrollLines(5);
        dumpGridState(builder.get(), "After scroll(5)");

        builder->setCursorPosition(15, 20);
        builder->addYpaintBuffer(createShapeSpanningLines(2, CELL_HEIGHT));
        dumpGridState(builder.get(), "Step4: cursor(15,20) 2 lines");

        std::cout << "\n>>> SCROLL 10 LINES <<<\n";
        builder->scrollLines(10);
        dumpGridState(builder.get(), "After scroll(10)");

        builder->setCursorPosition(0, 30);
        builder->addYpaintBuffer(createShapeSpanningLines(5, CELL_HEIGHT));
        dumpGridState(builder.get(), "Step5: cursor(0,30) 5 lines");

        expect(builder->primitiveCount() > 0_u) << "Should have primitives";
    };

    "gpu_layout_terminal_simulation"_test = [&dumpGridState] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Terminal-like: 120 columns x 50 rows
        const uint32_t VISIBLE_LINES = 50;
        const float CELL_HEIGHT = 16.0f;
        const float CELL_WIDTH = 8.0f;
        builder->setSceneBounds(0, 0, 960, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(CELL_WIDTH, CELL_HEIGHT);

        std::cout << "\n========== TERMINAL SIMULATION ==========\n";

        uint16_t cursorRow = 0;

        for (int i = 0; i < 10; i++) {
            uint32_t lines = (i % 3) + 1;
            uint16_t col = static_cast<uint16_t>((i * 7) % 80);

            builder->setCursorPosition(col, cursorRow);
            builder->addYpaintBuffer(createShapeSpanningLines(lines, CELL_HEIGHT));

            cursorRow += lines;

            if (cursorRow >= VISIBLE_LINES - 5) {
                uint16_t scrollAmount = 10;
                std::cout << ">>> SCROLL " << scrollAmount << " <<<\n";
                builder->scrollLines(scrollAmount);
                cursorRow -= scrollAmount;
            }
        }

        dumpGridState(builder.get(), "Final terminal state");

        expect(builder->primitiveCount() > 0_u) << "Should have primitives";

        const auto& grid = builder->gridStaging();
        uint32_t gridW = builder->gridWidth();
        uint32_t gridH = builder->gridHeight();
        uint32_t numCells = gridW * gridH;

        for (uint32_t c = 0; c < numCells && c < grid.size(); c++) {
            uint32_t offset = grid[c];
            expect(offset < grid.size()) << "Cell " << c << " has invalid offset " << offset;
        }
    };

    "gpu_layout_stress_50_shapes_verify_all"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 20;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 400, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(20.0f, CELL_HEIGHT);

        std::mt19937 rng(12345);
        std::uniform_int_distribution<uint32_t> lineDist(1, 3);
        uint32_t cursorRow = 0;

        // Add 50 shapes with scrolling, verify grid after each
        for (uint32_t i = 0; i < 50; i++) {
            uint32_t lines = lineDist(rng);

            // Scroll if needed
            while (cursorRow + lines > VISIBLE_LINES && cursorRow > 0) {
                builder->scrollLines(1);
                cursorRow--;
            }

            builder->setCursorPosition(0, static_cast<uint16_t>(cursorRow));
            auto buf = createShapeSpanningLines(lines, CELL_HEIGHT);
            auto res = builder->addYpaintBuffer(buf);
            expect(res.has_value()) << "addYpaintBuffer failed at i=" << i;

            cursorRow += lines;

            // Verify grid integrity every 5 iterations
            if (i % 5 == 0) {
                const auto& grid = builder->gridStaging();
                uint32_t primCount = builder->primitiveCount();
                uint32_t gridW = builder->gridWidth();
                uint32_t gridH = builder->gridHeight();
                uint32_t numCells = gridW * gridH;

                expect(primCount > 0_u) << "At i=" << i << " should have prims";

                // Verify all cell offsets and prim indices
                for (uint32_t c = 0; c < numCells && c < grid.size(); c++) {
                    uint32_t offset = grid[c];
                    expect(offset < grid.size())
                        << "i=" << i << " cell " << c << " bad offset " << offset;

                    if (offset < grid.size()) {
                        uint32_t count = grid[offset];
                        expect(offset + 1 + count <= grid.size())
                            << "i=" << i << " cell " << c << " overflow";

                        for (uint32_t j = 0; j < count; j++) {
                            uint32_t primIdx = grid[offset + 1 + j];
                            if ((primIdx & 0x80000000u) == 0) {
                                expect(primIdx < primCount)
                                    << "i=" << i << " cell " << c
                                    << " invalid primIdx " << primIdx;
                            }
                        }
                    }
                }
            }
        }
    };

    "gpu_layout_uniforms_correct"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const float SCENE_W = 800.0f;
        const float SCENE_H = 480.0f;
        const float CELL_W = 10.0f;
        const float CELL_H = 20.0f;

        builder->setSceneBounds(0, 0, SCENE_W, SCENE_H);
        builder->setGridCellSize(CELL_W, CELL_H);

        // Add a shape to trigger grid creation
        builder->setCursorPosition(0, 0);
        auto buf = createShapeSpanningLines(1, CELL_H);
        builder->addYpaintBuffer(buf);

        // Verify uniforms match what shader expects
        expect(std::abs(builder->sceneMinX() - 0.0f) < 0.01f);
        expect(std::abs(builder->sceneMinY() - 0.0f) < 0.01f);
        expect(std::abs(builder->sceneMaxX() - SCENE_W) < 0.01f);
        expect(std::abs(builder->sceneMaxY() - SCENE_H) < 0.01f);
        expect(std::abs(builder->cellSizeX() - CELL_W) < 0.01f);
        expect(std::abs(builder->cellSizeY() - CELL_H) < 0.01f);

        uint32_t expectedGridW = static_cast<uint32_t>(std::ceil(SCENE_W / CELL_W));
        uint32_t expectedGridH = static_cast<uint32_t>(std::ceil(SCENE_H / CELL_H));

        expect(builder->gridWidth() == expectedGridW)
            << "Grid width should be " << expectedGridW << ", got " << builder->gridWidth();
        expect(builder->gridHeight() == expectedGridH)
            << "Grid height should be " << expectedGridH << ", got " << builder->gridHeight();
    };

    "gpu_layout_empty_after_full_scroll"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 5;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 100, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Add shapes
        for (uint32_t i = 0; i < 5; i++) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i));
            auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
            builder->addYpaintBuffer(buf);
        }

        // Scroll all out
        builder->scrollLines(5);

        expect(builder->primitiveCount() == 0_u);
        expect(builder->hasContent() == false);

        // Grid should be empty or have all zero counts
        const auto& grid = builder->gridStaging();
        // Empty grid is valid
        if (!grid.empty()) {
            uint32_t gridW = builder->gridWidth();
            uint32_t gridH = builder->gridHeight();
            uint32_t numCells = gridW * gridH;

            uint32_t totalCount = 0;
            for (uint32_t c = 0; c < numCells && c < grid.size(); c++) {
                uint32_t offset = grid[c];
                if (offset < grid.size()) {
                    totalCount += grid[offset];
                }
            }
            expect(totalCount == 0_u) << "After full scroll, total count should be 0";
        }
    };

    "gpu_layout_prim_staging_matches_grid_refs"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 10;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 200, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(20.0f, CELL_HEIGHT);

        // Add 3 shapes at different positions
        builder->setCursorPosition(0, 1);
        builder->addYpaintBuffer(createShapeSpanningLines(1, CELL_HEIGHT));

        builder->setCursorPosition(0, 5);
        builder->addYpaintBuffer(createShapeSpanningLines(2, CELL_HEIGHT));

        builder->setCursorPosition(0, 8);
        builder->addYpaintBuffer(createShapeSpanningLines(1, CELL_HEIGHT));

        // Get grid and prim staging
        const auto& grid = builder->gridStaging();
        std::vector<uint32_t> primStaging;
        builder->buildPrimStaging(primStaging);

        uint32_t primCount = builder->primitiveCount();
        expect(primCount == 3_u);

        // Collect all unique prim indices from grid
        std::set<uint32_t> referencedPrims;
        uint32_t gridW = builder->gridWidth();
        uint32_t gridH = builder->gridHeight();
        uint32_t numCells = gridW * gridH;

        for (uint32_t c = 0; c < numCells && c < grid.size(); c++) {
            uint32_t offset = grid[c];
            if (offset < grid.size()) {
                uint32_t count = grid[offset];
                for (uint32_t j = 0; j < count; j++) {
                    uint32_t primIdx = grid[offset + 1 + j];
                    if ((primIdx & 0x80000000u) == 0) {
                        referencedPrims.insert(primIdx);
                    }
                }
            }
        }

        // All primitives should be referenced somewhere in grid
        expect(referencedPrims.size() == primCount)
            << "Grid should reference all " << primCount << " prims, but references "
            << referencedPrims.size();

        // Each referenced index should be < primCount
        for (uint32_t idx : referencedPrims) {
            expect(idx < primCount) << "Referenced prim " << idx << " >= primCount " << primCount;
        }
    };

    "gpu_layout_scroll_preserves_relative_positions"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 10;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 100, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Add shapes at rows 0, 2, 4, 6, 8
        for (uint32_t i = 0; i < 5; i++) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i * 2));
            builder->addYpaintBuffer(createShapeSpanningLines(1, CELL_HEIGHT));
        }

        // Get initial row values
        std::vector<int16_t> initialRows;
        std::vector<uint32_t> primStaging;
        builder->buildPrimStaging(primStaging);

        // Primitive format: every prim has data starting at offset table position
        // For scrolling mode, gridOffset is at word[0] = packed(col | row<<16)
        uint32_t primCount = builder->primitiveCount();
        // primStaging format: [primCount offsets][primCount primitive data blocks]
        // Each prim starts at primStaging[primCount + offset]
        // Layout: [0]=gridOffset, [1]=type, [2]=layer, [3+]=geometry
        for (uint32_t i = 0; i < primCount; i++) {
            uint32_t offset = primStaging[i];
            uint32_t primBase = primCount + offset;
            if (primBase < primStaging.size()) {
                uint32_t packed = primStaging[primBase + 0];  // word[0] = gridOffset
                int16_t row = static_cast<int16_t>((packed >> 16) & 0xFFFF);
                initialRows.push_back(row);
            }
        }

        expect(initialRows.size() == 5_u) << "Should have 5 initial rows";

        // Scroll 2 lines
        builder->scrollLines(2);

        // Get new rows
        primStaging.clear();
        builder->buildPrimStaging(primStaging);

        primCount = builder->primitiveCount();
        expect(primCount == 4_u) << "After scroll 2, should have 4 prims (row 0 gone)";

        // Remaining prims should have rows decreased by 2
        // Original rows were 0, 2, 4, 6, 8
        // After scroll 2: row 0 gone, remaining are -2 (gone), 0, 2, 4, 6
        // But row 0 shape (at original row 0) is removed because baseLine 0 is popped
        // Wait, let me think about this again...
        // Shapes at rows 0, 2, 4, 6, 8
        // scrollLines(2) pops 2 lines from front
        // Shape at row 0: baseLine=0, gets popped
        // Shape at row 2: baseLine=2, after scroll baseLine=0, row becomes 2-2=0
        // Shape at row 4: baseLine=4, after scroll baseLine=2, row becomes 4-2=2
        // etc.

        // Actually the logic is: baseLine is the LINE in _scrollLines where prim is stored
        // Row offset in primitive data is decremented by scroll amount

        // After scrollLines(2), we expect 4 remaining prims with rows:
        // original 2 -> 0, original 4 -> 2, original 6 -> 4, original 8 -> 6

        std::vector<int16_t> newRows;
        for (uint32_t i = 0; i < primCount; i++) {
            uint32_t offset = primStaging[i];
            uint32_t primBase = primCount + offset;
            if (primBase < primStaging.size()) {
                uint32_t packed = primStaging[primBase + 0];  // word[0] = gridOffset
                int16_t row = static_cast<int16_t>((packed >> 16) & 0xFFFF);
                newRows.push_back(row);
            }
        }

        // The 4 remaining shapes should have rows 0, 2, 4, 6 (decremented from 2,4,6,8)
        expect(newRows.size() == 4_u);
        if (newRows.size() == 4) {
            // Sort to compare
            std::sort(newRows.begin(), newRows.end());
            expect(newRows[0] == 0_i) << "First remaining row should be 0, got " << newRows[0];
            expect(newRows[1] == 2_i) << "Second remaining row should be 2, got " << newRows[1];
            expect(newRows[2] == 4_i) << "Third remaining row should be 4, got " << newRows[2];
            expect(newRows[3] == 6_i) << "Fourth remaining row should be 6, got " << newRows[3];
        }
    };

    //=========================================================================
    // COMPREHENSIVE GPU BUFFER VERIFICATION WITH ALL PRIMITIVE TYPES
    // Uses Circle, Box, Triangle, Ellipse, RoundedBox, Segment, Pentagon,
    // Hexagon, Star, Rhombus, Capsule, Heart, Cross, Ring to ensure the
    // GPU buffer format is correct for ALL primitive types, not just one.
    //=========================================================================

    "all_primitive_types_gpu_buffer_verification"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 20;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 400, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Define expected types and their IDs
        struct PrimInfo {
            uint32_t type;
            const char* name;
            uint16_t cursorRow;
        };

        std::vector<PrimInfo> primInfos;

        // Add Circle at row 0
        {
            auto buf = *YPaintBuffer::create();
            buf->addCircle(0, 50, 10, 8, 0xFF0000FF, 0, 0, 0);
            builder->setCursorPosition(0, 0);
            builder->addYpaintBuffer(buf);
            primInfos.push_back({0, "Circle", 0});
        }

        // Add Box at row 1
        {
            auto buf = *YPaintBuffer::create();
            buf->addBox(0, 50, 30, 10, 8, 0xFF00FF00, 0, 0, 0);
            builder->setCursorPosition(0, 1);
            builder->addYpaintBuffer(buf);
            primInfos.push_back({1, "Box", 1});
        }

        // Add Triangle at row 2
        {
            auto buf = *YPaintBuffer::create();
            buf->addTriangle(0, 30, 50, 70, 50, 50, 40, 0xFFFF0000, 0, 0, 0);
            builder->setCursorPosition(0, 2);
            builder->addYpaintBuffer(buf);
            primInfos.push_back({3, "Triangle", 2});
        }

        // Add Segment at row 3
        {
            auto buf = *YPaintBuffer::create();
            buf->addSegment(0, 10, 70, 90, 70, 0xFF00FFFF, 0, 2.0f, 0);
            builder->setCursorPosition(0, 3);
            builder->addYpaintBuffer(buf);
            primInfos.push_back({2, "Segment", 3});
        }

        // Add Ellipse at row 4
        {
            auto buf = *YPaintBuffer::create();
            buf->addEllipse(0, 50, 90, 15, 8, 0xFFFF00FF, 0, 0, 0);
            builder->setCursorPosition(0, 4);
            builder->addYpaintBuffer(buf);
            primInfos.push_back({6, "Ellipse", 4});
        }

        // Add RoundedBox at row 5
        {
            auto buf = *YPaintBuffer::create();
            buf->addRoundedBox(0, 50, 110, 20, 8, 3, 3, 3, 3, 0xFFAA5500, 0, 0, 0);
            builder->setCursorPosition(0, 5);
            builder->addYpaintBuffer(buf);
            primInfos.push_back({8, "RoundedBox", 5});
        }

        // Add Pentagon at row 6
        {
            auto buf = *YPaintBuffer::create();
            buf->addPentagon(0, 50, 130, 10, 0xFF5500AA, 0, 0, 0);
            builder->setCursorPosition(0, 6);
            builder->addYpaintBuffer(buf);
            primInfos.push_back({10, "Pentagon", 6});
        }

        // Add Hexagon at row 7
        {
            auto buf = *YPaintBuffer::create();
            buf->addHexagon(0, 50, 150, 10, 0xFF00AA55, 0, 0, 0);
            builder->setCursorPosition(0, 7);
            builder->addYpaintBuffer(buf);
            primInfos.push_back({11, "Hexagon", 7});
        }

        // Add Star at row 8
        {
            auto buf = *YPaintBuffer::create();
            buf->addStar(0, 50, 170, 12, 5, 5, 0xFFAAAA00, 0, 0, 0);
            builder->setCursorPosition(0, 8);
            builder->addYpaintBuffer(buf);
            primInfos.push_back({12, "Star", 8});
        }

        // Add Rhombus at row 9
        {
            auto buf = *YPaintBuffer::create();
            buf->addRhombus(0, 50, 190, 10, 15, 0xFF00AAAA, 0, 0, 0);
            builder->setCursorPosition(0, 9);
            builder->addYpaintBuffer(buf);
            primInfos.push_back({9, "Rhombus", 9});
        }

        // Add Heart at row 10
        {
            auto buf = *YPaintBuffer::create();
            buf->addHeart(0, 50, 210, 10, 0xFFFF5555, 0, 0, 0);
            builder->setCursorPosition(0, 10);
            builder->addYpaintBuffer(buf);
            primInfos.push_back({15, "Heart", 10});
        }

        // Add Cross at row 11 (layer, cx, cy, bx, by, r, fillColor, strokeColor, strokeWidth, round)
        {
            auto buf = *YPaintBuffer::create();
            buf->addCross(0, 50, 230, 15, 10, 3, 0xFF5555FF, 0, 0, 0);
            builder->setCursorPosition(0, 11);
            builder->addYpaintBuffer(buf);
            primInfos.push_back({16, "Cross", 11});
        }

        // Add Ring at row 12 (layer, cx, cy, nx, ny, r, th, fillColor, strokeColor, strokeWidth, round)
        {
            auto buf = *YPaintBuffer::create();
            buf->addRing(0, 50, 250, 0, 1, 12, 3, 0xFFFFFF55, 0, 0, 0);
            builder->setCursorPosition(0, 12);
            builder->addYpaintBuffer(buf);
            primInfos.push_back({14, "Ring", 12});
        }

        // Add Capsule at row 13
        {
            auto buf = *YPaintBuffer::create();
            buf->addCapsule(0, 30, 270, 70, 270, 5, 0xFF55FFFF, 0, 0, 0);
            builder->setCursorPosition(0, 13);
            builder->addYpaintBuffer(buf);
            primInfos.push_back({18, "Capsule", 13});
        }

        // Verify primitive count
        expect(builder->primitiveCount() == primInfos.size())
            << "Should have " << primInfos.size() << " prims";

        // Build primStaging to verify GPU format
        std::vector<uint32_t> primStaging;
        builder->buildPrimStaging(primStaging);

        uint32_t primCount = builder->primitiveCount();
        expect(primCount == primInfos.size());

        // Verify EACH primitive's GPU format:
        // word[0] = gridOffset (packed: col | row << 16)
        // word[1] = type
        // word[2] = layer
        for (uint32_t i = 0; i < primCount && i < primInfos.size(); i++) {
            uint32_t offset = primStaging[i];
            uint32_t primBase = primCount + offset;

            if (primBase + 2 < primStaging.size()) {
                uint32_t packedOffset = primStaging[primBase + 0];
                uint32_t type = primStaging[primBase + 1];
                uint32_t layer = primStaging[primBase + 2];

                uint16_t col = packedOffset & 0xFFFF;
                int16_t row = static_cast<int16_t>((packedOffset >> 16) & 0xFFFF);

                expect(col == 0_u) << primInfos[i].name << ": col should be 0";
                expect(row == static_cast<int16_t>(primInfos[i].cursorRow))
                    << primInfos[i].name << ": row should be " << primInfos[i].cursorRow
                    << ", got " << row;
                expect(type == primInfos[i].type)
                    << primInfos[i].name << ": type should be " << primInfos[i].type
                    << ", got " << type;
                expect(layer == 0_u) << primInfos[i].name << ": layer should be 0";
            } else {
                expect(false) << "primBase + 2 out of bounds for " << primInfos[i].name;
            }
        }

        // Now scroll 3 lines and verify gridOffsetRow is DECREMENTED
        builder->scrollLines(3);

        // Primitives removed when their baseLine (= primMaxLine) < 3
        // baseLine = cursorRow + floor(maxY / cellSizeY)
        // Circle: baseLine = 0 + 0 = 0 < 3 → REMOVED
        // Box: baseLine = 1 + 1 = 2 < 3 → REMOVED
        // Triangle: baseLine = 2 + 2 = 4 >= 3 → SURVIVES
        // So only 2 prims removed, 12 survive
        uint32_t newPrimCount = builder->primitiveCount();
        expect(newPrimCount == primInfos.size() - 2)
            << "After scroll 3, should have " << (primInfos.size() - 2) << " prims, got " << newPrimCount;

        // Rebuild staging
        primStaging.clear();
        builder->buildPrimStaging(primStaging);

        // Verify remaining prims have decremented rows
        std::vector<std::pair<int16_t, uint32_t>> rowsAndTypes;
        for (uint32_t i = 0; i < newPrimCount; i++) {
            uint32_t offset = primStaging[i];
            uint32_t primBase = newPrimCount + offset;
            if (primBase + 1 < primStaging.size()) {
                uint32_t packedOffset = primStaging[primBase + 0];
                uint32_t type = primStaging[primBase + 1];
                int16_t row = static_cast<int16_t>((packedOffset >> 16) & 0xFFFF);
                rowsAndTypes.push_back({row, type});
            }
        }

        // Sort by row for predictable order
        std::sort(rowsAndTypes.begin(), rowsAndTypes.end());

        // Expected after scroll 3 (gridOffset = original cursorRow - 3):
        // Triangle (type 3) was row 2, now row -1
        // Segment (type 2) was row 3, now row 0
        // Ellipse (type 6) was row 4, now row 1
        // RoundedBox (type 8) was row 5, now row 2
        // Pentagon (type 10) was row 6, now row 3
        // Hexagon (type 11) was row 7, now row 4
        // Star (type 12) was row 8, now row 5
        // Rhombus (type 9) was row 9, now row 6
        // Heart (type 15) was row 10, now row 7
        // Cross (type 16) was row 11, now row 8
        // Ring (type 14) was row 12, now row 9
        // Capsule (type 18) was row 13, now row 10

        std::vector<std::pair<int16_t, uint32_t>> expected = {
            {-1, 3},  // Triangle
            {0, 2},   // Segment
            {1, 6},   // Ellipse
            {2, 8},   // RoundedBox
            {3, 10},  // Pentagon
            {4, 11},  // Hexagon
            {5, 12},  // Star
            {6, 9},   // Rhombus
            {7, 15},  // Heart
            {8, 16},  // Cross
            {9, 14},  // Ring
            {10, 18}, // Capsule
        };

        expect(rowsAndTypes.size() == expected.size())
            << "Should have " << expected.size() << " prims after scroll";

        for (size_t i = 0; i < std::min(rowsAndTypes.size(), expected.size()); i++) {
            expect(rowsAndTypes[i].first == expected[i].first)
                << "Prim " << i << ": row should be " << expected[i].first
                << ", got " << rowsAndTypes[i].first;
            expect(rowsAndTypes[i].second == expected[i].second)
                << "Prim " << i << ": type should be " << expected[i].second
                << ", got " << rowsAndTypes[i].second;
        }

        // Scroll 5 more lines and verify
        builder->scrollLines(5);
        newPrimCount = builder->primitiveCount();
        // After scroll 3, baseLines were (original - 3): Triangle=1, Segment=3, Ellipse=5, ...
        // Scroll 5 more removes baseLine < 5: Triangle(1), Segment(3) removed
        // 10 primitives survive
        expect(newPrimCount == 10_u) << "After scroll 8 total, should have 10 prims";

        primStaging.clear();
        builder->buildPrimStaging(primStaging);

        // Verify types and rows of remaining prims
        rowsAndTypes.clear();
        for (uint32_t i = 0; i < newPrimCount; i++) {
            uint32_t offset = primStaging[i];
            uint32_t primBase = newPrimCount + offset;
            if (primBase + 1 < primStaging.size()) {
                uint32_t packedOffset = primStaging[primBase + 0];
                uint32_t type = primStaging[primBase + 1];
                int16_t row = static_cast<int16_t>((packedOffset >> 16) & 0xFFFF);
                rowsAndTypes.push_back({row, type});
            }
        }
        std::sort(rowsAndTypes.begin(), rowsAndTypes.end());

        // After 8 total scrolls (gridOffset = original cursorRow - 8):
        // Ellipse (type 6) was row 4, now row -4
        // RoundedBox (type 8) was row 5, now row -3
        // Pentagon (type 10) was row 6, now row -2
        // Hexagon (type 11) was row 7, now row -1
        // Star (type 12) was row 8, now row 0
        // Rhombus (type 9) was row 9, now row 1
        // Heart (type 15) was row 10, now row 2
        // Cross (type 16) was row 11, now row 3
        // Ring (type 14) was row 12, now row 4
        // Capsule (type 18) was row 13, now row 5
        expected = {
            {-4, 6},  // Ellipse
            {-3, 8},  // RoundedBox
            {-2, 10}, // Pentagon
            {-1, 11}, // Hexagon
            {0, 12},  // Star
            {1, 9},   // Rhombus
            {2, 15},  // Heart
            {3, 16},  // Cross
            {4, 14},  // Ring
            {5, 18},  // Capsule
        };

        for (size_t i = 0; i < std::min(rowsAndTypes.size(), expected.size()); i++) {
            expect(rowsAndTypes[i].first == expected[i].first)
                << "After scroll 8, prim " << i << ": row should be " << expected[i].first
                << ", got " << rowsAndTypes[i].first;
            expect(rowsAndTypes[i].second == expected[i].second)
                << "After scroll 8, prim " << i << ": type should be " << expected[i].second
                << ", got " << rowsAndTypes[i].second;
        }
    };

    "cursor_offset_applied_to_all_primitive_types"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 30;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 400, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Add primitives at cursor row 10 (non-zero) to verify offset is applied
        const uint16_t CURSOR_ROW = 10;
        builder->setCursorPosition(5, CURSOR_ROW);

        // Add multiple different primitive types at the SAME cursor position
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50, 5, 4, 0xFF0000FF, 0, 0, 0);      // type 0
        buf->addBox(0, 50, 5, 5, 4, 0xFF00FF00, 0, 0, 0);       // type 1
        buf->addEllipse(0, 50, 5, 6, 4, 0xFFFF0000, 0, 0, 0);   // type 6
        buf->addPentagon(0, 50, 5, 4, 0xFF00FFFF, 0, 0, 0);     // type 10
        buf->addHexagon(0, 50, 5, 4, 0xFFFF00FF, 0, 0, 0);      // type 11
        buf->addStar(0, 50, 5, 6, 3, 5, 0xFFFFFF00, 0, 0, 0);   // type 12
        buf->addHeart(0, 50, 5, 4, 0xFF888888, 0, 0, 0);        // type 15

        builder->addYpaintBuffer(buf);

        uint32_t primCount = builder->primitiveCount();
        expect(primCount == 7_u) << "Should have 7 prims";

        std::vector<uint32_t> primStaging;
        builder->buildPrimStaging(primStaging);

        std::vector<uint32_t> expectedTypes = {0, 1, 6, 10, 11, 12, 15};

        // Verify ALL prims have the correct gridOffsetRow (CURSOR_ROW)
        for (uint32_t i = 0; i < primCount; i++) {
            uint32_t offset = primStaging[i];
            uint32_t primBase = primCount + offset;

            uint32_t packedOffset = primStaging[primBase + 0];
            uint32_t type = primStaging[primBase + 1];

            uint16_t col = packedOffset & 0xFFFF;
            int16_t row = static_cast<int16_t>((packedOffset >> 16) & 0xFFFF);

            expect(col == 5_u) << "Prim " << i << " (type " << type << "): col should be 5, got " << col;
            expect(row == static_cast<int16_t>(CURSOR_ROW))
                << "Prim " << i << " (type " << type << "): row should be " << CURSOR_ROW << ", got " << row;
        }

        // Scroll 5 lines - all prims should have row decremented to 5
        builder->scrollLines(5);
        primCount = builder->primitiveCount();
        expect(primCount == 7_u) << "After scroll 5, should still have 7 prims";

        primStaging.clear();
        builder->buildPrimStaging(primStaging);

        for (uint32_t i = 0; i < primCount; i++) {
            uint32_t offset = primStaging[i];
            uint32_t primBase = primCount + offset;

            uint32_t packedOffset = primStaging[primBase + 0];
            uint32_t type = primStaging[primBase + 1];

            int16_t row = static_cast<int16_t>((packedOffset >> 16) & 0xFFFF);

            expect(row == 5_i)
                << "After scroll 5, prim " << i << " (type " << type << "): row should be 5, got " << row;
        }

        // Scroll 5 more - all prims should have row = 0
        builder->scrollLines(5);
        primStaging.clear();
        builder->buildPrimStaging(primStaging);
        primCount = builder->primitiveCount();

        for (uint32_t i = 0; i < primCount; i++) {
            uint32_t offset = primStaging[i];
            uint32_t primBase = primCount + offset;

            uint32_t packedOffset = primStaging[primBase + 0];
            int16_t row = static_cast<int16_t>((packedOffset >> 16) & 0xFFFF);

            expect(row == 0_i) << "After scroll 10, prim " << i << ": row should be 0, got " << row;
        }

        // Scroll 1 more - all prims should be GONE (row would be -1)
        builder->scrollLines(1);
        primCount = builder->primitiveCount();
        expect(primCount == 0_u) << "After scroll 11, all prims should be gone";
    };

    "verify_gpu_buffer_after_each_add_and_scroll"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *Painter::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 10;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 200, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Test: Add prim, verify buffer, scroll, verify buffer, repeat

        // Step 1: Add Circle at row 0
        {
            auto buf = *YPaintBuffer::create();
            buf->addCircle(0, 50, 10, 8, 0xFF0000FF, 0, 0, 0);
            builder->setCursorPosition(0, 0);
            builder->addYpaintBuffer(buf);

            std::vector<uint32_t> staging;
            builder->buildPrimStaging(staging);
            expect(builder->primitiveCount() == 1_u);

            uint32_t primBase = 1 + staging[0];
            uint32_t packed = staging[primBase + 0];
            uint32_t type = staging[primBase + 1];
            int16_t row = static_cast<int16_t>((packed >> 16) & 0xFFFF);

            expect(type == 0_u) << "Step 1: Circle type should be 0";
            expect(row == 0_i) << "Step 1: Circle row should be 0";
        }

        // Step 2: Add Box at row 3
        {
            auto buf = *YPaintBuffer::create();
            buf->addBox(0, 50, 70, 10, 8, 0xFF00FF00, 0, 0, 0);
            builder->setCursorPosition(0, 3);
            builder->addYpaintBuffer(buf);

            std::vector<uint32_t> staging;
            builder->buildPrimStaging(staging);
            expect(builder->primitiveCount() == 2_u);

            // Find the Box (type 1)
            bool foundBox = false;
            for (uint32_t i = 0; i < 2; i++) {
                uint32_t primBase = 2 + staging[i];
                uint32_t type = staging[primBase + 1];
                if (type == 1) {
                    uint32_t packed = staging[primBase + 0];
                    int16_t row = static_cast<int16_t>((packed >> 16) & 0xFFFF);
                    expect(row == 3_i) << "Step 2: Box row should be 3";
                    foundBox = true;
                }
            }
            expect(foundBox) << "Step 2: Should find Box";
        }

        // Step 3: Scroll 2 lines
        {
            builder->scrollLines(2);

            std::vector<uint32_t> staging;
            builder->buildPrimStaging(staging);
            expect(builder->primitiveCount() == 1_u) << "Step 3: Circle should be gone";

            // Box should now have row = 1 (was 3, scroll 2)
            uint32_t primBase = 1 + staging[0];
            uint32_t type = staging[primBase + 1];
            uint32_t packed = staging[primBase + 0];
            int16_t row = static_cast<int16_t>((packed >> 16) & 0xFFFF);

            expect(type == 1_u) << "Step 3: Remaining prim should be Box";
            expect(row == 1_i) << "Step 3: Box row should be 1 (was 3, scrolled 2)";
        }

        // Step 4: Add Triangle at row 5
        {
            auto buf = *YPaintBuffer::create();
            buf->addTriangle(0, 30, 110, 70, 110, 50, 90, 0xFFFF0000, 0, 0, 0);
            builder->setCursorPosition(0, 5);
            builder->addYpaintBuffer(buf);

            std::vector<uint32_t> staging;
            builder->buildPrimStaging(staging);
            expect(builder->primitiveCount() == 2_u);

            // Find Triangle and verify row
            bool foundTriangle = false;
            for (uint32_t i = 0; i < 2; i++) {
                uint32_t primBase = 2 + staging[i];
                uint32_t type = staging[primBase + 1];
                if (type == 3) {  // Triangle
                    uint32_t packed = staging[primBase + 0];
                    int16_t row = static_cast<int16_t>((packed >> 16) & 0xFFFF);
                    expect(row == 5_i) << "Step 4: Triangle row should be 5";
                    foundTriangle = true;
                }
            }
            expect(foundTriangle) << "Step 4: Should find Triangle";
        }

        // Step 5: Scroll 3 more lines (total 5)
        // Box baseLine=6 > 5 → survives
        // Triangle baseLine=10 > 5 → survives
        {
            builder->scrollLines(3);

            std::vector<uint32_t> staging;
            builder->buildPrimStaging(staging);
            uint32_t count = builder->primitiveCount();
            expect(count == 2_u) << "Step 5: Both Box and Triangle survive";

            std::map<uint32_t, int16_t> typeToRow;
            for (uint32_t i = 0; i < count; i++) {
                uint32_t primBase = count + staging[i];
                uint32_t type = staging[primBase + 1];
                uint32_t packed = staging[primBase + 0];
                int16_t row = static_cast<int16_t>((packed >> 16) & 0xFFFF);
                typeToRow[type] = row;
            }

            expect(typeToRow[1] == -2_i) << "Step 5: Box row should be -2 (was 3, scrolled 5)";
            expect(typeToRow[3] == 2_i) << "Step 5: Triangle row should be 2 (was 5, scrolled 3)";
        }

        // Step 6: Add Ellipse at row 8, Pentagon at row 9
        // Now have: Box (baseLine=6), Triangle (baseLine=10), Ellipse (baseLine=16), Pentagon (baseLine=18)
        {
            auto buf = *YPaintBuffer::create();
            buf->addEllipse(0, 50, 170, 10, 5, 0xFF00FFFF, 0, 0, 0);
            builder->setCursorPosition(0, 8);
            builder->addYpaintBuffer(buf);

            auto buf2 = *YPaintBuffer::create();
            buf2->addPentagon(0, 50, 190, 8, 0xFFFF00FF, 0, 0, 0);
            builder->setCursorPosition(0, 9);
            builder->addYpaintBuffer(buf2);

            std::vector<uint32_t> staging;
            builder->buildPrimStaging(staging);
            expect(builder->primitiveCount() == 4_u);

            // Verify all four prims
            std::map<uint32_t, int16_t> typeToRow;
            for (uint32_t i = 0; i < 4; i++) {
                uint32_t primBase = 4 + staging[i];
                uint32_t type = staging[primBase + 1];
                uint32_t packed = staging[primBase + 0];
                int16_t row = static_cast<int16_t>((packed >> 16) & 0xFFFF);
                typeToRow[type] = row;
            }

            expect(typeToRow[1] == -2_i) << "Step 6: Box row should be -2";
            expect(typeToRow[3] == 2_i) << "Step 6: Triangle row should be 2";
            expect(typeToRow[6] == 8_i) << "Step 6: Ellipse row should be 8";
            expect(typeToRow[10] == 9_i) << "Step 6: Pentagon row should be 9";
        }

        // Step 7: Scroll 5 lines (total 10)
        // Box baseLine=6 < 10 → REMOVED
        // Triangle baseLine=10 = 10 → survives (pop_front removes 0-9, index 10 remains)
        // Ellipse baseLine=16 > 10 → survives
        // Pentagon baseLine=18 > 10 → survives
        {
            builder->scrollLines(5);

            std::vector<uint32_t> staging;
            builder->buildPrimStaging(staging);
            uint32_t count = builder->primitiveCount();
            expect(count == 3_u) << "Step 7: Box gone, Triangle/Ellipse/Pentagon remain";

            std::map<uint32_t, int16_t> typeToRow;
            for (uint32_t i = 0; i < count; i++) {
                uint32_t primBase = count + staging[i];
                uint32_t type = staging[primBase + 1];
                uint32_t packed = staging[primBase + 0];
                int16_t row = static_cast<int16_t>((packed >> 16) & 0xFFFF);
                typeToRow[type] = row;
            }

            expect(typeToRow[3] == -3_i) << "Step 7: Triangle row should be -3 (was 2, scroll 5)";
            expect(typeToRow[6] == 3_i) << "Step 7: Ellipse row should be 3 (was 8, scroll 5)";
            expect(typeToRow[10] == 4_i) << "Step 7: Pentagon row should be 4 (was 9, scroll 5)";
        }
    };
};
