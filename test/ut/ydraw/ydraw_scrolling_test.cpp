//=============================================================================
// YDrawBuilder Scrolling Mode Unit Tests
//
// Tests terminal-style scrolling behavior:
// - Cursor positioning
// - Content overflow and scroll
// - Multi-page scrolling with progressive buffer additions
//=============================================================================

#include <algorithm>

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

    "offset_applied_at_cursor_position"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *YDrawBuilder::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Set scene bounds and cell size first
        builder->setSceneBounds(0, 0, 100, 100);
        builder->setGridCellSize(10.0f, 10.0f);

        // Set cursor at grid position (3, 5)
        builder->setCursorPosition(3, 5);

        // Create a buffer with a simple circle
        auto buf = *YDrawBuffer::create();
        buf->addCircle(0, 50, 50, 10, 0xFFFFFFFF, 0, 0, 0);

        builder->addYdrawBuffer(buf);

        // Verify cursor was stored
        expect(builder->cursorCol() == 3_u);
        expect(builder->cursorRow() == 5_u);

        // Build buffers and verify primitive count
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        // Expected: offsetX=3, offsetY=5 → packed as (3 | (5 << 16))
        uint32_t expectedOffset = 3u | (5u << 16);

        // Read from GPU buffer structure:
        // primitives buffer has: [offset_table...][primitive_data...]
        // For each prim: type=+0, layer=+1, offsetXY=+2, ...
        const float* storage = cardMgr->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = reinterpret_cast<const uint32_t*>(
            cardMgr->metadataAsU32());

        uint32_t primOff = meta[0];
        uint32_t primCnt = meta[1];
        uint32_t primDataBase = primOff + primCnt;

        // Read word offset from offset table, then find actual prim data
        uint32_t wordOff0 = readU32(storage, primOff);
        uint32_t absOff = primDataBase + wordOff0;

        // Offset is at word 2 in the primitive data
        uint32_t packedOffset = readU32(storage, absOff + 2);
        expect(packedOffset == expectedOffset) << "Expected offset " << expectedOffset
                                               << " but got " << packedOffset;
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

    "no_scroll_when_within_bounds"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *YDrawBuilder::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Scene: 10 lines (0-100 with cell height 10)
        builder->setSceneBounds(0, 0, 100, 100);
        builder->setGridCellSize(10.0f, 10.0f);

        // Cursor at row 2
        builder->setCursorPosition(0, 2);

        // Add buffer with content that fits (height < cell height)
        auto buf = *YDrawBuffer::create();
        buf->addCircle(0, 5, 5, 2, 0xFFFFFFFF, 0, 0, 0);  // Small circle

        builder->addYdrawBuffer(buf);

        // Check offset: should be (0, 2)
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        // Read from GPU buffer structure
        const float* storage = cardMgr->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = cardMgr->metadataAsU32();

        uint32_t primOff = meta[0];
        uint32_t primCnt = meta[1];
        uint32_t primDataBase = primOff + primCnt;
        uint32_t wordOff0 = readU32(storage, primOff);
        uint32_t absOff = primDataBase + wordOff0;

        uint32_t packedOffset = readU32(storage, absOff + 2);

        uint16_t offsetX = static_cast<uint16_t>(packedOffset & 0xFFFF);
        uint16_t offsetY = static_cast<uint16_t>((packedOffset >> 16) & 0xFFFF);

        expect(offsetX == 0_u);
        expect(offsetY == 2_u);
    };

    "progressive_scrolling"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *YDrawBuilder::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Scene: 5 lines
        builder->setSceneBounds(0, 0, 100, 50);
        builder->setGridCellSize(10.0f, 10.0f);

        // Add content progressively, simulating terminal output
        // Each iteration sets cursor to row N and adds content
        // When cursor exceeds scene height, scrolling deletes old primitives
        for (int row = 0; row < 10; ++row) {
            builder->setCursorPosition(0, static_cast<uint16_t>(row));
            auto buf = *YDrawBuffer::create();
            // Each circle is ~1 line tall
            buf->addCircle(0, 5, 5, 3, 0xFFFFFFFF, 0, 0, 0);
            builder->addYdrawBuffer(buf);
        }

        // After adding 10 rows to a 5-line display with aggressive cursor jumps,
        // scrolling deletes old primitives. By row 9, most are scrolled out.
        // We expect at least 1 primitive remains (the last one added)
        expect(builder->primitiveCount() >= 1_u);

        // Build to verify no crashes
        builder->declareBufferNeeds();
        cardMgr->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
    };

    "lightweight_factory_scrolling"_test = [] {
        auto gpuAlloc = testAllocator();

        // Test lightweight factory with scrolling mode
        auto builder = *YDrawBuilder::create(FontManager::Ptr{}, gpuAlloc, true);
        expect(builder->scrollingMode() == true);

        auto builder2 = *YDrawBuilder::create(FontManager::Ptr{}, gpuAlloc, false);
        expect(builder2->scrollingMode() == false);
    };

    "scrolling_primitive_count_debug"_test = [] {
        auto cardMgr = std::make_shared<MockCardManager>();
        auto gpuAlloc = testAllocator();

        auto builder = *YDrawBuilder::create(
            FontManager::Ptr{}, gpuAlloc, cardMgr, 0, true);

        // Scene: 5 lines
        builder->setSceneBounds(0, 0, 100, 50);
        builder->setGridCellSize(10.0f, 10.0f);

        expect(builder->sceneHeightInLines() == 5_u) << "Scene should have 5 lines";

        // Add first primitive at row 0
        builder->setCursorPosition(0, 0);
        auto buf0 = *YDrawBuffer::create();
        buf0->addCircle(0, 5, 5, 3, 0xFFFFFFFF, 0, 0, 0);
        builder->addYdrawBuffer(buf0);
        expect(builder->primitiveCount() == 1_u) << "After row 0: should have 1 prim";

        // Add at row 1
        builder->setCursorPosition(0, 1);
        auto buf1 = *YDrawBuffer::create();
        buf1->addCircle(0, 5, 5, 3, 0xFFFFFFFF, 0, 0, 0);
        builder->addYdrawBuffer(buf1);
        expect(builder->primitiveCount() == 2_u) << "After row 1: should have 2 prims";

        // Add at row 4 (last row before scroll)
        builder->setCursorPosition(0, 4);
        auto buf4 = *YDrawBuffer::create();
        buf4->addCircle(0, 5, 5, 3, 0xFFFFFFFF, 0, 0, 0);
        builder->addYdrawBuffer(buf4);
        expect(builder->primitiveCount() == 3_u) << "After row 4: should have 3 prims";

        // Add at row 5 (should trigger scroll, delete row 0)
        builder->setCursorPosition(0, 5);
        auto buf5 = *YDrawBuffer::create();
        buf5->addCircle(0, 5, 5, 3, 0xFFFFFFFF, 0, 0, 0);
        builder->addYdrawBuffer(buf5);
        // After scroll: row 0 prim deleted, so 3 prims remain (rows 1, 4, 5 -> now rows 0, 3, 4)
        expect(builder->primitiveCount() == 3_u) << "After row 5 (scroll): should have 3 prims";
    };
};
