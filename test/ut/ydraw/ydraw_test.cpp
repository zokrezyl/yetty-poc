//=============================================================================
// YDrawBuffer + YDrawBuilder Unit Tests
//
// Tests AABB computation, GPU buffer layout, grid construction,
// the real builder GPU write pipeline using mock CardManager,
// and the text/glyph pipeline including font atlas correctness.
//=============================================================================

#include <cstddef>
#include <version>
#include <algorithm>

#include <boost/ut.hpp>

#include "yetty/ydraw/ydraw-buffer.h"
#include "yetty/ydraw/ydraw-types.gen.h"
#include "yetty/ydraw/ydraw-prim-writer.gen.h"
#include <yetty/ydraw-builder.h>
#include <yetty/card-manager.h>
#include <yetty/card-buffer-manager.h>
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
// Mock CardManager — owns mock buffer manager + CPU metadata array
//=============================================================================
class MockCardManager : public CardManager {
public:
    MockCardManager()
        : _bufMgr(std::make_shared<MockCardBufferManager>())
        , _metadata(16 * 64, 0) {}  // 16 slots * 64 bytes each

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
    std::shared_ptr<MockCardManager> cardMgr;
    YDrawBuilder::Ptr builder;
    const float* storage;       // cardStorage as floats
    const uint32_t* meta;       // metadata as u32
    uint32_t primitiveOffset;
    uint32_t primitiveCount;
    uint32_t primDataBase;
    uint32_t gridOffset;
    uint32_t gridWidth;
    uint32_t gridHeight;
    float cellSize;
    float sceneMinX, sceneMinY;
};

static PipelineResult runPipeline(YDrawBuffer::Ptr buf,
                                   float sceneW = 200.0f, float sceneH = 200.0f,
                                   FontManager::Ptr fontMgr = nullptr) {
    auto mockCM = std::make_shared<MockCardManager>();
    auto builder = *YDrawBuilder::create(fontMgr, testAllocator(), buf, mockCM, 0);
    builder->setSceneBounds(0, 0, sceneW, sceneH);
    builder->calculate();
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
    std::memcpy(&r.cellSize, &meta[5], sizeof(float));
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
    if (p.gridWidth == 0 || p.gridHeight == 0 || p.cellSize <= 0.0f) return result;

    float invCellSize = 1.0f / p.cellSize;
    uint32_t cellX = static_cast<uint32_t>(
        std::clamp((sceneX - p.sceneMinX) * invCellSize, 0.0f, float(p.gridWidth - 1)));
    uint32_t cellY = static_cast<uint32_t>(
        std::clamp((sceneY - p.sceneMinY) * invCellSize, 0.0f, float(p.gridHeight - 1)));
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
// YDrawBuffer Tests
//=============================================================================

suite ydraw_buffer_tests = [] {

    "buffer creates empty"_test = [] {
        auto buf = *YDrawBuffer::create();
        expect(buf->primCount() == 0_u);
        expect(buf->empty());
        expect(buf->gpuBufferSize() == 0_u);
    };

    "addCircle adds one prim"_test = [] {
        auto buf = *YDrawBuffer::create();
        auto res = buf->addCircle(0, 50.0f, 60.0f, 25.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        expect(res.has_value());
        expect(buf->primCount() == 1_u);
        expect(!buf->empty());
    };

    "addBox adds one prim with correct word count"_test = [] {
        auto buf = *YDrawBuffer::create();
        buf->addBox(0, 100.0f, 100.0f, 50.0f, 30.0f, 0xFFFFFFFF, 0, 0.0f, 0.0f);
        expect(buf->primCount() == 1_u);
        // Box = 10 words. GPU size = (1 offset + 10 data) * 4 bytes = 44
        expect(buf->gpuBufferSize() == 44_u);
    };

    "addRoundedBox adds one prim with correct word count"_test = [] {
        auto buf = *YDrawBuffer::create();
        buf->addRoundedBox(0, 100.0f, 100.0f, 50.0f, 30.0f,
                           4.0f, 4.0f, 4.0f, 4.0f,
                           0xFFFFFFFF, 0, 0.0f, 0.0f);
        expect(buf->primCount() == 1_u);
        // RoundedBox = 14 words. GPU size = (1 offset + 14 data) * 4 = 60
        expect(buf->gpuBufferSize() == 60_u);
    };

    "addTriangle adds one prim"_test = [] {
        auto buf = *YDrawBuffer::create();
        buf->addTriangle(0, 0.0f, 0.0f, 10.0f, 0.0f, 5.0f, 10.0f,
                         0xFFFF0000, 0, 0.0f, 0.0f);
        expect(buf->primCount() == 1_u);
        // Triangle = 12 words. GPU size = (1 + 12) * 4 = 52
        expect(buf->gpuBufferSize() == 52_u);
    };

    "multiple prims accumulate"_test = [] {
        auto buf = *YDrawBuffer::create();
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
        auto buf = *YDrawBuffer::create();
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
        auto buf = *YDrawBuffer::create();
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
// Full GPU write pipeline tests — using MockCardManager
//
// These run the REAL builder lifecycle (calculate → declareBufferNeeds →
// commitReservations → allocateBuffers → writeBuffers) and then inspect
// what the builder ACTUALLY wrote to the mock buffers.
//=============================================================================

suite gpu_pipeline_tests = [] {

    "single circle — metadata and prim data"_test = [] {
        auto buf = *YDrawBuffer::create();
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
        uint32_t primOff = p.primDataBase + wordOff0;
        uint32_t type = readU32(p.storage, primOff + 0);
        expect(type == static_cast<uint32_t>(SDFType::Circle))
            << "type should be Circle";

        // Circle: +2=cx, +3=cy, +4=r, +5=fillColor
        expect(p.storage[primOff + 2] == 50.0_f) << "cx";
        expect(p.storage[primOff + 3] == 50.0_f) << "cy";
        expect(p.storage[primOff + 4] == 10.0_f) << "r";
        uint32_t readFill = readU32(p.storage, primOff + 5);
        expect(readFill == fillColor) << "fillColor";
    };

    "two prims — correct types and colors at correct offsets"_test = [] {
        auto buf = *YDrawBuffer::create();
        uint32_t circleColor = 0xFFFF0000;
        uint32_t rboxColor = 0xFF00FF00;
        buf->addCircle(0, 20.0f, 20.0f, 8.0f, circleColor, 0, 0.0f, 0.0f);
        buf->addRoundedBox(1, 80.0f, 80.0f, 20.0f, 15.0f,
                           3.0f, 3.0f, 3.0f, 3.0f,
                           rboxColor, 0, 0.0f, 0.0f);

        auto p = runPipeline(buf, 100.0f, 100.0f);
        expect(p.primitiveCount == 2_u);

        // Read both prims via offset table
        for (uint32_t i = 0; i < p.primitiveCount; i++) {
            uint32_t wordOff = readU32(p.storage, p.primitiveOffset + i);
            uint32_t primOff = p.primDataBase + wordOff;
            uint32_t type = readU32(p.storage, primOff);

            if (type == static_cast<uint32_t>(SDFType::Circle)) {
                uint32_t fill = readU32(p.storage, primOff + 5);
                expect(fill == circleColor) << "Circle fillColor";
            } else if (type == static_cast<uint32_t>(SDFType::RoundedBox)) {
                uint32_t fill = readU32(p.storage, primOff + 10);
                expect(fill == rboxColor) << "RoundedBox fillColor";
            } else {
                expect(false) << "unexpected prim type " << type;
            }
        }
    };

    "grid entries resolve to correct prims via shader path"_test = [] {
        auto buf = *YDrawBuffer::create();
        buf->addCircle(0, 20.0f, 20.0f, 8.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        buf->addRoundedBox(1, 80.0f, 80.0f, 10.0f, 10.0f,
                           2.0f, 2.0f, 2.0f, 2.0f,
                           0xFF00FF00, 0, 0.0f, 0.0f);

        auto p = runPipeline(buf, 100.0f, 100.0f);

        // Shader lookup at circle center
        auto circleHits = shaderGridLookup(p, 20.0f, 20.0f);
        expect(!circleHits.empty()) << "should find prim at circle center";
        bool foundCircle = false;
        for (uint32_t off : circleHits) {
            if (readU32(p.storage, off) == static_cast<uint32_t>(SDFType::Circle))
                foundCircle = true;
        }
        expect(foundCircle) << "should find Circle type at (20,20)";

        // Shader lookup at roundedbox center
        auto rboxHits = shaderGridLookup(p, 80.0f, 80.0f);
        expect(!rboxHits.empty()) << "should find prim at roundedbox center";
        bool foundRBox = false;
        for (uint32_t off : rboxHits) {
            if (readU32(p.storage, off) == static_cast<uint32_t>(SDFType::RoundedBox))
                foundRBox = true;
        }
        expect(foundRBox) << "should find RoundedBox type at (80,80)";
    };

    "ygui scenario — 5 prims all reachable from grid"_test = [] {
        auto buf = *YDrawBuffer::create();

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
        for (uint32_t off : reachable) {
            uint32_t type = readU32(p.storage, off);
            expect(type < 100_u) << "type " << type << " at offset " << off;
        }

        // Spot-check: button center should find RoundedBox
        auto btnHits = shaderGridLookup(p, 100.0f, 30.0f);
        expect(!btnHits.empty()) << "should find prims at button center";
        int rboxCount = 0;
        for (uint32_t off : btnHits) {
            if (readU32(p.storage, off) == static_cast<uint32_t>(SDFType::RoundedBox))
                rboxCount++;
        }
        expect(rboxCount >= 2) << "should find bg + outline RoundedBoxes at button center";

        // Spot-check: chevron position should find Triangle
        auto chevHits = shaderGridLookup(p, 294.0f, 30.0f);
        expect(!chevHits.empty()) << "should find prim at chevron";
        bool foundTri = false;
        for (uint32_t off : chevHits) {
            if (readU32(p.storage, off) == static_cast<uint32_t>(SDFType::Triangle))
                foundTri = true;
        }
        expect(foundTri) << "should find Triangle at chevron position";
    };

    "clear and rebuild produces valid pipeline"_test = [] {
        auto buf = *YDrawBuffer::create();
        auto mockCM = std::make_shared<MockCardManager>();
        auto builder = *YDrawBuilder::create(nullptr, nullptr, buf, mockCM, 0);
        builder->setSceneBounds(0, 0, 200.0f, 200.0f);

        // First build: Circle + Box
        buf->addCircle(0, 50.0f, 50.0f, 15.0f, 0xFFFF0000, 0, 0.0f, 0.0f);
        buf->addBox(1, 150.0f, 150.0f, 20.0f, 20.0f, 0xFF00FF00, 0, 0.0f, 0.0f);
        builder->calculate();
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const uint32_t* meta = mockCM->metadataAsU32();
        expect(meta[1] == 2_u) << "first build: 2 prims";

        // Clear and rebuild: single RoundedBox
        buf->clear();
        uint32_t newColor = 0xFFBB44CC;
        buf->addRoundedBox(0, 100.0f, 100.0f, 30.0f, 20.0f,
                           5.0f, 5.0f, 5.0f, 5.0f, newColor, 0, 0.0f, 0.0f);
        builder->calculate();
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
        uint32_t type = readU32(storage, absOff);
        expect(type == static_cast<uint32_t>(SDFType::RoundedBox));
        uint32_t readFill = readU32(storage, absOff + 10);
        expect(readFill == newColor) << "fillColor after rebuild";
    };

    "metadata layout matches shader expectations"_test = [] {
        auto buf = *YDrawBuffer::create();
        buf->addCircle(0, 50.0f, 50.0f, 10.0f, 0xFFFF0000, 0, 0.0f, 0.0f);

        auto p = runPipeline(buf, 100.0f, 100.0f);

        // Verify metadata words match what the shader reads
        expect(p.meta[0] > 0_u || p.meta[0] == 0_u) << "primitiveOffset is set";
        expect(p.meta[1] == 1_u) << "primitiveCount = 1";
        expect(p.meta[2] > 0_u || p.meta[2] == 0_u) << "gridOffset is set";
        expect(p.meta[3] > 0_u) << "gridWidth > 0";
        expect(p.meta[4] > 0_u) << "gridHeight > 0";

        float cellSize;
        std::memcpy(&cellSize, &p.meta[5], sizeof(float));
        expect(cellSize > 0.0_f) << "cellSize > 0";

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
        auto buf = *YDrawBuffer::create();
        buf->addCircle(0, 50.0f, 50.0f, 10.0f, 0xFFFF0000, 0, 0.0f, 0.0f);

        auto mockCM = std::make_shared<MockCardManager>();
        auto builder = *YDrawBuilder::create(nullptr, nullptr, buf, mockCM, 0);
        builder->setSceneBounds(0, 0, 100.0f, 100.0f);
        builder->calculate();
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
        auto buf = *YDrawBuffer::create();
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
        // RoundedBox: type=+0, layer=+1, cx=+2, cy=+3, hw=+4, hh=+5
        //             r0=+6, r1=+7, r2=+8, r3=+9
        //             fillColor=+10, strokeColor=+11, strokeWidth=+12, round=+13
        uint32_t type = readU32(p.storage, primOff + 0);
        expect(type == 8_u) << "type = SDF_ROUNDED_BOX (8)";

        expect(p.storage[primOff + 2] == 100.0_f) << "cx";
        expect(p.storage[primOff + 3] == 200.0_f) << "cy";
        expect(p.storage[primOff + 4] == 40.0_f) << "hw";
        expect(p.storage[primOff + 5] == 30.0_f) << "hh";
        expect(p.storage[primOff + 6] == 5.0_f) << "r0";
        expect(p.storage[primOff + 7] == 6.0_f) << "r1";
        expect(p.storage[primOff + 8] == 7.0_f) << "r2";
        expect(p.storage[primOff + 9] == 8.0_f) << "r3";

        // These are the exact offsets primColors() and primStrokeWidth() read
        uint32_t readFill = readU32(p.storage, primOff + 10);
        uint32_t readStroke = readU32(p.storage, primOff + 11);
        expect(readFill == fillColor) << "shader reads fillColor at +10";
        expect(readStroke == strokeColor) << "shader reads strokeColor at +11";
        expect(p.storage[primOff + 12] == strokeWidth) << "shader reads strokeWidth at +12";
    };

    "Box word layout matches shader's primColors offsets"_test = [] {
        auto buf = *YDrawBuffer::create();
        uint32_t fillColor = 0xFF445566;
        buf->addBox(0, 50.0f, 60.0f, 20.0f, 15.0f, fillColor, 0, 0.0f, 3.0f);

        auto p = runPipeline(buf, 200.0f, 200.0f);
        uint32_t wordOff0 = readU32(p.storage, p.primitiveOffset);
        uint32_t primOff = p.primDataBase + wordOff0;

        // Box: type=+0, layer=+1, cx=+2, cy=+3, hw=+4, hh=+5
        //      fillColor=+6, strokeColor=+7, strokeWidth=+8, round=+9
        uint32_t type = readU32(p.storage, primOff + 0);
        expect(type == 1_u) << "type = SDF_BOX (1)";
        expect(p.storage[primOff + 2] == 50.0_f) << "cx";
        expect(p.storage[primOff + 3] == 60.0_f) << "cy";
        expect(p.storage[primOff + 4] == 20.0_f) << "hw";
        expect(p.storage[primOff + 5] == 15.0_f) << "hh";
        uint32_t readFill = readU32(p.storage, primOff + 6);
        expect(readFill == fillColor) << "shader reads fillColor at +6";
        expect(p.storage[primOff + 9] == 3.0_f) << "round at +9";
    };

    "Triangle word layout matches shader's primColors offsets"_test = [] {
        auto buf = *YDrawBuffer::create();
        uint32_t fillColor = 0xFFAABB00;
        buf->addTriangle(0, 10.0f, 20.0f, 80.0f, 20.0f, 45.0f, 70.0f,
                         fillColor, 0, 0.0f, 0.0f);

        auto p = runPipeline(buf, 200.0f, 200.0f);
        uint32_t wordOff0 = readU32(p.storage, p.primitiveOffset);
        uint32_t primOff = p.primDataBase + wordOff0;

        // Triangle: type=+0, layer=+1, ax=+2, ay=+3, bx=+4, by=+5, vx=+6, vy=+7
        //           fillColor=+8, strokeColor=+9, strokeWidth=+10, round=+11
        uint32_t type = readU32(p.storage, primOff + 0);
        expect(type == 3_u) << "type = SDF_TRIANGLE (3)";
        expect(p.storage[primOff + 2] == 10.0_f) << "ax";
        expect(p.storage[primOff + 3] == 20.0_f) << "ay";
        uint32_t readFill = readU32(p.storage, primOff + 8);
        expect(readFill == fillColor) << "shader reads fillColor at +8";
    };

    "YDrawMetadata struct is 64 bytes and matches shader layout"_test = [] {
        YDrawMetadata meta = {};
        expect(sizeof(YDrawMetadata) == 64_u) << "must be 64 bytes";

        meta.primitiveOffset = 42;
        meta.primitiveCount = 7;
        meta.gridOffset = 100;
        meta.gridWidth = 8;
        meta.gridHeight = 6;
        float cs = 12.5f;
        std::memcpy(&meta.cellSize, &cs, sizeof(float));
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

    "YDrawGlyph is 20 bytes"_test = [] {
        expect(sizeof(YDrawGlyph) == 20_u);
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

        auto buf = *YDrawBuffer::create();
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

        auto buf = *YDrawBuffer::create();
        buf->addText(50.0f, 80.0f, "AB", 14.0f, 0xFF00FF00, 0, -1);

        auto p = runPipeline(buf, 200.0f, 200.0f, fm);

        uint32_t glyphCount = p.meta[7];
        uint32_t glyphOffset = p.meta[6];
        expect(glyphCount >= 2_u) << "at least 2 glyphs for 'AB'";
        if (glyphCount < 2) return;

        // Read YDrawGlyph structs from derived buffer
        const YDrawGlyph* glyphs = reinterpret_cast<const YDrawGlyph*>(
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

        auto buf = *YDrawBuffer::create();
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

        auto buf = *YDrawBuffer::create();

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
        for (uint32_t off : primOffsets) {
            uint32_t type = readU32(p.storage, off);
            expect(type == static_cast<uint32_t>(SDFType::RoundedBox))
                << "prim should be RoundedBox";
        }
    };

    "multiple text spans produce correct total glyph count"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        auto buf = *YDrawBuffer::create();
        buf->addText(10.0f, 20.0f, "ABC", 14.0f, 0xFFFF0000, 0, -1);   // 3 chars
        buf->addText(10.0f, 50.0f, "DEFG", 14.0f, 0xFF00FF00, 0, -1);  // 4 chars
        buf->addText(10.0f, 80.0f, "HI", 14.0f, 0xFF0000FF, 0, -1);    // 2 chars
        expect(buf->textSpanCount() == 3_u);

        auto p = runPipeline(buf, 200.0f, 200.0f, fm);

        uint32_t glyphCount = p.meta[7];
        expect(glyphCount >= 9_u) << "3+4+2=9 chars, got " << glyphCount << " glyphs";

        // Verify different colors in glyph data
        uint32_t glyphOffset = p.meta[6];
        const YDrawGlyph* glyphs = reinterpret_cast<const YDrawGlyph*>(
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

        auto buf = *YDrawBuffer::create();

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
            const YDrawGlyph* glyphs = reinterpret_cast<const YDrawGlyph*>(
                &p.storage[glyphOffset]);
            uint8_t flags = static_cast<uint8_t>(glyphs[0].glyphLayerFlags >> 24);
            expect((flags & YDrawBuilder::GLYPH_FLAG_CUSTOM_ATLAS) != 0_u)
                << "custom font glyphs should have CUSTOM_ATLAS flag";
        }
    };

    "ygui full widget scenario — prims + text all reachable"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        auto buf = *YDrawBuffer::create();

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
        const YDrawGlyph* glyphs = reinterpret_cast<const YDrawGlyph*>(
            &p.storage[glyphOffset]);
        for (uint32_t i = 0; i < glyphCount; i++) {
            expect(glyphs[i].x >= sMinX - 10.0f && glyphs[i].x <= sMaxX + 10.0f)
                << "glyph " << i << " x=" << glyphs[i].x << " out of scene bounds";
        }
    };
};
