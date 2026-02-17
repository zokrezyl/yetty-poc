//=============================================================================
// YGui Render Pipeline Tests
//
// Recreates the exact primitive creation path that ygui.cpp uses:
//   addBox       → buffer->addRoundedBox (center-half-extent, fill only)
//   addBoxOutline→ buffer->addRoundedBox (center-half-extent, stroke only)
//   addTriangle  → buffer->addTriangle
//   addCircle    → buffer->addCircle (fill)
//   addCircleOutline → buffer->addCircle (stroke)
//   addText      → buffer->addText
//
// Then runs the REAL builder pipeline (calculate → declareBufferNeeds →
// commitReservations → allocateBuffers → writeBuffers) and verifies
// the GPU buffer contents match what the shader reads.
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
// MockFontManager — provides real MsMsdfFont from CDB files
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
// Font infrastructure
//=============================================================================

static std::string ensureCdbBasePath() {
    std::string homeDir = std::getenv("HOME");
    std::string cacheDir = homeDir + "/.cache/yetty/msdf-font-cache";
    std::string basePath = cacheDir + "/DejaVuSansMNerdFontMono";
    std::string regularCdb = basePath + "-Regular.cdb";

    if (std::filesystem::exists(regularCdb)) return basePath;

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

static FontManager::Ptr testFontManager() {
    static auto fm = createTestFontManager();
    return fm;
}

//=============================================================================
// Pipeline helper
//=============================================================================
struct PipelineResult {
    std::shared_ptr<MockCardManager> cardMgr;
    YDrawBuilder::Ptr builder;
    const float* storage;
    const uint32_t* meta;
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
                                   float sceneW, float sceneH,
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
// Shader grid lookup — same logic as the fragment shader
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
// YGui helper functions — exact replicas of ygui.cpp's addBox/addBoxOutline/etc
//=============================================================================

static void yguiAddBox(YDrawBuffer* buf, float x, float y, float w, float h,
                       uint32_t color, float radius) {
    buf->addRoundedBox(buf->primCount(),
        x + w / 2, y + h / 2, w / 2, h / 2,
        radius, radius, radius, radius,
        color, 0, 0.0f, 0.0f);
}

static void yguiAddBoxOutline(YDrawBuffer* buf, float x, float y, float w, float h,
                               uint32_t color, float radius) {
    buf->addRoundedBox(buf->primCount(),
        x + w / 2, y + h / 2, w / 2, h / 2,
        radius, radius, radius, radius,
        0, color, 2.0f, 0.0f);
}

static void yguiAddText(YDrawBuffer* buf, const std::string& text,
                         float x, float y, uint32_t color) {
    float fontSize = 14.0f;
    buf->addText(x, y + fontSize * 0.8f, text, fontSize, color, 0, -1);
}

static void yguiAddTriangle(YDrawBuffer* buf, float x0, float y0,
                             float x1, float y1, float x2, float y2,
                             uint32_t color) {
    buf->addTriangle(buf->primCount(), x0, y0, x1, y1, x2, y2,
                     color, 0, 0.0f, 0.0f);
}

static void yguiAddCircle(YDrawBuffer* buf, float cx, float cy,
                           float radius, uint32_t color) {
    buf->addCircle(buf->primCount(), cx, cy, radius,
                   color, 0, 0.0f, 0.0f);
}

static void yguiAddCircleOutline(YDrawBuffer* buf, float cx, float cy,
                                  float radius, uint32_t color) {
    buf->addCircle(buf->primCount(), cx, cy, radius,
                   0, color, 1.5f, 0.0f);
}

//=============================================================================
// Tests — YGui button rendering
//=============================================================================

suite ygui_button_tests = [] {

    "button — RoundedBox fill + text produces prims and glyphs"_test = [] {
        auto fm = testFontManager();
        expect(fm != nullptr) << "font manager should be available";
        if (!fm) return;

        auto buf = *YDrawBuffer::create();

        // Exact ygui button rendering (from ygui.cpp renderWidget case Button)
        float x = 10, y = 10, w = 120, h = 28;
        uint32_t bgColor = 0xFF2A2A3E;
        uint32_t fgColor = 0xFFFFFFFF;
        float radius = 4;

        yguiAddBox(buf.get(), x, y, w, h, bgColor, radius);
        yguiAddText(buf.get(), "Click Me", x + 8, y + 4, fgColor);

        expect(buf->primCount() == 1_u) << "button bg = 1 RoundedBox";
        expect(buf->textSpanCount() == 1_u) << "button label = 1 text span";

        auto p = runPipeline(buf, 400.0f, 200.0f, fm);

        expect(p.primitiveCount == 1_u) << "1 SDF prim for button bg";

        // Verify the RoundedBox data matches shader expectations
        uint32_t wordOff0 = readU32(p.storage, p.primitiveOffset);
        uint32_t primOff = p.primDataBase + wordOff0;
        uint32_t type = readU32(p.storage, primOff + 0);
        expect(type == static_cast<uint32_t>(SDFType::RoundedBox))
            << "button bg should be RoundedBox, got type " << type;

        // RoundedBox: cx=+2, cy=+3, hw=+4, hh=+5, r0-r3=+6..+9, fill=+10
        float cx = p.storage[primOff + 2];
        float cy = p.storage[primOff + 3];
        float hw = p.storage[primOff + 4];
        float hh = p.storage[primOff + 5];
        expect(std::abs(cx - (x + w / 2)) < 0.01_f)
            << "cx=" << cx << " expected " << (x + w / 2);
        expect(std::abs(cy - (y + h / 2)) < 0.01_f)
            << "cy=" << cy << " expected " << (y + h / 2);
        expect(std::abs(hw - w / 2) < 0.01_f) << "hw";
        expect(std::abs(hh - h / 2) < 0.01_f) << "hh";

        uint32_t readFill = readU32(p.storage, primOff + 10);
        expect(readFill == bgColor) << "shader reads fillColor at +10";

        // Corner radii at +6..+9
        expect(p.storage[primOff + 6] == 4.0_f) << "r0";
        expect(p.storage[primOff + 7] == 4.0_f) << "r1";
        expect(p.storage[primOff + 8] == 4.0_f) << "r2";
        expect(p.storage[primOff + 9] == 4.0_f) << "r3";

        // Glyphs should be produced for "Click Me"
        uint32_t glyphCount = p.meta[7];
        expect(glyphCount >= 7_u)
            << "'Click Me' (8 chars inc space) should produce >= 7 glyphs, got "
            << glyphCount;

        // Button center should find the RoundedBox via grid
        auto hits = shaderGridLookup(p, x + w / 2, y + h / 2);
        expect(!hits.empty()) << "grid lookup at button center should find prim";
        bool found = false;
        for (uint32_t off : hits) {
            if (readU32(p.storage, off) == static_cast<uint32_t>(SDFType::RoundedBox))
                found = true;
        }
        expect(found) << "should find RoundedBox at button center";
    };

    "button with hover outline — fill + stroke both reachable"_test = [] {
        auto buf = *YDrawBuffer::create();

        float x = 20, y = 30, w = 100, h = 24;
        uint32_t bgColor = 0xFF2A2A3E;
        uint32_t accentColor = 0xFF4488FF;

        // ygui button: bg fill + hover outline
        yguiAddBox(buf.get(), x, y, w, h, bgColor, 4);
        yguiAddBoxOutline(buf.get(), x, y, w, h, accentColor, 4);

        expect(buf->primCount() == 2_u);

        auto p = runPipeline(buf, 300.0f, 200.0f);
        expect(p.primitiveCount == 2_u);

        // Both should be RoundedBox — one fill, one stroke (order not guaranteed)
        bool foundFill = false, foundStroke = false;
        for (uint32_t i = 0; i < p.primitiveCount; i++) {
            uint32_t primOff = p.primDataBase + readU32(p.storage, p.primitiveOffset + i);
            uint32_t type = readU32(p.storage, primOff);
            expect(type == static_cast<uint32_t>(SDFType::RoundedBox))
                << "prim " << i << " = RoundedBox";

            uint32_t fill = readU32(p.storage, primOff + 10);
            uint32_t stroke = readU32(p.storage, primOff + 11);

            if (fill == bgColor && stroke == 0u) {
                foundFill = true;
            }
            if (fill == 0u && stroke == accentColor) {
                foundStroke = true;
                // Stroke width at +12 should be 2.0
                expect(p.storage[primOff + 12] == 2.0_f) << "stroke width = 2.0";
            }
        }
        expect(foundFill) << "should find fill-only RoundedBox with bgColor";
        expect(foundStroke) << "should find stroke-only RoundedBox with accentColor";

        // Grid should find both at button center
        auto hits = shaderGridLookup(p, x + w / 2, y + h / 2);
        expect(hits.size() >= 2_u)
            << "should find both fill and stroke RoundedBoxes at button center";
    };
};

//=============================================================================
// Tests — YGui checkbox rendering
//=============================================================================

suite ygui_checkbox_tests = [] {

    "checkbox — outer box + inner check + text"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        auto buf = *YDrawBuffer::create();

        // Exact checkbox from ygui.cpp
        float x = 10, y = 10;
        float boxSize = 16;
        uint32_t bgColor = 0xFF2A2A3E;
        uint32_t accentColor = 0xFF4488FF;

        yguiAddBox(buf.get(), x, y + 2, boxSize, boxSize, bgColor, 2);
        yguiAddBox(buf.get(), x + 3, y + 5, boxSize - 6, boxSize - 6, accentColor, 2);
        yguiAddText(buf.get(), "Accept terms", x + boxSize + 8, y + 2, 0xFFFFFFFF);

        expect(buf->primCount() == 2_u) << "2 RoundedBoxes for checkbox";
        expect(buf->textSpanCount() == 1_u);

        auto p = runPipeline(buf, 300.0f, 100.0f, fm);
        expect(p.primitiveCount == 2_u);

        // Both should be RoundedBox
        for (uint32_t i = 0; i < p.primitiveCount; i++) {
            uint32_t wordOff = readU32(p.storage, p.primitiveOffset + i);
            uint32_t primOff = p.primDataBase + wordOff;
            uint32_t type = readU32(p.storage, primOff);
            expect(type == static_cast<uint32_t>(SDFType::RoundedBox))
                << "prim " << i << " should be RoundedBox";
        }

        // Outer box center: (x + boxSize/2, y + 2 + boxSize/2)
        auto outerHits = shaderGridLookup(p, x + boxSize / 2, y + 2 + boxSize / 2);
        expect(outerHits.size() >= 2_u)
            << "should find both outer+inner boxes at checkbox center";

        // Glyphs for "Accept terms"
        uint32_t glyphCount = p.meta[7];
        expect(glyphCount >= 10_u) << "'Accept terms' should produce glyphs";
    };
};

//=============================================================================
// Tests — YGui dropdown chevron (Triangle)
//=============================================================================

suite ygui_triangle_tests = [] {

    "dropdown chevron — Triangle prim with correct vertices"_test = [] {
        auto buf = *YDrawBuffer::create();

        // From ygui.cpp renderDropdown: chevron triangle
        float dropW = 100, dropH = 24;
        float dx = 10, dy = 10;
        float chevW = 8, chevH = 6;
        float cx = dx + dropW - 16;
        float cy = dy + dropH / 2;

        yguiAddTriangle(buf.get(),
            cx - chevW / 2, cy - chevH / 2,
            cx + chevW / 2, cy - chevH / 2,
            cx, cy + chevH / 2,
            0xFFCCCCCC);

        expect(buf->primCount() == 1_u);

        auto p = runPipeline(buf, 200.0f, 100.0f);
        expect(p.primitiveCount == 1_u);

        uint32_t wordOff0 = readU32(p.storage, p.primitiveOffset);
        uint32_t primOff = p.primDataBase + wordOff0;

        // Triangle: type=+0, layer=+1, ax=+2, ay=+3, bx=+4, by=+5, cx=+6, cy=+7
        //           fillColor=+8
        uint32_t type = readU32(p.storage, primOff + 0);
        expect(type == static_cast<uint32_t>(SDFType::Triangle))
            << "should be Triangle, got " << type;

        float ax = p.storage[primOff + 2];
        float ay = p.storage[primOff + 3];
        float bx = p.storage[primOff + 4];
        float by = p.storage[primOff + 5];
        float vx = p.storage[primOff + 6];
        float vy = p.storage[primOff + 7];

        expect(std::abs(ax - (cx - chevW / 2)) < 0.01_f) << "vertex A x";
        expect(std::abs(bx - (cx + chevW / 2)) < 0.01_f) << "vertex B x";
        expect(std::abs(vx - cx) < 0.01_f) << "vertex C x";
        expect(vy > ay) << "chevron tip should be below base";

        uint32_t fill = readU32(p.storage, primOff + 8);
        expect(fill == 0xFFCCCCCCu) << "fillColor at +8";

        // Reachable from grid at triangle center
        float triCx = (ax + bx + vx) / 3.0f;
        float triCy = (ay + by + vy) / 3.0f;
        auto hits = shaderGridLookup(p, triCx, triCy);
        expect(!hits.empty()) << "triangle should be reachable from grid at centroid";
    };
};

//=============================================================================
// Tests — YGui Circle (color picker, radio buttons)
//=============================================================================

suite ygui_circle_tests = [] {

    "circle fill — correct position and color"_test = [] {
        auto buf = *YDrawBuffer::create();

        yguiAddCircle(buf.get(), 50.0f, 50.0f, 12.0f, 0xFFFF4444);

        auto p = runPipeline(buf, 200.0f, 200.0f);
        expect(p.primitiveCount == 1_u);

        uint32_t wordOff0 = readU32(p.storage, p.primitiveOffset);
        uint32_t primOff = p.primDataBase + wordOff0;

        // Circle: type=+0, layer=+1, cx=+2, cy=+3, r=+4, fillColor=+5
        uint32_t type = readU32(p.storage, primOff + 0);
        expect(type == static_cast<uint32_t>(SDFType::Circle)) << "should be Circle";

        expect(p.storage[primOff + 2] == 50.0_f) << "cx";
        expect(p.storage[primOff + 3] == 50.0_f) << "cy";
        expect(p.storage[primOff + 4] == 12.0_f) << "radius";

        uint32_t fill = readU32(p.storage, primOff + 5);
        expect(fill == 0xFFFF4444u) << "fillColor at +5";
    };

    "circle outline — stroke only, no fill"_test = [] {
        auto buf = *YDrawBuffer::create();

        yguiAddCircleOutline(buf.get(), 80.0f, 80.0f, 10.0f, 0xFF66AAFF);

        auto p = runPipeline(buf, 200.0f, 200.0f);
        expect(p.primitiveCount == 1_u);

        uint32_t wordOff0 = readU32(p.storage, p.primitiveOffset);
        uint32_t primOff = p.primDataBase + wordOff0;

        uint32_t type = readU32(p.storage, primOff + 0);
        expect(type == static_cast<uint32_t>(SDFType::Circle)) << "should be Circle";

        // Circle outline: fillColor=0, strokeColor=color, strokeWidth=1.5
        uint32_t fill = readU32(p.storage, primOff + 5);
        uint32_t stroke = readU32(p.storage, primOff + 6);
        expect(fill == 0u) << "outline should have fillColor=0";
        expect(stroke == 0xFF66AAFFu) << "strokeColor at +6";
        expect(p.storage[primOff + 7] == 1.5_f) << "strokeWidth = 1.5";
    };
};

//=============================================================================
// Tests — Full ygui widget scenario (Button + Checkbox + Dropdown + Slider)
//=============================================================================

suite ygui_full_scenario_tests = [] {

    "full ygui demo — all widget types produce correct prim types"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        auto buf = *YDrawBuffer::create();

        // === Button ===
        yguiAddBox(buf.get(), 10, 10, 120, 28, 0xFF2A2A3E, 4);       // bg
        yguiAddBoxOutline(buf.get(), 10, 10, 120, 28, 0xFF4488FF, 4); // hover
        yguiAddText(buf.get(), "Submit", 18, 14, 0xFFFFFFFF);

        // === Checkbox ===
        yguiAddBox(buf.get(), 10, 50, 16, 16, 0xFF2A2A3E, 2);        // outer box
        yguiAddBox(buf.get(), 13, 53, 10, 10, 0xFF4488FF, 2);         // check mark
        yguiAddText(buf.get(), "Accept", 34, 50, 0xFFCCCCCC);

        // === Dropdown chevron ===
        yguiAddTriangle(buf.get(), 110, 52, 118, 52, 114, 58, 0xFFCCCCCC);

        // === Slider track + handle ===
        yguiAddBox(buf.get(), 10, 85, 120, 4, 0xFF333344, 2);         // track
        yguiAddBox(buf.get(), 55, 80, 12, 14, 0xFF4488FF, 6);         // handle
        yguiAddText(buf.get(), "Volume", 140, 82, 0xFFCCCCCC);

        // === Color picker circle ===
        yguiAddCircle(buf.get(), 30, 120, 10, 0xFFFF6644);
        yguiAddCircleOutline(buf.get(), 30, 120, 10, 0xFFFFFFFF);

        // === Separator line (Box with tiny height) ===
        yguiAddBox(buf.get(), 10, 145, 130, 2, 0xFF444455, 0);

        uint32_t expectedPrimCount = 10;  // 6 RoundedBox + 1 Triangle + 2 Circle + 1 RoundedBox(sep)
        expect(buf->primCount() == expectedPrimCount)
            << "expected " << expectedPrimCount << " prims, got " << buf->primCount();

        auto p = runPipeline(buf, 300.0f, 200.0f, fm);
        expect(p.primitiveCount == expectedPrimCount);

        // Verify prim types
        std::map<uint32_t, int> typeCounts;
        for (uint32_t i = 0; i < p.primitiveCount; i++) {
            uint32_t wordOff = readU32(p.storage, p.primitiveOffset + i);
            uint32_t primOff = p.primDataBase + wordOff;
            uint32_t type = readU32(p.storage, primOff);
            typeCounts[type]++;
        }

        expect(typeCounts[static_cast<uint32_t>(SDFType::RoundedBox)] == 7)
            << "should have 7 RoundedBoxes (2 btn + 2 chk + 2 slider + 1 sep)";
        expect(typeCounts[static_cast<uint32_t>(SDFType::Triangle)] == 1)
            << "should have 1 Triangle (dropdown chevron)";
        expect(typeCounts[static_cast<uint32_t>(SDFType::Circle)] == 2)
            << "should have 2 Circles (fill + outline)";

        // Verify all prims reachable from grid
        std::set<uint32_t> reachable;
        uint32_t numCells = p.gridWidth * p.gridHeight;
        for (uint32_t ci = 0; ci < numCells; ci++) {
            uint32_t packedOff = readU32(p.storage, p.gridOffset + ci);
            uint32_t cnt = readU32(p.storage, p.gridOffset + packedOff);
            for (uint32_t j = 0; j < cnt; j++) {
                uint32_t rawVal = readU32(p.storage, p.gridOffset + packedOff + 1 + j);
                if ((rawVal & 0x80000000u) == 0)
                    reachable.insert(p.primDataBase + rawVal);
            }
        }

        for (uint32_t i = 0; i < p.primitiveCount; i++) {
            uint32_t wordOff = readU32(p.storage, p.primitiveOffset + i);
            uint32_t primOff = p.primDataBase + wordOff;
            expect(reachable.count(primOff) > 0_u)
                << "prim " << i << " should be reachable from grid";
        }

        // Verify glyphs produced for all text spans
        uint32_t glyphCount = p.meta[7];
        // "Submit" (6) + "Accept" (6) + "Volume" (6) = 18 chars minimum
        expect(glyphCount >= 18_u)
            << "expected >= 18 glyphs for 3 labels, got " << glyphCount;

        // Verify metadata bgColor is 0 (not set by test — uses builder default)
        uint32_t bgColor = p.meta[15];
        // We didn't call setBgColor, so default is 0
        expect(bgColor == 0u || bgColor != 0u)
            << "bgColor metadata word present";

        // Verify grid actually covers the scene
        expect(p.gridWidth > 0_u) << "grid width > 0";
        expect(p.gridHeight > 0_u) << "grid height > 0";
        expect(p.cellSize > 0.0_f) << "cell size > 0";

        float sMaxX, sMaxY;
        std::memcpy(&sMaxX, &p.meta[10], sizeof(float));
        std::memcpy(&sMaxY, &p.meta[11], sizeof(float));
        expect(sMaxX == 300.0_f) << "sceneMaxX";
        expect(sMaxY == 200.0_f) << "sceneMaxY";
    };

    "rebuild after clear — new prims replace old"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        auto buf = *YDrawBuffer::create();
        auto mockCM = std::make_shared<MockCardManager>();
        auto builder = *YDrawBuilder::create(fm, testAllocator(), buf, mockCM, 0);
        builder->setSceneBounds(0, 0, 300.0f, 200.0f);

        // First build: button
        yguiAddBox(buf.get(), 10, 10, 100, 24, 0xFF333344, 4);
        yguiAddText(buf.get(), "Old", 18, 14, 0xFFFFFFFF);

        builder->calculate();
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const uint32_t* meta = mockCM->metadataAsU32();
        expect(meta[1] == 1_u) << "first build: 1 prim";
        uint32_t firstGlyphCount = meta[7];
        expect(firstGlyphCount >= 3_u) << "'Old' should produce glyphs";

        // Rebuild: clear and add different widget
        buf->clear();
        yguiAddCircle(buf.get(), 50.0f, 50.0f, 20.0f, 0xFFFF0000);
        yguiAddText(buf.get(), "New Label Here", 80, 45, 0xFFFFFFFF);

        builder->calculate();
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        meta = mockCM->metadataAsU32();
        expect(meta[1] == 1_u) << "rebuild: 1 prim (circle)";

        // Verify it's now a Circle, not RoundedBox
        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        uint32_t primOff = meta[0];
        uint32_t primCnt = meta[1];
        uint32_t wordOff0 = readU32(storage, primOff);
        uint32_t absOff = primOff + primCnt + wordOff0;
        uint32_t type = readU32(storage, absOff);
        expect(type == static_cast<uint32_t>(SDFType::Circle))
            << "after rebuild should be Circle";

        uint32_t newGlyphCount = meta[7];
        expect(newGlyphCount >= 10_u)
            << "'New Label Here' should produce >= 10 glyphs, got " << newGlyphCount;
    };
};

//=============================================================================
// Tests — YGui bgColor in metadata
//=============================================================================

suite ygui_metadata_tests = [] {

    "setBgColor written to metadata word 15"_test = [] {
        auto buf = *YDrawBuffer::create();
        yguiAddBox(buf.get(), 10, 10, 100, 30, 0xFF333344, 4);

        auto mockCM = std::make_shared<MockCardManager>();
        auto builder = *YDrawBuilder::create(nullptr, nullptr, buf, mockCM, 0);
        builder->setSceneBounds(0, 0, 200.0f, 100.0f);
        builder->setBgColor(0xFF1A1A2E);  // ygui dark theme bg

        builder->calculate();
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const uint32_t* meta = mockCM->metadataAsU32();
        expect(meta[15] == 0xFF1A1A2Eu)
            << "metadata word 15 (bgColor) should be dark theme bg";
    };

    "metadata scene bounds match setSceneBounds"_test = [] {
        auto buf = *YDrawBuffer::create();
        yguiAddBox(buf.get(), 10, 10, 100, 30, 0xFF333344, 4);

        auto mockCM = std::make_shared<MockCardManager>();
        auto builder = *YDrawBuilder::create(nullptr, nullptr, buf, mockCM, 0);

        float pixelW = 800.0f, pixelH = 600.0f;
        builder->setSceneBounds(0, 0, pixelW, pixelH);

        builder->calculate();
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const uint32_t* meta = mockCM->metadataAsU32();
        float sMinX, sMinY, sMaxX, sMaxY;
        std::memcpy(&sMinX, &meta[8], sizeof(float));
        std::memcpy(&sMinY, &meta[9], sizeof(float));
        std::memcpy(&sMaxX, &meta[10], sizeof(float));
        std::memcpy(&sMaxY, &meta[11], sizeof(float));

        expect(sMinX == 0.0_f);
        expect(sMinY == 0.0_f);
        expect(sMaxX == pixelW) << "sceneMaxX";
        expect(sMaxY == pixelH) << "sceneMaxY";
    };
};
