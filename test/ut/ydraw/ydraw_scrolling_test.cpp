//=============================================================================
// YDrawBuilder Scrolling Mode Unit Tests
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

#include "yetty/ydraw/ydraw-buffer.h"
#include "yetty/ydraw/ydraw-types.gen.h"
#include <yetty/ydraw-builder.h>
#include <yetty/card-manager.h>
#include <yetty/card-buffer-manager.h>
#include <yetty/gpu-allocator.h>

#include <cstring>
#include <cmath>
#include <map>
#include <vector>

using namespace boost::ut;
using namespace yetty;
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
static YDrawBuffer::Ptr createShapeSpanningLines(uint32_t numLines, float cellHeight) {
    auto buf = *YDrawBuffer::create();
    // Box at (10, 0) with width=50, height = numLines * cellHeight - 1
    float height = numLines * cellHeight - 1.0f;
    buf->addBox(0, 10, 0, 50, height, 0xFFFFFFFF, 0, 0, 0, 0);
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
        auto builderNoScroll = *YDrawBuilder::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, false);
        expect(builderNoScroll->scrollingMode() == false);

        // Create builder WITH scrolling mode
        auto builderScroll = *YDrawBuilder::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 1, true);
        expect(builderScroll->scrollingMode() == true);
    };

    "cursor_position_default"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *YDrawBuilder::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Default cursor position should be (0, 0)
        expect(builder->cursorCol() == 0_u);
        expect(builder->cursorRow() == 0_u);
    };

    "cursor_position_set"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *YDrawBuilder::create(
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

        auto builder = *YDrawBuilder::create(
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

        auto builder = *YDrawBuilder::create(
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
            auto result = builder->addYdrawBuffer(buf);
            expect(result.has_value()) << "addYdrawBuffer failed at i=" << i;

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

        auto builder = *YDrawBuilder::create(
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
            auto result = builder->addYdrawBuffer(buf);
            expect(result.has_value()) << "addYdrawBuffer failed at i=" << i;

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

        auto builder = *YDrawBuilder::create(
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
            builder->addYdrawBuffer(buf);

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

        auto builder = *YDrawBuilder::create(
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
            builder->addYdrawBuffer(buf);
        }
        expect(builder->primitiveCount() == 10_u) << "Should have 10 prims";

        // Jump cursor to line 20 and add more shapes
        builder->setCursorPosition(0, 20);
        for (uint32_t i = 0; i < 5; ++i) {
            builder->setCursorPosition(0, static_cast<uint16_t>(20 + i));
            auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
            builder->addYdrawBuffer(buf);

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

        auto builder = *YDrawBuilder::create(
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
            auto result = builder->addYdrawBuffer(buf);
            expect(result.has_value()) << "addYdrawBuffer failed at i=" << i;

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

        auto builder = *YDrawBuilder::create(
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
            builder->addYdrawBuffer(buf);
        }
        expect(builder->primitiveCount() == 10_u);

        // Now scroll 5 lines
        builder->scrollLines(5);
        expect(builder->primitiveCount() == 5_u) << "Should have 5 prims after scrolling 5 lines";

        // Add 5 more shapes at positions 5-9
        for (uint32_t i = 5; i < 10; ++i) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i));
            auto buf = createShapeSpanningLines(1, CELL_HEIGHT);
            builder->addYdrawBuffer(buf);
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

        auto builder = *YDrawBuilder::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Terminal: 10 lines
        const uint32_t VISIBLE_LINES = 10;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 800, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Add shape spanning lines 0-2 (3 lines)
        builder->setCursorPosition(0, 0);
        auto buf1 = createShapeSpanningLines(3, CELL_HEIGHT);
        builder->addYdrawBuffer(buf1);
        expect(builder->primitiveCount() == 1_u);

        // Add shape spanning lines 3-5 (3 lines)
        builder->setCursorPosition(0, 3);
        auto buf2 = createShapeSpanningLines(3, CELL_HEIGHT);
        builder->addYdrawBuffer(buf2);
        expect(builder->primitiveCount() == 2_u);

        // Add shape spanning lines 6-9 (4 lines)
        builder->setCursorPosition(0, 6);
        auto buf3 = createShapeSpanningLines(4, CELL_HEIGHT);
        builder->addYdrawBuffer(buf3);
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

        auto builder = *YDrawBuilder::create(
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
            auto result = builder->addYdrawBuffer(buf);
            expect(result.has_value()) << "addYdrawBuffer failed at i=" << i;

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

        auto builder = *YDrawBuilder::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Terminal: 5 lines
        const uint32_t VISIBLE_LINES = 5;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 100, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Add shape at line 0
        builder->setCursorPosition(0, 0);
        auto buf0 = createShapeSpanningLines(1, CELL_HEIGHT);
        builder->addYdrawBuffer(buf0);

        // Add shape at line 4
        builder->setCursorPosition(0, 4);
        auto buf4 = createShapeSpanningLines(1, CELL_HEIGHT);
        builder->addYdrawBuffer(buf4);

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

        auto builder = *YDrawBuilder::create(
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
                builder->addYdrawBuffer(buf);
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

        auto builder = *YDrawBuilder::create(
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
            builder->addYdrawBuffer(buf);
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
        auto builder = *YDrawBuilder::create(FontManager::Ptr{}, gpuAlloc, true);
        expect(builder->scrollingMode() == true);

        auto builder2 = *YDrawBuilder::create(FontManager::Ptr{}, gpuAlloc, false);
        expect(builder2->scrollingMode() == false);
    };

    //=========================================================================
    // GRID HEALTH VERIFICATION TESTS
    // Verify grid integrity after scrolling operations
    //=========================================================================

    "grid_health_after_100_scrolls"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *YDrawBuilder::create(
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
            auto result = builder->addYdrawBuffer(buf);
            expect(result.has_value()) << "addYdrawBuffer failed at i=" << i;
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

        auto builder = *YDrawBuilder::create(
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
            auto result = builder->addYdrawBuffer(buf);
            expect(result.has_value()) << "addYdrawBuffer failed at i=" << i;

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

        auto builder = *YDrawBuilder::create(
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
            auto result = builder->addYdrawBuffer(buf);
            expect(result.has_value()) << "addYdrawBuffer failed at i=" << i;

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

        auto builder = *YDrawBuilder::create(
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
            builder->addYdrawBuffer(buf);
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

        auto builder = *YDrawBuilder::create(
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
            builder->addYdrawBuffer(buf);
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

        auto builder = *YDrawBuilder::create(
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
                builder->addYdrawBuffer(buf);
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

        auto builder = *YDrawBuilder::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        const uint32_t VISIBLE_LINES = 10;
        const float CELL_HEIGHT = 20.0f;
        builder->setSceneBounds(0, 0, 200, VISIBLE_LINES * CELL_HEIGHT);
        builder->setGridCellSize(10.0f, CELL_HEIGHT);

        // Add shapes at specific positions
        // Shape A at lines 0-2 (3 lines)
        builder->setCursorPosition(0, 0);
        auto bufA = createShapeSpanningLines(3, CELL_HEIGHT);
        builder->addYdrawBuffer(bufA);

        // Shape B at lines 4-6 (3 lines)
        builder->setCursorPosition(0, 4);
        auto bufB = createShapeSpanningLines(3, CELL_HEIGHT);
        builder->addYdrawBuffer(bufB);

        // Shape C at lines 8-9 (2 lines)
        builder->setCursorPosition(0, 8);
        auto bufC = createShapeSpanningLines(2, CELL_HEIGHT);
        builder->addYdrawBuffer(bufC);

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
};
