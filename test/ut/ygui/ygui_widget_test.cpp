//=============================================================================
// YGui Widget → GPU Pipeline Tests
//
// Full pipeline: Widget::render(RenderContext) → YDrawBuffer → YDrawBuilder
//   → calculate() → declareBufferNeeds → allocateBuffers → writeBuffers
//   → verify GPU buffer has correct SDF prim types, colors, positions,
//     and grid reachability (same checks the shader does).
//=============================================================================

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

// Engine + widgets
#include "yetty/ygui/ygui-engine.h"
#include "yetty/ygui/ygui-widgets.h"
#include "yetty/ygui/ygui-io.h"
#include "yetty/ygui/widgets/button.h"
#include "yetty/ygui/widgets/checkbox.h"
#include "yetty/ygui/widgets/slider.h"
#include "yetty/ygui/widgets/dropdown.h"
#include "yetty/ygui/widgets/progress.h"
#include "yetty/ygui/widgets/separator.h"
#include "yetty/ygui/widgets/label.h"
#include "yetty/ygui/widgets/panel.h"
#include "yetty/ygui/widgets/collapsing-header.h"
#include "yetty/ygui/widgets/scroll-area.h"
#include "yetty/ygui/widgets/table.h"
#include "yetty/ygui/widgets/tabbar.h"
#include "yetty/ygui/widgets/choicebox.h"
#include "yetty/ygui/widgets/listbox.h"
#include "yetty/ygui/widgets/popup.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <map>
#include <set>
#include <vector>

using namespace boost::ut;
using namespace yetty;
using namespace yetty::card;
using namespace yetty::ygui;

//=============================================================================
// Helpers — class names prefixed with W to avoid ODR clashes with ygui_render_test
//=============================================================================

static uint32_t readU32(const float* buf, uint32_t wordIdx) {
    uint32_t val;
    std::memcpy(&val, &buf[wordIdx], sizeof(uint32_t));
    return val;
}

//-----------------------------------------------------------------------------
// TestIO — collects emitted events
//-----------------------------------------------------------------------------
class TestIO : public YGuiIO {
public:
    std::vector<WidgetEvent> events;
    void emitEvent(const WidgetEvent& e) override { events.push_back(e); }
};

//-----------------------------------------------------------------------------
// Mock CardBufferManager — CPU byte array with bump allocator
//-----------------------------------------------------------------------------
class WMockCardBufferManager : public CardBufferManager {
public:
    void reserve(uint32_t size) override { _pendingReservation += (size + 15) & ~15u; }
    Result<void> commitReservations() override {
        uint32_t total = std::max(_pendingReservation, 16u);
        _storage.resize(total, 0);
        _nextOffset = 0;
        _allocs.clear();
        _pendingReservation = 0;
        return Ok();
    }
    Result<BufferHandle> allocateBuffer(uint32_t slotIndex, const std::string& scope, uint32_t size) override {
        size = (size + 15) & ~15u;
        if (_nextOffset + size > _storage.size()) return Err<BufferHandle>("out of space");
        BufferHandle h{_storage.data() + _nextOffset, _nextOffset, size};
        _allocs[{slotIndex, scope}] = h;
        _nextOffset += size;
        return Ok(h);
    }
    void markBufferDirty(BufferHandle) override {}
    BufferHandle getBufferHandle(uint32_t s, const std::string& sc) const override {
        auto it = _allocs.find({s, sc});
        return it != _allocs.end() ? it->second : BufferHandle::invalid();
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

    const float* storageAsFloat() const { return reinterpret_cast<const float*>(_storage.data()); }
private:
    std::vector<uint8_t> _storage;
    uint32_t _nextOffset = 0;
    uint32_t _pendingReservation = 0;
    std::map<std::pair<uint32_t, std::string>, BufferHandle> _allocs;
};

//-----------------------------------------------------------------------------
// Mock CardManager
//-----------------------------------------------------------------------------
class WMockCardManager : public CardManager {
public:
    WMockCardManager() : _bufMgr(std::make_shared<WMockCardBufferManager>()), _metadata(16 * 64, 0) {}
    Result<MetadataHandle> allocateMetadata(uint32_t) override { return Ok(MetadataHandle{0, 64}); }
    Result<void> deallocateMetadata(MetadataHandle) override { return Ok(); }
    Result<void> writeMetadata(MetadataHandle h, const void* data, uint32_t size) override {
        if (h.offset + size > _metadata.size()) return Err<void>("overflow");
        std::memcpy(_metadata.data() + h.offset, data, size);
        return Ok();
    }
    Result<void> writeMetadataAt(MetadataHandle h, uint32_t offset, const void* data, uint32_t size) override {
        return writeMetadata(MetadataHandle{h.offset + offset, h.size - offset}, data, size);
    }
    CardBufferManager::Ptr bufferManager() const override { return _bufMgr; }
    CardTextureManager::Ptr textureManager() const override { return nullptr; }
    WGPUBuffer metadataBuffer() const override { return nullptr; }
    WGPUBindGroupLayout sharedBindGroupLayout() const override { return nullptr; }
    WGPUBindGroup sharedBindGroup() const override { return nullptr; }
    Result<void> updateBindGroup() override { return Ok(); }
    void invalidateBindGroup() override {}
    Result<void> flush(WGPUQueue) override { return Ok(); }

    std::shared_ptr<WMockCardBufferManager> mockBufMgr() const { return _bufMgr; }
    const uint32_t* metadataAsU32() const { return reinterpret_cast<const uint32_t*>(_metadata.data()); }
    float metadataFloat(uint32_t wordIdx) const {
        float v; std::memcpy(&v, &metadataAsU32()[wordIdx], sizeof(float)); return v;
    }
private:
    std::shared_ptr<WMockCardBufferManager> _bufMgr;
    std::vector<uint8_t> _metadata;
};

//-----------------------------------------------------------------------------
// Mock FontManager
//-----------------------------------------------------------------------------
class WMockFontManager : public FontManager {
public:
    WMockFontManager(MsMsdfFont::Ptr font, const std::string& cacheDir)
        : _font(std::move(font)), _cacheDir(cacheDir) {}
    Result<MsMsdfFont::Ptr> getMsMsdfFont(const std::string&) noexcept override { return Ok(_font); }
    MsMsdfFont::Ptr getDefaultMsMsdfFont() noexcept override { return _font; }
    BmFont::Ptr getDefaultBmFont() noexcept override { return nullptr; }
    ShaderFont::Ptr getDefaultShaderGlyphFont() noexcept override { return nullptr; }
    ShaderFont::Ptr getDefaultCardFont() noexcept override { return nullptr; }
    Result<VectorSdfFont::Ptr> getVectorSdfFont(const std::string&) noexcept override { return Err<VectorSdfFont::Ptr>("stub"); }
    VectorSdfFont::Ptr getDefaultVectorSdfFont() noexcept override { return nullptr; }
    Result<VectorCoverageFont::Ptr> getVectorCoverageFont(const std::string&) noexcept override { return Err<VectorCoverageFont::Ptr>("stub"); }
    VectorCoverageFont::Ptr getDefaultVectorCoverageFont() noexcept override { return nullptr; }
    Result<RasterFont::Ptr> getRasterFont(const std::string&, uint32_t, uint32_t) noexcept override { return Err<RasterFont::Ptr>("stub"); }
    RasterFont::Ptr getDefaultRasterFont() noexcept override { return nullptr; }
    void setDefaultFont(const std::string&) noexcept override {}
    bool hasFont(const std::string&) const noexcept override { return false; }
    const std::string& getCacheDir() const noexcept override { return _cacheDir; }
    MsdfCdbProvider::Ptr getCdbProvider() const noexcept override {
        static auto p = std::make_shared<CpuMsdfCdbProvider>();
        return p;
    }
private:
    MsMsdfFont::Ptr _font;
    std::string _cacheDir;
};

//-----------------------------------------------------------------------------
// Font helpers
//-----------------------------------------------------------------------------
std::string ensureCdbBasePath() {
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
        if (!std::filesystem::exists(cfg.cdbPath)) provider->generate(cfg);
    }
    return basePath;
}

GpuAllocator::Ptr testAllocator() {
    static auto a = std::make_shared<GpuAllocator>(nullptr);
    return a;
}

FontManager::Ptr testFontManager() {
    static FontManager::Ptr fm = [] {
        auto basePath = ensureCdbBasePath();
        auto fontRes = MsMsdfFont::create(basePath, testAllocator());
        if (!fontRes) return FontManager::Ptr(nullptr);
        std::string homeDir = std::getenv("HOME");
        return FontManager::Ptr(std::make_shared<WMockFontManager>(
            *fontRes, homeDir + "/.cache/yetty/msdf-font-cache"));
    }();
    return fm;
}

//=============================================================================
// Pipeline helper — Widget → Engine → Buffer → Builder → GPU storage
//=============================================================================

struct GpuResult {
    std::shared_ptr<WMockCardManager> cardMgr;
    YDrawBuilder::Ptr builder;
    YDrawBuffer::Ptr buffer;
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

GpuResult runWidgetPipeline(float sceneW, float sceneH,
                             std::vector<WidgetPtr> widgets,
                             FontManager::Ptr fm = nullptr) {
    auto buf = *YDrawBuffer::create();

    // Engine renders widgets to buffer
    YGuiEngine engine(buf.get());
    engine.setPixelSize(sceneW, sceneH);
    for (auto& w : widgets) engine.addWidget(w);
    engine.rebuild();

    // Builder processes buffer → GPU
    auto mockCM = std::make_shared<WMockCardManager>();
    auto builder = *YDrawBuilder::create(fm, testAllocator(), buf, std::static_pointer_cast<CardManager>(mockCM), 0);
    builder->setSceneBounds(0, 0, sceneW, sceneH);
    builder->setBgColor(0xFF1A1A2E);
    builder->calculate();
    builder->declareBufferNeeds();
    mockCM->mockBufMgr()->commitReservations();
    builder->allocateBuffers();
    builder->writeBuffers();

    const float* storage = mockCM->mockBufMgr()->storageAsFloat();
    const uint32_t* meta = mockCM->metadataAsU32();

    GpuResult r;
    r.cardMgr = mockCM;
    r.builder = builder;
    r.buffer = buf;
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
// Shader grid lookup — same logic as fragment shader
//=============================================================================
std::vector<uint32_t> shaderGridLookup(const GpuResult& p, float sceneX, float sceneY) {
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
        if ((rawIdx & 0x80000000u) != 0) continue; // skip glyphs
        result.push_back(p.primDataBase + rawIdx);
    }
    return result;
}

// Read prim type at a GPU word offset
uint32_t primType(const GpuResult& p, uint32_t primOff) {
    return readU32(p.storage, primOff);
}

// Read prim fill color (type-dependent offset from docs/ydraw.md)
uint32_t primFillColor(const GpuResult& p, uint32_t primOff) {
    uint32_t type = primType(p, primOff);
    switch (type) {
        case (uint32_t)SDFType::RoundedBox: return readU32(p.storage, primOff + 10);
        case (uint32_t)SDFType::Circle:     return readU32(p.storage, primOff + 5);
        case (uint32_t)SDFType::Triangle:   return readU32(p.storage, primOff + 8);
        case (uint32_t)SDFType::Box:        return readU32(p.storage, primOff + 6);
        default: return 0;
    }
}

uint32_t primStrokeColor(const GpuResult& p, uint32_t primOff) {
    uint32_t type = primType(p, primOff);
    switch (type) {
        case (uint32_t)SDFType::RoundedBox: return readU32(p.storage, primOff + 11);
        case (uint32_t)SDFType::Circle:     return readU32(p.storage, primOff + 6);
        case (uint32_t)SDFType::Triangle:   return readU32(p.storage, primOff + 9);
        case (uint32_t)SDFType::Box:        return readU32(p.storage, primOff + 7);
        default: return 0;
    }
}

// Count prim types in GPU buffer
std::map<uint32_t, int> countPrimTypes(const GpuResult& p) {
    std::map<uint32_t, int> counts;
    for (uint32_t i = 0; i < p.primitiveCount; i++) {
        uint32_t wordOff = readU32(p.storage, p.primitiveOffset + i);
        counts[primType(p, p.primDataBase + wordOff)]++;
    }
    return counts;
}

// Check if a prim with given type and fill color is reachable from grid at (x,y)
bool gridHasPrimWithFill(const GpuResult& p, float x, float y,
                          uint32_t expectedType, uint32_t expectedFill) {
    auto hits = shaderGridLookup(p, x, y);
    for (uint32_t off : hits) {
        if (primType(p, off) == expectedType && primFillColor(p, off) == expectedFill)
            return true;
    }
    return false;
}

bool gridHasPrimWithStroke(const GpuResult& p, float x, float y,
                            uint32_t expectedType, uint32_t expectedStroke) {
    auto hits = shaderGridLookup(p, x, y);
    for (uint32_t off : hits) {
        if (primType(p, off) == expectedType && primStrokeColor(p, off) == expectedStroke)
            return true;
    }
    return false;
}

//=============================================================================
// Button — full GPU pipeline
//=============================================================================

suite widget_gpu_button = [] {

    "Button → GPU: RoundedBox with correct fill at button center"_test = [] {
        auto btn = std::make_shared<Button>();
        btn->x = 10; btn->y = 10; btn->w = 100; btn->h = 28;
        btn->label = "Click";
        btn->bgColor = 0xFF2A2A3E;

        auto p = runWidgetPipeline(300, 200, {btn}, testFontManager());

        expect(p.primitiveCount >= 1_u) << "button needs >= 1 SDF prim, got " << p.primitiveCount;

        // Button center = (60, 24) — shader should find a RoundedBox with bgColor
        float cx = btn->x + btn->w / 2;
        float cy = btn->y + btn->h / 2;
        expect(gridHasPrimWithFill(p, cx, cy, (uint32_t)SDFType::RoundedBox, btn->bgColor))
            << "shader must find RoundedBox with bgColor at button center";

        // Glyphs for "Click"
        uint32_t glyphCount = p.meta[7];
        expect(glyphCount >= 5_u) << "'Click' should produce >= 5 glyphs, got " << glyphCount;
    };

    "Button hover → GPU: fill + stroke outline both in grid"_test = [] {
        auto btn = std::make_shared<Button>();
        btn->x = 20; btn->y = 20; btn->w = 120; btn->h = 28;
        btn->label = "Hover";
        btn->bgColor = 0xFF2A2A3E;
        btn->accentColor = 0xFF4488FF;
        btn->flags |= WIDGET_HOVER;

        auto p = runWidgetPipeline(300, 200, {btn}, testFontManager());

        expect(p.primitiveCount == 2_u) << "hovered button = fill + outline = 2 prims";

        float cx = btn->x + btn->w / 2;
        float cy = btn->y + btn->h / 2;

        expect(gridHasPrimWithFill(p, cx, cy, (uint32_t)SDFType::RoundedBox, btn->bgColor))
            << "fill RoundedBox reachable at center";
        expect(gridHasPrimWithStroke(p, cx, cy, (uint32_t)SDFType::RoundedBox, btn->accentColor))
            << "stroke RoundedBox (outline) reachable at center";
    };
};

//=============================================================================
// Checkbox — full GPU pipeline
//=============================================================================

suite widget_gpu_checkbox = [] {

    "Checkbox checked → GPU: outer + inner RoundedBox"_test = [] {
        auto chk = std::make_shared<Checkbox>();
        chk->x = 10; chk->y = 10; chk->w = 150; chk->h = 24;
        chk->label = "Feature";
        chk->bgColor = 0xFF2A2A3E;
        chk->accentColor = 0xFF4488FF;
        chk->flags |= WIDGET_CHECKED;

        auto p = runWidgetPipeline(300, 100, {chk}, testFontManager());

        expect(p.primitiveCount == 2_u) << "checked checkbox = outer + inner = 2 prims";

        // Checkbox box center = (10 + 8, 10 + 2 + 8) = (18, 20)
        float boxCx = chk->x + 8;
        float boxCy = chk->y + 10;
        expect(gridHasPrimWithFill(p, boxCx, boxCy, (uint32_t)SDFType::RoundedBox, chk->bgColor))
            << "outer box with bgColor at checkbox center";
        expect(gridHasPrimWithFill(p, boxCx, boxCy, (uint32_t)SDFType::RoundedBox, chk->accentColor))
            << "inner check with accentColor at checkbox center";
    };
};

//=============================================================================
// Slider — full GPU pipeline
//=============================================================================

suite widget_gpu_slider = [] {

    "Slider → GPU: track + fill + handle = 3 RoundedBoxes"_test = [] {
        auto sld = std::make_shared<Slider>();
        sld->x = 10; sld->y = 10; sld->w = 200; sld->h = 24;
        sld->value = 50; sld->minValue = 0; sld->maxValue = 100;
        sld->bgColor = 0xFF2A2A3E;
        sld->accentColor = 0xFF4488FF;

        auto p = runWidgetPipeline(400, 100, {sld});

        auto types = countPrimTypes(p);
        expect(types[(uint32_t)SDFType::RoundedBox] == 3)
            << "slider = 3 RoundedBoxes (track + fill + handle)";

        // Handle center ~ x + 50% * w = 110, y + h/2 = 22
        float handleX = sld->x + 0.5f * sld->w;
        float handleY = sld->y + sld->h / 2;
        auto hits = shaderGridLookup(p, handleX, handleY);
        expect(!hits.empty()) << "grid should find prims at slider handle";
    };
};

//=============================================================================
// Dropdown — full GPU pipeline
//=============================================================================

suite widget_gpu_dropdown = [] {

    "Dropdown closed → GPU: RoundedBox + Triangle"_test = [] {
        auto dd = std::make_shared<Dropdown>();
        dd->x = 10; dd->y = 10; dd->w = 150; dd->h = 28;
        dd->options = {"A", "B", "C"};
        dd->selectedIndex = 0;

        auto p = runWidgetPipeline(300, 100, {dd}, testFontManager());

        auto types = countPrimTypes(p);
        expect(types[(uint32_t)SDFType::RoundedBox] >= 1)
            << "dropdown bg = RoundedBox";
        expect(types[(uint32_t)SDFType::Triangle] == 1)
            << "dropdown chevron = Triangle";

        // Center of dropdown
        float cx = dd->x + dd->w / 2;
        float cy = dd->y + dd->h / 2;
        auto hits = shaderGridLookup(p, cx, cy);
        expect(!hits.empty()) << "dropdown center reachable from grid";
    };

    "Dropdown inside ScrollArea: click selects option"_test = [] {
        auto dd = std::make_shared<Dropdown>();
        dd->id = "dd1";
        dd->x = 10; dd->y = 130; dd->w = 150; dd->h = 28;
        dd->options = {"Alpha", "Beta", "Gamma"};
        dd->selectedIndex = 0;
        dd->onChange = "dd_changed";

        auto scroll = std::make_shared<ScrollArea>();
        scroll->id = "scroll";
        scroll->x = 0; scroll->y = 0; scroll->w = 300; scroll->h = 300;
        scroll->contentW = 300; scroll->contentH = 500;
        scroll->children = {dd};

        auto buf = *YDrawBuffer::create();
        TestIO io;
        YGuiEngine engine(buf.get(), &io);
        engine.setPixelSize(300, 300);
        engine.addWidget(scroll);
        engine.rebuild();

        expect(dd->selectedIndex == 0) << "initial selection is 0";
        expect(!dd->isOpen()) << "dropdown starts closed";

        // Click center of dropdown to open it
        float cx = dd->effectiveX + dd->w / 2;
        float cy = dd->effectiveY + dd->h / 2;
        engine.handleMouseDown(cx, cy, 0);
        engine.handleMouseUp(cx, cy, 0);

        expect(dd->isOpen()) << "dropdown must be open after click";

        // Rebuild so grid/focus are updated
        engine.rebuild();

        // Click on option index 2 ("Gamma")
        // Options start at effectiveY + h, each 24px tall
        float optX = dd->effectiveX + dd->w / 2;
        float optY = dd->effectiveY + dd->h + 2 * 24.0f + 12.0f;
        engine.handleMouseDown(optX, optY, 0);
        engine.handleMouseUp(optX, optY, 0);

        expect(!dd->isOpen()) << "dropdown must close after selection";
        expect(dd->selectedIndex == 2) << "selectedIndex must be 2 (Gamma), got " << dd->selectedIndex;

        // Verify change event was emitted
        bool foundChange = false;
        for (auto& e : io.events) {
            if (e.widgetId == "dd1" && e.eventType == "change" && e.value == "2")
                foundChange = true;
        }
        expect(foundChange) << "must emit change event with value '2'";
    };

    "Dropdown inside ScrollArea: multiple selections work"_test = [] {
        auto dd = std::make_shared<Dropdown>();
        dd->id = "dd2";
        dd->x = 10; dd->y = 10; dd->w = 150; dd->h = 28;
        dd->options = {"One", "Two", "Three", "Four"};
        dd->selectedIndex = 0;
        dd->onChange = "dd_sel";

        auto scroll = std::make_shared<ScrollArea>();
        scroll->id = "scroll2";
        scroll->x = 0; scroll->y = 0; scroll->w = 300; scroll->h = 400;
        scroll->contentW = 300; scroll->contentH = 500;
        scroll->children = {dd};

        auto buf = *YDrawBuffer::create();
        TestIO io;
        YGuiEngine engine(buf.get(), &io);
        engine.setPixelSize(300, 400);
        engine.addWidget(scroll);
        engine.rebuild();

        // Open
        float cx = dd->effectiveX + dd->w / 2;
        float cy = dd->effectiveY + dd->h / 2;
        engine.handleMouseDown(cx, cy, 0);
        engine.handleMouseUp(cx, cy, 0);
        expect(dd->isOpen()) << "open after click";
        engine.rebuild();

        // Select option 1 ("Two")
        float optY = dd->effectiveY + dd->h + 1 * 24.0f + 12.0f;
        engine.handleMouseDown(cx, optY, 0);
        engine.handleMouseUp(cx, optY, 0);

        expect(dd->selectedIndex == 1) << "selectedIndex must be 1 (Two), got " << dd->selectedIndex;
        expect(!dd->isOpen()) << "dropdown closes after selection";

        // Open again and select option 3 ("Four")
        io.events.clear();
        engine.rebuild();
        engine.handleMouseDown(cx, cy, 0);
        engine.handleMouseUp(cx, cy, 0);
        expect(dd->isOpen()) << "re-opened";
        engine.rebuild();

        float opt3Y = dd->effectiveY + dd->h + 3 * 24.0f + 12.0f;
        engine.handleMouseDown(cx, opt3Y, 0);
        engine.handleMouseUp(cx, opt3Y, 0);

        expect(dd->selectedIndex == 3) << "selectedIndex must be 3 (Four), got " << dd->selectedIndex;

        bool foundChange = false;
        for (auto& e : io.events) {
            if (e.widgetId == "dd2" && e.eventType == "change" && e.value == "3")
                foundChange = true;
        }
        expect(foundChange) << "must emit change event with value '3'";
    };

    "Dropdown click on header toggles open/close without changing selection"_test = [] {
        auto dd = std::make_shared<Dropdown>();
        dd->id = "dd3";
        dd->x = 10; dd->y = 10; dd->w = 150; dd->h = 28;
        dd->options = {"A", "B"};
        dd->selectedIndex = 0;
        dd->onChange = "dd_chg";

        auto scroll = std::make_shared<ScrollArea>();
        scroll->x = 0; scroll->y = 0; scroll->w = 300; scroll->h = 300;
        scroll->contentW = 300; scroll->contentH = 500;
        scroll->children = {dd};

        auto buf = *YDrawBuffer::create();
        TestIO io;
        YGuiEngine engine(buf.get(), &io);
        engine.setPixelSize(300, 300);
        engine.addWidget(scroll);
        engine.rebuild();

        // Open
        float cx = dd->effectiveX + dd->w / 2;
        float cy = dd->effectiveY + dd->h / 2;
        engine.handleMouseDown(cx, cy, 0);
        engine.handleMouseUp(cx, cy, 0);
        expect(dd->isOpen()) << "opened";
        engine.rebuild();

        // Click on the dropdown header (not in option area) to close
        engine.handleMouseDown(cx, cy, 0);
        engine.handleMouseUp(cx, cy, 0);

        expect(!dd->isOpen()) << "closed after clicking header again";
        expect(dd->selectedIndex == 0) << "selection unchanged, got " << dd->selectedIndex;

        // No change event should have been emitted
        bool foundChange = false;
        for (auto& e : io.events) {
            if (e.eventType == "change") foundChange = true;
        }
        expect(!foundChange) << "no change event when closing without selecting";
    };

    "Button onPress prevents parent from stealing click"_test = [] {
        // Simulate the close button scenario: Button inside a Panel header
        auto panel = std::make_shared<Panel>();
        panel->id = "panel";
        panel->x = 50; panel->y = 50; panel->w = 300; panel->h = 200;
        panel->headerH = 28;

        auto btn = std::make_shared<Button>();
        btn->id = "close-btn";
        btn->x = 250; btn->y = 0; btn->w = 28; btn->h = 28;
        btn->label = "X";
        btn->onClick = "close";
        panel->children.push_back(btn);

        auto buf = *YDrawBuffer::create();
        TestIO io;
        YGuiEngine engine(buf.get(), &io);
        engine.setPixelSize(500, 400);
        engine.addWidget(panel);
        engine.rebuild();

        // Click on the button (which is inside the panel header)
        float bx = btn->effectiveX + btn->w / 2;
        float by = btn->effectiveY + btn->h / 2;
        engine.handleMouseDown(bx, by, 0);
        engine.handleMouseUp(bx, by, 0);

        // Button must receive the click, not the panel
        bool foundClick = false;
        for (auto& e : io.events) {
            if (e.widgetId == "close-btn" && e.eventType == "click")
                foundClick = true;
        }
        expect(foundClick) << "Button must receive click event, not be stolen by Panel drag";
    };
};

//=============================================================================
// CollapsingHeader — closed vs open
//=============================================================================

suite widget_gpu_collapsing = [] {

    "CollapsingHeader closed → GPU: no child prims"_test = [] {
        auto hdr = std::make_shared<CollapsingHeader>();
        hdr->x = 10; hdr->y = 10; hdr->w = 200; hdr->h = 24;
        hdr->label = "Settings";
        // NOT open

        auto chk = std::make_shared<Checkbox>();
        chk->x = 0; chk->y = 0; chk->w = 150; chk->h = 24;
        chk->label = "Option";
        chk->flags |= WIDGET_CHECKED;
        hdr->children.push_back(chk);

        auto p = runWidgetPipeline(300, 200, {hdr}, testFontManager());

        // Header: 1 RoundedBox + 1 Triangle = 2 prims (no child prims)
        expect(p.primitiveCount == 2_u)
            << "closed header = 2 prims (box + arrow), got " << p.primitiveCount;
        // Only header text, not child text
        uint32_t glyphCount = p.meta[7];
        // "Settings" = 8 chars
        expect(glyphCount >= 7_u && glyphCount <= 12_u)
            << "only header label glyphs, not child, got " << glyphCount;
    };

    "CollapsingHeader open → GPU: header + child prims"_test = [] {
        auto hdr = std::make_shared<CollapsingHeader>();
        hdr->x = 10; hdr->y = 10; hdr->w = 200; hdr->h = 24;
        hdr->label = "Settings";
        hdr->flags |= WIDGET_OPEN;

        // Child at (0, 0) = relative to parent content area
        // Effective position: (hdr.x + 0, hdr.y + hdr.h + 0) = (10, 34)
        auto chk = std::make_shared<Checkbox>();
        chk->x = 0; chk->y = 0; chk->w = 150; chk->h = 24;
        chk->label = "Option";
        chk->bgColor = 0xFF2A2A3E;
        chk->flags |= WIDGET_CHECKED;
        hdr->children.push_back(chk);

        auto p = runWidgetPipeline(300, 200, {hdr}, testFontManager());

        // Header: 1 box + 1 triangle = 2 prims
        // Child checkbox checked: 1 outer + 1 inner = 2 prims
        expect(p.primitiveCount == 4_u)
            << "open header + checked checkbox = 4 prims, got " << p.primitiveCount;

        // Child checkbox rendered at effective (10, 34)
        // Checkbox box center: (effectiveX + 8, effectiveY + 10) = (18, 44)
        float chkCx = hdr->x + chk->x + 8;
        float chkCy = hdr->y + hdr->h + chk->y + 10;
        expect(gridHasPrimWithFill(p, chkCx, chkCy, (uint32_t)SDFType::RoundedBox, chk->bgColor))
            << "child checkbox outer box reachable from grid at (" << chkCx << "," << chkCy << ")";
    };
};

//=============================================================================
// ScrollArea — offset applied to children
//=============================================================================

suite widget_gpu_scroll = [] {

    "ScrollArea scrolled → child prim positions shifted in GPU"_test = [] {
        auto scroll = std::make_shared<ScrollArea>();
        scroll->x = 0; scroll->y = 0; scroll->w = 300; scroll->h = 200;
        scroll->contentW = 300; scroll->contentH = 500;
        scroll->scrollY = 0; // no scroll for baseline

        auto btn = std::make_shared<Button>();
        btn->x = 10; btn->y = 150; btn->w = 100; btn->h = 28;
        btn->label = "Btn";
        btn->bgColor = 0xFF2A2A3E;
        scroll->children.push_back(btn);

        // No scroll — button at (60, 164) should be reachable
        auto p1 = runWidgetPipeline(300, 200, {scroll}, testFontManager());
        float cx = btn->x + btn->w / 2;
        float cy = btn->y + btn->h / 2;
        expect(gridHasPrimWithFill(p1, cx, cy, (uint32_t)SDFType::RoundedBox, btn->bgColor))
            << "unscrolled: button at (60,164) reachable";

        // Scroll down 100px — button rendered at y=150-100=50, center at y=64
        scroll->scrollY = 100;
        auto p2 = runWidgetPipeline(300, 200, {scroll}, testFontManager());
        float scrolledCy = cy - 100;
        expect(gridHasPrimWithFill(p2, cx, scrolledCy, (uint32_t)SDFType::RoundedBox, btn->bgColor))
            << "scrolled: button shifted to y=64 reachable";
    };
};

//=============================================================================
// Table — inline rendering, no double children
//=============================================================================

suite widget_gpu_table = [] {

    "Table → GPU: header + row cells produce text glyphs"_test = [] {
        auto table = std::make_shared<Table>();
        table->x = 10; table->y = 10; table->w = 300; table->h = 48;
        table->columnWidths = {100, 100, 100};
        table->rowHeight = 24;

        auto header = std::make_shared<TableHeader>(); header->h = 24;
        auto hc1 = std::make_shared<TableCell>(); hc1->label = "Name";
        auto hc2 = std::make_shared<TableCell>(); hc2->label = "Val";
        header->children = {hc1, hc2};
        table->children.push_back(header);

        auto row = std::make_shared<TableRow>();
        auto rc1 = std::make_shared<TableCell>(); rc1->label = "Alpha";
        auto rc2 = std::make_shared<TableCell>(); rc2->label = "123";
        row->children = {rc1, rc2};
        table->children.push_back(row);

        auto p = runWidgetPipeline(400, 200, {table}, testFontManager());

        // Table bg + outline + header bg + borders + column dividers = several prims
        expect(p.primitiveCount >= 4_u) << "table produces prims, got " << p.primitiveCount;

        // 4 cell labels = text glyphs
        // "Name" (4) + "Val" (3) + "Alpha" (5) + "123" (3) = 15 chars
        uint32_t glyphCount = p.meta[7];
        expect(glyphCount >= 12_u) << "table cell text should produce glyphs, got " << glyphCount;

        // Table center reachable
        float cx = table->x + table->w / 2;
        float cy = table->y + table->h / 2;
        auto hits = shaderGridLookup(p, cx, cy);
        expect(!hits.empty()) << "table center reachable from grid";
    };
};

//=============================================================================
// TabBar — inline rendering
//=============================================================================

suite widget_gpu_tabbar = [] {

    "TabBar → GPU: bg + active tab highlight"_test = [] {
        auto tabs = std::make_shared<TabBar>();
        tabs->x = 10; tabs->y = 10; tabs->w = 300; tabs->h = 28;
        tabs->activeTab = 0;

        auto t1 = std::make_shared<TabItem>(); t1->label = "Tab1"; t1->w = 80;
        auto t2 = std::make_shared<TabItem>(); t2->label = "Tab2"; t2->w = 80;
        tabs->children = {t1, t2};

        auto p = runWidgetPipeline(400, 100, {tabs}, testFontManager());

        // bg + active tab highlight = 2 RoundedBoxes
        auto types = countPrimTypes(p);
        expect(types[(uint32_t)SDFType::RoundedBox] >= 2)
            << "tabbar bg + active highlight = >= 2 RoundedBoxes";

        // 2 tab label texts
        uint32_t glyphCount = p.meta[7];
        expect(glyphCount >= 6_u) << "'Tab1' + 'Tab2' glyphs, got " << glyphCount;
    };
};

//=============================================================================
// Full demo — all widget types, all prims reachable from grid
//=============================================================================

suite widget_gpu_full_demo = [] {

    "Full demo → GPU: all prims reachable from grid"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        // Build a mini demo
        auto btn = std::make_shared<Button>();
        btn->x = 10; btn->y = 10; btn->w = 100; btn->h = 28;
        btn->label = "Click"; btn->bgColor = 0xFF2A2A3E;

        auto chk = std::make_shared<Checkbox>();
        chk->x = 10; chk->y = 50; chk->w = 150; chk->h = 24;
        chk->label = "Check"; chk->flags |= WIDGET_CHECKED;

        auto sld = std::make_shared<Slider>();
        sld->x = 10; sld->y = 90; sld->w = 200; sld->h = 24;
        sld->value = 50; sld->minValue = 0; sld->maxValue = 100;

        auto dd = std::make_shared<Dropdown>();
        dd->x = 10; dd->y = 130; dd->w = 150; dd->h = 28;
        dd->options = {"A", "B", "C"};

        auto prog = std::make_shared<Progress>();
        prog->x = 10; prog->y = 180; prog->w = 200; prog->h = 20;
        prog->value = 75; prog->minValue = 0; prog->maxValue = 100;

        auto sep = std::make_shared<Separator>();
        sep->x = 10; sep->y = 210; sep->w = 200; sep->h = 10;

        auto p = runWidgetPipeline(400, 300, {btn, chk, sld, dd, prog, sep}, fm);

        expect(p.primitiveCount >= 10_u)
            << "full demo should produce >= 10 prims, got " << p.primitiveCount;

        // Verify ALL prims reachable from grid (critical for shader visibility)
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
                << "prim " << i << " (type=" << primType(p, primOff)
                << ") MUST be reachable from grid for shader to render it";
        }

        // Verify metadata
        expect(p.meta[15] == 0xFF1A1A2Eu) << "bgColor in metadata";

        float sMaxX, sMaxY;
        std::memcpy(&sMaxX, &p.meta[10], sizeof(float));
        std::memcpy(&sMaxY, &p.meta[11], sizeof(float));
        expect(sMaxX == 400.0_f) << "sceneMaxX";
        expect(sMaxY == 300.0_f) << "sceneMaxY";

        // Glyphs produced
        uint32_t glyphCount = p.meta[7];
        expect(glyphCount >= 10_u) << "text labels should produce glyphs, got " << glyphCount;
    };

    "Full demo rebuild → GPU: prims replaced correctly"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        auto buf = *YDrawBuffer::create();
        YGuiEngine engine(buf.get());
        engine.setPixelSize(300, 200);

        // First render: button
        auto btn = std::make_shared<Button>();
        btn->x = 10; btn->y = 10; btn->w = 100; btn->h = 28;
        btn->label = "Old"; btn->bgColor = 0xFF333344;
        engine.addWidget(btn);
        engine.rebuild();

        auto mockCM = std::make_shared<WMockCardManager>();
        auto builder = *YDrawBuilder::create(fm, testAllocator(), buf, std::static_pointer_cast<CardManager>(mockCM), 0);
        builder->setSceneBounds(0, 0, 300, 200);
        builder->setBgColor(0xFF1A1A2E);
        builder->calculate();
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const uint32_t* meta = mockCM->metadataAsU32();
        expect(meta[1] >= 1_u) << "first build has prims";

        // Second render: different widget
        engine.clearWidgets();
        auto chk = std::make_shared<Checkbox>();
        chk->x = 20; chk->y = 20; chk->w = 150; chk->h = 24;
        chk->label = "New"; chk->flags |= WIDGET_CHECKED;
        engine.addWidget(chk);
        engine.rebuild();

        builder->calculate();
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        meta = mockCM->metadataAsU32();
        expect(meta[1] == 2_u) << "rebuild: checked checkbox = 2 prims (outer + inner)";
    };
};

//=============================================================================
// DIRECT widget render tests — Widget::render(ctx) → Buffer, NO engine
//
// These verify the widgets produce SDF prims in the buffer BEFORE the builder
// or engine touch anything. If these fail, the widget render() is broken.
//=============================================================================

suite widget_direct_render = [] {

    "DIRECT: Button::render produces RoundedBox in buffer"_test = [] {
        auto buf = *YDrawBuffer::create();
        RenderContext ctx(buf.get());

        Button btn;
        btn.x = 10; btn.y = 20; btn.w = 100; btn.h = 28;
        btn.bgColor = 0xFF2A2A3E;
        btn.label = "Test";

        btn.render(ctx);

        // Buffer must have exactly 1 prim (the box) and 1 text span
        expect(buf->primCount() == 1_u) << "Button::render must produce 1 RoundedBox prim";
        expect(buf->textSpanCount() == 1_u) << "Button::render must produce 1 text span";

        // Verify the prim data layout matches the shader's expectations
        buf->forEachPrim([&](uint32_t /*id*/, const float* data, uint32_t wc) {
            expect(wc == 14_u) << "RoundedBox is 14 words";

            // Word 0: type = RoundedBox (8)
            uint32_t type;
            std::memcpy(&type, &data[0], sizeof(uint32_t));
            expect(type == (uint32_t)SDFType::RoundedBox) << "type must be RoundedBox(8)";

            // Word 2: cx = x + w/2 = 60
            expect(std::abs(data[2] - 60.0f) < 0.01f) << "cx = 60";
            // Word 3: cy = y + h/2 = 34
            expect(std::abs(data[3] - 34.0f) < 0.01f) << "cy = 34";
            // Word 4: hw = w/2 = 50
            expect(std::abs(data[4] - 50.0f) < 0.01f) << "hw = 50";
            // Word 5: hh = h/2 = 14
            expect(std::abs(data[5] - 14.0f) < 0.01f) << "hh = 14";
            // Words 6-9: corner radii = 4
            expect(data[6] == 4.0_f) << "r0 = 4";
            expect(data[7] == 4.0_f) << "r1 = 4";
            expect(data[8] == 4.0_f) << "r2 = 4";
            expect(data[9] == 4.0_f) << "r3 = 4";
            // Word 10: fillColor = bgColor
            uint32_t fillColor;
            std::memcpy(&fillColor, &data[10], sizeof(uint32_t));
            expect(fillColor == 0xFF2A2A3Eu) << "fillColor must be bgColor";
            // Word 11: strokeColor = 0
            uint32_t strokeColor;
            std::memcpy(&strokeColor, &data[11], sizeof(uint32_t));
            expect(strokeColor == 0u) << "strokeColor must be 0 (fill only)";
            // Word 12: strokeWidth = 0
            expect(data[12] == 0.0_f) << "strokeWidth = 0";
        });
    };

    "DIRECT: Button hover produces fill + outline in buffer"_test = [] {
        auto buf = *YDrawBuffer::create();
        RenderContext ctx(buf.get());

        Button btn;
        btn.x = 10; btn.y = 10; btn.w = 100; btn.h = 28;
        btn.bgColor = 0xFF2A2A3E;
        btn.accentColor = 0xFF4488FF;
        btn.flags |= WIDGET_HOVER;
        btn.label = "Hov";

        btn.render(ctx);

        expect(buf->primCount() == 2_u) << "hovered Button: fill + outline = 2 prims";

        int fillCount = 0, outlineCount = 0;
        buf->forEachPrim([&](uint32_t, const float* data, uint32_t) {
            uint32_t fill, stroke;
            std::memcpy(&fill, &data[10], sizeof(uint32_t));
            std::memcpy(&stroke, &data[11], sizeof(uint32_t));
            if (fill == 0xFF2A2A3Eu && stroke == 0u) fillCount++;
            if (fill == 0u && stroke == 0xFF4488FFu) outlineCount++;
        });
        expect(fillCount == 1) << "must have 1 fill RoundedBox";
        expect(outlineCount == 1) << "must have 1 outline RoundedBox";
    };

    "DIRECT: Checkbox checked produces 2 RoundedBoxes in buffer"_test = [] {
        auto buf = *YDrawBuffer::create();
        RenderContext ctx(buf.get());

        Checkbox chk;
        chk.x = 10; chk.y = 10; chk.w = 150; chk.h = 24;
        chk.bgColor = 0xFF2A2A3E;
        chk.accentColor = 0xFF4488FF;
        chk.flags |= WIDGET_CHECKED;
        chk.label = "Opt";

        chk.render(ctx);

        expect(buf->primCount() == 2_u) << "checked Checkbox: outer + inner = 2 prims";
        expect(buf->textSpanCount() == 1_u) << "Checkbox label = 1 text span";

        int outerCount = 0, innerCount = 0;
        buf->forEachPrim([&](uint32_t, const float* data, uint32_t) {
            uint32_t type, fill;
            std::memcpy(&type, &data[0], sizeof(uint32_t));
            std::memcpy(&fill, &data[10], sizeof(uint32_t));
            expect(type == (uint32_t)SDFType::RoundedBox) << "checkbox prim must be RoundedBox";
            if (fill == 0xFF2A2A3Eu) outerCount++;
            if (fill == 0xFF4488FFu) innerCount++;
        });
        expect(outerCount == 1) << "outer box with bgColor";
        expect(innerCount == 1) << "inner check with accentColor";
    };

    "DIRECT: Slider produces 3 RoundedBoxes in buffer"_test = [] {
        auto buf = *YDrawBuffer::create();
        RenderContext ctx(buf.get());

        Slider sld;
        sld.x = 10; sld.y = 10; sld.w = 200; sld.h = 24;
        sld.value = 50; sld.minValue = 0; sld.maxValue = 100;
        sld.bgColor = 0xFF2A2A3E;
        sld.accentColor = 0xFF4488FF;

        sld.render(ctx);

        expect(buf->primCount() == 3_u) << "Slider: track + fill + handle = 3 prims";

        int roundedBoxCount = 0;
        buf->forEachPrim([&](uint32_t, const float* data, uint32_t) {
            uint32_t type;
            std::memcpy(&type, &data[0], sizeof(uint32_t));
            if (type == (uint32_t)SDFType::RoundedBox) roundedBoxCount++;
        });
        expect(roundedBoxCount == 3) << "all 3 slider prims are RoundedBox";
    };

    "DIRECT: Dropdown produces RoundedBox + Triangle in buffer"_test = [] {
        auto buf = *YDrawBuffer::create();
        RenderContext ctx(buf.get());

        Dropdown dd;
        dd.x = 10; dd.y = 10; dd.w = 150; dd.h = 28;
        dd.options = {"A", "B", "C"};
        dd.selectedIndex = 0;

        dd.render(ctx);

        int rboxCount = 0, triCount = 0;
        buf->forEachPrim([&](uint32_t, const float* data, uint32_t) {
            uint32_t type;
            std::memcpy(&type, &data[0], sizeof(uint32_t));
            if (type == (uint32_t)SDFType::RoundedBox) rboxCount++;
            if (type == (uint32_t)SDFType::Triangle) triCount++;
        });
        expect(rboxCount >= 1) << "dropdown bg = at least 1 RoundedBox";
        expect(triCount == 1) << "dropdown chevron = 1 Triangle";
    };

    "DIRECT: Separator produces 1 RoundedBox in buffer"_test = [] {
        auto buf = *YDrawBuffer::create();
        RenderContext ctx(buf.get());

        Separator sep;
        sep.x = 10; sep.y = 10; sep.w = 200; sep.h = 10;

        sep.render(ctx);

        expect(buf->primCount() == 1_u) << "Separator = 1 prim";
    };

    "DIRECT: Progress produces 2 RoundedBoxes in buffer"_test = [] {
        auto buf = *YDrawBuffer::create();
        RenderContext ctx(buf.get());

        Progress prog;
        prog.x = 10; prog.y = 10; prog.w = 200; prog.h = 20;
        prog.value = 75; prog.minValue = 0; prog.maxValue = 100;

        prog.render(ctx);

        expect(buf->primCount() == 2_u) << "Progress: track + fill = 2 prims";
    };
};

//=============================================================================
// DIRECT render → Builder GPU pipeline (no engine)
//=============================================================================

suite widget_direct_to_gpu = [] {

    "DIRECT → GPU: Button SDF prim reachable from grid"_test = [] {
        auto buf = *YDrawBuffer::create();
        RenderContext ctx(buf.get());

        Button btn;
        btn.x = 10; btn.y = 10; btn.w = 100; btn.h = 28;
        btn.bgColor = 0xFF2A2A3E;
        btn.label = "X";

        btn.render(ctx);
        buf->setSceneBounds(0, 0, 300, 200);

        auto fm = testFontManager();
        auto mockCM = std::make_shared<WMockCardManager>();
        auto builder = *YDrawBuilder::create(fm, testAllocator(), buf,
            std::static_pointer_cast<CardManager>(mockCM), 0);
        builder->setSceneBounds(0, 0, 300, 200);
        builder->setBgColor(0xFF1A1A2E);
        builder->calculate();
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32();

        uint32_t primOff = meta[0];
        uint32_t primCnt = meta[1];
        uint32_t primDataBase = primOff + primCnt;

        expect(primCnt >= 1_u) << "GPU must have >= 1 prim";

        uint32_t wordOff = readU32(storage, primOff);
        uint32_t absOff = primDataBase + wordOff;
        uint32_t type = readU32(storage, absOff + 0);
        expect(type == (uint32_t)SDFType::RoundedBox)
            << "GPU prim[0] type must be RoundedBox, got " << type;

        uint32_t gpuFill = readU32(storage, absOff + 10);
        expect(gpuFill == 0xFF2A2A3Eu)
            << "GPU prim[0] fillColor at word +10 must be bgColor";

        // Grid reachability at button center
        float cx = 60.0f, cy = 24.0f;
        GpuResult p;
        p.storage = storage; p.meta = meta;
        p.primitiveOffset = primOff; p.primitiveCount = primCnt;
        p.primDataBase = primDataBase;
        p.gridOffset = meta[2]; p.gridWidth = meta[3]; p.gridHeight = meta[4];
        std::memcpy(&p.cellSize, &meta[5], sizeof(float));
        std::memcpy(&p.sceneMinX, &meta[8], sizeof(float));
        std::memcpy(&p.sceneMinY, &meta[9], sizeof(float));

        expect(gridHasPrimWithFill(p, cx, cy, (uint32_t)SDFType::RoundedBox, 0xFF2A2A3Eu))
            << "shader grid at button center must find RoundedBox with bgColor";
    };
};

//=============================================================================
// Card-exact init sequence — replicate ygui.cpp init() step by step
//=============================================================================

suite card_exact_flow = [] {

    "Card init sequence: builder before engine, all prims in GPU"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        // Step 1: Create buffer (card init line 349)
        auto buf = *YDrawBuffer::create();

        // Step 2: Create builder BEFORE engine (card init line 368-372)
        auto mockCM = std::make_shared<WMockCardManager>();
        auto builder = *YDrawBuilder::create(fm, testAllocator(), buf,
            std::static_pointer_cast<CardManager>(mockCM), 0);

        // Step 3: Set viewport (card init line 373)
        builder->setViewport(40, 15);

        // Step 4: Create engine (card init line 377)
        YGuiEngine engine(buf.get());

        // Step 5: Set pixel size (card init line 381)
        float pixelW = 400, pixelH = 300;
        engine.setPixelSize(pixelW, pixelH);

        // Step 6: Add widgets
        auto btn = std::make_shared<Button>();
        btn->x = 10; btn->y = 10; btn->w = 120; btn->h = 28;
        btn->label = "Click Me"; btn->bgColor = 0xFF2A2A3E;
        engine.addWidget(btn);

        auto chk = std::make_shared<Checkbox>();
        chk->x = 10; chk->y = 50; chk->w = 150; chk->h = 24;
        chk->label = "Option"; chk->flags |= WIDGET_CHECKED;
        engine.addWidget(chk);

        auto sld = std::make_shared<Slider>();
        sld->x = 10; sld->y = 90; sld->w = 200; sld->h = 24;
        sld->value = 50; sld->minValue = 0; sld->maxValue = 100;
        engine.addWidget(sld);

        // Step 7: engine.rebuild() (card init line 391)
        engine.rebuild();

        // Verify buffer has prims BEFORE builder touches it
        expect(buf->primCount() >= 6_u)
            << "buffer after rebuild: btn(1)+chk(2)+sld(3)=6+ prims, got " << buf->primCount();

        // Step 8-9: builder.setSceneBounds + setBgColor (card init line 392-393)
        builder->setSceneBounds(0, 0, pixelW, pixelH);
        builder->setBgColor(0xFF1A1A2E);

        // Step 10: builder.calculate() (card init line 394)
        builder->calculate();

        // Step 11-12: Framework first-frame calls
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        // Step 13: Verify GPU
        const float* storage = mockCM->mockBufMgr()->storageAsFloat();
        const uint32_t* meta = mockCM->metadataAsU32();

        uint32_t primCnt = meta[1];
        expect(primCnt >= 6_u)
            << "GPU: btn(1)+chk(2)+sld(3)=6+ prims, got " << primCnt;

        // Verify all prims reachable from grid
        uint32_t primOff = meta[0];
        uint32_t primDataBase = primOff + primCnt;
        uint32_t gridOff = meta[2];
        uint32_t gridW = meta[3], gridH = meta[4];

        std::set<uint32_t> reachable;
        for (uint32_t ci = 0; ci < gridW * gridH; ci++) {
            uint32_t packed = readU32(storage, gridOff + ci);
            uint32_t cnt = readU32(storage, gridOff + packed);
            for (uint32_t j = 0; j < cnt; j++) {
                uint32_t raw = readU32(storage, gridOff + packed + 1 + j);
                if ((raw & 0x80000000u) == 0)
                    reachable.insert(primDataBase + raw);
            }
        }

        for (uint32_t i = 0; i < primCnt; i++) {
            uint32_t wordOff = readU32(storage, primOff + i);
            uint32_t off = primDataBase + wordOff;
            expect(reachable.count(off) > 0_u)
                << "prim " << i << " (type=" << readU32(storage, off)
                << ") NOT reachable from grid — shader WILL NOT render it";
        }

        // Verify metadata
        expect(meta[15] == 0xFF1A1A2Eu) << "bgColor in metadata";

        // Verify glyphs
        uint32_t glyphCnt = meta[7];
        expect(glyphCnt >= 13_u) << "text labels produce glyphs, got " << glyphCnt;
    };

    "Card renderToStaging: dirty rebuild replaces prims"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        auto buf = *YDrawBuffer::create();
        auto mockCM = std::make_shared<WMockCardManager>();
        auto builder = *YDrawBuilder::create(fm, testAllocator(), buf,
            std::static_pointer_cast<CardManager>(mockCM), 0);
        builder->setViewport(30, 10);

        YGuiEngine engine(buf.get());
        engine.setPixelSize(300, 200);

        auto btn = std::make_shared<Button>();
        btn->x = 10; btn->y = 10; btn->w = 100; btn->h = 28;
        btn->label = "A"; btn->bgColor = 0xFF333344;
        engine.addWidget(btn);

        // Init
        engine.rebuild();
        builder->setSceneBounds(0, 0, 300, 200);
        builder->setBgColor(0xFF1A1A2E);
        builder->calculate();
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        const uint32_t* meta = mockCM->metadataAsU32();
        expect(meta[1] >= 1_u) << "first frame: at least 1 prim";

        // Simulate hover → dirty
        btn->flags |= WIDGET_HOVER;
        engine.markDirty();

        // renderToStaging equivalent
        if (engine.isDirty()) {
            engine.rebuild();
            builder->calculate();
        }
        builder->declareBufferNeeds();
        mockCM->mockBufMgr()->commitReservations();
        builder->allocateBuffers();
        builder->writeBuffers();

        meta = mockCM->metadataAsU32();
        expect(meta[1] == 2_u) << "after hover: fill + outline = 2 prims, got " << meta[1];
    };
};

//=============================================================================
// ScrollArea with multiple children — THE bug scenario
//
// Individual widgets render fine. Wrapping them in a ScrollArea breaks
// SDF rendering. These tests reproduce the failure by comparing the same
// widget set with and without ScrollArea wrapping.
//=============================================================================

// Helper: create a standard set of child widgets for scroll tests
static std::vector<WidgetPtr> makeScrollChildren() {
    auto btn = std::make_shared<Button>();
    btn->x = 10; btn->y = 10; btn->w = 100; btn->h = 28;
    btn->label = "Click"; btn->bgColor = 0xFF2A2A3E;

    auto chk = std::make_shared<Checkbox>();
    chk->x = 10; chk->y = 50; chk->w = 150; chk->h = 24;
    chk->label = "Enable"; chk->flags |= WIDGET_CHECKED;

    auto sld = std::make_shared<Slider>();
    sld->x = 10; sld->y = 84; sld->w = 200; sld->h = 24;
    sld->value = 50; sld->minValue = 0; sld->maxValue = 100;

    auto dd = std::make_shared<Dropdown>();
    dd->x = 10; dd->y = 120; dd->w = 150; dd->h = 28;
    dd->options = {"Light", "Dark", "System"};
    dd->selectedIndex = 1;

    auto sep = std::make_shared<Separator>();
    sep->x = 10; sep->y = 160; sep->w = 200; sep->h = 10;

    return {btn, chk, sld, dd, sep};
}

// Helper: count how many prims the scroll area ITSELF adds
// (bg box + scrollbar track/thumb for V/H + corner fill)
static uint32_t countScrollAreaOwnPrims(float scrollW, float scrollH,
                                         float contentW, float contentH) {
    uint32_t count = 1; // bg box always
    float scrollbarW = 12, scrollbarH = 12;
    float viewW = scrollW - scrollbarW;
    float viewH = scrollH - scrollbarH;
    float cW = contentW > 0 ? contentW : viewW;
    float cH = contentH > 0 ? contentH : viewH;
    if (cH > viewH) count += 2; // V track + V thumb
    if (cW > viewW) count += 2; // H track + H thumb
    if (cW > viewW && cH > viewH) count += 1; // corner fill
    return count;
}

// Helper: verify ALL prims are reachable from grid
static void verifyAllPrimsReachable(const GpuResult& p, const std::string& label) {
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
            << label << ": prim " << i << " (type=" << primType(p, primOff)
            << " fill=0x" << std::hex << primFillColor(p, primOff) << std::dec
            << ") NOT reachable from grid — shader WILL NOT render it";
    }
}

suite widget_gpu_scroll_multi = [] {

    //-------------------------------------------------------------------------
    // CONTROL: Same widgets flat (no ScrollArea) — all prims reachable
    //-------------------------------------------------------------------------
    "CONTROL: flat widgets (no ScrollArea) → all prims reachable"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        auto children = makeScrollChildren();
        // btn(1) + chk(2) + sld(3) + dd(2) + sep(1) = 9 prims
        auto p = runWidgetPipeline(400, 300, children, fm);

        expect(p.primitiveCount >= 9_u)
            << "flat widgets: btn(1)+chk(2)+sld(3)+dd(2)+sep(1) >= 9, got "
            << p.primitiveCount;

        verifyAllPrimsReachable(p, "flat");
    };

    //-------------------------------------------------------------------------
    // BUG TEST: ScrollArea wrapping same widgets — all prims reachable
    //-------------------------------------------------------------------------
    "ScrollArea with multiple children → all prims reachable from grid"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        auto scroll = std::make_shared<ScrollArea>();
        scroll->x = 0; scroll->y = 0; scroll->w = 300; scroll->h = 200;
        scroll->contentW = 300; scroll->contentH = 400;
        scroll->children = makeScrollChildren();

        auto p = runWidgetPipeline(300, 200, {scroll}, fm);

        uint32_t scrollOwnPrims = countScrollAreaOwnPrims(300, 200, 300, 400);
        // scroll prims + child prims (9)
        expect(p.primitiveCount >= scrollOwnPrims + 9)
            << "scroll(" << scrollOwnPrims << ") + children(9) = "
            << (scrollOwnPrims + 9) << ", got " << p.primitiveCount;

        verifyAllPrimsReachable(p, "scrollarea-multi");
    };

    //-------------------------------------------------------------------------
    // BUG TEST: prim count consistency — scroll vs flat
    //-------------------------------------------------------------------------
    "ScrollArea prim count = scroll own prims + child prims"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        // Flat
        auto flatChildren = makeScrollChildren();
        auto pFlat = runWidgetPipeline(400, 300, flatChildren, fm);

        // Wrapped in ScrollArea
        auto scroll = std::make_shared<ScrollArea>();
        scroll->x = 0; scroll->y = 0; scroll->w = 300; scroll->h = 200;
        scroll->contentW = 300; scroll->contentH = 400;
        scroll->children = makeScrollChildren();

        auto pScroll = runWidgetPipeline(300, 200, {scroll}, fm);

        uint32_t scrollOwnPrims = countScrollAreaOwnPrims(300, 200, 300, 400);
        uint32_t expectedTotal = pFlat.primitiveCount + scrollOwnPrims;

        expect(pScroll.primitiveCount == expectedTotal)
            << "scroll total (" << pScroll.primitiveCount
            << ") should be flat (" << pFlat.primitiveCount
            << ") + scroll own (" << scrollOwnPrims
            << ") = " << expectedTotal;
    };

    //-------------------------------------------------------------------------
    // BUG TEST: each child widget individually reachable inside ScrollArea
    //-------------------------------------------------------------------------
    "ScrollArea children: each child RoundedBox reachable at its position"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        auto btn = std::make_shared<Button>();
        btn->x = 10; btn->y = 10; btn->w = 100; btn->h = 28;
        btn->label = "Click"; btn->bgColor = 0xFF2A2A3E;

        auto chk = std::make_shared<Checkbox>();
        chk->x = 10; chk->y = 50; chk->w = 150; chk->h = 24;
        chk->label = "Enable"; chk->bgColor = 0xFF2A2A3E;
        chk->flags |= WIDGET_CHECKED;

        auto sld = std::make_shared<Slider>();
        sld->x = 10; sld->y = 84; sld->w = 200; sld->h = 24;
        sld->value = 50; sld->minValue = 0; sld->maxValue = 100;
        sld->bgColor = 0xFF2A2A3E;

        auto scroll = std::make_shared<ScrollArea>();
        scroll->x = 0; scroll->y = 0; scroll->w = 300; scroll->h = 200;
        scroll->contentW = 300; scroll->contentH = 400;
        scroll->scrollX = 0; scroll->scrollY = 0; // no scroll offset
        scroll->children = {btn, chk, sld};

        auto p = runWidgetPipeline(300, 200, {scroll}, fm);

        // Button center
        float btnCx = btn->x + btn->w / 2;  // 60
        float btnCy = btn->y + btn->h / 2;  // 24
        expect(gridHasPrimWithFill(p, btnCx, btnCy,
            (uint32_t)SDFType::RoundedBox, btn->bgColor))
            << "button RoundedBox reachable inside ScrollArea at (" << btnCx << "," << btnCy << ")";

        // Checkbox outer box center
        float chkCx = chk->x + 8;   // 18
        float chkCy = chk->y + 10;  // 60
        expect(gridHasPrimWithFill(p, chkCx, chkCy,
            (uint32_t)SDFType::RoundedBox, chk->bgColor))
            << "checkbox RoundedBox reachable inside ScrollArea at (" << chkCx << "," << chkCy << ")";

        // Slider — any RoundedBox at slider center
        float sldCx = sld->x + sld->w / 2;  // 110
        float sldCy = sld->y + sld->h / 2;  // 96
        auto hits = shaderGridLookup(p, sldCx, sldCy);
        bool foundSliderPrim = false;
        for (uint32_t off : hits) {
            if (primType(p, off) == (uint32_t)SDFType::RoundedBox)
                foundSliderPrim = true;
        }
        expect(foundSliderPrim)
            << "slider RoundedBox reachable inside ScrollArea at (" << sldCx << "," << sldCy << ")";
    };

    //-------------------------------------------------------------------------
    // BUG TEST: ScrollArea with MANY buttons (matches 15-buttons but wrapped)
    //-------------------------------------------------------------------------
    "ScrollArea with 15 buttons → all prims reachable"_test = [] {
        auto scroll = std::make_shared<ScrollArea>();
        scroll->x = 0; scroll->y = 0; scroll->w = 200; scroll->h = 300;
        scroll->contentW = 200; scroll->contentH = 600;

        for (int i = 0; i < 15; i++) {
            auto btn = std::make_shared<Button>();
            btn->x = 10; btn->y = 10.0f + i * 32.0f;
            btn->w = 100; btn->h = 28;
            btn->label = "Btn " + std::to_string(i + 1);
            btn->bgColor = 0xFF2A2A3E;
            scroll->children.push_back(btn);
        }

        auto p = runWidgetPipeline(200, 300, {scroll});

        uint32_t scrollOwnPrims = countScrollAreaOwnPrims(200, 300, 200, 600);
        // Only 9 of 15 buttons are visible (clip area h=288, buttons at y=10+i*32,
        // button 9 at y=298 is fully clipped). Each visible button = 1 box prim.
        expect(p.primitiveCount >= scrollOwnPrims + 9)
            << "15 buttons in scroll (9 visible): expected >= " << (scrollOwnPrims + 9)
            << ", got " << p.primitiveCount;

        verifyAllPrimsReachable(p, "scroll-15-buttons");
    };

    //-------------------------------------------------------------------------
    // BUG TEST: ScrollArea empty (just scrollbars) — prims reachable
    //-------------------------------------------------------------------------
    "ScrollArea empty (no children, overflow) → all prims reachable"_test = [] {
        auto scroll = std::make_shared<ScrollArea>();
        scroll->x = 0; scroll->y = 0; scroll->w = 180; scroll->h = 160;
        scroll->contentW = 300; scroll->contentH = 400;

        auto p = runWidgetPipeline(180, 160, {scroll});

        uint32_t scrollOwnPrims = countScrollAreaOwnPrims(180, 160, 300, 400);
        expect(p.primitiveCount == scrollOwnPrims)
            << "empty scroll: " << scrollOwnPrims << " prims, got " << p.primitiveCount;

        verifyAllPrimsReachable(p, "scroll-empty");
    };

    //-------------------------------------------------------------------------
    // BUG TEST: ScrollArea no-overflow (no scrollbars) with one child
    //-------------------------------------------------------------------------
    "ScrollArea no-overflow + child → all prims reachable"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        auto btn = std::make_shared<Button>();
        btn->x = 10; btn->y = 10; btn->w = 80; btn->h = 30;
        btn->label = "Hello"; btn->bgColor = 0xFF2A2A3E;

        auto scroll = std::make_shared<ScrollArea>();
        scroll->x = 0; scroll->y = 0; scroll->w = 180; scroll->h = 160;
        scroll->contentW = 100; scroll->contentH = 100;
        scroll->children = {btn};

        auto p = runWidgetPipeline(180, 160, {scroll}, fm);

        // no-overflow: scroll bg (1) + button (1) = 2 prims
        uint32_t scrollOwnPrims = countScrollAreaOwnPrims(180, 160, 100, 100);
        expect(p.primitiveCount >= scrollOwnPrims + 1)
            << "no-overflow scroll + btn: expected >= " << (scrollOwnPrims + 1)
            << ", got " << p.primitiveCount;

        verifyAllPrimsReachable(p, "scroll-no-overflow");

        // Button should be reachable
        float cx = btn->x + btn->w / 2;
        float cy = btn->y + btn->h / 2;
        expect(gridHasPrimWithFill(p, cx, cy,
            (uint32_t)SDFType::RoundedBox, btn->bgColor))
            << "button reachable in no-overflow scroll";
    };

    //-------------------------------------------------------------------------
    // BUG TEST: original ygui-demo layout (two columns, mixed widgets)
    //-------------------------------------------------------------------------
    "Full ygui-demo layout → all prims reachable"_test = [] {
        auto fm = testFontManager();
        if (!fm) return;

        // Left column
        auto btn = std::make_shared<Button>();
        btn->x = 10; btn->y = 10; btn->w = 100; btn->h = 30;
        btn->label = "Click Me"; btn->bgColor = 0xFF2A2A3E;

        auto chk = std::make_shared<Checkbox>();
        chk->x = 10; chk->y = 50; chk->w = 150; chk->h = 24;
        chk->label = "Enable"; chk->flags |= WIDGET_CHECKED;

        auto sld = std::make_shared<Slider>();
        sld->x = 10; sld->y = 84; sld->w = 200; sld->h = 24;
        sld->value = 50; sld->minValue = 0; sld->maxValue = 100;

        // Right column
        auto dd = std::make_shared<Dropdown>();
        dd->x = 230; dd->y = 10; dd->w = 150; dd->h = 28;
        dd->options = {"Light", "Dark", "System"};
        dd->selectedIndex = 1;

        auto prog = std::make_shared<Progress>();
        prog->x = 230; prog->y = 50; prog->w = 150; prog->h = 20;
        prog->value = 75; prog->minValue = 0; prog->maxValue = 100;

        auto sep = std::make_shared<Separator>();
        sep->x = 10; sep->y = 120; sep->w = 370; sep->h = 10;

        // Wrap everything in a scroll area (like the demo does)
        auto scroll = std::make_shared<ScrollArea>();
        scroll->x = 0; scroll->y = 0; scroll->w = 400; scroll->h = 300;
        scroll->contentW = 400; scroll->contentH = 500;
        scroll->children = {btn, chk, sld, dd, prog, sep};

        auto p = runWidgetPipeline(400, 300, {scroll}, fm);

        uint32_t scrollOwnPrims = countScrollAreaOwnPrims(400, 300, 400, 500);
        // btn(1)+chk(2)+sld(3)+dd(2)+prog(2)+sep(1) = 11 child prims
        expect(p.primitiveCount >= scrollOwnPrims + 11)
            << "full demo in scroll: expected >= " << (scrollOwnPrims + 11)
            << ", got " << p.primitiveCount;

        verifyAllPrimsReachable(p, "ygui-demo-layout");

        // Spot-check: button reachable
        expect(gridHasPrimWithFill(p, btn->x + btn->w / 2, btn->y + btn->h / 2,
            (uint32_t)SDFType::RoundedBox, btn->bgColor))
            << "button reachable in demo layout";

        // Spot-check: dropdown reachable
        float ddCx = dd->x + dd->w / 2;
        float ddCy = dd->y + dd->h / 2;
        auto hits = shaderGridLookup(p, ddCx, ddCy);
        expect(!hits.empty()) << "dropdown reachable in demo layout";
    };

    //-------------------------------------------------------------------------
    // DIRECT: ScrollArea::renderAll produces correct prim count in buffer
    //-------------------------------------------------------------------------
    "DIRECT: ScrollArea::renderAll produces scroll + child prims"_test = [] {
        auto buf = *YDrawBuffer::create();
        RenderContext ctx(buf.get());

        auto btn = std::make_shared<Button>();
        btn->x = 10; btn->y = 10; btn->w = 100; btn->h = 28;
        btn->label = "Click"; btn->bgColor = 0xFF2A2A3E;

        auto chk = std::make_shared<Checkbox>();
        chk->x = 10; chk->y = 50; chk->w = 150; chk->h = 24;
        chk->label = "Enable"; chk->flags |= WIDGET_CHECKED;

        auto scroll = std::make_shared<ScrollArea>();
        scroll->x = 0; scroll->y = 0; scroll->w = 300; scroll->h = 200;
        scroll->contentW = 300; scroll->contentH = 400;
        scroll->children = {btn, chk};

        scroll->renderAll(ctx);

        uint32_t scrollOwnPrims = countScrollAreaOwnPrims(300, 200, 300, 400);
        // btn = 1, chk (checked) = 2, scroll own
        uint32_t expected = scrollOwnPrims + 1 + 2;
        expect(buf->primCount() == expected)
            << "ScrollArea::renderAll: scroll(" << scrollOwnPrims
            << ") + btn(1) + chk(2) = " << expected
            << ", got " << buf->primCount();

        // Verify prims actually exist — iterate buffer
        int roundedBoxCount = 0;
        buf->forEachPrim([&](uint32_t, const float* data, uint32_t) {
            uint32_t type;
            std::memcpy(&type, &data[0], sizeof(uint32_t));
            if (type == (uint32_t)SDFType::RoundedBox) roundedBoxCount++;
        });
        expect(roundedBoxCount == (int)expected)
            << "all prims should be RoundedBox, got " << roundedBoxCount;
    };

    //-------------------------------------------------------------------------
    // DIRECT: ScrollArea offset stack doesn't corrupt child positions
    //-------------------------------------------------------------------------
    "DIRECT: ScrollArea pushOffset(0,0) doesn't alter child prim positions"_test = [] {
        // Render button FLAT
        auto buf1 = *YDrawBuffer::create();
        RenderContext ctx1(buf1.get());

        Button btn1;
        btn1.x = 10; btn1.y = 10; btn1.w = 100; btn1.h = 28;
        btn1.bgColor = 0xFFAA5500;
        btn1.label = "X";
        btn1.render(ctx1);

        // Render button inside ScrollArea (scrollX=0, scrollY=0)
        auto buf2 = *YDrawBuffer::create();
        RenderContext ctx2(buf2.get());

        auto btn2 = std::make_shared<Button>();
        btn2->x = 10; btn2->y = 10; btn2->w = 100; btn2->h = 28;
        btn2->bgColor = 0xFFAA5500;
        btn2->label = "X";

        auto scroll = std::make_shared<ScrollArea>();
        scroll->x = 0; scroll->y = 0; scroll->w = 300; scroll->h = 200;
        scroll->contentW = 300; scroll->contentH = 400;
        scroll->scrollX = 0; scroll->scrollY = 0;
        scroll->children = {btn2};
        scroll->renderAll(ctx2);

        // Both buffers should have the button at the same position
        // The scroll area adds its own prims first, then the button
        float flatCx = 0, flatCy = 0;
        buf1->forEachPrim([&](uint32_t, const float* data, uint32_t) {
            flatCx = data[2]; flatCy = data[3]; // cx, cy
        });

        // Find the button prim in scroll buffer (it has btn->bgColor)
        float scrollCx = 0, scrollCy = 0;
        bool found = false;
        buf2->forEachPrim([&](uint32_t, const float* data, uint32_t) {
            uint32_t fill;
            std::memcpy(&fill, &data[10], sizeof(uint32_t));
            if (fill == 0xFFAA5500 && !found) {
                scrollCx = data[2]; scrollCy = data[3];
                found = true;
            }
        });

        expect(found) << "button prim found in scroll buffer";
        expect(std::abs(flatCx - scrollCx) < 0.01f)
            << "button cx same: flat=" << flatCx << " scroll=" << scrollCx;
        expect(std::abs(flatCy - scrollCy) < 0.01f)
            << "button cy same: flat=" << flatCy << " scroll=" << scrollCy;
    };
};
