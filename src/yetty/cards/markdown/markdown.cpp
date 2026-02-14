#include "markdown.h"
#include <yetty/ydraw-builder.h>
#include "../hdraw/hdraw.h"  // For SDFPrimitive
#include <yetty/msdf-glyph-data.h>
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>

namespace yetty::card {

constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;

//=============================================================================
// MarkdownImpl - full implementation with builder and GPU buffers
//=============================================================================

class MarkdownImpl : public Markdown {
public:
    MarkdownImpl(const YettyContext& ctx,
                 int32_t x, int32_t y,
                 uint32_t widthCells, uint32_t heightCells,
                 const std::string& args, const std::string& payload)
        : Markdown(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;
        auto builderRes = YDrawBuilder::create(ctx.fontManager, ctx.globalAllocator);
        if (builderRes) {
            _builder = *builderRes;
            _builder->addFlags(YDrawBuilder::FLAG_UNIFORM_SCALE);
        } else {
            yerror("MarkdownImpl: failed to create builder");
        }
    }

    ~MarkdownImpl() override { dispose(); }

    //=========================================================================
    // Card lifecycle
    //=========================================================================

    bool needsTexture() const override { return false; }

    uint32_t metadataSlotIndex() const override {
        return _metaHandle.offset / 64;
    }

    void setCellSize(float cellWidth, float cellHeight) override {
        if (cellWidth == _cellWidth && cellHeight == _cellHeight) return;
        _cellWidth = cellWidth;
        _cellHeight = cellHeight;
        _needsLayout = true;
    }

    void renderToStaging(float time) override {
        (void)time;
        if (!_needsLayout) return;
        _needsLayout = false;

        if (!_userFontSize) {
            _fontSize = static_cast<float>(_cellHeight);
        }

        float sceneW = static_cast<float>(_widthCells * _cellWidth);
        float sceneH = static_cast<float>(_heightCells * _cellHeight);
        _builder->setSceneBounds(0, 0, sceneW, sceneH);

        _builder->clear();
        generatePrimitives();

        _dirty = true;
        _metadataDirty = true;
    }

    Result<bool> onEvent(const base::Event& event) override {
        if (event.type == base::Event::Type::SetFocus) {
            if (event.setFocus.objectId == id()) {
                _focused = true;
                return Ok(true);
            }
            _focused = false;
            return Ok(false);
        }

        if (event.type == base::Event::Type::CardScroll &&
            event.cardScroll.targetId == id()) {

            float sceneW = _builder->sceneMaxX() - _builder->sceneMinX();
            float sceneH = _builder->sceneMaxY() - _builder->sceneMinY();

            if (event.cardScroll.mods & GLFW_MOD_CONTROL) {
                // Ctrl+wheel = zoom
                float zoomFactor = std::exp(event.cardScroll.dy * 0.1f);
                _viewZoom = std::clamp(_viewZoom * zoomFactor, 0.1f, 50.0f);
                _metadataDirty = true;
                return Ok(true);
            } else if (event.cardScroll.mods & GLFW_MOD_SHIFT) {
                // Shift+wheel = horizontal scroll
                _viewPanX += event.cardScroll.dy * 0.05f * sceneW / _viewZoom;
                _metadataDirty = true;
                return Ok(true);
            } else {
                // Wheel = vertical scroll
                _viewPanY += event.cardScroll.dy * 0.05f * sceneH / _viewZoom;
                _metadataDirty = true;
                return Ok(true);
            }
        }

        return Ok(false);
    }

    Result<void> dispose() override {
        if (!weak_from_this().expired()) {
            auto loopResult = base::EventLoop::instance();
            if (loopResult) {
                (*loopResult)->deregisterListener(sharedAs<base::EventListener>());
            }
        }
        _derivedStorage = StorageHandle::invalid();
        _grid = nullptr;
        _gridSize = 0;
        _primStorage = StorageHandle::invalid();
        _primitives = nullptr;
        _primCount = 0;
        _primCapacity = 0;
        if (_metaHandle.isValid() && _cardMgr) {
            if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
                yerror("MarkdownImpl::dispose: deallocateMetadata failed: {}", error_msg(res));
            }
            _metaHandle = MetadataHandle::invalid();
        }
        return Ok();
    }

    void suspend() override {
        if (_primStorage.isValid() && _primCount > 0 && _builder) {
            auto& staging = _builder->primStagingMut();
            staging.resize(_primCount);
            std::memcpy(staging.data(), _primitives, _primCount * sizeof(SDFPrimitive));
        }
        _derivedStorage = StorageHandle::invalid();
        _grid = nullptr;
        _gridSize = 0;
        _primStorage = StorageHandle::invalid();
        _primitives = nullptr;
        _primCount = 0;
        _primCapacity = 0;
    }

    void declareBufferNeeds() override {
        if (!_builder) return;

        if (_primStorage.isValid() && _primCount > 0) {
            auto& staging = _builder->primStagingMut();
            if (staging.empty()) {
                staging.resize(_primCount);
                std::memcpy(staging.data(), _primitives, _primCount * sizeof(SDFPrimitive));
            } else {
                std::vector<SDFPrimitive> merged(_primCount);
                std::memcpy(merged.data(), _primitives, _primCount * sizeof(SDFPrimitive));
                merged.insert(merged.end(), staging.begin(), staging.end());
                staging = std::move(merged);
            }
        }
        _derivedStorage = StorageHandle::invalid();
        _grid = nullptr;
        _gridSize = 0;
        _primStorage = StorageHandle::invalid();
        _primitives = nullptr;
        _primCount = 0;
        _primCapacity = 0;

        const auto& primStaging = _builder->primStaging();
        const auto& glyphs = _builder->glyphs();

        if (primStaging.empty() && glyphs.empty()) {
            _builder->clearGridStaging();
            return;
        }

        if (!primStaging.empty()) {
            uint32_t primSize = static_cast<uint32_t>(primStaging.size()) * sizeof(SDFPrimitive);
            _cardMgr->bufferManager()->reserve(primSize);
        }

        if (_builder->gridStaging().empty()) {
            _builder->calculate();
        }

        uint32_t gridBytes = static_cast<uint32_t>(_builder->gridStaging().size()) * sizeof(uint32_t);
        uint32_t glyphBytes = static_cast<uint32_t>(glyphs.size() * sizeof(YDrawGlyph));
        uint32_t derivedSize = gridBytes + glyphBytes;

        if (derivedSize > 0) {
            _cardMgr->bufferManager()->reserve(derivedSize);
        }
    }

    Result<void> allocateBuffers() override {
        if (!_builder) return Ok();

        const auto& primStaging = _builder->primStaging();
        const auto& gridStaging = _builder->gridStaging();
        const auto& glyphs = _builder->glyphs();

        if (!primStaging.empty()) {
            uint32_t count = static_cast<uint32_t>(primStaging.size());
            uint32_t allocBytes = count * sizeof(SDFPrimitive);
            auto primResult = _cardMgr->bufferManager()->allocateBuffer(
                metadataSlotIndex(), "prims", allocBytes);
            if (!primResult) {
                return Err<void>("MarkdownImpl::allocateBuffers: prim alloc failed");
            }
            _primStorage = *primResult;
            _primitives = reinterpret_cast<SDFPrimitive*>(_primStorage.data);
            _primCapacity = count;
            _primCount = count;
            std::memcpy(_primitives, primStaging.data(), count * sizeof(SDFPrimitive));
            _builder->primStagingMut().clear();
            _builder->primStagingMut().shrink_to_fit();
            _cardMgr->bufferManager()->markBufferDirty(_primStorage);
        }

        uint32_t gridBytes = static_cast<uint32_t>(gridStaging.size()) * sizeof(uint32_t);
        uint32_t glyphBytes = static_cast<uint32_t>(glyphs.size() * sizeof(YDrawGlyph));
        uint32_t derivedSize = gridBytes + glyphBytes;

        if (derivedSize > 0) {
            auto storageResult = _cardMgr->bufferManager()->allocateBuffer(
                metadataSlotIndex(), "derived", derivedSize);
            if (!storageResult) {
                return Err<void>("MarkdownImpl::allocateBuffers: derived alloc failed");
            }
            _derivedStorage = *storageResult;

            uint8_t* base = _derivedStorage.data;
            uint32_t offset = 0;

            _grid = reinterpret_cast<uint32_t*>(base + offset);
            _gridSize = static_cast<uint32_t>(gridStaging.size());
            _gridOffset = (_derivedStorage.offset + offset) / sizeof(float);
            if (!gridStaging.empty()) {
                std::memcpy(base + offset, gridStaging.data(), gridBytes);
            }
            offset += gridBytes;

            _glyphOffset = (_derivedStorage.offset + offset) / sizeof(float);
            if (!glyphs.empty()) {
                std::memcpy(base + offset, glyphs.data(), glyphBytes);
            }
            offset += glyphBytes;

            _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
        }

        _primitiveOffset = _primStorage.isValid() ? _primStorage.offset / sizeof(float) : 0;
        _gridWidth = _builder->gridWidth();
        _gridHeight = _builder->gridHeight();
        _metadataDirty = true;
        _dirty = false;

        return Ok();
    }

    Result<void> allocateTextures() override { return Ok(); }
    Result<void> writeTextures() override { return Ok(); }

    Result<void> finalize() override {
        if (!_builder) return Ok();

        if (_dirty) {
            if (auto res = rebuildAndUpload(); !res) return res;
            _dirty = false;
        }

        if (_metadataDirty) {
            if (auto res = uploadMetadata(); !res) return res;
            _metadataDirty = false;
        }

        return Ok();
    }

    //=========================================================================
    // Init
    //=========================================================================

    Result<void> init() {
        auto metaResult = _cardMgr->allocateMetadata(sizeof(YDrawMetadata));
        if (!metaResult) {
            return Err<void>("MarkdownImpl::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        parseArgs(_argsStr);

        std::string content;

        if (_inputArg == "-") {
            content = _payloadStr;
        } else if (!_inputArg.empty()) {
            std::ifstream file(_inputArg);
            if (file) {
                std::stringstream buffer;
                buffer << file.rdbuf();
                content = buffer.str();
            } else {
                return Err<void>("MarkdownImpl::init: failed to open: " + _inputArg);
            }
        }

        parseMarkdown(content);

        // Primitives are generated in setCellSize() once we know the cell dimensions.
        // setCellSize() is called by GPUScreen right after card creation.

        // Register for scroll/focus events
        auto loopResult = base::EventLoop::instance();
        if (loopResult) {
            auto self = sharedAs<base::EventListener>();
            (*loopResult)->registerListener(base::Event::Type::SetFocus, self, 1000);
            (*loopResult)->registerListener(base::Event::Type::CardScroll, self, 1000);
        }

        _dirty = true;
        _metadataDirty = true;
        return Ok();
    }

private:
    //=========================================================================
    // GPU rebuild
    //=========================================================================

    Result<void> rebuildAndUpload() {
        _builder->computeSceneBoundsFromPrims(_primitives, _primCount);

        _builder->computeGridDims(_primitives, _primCount);
        _builder->buildGridFromPrims(_primitives, _primCount);

        const auto& gridData = _builder->gridStaging();
        uint32_t gridBytes = static_cast<uint32_t>(gridData.size()) * sizeof(uint32_t);
        uint32_t glyphBytes = static_cast<uint32_t>(
            _builder->glyphs().size() * sizeof(YDrawGlyph));
        uint32_t derivedTotalSize = gridBytes + glyphBytes;

        if (derivedTotalSize > 0) {
            if (!_derivedStorage.isValid() || derivedTotalSize > _derivedStorage.size) {
                auto storageResult = _cardMgr->bufferManager()->allocateBuffer(
                    metadataSlotIndex(), "derived", derivedTotalSize);
                if (!storageResult) {
                    return Err<void>("MarkdownImpl::rebuild: derived alloc failed");
                }
                _derivedStorage = *storageResult;
            }
        }

        if (_derivedStorage.isValid() && derivedTotalSize > 0) {
            uint8_t* base = _derivedStorage.data;
            std::memset(base, 0, _derivedStorage.size);
            uint32_t offset = 0;

            _grid = reinterpret_cast<uint32_t*>(base + offset);
            _gridSize = static_cast<uint32_t>(gridData.size());
            _gridWidth = _builder->gridWidth();
            _gridHeight = _builder->gridHeight();
            _gridOffset = (_derivedStorage.offset + offset) / sizeof(float);
            std::memcpy(base + offset, gridData.data(), gridBytes);
            offset += gridBytes;

            _glyphOffset = (_derivedStorage.offset + offset) / sizeof(float);
            const auto& glyphs = _builder->glyphs();
            if (!glyphs.empty()) {
                std::memcpy(base + offset, glyphs.data(), glyphBytes);
            }

            _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
        }

        _primitiveOffset = _primStorage.isValid() ? _primStorage.offset / sizeof(float) : 0;
        if (_primStorage.isValid()) {
            _cardMgr->bufferManager()->markBufferDirty(_primStorage);
        }
        _metadataDirty = true;
        return Ok();
    }

    Result<void> uploadMetadata() {
        if (!_metaHandle.isValid()) {
            return Err<void>("MarkdownImpl::uploadMetadata: invalid handle");
        }

        // Pack zoom as f16
        uint32_t zoomBits;
        {
            uint32_t f32bits;
            std::memcpy(&f32bits, &_viewZoom, sizeof(float));
            uint32_t sign = (f32bits >> 16) & 0x8000;
            int32_t  exp  = ((f32bits >> 23) & 0xFF) - 127 + 15;
            uint32_t mant = (f32bits >> 13) & 0x3FF;
            if (exp <= 0) { exp = 0; mant = 0; }
            else if (exp >= 31) { exp = 31; mant = 0; }
            zoomBits = sign | (static_cast<uint32_t>(exp) << 10) | mant;
        }

        // Pack pan as normalized i16
        float contentW = _builder->sceneMaxX() - _builder->sceneMinX();
        float contentH = _builder->sceneMaxY() - _builder->sceneMinY();
        int16_t panXi16 = static_cast<int16_t>(std::clamp(
            _viewPanX / std::max(contentW, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));
        int16_t panYi16 = static_cast<int16_t>(std::clamp(
            _viewPanY / std::max(contentH, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));

        float sceneMinX = _builder->sceneMinX();
        float sceneMinY = _builder->sceneMinY();
        float sceneMaxX = _builder->sceneMaxX();
        float sceneMaxY = _builder->sceneMaxY();
        float cellSize = _builder->cellSize();

        YDrawMetadata meta = {};
        meta.primitiveOffset = _primitiveOffset;
        meta.primitiveCount = _primCount;
        meta.gridOffset = _gridOffset;
        meta.gridWidth = _gridWidth;
        meta.gridHeight = _gridHeight;
        std::memcpy(&meta.cellSize, &cellSize, sizeof(float));
        meta.glyphOffset = _glyphOffset;
        meta.glyphCount = static_cast<uint32_t>(_builder->glyphs().size());
        std::memcpy(&meta.sceneMinX, &sceneMinX, sizeof(float));
        std::memcpy(&meta.sceneMinY, &sceneMinY, sizeof(float));
        std::memcpy(&meta.sceneMaxX, &sceneMaxX, sizeof(float));
        std::memcpy(&meta.sceneMaxY, &sceneMaxY, sizeof(float));
        meta.widthCells  = (_widthCells & 0xFFFF) | (static_cast<uint32_t>(static_cast<uint16_t>(panXi16)) << 16);
        meta.heightCells = (_heightCells & 0xFFFF) | (static_cast<uint32_t>(static_cast<uint16_t>(panYi16)) << 16);
        meta.flags = (_builder->flags() & 0xFFFF) | (zoomBits << 16);
        meta.bgColor = _builder->bgColor();

        if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
            return Err<void>("MarkdownImpl::uploadMetadata: write failed");
        }
        return Ok();
    }

    //=========================================================================
    // Args parsing
    //=========================================================================

    void parseArgs(const std::string& args) {
        if (args.empty()) return;

        std::istringstream iss(args);
        std::string token;

        while (iss >> token) {
            if (token == "-i" || token == "--input") {
                std::string val;
                if (iss >> val) {
                    _inputArg = val;
                }
            } else if (token.substr(0, 12) == "--font-size=") {
                _fontSize = std::stof(token.substr(12));
                _userFontSize = true;
            } else if (token.substr(0, 11) == "--bg-color=") {
                std::string hex = token.substr(11);
                if (hex[0] == '#') hex = hex.substr(1);
                uint32_t rgb = std::stoul(hex, nullptr, 16);
                if (hex.size() <= 6) {
                    _builder->setBgColor(0xFF000000 | ((rgb & 0xFF) << 16) |
                                         (rgb & 0xFF00) | ((rgb >> 16) & 0xFF));
                }
            } else if (token.substr(0, 15) == "--line-spacing=") {
                _lineSpacing = std::stof(token.substr(15));
            }
        }
    }

    //=========================================================================
    // Markdown Parser
    //=========================================================================

    void parseMarkdown(const std::string& content) {
        _parsedLines.clear();

        std::istringstream stream(content);
        std::string line;

        while (std::getline(stream, line)) {
            ParsedLine textLine;

            // Check for headers
            int headerLevel = 0;
            size_t i = 0;
            while (i < line.size() && line[i] == '#') {
                headerLevel++;
                i++;
            }
            if (headerLevel > 0 && i < line.size() && line[i] == ' ') {
                line = line.substr(i + 1);
                textLine.scale = 1.0f + (0.15f * (7 - std::min(headerLevel, 6)));
            } else {
                headerLevel = 0;
            }

            // Check for bullet list
            bool isBullet = false;
            if (line.size() >= 2 && (line[0] == '-' || line[0] == '*') && line[1] == ' ') {
                isBullet = true;
                textLine.indent = 20.0f;
                line = line.substr(2);
            }

            // Parse inline styles
            size_t pos = 0;
            while (pos < line.size()) {
                ParsedSpan span;

                if (line[pos] == '`') {
                    size_t end = line.find('`', pos + 1);
                    if (end != std::string::npos) {
                        span.text = line.substr(pos + 1, end - pos - 1);
                        span.isCode = true;
                        span.style = ParsedSpan::Regular;
                        textLine.spans.push_back(span);
                        pos = end + 1;
                        continue;
                    }
                }

                if (pos + 2 < line.size() && line.substr(pos, 3) == "***") {
                    size_t end = line.find("***", pos + 3);
                    if (end != std::string::npos) {
                        span.text = line.substr(pos + 3, end - pos - 3);
                        span.style = ParsedSpan::BoldItalic;
                        textLine.spans.push_back(span);
                        pos = end + 3;
                        continue;
                    }
                }

                if (pos + 1 < line.size() && line.substr(pos, 2) == "**") {
                    size_t end = line.find("**", pos + 2);
                    if (end != std::string::npos) {
                        span.text = line.substr(pos + 2, end - pos - 2);
                        span.style = ParsedSpan::Bold;
                        textLine.spans.push_back(span);
                        pos = end + 2;
                        continue;
                    }
                }

                if (line[pos] == '*') {
                    size_t end = line.find('*', pos + 1);
                    if (end != std::string::npos) {
                        span.text = line.substr(pos + 1, end - pos - 1);
                        span.style = ParsedSpan::Italic;
                        textLine.spans.push_back(span);
                        pos = end + 1;
                        continue;
                    }
                }

                size_t next = line.find_first_of("*`", pos);
                if (next == std::string::npos) next = line.size();
                if (next > pos) {
                    span.text = line.substr(pos, next - pos);
                    span.style = (headerLevel > 0) ? ParsedSpan::Bold : ParsedSpan::Regular;
                    span.headerLevel = static_cast<uint8_t>(headerLevel);
                    textLine.spans.push_back(span);
                    pos = next;
                } else {
                    span.text = std::string(1, line[pos]);
                    span.style = ParsedSpan::Regular;
                    textLine.spans.push_back(span);
                    pos++;
                }
            }

            if (isBullet && !textLine.spans.empty()) {
                ParsedSpan bullet;
                bullet.text = "\xE2\x80\xA2 ";  // Unicode bullet
                bullet.style = ParsedSpan::Regular;
                bullet.isBullet = true;
                textLine.spans.insert(textLine.spans.begin(), bullet);
            }

            if (textLine.spans.empty()) {
                ParsedSpan empty;
                empty.text = "";
                empty.style = ParsedSpan::Regular;
                textLine.spans.push_back(empty);
            }

            _parsedLines.push_back(textLine);
        }
    }

    //=========================================================================
    // Generate SDF primitives from parsed markdown
    //=========================================================================

    void generatePrimitives() {
        float cursorY = 2.0f;
        float lineHeight = _fontSize * _lineSpacing;

        for (const auto& line : _parsedLines) {
            float cursorX = 2.0f + line.indent;
            float scale = line.scale;
            float scaledSize = _fontSize * scale;
            float scaledLineHeight = lineHeight * scale;

            for (const auto& span : line.spans) {
                if (span.text.empty()) continue;

                uint32_t color;
                if (span.isCode) {
                    color = _codeColor;
                } else if (span.headerLevel > 0) {
                    color = _headerColor;
                } else if (span.style == ParsedSpan::Bold ||
                           span.style == ParsedSpan::BoldItalic) {
                    color = _boldColor;
                } else {
                    color = _textColor;
                }

                if (span.isCode) {
                    float textWidth = span.text.size() * scaledSize * 0.6f;
                    _builder->addBox(cursorX + textWidth * 0.5f,
                                     cursorY + scaledSize * 0.4f,
                                     textWidth * 0.5f + 1.0f,
                                     scaledSize * 0.5f + 0.5f,
                                     _codeBgColor, 0, 0, 1.0f, 0);
                }

                _builder->addText(cursorX, cursorY, span.text, scaledSize, color, 0);

                cursorX += span.text.size() * scaledSize * 0.6f;
            }

            cursorY += scaledLineHeight;
        }
    }

    //=========================================================================
    // Members
    //=========================================================================

    YDrawBuilder::Ptr _builder;
    std::string _argsStr;
    std::string _payloadStr;
    std::string _inputArg;  // -i/--input: "-" = payload is content, else file path
    std::vector<ParsedLine> _parsedLines;

    // GPU state
    StorageHandle _primStorage = StorageHandle::invalid();
    SDFPrimitive* _primitives = nullptr;
    uint32_t _primCount = 0;
    uint32_t _primCapacity = 0;
    StorageHandle _derivedStorage = StorageHandle::invalid();
    uint32_t* _grid = nullptr;
    uint32_t _gridSize = 0;
    uint32_t _primitiveOffset = 0;
    uint32_t _gridOffset = 0;
    uint32_t _glyphOffset = 0;
    uint32_t _gridWidth = 0;
    uint32_t _gridHeight = 0;
    bool _dirty = true;
    bool _metadataDirty = true;

    // Rendering parameters
    uint32_t _cellWidth = 0;
    uint32_t _cellHeight = 0;
    bool _needsLayout = true;
    bool _userFontSize = false;
    bool _focused = false;
    float _fontSize = 14.0f;
    float _lineSpacing = 1.4f;
    float _viewZoom = 1.0f;
    float _viewPanX = 0.0f;
    float _viewPanY = 0.0f;
    uint32_t _textColor = 0xFFE6E6E6;
    uint32_t _boldColor = 0xFFFFFFFF;
    uint32_t _codeColor = 0xFF66CC99;
    uint32_t _headerColor = 0xFFFFFFFF;
    uint32_t _codeBgColor = 0xFF3D3D3D;
};

//=============================================================================
// Factory
//=============================================================================

Result<CardPtr> Markdown::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("Markdown::create: pos=({},{}) size={}x{} payload_len={}",
          x, y, widthCells, heightCells, payload.size());

    if (!ctx.cardManager) {
        yerror("Markdown::create: null CardBufferManager!");
        return Err<CardPtr>("Markdown::create: null CardBufferManager");
    }

    auto card = std::make_shared<MarkdownImpl>(
        ctx, x, y, widthCells, heightCells, args, payload);

    if (auto res = card->init(); !res) {
        yerror("Markdown::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("Markdown::create: init failed");
    }

    yinfo("Markdown::create: SUCCESS");
    return Ok<CardPtr>(card);
}

} // namespace yetty::card
