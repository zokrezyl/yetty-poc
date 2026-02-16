#pragma once

#include <yetty/card.h>
#include <yetty/yetty-context.h>
#include <string>
#include <vector>

namespace yetty::card {

//=============================================================================
// ParsedSpan - a run of styled text from markdown parsing
//=============================================================================
struct ParsedSpan {
    std::string text;
    enum Style { Regular, Bold, Italic, BoldItalic } style = Regular;
    uint8_t headerLevel = 0;  // 0=normal, 1-6=header
    bool isCode = false;
    bool isBullet = false;
};

//=============================================================================
// ParsedLine - a line of text with styled spans from markdown parsing
//=============================================================================
struct ParsedLine {
    std::vector<ParsedSpan> spans;
    float indent = 0.0f;
    float scale = 1.0f;  // For headers
};

//=============================================================================
// Markdown - SDF card that renders markdown content via YDrawBuilder
//
// Parses markdown text and generates addText() / addBox() calls for
// headers, bold, italic, code, bullet lists.
// Implementation (GPU buffers, parsing) is in MarkdownImpl.
//=============================================================================
class Markdown : public Card {
public:
    using Ptr = std::shared_ptr<Markdown>;

    static constexpr uint32_t SHADER_GLYPH = 0x100003;

    static Result<CardPtr> create(
        const YettyContext& ctx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    ~Markdown() override = default;
    const char* typeName() const override { return "markdown"; }
    bool needsBuffer() const override { return true; }

    // Card accessors
    uint32_t metadataOffset() const override { return _metaHandle.offset; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }
    uint32_t shaderGlyph() const override { return _shaderGlyph; }
    int32_t x() const override { return _x; }
    int32_t y() const override { return _y; }
    void setPosition(int32_t x, int32_t y) override { _x = x; _y = y; }
    uint32_t widthCells() const override { return _widthCells; }
    uint32_t heightCells() const override { return _heightCells; }
    const std::string& name() const override { return _name; }
    void setName(const std::string& n) override { _name = n; }
    bool hasName() const override { return !_name.empty(); }
    void setScreenOrigin(float sx, float sy) override { _screenOriginX = sx; _screenOriginY = sy; }

protected:
    Markdown(CardManager::Ptr mgr, const GPUContext& gpu,
             int32_t x, int32_t y, uint32_t widthCells, uint32_t heightCells)
        : _cardMgr(std::move(mgr)), _gpu(gpu)
        , _x(x), _y(y), _widthCells(widthCells), _heightCells(heightCells) {}

    CardManager::Ptr _cardMgr;
    GPUContext _gpu;
    MetadataHandle _metaHandle = MetadataHandle::invalid();
    uint32_t _shaderGlyph = 0;
    int32_t _x, _y;
    uint32_t _widthCells, _heightCells;
    float _screenOriginX = 0.0f, _screenOriginY = 0.0f;
    std::string _name;
};

} // namespace yetty::card
