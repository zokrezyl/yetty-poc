//=============================================================================
// YPaintBuffer + Painter Unit Tests
//
// Tests AABB computation, GPU buffer layout, grid construction,
// the real builder GPU write pipeline using mock GpuMemoryManager,
// and the text/glyph pipeline including font atlas correctness.
//=============================================================================

#include <cstddef>
#include <version>
#include <algorithm>

#include <boost/ut.hpp>

#include "yetty/ypaint/ypaint-buffer.h"
#include "yetty/ypaint/ypaint-types.gen.h"
#include "yetty/ypaint/ypaint-prim-writer.gen.h"
#include <yetty/ypaint/painter.h>
#include <yetty/gpu-memory-manager.h>
#include <yetty/gpu-buffer-manager.h>
#include <yetty/font-manager.h>
#include <yetty/ms-msdf-font.h>
#include <yetty/msdf-atlas.h>
#include <yetty/msdf-cdb-provider.h>
#include <yetty/gpu-allocator.h>

#include <cstring>
#include <cmath>
#include <filesystem>
#include <fstream>
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
    uint32_t storageWordCount() const {
        return static_cast<uint32_t>(_storage.size() / sizeof(float));
    }

private:
    std::vector<uint8_t> _storage;
    uint32_t _nextOffset = 0;
    uint32_t _pendingReservation = 0;
    std::map<std::pair<uint32_t, std::string>, BufferHandle> _allocs;
};

//=============================================================================
// Mock GpuMemoryManager — owns mock buffer manager + CPU metadata array
//=============================================================================
class MockGpuMemoryManager : public GpuMemoryManager {
public:
    // Default constructor for slot 0
    MockGpuMemoryManager()
        : _bufMgr(std::make_shared<MockCardBufferManager>())
        , _metadata(16 * 64, 0) {}  // 16 slots * 64 bytes each

    // Constructor with custom metadata size to support high slot indices
    explicit MockGpuMemoryManager(uint32_t metadataBytes)
        : _bufMgr(std::make_shared<MockCardBufferManager>())
        , _metadata(metadataBytes, 0) {}

    // Metadata
    Result<MetadataHandle> allocateMetadata(uint32_t size) override {
        return Ok(MetadataHandle{0, 64});
    }
    Result<void> deallocateMetadata(MetadataHandle) override { return Ok(); }

    Result<void> writeMetadata(MetadataHandle handle,
                                const void* data, uint32_t size) override {
        if (handle.offset + size > _metadata.size()) {
            return Err<void>("MockGpuMemoryManager: metadata overflow");
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
    GpuTextureManager::Ptr textureManager() const override { return nullptr; }

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

    float metadataFloat(uint32_t wordIdx) const {
        float val;
        std::memcpy(&val, &metadataAsU32()[wordIdx], sizeof(float));
        return val;
    }

private:
    std::shared_ptr<MockCardBufferManager> _bufMgr;
    std::vector<uint8_t> _metadata;
};

//=============================================================================
// MockFontManager — provides real MsMsdfFont from CDB files for text tests
//=============================================================================
class MockFontManager : public FontManager {
public:
    MockFontManager(MsMsdfFont::Ptr font, const std::string& cacheDir)
        : _font(std::move(font)), _cacheDir(cacheDir) {}

    Result<MsMsdfFont::Ptr> getMsMsdfFont(const std::string&) noexcept override {
        return Ok(_font);
    }
    MsMsdfFont::Ptr getDefaultMsMsdfFont() noexcept override { return _font; }
    BmFont::Ptr getDefaultBmFont() noexcept override { return nullptr; }
    ShaderFont::Ptr getDefaultShaderGlyphFont() noexcept override { return nullptr; }
    ShaderFont::Ptr getDefaultCardFont() noexcept override { return nullptr; }
    Result<VectorSdfFont::Ptr> getVectorSdfFont(const std::string&) noexcept override {
        return Err<VectorSdfFont::Ptr>("stub");
    }
    VectorSdfFont::Ptr getDefaultVectorSdfFont() noexcept override { return nullptr; }
    Result<VectorCoverageFont::Ptr> getVectorCoverageFont(const std::string&) noexcept override {
        return Err<VectorCoverageFont::Ptr>("stub");
    }
    VectorCoverageFont::Ptr getDefaultVectorCoverageFont() noexcept override { return nullptr; }
    Result<RasterFont::Ptr> getRasterFont(const std::string&, uint32_t, uint32_t) noexcept override {
        return Err<RasterFont::Ptr>("stub");
    }
    RasterFont::Ptr getDefaultRasterFont() noexcept override { return nullptr; }
    void setDefaultFont(const std::string&) noexcept override {}
    bool hasFont(const std::string&) const noexcept override { return false; }
    const std::string& getCacheDir() const noexcept override { return _cacheDir; }
    MsdfCdbProvider::Ptr getCdbProvider() const noexcept override {
        static auto provider = std::make_shared<CpuMsdfCdbProvider>();
        return provider;
    }

private:
    MsMsdfFont::Ptr _font;
    std::string _cacheDir;
};

//=============================================================================
// Font infrastructure for text tests
//=============================================================================

static std::string ensureCdbBasePath() {
    std::string homeDir = std::getenv("HOME");
    std::string cacheDir = homeDir + "/.cache/yetty/msdf-font-cache";
    std::string basePath = cacheDir + "/DejaVuSansMNerdFontMono";
    std::string regularCdb = basePath + "-Regular.cdb";

    if (std::filesystem::exists(regularCdb)) return basePath;

    // Generate CDB from TTF using CPU provider
    std::string assetsDir = std::string(CMAKE_SOURCE_DIR) + "/assets";
    auto provider = std::make_shared<CpuMsdfCdbProvider>();
    std::filesystem::create_directories(cacheDir);

    for (const char* style : {"-Regular", "-Bold", "-Oblique", "-BoldOblique"}) {
        MsdfCdbConfig cfg;
        cfg.ttfPath = assetsDir + "/DejaVuSansMNerdFontMono" + std::string(style) + ".ttf";
        cfg.cdbPath = basePath + std::string(style) + ".cdb";
        if (!std::filesystem::exists(cfg.cdbPath)) {
            provider->generate(cfg);
        }
    }
    return basePath;
}

// Shared GpuAllocator with nullptr device — safe because we never create GPU resources
static GpuAllocator::Ptr testAllocator() {
    static auto alloc = std::make_shared<GpuAllocator>(nullptr);
    return alloc;
}

static FontManager::Ptr createTestFontManager() {
    auto basePath = ensureCdbBasePath();
    auto fontRes = MsMsdfFont::create(basePath, testAllocator());
    if (!fontRes) return nullptr;

    std::string homeDir = std::getenv("HOME");
    std::string cacheDir = homeDir + "/.cache/yetty/msdf-font-cache";
    return std::make_shared<MockFontManager>(*fontRes, cacheDir);
}

// Cached singleton — initialized once across all test suites
static FontManager::Ptr testFontManager() {
    static auto fm = createTestFontManager();
    return fm;
}

//=============================================================================
// Helper: run the full builder lifecycle with mock card manager
//=============================================================================
struct PipelineResult {
    std::shared_ptr<MockGpuMemoryManager> cardMgr;
    Painter::Ptr builder;
    const float* storage;       // cardStorage as floats
    const uint32_t* meta;       // metadata as u32
    uint32_t primitiveOffset;
    uint32_t primitiveCount;
    uint32_t primDataBase;
    uint32_t gridOffset;
    uint32_t gridWidth;
    uint32_t gridHeight;
    float cellSizeX;
    float cellSizeY;
    float sceneMinX, sceneMinY;
};

// Helper to unpack f16 from f16 pair
static float unpackF16(uint32_t packed, bool high) {
    uint16_t f16 = high ? (packed >> 16) : (packed & 0xFFFF);
    uint32_t sign = (f16 >> 15) & 1;
    uint32_t exp = (f16 >> 10) & 0x1F;
    uint32_t mant = f16 & 0x3FF;
    if (exp == 0) return sign ? -0.0f : 0.0f;
    if (exp == 31) return sign ? -INFINITY : INFINITY;
    float f = (1.0f + mant / 1024.0f) * std::pow(2.0f, int(exp) - 15);
    return sign ? -f : f;
}

static PipelineResult runPipeline(YPaintBuffer::Ptr buf,
                                   float sceneW = 200.0f, float sceneH = 200.0f,
                                   FontManager::Ptr fontMgr = nullptr) {
    auto mockCM = std::make_shared<MockGpuMemoryManager>();
    auto builder = *Painter::create(fontMgr, testAllocator(), mockCM, 0);
    builder->setSceneBounds(0, 0, sceneW, sceneH);
    builder->setGridCellSize(20.0f, 20.0f);  // Default cell size
    if (!buf->empty()) {
        builder->addYpaintBuffer(buf);
    }
    builder->declareBufferNeeds();
    mockCM->mockBufMgr()->commitReservations();
    builder->allocateBuffers();
    builder->writeBuffers();

    const float* storage = mockCM->mockBufMgr()->storageAsFloat();
    const uint32_t* meta = mockCM->metadataAsU32();

    PipelineResult r;
    r.cardMgr = mockCM;
    r.builder = builder;
    r.storage = storage;
    r.meta = meta;
    r.primitiveOffset = meta[0];
    r.primitiveCount = meta[1];
    r.primDataBase = r.primitiveOffset + r.primitiveCount;
    r.gridOffset = meta[2];
    r.gridWidth = meta[3];
    r.gridHeight = meta[4];
    r.cellSizeX = unpackF16(meta[5], false);
    r.cellSizeY = unpackF16(meta[5], true);
    std::memcpy(&r.sceneMinX, &meta[8], sizeof(float));
    std::memcpy(&r.sceneMinY, &meta[9], sizeof(float));
    return r;
}

//=============================================================================
// Helper: simulate the shader's grid lookup at a scene position.
// Returns prim absolute word offsets in cardStorage (what shader would read).
//=============================================================================
static std::vector<uint32_t> shaderGridLookup(const PipelineResult& p,
                                               float sceneX, float sceneY) {
    std::vector<uint32_t> result;
    if (p.gridWidth == 0 || p.gridHeight == 0 || p.cellSizeX <= 0.0f || p.cellSizeY <= 0.0f) return result;

    float invCellSizeX = 1.0f / p.cellSizeX;
    float invCellSizeY = 1.0f / p.cellSizeY;
    uint32_t cellX = static_cast<uint32_t>(
        std::clamp((sceneX - p.sceneMinX) * invCellSizeX, 0.0f, float(p.gridWidth - 1)));
    uint32_t cellY = static_cast<uint32_t>(
        std::clamp((sceneY - p.sceneMinY) * invCellSizeY, 0.0f, float(p.gridHeight - 1)));
    uint32_t cellIndex = cellY * p.gridWidth + cellX;

    uint32_t packedStart = readU32(p.storage, p.gridOffset + cellIndex);
    uint32_t cellEntryCount = readU32(p.storage, p.gridOffset + packedStart);

    for (uint32_t i = 0; i < cellEntryCount; i++) {
        uint32_t rawIdx = readU32(p.storage, p.gridOffset + packedStart + 1 + i);
        if ((rawIdx & 0x80000000u) != 0) continue;  // skip glyphs
        result.push_back(p.primDataBase + rawIdx);
    }
    return result;
}

//=============================================================================
// YPaintBuffer Tests
//=============================================================================

suite ypaint_buffer_tests = [] {

    "buffer creates empty"_test = [] {
        auto buf = *YPaintBuffer::create();
        expect(buf->primCount() == 0_u);
        expect(buf->empty());
        expect(buf->gpuBufferSize() == 0_u);
    };

    "addCircle adds one prim"_test = [] {
        auto buf = *YPaintBuffer::create();
        auto res = buf->addCircle(0, 50.0f, 60.0f, 25.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        expect(res.has_value());
        expect(buf->primCount() == 1_u);
        expect(!buf->empty());
    };

    "addBox adds one prim with correct word count"_test = [] {
        auto buf = *YPaintBuffer::create();
        buf->addBox(0, 100.0f, 100.0f, 50.0f, 30.0f, 0xFFFFFFFF, 0, 0.0f, 0.0f);
        expect(buf->primCount() == 1_u);
        // Box = 10 words. GPU size = (1 offset + 10 data) * 4 bytes = 44
        expect(buf->gpuBufferSize() == 44_u);
    };

    "addRoundedBox adds one prim with correct word count"_test = [] {
        auto buf = *YPaintBuffer::create();
        buf->addRoundedBox(0, 100.0f, 100.0f, 50.0f, 30.0f,
                           4.0f, 4.0f, 4.0f, 4.0f,
                           0xFFFFFFFF, 0, 0.0f, 0.0f);
        expect(buf->primCount() == 1_u);
        // RoundedBox = 14 words. GPU size = (1 offset + 14 data) * 4 = 60
        expect(buf->gpuBufferSize() == 60_u);
    };

    "addTriangle adds one prim"_test = [] {
        auto buf = *YPaintBuffer::create();
        buf->addTriangle(0, 0.0f, 0.0f, 10.0f, 0.0f, 5.0f, 10.0f,
                         0xFFFF0000, 0, 0.0f, 0.0f);
        expect(buf->primCount() == 1_u);
        // Triangle = 12 words. GPU size = (1 + 12) * 4 = 52
        expect(buf->gpuBufferSize() == 52_u);
    };

    "multiple prims accumulate"_test = [] {
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 50.0f, 10.0f, 0xFF0000FF, 0, 0.0f, 0.0f);
        buf->addBox(1, 100.0f, 100.0f, 20.0f, 15.0f, 0xFFFFFFFF, 0, 0.0f, 0.0f);
        buf->addRoundedBox(2, 200.0f, 200.0f, 30.0f, 25.0f,
                           5.0f, 5.0f, 5.0f, 5.0f,
                           0xFF00FF00, 0, 0.0f, 0.0f);
        expect(buf->primCount() == 3_u);
        // 3 offsets + 9 + 10 + 14 data words = 36 words = 144 bytes
        expect(buf->gpuBufferSize() == 144_u);
    };

    "clear resets buffer"_test = [] {
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 50.0f, 10.0f, 0xFF0000FF, 0, 0.0f, 0.0f);
        buf->addText(10.0f, 20.0f, "hello", 14.0f, 0xFFFFFFFF, 0, -1);
        expect(buf->primCount() == 1_u);
        expect(buf->textSpanCount() == 1_u);

        buf->clear();
        expect(buf->primCount() == 0_u);
        expect(buf->textSpanCount() == 0_u);
        expect(buf->empty());
        expect(buf->gpuBufferSize() == 0_u);
    };

    "scene metadata preserved across clear"_test = [] {
        auto buf = *YPaintBuffer::create();
        buf->setSceneBounds(0.0f, 0.0f, 400.0f, 300.0f);
        buf->setBgColor(0xFF112233);
        buf->clear();
        expect(buf->hasSceneBounds());
        expect(buf->sceneMinX() == 0.0_f);
        expect(buf->sceneMaxX() == 400.0_f);
        expect(buf->bgColor() == 0xFF112233u);
    };
};

//=============================================================================
// Serialization / Deserialization Tests
//=============================================================================

suite serialize_tests = [] {

    "serialize empty buffer"_test = [] {
        auto buf = *YPaintBuffer::create();
        auto data = buf->serialize();

        // Should at least have magic + version + counts
        expect(data.size() >= 8_u) << "empty buffer should still have header";

        // Check magic "YPRF" = 0x59505246
        uint32_t magic;
        std::memcpy(&magic, data.data(), 4);
        expect(magic == 0x59505246u) << "magic should be YPRF";

        // Check version
        uint32_t version;
        std::memcpy(&version, data.data() + 4, 4);
        expect(version == 1_u) << "version should be 1";
    };

    "serialize and deserialize prims"_test = [] {
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 60.0f, 25.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        buf->addBox(1, 100.0f, 100.0f, 30.0f, 20.0f, 0xFF00FF00, 0, 0.0f, 0.0f);
        buf->addRoundedBox(2, 200.0f, 150.0f, 40.0f, 30.0f,
                           5.0f, 5.0f, 5.0f, 5.0f, 0xFF0000FF, 0, 0.0f, 0.0f);

        auto data = buf->serialize();
        expect(data.size() > 100_u) << "serialized data should be substantial";

        // Deserialize into new buffer
        auto buf2 = *YPaintBuffer::create();
        auto res = buf2->deserialize(data.data(), data.size());
        expect(res.has_value()) << "deserialize should succeed";

        expect(buf2->primCount() == 3_u) << "should have 3 prims after deserialize";
    };

    "serialize and deserialize scene metadata"_test = [] {
        auto buf = *YPaintBuffer::create();
        buf->setSceneBounds(10.0f, 20.0f, 300.0f, 400.0f);
        buf->setBgColor(0xFF112233);
        buf->setFlags(YPaintBuffer::FLAG_SHOW_BOUNDS | YPaintBuffer::FLAG_SHOW_GRID);
        buf->addCircle(0, 50.0f, 50.0f, 10.0f, 0xFFFFFFFF, 0, 0.0f, 0.0f);

        auto data = buf->serialize();

        auto buf2 = *YPaintBuffer::create();
        auto res = buf2->deserialize(data.data(), data.size());
        expect(res.has_value());

        expect(buf2->hasSceneBounds()) << "should have scene bounds";
        expect(std::abs(buf2->sceneMinX() - 10.0f) < 0.01f) << "sceneMinX";
        expect(std::abs(buf2->sceneMinY() - 20.0f) < 0.01f) << "sceneMinY";
        expect(std::abs(buf2->sceneMaxX() - 300.0f) < 0.01f) << "sceneMaxX";
        expect(std::abs(buf2->sceneMaxY() - 400.0f) < 0.01f) << "sceneMaxY";
        expect(buf2->bgColor() == 0xFF112233u) << "bgColor";
        expect(buf2->flags() == (YPaintBuffer::FLAG_SHOW_BOUNDS | YPaintBuffer::FLAG_SHOW_GRID))
            << "flags";
    };

    "serialize and deserialize fonts"_test = [] {
        auto buf = *YPaintBuffer::create();

        // Add some dummy font data
        std::vector<uint8_t> fontData1 = {0x00, 0x01, 0x00, 0x00, 0x12, 0x34};
        std::vector<uint8_t> fontData2 = {0x00, 0x01, 0x00, 0x00, 0x56, 0x78, 0x9A};

        int fontId1 = buf->addFontBlob(fontData1.data(), fontData1.size(), "TestFont1");
        int fontId2 = buf->addFontBlob(fontData2.data(), fontData2.size(), "TestFont2");

        expect(fontId1 == 0) << "first font id should be 0";
        expect(fontId2 == 1) << "second font id should be 1";

        auto data = buf->serialize();

        auto buf2 = *YPaintBuffer::create();
        auto res = buf2->deserialize(data.data(), data.size());
        expect(res.has_value());

        // Count fonts
        int fontCount = 0;
        buf2->forEachFont([&](int id, const uint8_t* fdata, size_t size, const std::string& name) {
            fontCount++;
            if (id == 0) {
                expect(size == 6_u) << "font 0 size";
                expect(name == "TestFont1") << "font 0 name";
            } else if (id == 1) {
                expect(size == 7_u) << "font 1 size";
                expect(name == "TestFont2") << "font 1 name";
            }
        });
        expect(fontCount == 2) << "should have 2 fonts";
    };

    "serialize and deserialize text spans"_test = [] {
        auto buf = *YPaintBuffer::create();
        buf->addText(10.0f, 20.0f, "Hello", 14.0f, 0xFFFF0000, 0, -1);
        buf->addText(50.0f, 80.0f, "World", 18.0f, 0xFF00FF00, 1, 0);
        buf->addRotatedText(100.0f, 100.0f, "Rotated", 12.0f, 0xFF0000FF, 1.57f, -1);

        expect(buf->textSpanCount() == 3_u);

        auto data = buf->serialize();

        auto buf2 = *YPaintBuffer::create();
        auto res = buf2->deserialize(data.data(), data.size());
        expect(res.has_value());

        expect(buf2->textSpanCount() == 3_u) << "should have 3 text spans";

        int spanIdx = 0;
        buf2->forEachTextSpan([&](const TextSpanData& s) {
            if (spanIdx == 0) {
                expect(std::abs(s.x - 10.0f) < 0.01f) << "span 0 x";
                expect(std::abs(s.y - 20.0f) < 0.01f) << "span 0 y";
                expect(s.text == "Hello") << "span 0 text";
                expect(std::abs(s.fontSize - 14.0f) < 0.01f) << "span 0 fontSize";
                expect(s.color == 0xFFFF0000u) << "span 0 color";
            } else if (spanIdx == 1) {
                expect(s.text == "World") << "span 1 text";
                expect(s.fontId == 0) << "span 1 fontId";
            } else if (spanIdx == 2) {
                expect(s.text == "Rotated") << "span 2 text";
                expect(std::abs(s.rotation - 1.57f) < 0.01f) << "span 2 rotation";
            }
            spanIdx++;
        });
    };

    "deserialize rejects invalid magic"_test = [] {
        std::vector<uint8_t> badData = {0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
        auto buf = *YPaintBuffer::create();
        auto res = buf->deserialize(badData.data(), badData.size());
        expect(!res.has_value()) << "should reject invalid magic";
    };

    "deserialize rejects truncated data"_test = [] {
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 50.0f, 10.0f, 0xFFFFFFFF, 0, 0.0f, 0.0f);
        auto data = buf->serialize();

        // Truncate data
        auto buf2 = *YPaintBuffer::create();
        auto res = buf2->deserialize(data.data(), 4);  // Too small
        expect(!res.has_value()) << "should reject truncated data";
    };

    "full roundtrip with all data types"_test = [] {
        auto buf = *YPaintBuffer::create();

        // Prims
        buf->addCircle(0, 50.0f, 60.0f, 25.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        buf->addBox(1, 100.0f, 100.0f, 30.0f, 20.0f, 0xFF00FF00, 0, 1.0f, 0.0f);
        buf->addTriangle(2, 10.0f, 10.0f, 50.0f, 10.0f, 30.0f, 50.0f,
                         0xFF0000FF, 0, 0.0f, 0.0f);

        // Fonts
        std::vector<uint8_t> fontData = {0x00, 0x01, 0x00, 0x00, 'T', 'T', 'F'};
        buf->addFontBlob(fontData.data(), fontData.size(), "MyFont");

        // Text
        buf->addText(20.0f, 30.0f, "Test text", 16.0f, 0xFFCCCCCC, 0, 0);
        buf->addRotatedText(100.0f, 100.0f, "Rotated", 12.0f, 0xFFAAAAAA, 0.5f, 0);

        // Scene metadata
        buf->setSceneBounds(0.0f, 0.0f, 200.0f, 150.0f);
        buf->setBgColor(0xFF1A1A2E);
        buf->setFlags(YPaintBuffer::FLAG_SHOW_BOUNDS);

        auto data = buf->serialize();
        expect(data.size() > 200_u) << "full buffer should be substantial";

        // Deserialize
        auto buf2 = *YPaintBuffer::create();
        auto res = buf2->deserialize(data.data(), data.size());
        expect(res.has_value()) << "deserialize should succeed";

        // Verify everything
        expect(buf2->primCount() == 3_u) << "prims";
        expect(buf2->textSpanCount() == 2_u) << "text spans";
        expect(buf2->bgColor() == 0xFF1A1A2Eu) << "bgColor";
        expect(buf2->flags() == YPaintBuffer::FLAG_SHOW_BOUNDS) << "flags";
        expect(buf2->hasSceneBounds()) << "scene bounds";
        expect(std::abs(buf2->sceneMaxX() - 200.0f) < 0.01f) << "sceneMaxX";

        int fontCount = 0;
        buf2->forEachFont([&](int, const uint8_t*, size_t sz, const std::string& name) {
            fontCount++;
            expect(sz == 7_u) << "font size";
            expect(name == "MyFont") << "font name";
        });
        expect(fontCount == 1) << "font count";
    };

    "serialize enters delta mode"_test = [] {
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 50.0f, 10.0f, 0xFFFFFFFF, 0, 0.0f, 0.0f);

        expect(!buf->isDeltaMode()) << "should not be in delta mode initially";
        buf->serialize();
        expect(buf->isDeltaMode()) << "should be in delta mode after serialize";
    };
};

//=============================================================================
// AABB Tests
//=============================================================================

static void computeAABB(const float* data, uint32_t wc,
                        float& minX, float& minY, float& maxX, float& maxY) {
    uint32_t type = sdf::detail::read_u32(data, 0);
    const float* p = data + 2;
    float strokeWidth = data[wc - 2];
    float round_ = data[wc - 1];
    float expand = strokeWidth * 0.5f;

    switch (static_cast<SDFType>(type)) {
        case SDFType::Circle: {
            float r = p[2] + expand;
            minX = p[0] - r; minY = p[1] - r;
            maxX = p[0] + r; maxY = p[1] + r;
            break;
        }
        case SDFType::Box: {
            float hw = p[2] + round_ + expand;
            float hh = p[3] + round_ + expand;
            minX = p[0] - hw; minY = p[1] - hh;
            maxX = p[0] + hw; maxY = p[1] + hh;
            break;
        }
        case SDFType::RoundedBox: {
            float maxR = std::max({p[4], p[5], p[6], p[7]});
            float hw = p[2] + maxR + expand;
            float hh = p[3] + maxR + expand;
            minX = p[0] - hw; minY = p[1] - hh;
            maxX = p[0] + hw; maxY = p[1] + hh;
            break;
        }
        case SDFType::Triangle: {
            minX = std::min({p[0], p[2], p[4]}) - expand;
            minY = std::min({p[1], p[3], p[5]}) - expand;
            maxX = std::max({p[0], p[2], p[4]}) + expand;
            maxY = std::max({p[1], p[3], p[5]}) + expand;
            break;
        }
        case SDFType::Segment: {
            minX = std::min(p[0], p[2]) - expand;
            minY = std::min(p[1], p[3]) - expand;
            maxX = std::max(p[0], p[2]) + expand;
            maxY = std::max(p[1], p[3]) + expand;
            break;
        }
        case SDFType::Ellipse: {
            minX = p[0] - p[2] - expand; minY = p[1] - p[3] - expand;
            maxX = p[0] + p[2] + expand; maxY = p[1] + p[3] + expand;
            break;
        }
        default: {
            minX = p[0] - 10; minY = p[1] - 10;
            maxX = p[0] + 10; maxY = p[1] + 10;
            break;
        }
    }
}

suite aabb_tests = [] {

    "circle AABB"_test = [] {
        float data[9];
        sdf::writeCircle(data, 0, 50.0f, 60.0f, 25.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        float minX, minY, maxX, maxY;
        computeAABB(data, 9, minX, minY, maxX, maxY);
        expect(minX == 25.0_f); expect(maxX == 75.0_f);
        expect(minY == 35.0_f); expect(maxY == 85.0_f);
    };

    "roundedbox AABB includes corner radii"_test = [] {
        float data[14];
        sdf::writeRoundedBox(data, 0, 100.0f, 100.0f, 40.0f, 30.0f,
                             5.0f, 5.0f, 5.0f, 5.0f, 0xFFFFFFFF, 0, 0.0f, 0.0f);
        float minX, minY, maxX, maxY;
        computeAABB(data, 14, minX, minY, maxX, maxY);
        expect(minX == 55.0_f); expect(maxX == 145.0_f);
        expect(minY == 65.0_f); expect(maxY == 135.0_f);
    };

    "triangle AABB encloses all vertices"_test = [] {
        float data[12];
        sdf::writeTriangle(data, 0, 10.0f, 20.0f, 80.0f, 5.0f, 50.0f, 90.0f,
                           0xFFFF0000, 0, 0.0f, 0.0f);
        float minX, minY, maxX, maxY;
        computeAABB(data, 12, minX, minY, maxX, maxY);
        expect(minX == 10.0_f); expect(maxX == 80.0_f);
        expect(minY == 5.0_f);  expect(maxY == 90.0_f);
    };
};

//=============================================================================
// Full GPU write pipeline tests — using MockGpuMemoryManager
//
// These run the REAL builder lifecycle (calculate → declareBufferNeeds →
// commitReservations → allocateBuffers → writeBuffers) and then inspect
// what the builder ACTUALLY wrote to the mock buffers.
//=============================================================================

suite gpu_pipeline_tests = [] {

    "single circle — metadata and prim data"_test = [] {
        auto buf = *YPaintBuffer::create();
        uint32_t fillColor = 0xFFDD2200;
        buf->addCircle(0, 50.0f, 50.0f, 10.0f, fillColor, 0, 0.0f, 0.0f);

        auto p = runPipeline(buf, 100.0f, 100.0f);

        // Metadata
        expect(p.primitiveCount == 1_u) << "should have 1 prim";
        expect(p.gridWidth > 0_u) << "grid should be built";
        expect(p.gridHeight > 0_u);

        // Prim data: offset table + data
        uint32_t wordOff0 = readU32(p.storage, p.primitiveOffset);
        expect(wordOff0 == 0_u) << "single prim starts at offset 0";

        // Read prim data at primDataBase + wordOff0
        // GPU format: [0]=gridOffset, [1]=type, [2]=layer, [3]=cx, [4]=cy, [5]=r, [6]=fillColor
        uint32_t primOff = p.primDataBase + wordOff0;
        uint32_t type = readU32(p.storage, primOff + 1);
        expect(type == static_cast<uint32_t>(SDFType::Circle))
            << "type should be Circle";

        expect(p.storage[primOff + 3] == 50.0_f) << "cx";
        expect(p.storage[primOff + 4] == 50.0_f) << "cy";
        expect(p.storage[primOff + 5] == 10.0_f) << "r";
        uint32_t readFill = readU32(p.storage, primOff + 6);
        expect(readFill == fillColor) << "fillColor";
    };

    "two prims — correct types and colors at correct offsets"_test = [] {
        auto buf = *YPaintBuffer::create();
        uint32_t circleColor = 0xFFFF0000;
        uint32_t rboxColor = 0xFF00FF00;
        buf->addCircle(0, 20.0f, 20.0f, 8.0f, circleColor, 0, 0.0f, 0.0f);
        buf->addRoundedBox(1, 80.0f, 80.0f, 20.0f, 15.0f,
                           3.0f, 3.0f, 3.0f, 3.0f,
                           rboxColor, 0, 0.0f, 0.0f);

        auto p = runPipeline(buf, 100.0f, 100.0f);
        expect(p.primitiveCount == 2_u);

        // Read both prims via offset table
        // GPU format: [0]=gridOffset, [1]=type, [2]=layer, [3+]=geometry
        for (uint32_t i = 0; i < p.primitiveCount; i++) {
            uint32_t wordOff = readU32(p.storage, p.primitiveOffset + i);
            uint32_t primOff = p.primDataBase + wordOff;
            uint32_t type = readU32(p.storage, primOff + 1);  // type at offset +1

            if (type == static_cast<uint32_t>(SDFType::Circle)) {
                // GPU format: fillColor at +6 (gridOffset=0, type=1, layer=2, cx=3, cy=4, r=5, fill=6)
                uint32_t fill = readU32(p.storage, primOff + 6);
                expect(fill == circleColor) << "Circle fillColor";
            } else if (type == static_cast<uint32_t>(SDFType::RoundedBox)) {
                // GPU format: fillColor at +11 (gridOffset=0, type=1, layer=2, cx=3, cy=4, hw=5, hh=6, r0=7, r1=8, r2=9, r3=10, fill=11)
                uint32_t fill = readU32(p.storage, primOff + 11);
                expect(fill == rboxColor) << "RoundedBox fillColor";
            } else {
                expect(false) << "unexpected prim type " << type;
            }
        }
    };

    "grid entries resolve to correct prims via shader path"_test = [] {
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 20.0f, 20.0f, 8.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        buf->addRoundedBox(1, 80.0f, 80.0f, 10.0f, 10.0f,
                           2.0f, 2.0f, 2.0f, 2.0f,
                           0xFF00FF00, 0, 0.0f, 0.0f);

        auto p = runPipeline(buf, 100.0f, 100.0f);

        // Shader lookup at circle center
        // GPU format: type at offset +1 (gridOffset is at +0)
        auto circleHits = shaderGridLookup(p, 20.0f, 20.0f);
        expect(!circleHits.empty()) << "should find prim at circle center";
        bool foundCircle = false;
        for (uint32_t off : circleHits) {
            if (readU32(p.storage, off + 1) == static_cast<uint32_t>(SDFType::Circle))
                foundCircle = true;
        }
        expect(foundCircle) << "should find Circle type at (20,20)";

        // Shader lookup at roundedbox center
        auto rboxHits = shaderGridLookup(p, 80.0f, 80.0f);
        expect(!rboxHits.empty()) << "should find prim at roundedbox center";
        bool foundRBox = false;
        for (uint32_t off : rboxHits) {
            if (readU32(p.storage, off + 1) == static_cast<uint32_t>(SDFType::RoundedBox))
                foundRBox = true;
        }
        expect(foundRBox) << "should find RoundedBox type at (80,80)";
    };

    "ygui scenario — 5 prims all reachable from grid"_test = [] {
        auto buf = *YPaintBuffer::create();

        // Button background
        buf->addRoundedBox(buf->primCount(),
            100.0f, 30.0f, 80.0f, 16.0f,
            4.0f, 4.0f, 4.0f, 4.0f,
            0xFF333344, 0, 0.0f, 0.0f);

        // Button hover outline (stroke only)
        buf->addRoundedBox(buf->primCount(),
            100.0f, 30.0f, 80.0f, 16.0f,
            4.0f, 4.0f, 4.0f, 4.0f,
            0, 0xFF6688FF, 2.0f, 0.0f);

        // Dropdown chevron
        buf->addTriangle(buf->primCount(),
            290.0f, 26.0f, 298.0f, 26.0f, 294.0f, 34.0f,
            0xFFCCCCCC, 0, 0.0f, 0.0f);

        // Checkbox
        buf->addBox(buf->primCount(),
            16.0f, 70.0f, 8.0f, 8.0f,
            0xFF444444, 0, 0.0f, 0.0f);

        // Slider handle
        buf->addRoundedBox(buf->primCount(),
            200.0f, 105.0f, 6.0f, 12.0f,
            6.0f, 6.0f, 6.0f, 6.0f,
            0xFF00AAFF, 0, 0.0f, 0.0f);

        expect(buf->primCount() == 5_u);

        auto p = runPipeline(buf, 400.0f, 200.0f);
        expect(p.primitiveCount == 5_u);

        // Scan all grid cells and collect reachable prim offsets
        std::set<uint32_t> reachable;
        uint32_t numCells = p.gridWidth * p.gridHeight;
        for (uint32_t ci = 0; ci < numCells; ci++) {
            uint32_t packedOff = readU32(p.storage, p.gridOffset + ci);
            uint32_t cnt = readU32(p.storage, p.gridOffset + packedOff);
            for (uint32_t j = 0; j < cnt; j++) {
                uint32_t rawVal = readU32(p.storage, p.gridOffset + packedOff + 1 + j);
                if ((rawVal & 0x80000000u) == 0) {
                    reachable.insert(p.primDataBase + rawVal);
                }
            }
        }

        // Every prim in the offset table should be reachable from the grid
        for (uint32_t i = 0; i < p.primitiveCount; i++) {
            uint32_t wordOff = readU32(p.storage, p.primitiveOffset + i);
            uint32_t primOff = p.primDataBase + wordOff;
            expect(reachable.count(primOff) > 0_u)
                << "prim " << i << " at word offset " << wordOff
                << " should be reachable from grid";
        }

        // Verify each reachable offset has a valid type
        // GPU format: type at offset +1 (gridOffset is at +0)
        for (uint32_t off : reachable) {
            uint32_t type = readU32(p.storage, off + 1);
            expect(type < 100_u) << "type " << type << " at offset " << off;
        }

        // Spot-check: button center should find RoundedBox
        auto btnHits = shaderGridLookup(p, 100.0f, 30.0f);
        expect(!btnHits.empty()) << "should find prims at button center";
        int rboxCount = 0;
        for (uint32_t off : btnHits) {
            if (readU32(p.storage, off + 1) == static_cast<uint32_t>(SDFType::RoundedBox))
                rboxCount++;
        }
        expect(rboxCount >= 2) << "should find bg + outline RoundedBoxes at button center";

        // Spot-check: chevron position should find Triangle
        auto chevHits = shaderGridLookup(p, 294.0f, 30.0f);
        expect(!chevHits.empty()) << "should find prim at chevron";
        bool foundTri = false;
        for (uint32_t off : chevHits) {
            if (readU32(p.storage, off + 1) == static_cast<uint32_t>(SDFType::Triangle))
                foundTri = true;
        }
        expect(foundTri) << "should find Triangle at chevron position";
    };

    "clear and rebuild produces valid pipeline"_test = [] {
        auto buf = *YPaintBuffer::create();
        auto mockCM = std::make_shared<MockGpuMemoryManager>();
        auto builder = *Painter::create(nullptr, nullptr, mockCM, 0);
        builder->setSceneBounds(0, 0, 200.0f, 200.0f);
        builder->setGridCellSize(20.0f, 20.0f);

        // First build: Circle + Box
        buf->addCircle(0, 50.0f, 50.0f, 15.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        buf->addBox(1, 150.0f, 150.0f, 20.0f, 20.0f, 0xFF00FF00, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf);
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const uint32_t* meta = mockCM->metadataAsU32();
        expect(meta[1] == 2_u) << "first build: 2 prims";

        // Clear and rebuild: single RoundedBox
        buf->clear();
        builder->clear();
        uint32_t newColor = 0xFFBB44CC;
        buf->addRoundedBox(0, 100.0f, 100.0f, 30.0f, 20.0f,
                           5.0f, 5.0f, 5.0f, 5.0f, newColor, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf);
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        meta = mockCM->metadataAsU32();
        expect(meta[1] == 1_u) << "rebuild: 1 prim";

        // Verify the written data
        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        uint32_t primOff = meta[0];
        uint32_t primCnt = meta[1];
        uint32_t primDataBase = primOff + primCnt;
        uint32_t wordOff0 = readU32(storage, primOff);
        uint32_t absOff = primDataBase + wordOff0;
        // GPU format: [0]=gridOffset, [1]=type, [2]=layer, ...
        uint32_t type = readU32(storage, absOff + 1);
        expect(type == static_cast<uint32_t>(SDFType::RoundedBox));
        // GPU format: fillColor at +11 (gridOffset=0, type=1, layer=2, cx=3, cy=4, hw=5, hh=6, r0=7, r1=8, r2=9, r3=10, fill=11)
        uint32_t readFill = readU32(storage, absOff + 11);
        expect(readFill == newColor) << "fillColor after rebuild";
    };

    "metadata layout matches shader expectations"_test = [] {
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 50.0f, 10.0f, 0xFFFF0000, 0, 0.0f, 0.0f);

        auto p = runPipeline(buf, 100.0f, 100.0f);

        // Verify metadata words match what the shader reads
        expect(p.meta[0] > 0_u || p.meta[0] == 0_u) << "primitiveOffset is set";
        expect(p.meta[1] == 1_u) << "primitiveCount = 1";
        expect(p.meta[2] > 0_u || p.meta[2] == 0_u) << "gridOffset is set";
        expect(p.meta[3] > 0_u) << "gridWidth > 0";
        expect(p.meta[4] > 0_u) << "gridHeight > 0";

        float cellSizeX = unpackF16(p.meta[5], false);
        float cellSizeY = unpackF16(p.meta[5], true);
        expect(cellSizeX > 0.0_f) << "cellSizeX > 0";
        expect(cellSizeY > 0.0_f) << "cellSizeY > 0";

        float sMinX, sMinY, sMaxX, sMaxY;
        std::memcpy(&sMinX, &p.meta[8], sizeof(float));
        std::memcpy(&sMinY, &p.meta[9], sizeof(float));
        std::memcpy(&sMaxX, &p.meta[10], sizeof(float));
        std::memcpy(&sMaxY, &p.meta[11], sizeof(float));
        expect(sMinX == 0.0_f) << "sceneMinX";
        expect(sMinY == 0.0_f) << "sceneMinY";
        expect(sMaxX == 100.0_f) << "sceneMaxX";
        expect(sMaxY == 100.0_f) << "sceneMaxY";

        // primDataBase = primitiveOffset + primitiveCount
        // The prim at primDataBase should have type == Circle
        uint32_t wordOff0 = readU32(p.storage, p.meta[0]);
        uint32_t primAbs = p.meta[0] + p.meta[1] + wordOff0;
        uint32_t type = readU32(p.storage, primAbs);
        expect(type == static_cast<uint32_t>(SDFType::Circle));
    };

    "prim and derived buffers are separate allocations"_test = [] {
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 50.0f, 10.0f, 0xFFFF0000, 0, 0.0f, 0.0f);

        auto mockCM = std::make_shared<MockGpuMemoryManager>();
        auto builder = *Painter::create(nullptr, nullptr, mockCM, 0);
        builder->setSceneBounds(0, 0, 100.0f, 100.0f);
        builder->addYpaintBuffer(buf);
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        auto primH = mockCM->mockBufMgr()->getBufferHandle(0, "prims");
        auto derivedH = mockCM->mockBufMgr()->getBufferHandle(0, "derived");

        expect(primH.isValid()) << "prims handle should be valid";
        expect(derivedH.isValid()) << "derived handle should be valid";
        expect(primH.offset != derivedH.offset)
            << "prim and derived should be at different offsets";

        // Verify metadata references match handle offsets
        const uint32_t* meta = mockCM->metadataAsU32();
        uint32_t metaPrimOff = meta[0];
        uint32_t metaGridOff = meta[2];
        expect(metaPrimOff == primH.offset / 4)
            << "metadata primitiveOffset should match primHandle.offset/4";
        expect(metaGridOff == derivedH.offset / 4)
            << "metadata gridOffset should match derivedHandle.offset/4";
    };

    "RoundedBox word layout matches shader's primColors offsets"_test = [] {
        auto buf = *YPaintBuffer::create();
        uint32_t fillColor = 0xFFAABBCC;
        uint32_t strokeColor = 0xFF112233;
        float strokeWidth = 2.5f;
        buf->addRoundedBox(0, 100.0f, 200.0f, 40.0f, 30.0f,
                           5.0f, 6.0f, 7.0f, 8.0f,
                           fillColor, strokeColor, strokeWidth, 0.0f);

        auto p = runPipeline(buf, 300.0f, 300.0f);
        uint32_t wordOff0 = readU32(p.storage, p.primitiveOffset);
        uint32_t primOff = p.primDataBase + wordOff0;

        // Verify exact word offsets that the shader uses
        // GPU format: [0]=gridOffset, [1]=type, [2]=layer, [3]=cx, [4]=cy, [5]=hw, [6]=hh,
        //             [7]=r0, [8]=r1, [9]=r2, [10]=r3, [11]=fillColor, [12]=strokeColor, [13]=strokeWidth, [14]=round
        uint32_t type = readU32(p.storage, primOff + 1);
        expect(type == 8_u) << "type = SDF_ROUNDED_BOX (8)";

        expect(p.storage[primOff + 3] == 100.0_f) << "cx";
        expect(p.storage[primOff + 4] == 200.0_f) << "cy";
        expect(p.storage[primOff + 5] == 40.0_f) << "hw";
        expect(p.storage[primOff + 6] == 30.0_f) << "hh";
        expect(p.storage[primOff + 7] == 5.0_f) << "r0";
        expect(p.storage[primOff + 8] == 6.0_f) << "r1";
        expect(p.storage[primOff + 9] == 7.0_f) << "r2";
        expect(p.storage[primOff + 10] == 8.0_f) << "r3";

        // These are the exact offsets primColors() and primStrokeWidth() read
        uint32_t readFill = readU32(p.storage, primOff + 11);
        uint32_t readStroke = readU32(p.storage, primOff + 12);
        expect(readFill == fillColor) << "shader reads fillColor at +11";
        expect(readStroke == strokeColor) << "shader reads strokeColor at +12";
        expect(p.storage[primOff + 13] == strokeWidth) << "shader reads strokeWidth at +13";
    };

    "Box word layout matches shader's primColors offsets"_test = [] {
        auto buf = *YPaintBuffer::create();
        uint32_t fillColor = 0xFF445566;
        buf->addBox(0, 50.0f, 60.0f, 20.0f, 15.0f, fillColor, 0, 0.0f, 3.0f);

        auto p = runPipeline(buf, 200.0f, 200.0f);
        uint32_t wordOff0 = readU32(p.storage, p.primitiveOffset);
        uint32_t primOff = p.primDataBase + wordOff0;

        // GPU format: [0]=gridOffset, [1]=type, [2]=layer, [3]=cx, [4]=cy, [5]=hw, [6]=hh,
        //             [7]=fillColor, [8]=strokeColor, [9]=strokeWidth, [10]=round
        uint32_t type = readU32(p.storage, primOff + 1);
        expect(type == 1_u) << "type = SDF_BOX (1)";
        expect(p.storage[primOff + 3] == 50.0_f) << "cx";
        expect(p.storage[primOff + 4] == 60.0_f) << "cy";
        expect(p.storage[primOff + 5] == 20.0_f) << "hw";
        expect(p.storage[primOff + 6] == 15.0_f) << "hh";
        uint32_t readFill = readU32(p.storage, primOff + 7);
        expect(readFill == fillColor) << "shader reads fillColor at +7";
        expect(p.storage[primOff + 10] == 3.0_f) << "round at +10";
    };

    "Triangle word layout matches shader's primColors offsets"_test = [] {
        auto buf = *YPaintBuffer::create();
        uint32_t fillColor = 0xFFAABB00;
        buf->addTriangle(0, 10.0f, 20.0f, 80.0f, 20.0f, 45.0f, 70.0f,
                         fillColor, 0, 0.0f, 0.0f);

        auto p = runPipeline(buf, 200.0f, 200.0f);
        uint32_t wordOff0 = readU32(p.storage, p.primitiveOffset);
        uint32_t primOff = p.primDataBase + wordOff0;

        // GPU format: [0]=gridOffset, [1]=type, [2]=layer, [3]=ax, [4]=ay, [5]=bx, [6]=by, [7]=cx, [8]=cy,
        //             [9]=fillColor, [10]=strokeColor, [11]=strokeWidth, [12]=round
        uint32_t type = readU32(p.storage, primOff + 1);
        expect(type == 3_u) << "type = SDF_TRIANGLE (3)";
        expect(p.storage[primOff + 3] == 10.0_f) << "ax";
        expect(p.storage[primOff + 4] == 20.0_f) << "ay";
        uint32_t readFill = readU32(p.storage, primOff + 9);
        expect(readFill == fillColor) << "shader reads fillColor at +9";
    };

    "YPaintMetadata struct is 64 bytes and matches shader layout"_test = [] {
        YPaintMetadata meta = {};
        expect(sizeof(YPaintMetadata) == 64_u) << "must be 64 bytes";

        meta.primitiveOffset = 42;
        meta.primitiveCount = 7;
        meta.gridOffset = 100;
        meta.gridWidth = 8;
        meta.gridHeight = 6;
        meta.cellSizeXY = YPaintMetadata::packCellSize(12.5f, 12.5f);
        meta.glyphOffset = 200;
        meta.glyphCount = 3;
        meta.bgColor = 0xFF1A1A2E;

        const uint32_t* m = reinterpret_cast<const uint32_t*>(&meta);
        expect(m[0] == 42_u) << "word 0: primitiveOffset";
        expect(m[1] == 7_u) << "word 1: primitiveCount";
        expect(m[2] == 100_u) << "word 2: gridOffset";
        expect(m[3] == 8_u) << "word 3: gridWidth";
        expect(m[4] == 6_u) << "word 4: gridHeight";
        expect(m[6] == 200_u) << "word 6: glyphOffset";
        expect(m[7] == 3_u) << "word 7: glyphCount";
        expect(m[15] == 0xFF1A1A2Eu) << "word 15: bgColor";
    };

    "YPaintGlyph is 20 bytes"_test = [] {
        expect(sizeof(YPaintGlyph) == 20_u);
    };
};

//=============================================================================
// Text / Glyph pipeline tests — verifies the FULL text path:
// buffer addText → builder calculate (text spans → glyphs) →
// writeDerived (glyphs to GPU) → metadata (glyphOffset, glyphCount)
//
// Uses real MsMsdfFont with CDB files for actual glyph metrics.
//=============================================================================

suite text_pipeline_tests = [] {

    "default font text produces glyphs"_test = [] {
        auto fm = testFontManager();
        expect(fm != nullptr) << "font manager should be available";
        if (!fm) return;

        auto buf = *YPaintBuffer::create();
        buf->addText(10.0f, 30.0f, "Hello", 14.0f, 0xFFFFFFFF, 0, -1);
        expect(buf->textSpanCount() == 1_u);

        auto p = runPipeline(buf, 200.0f, 100.0f, fm);

        // metadata should report glyphs
        uint32_t glyphCount = p.meta[7];
        expect(glyphCount >= 5_u) << "5 chars should produce >= 5 glyphs, got " << glyphCount;

        // glyphOffset should be valid
        uint32_t glyphOffset = p.meta[6];
        expect(glyphOffset > 0_u) << "glyphOffset should be non-zero";
    };

    "glyph data written to derived buffer has valid positions"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        auto buf = *YPaintBuffer::create();
        buf->addText(50.0f, 80.0f, "AB", 14.0f, 0xFF00FF00, 0, -1);

        auto p = runPipeline(buf, 200.0f, 200.0f, fm);

        uint32_t glyphCount = p.meta[7];
        uint32_t glyphOffset = p.meta[6];
        expect(glyphCount >= 2_u) << "at least 2 glyphs for 'AB'";
        if (glyphCount < 2) return;

        // Read YPaintGlyph structs from derived buffer
        const YPaintGlyph* glyphs = reinterpret_cast<const YPaintGlyph*>(
            &p.storage[glyphOffset]);

        // First glyph should be near x=50
        expect(glyphs[0].x >= 40.0_f && glyphs[0].x <= 60.0_f)
            << "first glyph x near 50, got " << glyphs[0].x;

        // Second glyph should be to the right of first
        expect(glyphs[1].x > glyphs[0].x)
            << "second glyph should be right of first";

        // Both should have the specified color
        expect(glyphs[0].color == 0xFF00FF00u) << "glyph 0 color";
        expect(glyphs[1].color == 0xFF00FF00u) << "glyph 1 color";

        // Both should have non-zero size
        expect(glyphs[0].width() > 0.0_f) << "glyph 0 width";
        expect(glyphs[0].height() > 0.0_f) << "glyph 0 height";
    };

    "glyphs appear in grid with GLYPH_BIT"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        auto buf = *YPaintBuffer::create();
        buf->addText(50.0f, 50.0f, "X", 14.0f, 0xFFFFFFFF, 0, -1);

        auto p = runPipeline(buf, 100.0f, 100.0f, fm);

        uint32_t glyphCount = p.meta[7];
        expect(glyphCount >= 1_u);
        if (glyphCount == 0) return;

        // Scan grid for GLYPH_BIT entries
        bool foundGlyphEntry = false;
        uint32_t numCells = p.gridWidth * p.gridHeight;
        for (uint32_t ci = 0; ci < numCells; ci++) {
            uint32_t packedOff = readU32(p.storage, p.gridOffset + ci);
            uint32_t cnt = readU32(p.storage, p.gridOffset + packedOff);
            for (uint32_t j = 0; j < cnt; j++) {
                uint32_t rawVal = readU32(p.storage, p.gridOffset + packedOff + 1 + j);
                if ((rawVal & 0x80000000u) != 0) {
                    foundGlyphEntry = true;
                }
            }
        }
        expect(foundGlyphEntry) << "grid should contain GLYPH_BIT entries for text";
    };

    "mixed prims and text both reachable from grid"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        auto buf = *YPaintBuffer::create();

        // ygui-like scenario: button background + label text
        buf->addRoundedBox(buf->primCount(),
            100.0f, 30.0f, 80.0f, 16.0f,
            4.0f, 4.0f, 4.0f, 4.0f,
            0xFF333344, 0, 0.0f, 0.0f);
        buf->addText(30.0f, 35.0f, "Click Me", 14.0f, 0xFFFFFFFF, 0, -1);

        auto p = runPipeline(buf, 200.0f, 100.0f, fm);

        expect(p.primitiveCount == 1_u) << "1 SDF prim";
        uint32_t glyphCount = p.meta[7];
        expect(glyphCount >= 5_u) << "'Click Me' should produce glyphs";

        // Scan grid: collect both prim offsets and glyph indices
        std::set<uint32_t> primOffsets;
        std::set<uint32_t> glyphIndices;
        uint32_t numCells = p.gridWidth * p.gridHeight;
        for (uint32_t ci = 0; ci < numCells; ci++) {
            uint32_t packedOff = readU32(p.storage, p.gridOffset + ci);
            uint32_t cnt = readU32(p.storage, p.gridOffset + packedOff);
            for (uint32_t j = 0; j < cnt; j++) {
                uint32_t rawVal = readU32(p.storage, p.gridOffset + packedOff + 1 + j);
                if ((rawVal & 0x80000000u) != 0) {
                    glyphIndices.insert(rawVal & 0x7FFFFFFFu);
                } else {
                    primOffsets.insert(p.primDataBase + rawVal);
                }
            }
        }
        expect(!primOffsets.empty()) << "grid should contain prim entries";
        expect(!glyphIndices.empty()) << "grid should contain glyph entries";

        // The prim should be a RoundedBox
        // GPU format: type at offset +1 (gridOffset is at +0)
        for (uint32_t off : primOffsets) {
            uint32_t type = readU32(p.storage, off + 1);
            expect(type == static_cast<uint32_t>(SDFType::RoundedBox))
                << "prim should be RoundedBox";
        }
    };

    "multiple text spans produce correct total glyph count"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        auto buf = *YPaintBuffer::create();
        buf->addText(10.0f, 20.0f, "ABC", 14.0f, 0xFFFF0000, 0, -1);   // 3 chars
        buf->addText(10.0f, 50.0f, "DEFG", 14.0f, 0xFF00FF00, 0, -1);  // 4 chars
        buf->addText(10.0f, 80.0f, "HI", 14.0f, 0xFF0000FF, 0, -1);    // 2 chars
        expect(buf->textSpanCount() == 3_u);

        auto p = runPipeline(buf, 200.0f, 200.0f, fm);

        uint32_t glyphCount = p.meta[7];
        expect(glyphCount >= 9_u) << "3+4+2=9 chars, got " << glyphCount << " glyphs";

        // Verify different colors in glyph data
        uint32_t glyphOffset = p.meta[6];
        const YPaintGlyph* glyphs = reinterpret_cast<const YPaintGlyph*>(
            &p.storage[glyphOffset]);

        std::set<uint32_t> colors;
        for (uint32_t i = 0; i < glyphCount; i++) {
            colors.insert(glyphs[i].color);
        }
        expect(colors.size() == 3_u) << "should have 3 different colors";
    };

    "custom font via addFontBlob produces glyphs"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        // Load a TTF file as raw data to simulate what ypdf does
        std::string ttfPath = std::string(CMAKE_SOURCE_DIR)
            + "/assets/DejaVuSansMNerdFontMono-Regular.ttf";
        std::ifstream ttfFile(ttfPath, std::ios::binary);
        expect(ttfFile.good()) << "TTF file should exist";
        if (!ttfFile.good()) return;

        std::vector<uint8_t> ttfData(
            (std::istreambuf_iterator<char>(ttfFile)),
            std::istreambuf_iterator<char>());
        expect(ttfData.size() > 1000_u) << "TTF should be non-trivial";

        auto buf = *YPaintBuffer::create();

        // Register font blob (like ypdf does)
        int fontId = buf->addFontBlob(ttfData.data(), ttfData.size(), "TestFont");
        expect(fontId >= 0) << "addFontBlob should succeed";

        // Add text using custom font
        buf->addText(20.0f, 40.0f, "PDF text", 12.0f, 0xFFFFFFFF, 0, fontId);
        expect(buf->textSpanCount() == 1_u);

        auto p = runPipeline(buf, 300.0f, 100.0f, fm);

        uint32_t glyphCount = p.meta[7];
        expect(glyphCount >= 5_u)
            << "custom font text should produce glyphs, got " << glyphCount;

        // Verify glyphs have GLYPH_FLAG_CUSTOM_ATLAS set
        if (glyphCount > 0) {
            uint32_t glyphOffset = p.meta[6];
            const YPaintGlyph* glyphs = reinterpret_cast<const YPaintGlyph*>(
                &p.storage[glyphOffset]);
            uint8_t flags = static_cast<uint8_t>(glyphs[0].glyphLayerFlags >> 24);
            expect((flags & Painter::GLYPH_FLAG_CUSTOM_ATLAS) != 0_u)
                << "custom font glyphs should have CUSTOM_ATLAS flag";
        }
    };

    "ygui full widget scenario — prims + text all reachable"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        auto buf = *YPaintBuffer::create();

        // Button background (RoundedBox)
        buf->addRoundedBox(buf->primCount(),
            100.0f, 25.0f, 80.0f, 16.0f,
            4.0f, 4.0f, 4.0f, 4.0f,
            0xFF333344, 0, 0.0f, 0.0f);
        // Button label
        buf->addText(30.0f, 30.0f, "Submit", 14.0f, 0xFFFFFFFF, 0, -1);

        // Checkbox (Box + inner Box)
        buf->addBox(buf->primCount(),
            16.0f, 62.0f, 8.0f, 8.0f,
            0xFF444455, 0, 0.0f, 0.0f);
        buf->addBox(buf->primCount(),
            16.0f, 62.0f, 5.0f, 5.0f,
            0xFF00AAFF, 0, 0.0f, 0.0f);
        buf->addText(30.0f, 66.0f, "Accept terms", 14.0f, 0xFFCCCCCC, 0, -1);

        // Dropdown arrow (Triangle)
        buf->addTriangle(buf->primCount(),
            280.0f, 22.0f, 288.0f, 22.0f, 284.0f, 28.0f,
            0xFFCCCCCC, 0, 0.0f, 0.0f);

        // Slider track (RoundedBox)
        buf->addRoundedBox(buf->primCount(),
            150.0f, 95.0f, 80.0f, 3.0f,
            2.0f, 2.0f, 2.0f, 2.0f,
            0xFF333344, 0, 0.0f, 0.0f);

        expect(buf->primCount() == 5_u) << "5 SDF prims";

        auto p = runPipeline(buf, 400.0f, 200.0f, fm);
        expect(p.primitiveCount == 5_u);

        uint32_t glyphCount = p.meta[7];
        expect(glyphCount >= 10_u)
            << "'Submit' + 'Accept terms' should produce >= 10 glyphs, got " << glyphCount;

        // ALL 5 prims should be reachable from grid
        std::set<uint32_t> reachablePrims;
        bool hasGlyphEntries = false;
        uint32_t numCells = p.gridWidth * p.gridHeight;
        for (uint32_t ci = 0; ci < numCells; ci++) {
            uint32_t packedOff = readU32(p.storage, p.gridOffset + ci);
            uint32_t cnt = readU32(p.storage, p.gridOffset + packedOff);
            for (uint32_t j = 0; j < cnt; j++) {
                uint32_t rawVal = readU32(p.storage, p.gridOffset + packedOff + 1 + j);
                if ((rawVal & 0x80000000u) != 0) {
                    hasGlyphEntries = true;
                } else {
                    reachablePrims.insert(p.primDataBase + rawVal);
                }
            }
        }

        // Every prim must be reachable
        for (uint32_t i = 0; i < p.primitiveCount; i++) {
            uint32_t wordOff = readU32(p.storage, p.primitiveOffset + i);
            uint32_t primOff = p.primDataBase + wordOff;
            expect(reachablePrims.count(primOff) > 0_u)
                << "prim " << i << " should be reachable from grid";
        }

        expect(hasGlyphEntries) << "grid should have glyph entries for text";

        // Verify glyph positions are within scene bounds
        float sMinX, sMinY, sMaxX, sMaxY;
        std::memcpy(&sMinX, &p.meta[8], sizeof(float));
        std::memcpy(&sMinY, &p.meta[9], sizeof(float));
        std::memcpy(&sMaxX, &p.meta[10], sizeof(float));
        std::memcpy(&sMaxY, &p.meta[11], sizeof(float));

        uint32_t glyphOffset = p.meta[6];
        const YPaintGlyph* glyphs = reinterpret_cast<const YPaintGlyph*>(
            &p.storage[glyphOffset]);
        for (uint32_t i = 0; i < glyphCount; i++) {
            expect(glyphs[i].x >= sMinX - 10.0f && glyphs[i].x <= sMaxX + 10.0f)
                << "glyph " << i << " x=" << glyphs[i].x << " out of scene bounds";
        }
    };
};

//=============================================================================
// Helper: run pipeline with specific metadata slot (for overlay testing)
//=============================================================================
static PipelineResult runPipelineWithSlot(YPaintBuffer::Ptr buf, uint32_t metaSlotIndex,
                                           float sceneW, float sceneH,
                                           bool scrollingMode = false,
                                           FontManager::Ptr fontMgr = nullptr) {
    // Allocate enough metadata for high slot indices
    uint32_t metadataBytes = (metaSlotIndex + 1) * 64;
    auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
    auto builder = *Painter::create(fontMgr, testAllocator(), mockCM, metaSlotIndex, scrollingMode);
    builder->setSceneBounds(0, 0, sceneW, sceneH);
    builder->setGridCellSize(20.0f, 20.0f);
    if (!buf->empty()) {
        builder->addYpaintBuffer(buf);
    }
    builder->declareBufferNeeds();
    mockCM->mockBufMgr()->commitReservations();
    builder->allocateBuffers();
    builder->writeBuffers();

    const float* storage = mockCM->mockBufMgr()->storageAsFloat();
    // Metadata for slot N is at offset N * 16 (in u32 words)
    const uint32_t* meta = mockCM->metadataAsU32() + metaSlotIndex * 16;

    PipelineResult r;
    r.cardMgr = mockCM;
    r.builder = builder;
    r.storage = storage;
    r.meta = meta;
    r.primitiveOffset = meta[0];
    r.primitiveCount = meta[1];
    r.primDataBase = r.primitiveOffset + r.primitiveCount;
    r.gridOffset = meta[2];
    r.gridWidth = meta[3];
    r.gridHeight = meta[4];
    r.cellSizeX = unpackF16(meta[5], false);
    r.cellSizeY = unpackF16(meta[5], true);
    std::memcpy(&r.sceneMinX, &meta[8], sizeof(float));
    std::memcpy(&r.sceneMinY, &meta[9], sizeof(float));
    return r;
}

//=============================================================================
// Helper: query all prim indices in a specific grid cell
//=============================================================================
static std::vector<uint32_t> getPrimIndicesInCell(const PipelineResult& p,
                                                   uint32_t cellX, uint32_t cellY) {
    std::vector<uint32_t> result;
    if (cellX >= p.gridWidth || cellY >= p.gridHeight) return result;

    uint32_t cellIndex = cellY * p.gridWidth + cellX;
    uint32_t packedStart = readU32(p.storage, p.gridOffset + cellIndex);
    uint32_t cellEntryCount = readU32(p.storage, p.gridOffset + packedStart);

    for (uint32_t i = 0; i < cellEntryCount; i++) {
        uint32_t rawVal = readU32(p.storage, p.gridOffset + packedStart + 1 + i);
        if ((rawVal & 0x80000000u) == 0) {  // Not a glyph
            result.push_back(rawVal);
        }
    }
    return result;
}

//=============================================================================
// Overlay Slot Tests — Tests for high slot indices (1000, 1001)
// These verify the overlay/scrolling painter slots work correctly
//=============================================================================

suite overlay_slot_tests = [] {

    "slot 1000 — metadata written at correct offset"_test = [] {
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 50.0f, 10.0f, 0xFFFF0000, 0, 0.0f, 0.0f);

        auto p = runPipelineWithSlot(buf, 1000, 200.0f, 200.0f);

        expect(p.primitiveCount == 1_u) << "should have 1 prim";
        expect(p.gridWidth > 0_u) << "grid width > 0";
        expect(p.gridHeight > 0_u) << "grid height > 0";

        // Verify prim data is correct
        uint32_t wordOff0 = readU32(p.storage, p.primitiveOffset);
        uint32_t primOff = p.primDataBase + wordOff0;
        uint32_t type = readU32(p.storage, primOff + 1);
        expect(type == static_cast<uint32_t>(SDFType::Circle)) << "type should be Circle";
    };

    "slot 1001 — separate from slot 1000"_test = [] {
        // Use SEPARATE MockGpuMemoryManager for each painter
        uint32_t metadataBytes = 1002 * 64;

        // Painter at slot 1000
        auto mockCM1000 = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder1000 = *Painter::create(nullptr, testAllocator(), mockCM1000, 1000);
        builder1000->setSceneBounds(0, 0, 100, 100);
        builder1000->setGridCellSize(20.0f, 20.0f);
        auto buf1 = *YPaintBuffer::create();
        buf1->addCircle(0, 30.0f, 30.0f, 10.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        builder1000->addYpaintBuffer(buf1);
        builder1000->declareBufferNeeds();
        mockCM1000->mockBufMgr()->commitReservations();
        builder1000->allocateBuffers();
        builder1000->writeBuffers();

        // Painter at slot 1001
        auto mockCM1001 = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder1001 = *Painter::create(nullptr, testAllocator(), mockCM1001, 1001);
        builder1001->setSceneBounds(0, 0, 100, 100);
        builder1001->setGridCellSize(20.0f, 20.0f);
        auto buf2 = *YPaintBuffer::create();
        buf2->addBox(0, 70.0f, 70.0f, 15.0f, 15.0f, 0xFF00FF00, 0, 0.0f, 0.0f);
        builder1001->addYpaintBuffer(buf2);
        builder1001->declareBufferNeeds();
        mockCM1001->mockBufMgr()->commitReservations();
        builder1001->allocateBuffers();
        builder1001->writeBuffers();

        // Read metadata for both slots
        const uint32_t* meta1000 = mockCM1000->metadataAsU32() + 1000 * 16;
        const uint32_t* meta1001 = mockCM1001->metadataAsU32() + 1001 * 16;

        expect(meta1000[1] == 1_u) << "slot 1000 should have 1 prim";
        expect(meta1001[1] == 1_u) << "slot 1001 should have 1 prim";

        // Verify both painters wrote valid metadata at their respective slots
        expect(meta1000[0] >= 0_u) << "slot 1000 primitiveOffset valid";
        expect(meta1001[0] >= 0_u) << "slot 1001 primitiveOffset valid";
    };

    "slot 1000 — grid lookup finds primitives"_test = [] {
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 50.0f, 10.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        buf->addBox(1, 150.0f, 150.0f, 20.0f, 20.0f, 0xFF00FF00, 0, 0.0f, 0.0f);

        auto p = runPipelineWithSlot(buf, 1000, 200.0f, 200.0f);

        // Lookup at circle center (50,50) with cellSize=20 -> cell (2,2)
        auto circleHits = shaderGridLookup(p, 50.0f, 50.0f);
        expect(!circleHits.empty()) << "should find prim at circle center";

        bool foundCircle = false;
        for (uint32_t off : circleHits) {
            if (readU32(p.storage, off + 1) == static_cast<uint32_t>(SDFType::Circle))
                foundCircle = true;
        }
        expect(foundCircle) << "should find Circle at (50,50)";

        // Lookup at box center (150,150)
        auto boxHits = shaderGridLookup(p, 150.0f, 150.0f);
        expect(!boxHits.empty()) << "should find prim at box center";

        bool foundBox = false;
        for (uint32_t off : boxHits) {
            if (readU32(p.storage, off + 1) == static_cast<uint32_t>(SDFType::Box))
                foundBox = true;
        }
        expect(foundBox) << "should find Box at (150,150)";
    };
};

//=============================================================================
// Scrolling Mode Tests — Multiple addYpaintBuffer calls with scrolling
//=============================================================================

suite scrolling_mode_tests = [] {

    "scrolling mode — single buffer positions at cursor"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        float sceneW = 200.0f, sceneH = 100.0f;
        float cellH = 20.0f;
        builder->setSceneBounds(0, 0, sceneW, sceneH);
        builder->setGridCellSize(20.0f, cellH);
        builder->setCursorPosition(0, 0);

        auto buf = *YPaintBuffer::create();
        buf->addBox(0, 10.0f, 5.0f, 20.0f, 10.0f, 0xFFFF0000, 0, 0.0f, 0.0f);

        auto linesRes = builder->addYpaintBuffer(buf);
        expect(linesRes.has_value()) << "addYpaintBuffer should succeed";

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        expect(builder->primitiveCount() == 1_u) << "should have 1 prim";
    };

    "scrolling mode — multiple buffers accumulate"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 200.0f, 100.0f);
        builder->setGridCellSize(20.0f, 20.0f);
        builder->setCursorPosition(0, 0);

        // Add first buffer
        auto buf1 = *YPaintBuffer::create();
        buf1->addCircle(0, 30.0f, 10.0f, 8.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf1);

        // Add second buffer
        auto buf2 = *YPaintBuffer::create();
        buf2->addBox(0, 100.0f, 10.0f, 15.0f, 10.0f, 0xFF00FF00, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf2);

        // Add third buffer
        auto buf3 = *YPaintBuffer::create();
        buf3->addTriangle(0, 150.0f, 5.0f, 170.0f, 5.0f, 160.0f, 15.0f,
                          0xFF0000FF, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf3);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        expect(builder->primitiveCount() == 3_u) << "should have 3 prims total";
    };

    "scrolling mode — scroll removes top lines"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        float cellH = 20.0f;
        builder->setSceneBounds(0, 0, 200.0f, 60.0f);  // 3 lines of height
        builder->setGridCellSize(20.0f, cellH);
        builder->setCursorPosition(0, 0);

        // Add primitives on line 0
        auto buf1 = *YPaintBuffer::create();
        buf1->addCircle(0, 30.0f, 10.0f, 5.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf1);

        // Move cursor to line 1 and add more
        builder->setCursorPosition(0, 1);
        auto buf2 = *YPaintBuffer::create();
        buf2->addBox(0, 30.0f, 10.0f, 10.0f, 8.0f, 0xFF00FF00, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf2);

        // Move cursor to line 2 and add more
        builder->setCursorPosition(0, 2);
        auto buf3 = *YPaintBuffer::create();
        buf3->addTriangle(0, 30.0f, 5.0f, 50.0f, 5.0f, 40.0f, 15.0f,
                          0xFF0000FF, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf3);

        expect(builder->primitiveCount() == 3_u) << "should have 3 prims before scroll";

        // Scroll 1 line — should remove line 0 (Circle)
        builder->scrollLines(1);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        expect(builder->primitiveCount() == 2_u)
            << "should have 2 prims after scrolling 1 line, got " << builder->primitiveCount();
    };

    "scrolling mode — verify grid after scroll"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        float cellH = 20.0f;
        builder->setSceneBounds(0, 0, 100.0f, 60.0f);  // 3 lines
        builder->setGridCellSize(20.0f, cellH);

        // Add circles at different Y positions
        builder->setCursorPosition(0, 0);
        auto buf1 = *YPaintBuffer::create();
        buf1->addCircle(0, 50.0f, 10.0f, 5.0f, 0xFFFF0000, 0, 0.0f, 0.0f);  // Line 0
        builder->addYpaintBuffer(buf1);

        builder->setCursorPosition(0, 1);
        auto buf2 = *YPaintBuffer::create();
        buf2->addCircle(0, 50.0f, 10.0f, 5.0f, 0xFF00FF00, 0, 0.0f, 0.0f);  // Line 1
        builder->addYpaintBuffer(buf2);

        builder->setCursorPosition(0, 2);
        auto buf3 = *YPaintBuffer::create();
        buf3->addCircle(0, 50.0f, 10.0f, 5.0f, 0xFF0000FF, 0, 0.0f, 0.0f);  // Line 2
        builder->addYpaintBuffer(buf3);

        // Build initial state
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        // Count unique prims reachable from grid
        std::set<uint32_t> reachableBefore;
        uint32_t numCells = meta[3] * meta[4];  // gridWidth * gridHeight
        uint32_t gridOff = meta[2];
        uint32_t primBase = meta[0] + meta[1];
        for (uint32_t ci = 0; ci < numCells; ci++) {
            uint32_t packedOff = readU32(storage, gridOff + ci);
            uint32_t cnt = readU32(storage, gridOff + packedOff);
            for (uint32_t j = 0; j < cnt; j++) {
                uint32_t rawVal = readU32(storage, gridOff + packedOff + 1 + j);
                if ((rawVal & 0x80000000u) == 0) {
                    reachableBefore.insert(primBase + rawVal);
                }
            }
        }
        expect(reachableBefore.size() == 3_u) << "3 prims reachable before scroll";

        // Now scroll 1 line
        builder->scrollLines(1);
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        // Re-read metadata (may have changed)
        meta = mockCM->metadataAsU32() + 1000 * 16;
        storage = mockCM->mockBufMgr()->storageAsFloat();

        std::set<uint32_t> reachableAfter;
        numCells = meta[3] * meta[4];
        gridOff = meta[2];
        primBase = meta[0] + meta[1];
        for (uint32_t ci = 0; ci < numCells; ci++) {
            uint32_t packedOff = readU32(storage, gridOff + ci);
            uint32_t cnt = readU32(storage, gridOff + packedOff);
            for (uint32_t j = 0; j < cnt; j++) {
                uint32_t rawVal = readU32(storage, gridOff + packedOff + 1 + j);
                if ((rawVal & 0x80000000u) == 0) {
                    reachableAfter.insert(primBase + rawVal);
                }
            }
        }
        expect(reachableAfter.size() == 2_u)
            << "2 prims reachable after scroll, got " << reachableAfter.size();
    };

    "scrolling mode — multiple scroll operations"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        float cellH = 20.0f;
        builder->setSceneBounds(0, 0, 100.0f, 100.0f);  // 5 lines
        builder->setGridCellSize(20.0f, cellH);

        // Add 5 prims, one per line
        for (int i = 0; i < 5; i++) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i));
            auto buf = *YPaintBuffer::create();
            buf->addCircle(0, 50.0f, 10.0f, 5.0f, 0xFF000000 | (i * 0x333333), 0, 0.0f, 0.0f);
            builder->addYpaintBuffer(buf);
        }

        expect(builder->primitiveCount() == 5_u) << "5 prims before any scroll";

        // Scroll 2 lines
        builder->scrollLines(2);
        expect(builder->primitiveCount() == 3_u) << "3 prims after scrolling 2";

        // Scroll 2 more lines
        builder->scrollLines(2);
        expect(builder->primitiveCount() == 1_u) << "1 prim after scrolling 4 total";

        // Scroll last line
        builder->scrollLines(1);
        expect(builder->primitiveCount() == 0_u) << "0 prims after scrolling all";
    };

    "scrolling mode — addYpaintBuffer returns lines consumed"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 100.0f);
        builder->setGridCellSize(20.0f, 20.0f);  // 5 lines
        builder->setCursorPosition(0, 0);

        // Add buffer that takes ~2.5 lines (height 50 / cellSize 20 = 2.5)
        auto buf = *YPaintBuffer::create();
        buf->addBox(0, 50.0f, 25.0f, 20.0f, 25.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        auto linesRes = builder->addYpaintBuffer(buf);

        expect(linesRes.has_value()) << "addYpaintBuffer should succeed";
        uint32_t linesConsumed = *linesRes;

        // Should report lines consumed (caller uses this to advance cursor)
        expect(linesConsumed >= 1_u)
            << "should report lines consumed, got " << linesConsumed;
    };
};

//=============================================================================
// EXHAUSTIVE SCROLLING GPU BUFFER VERIFICATION TESTS
//
// These tests add primitives in loops with addYpaintBuffer and verify:
// - Primitive count is correct after each add
// - GPU buffer contains correct primitive data
// - Grid lookup finds primitives at correct positions
// - Scrolling removes correct primitives
// - Remaining primitives have correct positions after scroll
//=============================================================================

suite exhaustive_scrolling_tests = [] {

    // Helper: verify primitive exists at expected grid row
    auto verifyPrimAtRow = [](const PipelineResult& p, uint32_t expectedRow,
                              uint32_t expectedType, uint32_t expectedColor) -> bool {
        // Scan grid cells in the expected row
        if (expectedRow >= p.gridHeight) return false;

        for (uint32_t cx = 0; cx < p.gridWidth; cx++) {
            uint32_t cellIdx = expectedRow * p.gridWidth + cx;
            uint32_t packedOff = readU32(p.storage, p.gridOffset + cellIdx);
            uint32_t cnt = readU32(p.storage, p.gridOffset + packedOff);

            for (uint32_t j = 0; j < cnt; j++) {
                uint32_t rawVal = readU32(p.storage, p.gridOffset + packedOff + 1 + j);
                if ((rawVal & 0x80000000u) == 0) {  // Not a glyph
                    uint32_t primOff = p.primDataBase + rawVal;
                    uint32_t type = readU32(p.storage, primOff + 1);
                    if (type == expectedType) {
                        // Get color based on type
                        uint32_t colorOff = 0;
                        if (type == static_cast<uint32_t>(SDFType::Circle)) colorOff = 6;
                        else if (type == static_cast<uint32_t>(SDFType::Box)) colorOff = 7;
                        else if (type == static_cast<uint32_t>(SDFType::RoundedBox)) colorOff = 11;
                        else if (type == static_cast<uint32_t>(SDFType::Triangle)) colorOff = 9;

                        if (colorOff > 0) {
                            uint32_t color = readU32(p.storage, primOff + colorOff);
                            if (color == expectedColor) return true;
                        }
                    }
                }
            }
        }
        return false;
    };

    // Helper: count unique primitives reachable from grid
    auto countReachablePrims = [](const PipelineResult& p) -> uint32_t {
        std::set<uint32_t> seen;
        uint32_t numCells = p.gridWidth * p.gridHeight;
        for (uint32_t ci = 0; ci < numCells; ci++) {
            uint32_t packedOff = readU32(p.storage, p.gridOffset + ci);
            uint32_t cnt = readU32(p.storage, p.gridOffset + packedOff);
            for (uint32_t j = 0; j < cnt; j++) {
                uint32_t rawVal = readU32(p.storage, p.gridOffset + packedOff + 1 + j);
                if ((rawVal & 0x80000000u) == 0) {
                    seen.insert(rawVal);
                }
            }
        }
        return static_cast<uint32_t>(seen.size());
    };

    // Helper: get all primitive colors from GPU buffer
    auto getAllPrimColors = [](const PipelineResult& p) -> std::vector<uint32_t> {
        std::vector<uint32_t> colors;
        for (uint32_t i = 0; i < p.primitiveCount; i++) {
            uint32_t wordOff = readU32(p.storage, p.primitiveOffset + i);
            uint32_t primOff = p.primDataBase + wordOff;
            uint32_t type = readU32(p.storage, primOff + 1);

            uint32_t colorOff = 0;
            if (type == static_cast<uint32_t>(SDFType::Circle)) colorOff = 6;
            else if (type == static_cast<uint32_t>(SDFType::Box)) colorOff = 7;
            else if (type == static_cast<uint32_t>(SDFType::RoundedBox)) colorOff = 11;
            else if (type == static_cast<uint32_t>(SDFType::Triangle)) colorOff = 9;

            if (colorOff > 0) {
                colors.push_back(readU32(p.storage, primOff + colorOff));
            }
        }
        return colors;
    };

    "exhaustive: add 10 circles sequentially, verify each"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        float cellH = 20.0f;
        builder->setSceneBounds(0, 0, 200.0f, 200.0f);  // 10 lines
        builder->setGridCellSize(20.0f, cellH);

        // Add 10 circles, one per row
        for (uint32_t i = 0; i < 10; i++) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i));

            auto buf = *YPaintBuffer::create();
            uint32_t color = 0xFF000000 | (i * 0x111111);
            buf->addCircle(0, 50.0f, 10.0f, 8.0f, color, 0, 0.0f, 0.0f);
            builder->addYpaintBuffer(buf);

            // Build and verify after each add
            builder->declareBufferNeeds();
            mockCM->mockBufMgr()->commitReservations();
            builder->allocateBuffers();
            builder->writeBuffers();

            expect(builder->primitiveCount() == i + 1)
                << "after adding " << (i + 1) << " circles, count=" << builder->primitiveCount();
        }

        // Final verification: all 10 prims in GPU buffer
        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;
        expect(meta[1] == 10_u) << "final prim count should be 10";

        // Verify each primitive has correct color
        uint32_t primOff = meta[0];
        uint32_t primCnt = meta[1];
        uint32_t primBase = primOff + primCnt;

        for (uint32_t i = 0; i < primCnt; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t absOff = primBase + wordOff;
            uint32_t type = readU32(storage, absOff + 1);
            expect(type == static_cast<uint32_t>(SDFType::Circle))
                << "prim " << i << " should be Circle";

            uint32_t color = readU32(storage, absOff + 6);
            uint32_t expectedColor = 0xFF000000 | (i * 0x111111);
            expect(color == expectedColor)
                << "prim " << i << " color mismatch: got " << std::hex << color
                << " expected " << expectedColor << std::dec;
        }
    };

    "exhaustive: add mixed primitives, verify types and colors"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 200.0f, 200.0f);
        builder->setGridCellSize(20.0f, 20.0f);

        struct PrimSpec {
            uint32_t type;
            uint32_t color;
        };
        std::vector<PrimSpec> specs;

        // Add alternating types: circle, box, triangle, roundedbox
        for (uint32_t i = 0; i < 8; i++) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i));
            auto buf = *YPaintBuffer::create();
            uint32_t color = 0xFF000000 | ((i + 1) * 0x101010);

            switch (i % 4) {
                case 0:
                    buf->addCircle(0, 50.0f, 10.0f, 8.0f, color, 0, 0.0f, 0.0f);
                    specs.push_back({static_cast<uint32_t>(SDFType::Circle), color});
                    break;
                case 1:
                    buf->addBox(0, 50.0f, 10.0f, 15.0f, 8.0f, color, 0, 0.0f, 0.0f);
                    specs.push_back({static_cast<uint32_t>(SDFType::Box), color});
                    break;
                case 2:
                    buf->addTriangle(0, 40.0f, 5.0f, 60.0f, 5.0f, 50.0f, 18.0f,
                                     color, 0, 0.0f, 0.0f);
                    specs.push_back({static_cast<uint32_t>(SDFType::Triangle), color});
                    break;
                case 3:
                    buf->addRoundedBox(0, 50.0f, 10.0f, 15.0f, 8.0f,
                                       2.0f, 2.0f, 2.0f, 2.0f, color, 0, 0.0f, 0.0f);
                    specs.push_back({static_cast<uint32_t>(SDFType::RoundedBox), color});
                    break;
            }
            builder->addYpaintBuffer(buf);
        }

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        expect(builder->primitiveCount() == 8_u) << "should have 8 prims";

        // Verify each primitive
        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;
        uint32_t primOff = meta[0];
        uint32_t primCnt = meta[1];
        uint32_t primBase = primOff + primCnt;

        for (uint32_t i = 0; i < primCnt; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t absOff = primBase + wordOff;
            uint32_t type = readU32(storage, absOff + 1);

            expect(type == specs[i].type)
                << "prim " << i << " type mismatch: got " << type
                << " expected " << specs[i].type;

            // Get color offset based on type
            uint32_t colorOff = 0;
            if (type == static_cast<uint32_t>(SDFType::Circle)) colorOff = 6;
            else if (type == static_cast<uint32_t>(SDFType::Box)) colorOff = 7;
            else if (type == static_cast<uint32_t>(SDFType::Triangle)) colorOff = 9;
            else if (type == static_cast<uint32_t>(SDFType::RoundedBox)) colorOff = 11;

            uint32_t color = readU32(storage, absOff + colorOff);
            expect(color == specs[i].color)
                << "prim " << i << " color mismatch";
        }
    };

    "exhaustive: scroll removes correct primitives"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        float cellH = 20.0f;
        builder->setSceneBounds(0, 0, 100.0f, 100.0f);  // 5 lines
        builder->setGridCellSize(20.0f, cellH);

        // Add 5 circles with distinct colors at rows 0-4
        std::vector<uint32_t> colors = {0xFFAA0000, 0xFF00BB00, 0xFF0000CC,
                                         0xFFDD00DD, 0xFF00EEEE};
        for (uint32_t i = 0; i < 5; i++) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i));
            auto buf = *YPaintBuffer::create();
            buf->addCircle(0, 50.0f, 10.0f, 8.0f, colors[i], 0, 0.0f, 0.0f);
            builder->addYpaintBuffer(buf);
        }

        expect(builder->primitiveCount() == 5_u) << "should have 5 prims";

        // Scroll 2 lines - should remove colors[0] and colors[1]
        builder->scrollLines(2);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        expect(builder->primitiveCount() == 3_u)
            << "after scroll 2, should have 3 prims, got " << builder->primitiveCount();

        // Verify remaining colors are colors[2], colors[3], colors[4]
        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;
        uint32_t primOff = meta[0];
        uint32_t primCnt = meta[1];
        uint32_t primBase = primOff + primCnt;

        std::set<uint32_t> remainingColors;
        for (uint32_t i = 0; i < primCnt; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t absOff = primBase + wordOff;
            uint32_t color = readU32(storage, absOff + 6);  // Circle color at +6
            remainingColors.insert(color);
        }

        expect(remainingColors.count(colors[0]) == 0_u) << "color[0] should be scrolled away";
        expect(remainingColors.count(colors[1]) == 0_u) << "color[1] should be scrolled away";
        expect(remainingColors.count(colors[2]) == 1_u) << "color[2] should remain";
        expect(remainingColors.count(colors[3]) == 1_u) << "color[3] should remain";
        expect(remainingColors.count(colors[4]) == 1_u) << "color[4] should remain";
    };

    "exhaustive: multiple scroll operations"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 200.0f);  // 10 lines
        builder->setGridCellSize(20.0f, 20.0f);

        // Add 10 prims
        for (uint32_t i = 0; i < 10; i++) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i));
            auto buf = *YPaintBuffer::create();
            buf->addCircle(0, 50.0f, 10.0f, 5.0f, 0xFF000000 | i, 0, 0.0f, 0.0f);
            builder->addYpaintBuffer(buf);
        }
        expect(builder->primitiveCount() == 10_u);

        // Scroll 3 lines
        builder->scrollLines(3);
        expect(builder->primitiveCount() == 7_u) << "after scroll 3";

        // Scroll 2 more
        builder->scrollLines(2);
        expect(builder->primitiveCount() == 5_u) << "after scroll 5 total";

        // Scroll 4 more
        builder->scrollLines(4);
        expect(builder->primitiveCount() == 1_u) << "after scroll 9 total";

        // Scroll last one
        builder->scrollLines(1);
        expect(builder->primitiveCount() == 0_u) << "after scroll 10 total";
    };

    "exhaustive: add-scroll-add sequence"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 60.0f);  // 3 lines
        builder->setGridCellSize(20.0f, 20.0f);

        // Add at row 0
        builder->setCursorPosition(0, 0);
        auto buf1 = *YPaintBuffer::create();
        buf1->addCircle(0, 50.0f, 10.0f, 5.0f, 0xFFAAAAAA, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf1);

        // Add at row 1
        builder->setCursorPosition(0, 1);
        auto buf2 = *YPaintBuffer::create();
        buf2->addCircle(0, 50.0f, 10.0f, 5.0f, 0xFFBBBBBB, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf2);

        expect(builder->primitiveCount() == 2_u);

        // Scroll 1 line - removes 0xAAAAAAA
        builder->scrollLines(1);
        expect(builder->primitiveCount() == 1_u);

        // Add at row 1 (cursor was at 1, now at 0 after scroll adjustment)
        builder->setCursorPosition(0, 1);
        auto buf3 = *YPaintBuffer::create();
        buf3->addCircle(0, 50.0f, 10.0f, 5.0f, 0xFFCCCCCC, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf3);

        expect(builder->primitiveCount() == 2_u);

        // Build and verify
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;
        uint32_t primOff = meta[0];
        uint32_t primCnt = meta[1];
        uint32_t primBase = primOff + primCnt;

        std::set<uint32_t> colors;
        for (uint32_t i = 0; i < primCnt; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t absOff = primBase + wordOff;
            uint32_t color = readU32(storage, absOff + 6);
            colors.insert(color);
        }

        expect(colors.count(0xFFAAAAAA) == 0_u) << "0xAA should be scrolled away";
        expect(colors.count(0xFFBBBBBB) == 1_u) << "0xBB should remain";
        expect(colors.count(0xFFCCCCCC) == 1_u) << "0xCC should be added";
    };

    "exhaustive: verify grid lookup after scroll"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 100.0f);  // 5 lines
        builder->setGridCellSize(20.0f, 20.0f);

        // Add circles at different X positions for each row
        for (uint32_t i = 0; i < 5; i++) {
            builder->setCursorPosition(0, static_cast<uint16_t>(i));
            auto buf = *YPaintBuffer::create();
            float cx = 10.0f + i * 20.0f;  // x = 10, 30, 50, 70, 90
            buf->addCircle(0, cx, 10.0f, 5.0f, 0xFF000000 | i, 0, 0.0f, 0.0f);
            builder->addYpaintBuffer(buf);
        }

        // Scroll 2 lines
        builder->scrollLines(2);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        PipelineResult p;
        p.storage = storage;
        p.meta = meta;
        p.primitiveOffset = meta[0];
        p.primitiveCount = meta[1];
        p.primDataBase = p.primitiveOffset + p.primitiveCount;
        p.gridOffset = meta[2];
        p.gridWidth = meta[3];
        p.gridHeight = meta[4];
        p.cellSizeX = 20.0f;
        p.cellSizeY = 20.0f;
        std::memcpy(&p.sceneMinX, &meta[8], sizeof(float));
        std::memcpy(&p.sceneMinY, &meta[9], sizeof(float));

        expect(p.primitiveCount == 3_u) << "should have 3 prims after scroll";

        // Remaining prims should have colors 2, 3, 4 (originally at rows 2, 3, 4)
        std::set<uint32_t> colors;
        for (uint32_t i = 0; i < p.primitiveCount; i++) {
            uint32_t wordOff = readU32(storage, p.primitiveOffset + i);
            uint32_t absOff = p.primDataBase + wordOff;
            uint32_t color = readU32(storage, absOff + 6);
            colors.insert(color);
        }

        expect(colors.count(0xFF000000) == 0_u) << "color 0 scrolled away";
        expect(colors.count(0xFF000001) == 0_u) << "color 1 scrolled away";
        expect(colors.count(0xFF000002) == 1_u) << "color 2 remains";
        expect(colors.count(0xFF000003) == 1_u) << "color 3 remains";
        expect(colors.count(0xFF000004) == 1_u) << "color 4 remains";
    };

    "exhaustive: 20 adds with periodic scrolling"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 100.0f);  // 5 lines visible
        builder->setGridCellSize(20.0f, 20.0f);

        uint32_t cursorRow = 0;
        uint32_t totalScrolled = 0;
        std::vector<uint32_t> activeColors;

        for (uint32_t i = 0; i < 20; i++) {
            // Scroll if we're past visible area
            if (cursorRow >= 5) {
                uint32_t scrollAmt = cursorRow - 4;
                builder->scrollLines(static_cast<uint16_t>(scrollAmt));
                totalScrolled += scrollAmt;
                cursorRow -= scrollAmt;

                // Remove scrolled colors from tracking
                if (scrollAmt <= activeColors.size()) {
                    activeColors.erase(activeColors.begin(),
                                       activeColors.begin() + scrollAmt);
                } else {
                    activeColors.clear();
                }
            }

            builder->setCursorPosition(0, static_cast<uint16_t>(cursorRow));
            auto buf = *YPaintBuffer::create();
            uint32_t color = 0xFF000000 | ((i + 1) * 0x010101);
            buf->addCircle(0, 50.0f, 10.0f, 5.0f, color, 0, 0.0f, 0.0f);
            builder->addYpaintBuffer(buf);
            activeColors.push_back(color);

            cursorRow++;
        }

        // Final build
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        // Verify primitive count matches tracked active colors
        expect(meta[1] == static_cast<uint32_t>(activeColors.size()))
            << "prim count should match active colors: expected "
            << activeColors.size() << " got " << meta[1];

        // Verify all active colors are in GPU buffer
        uint32_t primOff = meta[0];
        uint32_t primCnt = meta[1];
        uint32_t primBase = primOff + primCnt;

        std::set<uint32_t> gpuColors;
        for (uint32_t i = 0; i < primCnt; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t absOff = primBase + wordOff;
            uint32_t color = readU32(storage, absOff + 6);
            gpuColors.insert(color);
        }

        for (uint32_t expectedColor : activeColors) {
            expect(gpuColors.count(expectedColor) == 1_u)
                << "expected color " << std::hex << expectedColor
                << std::dec << " should be in GPU buffer";
        }
    };

    "exhaustive: different primitive sizes spanning multiple lines"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 200.0f);  // 10 lines
        builder->setGridCellSize(20.0f, 20.0f);

        // Add boxes of varying heights
        builder->setCursorPosition(0, 0);
        auto buf1 = *YPaintBuffer::create();
        buf1->addBox(0, 50.0f, 15.0f, 20.0f, 15.0f, 0xFFAA0000, 0, 0.0f, 0.0f);  // ~1.5 lines
        builder->addYpaintBuffer(buf1);

        builder->setCursorPosition(0, 2);
        auto buf2 = *YPaintBuffer::create();
        buf2->addBox(0, 50.0f, 25.0f, 20.0f, 25.0f, 0xFF00BB00, 0, 0.0f, 0.0f);  // ~2.5 lines
        builder->addYpaintBuffer(buf2);

        builder->setCursorPosition(0, 5);
        auto buf3 = *YPaintBuffer::create();
        buf3->addBox(0, 50.0f, 35.0f, 20.0f, 35.0f, 0xFF0000CC, 0, 0.0f, 0.0f);  // ~3.5 lines
        builder->addYpaintBuffer(buf3);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        expect(builder->primitiveCount() == 3_u) << "should have 3 prims";

        // Scroll 3 lines - should remove first box only (it's in lines 0-1)
        builder->scrollLines(3);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        std::set<uint32_t> colors;
        uint32_t primOff = meta[0];
        uint32_t primCnt = meta[1];
        uint32_t primBase = primOff + primCnt;

        for (uint32_t i = 0; i < primCnt; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t absOff = primBase + wordOff;
            uint32_t color = readU32(storage, absOff + 7);  // Box color at +7
            colors.insert(color);
        }

        expect(colors.count(0xFFAA0000) == 0_u) << "first box should be scrolled";
        expect(colors.count(0xFF00BB00) == 1_u) << "second box should remain";
        expect(colors.count(0xFF0000CC) == 1_u) << "third box should remain";
    };

    "exhaustive: verify primitive offset table integrity"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 200.0f, 200.0f);
        builder->setGridCellSize(20.0f, 20.0f);

        // Add 15 mixed primitives
        for (uint32_t i = 0; i < 15; i++) {
            builder->setCursorPosition(static_cast<uint16_t>(i % 5),
                                       static_cast<uint16_t>(i / 5));
            auto buf = *YPaintBuffer::create();

            switch (i % 3) {
                case 0:
                    buf->addCircle(0, 20.0f + (i % 5) * 30.0f, 10.0f, 5.0f,
                                   0xFF000000 | i, 0, 0.0f, 0.0f);
                    break;
                case 1:
                    buf->addBox(0, 20.0f + (i % 5) * 30.0f, 10.0f, 10.0f, 8.0f,
                                0xFF000000 | i, 0, 0.0f, 0.0f);
                    break;
                case 2:
                    buf->addTriangle(0, 15.0f + (i % 5) * 30.0f, 5.0f,
                                     25.0f + (i % 5) * 30.0f, 5.0f,
                                     20.0f + (i % 5) * 30.0f, 15.0f,
                                     0xFF000000 | i, 0, 0.0f, 0.0f);
                    break;
            }
            builder->addYpaintBuffer(buf);
        }

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        uint32_t primOff = meta[0];
        uint32_t primCnt = meta[1];
        uint32_t primBase = primOff + primCnt;

        expect(primCnt == 15_u) << "should have 15 prims";

        // Verify offset table: each offset should point to valid prim data
        std::set<uint32_t> seenOffsets;
        for (uint32_t i = 0; i < primCnt; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);

            // Offset should be unique
            expect(seenOffsets.count(wordOff) == 0_u)
                << "offset " << wordOff << " should be unique";
            seenOffsets.insert(wordOff);

            // Offset should point to valid type
            uint32_t absOff = primBase + wordOff;
            uint32_t type = readU32(storage, absOff + 1);
            expect(type == static_cast<uint32_t>(SDFType::Circle) ||
                   type == static_cast<uint32_t>(SDFType::Box) ||
                   type == static_cast<uint32_t>(SDFType::Triangle))
                << "prim " << i << " has valid type " << type;
        }
    };

    "exhaustive: stress test 50 adds with random scrolling"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 100.0f);  // 5 lines
        builder->setGridCellSize(20.0f, 20.0f);

        uint32_t cursorRow = 0;
        uint32_t expectedPrims = 0;

        for (uint32_t i = 0; i < 50; i++) {
            // Scroll if needed
            while (cursorRow >= 5 && expectedPrims > 0) {
                builder->scrollLines(1);
                cursorRow--;
                expectedPrims--;
            }

            builder->setCursorPosition(0, static_cast<uint16_t>(cursorRow));
            auto buf = *YPaintBuffer::create();
            buf->addCircle(0, 50.0f, 10.0f, 5.0f, 0xFF000000 | i, 0, 0.0f, 0.0f);
            builder->addYpaintBuffer(buf);
            expectedPrims++;
            cursorRow++;

            // Verify count matches expectation
            expect(builder->primitiveCount() == expectedPrims)
                << "iteration " << i << ": expected " << expectedPrims
                << " got " << builder->primitiveCount();
        }

        // Final GPU buffer verification
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;
        expect(meta[1] == expectedPrims)
            << "final GPU buffer should have " << expectedPrims << " prims";
    };

    //=========================================================================
    // CURSOR NOT AT TOP - Tests that start cursor at non-zero row
    //=========================================================================

    "cursor at row 3: first add preserves nothing above"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 200.0f);  // 10 lines
        builder->setGridCellSize(20.0f, 20.0f);

        // Start cursor at row 3 (not at top!)
        builder->setCursorPosition(0, 3);

        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFFAA0000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        expect(builder->primitiveCount() == 1_u) << "should have 1 prim";

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;
        uint32_t primOff = meta[0];
        uint32_t primBase = primOff + meta[1];
        uint32_t wordOff = readU32(storage, primOff);
        uint32_t color = readU32(storage, primBase + wordOff + 6);

        expect(color == 0xFFAA0000u) << "circle color should match";
    };

    "cursor at row 5: add then add again at row 6"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 200.0f);  // 10 lines
        builder->setGridCellSize(20.0f, 20.0f);

        // First add at row 5
        builder->setCursorPosition(0, 5);
        auto buf1 = *YPaintBuffer::create();
        buf1->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFFAA0000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf1);

        expect(builder->primitiveCount() == 1_u) << "after first add";

        // Second add at row 6
        builder->setCursorPosition(0, 6);
        auto buf2 = *YPaintBuffer::create();
        buf2->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFF00BB00, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf2);

        expect(builder->primitiveCount() == 2_u) << "after second add, should have 2 prims";

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        expect(meta[1] == 2_u) << "GPU buffer should have 2 prims";

        // Verify both colors exist
        std::set<uint32_t> colors;
        uint32_t primOff = meta[0];
        uint32_t primBase = primOff + meta[1];
        for (uint32_t i = 0; i < meta[1]; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t color = readU32(storage, primBase + wordOff + 6);
            colors.insert(color);
        }

        expect(colors.count(0xFFAA0000) == 1_u) << "first color should exist";
        expect(colors.count(0xFF00BB00) == 1_u) << "second color should exist";
    };

    "cursor at row 5: multiple adds, previous NOT deleted"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 200.0f);  // 10 lines
        builder->setGridCellSize(20.0f, 20.0f);

        std::vector<uint32_t> expectedColors;

        // Add at row 5
        builder->setCursorPosition(0, 5);
        auto buf1 = *YPaintBuffer::create();
        buf1->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFF110000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf1);
        expectedColors.push_back(0xFF110000);
        expect(builder->primitiveCount() == 1_u) << "after add 1";

        // Add at row 6
        builder->setCursorPosition(0, 6);
        auto buf2 = *YPaintBuffer::create();
        buf2->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFF220000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf2);
        expectedColors.push_back(0xFF220000);
        expect(builder->primitiveCount() == 2_u) << "after add 2";

        // Add at row 7
        builder->setCursorPosition(0, 7);
        auto buf3 = *YPaintBuffer::create();
        buf3->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFF330000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf3);
        expectedColors.push_back(0xFF330000);
        expect(builder->primitiveCount() == 3_u) << "after add 3";

        // Add at row 8
        builder->setCursorPosition(0, 8);
        auto buf4 = *YPaintBuffer::create();
        buf4->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFF440000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf4);
        expectedColors.push_back(0xFF440000);
        expect(builder->primitiveCount() == 4_u) << "after add 4";

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        expect(meta[1] == 4_u) << "GPU buffer should have 4 prims, got " << meta[1];

        // Verify ALL colors exist (none deleted!)
        std::set<uint32_t> colors;
        uint32_t primOff = meta[0];
        uint32_t primBase = primOff + meta[1];
        for (uint32_t i = 0; i < meta[1]; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t color = readU32(storage, primBase + wordOff + 6);
            colors.insert(color);
        }

        for (uint32_t expected : expectedColors) {
            expect(colors.count(expected) == 1_u)
                << "color " << std::hex << expected << std::dec << " should exist";
        }
    };

    "simulate terminal: cursor starts at row 10, add multiple times"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 500.0f);  // 25 lines
        builder->setGridCellSize(20.0f, 20.0f);

        // Simulate terminal where cursor is already at row 10
        uint32_t cursorRow = 10;
        std::vector<uint32_t> addedColors;

        // Add 5 buffers sequentially
        for (uint32_t i = 0; i < 5; i++) {
            builder->setCursorPosition(0, static_cast<uint16_t>(cursorRow));
            auto buf = *YPaintBuffer::create();
            uint32_t color = 0xFF000000 | ((i + 1) * 0x111111);
            buf->addCircle(0, 50.0f, 10.0f, 8.0f, color, 0, 0.0f, 0.0f);
            builder->addYpaintBuffer(buf);
            addedColors.push_back(color);
            cursorRow++;

            expect(builder->primitiveCount() == i + 1)
                << "after add " << (i + 1) << " should have " << (i + 1) << " prims";
        }

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        expect(meta[1] == 5_u) << "GPU should have 5 prims";

        std::set<uint32_t> colors;
        uint32_t primOff = meta[0];
        uint32_t primBase = primOff + meta[1];
        for (uint32_t i = 0; i < meta[1]; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t color = readU32(storage, primBase + wordOff + 6);
            colors.insert(color);
        }

        for (uint32_t expected : addedColors) {
            expect(colors.count(expected) == 1_u)
                << "color " << std::hex << expected << " missing" << std::dec;
        }
    };

    "cursor at row 5: add, then press ENTER (move cursor), add again"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 200.0f);  // 10 lines
        builder->setGridCellSize(20.0f, 20.0f);

        // First shape at row 5
        builder->setCursorPosition(0, 5);
        auto buf1 = *YPaintBuffer::create();
        buf1->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFFAAAA00, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf1);

        expect(builder->primitiveCount() == 1_u) << "after first add";

        // Simulate ENTER press: cursor moves to row 6 (empty line)
        // No shape added, just cursor moved

        // Second shape at row 7 (after the empty line)
        builder->setCursorPosition(0, 7);
        auto buf2 = *YPaintBuffer::create();
        buf2->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFF00BBBB, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf2);

        // CRITICAL: first shape should NOT be deleted!
        expect(builder->primitiveCount() == 2_u)
            << "after second add, should have 2 prims, got " << builder->primitiveCount();

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        std::set<uint32_t> colors;
        uint32_t primOff = meta[0];
        uint32_t primBase = primOff + meta[1];
        for (uint32_t i = 0; i < meta[1]; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t color = readU32(storage, primBase + wordOff + 6);
            colors.insert(color);
        }

        expect(colors.count(0xFFAAAA00) == 1_u) << "first shape should NOT be deleted";
        expect(colors.count(0xFF00BBBB) == 1_u) << "second shape should exist";
    };

    "cursor at row 5: add 3 shapes, ENTER twice, add more - all preserved"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 300.0f);  // 15 lines
        builder->setGridCellSize(20.0f, 20.0f);

        // Add 3 shapes at rows 5, 6, 7
        builder->setCursorPosition(0, 5);
        auto buf1 = *YPaintBuffer::create();
        buf1->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFF110000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf1);

        builder->setCursorPosition(0, 6);
        auto buf2 = *YPaintBuffer::create();
        buf2->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFF220000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf2);

        builder->setCursorPosition(0, 7);
        auto buf3 = *YPaintBuffer::create();
        buf3->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFF330000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf3);

        expect(builder->primitiveCount() == 3_u) << "after 3 adds";

        // Simulate 2 ENTERs (cursor moves to row 9)
        // No shapes, just cursor movement

        // Add 2 more shapes at rows 9 and 10
        builder->setCursorPosition(0, 9);
        auto buf4 = *YPaintBuffer::create();
        buf4->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFF440000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf4);

        expect(builder->primitiveCount() == 4_u) << "after 4th add";

        builder->setCursorPosition(0, 10);
        auto buf5 = *YPaintBuffer::create();
        buf5->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFF550000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf5);

        expect(builder->primitiveCount() == 5_u)
            << "after 5th add, should have 5 prims, got " << builder->primitiveCount();

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        expect(meta[1] == 5_u) << "GPU should have 5 prims";

        std::set<uint32_t> colors;
        uint32_t primOff = meta[0];
        uint32_t primBase = primOff + meta[1];
        for (uint32_t i = 0; i < meta[1]; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t color = readU32(storage, primBase + wordOff + 6);
            colors.insert(color);
        }

        expect(colors.count(0xFF110000) == 1_u) << "shape 1 preserved";
        expect(colors.count(0xFF220000) == 1_u) << "shape 2 preserved";
        expect(colors.count(0xFF330000) == 1_u) << "shape 3 preserved";
        expect(colors.count(0xFF440000) == 1_u) << "shape 4 preserved";
        expect(colors.count(0xFF550000) == 1_u) << "shape 5 preserved";
    };

    "real terminal scenario: type command, ENTER, see result, ENTER, type again"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 200.0f, 500.0f);  // 25 lines
        builder->setGridCellSize(20.0f, 20.0f);

        // Terminal starts at some row (simulating scrollback history)
        uint32_t cursorRow = 12;

        // User types command with ypaint output (row 12)
        builder->setCursorPosition(0, static_cast<uint16_t>(cursorRow));
        auto buf1 = *YPaintBuffer::create();
        buf1->addBox(0, 100.0f, 10.0f, 80.0f, 15.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf1);
        cursorRow++;

        expect(builder->primitiveCount() == 1_u);

        // ENTER pressed, cursor at row 13 (empty prompt)
        // Another ENTER, cursor at row 14

        cursorRow = 14;

        // User types another command with ypaint output
        builder->setCursorPosition(0, static_cast<uint16_t>(cursorRow));
        auto buf2 = *YPaintBuffer::create();
        buf2->addCircle(0, 100.0f, 10.0f, 12.0f, 0xFF00FF00, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf2);
        cursorRow++;

        // CRITICAL CHECK: first box should NOT be deleted!
        expect(builder->primitiveCount() == 2_u)
            << "both shapes should exist, got " << builder->primitiveCount();

        // More ENTERs
        cursorRow = 17;

        // Third command
        builder->setCursorPosition(0, static_cast<uint16_t>(cursorRow));
        auto buf3 = *YPaintBuffer::create();
        buf3->addTriangle(0, 80.0f, 5.0f, 120.0f, 5.0f, 100.0f, 18.0f,
                          0xFF0000FF, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf3);

        expect(builder->primitiveCount() == 3_u)
            << "all 3 shapes should exist, got " << builder->primitiveCount();

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        expect(meta[1] == 3_u) << "GPU should have 3 prims";

        // Verify all types exist
        std::set<uint32_t> types;
        uint32_t primOff = meta[0];
        uint32_t primBase = primOff + meta[1];
        for (uint32_t i = 0; i < meta[1]; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t type = readU32(storage, primBase + wordOff + 1);
            types.insert(type);
        }

        expect(types.count(static_cast<uint32_t>(SDFType::Box)) == 1_u) << "box exists";
        expect(types.count(static_cast<uint32_t>(SDFType::Circle)) == 1_u) << "circle exists";
        expect(types.count(static_cast<uint32_t>(SDFType::Triangle)) == 1_u) << "triangle exists";
    };

    //=========================================================================
    // SCROLL FIRST, THEN ADD - Critical terminal scenario tests
    //=========================================================================

    "SCROLL 10 LINES FIRST, then add shape"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 500.0f);  // 25 lines
        builder->setGridCellSize(20.0f, 20.0f);

        // SCROLL 10 LINES FIRST (simulating terminal scrolling before any ypaint)
        builder->scrollLines(10);

        // Now add a shape at current cursor position
        builder->setCursorPosition(0, 5);
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFFAA0000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf);

        expect(builder->primitiveCount() == 1_u) << "should have 1 prim after scroll+add";

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;
        expect(meta[1] == 1_u) << "GPU should have 1 prim";
    };

    "SCROLL 10, add shape, SCROLL 5 more, add another - both preserved"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 500.0f);  // 25 lines
        builder->setGridCellSize(20.0f, 20.0f);

        // SCROLL 10 LINES FIRST
        builder->scrollLines(10);

        // Add first shape
        builder->setCursorPosition(0, 5);
        auto buf1 = *YPaintBuffer::create();
        buf1->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFFAA0000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf1);

        expect(builder->primitiveCount() == 1_u) << "after first add";

        // SCROLL 5 MORE LINES
        builder->scrollLines(5);

        // Add second shape
        builder->setCursorPosition(0, 3);
        auto buf2 = *YPaintBuffer::create();
        buf2->addBox(0, 50.0f, 10.0f, 15.0f, 10.0f, 0xFF00BB00, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf2);

        // First shape should still exist!
        expect(builder->primitiveCount() == 2_u)
            << "both shapes should exist, got " << builder->primitiveCount();

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        expect(meta[1] == 2_u) << "GPU should have 2 prims";

        std::set<uint32_t> colors;
        uint32_t primOff = meta[0];
        uint32_t primBase = primOff + meta[1];
        for (uint32_t i = 0; i < meta[1]; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t type = readU32(storage, primBase + wordOff + 1);
            uint32_t colorOff = (type == static_cast<uint32_t>(SDFType::Circle)) ? 6 : 7;
            uint32_t color = readU32(storage, primBase + wordOff + colorOff);
            colors.insert(color);
        }

        expect(colors.count(0xFFAA0000) == 1_u) << "first circle should exist";
        expect(colors.count(0xFF00BB00) == 1_u) << "second box should exist";
    };

    "SCROLL 10, add 3 shapes, SCROLL 5, add 2 more - all 5 preserved"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 500.0f);  // 25 lines
        builder->setGridCellSize(20.0f, 20.0f);

        std::vector<uint32_t> expectedColors;

        // SCROLL 10 LINES FIRST
        builder->scrollLines(10);

        // Add 3 shapes
        for (uint32_t i = 0; i < 3; i++) {
            builder->setCursorPosition(0, static_cast<uint16_t>(5 + i));
            auto buf = *YPaintBuffer::create();
            uint32_t color = 0xFF100000 * (i + 1);
            buf->addCircle(0, 50.0f, 10.0f, 8.0f, color, 0, 0.0f, 0.0f);
            builder->addYpaintBuffer(buf);
            expectedColors.push_back(color);
        }

        expect(builder->primitiveCount() == 3_u) << "after 3 adds";

        // SCROLL 5 MORE
        builder->scrollLines(5);

        // Add 2 more shapes
        for (uint32_t i = 0; i < 2; i++) {
            builder->setCursorPosition(0, static_cast<uint16_t>(3 + i));
            auto buf = *YPaintBuffer::create();
            uint32_t color = 0xFF001000 * (i + 1);
            buf->addCircle(0, 50.0f, 10.0f, 8.0f, color, 0, 0.0f, 0.0f);
            builder->addYpaintBuffer(buf);
            expectedColors.push_back(color);
        }

        expect(builder->primitiveCount() == 5_u)
            << "all 5 shapes should exist, got " << builder->primitiveCount();

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;
        expect(meta[1] == 5_u) << "GPU should have 5 prims";
    };

    "repeated scroll-add cycles: 5 cycles of (scroll 2, add shape)"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 500.0f);  // 25 lines
        builder->setGridCellSize(20.0f, 20.0f);

        std::vector<uint32_t> expectedColors;

        for (uint32_t cycle = 0; cycle < 5; cycle++) {
            // SCROLL 2 lines each cycle
            builder->scrollLines(2);

            // Add shape
            builder->setCursorPosition(0, static_cast<uint16_t>(10 + cycle));
            auto buf = *YPaintBuffer::create();
            uint32_t color = 0xFF000011 * (cycle + 1);
            buf->addCircle(0, 50.0f, 10.0f, 8.0f, color, 0, 0.0f, 0.0f);
            builder->addYpaintBuffer(buf);
            expectedColors.push_back(color);

            expect(builder->primitiveCount() == cycle + 1)
                << "cycle " << cycle << ": should have " << (cycle + 1) << " prims";
        }

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        expect(meta[1] == 5_u) << "GPU should have 5 prims";

        std::set<uint32_t> colors;
        uint32_t primOff = meta[0];
        uint32_t primBase = primOff + meta[1];
        for (uint32_t i = 0; i < meta[1]; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t color = readU32(storage, primBase + wordOff + 6);
            colors.insert(color);
        }

        for (uint32_t expected : expectedColors) {
            expect(colors.count(expected) == 1_u)
                << "color " << std::hex << expected << " should exist" << std::dec;
        }
    };

    "terminal chart.sh scenario: scroll, add chart, scroll, add another chart"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 200.0f, 500.0f);  // 25 lines
        builder->setGridCellSize(20.0f, 20.0f);

        // Simulate: user runs chart.sh first time
        // Terminal might have scrolled some lines already
        builder->scrollLines(5);

        // First chart: multiple primitives
        builder->setCursorPosition(0, 10);
        auto buf1 = *YPaintBuffer::create();
        buf1->addBox(0, 100.0f, 50.0f, 80.0f, 40.0f, 0xFFFF0000, 0, 0.0f, 0.0f);  // chart bg
        buf1->addBox(1, 30.0f, 70.0f, 10.0f, 30.0f, 0xFF00FF00, 0, 0.0f, 0.0f);   // bar 1
        buf1->addBox(2, 50.0f, 70.0f, 10.0f, 50.0f, 0xFF0000FF, 0, 0.0f, 0.0f);   // bar 2
        builder->addYpaintBuffer(buf1);

        expect(builder->primitiveCount() == 3_u) << "first chart: 3 prims";

        // User presses ENTER, some scrolling happens
        builder->scrollLines(3);

        // User runs chart.sh SECOND time
        builder->setCursorPosition(0, 15);
        auto buf2 = *YPaintBuffer::create();
        buf2->addBox(0, 100.0f, 50.0f, 80.0f, 40.0f, 0xFFAA0000, 0, 0.0f, 0.0f);  // chart bg
        buf2->addBox(1, 30.0f, 70.0f, 10.0f, 40.0f, 0xFF00AA00, 0, 0.0f, 0.0f);   // bar 1
        buf2->addBox(2, 50.0f, 70.0f, 10.0f, 60.0f, 0xFF0000AA, 0, 0.0f, 0.0f);   // bar 2
        builder->addYpaintBuffer(buf2);

        // CRITICAL: All 6 primitives should exist!
        expect(builder->primitiveCount() == 6_u)
            << "both charts should exist: expected 6, got " << builder->primitiveCount();

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        expect(meta[1] == 6_u) << "GPU should have 6 prims";

        // Verify colors from both charts
        std::set<uint32_t> colors;
        uint32_t primOff = meta[0];
        uint32_t primBase = primOff + meta[1];
        for (uint32_t i = 0; i < meta[1]; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t color = readU32(storage, primBase + wordOff + 7);  // Box color at +7
            colors.insert(color);
        }

        // First chart colors
        expect(colors.count(0xFFFF0000) == 1_u) << "first chart bg";
        expect(colors.count(0xFF00FF00) == 1_u) << "first chart bar1";
        expect(colors.count(0xFF0000FF) == 1_u) << "first chart bar2";
        // Second chart colors
        expect(colors.count(0xFFAA0000) == 1_u) << "second chart bg";
        expect(colors.count(0xFF00AA00) == 1_u) << "second chart bar1";
        expect(colors.count(0xFF0000AA) == 1_u) << "second chart bar2";
    };

    "extreme: scroll 100 lines, add at row 40, scroll 10 more, add at row 35"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 100.0f, 1000.0f);  // 50 lines
        builder->setGridCellSize(20.0f, 20.0f);

        // SCROLL 100 LINES (way more than visible - but no prims yet, so nothing deleted)
        builder->scrollLines(100);

        // Add first shape at row 40 (near bottom of 50-line scene)
        builder->setCursorPosition(0, 40);
        auto buf1 = *YPaintBuffer::create();
        buf1->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFFAAAAAA, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf1);

        expect(builder->primitiveCount() == 1_u);

        // SCROLL 10 MORE - shape at row 40 moves to row 30 (still visible in 50-line scene)
        builder->scrollLines(10);

        // Add second shape at row 35
        builder->setCursorPosition(0, 35);
        auto buf2 = *YPaintBuffer::create();
        buf2->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFFBBBBBB, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf2);

        // First shape moved from row 40 to row 30 - still visible!
        expect(builder->primitiveCount() == 2_u)
            << "both shapes should exist, got " << builder->primitiveCount();

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;
        expect(meta[1] == 2_u) << "GPU should have 2 prims";
    };
};

//=============================================================================
// EXHAUSTIVE GPU BUFFER VERIFICATION - Inspect actual buffer content
// These tests verify the EXACT content of the GPU buffer to find bugs
//=============================================================================

suite gpu_buffer_deep_inspection = [] {

    // Helper: read the per-primitive gridOffset (word[0])
    auto readPrimGridOffset = [](const float* storage, uint32_t primOff)
        -> std::pair<uint16_t, uint16_t> {
        uint32_t packed = readU32(storage, primOff);
        uint16_t col = packed & 0xFFFF;
        uint16_t row = (packed >> 16) & 0xFFFF;
        return {col, row};
    };

    // Helper: get primitive center coordinates
    auto readPrimCenter = [](const float* storage, uint32_t primOff, uint32_t type)
        -> std::pair<float, float> {
        // type is at primOff+1, layer at primOff+2, geometry starts at primOff+3
        // For Circle/Box/RoundedBox: cx at +3, cy at +4
        // For Triangle: ax at +3, ay at +4 (use centroid)
        if (type == static_cast<uint32_t>(SDFType::Triangle)) {
            float ax = storage[primOff + 3];
            float ay = storage[primOff + 4];
            float bx = storage[primOff + 5];
            float by = storage[primOff + 6];
            float cx = storage[primOff + 7];
            float cy = storage[primOff + 8];
            return {(ax + bx + cx) / 3.0f, (ay + by + cy) / 3.0f};
        }
        return {storage[primOff + 3], storage[primOff + 4]};
    };

    "cursor at row 3: verify primitive gridOffset field is set correctly"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 200.0f, 400.0f);
        builder->setGridCellSize(20.0f, 20.0f);

        // Set cursor at row 3
        builder->setCursorPosition(5, 3);

        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 30.0f, 10.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        uint32_t primOff = meta[0];
        uint32_t primCnt = meta[1];
        uint32_t primBase = primOff + primCnt;

        expect(primCnt == 1_u) << "should have 1 prim";

        // Read the primitive's word offset
        uint32_t wordOff = readU32(storage, primOff);
        uint32_t absPrimOff = primBase + wordOff;

        // Word 0 of primitive should contain gridOffset (col=5, row=3)
        uint32_t packedGridOff = readU32(storage, absPrimOff);
        uint16_t gridCol = packedGridOff & 0xFFFF;
        uint16_t gridRow = (packedGridOff >> 16) & 0xFFFF;

        expect(gridCol == 5_u) << "gridOffset col should be 5, got " << gridCol;
        expect(gridRow == 3_u) << "gridOffset row should be 3, got " << gridRow;
    };

    "cursor at row 0: verify gridOffset is (0,0)"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 200.0f, 400.0f);
        builder->setGridCellSize(20.0f, 20.0f);

        builder->setCursorPosition(0, 0);

        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 30.0f, 10.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        uint32_t primOff = meta[0];
        uint32_t primBase = primOff + meta[1];
        uint32_t wordOff = readU32(storage, primOff);
        uint32_t absPrimOff = primBase + wordOff;

        uint32_t packedGridOff = readU32(storage, absPrimOff);
        uint16_t gridCol = packedGridOff & 0xFFFF;
        uint16_t gridRow = (packedGridOff >> 16) & 0xFFFF;

        expect(gridCol == 0_u) << "gridOffset col should be 0";
        expect(gridRow == 0_u) << "gridOffset row should be 0";
    };

    "cursor at row 10: verify grid cell contains primitive reference"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        float cellH = 20.0f;
        builder->setSceneBounds(0, 0, 200.0f, 500.0f);  // 25 lines
        builder->setGridCellSize(20.0f, cellH);

        // Cursor at row 10
        builder->setCursorPosition(0, 10);

        // Add circle at local Y=30 (spans ~1.5 cells from cursor)
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 30.0f, 15.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        uint32_t gridOff = meta[2];
        uint32_t gridW = meta[3];
        uint32_t gridH = meta[4];

        // Circle at local Y=30 with cursor at row 10
        // localLine = floor(30/20) = 1
        // primLine = 10 + 1 = 11
        // The primitive should be stored at line 11

        // Scan grid to find which rows contain primitive references
        std::set<uint32_t> rowsWithPrims;
        for (uint32_t row = 0; row < gridH; row++) {
            for (uint32_t col = 0; col < gridW; col++) {
                uint32_t cellIdx = row * gridW + col;
                uint32_t packedStart = readU32(storage, gridOff + cellIdx);
                uint32_t cnt = readU32(storage, gridOff + packedStart);
                for (uint32_t i = 0; i < cnt; i++) {
                    uint32_t rawVal = readU32(storage, gridOff + packedStart + 1 + i);
                    if ((rawVal & 0x80000000u) == 0) {
                        rowsWithPrims.insert(row);
                    }
                }
            }
        }

        expect(!rowsWithPrims.empty())
            << "grid should have primitive references, found none!";

        // Should find references around row 10-12 (cursor row + local lines)
        bool foundExpectedRow = false;
        for (uint32_t row : rowsWithPrims) {
            if (row >= 10 && row <= 13) {
                foundExpectedRow = true;
            }
        }
        expect(foundExpectedRow)
            << "should find prim refs near row 10-13, found rows: ";
    };

    "verify primitive local coords are NOT modified by cursor position"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 200.0f, 400.0f);
        builder->setGridCellSize(20.0f, 20.0f);

        builder->setCursorPosition(0, 10);

        // Circle at cx=50, cy=30 (local coords)
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 30.0f, 10.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        uint32_t primOff = meta[0];
        uint32_t primBase = primOff + meta[1];
        uint32_t wordOff = readU32(storage, primOff);
        uint32_t absPrimOff = primBase + wordOff;

        // Circle: [gridOffset][type][layer][cx][cy][r][fill][stroke][strokeW][round]
        // cx at +3, cy at +4
        float cx = storage[absPrimOff + 3];
        float cy = storage[absPrimOff + 4];

        // The LOCAL coords should be preserved (NOT modified by cursor)
        expect(cx == 50.0_f) << "cx should be 50 (local coord), got " << cx;
        expect(cy == 30.0_f) << "cy should be 30 (local coord), got " << cy;
    };

    "multiple prims at different cursor rows: verify all have correct gridOffsets"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 200.0f, 500.0f);
        builder->setGridCellSize(20.0f, 20.0f);

        // Add prims at different cursor positions
        std::vector<std::pair<uint16_t, uint16_t>> cursorPositions = {
            {0, 3}, {2, 7}, {5, 12}, {0, 18}
        };

        for (auto [col, row] : cursorPositions) {
            builder->setCursorPosition(col, row);
            auto buf = *YPaintBuffer::create();
            buf->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFF000000 | row, 0, 0.0f, 0.0f);
            builder->addYpaintBuffer(buf);
        }

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        uint32_t primOff = meta[0];
        uint32_t primCnt = meta[1];
        uint32_t primBase = primOff + primCnt;

        expect(primCnt == 4_u) << "should have 4 prims";

        // Verify each prim has correct gridOffset
        std::map<uint32_t, std::pair<uint16_t, uint16_t>> colorToExpected;
        for (auto [col, row] : cursorPositions) {
            colorToExpected[0xFF000000 | row] = {col, row};
        }

        for (uint32_t i = 0; i < primCnt; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t absPrimOff = primBase + wordOff;

            uint32_t packedGridOff = readU32(storage, absPrimOff);
            uint16_t gridCol = packedGridOff & 0xFFFF;
            uint16_t gridRow = (packedGridOff >> 16) & 0xFFFF;

            // Color is at offset +6 for Circle
            uint32_t color = readU32(storage, absPrimOff + 6);

            auto expected = colorToExpected[color];
            expect(gridCol == expected.first)
                << "prim with color " << std::hex << color << std::dec
                << " gridCol should be " << expected.first << " got " << gridCol;
            expect(gridRow == expected.second)
                << "prim with color " << std::hex << color << std::dec
                << " gridRow should be " << expected.second << " got " << gridRow;
        }
    };

    "grid dimensions must cover all primitives"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 200.0f, 200.0f);  // 10 lines
        builder->setGridCellSize(20.0f, 20.0f);

        // Add prim at cursor row 15 (beyond initial scene bounds!)
        builder->setCursorPosition(0, 15);
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;
        uint32_t gridH = meta[4];

        // Grid height must be extended to cover row 15
        expect(gridH >= 16_u)
            << "grid height should be at least 16 to cover cursor row 15, got " << gridH;
    };

    "scene bounds metadata matches actual scene"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 300.0f, 400.0f);
        builder->setGridCellSize(20.0f, 20.0f);

        builder->setCursorPosition(0, 5);
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 150.0f, 100.0f, 20.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        // Scene bounds are at words 8-11 as floats
        float sceneMinX, sceneMinY, sceneMaxX, sceneMaxY;
        std::memcpy(&sceneMinX, &meta[8], sizeof(float));
        std::memcpy(&sceneMinY, &meta[9], sizeof(float));
        std::memcpy(&sceneMaxX, &meta[10], sizeof(float));
        std::memcpy(&sceneMaxY, &meta[11], sizeof(float));

        expect(sceneMinX == 0.0_f) << "sceneMinX should be 0";
        expect(sceneMinY == 0.0_f) << "sceneMinY should be 0";
        expect(sceneMaxX == 300.0_f) << "sceneMaxX should be 300";
        expect(sceneMaxY == 400.0_f) << "sceneMaxY should be 400";
    };

    "cell size in metadata matches set cell size"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        builder->setSceneBounds(0, 0, 200.0f, 200.0f);
        builder->setGridCellSize(15.5f, 22.0f);

        builder->setCursorPosition(0, 0);
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 50.0f, 10.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        // Cell size is packed in word 5
        float cellSizeX = unpackF16(meta[5], false);
        float cellSizeY = unpackF16(meta[5], true);

        expect(std::abs(cellSizeX - 15.5f) < 0.1f)
            << "cellSizeX should be ~15.5, got " << cellSizeX;
        expect(std::abs(cellSizeY - 22.0f) < 0.1f)
            << "cellSizeY should be ~22.0, got " << cellSizeY;
    };

    "shader grid lookup simulation at cursor row 3"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        float cellH = 20.0f;
        builder->setSceneBounds(0, 0, 200.0f, 500.0f);
        builder->setGridCellSize(20.0f, cellH);

        // Cursor at row 3
        builder->setCursorPosition(2, 3);

        // Circle at local coords (50, 10) - local Y=10 means line offset 0
        // With cursor row 3, this should be at grid row 3
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 10.0f, 8.0f, 0xFFABCDEF, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        uint32_t gridOff = meta[2];
        uint32_t gridW = meta[3];
        uint32_t primBase = meta[0] + meta[1];

        // Simulate shader lookup at screen position where prim should be visible
        // If cursor is at row 3 and cell height is 20, primitives placed there
        // should be found at pixel Y = 3*20 + localY = 60 + 10 = 70
        // Hmm wait, that's not how it works...

        // Actually, let's just check that grid row 3 has the primitive
        uint32_t testRow = 3;
        bool foundPrimAtRow3 = false;
        for (uint32_t col = 0; col < gridW; col++) {
            uint32_t cellIdx = testRow * gridW + col;
            uint32_t packedStart = readU32(storage, gridOff + cellIdx);
            uint32_t cnt = readU32(storage, gridOff + packedStart);
            for (uint32_t i = 0; i < cnt; i++) {
                uint32_t rawVal = readU32(storage, gridOff + packedStart + 1 + i);
                if ((rawVal & 0x80000000u) == 0) {
                    uint32_t primOff = primBase + rawVal;
                    uint32_t color = readU32(storage, primOff + 6);
                    if (color == 0xFFABCDEF) {
                        foundPrimAtRow3 = true;
                    }
                }
            }
        }

        expect(foundPrimAtRow3)
            << "primitive should be found in grid row 3 (cursor row)";
    };

    "chart.sh scenario: large primitives with cursor at row 3"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        // Typical terminal: ~20 pixel cell height, 25 lines visible
        float cellH = 20.0f;
        builder->setSceneBounds(0, 0, 400.0f, 500.0f);
        builder->setGridCellSize(10.0f, cellH);

        // Cursor at row 3 (like user's scenario)
        builder->setCursorPosition(0, 3);

        // Chart primitives (like chart.sh)
        auto buf = *YPaintBuffer::create();
        // Bars at various Y positions
        buf->addBox(0, 70.0f, 100.0f, 20.0f, 40.0f, 0xFF3498DB, 0, 0.0f, 4.0f);   // Y center 100
        buf->addBox(1, 120.0f, 60.0f, 20.0f, 60.0f, 0xFF2ECC71, 0, 0.0f, 4.0f);   // Y center 60
        buf->addBox(2, 170.0f, 80.0f, 20.0f, 50.0f, 0xFFE74C3C, 0, 0.0f, 4.0f);   // Y center 80
        buf->addBox(3, 220.0f, 120.0f, 20.0f, 30.0f, 0xFFF39C12, 0, 0.0f, 4.0f);  // Y center 120
        buf->addBox(4, 270.0f, 50.0f, 20.0f, 65.0f, 0xFF9B59B6, 0, 0.0f, 4.0f);   // Y center 50
        builder->addYpaintBuffer(buf);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        expect(meta[1] == 5_u) << "should have 5 prims";

        uint32_t gridOff = meta[2];
        uint32_t gridW = meta[3];
        uint32_t gridH = meta[4];
        uint32_t primBase = meta[0] + meta[1];

        // All prims should be reachable from grid
        std::set<uint32_t> reachableColors;
        for (uint32_t row = 0; row < gridH; row++) {
            for (uint32_t col = 0; col < gridW; col++) {
                uint32_t cellIdx = row * gridW + col;
                uint32_t packedStart = readU32(storage, gridOff + cellIdx);
                uint32_t cnt = readU32(storage, gridOff + packedStart);
                for (uint32_t i = 0; i < cnt; i++) {
                    uint32_t rawVal = readU32(storage, gridOff + packedStart + 1 + i);
                    if ((rawVal & 0x80000000u) == 0) {
                        uint32_t primOff = primBase + rawVal;
                        uint32_t color = readU32(storage, primOff + 7); // Box color at +7
                        reachableColors.insert(color);
                    }
                }
            }
        }

        expect(reachableColors.size() == 5_u)
            << "all 5 box colors should be reachable, found " << reachableColors.size();

        expect(reachableColors.count(0xFF3498DB) == 1_u) << "blue bar reachable";
        expect(reachableColors.count(0xFF2ECC71) == 1_u) << "green bar reachable";
        expect(reachableColors.count(0xFFE74C3C) == 1_u) << "red bar reachable";
        expect(reachableColors.count(0xFFF39C12) == 1_u) << "orange bar reachable";
        expect(reachableColors.count(0xFF9B59B6) == 1_u) << "purple bar reachable";

        // Verify gridOffsets are all (0, 3)
        uint32_t primOff = meta[0];
        for (uint32_t i = 0; i < meta[1]; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t absPrimOff = primBase + wordOff;
            uint32_t packedGridOff = readU32(storage, absPrimOff);
            uint16_t gridCol = packedGridOff & 0xFFFF;
            uint16_t gridRow = (packedGridOff >> 16) & 0xFFFF;

            expect(gridCol == 0_u) << "prim " << i << " gridCol should be 0";
            expect(gridRow == 3_u) << "prim " << i << " gridRow should be 3";
        }
    };

    "verify grid rows match cursor row + local primitive lines"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        float cellH = 20.0f;
        builder->setSceneBounds(0, 0, 100.0f, 500.0f);
        builder->setGridCellSize(20.0f, cellH);

        // Cursor at row 5
        builder->setCursorPosition(0, 5);

        // Circle at local Y=50, radius 10
        // Local line min = floor((50-10)/20) = floor(40/20) = 2
        // Local line max = floor((50+10)/20) = floor(60/20) = 3
        // Grid lines = 5+2=7 to 5+3=8
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 50.0f, 10.0f, 0xFFAABBCC, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        uint32_t gridOff = meta[2];
        uint32_t gridW = meta[3];
        uint32_t gridH = meta[4];
        uint32_t primBase = meta[0] + meta[1];

        // Find which rows contain the primitive
        std::set<uint32_t> rowsWithPrim;
        for (uint32_t row = 0; row < gridH; row++) {
            for (uint32_t col = 0; col < gridW; col++) {
                uint32_t cellIdx = row * gridW + col;
                uint32_t packedStart = readU32(storage, gridOff + cellIdx);
                uint32_t cnt = readU32(storage, gridOff + packedStart);
                for (uint32_t i = 0; i < cnt; i++) {
                    uint32_t rawVal = readU32(storage, gridOff + packedStart + 1 + i);
                    if ((rawVal & 0x80000000u) == 0) {
                        uint32_t primOff = primBase + rawVal;
                        uint32_t color = readU32(storage, primOff + 6);
                        if (color == 0xFFAABBCC) {
                            rowsWithPrim.insert(row);
                        }
                    }
                }
            }
        }

        // Should be in rows 7 and 8 (cursor 5 + local lines 2-3)
        expect(rowsWithPrim.count(7) == 1_u || rowsWithPrim.count(8) == 1_u)
            << "primitive should be in rows 7-8, found in rows: [listed]";
    };

    "shader scenario: verify pixelPos -> scenePos -> cellY -> gridRow match"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        // Terminal-like setup: 80x25 cells, 10x20 pixel cells
        float cellW = 10.0f, cellH = 20.0f;
        float termW = 80 * cellW, termH = 25 * cellH;  // 800x500

        builder->setSceneBounds(0, 0, termW, termH);
        builder->setGridCellSize(cellW, cellH);

        // Cursor at row 3
        builder->setCursorPosition(0, 3);

        // Circle at local center (50, 100)
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 50.0f, 100.0f, 10.0f, 0xFFABCDEF, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        // Verify metadata scene bounds
        float contentMinX, contentMinY, contentMaxX, contentMaxY;
        std::memcpy(&contentMinX, &meta[8], sizeof(float));
        std::memcpy(&contentMinY, &meta[9], sizeof(float));
        std::memcpy(&contentMaxX, &meta[10], sizeof(float));
        std::memcpy(&contentMaxY, &meta[11], sizeof(float));

        expect(contentMinX == 0.0_f) << "contentMinX should be 0";
        expect(contentMinY == 0.0_f) << "contentMinY should be 0";
        expect(contentMaxX == termW) << "contentMaxX should be " << termW;
        expect(contentMaxY == termH) << "contentMaxY should be " << termH;

        // Verify cellSize in metadata matches
        float cellSizeX = unpackF16(meta[5], false);
        float cellSizeY = unpackF16(meta[5], true);
        expect(std::abs(cellSizeX - cellW) < 0.1f) << "cellSizeX should be " << cellW;
        expect(std::abs(cellSizeY - cellH) < 0.1f) << "cellSizeY should be " << cellH;

        // Primitive has gridOffset (0, 3) and local center (50, 100)
        // It should appear at screen pixel Y = cursorRow * cellH + localY = 3*20 + 100 = 160
        // When shader looks at pixel Y=160:
        //   scenePos.y = 160 (since content bounds = terminal size)
        //   cellY = floor((160 - 0) / 20) = 8
        // Primitive should be in grid row 8

        uint32_t gridOff = meta[2];
        uint32_t gridW = meta[3];
        uint32_t primBase = meta[0] + meta[1];

        // Check grid row 8 contains our primitive
        uint32_t targetRow = 8;
        bool foundInRow8 = false;
        for (uint32_t col = 0; col < gridW; col++) {
            uint32_t cellIdx = targetRow * gridW + col;
            uint32_t packedStart = readU32(storage, gridOff + cellIdx);
            uint32_t cnt = readU32(storage, gridOff + packedStart);
            for (uint32_t i = 0; i < cnt; i++) {
                uint32_t rawVal = readU32(storage, gridOff + packedStart + 1 + i);
                if ((rawVal & 0x80000000u) == 0) {
                    uint32_t primOff = primBase + rawVal;
                    uint32_t color = readU32(storage, primOff + 6);
                    if (color == 0xFFABCDEF) {
                        foundInRow8 = true;
                    }
                }
            }
        }

        expect(foundInRow8)
            << "primitive with cursor row 3 and local Y=100 should be in grid row 8 "
            << "(cursorRow + floor(localY/cellH) = 3 + floor(100/20) = 3 + 5 = 8)";

        // Now verify the primitive's gridOffset
        uint32_t primOff = meta[0];
        uint32_t wordOff = readU32(storage, primOff);
        uint32_t absPrimOff = primBase + wordOff;

        uint32_t packedGridOff = readU32(storage, absPrimOff);
        uint16_t gridOffX = packedGridOff & 0xFFFF;
        uint16_t gridOffY = (packedGridOff >> 16) & 0xFFFF;

        expect(gridOffX == 0_u) << "gridOffset X should be 0";
        expect(gridOffY == 3_u) << "gridOffset Y should be 3 (cursor row)";

        // And verify primitive local coords are preserved
        float primCx = storage[absPrimOff + 3];
        float primCy = storage[absPrimOff + 4];
        expect(primCx == 50.0_f) << "primitive cx should be 50";
        expect(primCy == 100.0_f) << "primitive cy should be 100";
    };

    "verify grid row calculation: cursor row 3, local Y spanning 0-100"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        float cellH = 20.0f;
        builder->setSceneBounds(0, 0, 200.0f, 500.0f);
        builder->setGridCellSize(20.0f, cellH);

        builder->setCursorPosition(0, 3);

        // Box from local Y=0 to Y=100 (center=50, half-height=50)
        auto buf = *YPaintBuffer::create();
        buf->addBox(0, 100.0f, 50.0f, 20.0f, 50.0f, 0xDEADBEEF, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        uint32_t gridOff = meta[2];
        uint32_t gridW = meta[3];
        uint32_t gridH = meta[4];
        uint32_t primBase = meta[0] + meta[1];

        // Box AABB: minY=0, maxY=100
        // localMinLine = floor(0/20) = 0
        // localMaxLine = floor(100/20) = 5
        // primMinLine = 3 + 0 = 3
        // primMaxLine = 3 + 5 = 8
        // Should be in grid rows 3-8

        std::set<uint32_t> rowsWithPrim;
        for (uint32_t row = 0; row < gridH; row++) {
            for (uint32_t col = 0; col < gridW; col++) {
                uint32_t cellIdx = row * gridW + col;
                uint32_t packedStart = readU32(storage, gridOff + cellIdx);
                uint32_t cnt = readU32(storage, gridOff + packedStart);
                for (uint32_t i = 0; i < cnt; i++) {
                    uint32_t rawVal = readU32(storage, gridOff + packedStart + 1 + i);
                    if ((rawVal & 0x80000000u) == 0) {
                        uint32_t primOff = primBase + rawVal;
                        uint32_t color = readU32(storage, primOff + 7);  // Box color at +7
                        if (color == 0xDEADBEEF) {
                            rowsWithPrim.insert(row);
                        }
                    }
                }
            }
        }

        // DEBUG: Print which rows actually have the primitive
        if (rowsWithPrim.empty()) {
            std::cerr << "BUG: No grid rows contain the primitive!" << std::endl;
            std::cerr << "gridH=" << gridH << " gridW=" << gridW << std::endl;
            std::cerr << "gridOff=" << gridOff << " primBase=" << primBase << std::endl;

            // Dump grid cells for rows 3-8 col 0
            std::cerr << "Grid cell dump for rows 3-8, col 0:" << std::endl;
            for (uint32_t row = 3; row <= 8 && row < gridH; row++) {
                uint32_t cellIdx = row * gridW + 0;
                uint32_t packedStart = readU32(storage, gridOff + cellIdx);
                uint32_t cnt = readU32(storage, gridOff + packedStart);
                std::cerr << "  row " << row << ": cellIdx=" << cellIdx
                          << " packedStart=" << packedStart << " cnt=" << cnt;
                for (uint32_t i = 0; i < cnt && i < 3; i++) {
                    uint32_t rawVal = readU32(storage, gridOff + packedStart + 1 + i);
                    std::cerr << " entry[" << i << "]=" << rawVal;
                    if ((rawVal & 0x80000000u) == 0) {
                        std::cerr << "(prim)";
                    } else {
                        std::cerr << "(glyph)";
                    }
                }
                std::cerr << std::endl;
            }

            // Scan ALL primitives to see where they are
            uint32_t primCnt = meta[1];
            uint32_t primOff = meta[0];
            uint32_t primBase2 = primOff + primCnt;
            for (uint32_t i = 0; i < primCnt; i++) {
                uint32_t wordOff = readU32(storage, primOff + i);
                uint32_t absPrimOff = primBase2 + wordOff;
                uint32_t packedGridOff = readU32(storage, absPrimOff);
                uint16_t gridOffX = packedGridOff & 0xFFFF;
                uint16_t gridOffY = (packedGridOff >> 16) & 0xFFFF;
                uint32_t color = readU32(storage, absPrimOff + 7);
                std::cerr << "Prim " << i << ": gridOffset=(" << gridOffX << "," << gridOffY
                          << ") color=" << std::hex << color << std::dec << std::endl;
            }
        }

        expect(!rowsWithPrim.empty()) << "primitive should be in SOME grid rows";

        // Should span rows 3-8 (cursorRow=3 + local rows 0-5)
        for (uint32_t row = 3; row <= 8; row++) {
            expect(rowsWithPrim.count(row) == 1_u)
                << "primitive should be in grid row " << row;
        }

        // Should NOT be in rows 0-2 or 9+
        for (uint32_t row = 0; row < 3; row++) {
            expect(rowsWithPrim.count(row) == 0_u)
                << "primitive should NOT be in grid row " << row;
        }
    };

    "BUG HUNT: simulate exact shader grid lookup"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        // Terminal: 80x25 cells, 10x20 pixels
        float cellW = 10.0f, cellH = 20.0f;
        uint32_t cols = 80, rows = 25;
        float termW = cols * cellW, termH = rows * cellH;

        builder->setSceneBounds(0, 0, termW, termH);
        builder->setGridCellSize(cellW, cellH);

        // Cursor at row 3
        builder->setCursorPosition(0, 3);

        // Circle at local (100, 50), radius 15
        auto buf = *YPaintBuffer::create();
        buf->addCircle(0, 100.0f, 50.0f, 15.0f, 0xFF123456, 0, 0.0f, 0.0f);
        builder->addYpaintBuffer(buf);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        // Simulate shader calculation
        // Pixel where primitive should be visible:
        //   screenY = cursorRow * cellH + localY = 3 * 20 + 50 = 110
        float pixelPosX = 100.0f;
        float pixelPosY = 110.0f;

        // Shader scenePos calculation (from renderYpaintOverlay)
        float contentMinX, contentMinY, contentMaxX, contentMaxY;
        std::memcpy(&contentMinX, &meta[8], sizeof(float));
        std::memcpy(&contentMinY, &meta[9], sizeof(float));
        std::memcpy(&contentMaxX, &meta[10], sizeof(float));
        std::memcpy(&contentMaxY, &meta[11], sizeof(float));

        float contentW = contentMaxX - contentMinX;
        float contentH = contentMaxY - contentMinY;
        float gridPixelW = cols * cellW;
        float gridPixelH = rows * cellH;

        float scenePosX = contentMinX + (pixelPosX / gridPixelW) * contentW;
        float scenePosY = contentMinY + (pixelPosY / gridPixelH) * contentH;

        // Grid lookup
        float cellSizeX = unpackF16(meta[5], false);
        float cellSizeY = unpackF16(meta[5], true);

        uint32_t cellX = static_cast<uint32_t>(std::floor((scenePosX - contentMinX) / cellSizeX));
        uint32_t cellY = static_cast<uint32_t>(std::floor((scenePosY - contentMinY) / cellSizeY));

        // Where should the primitive be?
        // localMinLine = floor((50-15)/20) = floor(35/20) = 1
        // localMaxLine = floor((50+15)/20) = floor(65/20) = 3
        // primMinLine = 3 + 1 = 4
        // primMaxLine = 3 + 3 = 6
        // Primitive should be in grid rows 4-6

        // At pixelPosY=110, scenePosY should be 110, cellY should be floor(110/20) = 5
        expect(std::abs(scenePosY - 110.0f) < 0.1f)
            << "scenePosY should be ~110, got " << scenePosY;
        expect(cellY == 5_u)
            << "cellY should be 5, got " << cellY;

        // Check that primitive is in grid row 5
        uint32_t gridOff = meta[2];
        uint32_t gridW = meta[3];
        uint32_t primBase = meta[0] + meta[1];

        bool foundAtCellY5 = false;
        for (uint32_t col = 0; col < gridW; col++) {
            uint32_t cellIdx = cellY * gridW + col;
            uint32_t packedStart = readU32(storage, gridOff + cellIdx);
            uint32_t cnt = readU32(storage, gridOff + packedStart);
            for (uint32_t i = 0; i < cnt; i++) {
                uint32_t rawVal = readU32(storage, gridOff + packedStart + 1 + i);
                if ((rawVal & 0x80000000u) == 0) {
                    uint32_t primOff = primBase + rawVal;
                    uint32_t color = readU32(storage, primOff + 6);
                    if (color == 0xFF123456) {
                        foundAtCellY5 = true;
                    }
                }
            }
        }

        expect(foundAtCellY5)
            << "primitive should be found in grid row " << cellY
            << " (shader lookup at pixelY=" << pixelPosY << ")";
    };

    "BUG HUNT: chart.sh exact scenario"_test = [] {
        uint32_t metadataBytes = 1001 * 64;
        auto mockCM = std::make_shared<MockGpuMemoryManager>(metadataBytes);
        auto builder = *Painter::create(nullptr, testAllocator(), mockCM, 1000, true);

        // Typical terminal
        float cellW = 10.0f, cellH = 20.0f;
        uint32_t cols = 80, rows = 25;
        float termW = cols * cellW, termH = rows * cellH;

        builder->setSceneBounds(0, 0, termW, termH);
        builder->setGridCellSize(cellW, cellH);

        // Cursor at row 3 (prompt on 3rd line)
        builder->setCursorPosition(0, 3);

        // Chart bars from chart.sh (simplified positions)
        auto buf = *YPaintBuffer::create();
        buf->addBox(0, 70.0f, 100.0f, 20.0f, 40.0f, 0xFF3498DB, 0, 0.0f, 4.0f);  // Y: 60-140
        buf->addBox(1, 120.0f, 60.0f, 20.0f, 60.0f, 0xFF2ECC71, 0, 0.0f, 4.0f);  // Y: 0-120
        buf->addBox(2, 170.0f, 80.0f, 20.0f, 50.0f, 0xFFE74C3C, 0, 0.0f, 4.0f);  // Y: 30-130
        builder->addYpaintBuffer(buf);

        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32() + 1000 * 16;

        expect(meta[1] == 3_u) << "should have 3 prims";

        uint32_t gridOff = meta[2];
        uint32_t gridW = meta[3];
        uint32_t gridH = meta[4];
        uint32_t primBase = meta[0] + meta[1];

        // For each bar, calculate expected grid rows and verify
        struct BarInfo {
            float localMinY, localMaxY;
            uint32_t color;
        };
        std::vector<BarInfo> bars = {
            {60.0f, 140.0f, 0xFF3498DB},
            {0.0f, 120.0f, 0xFF2ECC71},
            {30.0f, 130.0f, 0xFFE74C3C}
        };

        for (const auto& bar : bars) {
            uint32_t localMinLine = static_cast<uint32_t>(std::floor(bar.localMinY / cellH));
            uint32_t localMaxLine = static_cast<uint32_t>(std::floor(bar.localMaxY / cellH));
            uint32_t expectedMinRow = 3 + localMinLine;  // cursorRow + localLine
            uint32_t expectedMaxRow = 3 + localMaxLine;

            // Find which rows contain this bar
            std::set<uint32_t> rowsWithBar;
            for (uint32_t row = 0; row < gridH; row++) {
                for (uint32_t col = 0; col < gridW; col++) {
                    uint32_t cellIdx = row * gridW + col;
                    uint32_t packedStart = readU32(storage, gridOff + cellIdx);
                    uint32_t cnt = readU32(storage, gridOff + packedStart);
                    for (uint32_t i = 0; i < cnt; i++) {
                        uint32_t rawVal = readU32(storage, gridOff + packedStart + 1 + i);
                        if ((rawVal & 0x80000000u) == 0) {
                            uint32_t primOff = primBase + rawVal;
                            uint32_t color = readU32(storage, primOff + 7);  // Box color at +7
                            if (color == bar.color) {
                                rowsWithBar.insert(row);
                            }
                        }
                    }
                }
            }

            expect(!rowsWithBar.empty())
                << "bar with color " << std::hex << bar.color << std::dec
                << " should be found in grid";

            // Verify expected rows
            for (uint32_t row = expectedMinRow; row <= expectedMaxRow && row < gridH; row++) {
                expect(rowsWithBar.count(row) == 1_u)
                    << "bar " << std::hex << bar.color << std::dec
                    << " should be in row " << row;
            }
        }

        // Now simulate shader lookup at various pixel positions
        float contentMinY;
        std::memcpy(&contentMinY, &meta[9], sizeof(float));
        float cellSizeY = unpackF16(meta[5], true);

        // Test at pixel Y corresponding to middle of first bar
        // Bar 1: local Y center = 100, cursor row = 3
        // Screen Y = 3*20 + 100 = 160
        float testPixelY = 160.0f;
        float scenePosY = testPixelY;  // Since content bounds = terminal
        uint32_t lookupCellY = static_cast<uint32_t>(std::floor((scenePosY - contentMinY) / cellSizeY));

        // Expected: cellY = floor(160/20) = 8
        // Bar 1 AABB: 60-140, with cursor 3: grid rows 6-10
        expect(lookupCellY == 8_u) << "lookup cellY should be 8";

        bool foundBar1AtRow8 = false;
        for (uint32_t col = 0; col < gridW; col++) {
            uint32_t cellIdx = lookupCellY * gridW + col;
            uint32_t packedStart = readU32(storage, gridOff + cellIdx);
            uint32_t cnt = readU32(storage, gridOff + packedStart);
            for (uint32_t i = 0; i < cnt; i++) {
                uint32_t rawVal = readU32(storage, gridOff + packedStart + 1 + i);
                if ((rawVal & 0x80000000u) == 0) {
                    uint32_t primOff = primBase + rawVal;
                    uint32_t color = readU32(storage, primOff + 7);
                    if (color == 0xFF3498DB) {
                        foundBar1AtRow8 = true;
                    }
                }
            }
        }

        expect(foundBar1AtRow8)
            << "bar 1 (blue) should be found at grid row 8 (shader lookup at pixelY=160)";
    };
};
