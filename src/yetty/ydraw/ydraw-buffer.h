#pragma once

#include "ydraw-prim-writer.gen.h"
#include <yetty/base/object.h>
#include <yetty/base/factory.h>
#include <yetty/result.hpp>
#include <cstdint>
#include <cstring>
#include <functional>
#include <memory>
#include <string>
#include <map>
#include <vector>

namespace yetty {

//=============================================================================
// TextSpanData — high-level text span stored in the buffer
//=============================================================================
struct TextSpanData {
    float x, y;
    std::string text;
    float fontSize;
    uint32_t color;
    uint32_t layer;
    int fontId;
    float rotation;  // 0 = horizontal
};

//=============================================================================
// YDrawBuffer — pure data container for primitives, text, fonts, scene metadata
//=============================================================================
class YDrawBuffer : public base::Object,
                    public base::ObjectFactory<YDrawBuffer> {
public:
    using Ptr = std::shared_ptr<YDrawBuffer>;

    static Result<Ptr> createImpl();

    ~YDrawBuffer() override = default;
    const char* typeName() const override { return "YDrawBuffer"; }

    static constexpr uint32_t AUTO_ID = 0xFFFFFFFF;
    static constexpr uint32_t AUTO_ID_BASE = 0x80000000;

    // Rendering flags (mirrored from YDrawBuilder for buffer-only usage)
    static constexpr uint32_t FLAG_SHOW_BOUNDS = 1;
    static constexpr uint32_t FLAG_SHOW_GRID = 2;
    static constexpr uint32_t FLAG_SHOW_EVAL_COUNT = 4;
    static constexpr uint32_t FLAG_HAS_3D = 8;
    static constexpr uint32_t FLAG_UNIFORM_SCALE = 16;
    static constexpr uint32_t FLAG_CUSTOM_ATLAS = 32;

    // --- Generated per-type add/update methods ---
#include "ydraw-buffer.gen.inc"

    // --- Generic operations ---
    Result<void> remove(uint32_t id);
    uint32_t primCount() const { return static_cast<uint32_t>(_prims.size()); }
    bool empty() const { return _prims.empty() && _textSpans.empty(); }
    void clear();

    // --- Iteration (primitives) ---

    // Iterate all primitives. Callback: fn(uint32_t id, const float* data, uint32_t wordCount)
    template<typename F>
    void forEachPrim(F&& fn) const {
        for (const auto& [id, pd] : _prims) {
            fn(id, pd.words.data(), static_cast<uint32_t>(pd.words.size()));
        }
    }

    // --- Font blob storage (raw data, no FreeType) ---

    int addFontBlob(const uint8_t* data, size_t size, const std::string& name);

    // --- Text span storage ---

    void addText(float x, float y, const std::string& text,
                 float fontSize, uint32_t color, uint32_t layer, int fontId);
    void addRotatedText(float x, float y, const std::string& text,
                        float fontSize, uint32_t color, float rotation, int fontId);

    // Iterate text spans. Callback: fn(const TextSpanData& span)
    template<typename F>
    void forEachTextSpan(F&& fn) const {
        for (const auto& span : _textSpans) {
            fn(span);
        }
    }

    uint32_t textSpanCount() const { return static_cast<uint32_t>(_textSpans.size()); }

    // --- Font iteration (for card-side MSDF registration) ---

    // Callback: fn(int fontId, const uint8_t* data, size_t size, const std::string& name)
    template<typename F>
    void forEachFont(F&& fn) const {
        for (const auto& fd : _fonts) {
            fn(fd.fontId, fd.ttfData.data(), fd.ttfData.size(), fd.name);
        }
    }

    // --- Scene metadata ---

    void setSceneBounds(float minX, float minY, float maxX, float maxY);
    void setBgColor(uint32_t color) { _bgColor = color; }
    uint32_t bgColor() const { return _bgColor; }
    float sceneMinX() const { return _sceneMinX; }
    float sceneMinY() const { return _sceneMinY; }
    float sceneMaxX() const { return _sceneMaxX; }
    float sceneMaxY() const { return _sceneMaxY; }
    bool hasSceneBounds() const { return _hasSceneBounds; }

    void setFlags(uint32_t f) { _flags = f; }
    void addFlags(uint32_t f) { _flags |= f; }
    uint32_t flags() const { return _flags; }

    // --- Output ---

    // Build contiguous compact data: [prim0_data][prim1_data]...
    // Returns total word count.
    uint32_t buildCompact(std::vector<float>& out) const;

    // Write GPU format: [offset_table][compact_data].
    // Fills wordOffsets for grid translation.
    // buf must have at least gpuBufferSize() bytes.
    void writeGPU(float* buf, uint32_t bufBytes,
                  std::vector<uint32_t>& wordOffsets) const;

    // Compute bytes needed for GPU buffer.
    uint32_t gpuBufferSize() const;

    // --- Serialization (wire format) ---

    // Full serialize — prims, fonts, text spans, scene metadata. Enters delta mode.
    std::vector<uint8_t> serialize();

    // Delta serialize — returns delta blob, applies to main, clears delta.
    std::vector<uint8_t> serializeDelta();

    // Whether in delta mode (after first serialize).
    bool isDeltaMode() const { return _deltaMode; }

    // Deserialize full buffer (reverse of serialize).
    Result<void> deserialize(const uint8_t* data, size_t size);

    // Update an existing primitive's word data.
    Result<void> updatePrim(uint32_t id, const float* data, uint32_t wordCount);

protected:
    YDrawBuffer() = default;

private:
    Result<uint32_t> addPrim(uint32_t id, const float* data, uint32_t wordCount);
    uint32_t nextAutoId();

    struct PrimData {
        std::vector<float> words;
    };
    std::map<uint32_t, PrimData> _prims;
    uint32_t _nextAutoId = AUTO_ID_BASE;

    // Delta mode
    bool _deltaMode = false;
    struct DeltaOp {
        enum Type : uint8_t { UPSERT, REMOVE };
        Type type;
        uint32_t id;
        std::vector<float> data; // empty for REMOVE
    };
    std::vector<DeltaOp> _delta;

    // Font blob storage (raw TTF data for serialization, no FreeType)
    struct FontBlob {
        int fontId;
        std::string name;
        std::vector<uint8_t> ttfData;
    };
    std::vector<FontBlob> _fonts;
    int _nextFontId = 0;

    // Text spans
    std::vector<TextSpanData> _textSpans;

    // Scene metadata
    uint32_t _bgColor = 0;
    uint32_t _flags = 0;
    float _sceneMinX = 0, _sceneMinY = 0, _sceneMaxX = 0, _sceneMaxY = 0;
    bool _hasSceneBounds = false;
};

} // namespace yetty
